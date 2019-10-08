#define REALTIME
#define MTHACK
/* 
 * Copyright 2012-2015 The MathWorks, Inc.
 *
 * File: rt_main.c
 *
 * Abstract:
 *
 *   A real-time main that runs generated Simulink Coder code under most 
 *   operating systems.  Based on the definition of MULTITASKING, a single-task 
 *   or multitask step function is employed.
 *
 *   This file is a useful starting point for creating a custom main when 
 *   targeting a custom floating point processor or integer micro-controller.
 *
 *   Alternatively for ERT targets, you can generate a sample ert_main.c file 
 *   with the generated code by selecting the "Generate an example main program"
 *   option.  In this case, ert_main.c is precisely customized to the
 *   model requirements.  
 *
 * Required Defines:
 *
 *   MODEL - Model name
 *   NUMST - Number of sample times
 *
 */

/*==================*
 * Required defines *
 *==================*/

#ifndef MODEL
# error Must specify a model name.  Define MODEL=name.
#else
/* create generic macros that work with any model */
# define EXPAND_CONCAT(name1,name2) name1 ## name2
# define CONCAT(name1,name2) EXPAND_CONCAT(name1,name2)
# define MODEL_INITIALIZE CONCAT(MODEL,_initialize)
# define MODEL_STEP       CONCAT(MODEL,_step)
# define MODEL_TERMINATE  CONCAT(MODEL,_terminate)
# define RT_MDL           CONCAT(MODEL,_M)
#endif

#ifndef NUMST
# error Must specify the number of sample times.  Define NUMST=number.
#endif

#if CLASSIC_INTERFACE == 1
# error "Classic call interface is not supported by rt_main.c."
#endif

#if ONESTEPFCN==0
#error Separate output and update functions are not supported by rt_main.c. \
You must update rt_main.c to suit your application needs, or select \
the 'Single output/update function' option.
#endif

#if TERMFCN==0
#error The terminate function is required by rt_main.c. \
You must update rt_main.c to suit your application needs, or select \
the 'Terminate function required' option.
#endif

#if MULTI_INSTANCE_CODE==1
#error The static version of rt_main.c does not support reusable \
code generation.  Either deselect ERT option 'Generate reusable code', \
select option 'Generate an example main program', or modify rt_main.c for \
your application needs.
#endif

#define QUOTE1(name) #name
#define QUOTE(name) QUOTE1(name)    /* need to expand name    */

#ifndef SAVEFILE
# define MATFILE2(file) #file ".mat"
# define MATFILE1(file) MATFILE2(file)
# define MATFILE MATFILE1(MODEL)
#else
# define MATFILE QUOTE(SAVEFILE)
#endif
        
#ifndef EXIT_FAILURE
#define EXIT_FAILURE  1
#endif
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS  0
#endif        

/*==========*
 * Includes *
 *==========*/

#include "rtwtypes.h"
#if !defined(INTEGER_CODE) || INTEGER_CODE == 0
# include <stdio.h>    /* optional for printf */
#else
#ifdef __cplusplus
extern "C" {
#endif
  extern int printf(const char *, ...); 
  extern int fflush(void *);
#ifdef __cplusplus
}
#endif
#endif
#include <string.h>  /* optional for strcmp */
#include <signal.h>
#include <time.h>
#include "rtmodel.h" /* optional for automated builds */

#include "/usr/local/include/timer_posix/timer.h"
#include "/usr/local/include/soem/ec.h"

#include "rt_logging.h"
#ifdef UseMMIDataLogging
#include "rt_logging_mmi.h"
#endif

#include "ext_work.h"

#ifdef MODEL_STEP_FCN_CONTROL_USED
#error The static version of rt_main.c does not support model step function prototype control.
#endif

/*========================* 
 * Setup for multitasking * 
 *========================*/

/* 
 * Let MT be synonym for MULTITASKING (to shorten command line for DOS) 
 */
#if defined(MT)
# if MT == 0
# undef MT
# else
# define MULTITASKING 1
# endif
#endif

#if defined(TID01EQ) && TID01EQ == 1
#define FIRST_TID 1
#else 
#define FIRST_TID 0
#endif

/*====================*
 * External functions *
 *====================*/

extern void MODEL_INITIALIZE(void);
extern void MODEL_TERMINATE(void);

#if !defined(MULTITASKING)
 extern void MODEL_STEP(void);       /* single-rate step function */
#else
 extern void MODEL_STEP(int_T tid);  /* multirate step function */
#endif

/*==================================*
 * Global data local to this module *
 *==================================*/
#ifndef MULTITASKING
static boolean_T OverrunFlags[1];    /* ISR overrun flags */
static boolean_T eventFlags[1];      /* necessary for overlapping preemption */
#else
static boolean_T OverrunFlags[NUMST];
static boolean_T eventFlags[NUMST]; 
#endif

/*=========================================================*
 * Multi threading heading to provide real multi-threading *
 *=========================================================*/
#ifdef MTHACK

#ifndef TIMING_STATS
#define TIMING_STATS
#endif

#include "../../mtt/mt_data.h"

typedef struct tag_mt_data {
	sem_t task_ready[NUMST];
	sem_t task_start[NUMST];
	sem_t protected_rate_transition[NUMST];
	pthread_t t1[NUMST];
	pthread_attr_t attr_t1[NUMST];
	int tid[NUMST];			/* task number */
	int cpu[NUMST];			/* cpu affinity */
	int prio[NUMST];		/* scheduling priority */
	double stepsize[NUMST];	/* stepsize */
	int trigger[NUMST];		/* 0 (not triggered) or 1 (triggered) */
	double prev_trigger[NUMST];
	int sync[NUMST];
	int busyreadbit[NUMST];
	int state[NUMST];
	int missed[NUMST];
	int nsteps[NUMST];
	double dt[NUMST];
	timer_stat_t stats_struct[NUMST];
} mt_data, *pmt_data;

#ifndef NO_EC
static int EC_Check_cntr = 0;
#define STARTUP_DELAY_EC_CHECK_CNT 2000
#endif

static mt_data mtd;

static int first_run = 1;

#include "../../mtt/mtt_functions.c"

#endif

/* Added to handle killall signal */
static int StopRTA=0;

static void signal_catch(int sig)
{
	StopRTA = 1;
}

/*===================*
 * Visible functions *
 *===================*/

#if !defined(MULTITASKING) /* single task */

/* Function: rtOneStep ========================================================
 *
 * Abstract:
 *   Perform one step of the model.  This function is modeled such that
 *   it could be called from an interrupt service routine (ISR) with minor
 *   modifications.
 */
static void rt_OneStep(void)
{
    /* Disable interrupts here */

    /***********************************************
     * Check and see if base step time is too fast *
     ***********************************************/
    if (OverrunFlags[0]++) {
        rtmSetErrorStatus(RT_MDL, "Overrun");
    }

    /*************************************************
     * Check and see if an error status has been set *
     * by an overrun or by the generated code.       *
     *************************************************/
    if (rtmGetErrorStatus(RT_MDL) != NULL) {
        return;
    }

    /* Save FPU context here (if necessary) */
    /* Reenable interrupts here */
    /* Set model inputs here */

    /**************
     * Step model *
     **************/
    MODEL_STEP();

    /* Get model outputs here */

    /**************************
     * Decrement overrun flag *
     **************************/
    OverrunFlags[0]--;

    rtExtModeCheckEndTrigger();

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Reenable interrupts here */

} /* end rtOneStep */

#else /* multitask */

/* Function: rtOneStep ========================================================
 *
 * Abstract:
 *   Perform one step of the model.  This function is modeled such that
 *   it could be called from an interrupt service routine (ISR) with minor
 *   modifications.
 *
 *   Note that error checking is only necessary when this routine is
 *   attached to an interrupt.
 *
 *   Also, you may wish to unroll any or all of for and while loops to
 *   improve the real-time performance of this function.
 */
static void rt_OneStep(void)
{
    int_T i;

    /* Disable interrupts here */

    /***********************************************
     * Check and see if base step time is too fast *
     ***********************************************/
    if (OverrunFlags[0]++) {
        rtmSetErrorStatus(RT_MDL, "Overrun");
    }

    /*************************************************
     * Check and see if an error status has been set *
     * by an overrun or by the generated code.       *
     *************************************************/
    if (rtmGetErrorStatus(RT_MDL) != NULL) {
        return;
    }

    /* Save FPU context here (if necessary) */
    /* Reenable interrupts here */
#ifdef MTHACK
	/*	check which of the busy asynchronous tasks are ready */

	/*	first check which tasks are ready */
	for (i=FIRST_TID+1; i<NUMST; i++) {
		mtd.sync[i]=0;
		if ( OverrunFlags[i]==1 ) {
			if ( !mt_data_check_task_ready(i, 1) ) {
				mtd.sync[i]=1;
			}
		}
	}
#else  
    /*************************************************
     * Update EventFlags and check subrate overrun   *
     *************************************************/
    for (i = FIRST_TID+1; i < NUMST; i++) {
        if (rtmStepTask(RT_MDL,i) && eventFlags[i]++) {
            OverrunFlags[0]--;
            OverrunFlags[i]++;
            /* Sampling too fast */
            rtmSetErrorStatus(RT_MDL, "Overrun");
            return;
        }
        if (++rtmTaskCounter(RT_MDL,i) == rtmCounterLimit(RT_MDL,i))
            rtmTaskCounter(RT_MDL, i) = 0;
    }

    /* Set model inputs associated with base rate here */
#endif
    
    /*******************************************
     * Step the model for the base sample time *
     *******************************************/
    MODEL_STEP(0);

    /* Get model outputs associated with base rate here */

    /************************************************************************
     * Model step complete for base sample time, now it is okay to          *
     * re-interrupt this ISR.                                               *
     ************************************************************************/
    OverrunFlags[0]--;

    /*********************************************************
     * Step the model for any other sample times (subrates) *
     *********************************************************/
#ifdef MTHACK
	/*	start periodic tasks */
	for (i=FIRST_TID+1; i<NUMST; i++) {
		if ( mtd.sync[i] ) {
			OverrunFlags[i]--; /* postponed such that base rate could see it */
		}

		if ( mtd.trigger[i]==TRIGGERED ) {
			/*printf("Task %d triggered.\n", i);*/

			if (OverrunFlags[i]++) {  /* Are we sampling too fast for sample time "i"? */
				/* rtmSetErrorStatus(RT_MDL, "Overrun"); */
				/*				sample time is too fast, skip task step */
				mtd.trigger[i]=NOT_TRIGGERED;
				mtd.missed[i]++;
				return;
			}
		}
	}

	for (i=FIRST_TID+1; i<NUMST; i++) {
		if ( ( mtd.trigger[i]==TRIGGERED ) || ( mtd.state[i]==IN_STATE ) ) {
			mtd.nsteps[i]++;
			mtd.trigger[i]=NOT_TRIGGERED;	

			if(mtd.stats_struct[i].icnt < mtd.stats_struct[i].idelay){
				/* set dt during first few samples */
				/* copy dt into statistic struct dt */
				mtd.stats_struct[i].dt = mtd.dt[i];
			}else{
				/* set dt to -1 to prevent continuous new dt calculations */
				mtd.dt[i] = -1;
			}
			/* start async task */
			mt_data_set_task_start(i);
		}
	}

#else     
    for (i = FIRST_TID+1; i < NUMST; i++) {
        /*************************************************************
         * If task "i" is running, don't run any lower priority task *
         *************************************************************/
        if (OverrunFlags[i]) return; 

        if (eventFlags[i]) {
            OverrunFlags[i]++;

            /* Set model inputs associated with subrate here */

            /******************************************
             * Step the model for sample time "i" *
             ******************************************/
            MODEL_STEP(i);

            /* Get model outputs associated with subrate here */
            
            /**********************************************
             * Indicate task complete for sample time "i" *
             **********************************************/
            OverrunFlags[i]--;
            eventFlags[i]--;
        }
    }
#endif
    
    rtExtModeCheckEndTrigger();
    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */

} /* end rtOneStep */

#endif /* MULTITASKING */

/* Function: rt_InitModel ====================================================
 * 
 * Abstract: 
 *   Initialized the model and the overrun flags
 *
 */
static void rt_InitModel(void)
{
#if defined(MULTITASKING)
    int i;
    for(i=0; i < NUMST; i++) {
        OverrunFlags[i] = 0;
        eventFlags[i] = 0;
    }
#else
    OverrunFlags[0] = 0;
    eventFlags[0] = 0;
#endif

    /************************
     * Initialize the model *
     ************************/
    MODEL_INITIALIZE();
}

/* Function: rt_TermModel ====================================================
 * 
 * Abstract:
 *   Terminates the model and prints the error status
 *
 */
static int_T rt_TermModel(void)
{
    MODEL_TERMINATE();
    
    {
        const char_T *errStatus = (const char_T *) (rtmGetErrorStatus(RT_MDL));
        int_T i = 0;
        
        if (errStatus != NULL && strcmp(errStatus, "Simulation finished")) {
            (void)printf("%s\n", errStatus);
#if defined(MULTITASKING)
            for (i = 0; i < NUMST; i++) {
                if (OverrunFlags[i]) {
                    (void)printf("ISR overrun - sampling rate too"
                                 "fast for sample time index %d.\n", i);
                }
            }
#else
           if (OverrunFlags[i]) { 
               (void)printf("ISR overrun - base sampling rate too fast.\n");
           }
#endif
            return(1);
        }
    }
    
    return(0);
}

#ifdef  REALTIME
static int user_apc( int a, int b )
{
	if (rtmGetErrorStatus(RT_MDL) == NULL &&
			!rtmGetStopRequested(RT_MDL) && !StopRTA) {
#ifndef NO_EC        
        if (first_run){
            first_run = 0;
            if (ec_scan() != 0){
                printf("setting to OP state failed\n");
                return(EXIT_FAILURE);
            }
        }
		ec_io(); /* perform I/O first to obtain measurement inputs */

        /* STARTUP_DELAY_EC_CHECK_CNT cnts after start, check states once again */
        if(EC_Check_cntr<=STARTUP_DELAY_EC_CHECK_CNT){
            if(EC_Check_cntr==STARTUP_DELAY_EC_CHECK_CNT){
                printf("Checking states once again...\n");
                int numslaves_NOT_op = 0;
                numslaves_NOT_op = ec_check_states();
                printf("Number of slaves that where NOT in operational mode: %d\n",numslaves_NOT_op);
                if(numslaves_NOT_op){
                    printf("Errors encountered, so check states again...\n");
                    usleep(500);ec_check_states();usleep(500);// Wait a little, request OP state, and wait a little
                    if(ec_check_states()){ // Here OP state should be reached, if not, restart motion!
                        printf("\nNot succeeded in geting all slaves to OP-state!\nRestarting...\n\n");
                        exit(1);
                    }
                }else{
                    printf("Good to go!\n");
                }
            }
            EC_Check_cntr++;
        }
#endif               
		rt_OneStep();        
	} else {
		return 1;
	}

	return 0;
}
#endif

/* Function: main =============================================================
 *
 * Abstract:
 *   Execute model on a generic target such as a workstation.
 */

int_T main(int_T argc, const char *argv[])
{
    int_T retval;
#ifdef MTHACK
	int i;
#endif    
    /* External mode */
    rtParseArgsForExtMode(argc, argv);
 
    /*******************************************
     * warn if the model will run indefinitely *
     *******************************************/
#if MAT_FILE==0 && EXT_MODE==0
    printf("warning: the simulation will run with no stop time; "
           "to change this behavior select the 'MAT-file logging' option\n");
    fflush(NULL);
#endif

    (void)printf("\n** starting the model **\n");

    /************************
     * Initialize the model *
     ************************/
    rt_InitModel();

    /* External mode */
    rtSetTFinalForExtMode(&rtmGetTFinal(RT_MDL));
    rtExtModeCheckInit(NUMST);
    rtExtModeWaitForStartPkt(rtmGetRTWExtModeInfo(RT_MDL),
                             NUMST,
                             (boolean_T *)&rtmGetStopRequested(RT_MDL));

    /* Initialization of Robocup EtherCAT and other real-time functionality */
    char netif[10];
    double StepSize, Freq;
    StepSize=rtmGetStepSize(RT_MDL);
    Freq=1/StepSize;
    
#ifdef MTHACK
	printf("FIRST_TID = %d\n", FIRST_TID);

	printf("Initializing tasks:\n");
	mt_data_init(0);
	for (i=FIRST_TID+1; i<NUMST; i++) {
		mt_data_init(i);
		mtd.tid[i]=i;

		mtd.cpu[i]=-1;
		mtd.prio[i]=-1;
		mtd.stepsize[i]=-1;
		mtd.trigger[i]=NOT_TRIGGERED;
		mtd.sync[i]=0;
		mtd.state[i]=IN_STATE;
		mtd.missed[i]=0;
		mtd.nsteps[i]=0;
		mtd.dt[i]=-1.0;
		mtd.prev_trigger[i] = -1.0;
#ifdef TIMING_STATS
		mtd.dt[i]=StepSize;
#endif
		mt_create_task(i);
		memset((void*)&mtd.stats_struct[i],0,sizeof(timer_stat_t));
		mtd.stats_struct[i].idelay = 2000; /* subrates will start after 2000 samples */
		OverrunFlags[i]++;
	}
	sleep(1);
#endif
    
    /*catch signals */
    signal(SIGTERM, signal_catch);
    signal(SIGINT, signal_catch);

#ifdef REALTIME
	printf("MTtarget v1.0, M.J.G. v.d. Molengraft, 2010\n");
#ifdef MTHACK
	printf("Multi-tasking support enabled.\n");
#endif
    
    /* postpone the statistics calculation of the timer to avoid strange number caused by initialization */
    rtc_postpone_statistics_calculation((unsigned int)Freq*5);
    
    printf("RTA has been started.\n");

    printf("Base rate = %f Hz\n", Freq);
#ifndef NO_EC
    strcpy(netif, "eth");
    *(netif+3)=(char) (48+NETIF_ID);
    *(netif+4)='\0';
    printf("Net interface = %s.\n", netif);

    if (ec_initial(netif) != 0){
    	printf("Initialization failed\n");
    	return(EXIT_FAILURE);
    }
    /* ec_scan is now done on the first run of user_apc() */
// 	if (ec_scan() != 0){
// 		printf("setting to OP state failed\n");
// 		return -1;
// 	}
#endif
    /* define priority of main task */
    int prio = 50;
#ifdef PRIO
    prio = PRIO;
#endif

	printf("Priority of main task set to : %d\n",prio);
	rtc_timer(Freq, user_apc, prio);
#else
	while (rtmGetErrorStatus(RT_MDL) == NULL &&
			!rtmGetStopRequested(RT_MDL) && !StopRTA) {
		rt_OneStep();
	}
#endif

#ifdef MTHACK
	for (i=FIRST_TID+1; i<NUMST; i++) {
		printf("Task %d has missed %d steps out of %d\n", i, mtd.missed[i], mtd.missed[i]+mtd.nsteps[i]);
	}
#endif

    /*******************************
     * Cleanup and exit (optional) *
     *******************************/

#ifdef UseMMIDataLogging
    rt_CleanUpForStateLogWithMMI(rtmGetRTWLogInfo(RT_MDL));
#endif
    rt_StopDataLogging(MATFILE,rtmGetRTWLogInfo(RT_MDL));

    rtExtModeShutdown(NUMST);

    retval = rt_TermModel();

#ifdef MTHACK
	/*  free space for change map */
	printf("Freeing tasks...");
	for (i=FIRST_TID+1; i<NUMST; i++) {
		mt_delete_task(i);
		OverrunFlags[i]=0;
	}
	printf("done.\n");
#endif
    
#ifndef NO_EC    
	ec_exit();
#endif    
	rtc_print_timer_statistics();

#ifndef SIMULATOR
	/* print logging to file */
	time_t rawtime;
	struct tm * timeinfo;
	char timestr [20],filename[150];
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (timestr,20,"%y%m%d_%H%M%S",timeinfo);
	snprintf(filename,150,"log/%s_timerlog_%s.log",argv[0],timestr);
	rtc_save_timer_statistics(filename);

#ifdef MTHACK
#ifdef TIMING_STATS
	for (i=FIRST_TID+1; i<NUMST; i++) {
		/* print sub task logging to file */
		if(mtd.stats_struct[i].total_time > 0)
		{
			snprintf(filename,150,"log/%s_timerlog_task%d_%s.log",argv[0],i,timestr);
			mtt_save_timer_statistics(filename,&mtd.stats_struct[i]);
		}
	}
#endif /* TIMINGSTATS */
#endif /* MTHACK */
#endif /* SIMULATOR */
    
    return retval;
}

/* [EOF] mtt_main.c */
