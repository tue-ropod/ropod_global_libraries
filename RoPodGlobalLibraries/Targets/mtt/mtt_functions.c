/*
 * mtt_functions.c
 *
 *  Created on: Feb 1, 2015
 *      Author: Koen Meessen
 */

#include "mtt_functions.h"

static int get_expiration_time(struct timespec* pts_exp, int us_timeout)
{
	/*    get absolute expiration time us_timeout us from now */

	time_t s, ds;
	unsigned int ns, dns;
	struct timeval tv;

	gettimeofday(&tv, NULL);

	/*    first, convert time of day to timespec format */
	ns=tv.tv_usec*1000;
	s=ns/1000000000;
	ns=ns-s*1000000000;

	pts_exp->tv_sec=tv.tv_sec+s;
	pts_exp->tv_nsec=ns;

	/*    add us_timeout us */
	ds=us_timeout/1000000;
	dns=(us_timeout-ds*1000000)*1000;
	pts_exp->tv_sec=pts_exp->tv_sec+ds;
	pts_exp->tv_nsec=pts_exp->tv_nsec+dns;

	return 0;
}

static int mt_data_init(int task_id)
{
	/*  semaphores are shared between threads (second argument) */

	/*  task_ready is true at start (third argument) */
	if ( TEMP_FAILURE_RETRY( sem_init(&(mtd.task_ready[task_id]), 0, 1) )==-1) {
		printf("mt_data_init reports: failed to initialize task_ready.\n");
		return -1;
	}

	/*  task_start is not yet given at start (third argument) */
	if ( TEMP_FAILURE_RETRY( sem_init(&(mtd.task_start[task_id]), 0, 0) )==-1) {
		printf("mt_data_init reports: failed to initialize task_start.\n");
		return -1;
	}

	/*  protected_rate_transition is true at start (third argument) */
	if ( TEMP_FAILURE_RETRY( sem_init(&(mtd.protected_rate_transition[task_id]), 0, 1) )==-1) {
		printf("mt_data_init reports: failed to initialize protected_rate_transition.\n");
		return -1;
	}

	return 0;
}

static int mt_data_check_task_ready(int task_id, int us_timeout)
{
	struct timespec ts_exp;

	get_expiration_time(&ts_exp, us_timeout);

	if ( TEMP_FAILURE_RETRY( sem_timedwait(&(mtd.task_ready[task_id]), &ts_exp) )==-1 ) {
		/* printf("mt_data_check_task_ready reports: timeout.\n"); */
		return -1;
	}

	return 0;
}

static int mt_data_set_task_ready(int task_id)
{
	if ( TEMP_FAILURE_RETRY( sem_post(&(mtd.task_ready[task_id])) )==-1) {
		printf("mt_data_set_task_ready reports: failed to set task_ready.");
		return -1;
	}

	return 0;
}

static int mt_data_check_task_start(int task_id)
{
	if ( TEMP_FAILURE_RETRY( sem_wait(&(mtd.task_start[task_id])) )==-1 ) {
		printf("mt_data_check_task_start reports: failed to get task_start.\n");
		return -1;
	}

	return 0;
}

static int mt_data_set_task_start(int task_id)
{
	if ( TEMP_FAILURE_RETRY( sem_post(&(mtd.task_start[task_id])) )==-1) {
		printf("mt_data_set_task_start reports: failed to set task_start.");
		return -1;
	}

	return 0;
}


int mt_data_copy_start(int task_id)
{
	if ( TEMP_FAILURE_RETRY( sem_wait(&(mtd.protected_rate_transition[task_id])) )==-1 ) {
		printf("mt_data_copy_start reports: failed to get protected_rate_transition.\n");
		return -1;
	}

	return 0;
}

int mt_data_copy_stop(int task_id)
{
	if ( TEMP_FAILURE_RETRY( sem_post(&(mtd.protected_rate_transition[task_id])) )==-1) {
		printf("mt_data_copy_start reports: failed to set protected_rate_transition.");
		return -1;
	}

	return 0;
}

static void* task_thread(void* parg)
{
	int tid;

	tid=*((int*) parg);

	while (1) {
#ifdef DEBUG_MT
		printf("   Task %d: waiting for start...\n", tid);
#endif
		mt_data_check_task_start(tid);
#ifdef DEBUG_MT
		printf("   Task %d: running...\n", tid);
#endif
	    double t0;
	    /* get start time */
	    t0 = get_time();

		MODEL_STEP(tid);

	    /* get stop time and diff */
	    mtt_add_to_stats(get_time() - t0, &mtd.stats_struct[tid]);
#ifdef DEBUG_MT
		printf("   Task %d: ready.\n", tid);
#endif
		mt_data_set_task_ready(tid);
	}

	return NULL;
}

static int mt_create_task(int task_id)
{
	/*	create thread */
	pthread_attr_init(&(mtd.attr_t1[task_id]));
	pthread_attr_setdetachstate(&(mtd.attr_t1[task_id]), PTHREAD_CREATE_DETACHED);
	if (pthread_create(&(mtd.t1[task_id]), &(mtd.attr_t1[task_id]), DEC task_thread, (void*) &mtd.tid[task_id])) {
		printf("mt_create_task reports: cannot create task_thread.\n");
	} else {
		printf("mt_create_task reports: task_thread has been created.\n");
	}

	return 0;
}

static int mt_delete_task(int task_id)
{
	/*	delete thread */
	pthread_cancel(mtd.t1[task_id]);

	return 0;
}

int write_task_parameters(int cpu, int prio, double stepsize, int tid)
{
	int ok;

	ok=1;

	if ( tid>FIRST_TID ) { /* subtasks only */
		if ( ( mtd.cpu[tid]>=0 ) && ( mtd.cpu[tid]!=cpu ) ) { ok=0; }
		if ( ( mtd.prio[tid]>=0 ) && ( mtd.prio[tid]!=prio ) ) { ok=0; }
		if ( ( mtd.stepsize[tid]>=0 ) && ( mtd.stepsize[tid]!=stepsize ) ) { ok=0; }

		if (ok) {
			mtd.cpu[tid]=cpu;
			mtd.prio[tid]=prio;
			mtd.stepsize[tid]=stepsize;
		} else {
			/* printf("write_task_parameters: inconsistent parameters for task %d.\n", tid); */
			rtmSetErrorStatus(RT_MDL, "inconsistent task parameters");
		}
	}

	return 0;
}

int write_task_trigger(int trigger, int tid)
{
	if ( tid>FIRST_TID ) {
		mtd.trigger[tid]=trigger;
	}

	if(mtd.dt[tid] >= 0){
		/* calculate new dt, used for statistics, once copied by statistics
		 * it is set to -1
		 */
		double t = get_time();
		if(mtd.prev_trigger[tid] > 0){
			mtd.dt[tid] = t - mtd.prev_trigger[tid];
		}
		mtd.prev_trigger[tid] = t;
	}

	return 0;
}

int read_task_status(int tid)
{
	if ( tid>FIRST_TID ) {
		if ( OverrunFlags[tid]==1 ) {
			/* printf("task %d is NOT ready.\n", tid); */
			return 0;
		} else {
			/* printf("task %d is ready.\n", tid); */
			return 1;
		}
	}

	return 0;
}

int write_task_state(int state, int tid)
{
	if ( tid>FIRST_TID ) {
		mtd.state[tid]=state;
	}

	return 0;
}

/*=================================================================*/
int mtt_save_timer_statistics(char* filename, ptimer_stat_t pstats_struct)
{
	int i;
	FILE* fp=NULL;
	unsigned long long nsamples = pstats_struct->icnt - pstats_struct->idelay;

	if( pstats_struct->icnt <= pstats_struct->idelay ){
		printf("No statistics available\n");
		return 0;
	}
	fp = fopen (filename, "w");

	if(fp == NULL){
		printf("rtc_save_timer_statistics could not open logfile to write statistics\n");
		return 0;
	}

	double avg_time = pstats_struct->total_time/nsamples;

	fprintf(fp,"Delay time:\t%d\nSample time:\t%.3e\nHIST1_RES:\t%d\nHIST2_MAX:\t%d\nNSAMPLES:\t%lld\nAVG_EXE_TIME:\t%.3e\nMAX_EXE_TIME:\t%.3e\nNOVERRUNS:\t%d\n",
			pstats_struct->idelay,pstats_struct->dt,HIST1_RESOLUTION,HIST2_MAX_EXECUTION_TIME,nsamples,avg_time,pstats_struct->max_time,pstats_struct->ioverrun);

	for(i=0;i<(100/HIST1_RESOLUTION)*2;i++){
		fprintf(fp,"%d\t[%d - %d%%]\t%d\t(%.1f%%)\n",HIST1_RESOLUTION*(2*i+1)/2
										,HIST1_RESOLUTION*i,HIST1_RESOLUTION*(i+1),pstats_struct->hist1[i]
				                        ,((double)pstats_struct->hist1[i]*100.0)/nsamples);
	}

	for(i=0;i<HIST2_MAX_EXECUTION_TIME-1;i++){
		fprintf(fp,"%d\t[%d - %d%%]\t%d\t(%.1f%%)\n",(2*i+5)*100/2,(i+2)*100,(i+3)*100,pstats_struct->hist2[i]
				                        ,((double)pstats_struct->hist2[i]*100.0)/nsamples);
	}
	i = HIST2_MAX_EXECUTION_TIME-1;
	fprintf(fp,"# [>%d%%] %d (%.1f%%)\n",(i+2)*100,pstats_struct->hist2[i]
			                        ,((double)pstats_struct->hist2[i]*100.0)/nsamples);

	fprintf(fp,"# Average execution time :\t%.3e [s] (%.1f%% of sample time)\n",avg_time,avg_time/pstats_struct->dt*100.0);
	fprintf(fp,"# Maximum execution time :\t%.3e [s] (%.1f%% of sample time)\n",pstats_struct->max_time,pstats_struct->max_time/pstats_struct->dt*100.0);
	fprintf(fp,"# Number of sample overruns :\t%d (out of %lld)\n",pstats_struct->ioverrun,nsamples);

	fclose(fp);
    return 0;
}

void mtt_add_to_stats(double diff, ptimer_stat_t pstats_struct)
{
	int idx;

	pstats_struct->icnt++;
	if(pstats_struct->icnt <= pstats_struct->idelay){
		/* start saving stats after idelay samples */
		return;
	}
	/* check for sample overrun */
    if(diff > pstats_struct->dt){
    	pstats_struct->ioverrun++;
    }
    /* check if it exceeds the maximum execution time */
    if(diff > pstats_struct->max_time){
    	pstats_struct->max_time = diff;
    }
    pstats_struct->total_time += diff;
    /* add the timing to the histogram */
    if(diff > 2*pstats_struct->dt){
    	/* put in hist2, resolution of histogram is 1 sample */
    	idx = round(diff/pstats_struct->dt)-2; /* -2 because first two seconds are covered by hist1 */
    	if(idx>HIST2_MAX_EXECUTION_TIME-1){
    		idx = HIST2_MAX_EXECUTION_TIME-1;
    	}else if(idx <= 0){
    		idx = 0;
    	}
    	pstats_struct->hist2[idx]++;
    }else{
    	/* put in hist1 */
    	idx = round((100.0/HIST1_RESOLUTION)*(diff/pstats_struct->dt));
    	if(idx>((100/HIST1_RESOLUTION) * 2)-1){
    		idx = ((100/HIST1_RESOLUTION) * 2)-1;
    	}else if(idx <= 0){
    		idx = 0;
    	}
    	pstats_struct->hist1[idx]++;
    }
}

double get_time()
{
  double t;
  struct timespec tva;

  clock_gettime(CLOCK_REALTIME, &tva);

  t = (double) (tva.tv_sec)+((double) (tva.tv_nsec))/NSEC_PER_SEC_F;

  return t;
}
/*=============================================================*/
