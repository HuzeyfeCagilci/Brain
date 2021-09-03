#include <Arduino.h>
#include <stdlib.h>
#define DV 10000

#include "task.h"
#include "funcs_1.h"

Task_node *head;

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

	Task_node_config(&head);
	info(NULL);
	Task_node_add(&head, Task_create(blink, Task_arg_create(s, 1000), 4, Scheduled_Task));
	Task_node_add(&head, Task_create(print_task_node, Task_arg_create(&head, 2000), 4, Scheduled_Task));
	//Task_node_add(&head, Task_create(info, Task_arg_create(NULL, 2000), 6, Scheduled_Task));
	//Task_node_add(&head, Task_create(blink, Task_arg_create(s, 1000), 4, Scheduled_Task));
	info(NULL);
	print_task_node(&head);

	Serial.println(F("ok"));
	//Serial.println(Task_node_size(head));

	//delay(5000);
}

void loop()
{
	UPT;
	Task_node_run(&head);
}