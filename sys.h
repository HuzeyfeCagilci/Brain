# ifndef SYS_H
# define SYS_H

#include "sys_dec.h"

struct system System;

void Sys_setup()
{
	__id__ = 0;	
	Task_node_config(&System.tasks);
	Task_node_config(&System.sys_tasks);
	Task_node_config(&System.tmp);
	Task_node_add(&System.sys_tasks,Task_create(Sleep_no_task, NULL, 1, Endless_Task));
}

void Sleep_no_task(void *)
{
	if(Task_node_size(System.tasks)==0 && Task_node_size(System.tmp)==0)
	{
		Watchdog.enable();
		Serial.println(F("Sleep..."));
		delay(1000);
		Watchdog.sleep();
	}
}

# endif