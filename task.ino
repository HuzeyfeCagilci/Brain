#include <Arduino.h>
#include "task.h"
#include "task.c"

void yaz(void);
void yaz_i(void);
void blink(void);
void info(void);
void yazdir_tsk(void);

extern void *__data_end;
extern void *__bss_end;

struct blink_stc
{
	int led;
	_Bool ok;
} bl1;

Service_node *head;
//Task_node *hd;
//Task_node *lp;

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

int ids[10];

void setup()
{
	__id__ = 0;
	Serial.begin(9600);

	info(NULL);

	Serial.print(F("__data_end = "));
	Serial.println((int) &__data_end);
	Serial.print(F("__bss_end = "));
	Serial.println((int) &__bss_end);
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
	
	head = Service_node_init();
	Service_node_add(&head, {info, NULL, 1000, 1, 0});

	info(NULL);
	Serial.println(F("-------------------"));
}

void loop()
{
	milisn = millis() % DV;
	Service_node_run(head);
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
	Serial.print(F("Free Ram = "));
	Serial.println(freeRam());
	/*Serial.print("Size of hd = ");
	Serial.println(Task_node_size(hd));
	Serial.print("Size of lp = ");
	Serial.println(Task_node_size(lp));
	Serial.print("Size of head = ");
	Serial.println(Service_node_size(head));*/
}

void yazdir_tsk(void *argv)
{
	Task_node *node = *((Task_node **)argv);
	int i = 0;
	while (node != 0)
	{
		i++;
		if (i > 10)
		{
			Serial.println(F("Aşım gerçekleşti."));
			return;
		}
		Serial.print("adres = ");
		Serial.println((int)node);
		Serial.print("id = ");
		Serial.println(node->id);
		Serial.print("next = ");
		Serial.println((int)node->next);
		node = node->next;
	}
	Serial.println(F("yazdir_tsk sonu"));
}