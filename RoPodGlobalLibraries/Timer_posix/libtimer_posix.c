
/*
  timer_posix.c

  Rene van de Molengraft, June, 2004
  revision history: June, 28th, 2004: start
                    October, 2nd, 2004: added rtc_ prefix to routine names
                    October, 23th, 2005: synchronized original timer to fusion-timer
                    October, 30th, 2005: added frequency check
                    October, 11th, 2007: changed to posix timer
                    June, 5th, 2009: Changed to posix timed wait by Arthur Ketels
                    April, 23rd, 2016: Changed Posix-Timer to MONOTONIC for implementation with Chrony by Wouter Kuijpers
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sched.h>
#include <math.h>
#include "timer.h"

#define TIMING_STATS

#define USE_MONOTONIC 1

static timer_stat_t stats_struct;

/* global variables */
static int first_time = 1;
static struct timespec tv0, tva;
static unsigned int statistics_postpone_samples = 0;

int rtc_usleep(int usecs)
{
	usleep(usecs);
	return 0;
}
int rtc_reset_time()
{
  clock_gettime(CLOCK_REALTIME,&tv0);

  return 0;
}

double rtc_get_time()
{
  double t;

  clock_gettime(CLOCK_REALTIME, &tva);

  t = (double) (tva.tv_sec-tv0.tv_sec)+((double) (tva.tv_nsec-tv0.tv_nsec))/NSEC_PER_SEC_F;

  return t;
}

int rtc_get_timer_stat(ptimer_stat_t pstat)
{
	memcpy(pstat,&stats_struct,sizeof(timer_stat_t));

    return 0;
}
void rtc_postpone_statistics_calculation(unsigned int nsamples)
{
	statistics_postpone_samples = nsamples;

	return;
}

int rtc_print_timer_statistics()
{
	unsigned long long nsamples = stats_struct.icnt - stats_struct.idelay;

	if( stats_struct.icnt <= stats_struct.idelay ){
		printf("No statistics available\n");
		return 0;
	}
	double avg_time = stats_struct.total_time/nsamples;

	if(stats_struct.idelay > 0){
		printf("The statistics calculations started %.1f [s] after initialization.\n",stats_struct.idelay*stats_struct.dt);
	}

	printf("Average execution time :\t%.3e [s] (%.1f%% of sample time)\n",avg_time,avg_time/stats_struct.dt*100.0);
	printf("Maximum execution time :\t%.3e [s] (%.1f%% of sample time)\n",stats_struct.max_time,stats_struct.max_time/stats_struct.dt*100.0);
	printf("Number of sample overruns :\t%d (out of %lld)\n",stats_struct.ioverrun,nsamples);


    return 0;
}

int rtc_save_timer_statistics(char* filename)
{
	int i;
	FILE* fp=NULL;
	unsigned long long nsamples = stats_struct.icnt - stats_struct.idelay;

	if( stats_struct.icnt <= stats_struct.idelay ){
		printf("No statistics available\n");
		return 0;
	}
	fp = fopen (filename, "w");

	if(fp == NULL){
		printf("rtc_save_timer_statistics could not open logfile to write statistics\n");
		return 0;
	}

	double avg_time = stats_struct.total_time/nsamples;

	fprintf(fp,"Delay time:\t%d\nSample time:\t%.3e\nHIST1_RES:\t%d\nHIST2_MAX:\t%d\nNSAMPLES:\t%lld\nAVG_EXE_TIME:\t%.3e\nMAX_EXE_TIME:\t%.3e\nNOVERRUNS:\t%d\n",
			stats_struct.idelay,stats_struct.dt,HIST1_RESOLUTION,HIST2_MAX_EXECUTION_TIME,nsamples,avg_time,stats_struct.max_time,stats_struct.ioverrun);

	for(i=0;i<(100/HIST1_RESOLUTION)*2;i++){
		fprintf(fp,"%d\t[%d - %d%%]\t%d\t(%.1f%%)\n",HIST1_RESOLUTION*(2*i+1)/2
										,HIST1_RESOLUTION*i,HIST1_RESOLUTION*(i+1),stats_struct.hist1[i]
				                        ,((double)stats_struct.hist1[i]*100.0)/nsamples);
	}

	for(i=0;i<HIST2_MAX_EXECUTION_TIME-1;i++){
		fprintf(fp,"%d\t[%d - %d%%]\t%d\t(%.1f%%)\n",(2*i+5)*100/2,(i+2)*100,(i+3)*100,stats_struct.hist2[i]
				                        ,((double)stats_struct.hist2[i]*100.0)/nsamples);
	}
	i = HIST2_MAX_EXECUTION_TIME-1;
	fprintf(fp,"# [>%d%%] %d (%.1f%%)\n",(i+2)*100,stats_struct.hist2[i]
			                        ,((double)stats_struct.hist2[i]*100.0)/nsamples);

	fprintf(fp,"# Average execution time :\t%.3e [s] (%.1f%% of sample time)\n",avg_time,avg_time/stats_struct.dt*100.0);
	fprintf(fp,"# Maximum execution time :\t%.3e [s] (%.1f%% of sample time)\n",stats_struct.max_time,stats_struct.max_time/stats_struct.dt*100.0);
	fprintf(fp,"# Number of sample overruns :\t%d (out of %lld)\n",stats_struct.ioverrun,nsamples);
	fclose(fp);
    return 0;
}

void add_to_stats(double diff)
{
	int idx;
	stats_struct.icnt++;

	if(stats_struct.icnt < stats_struct.idelay){
		/* start saving stats after idelay samples */
		return;
	}
	/* check for sample overrun */
    if(diff > stats_struct.dt){
    	stats_struct.ioverrun++;
    }
    /* check if it exceeds the maximum execution time */
    if(diff > stats_struct.max_time){
    	stats_struct.max_time = diff;
    }
    stats_struct.total_time += diff;
    /* add the timing to the histogram */
    if(diff > 2*stats_struct.dt){
    	/* put in hist2, resolution of histogram is 1 sample */
    	idx = round(diff/stats_struct.dt)-2; /* -2 because first two seconds are covered by hist1 */
    	if(idx>HIST2_MAX_EXECUTION_TIME-1){
    		idx = HIST2_MAX_EXECUTION_TIME-1;
    	}else if(idx <= 0){
    		idx = 0;
    	}
    	stats_struct.hist2[idx]++;
    }else{
    	/* put in hist1 */
    	idx = round((100.0/HIST1_RESOLUTION)*(diff/stats_struct.dt));
    	if(idx>((100/HIST1_RESOLUTION) * 2)-1){
    		idx = ((100/HIST1_RESOLUTION) * 2)-1;
    	}else if(idx <= 0){
    		idx = 0;
    	}
    	stats_struct.hist1[idx]++;
    }
}

int rtc_signalhandler(PTIMERCALLBACK the_user_apc)
{
    int stop;

    if(first_time){
    	/* reset global time */
    	rtc_reset_time();
    	first_time = 0;
    }
#ifdef TIMING_STATS
    double t0;
    /* get start time */
    t0 = rtc_get_time();
#endif

    /* call user apc */
    stop = the_user_apc(0,0);

#ifdef TIMING_STATS
    /* get stop time and diff */
    add_to_stats(rtc_get_time() - t0);
#endif
    return stop;
}


/* add ns to timespec */
void rtc_add_timespec(struct timespec *ts, signed long long addtime)
{
	signed long long sec, nsec;
	
	nsec = addtime % NSEC_PER_SEC;
	sec = (addtime - nsec) / NSEC_PER_SEC;
	ts->tv_sec += sec;
	ts->tv_nsec += nsec;
	if ( ts->tv_nsec > NSEC_PER_SEC ) 
	{ 
		nsec = ts->tv_nsec % NSEC_PER_SEC;
		ts->tv_sec += (ts->tv_nsec - nsec) / NSEC_PER_SEC;
		ts->tv_nsec = nsec;
	}	
}	

void rtc_rttask(double dt,PTIMERCALLBACK the_user_apc)
{
	struct timespec ts;
	int rc, stop=0;
	signed long long cycletime_ns;
	pthread_condattr_t      condattr;
	pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;
    /* This part of the code changes the attribute of the condition for the
     * pthread_cond_timedwait-function ahead. This now makes the Motion,
     * run on the CLOCK_MONOTONIC which is not affected by NTP-software. */
	if(USE_MONOTONIC){
	    pthread_condattr_init( &condattr);
	    pthread_condattr_setclock( &condattr, CLOCK_MONOTONIC);
	    pthread_cond_init( &cond, &condattr);
	}
	else
	{
	    pthread_condattr_init( &condattr);
	    pthread_condattr_setclock( &condattr, CLOCK_REALTIME);
	    pthread_cond_init( &cond, &condattr);
	}
	pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;
	
    /* Depending on the clock chosen, the current time of the clock needs
     * to be obtained from CLOCK_MONOTONIC or CLOCK_REALTIME. */
	if(USE_MONOTONIC)
	{
		clock_gettime(CLOCK_MONOTONIC, &ts); // clock_gettime is stored in <time.h>
	}
	else
	{
		clock_gettime(CLOCK_REALTIME, &ts);
	}
    /* Convert from timeval to timespec */
	cycletime_ns = dt * NSEC_PER_SEC; /* cycletime in ns */
	while(!stop)
	{	
		/* calculate next cycle start */
		rtc_add_timespec(&ts, cycletime_ns);
		/* wait to cycle start */
		rc = pthread_cond_timedwait(&cond, &mutex, &ts);
		stop = rtc_signalhandler(the_user_apc);
	}	 
}


int rtc_timer(double freq, PTIMERCALLBACK user_apc, int priority)
{
	struct sched_param schp;
	int prio_min, prio_max;
	double dt;

	/* check input timer frequency */
	if (freq <= 0.0) {
		printf("rtc_timer (posix) reports: invalid timer frequency : %f.\n",freq);
		return -1;
	}

	dt = 1.0 / freq;

	mlockall(MCL_CURRENT|MCL_FUTURE);

	/* set priority */
	prio_min = sched_get_priority_min(SCHED_FIFO);
	prio_max = sched_get_priority_max(SCHED_FIFO);

	if( ( priority >= prio_min ) & ( priority <= prio_max ) ) {
		memset(&schp, 0, sizeof(schp));
		schp.sched_priority = priority;

		if(sched_setscheduler(0, SCHED_FIFO, &schp) != 0) {
			printf("rtc_timer (posix) reports: error sched_setscheduler");
			usleep(2000000);
			return -1;
		}
		printf("rtc_timer (posix) reports: SCHED_FIFO priority set to %d.\n", priority);
	}

	/* ignore SIGALRM signals */
	if (priority > 0) {
		signal(SIGALRM, SIG_IGN);
		printf("rtc_timer (posix) reports: ignoring SIGALRM.\n");
	}

	printf("rtc_timer (posix) running at %f Hz.\n", freq);

	/* reset structure */
	memset((void*)&stats_struct,0,sizeof(stats_struct));
	/* set the possibly set postpone */
	stats_struct.idelay = statistics_postpone_samples;
	/* store the sample time */
	stats_struct.dt = dt;

	/* create RT task */
	rtc_rttask(dt,user_apc);

	printf("rtc_timer (posix) has been stopped.\n");

	return 0;
}
