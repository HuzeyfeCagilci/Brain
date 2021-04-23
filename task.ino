#include <Arduino.h>
#include "task.h"
#include "task.ino.h"

blink_stc bl = {LED_BUILTIN, 0};
int ids[4];

void setup()
{
	__id__ = 0;
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);

	info(NULL);

	Serial.println(F("-------------------"));
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

	Service_node_add(&head, {blink, &bl, 1000});

	info(NULL);
	Serial.println(F("-------------------"));
}