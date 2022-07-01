# ifndef SYS_H
# define SYS_H

#include "sys_dec.h"

struct system System;

void Sys_setup()
{
	Task_node_config(&System.tasks);
	Task_node_add(&System.sys_tasks,Task_create(Sleep_no_task, NULL, 1, Endless_Task));
	__id__ = 0;
}

void Sleep_no_task(void *)
{
	if(Task_node_size(System.tasks)==1 /*&& Task_node_size(System.tmp)*/)
	{
		Serial.println("Uyku...");
		delay(1000);
		Watchdog.sleep();
	}
}

# endif