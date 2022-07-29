// Not tested

#ifndef FUNCS_H
#define FUNCS_H

#include "funcs_dec.h"
#include "sys.h"

int freeRam()
{
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void blink(void *led_ptr)
{
	blink_stc *stc = (blink_stc *)led_ptr;

	if (stc->ok)
	{
		digitalWrite(stc->led, LOW);
		stc->ok = false;
	}
	else
	{
		digitalWrite(stc->led, HIGH);
		stc->ok = true;
	}
}

void info(void *)
{
	extern int __heap_start, *__brkval;
	int v;
	Serial.println(F("\n--- Free Ram ---"));
	Serial.println(F("Free Ram\thp_st\tbrkv\tv"));
	Serial.print(freeRam());
	Serial.print(F("\t\t"));
	Serial.print(__heap_start);
	Serial.print(F("\t"));
	Serial.print((int)__brkval);
	Serial.print(F("\t"));
	Serial.println((int)&v);
	Serial.println(F("----------------"));
}

void print_task_node(void *argv)
{
	Task_node *node = *((Task_node **)argv);
	byte i = 0;

	Serial.println(F("\n--- Task Node ---"));

	Serial.println(F("No\tadres\tid\ttype\tcount\tnext\tperiod\tlast"));

	do
	{
		if (i > 10)
		{
			Serial.println(F("Overflow error"));
			return;
		}

		Serial.print(i);
		Serial.print(F("\t"));
		Serial.print((int)node);
		Serial.print(F("\t"));
		Serial.print(node->id);
		Serial.print(F("\t"));
		Serial.print(node->task.type);
		Serial.print(F("\t"));
		Serial.print(node->task.count);
		Serial.print(F("\t"));
		Serial.print((int)node->next);
		if (node->task.type == Scheduled_Endless_Task || node->task.type == Scheduled_Task)
		{
			Task_arg *tmp = (Task_arg *)node->task.argv;
			Serial.print(F("\t"));
			Serial.print((long)tmp->period);
			Serial.print(F("\t"));
			Serial.print((long)tmp->last);
		}
		Serial.println();
		node = node->next;
		i++;
	} while (node != 0);
	Serial.println(F("-----------------"));
}

void print_task(void *argv)
{
	Task_node *node = (Task_node *)argv;
	Serial.println(F("adres\tid\tnext"));
	Serial.print((int)node);
	Serial.print(F("\t"));
	Serial.print((byte)node->id);
	Serial.print(F("\t"));
	Serial.println((int)node->next);
}

_return_ set_blink(byte pin, u32 period, u8 count, _task_type_ type)
{
	if (System.pins[pin])
		return Pin_is_busy;

	pinMode(pin, OUTPUT);

	blink_stc *bs = (blink_stc *)malloc(sizeof(blink_stc));

	if (bs == 0)
		return Malloc_fail;

	bs->led = pin;

	/*System.ids[pin] = */ Task_node_add(&System.tasks, Task_create(blink, Task_arg_create(bs, period), count, type));

	System.pins[pin] = 1;

	return Success;
}

#endif // FUNCS_H