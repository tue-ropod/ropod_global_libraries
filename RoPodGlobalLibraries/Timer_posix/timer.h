#ifndef TIMER_H
#define TIMER_H

typedef int (*PTIMERCALLBACK) (int,int);

#define HIST1_RESOLUTION 2 /* in percentage */
#define HIST2_MAX_EXECUTION_TIME 99 /* in samples */
typedef struct tag_timer_stat
{
    unsigned long long icnt;
    unsigned int idelay;
    unsigned int ioverrun;
    double total_time;
    double max_time;
    double dt;
    unsigned int hist1[(100/HIST1_RESOLUTION) * 2]; /* used for execution times between 0 and 200% */
    unsigned int hist2[HIST2_MAX_EXECUTION_TIME]; /* used for execution times larger than 200%, resolution is 1 sample */
} timer_stat_t, *ptimer_stat_t;

#ifdef __cplusplus
extern "C" {
#endif
int    	rtc_usleep(int);
int		rtc_reset_time();
double 	rtc_get_time();
int 	rtc_get_timer_stat(ptimer_stat_t);
void	rtc_postpone_statistics_calculation(unsigned int nsamples);
int 	rtc_print_timer_statistics();
int 	rtc_save_timer_statistics(char*);
void	add_to_stats(double diff);
int		rtc_signalhandler(PTIMERCALLBACK the_user_apc);
void	rtc_add_timespec(struct timespec *ts, signed long long addtime);
void	rtc_rttask(double dt,PTIMERCALLBACK the_user_apc);
int		rtc_timer(double freq, PTIMERCALLBACK user_apc, int priority);
#ifdef __cplusplus
}
#endif

#define NSEC_PER_SEC 1000000000L
#define USEC_PER_SEC 1000000L
#define USEC_PER_SEC_F 1000000.0
#define NSEC_PER_SEC_F 1000000000.0

#endif /* TIMER_H */
