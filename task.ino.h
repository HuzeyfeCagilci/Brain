#include "task.h"

Service_node *head;
Task_node *hd;
//Task_node *lp;

void yaz(void *argv);
void yaz_i(void *argv);
void blink(void *argv);
void info(void *argv);
void yazdir_tsk(void *argv);
void iter(void *argv);
int freeRam();

void foo(void *argv) {}

extern void *__data_end;
extern void *__bss_end;

#ifdef attr
#ifndef _atr_
#define _atr_ __attribute__((packed, aligned(1)))
#endif // _atr_
#else
#ifndef _atr_
#define _atr_
#endif // _atr_
#endif // atrr

struct blink_struct
{
	int led;
	_Bool ok;
} _atr_;

typedef blink_struct blink_stc;

void blink(void *led_ptr)
{
	blink_stc *stc = (blink_stc *)led_ptr;

	if (stc->ok)
	{
		digitalWrite(stc->led, LOW);
		stc->ok = False;
	}
	else
	{
		digitalWrite(stc->led, HIGH);
		stc->ok = True;
	}
	//Serial.println(F("blink"));
}

void SSil(Service_node **head, byte id)
{
	Node_return ret = Service_node_delete(head, id);
	switch (ret)
	{
	case Success:
		Serial.print(F("Succesfully deleted node whose id is "));
		Serial.println(id);
		break;
	case Cannot_find:
		Serial.println(F("There isn't node whose id is "));
		Serial.println(id);
		break;
	default:
		break;
	}
}

void yaz(void *argv)
{
	char *arg = (char *)argv;
	Serial.println(arg);
}

void yaz_i(void *argv)
{
	int *arg = (int *)argv;
	Serial.println(*arg);
}

int freeRam()
{
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void info(void *)
{
	Serial.print(F("Free Ram = "));
	Serial.println(freeRam());
}

void yazdir_tsk(void *argv)
{
	Task_node *node = (Task_node *)argv;
	int i = 0;
	while (node != 0)
	{
		i++;
		if (i > 10)
		{
			Serial.println(F("Aşım gerçekleşti."));
			return;
		}
		Serial.print(F("adres = "));
		Serial.println((int)node);
		Serial.print(F("id = "));
		Serial.println((byte)node->id);
		Serial.print(F("next = "));
		Serial.println((int)node->next);
		node = node->next;
	}
	Serial.println(F("yazdir_tsk sonu"));
}