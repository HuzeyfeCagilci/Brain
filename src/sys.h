#ifndef SYS_H
#define SYS_H

#include "sys_dec.h"

struct system System;

void Sys_setup()
{
	#ifndef enable_hash
	__id__ = 0;
	#endif
	Task_node_config(&System.tasks);
	Task_node_config(&System.sys_tasks);
	Task_node_config(&System.tmp);
	for (u8 i = 0; i < 16; i++)
		System.pins[i] = 0;
#ifdef AFSD
	Task_node_add(&System.sys_tasks, Task_create(Sleep_no_task, NULL, 1, Endless_Task));
#endif
}

#ifdef AFSD
void Sleep_no_task(void *)
{
	if (Task_node_size(System.tasks) == 0 && Task_node_size(System.tmp) == 0)
	{
		Watchdog.enable();
		Serial.println(F("Sleep..."));
		delay(1000);
		Watchdog.sleep();
	}
}

#endif // AFSD

#endif // SYS_H