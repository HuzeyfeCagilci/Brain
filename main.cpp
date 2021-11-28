#include <Arduino.h>
#include <stdlib.h>
#define DV 10000

#include "task.h"
#include "sys.h"
#include "funcs_1.h"
#include "funcs_2.h"


void setup()
{
	Serial.begin(9600);
	__id__ = 0;
	pinMode(LED_BUILTIN, OUTPUT);

	blink_stc *s = (blink_stc *)malloc(sizeof(blink_stc));
	s->led = LED_BUILTIN;
	s->ok = 0;

	Serial.println(F("\n--- Size of ---"));
	Serial.println(F("Task\t\tTask_node\tTask_arg\tDealy_stc\tBlink_stc"));
	Serial.print(sizeof(Task));
	Serial.print(F("\t\t"));
	Serial.print(sizeof(Task_node));
	Serial.print(F("\t\t"));
	Serial.print(sizeof(Task_arg));
	Serial.print(F("\t\t"));
	Serial.print(sizeof(delay_stc));
	Serial.print(F("\t\t"));
	Serial.println(sizeof(blink_stc));
	Serial.println(F("---------------"));

	Task_node_config(&System.tasks);
	info(NULL);
	//Task_node_add(&System.tasks, Task_create(blink, Task_arg_create(s, 1000), 4, Scheduled_Task));
	set_blink(LED_BUILTIN, 1000, 1, Scheduled_Endless_Task);
	
	Task_node **tmp = (Task_node**)malloc(sizeof(Task_node*));
	*tmp = System.tasks;
	
	Task_node_add(&System.tasks, Task_create(print_task_node, Task_arg_create(tmp, 2000), 2, Scheduled_Task));
	//Task_node_add(&System.tasks, Task_create(info, Task_arg_create(NULL, 2000), 2, Scheduled_Endless_Task));
	info(NULL);
	print_task_node(&System.tasks);

	Serial.println(F("ok"));

	//delay(5000);
}

void loop()
{
	UPT;
	Task_node_run(&System.tasks);
}