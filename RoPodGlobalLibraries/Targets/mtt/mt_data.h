#ifndef _MT_DATA_
#define _MT_DATA_

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>
#include <sys/errno.h>
#include <pthread.h>
#include <semaphore.h>

#define INFPERIOD	1000000.0
#define FIRST_SLAVETK	10

#define PERIODIC_TASK	1
#define SLAVE_TASK		2

#define TRIGGER		(2*FIRST_SLAVETK)

#define NOT_TRIGGERED	0
#define TRIGGERED	1

#define IN_STATE	0 /* initialization state */
#define OP_STATE	1 /* operational state */

#ifndef DEC
#define DEC ( void* (*)(void*) )
#endif

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(expression) \
		({ \
	long int _result; \
	do _result = (long int) (expression); \
	while (_result == -1L && errno == EINTR); \
	_result; \
})
#endif

#endif /* _MT_DATA_ */



