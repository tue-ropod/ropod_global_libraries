/*
 * mtt_functions.h
 *
 *  Created on: Mar 27, 2016
 *      Author: Yanick Douven
 */

#ifndef MTT_FUNCTIONS_H
#define MTT_FUNCTIONS_H

#include "/usr/local/include/timer_posix/timer.h"

static int get_expiration_time(struct timespec* pts_exp, int us_timeout);
static int mt_data_init(int task_id);
static int mt_data_check_task_ready(int task_id, int us_timeout);
static int mt_data_set_task_ready(int task_id);
static int mt_data_check_task_start(int task_id);
static int mt_data_set_task_start(int task_id);
int mt_data_copy_start(int task_id);
int mt_data_copy_stop(int task_id);
static void* task_thread(void* parg);
static int mt_create_task(int task_id);
static int mt_delete_task(int task_id);
int write_task_parameters(int cpu, int prio, double stepsize, int tid);
int write_task_trigger(int trigger, int tid);
int read_task_status(int tid);
int write_task_state(int state, int tid);
int mtt_save_timer_statistics(char* filename, ptimer_stat_t pstats_struct);
void mtt_add_to_stats(double diff, ptimer_stat_t pstats_struct);
double get_time();

#endif /* MTT_FUNCTIONS_H */
