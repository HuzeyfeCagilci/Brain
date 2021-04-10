#include <Arduino.h>
#include "task.h"

Service_node *head;
//Task_node *hd;
//Task_node *lp;

int ids[10];

#include "task.ino.h"

void setup()
{
	__id__ = 0;
	Serial.begin(9600);

	info(NULL);

	int *iterator = (int *)malloc(sizeof(int));
	*iterator = 0;

	Serial.print(F("Size of int = "));
	Serial.println(sizeof(int));
	Serial.print(F("__data_end = "));
	Serial.println((int)&__data_end);
	Serial.print(F("__bss_end = "));
	Serial.println((int)&__bss_end);
	Serial.print(F("Size of Task = "));
	Serial.println(sizeof(struct Task));
	Serial.print(F("Size of Task_node = "));
	Serial.println(sizeof(struct Task_node));
	Serial.print(F("Size of Service = "));
	Serial.println(sizeof(struct Service));
	Serial.print(F("Size of Service_node = "));
	Serial.println(sizeof(struct Service_node));
	Serial.print(F("Size of delay_stc = "));
	Serial.println(sizeof(struct delay_struct));

	Service_node_add(&head, {info, NULL, 1000, 1, 0});
	ids[0] = Service_node_add(&head, {foo, NULL, 1000, 1, 0});
	ids[1] = Service_node_add(&head, {foo, NULL, 1000, 1, 0});
	ids[2] = Service_node_add(&head, {iter, iterator, 1000, 1, 0});

	info(NULL);
	Serial.println(F("-------------------"));
}

void loop()
{
	_time_ = millis() % DV;
	Service_node_run(head);
}
