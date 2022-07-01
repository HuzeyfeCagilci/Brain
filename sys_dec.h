#ifndef SYS_DEC_H
#define SYS_DEC_H

#include <Adafruit_SleepyDog.h>

#include "task.h"

void Sys_setup();
void Sleep_no_task(void *);

struct system
{
	Task_node * tasks;
	Task_node * sys_tasks;
	Task_node * tmp;
	bool pins [16];
}_atr_;

#endif