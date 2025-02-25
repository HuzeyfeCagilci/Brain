#ifndef SYS_DEC_H
#define SYS_DEC_H

#ifdef AFSD
#include <Adafruit_SleepyDog.h>
#endif

#include "task.h"

/* Config sys (system node) */
void Sys_setup();

/* If there is no task,
 * sleep by using watchdog timer.
 * This function is added on node
 * in Sys_setup function.*/
#ifdef AFSD
void Sleep_no_task(void *);
#endif

struct system
{
	/* Node for user tasks */
	Task_node *tasks;
	/* Node for system tasks */
	Task_node *sys_tasks;
	/* Node for disposable tasks */
	Task_node *tmp;
	/* Is pin using */
	bool pins[16];
	// u8 ids[16];
} _atr_;

#endif
