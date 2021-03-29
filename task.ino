#include "services.h"
#include "services.c"
#include "mem/MemoryFree.h"
#include "mem/MemoryFree.cpp"

void yaz(void);
void yaz_i(void);
void blink(void);
void info(void);
void yazdir_tsk(void);

Service_node *head;
Task_node *hd;
Task_node *lp;

void setup()
{
	__id__ = 0;
	Serial.begin(9600);

	Serial.print(F("Bos bellek = "));
	Serial.println(freeMemory());

	Serial.print(F("Size of Task = "));
	Serial.println(sizeof(struct Task));
	Serial.print(F("Size of Task_node = "));
	Serial.println(sizeof(struct Task_node));
	Serial.print(F("Size of Service = "));
	Serial.println(sizeof(struct Service));
	Serial.print(F("Size of Service_node = "));
	Serial.println(sizeof(struct Service_node));
	Serial.println(F("-------------------"));
}

void loop()
{
	/*milisn = millis() % DV;
	Service_node_run(head);
	Task_node_run(&hd);
	Task_node_loop_run(lp);*/
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
	int _led = *(int *)led_ptr;
	static bool ok = false;

	if (ok)
	{
		digitalWrite(_led, LOW);
		ok = false;
	}
	else
	{
		digitalWrite(_led, HIGH);
		ok = true;
	}
}
/* 
void foo(void *)
{
	static delay_stc stc;
	dly_init(&stc);
	static int no = 0, times = 0;

	while (stc.run)
	{
		switch (no)
		{
		case 0:
			no++;
			times++;
			//Serial.println(no);
			mydelay(1000, &stc);
			break;

		case 1:
			no = 0;
			times++;
			//Serial.println(no);
			mydelay(1000, &stc);
			break;

		default:
			no = 0;
			break;
		}

		if (times >= 4)
		{
			times = 0;
			Serial.println(F("\nFoo'dan selamlar.\n"));
			Serial.println((int)lp);

			Task_node_delete(&lp, id);
			Serial.println(F("Silindi"));
			yazdir_tsk(&lp);
			Task_node_add(lp, {&foo2, NULL});
			Task_node_add(lp, {&foo3, NULL});
			yazdir_tsk(&lp);
		}
	}
}

void foo2(void *)
{
	static delay_stc stc;
	dly_init(&stc);
	static int no = 0, times = 0;

	while (stc.run)
	{
		switch (no)
		{
		case 0:
			no++;
			times++;
			//Serial.println(no);
			mydelay(1000, &stc);
			break;

		case 1:
			no = 0;
			times++;
			//Serial.println(no);
			mydelay(1000, &stc);
			break;

		default:
			no = 0;
			break;
		}

		if (times >= 4)
		{
			times = 0;
			Serial.println(F("\nFoo2'den selamlar.\n"));
		}
	}
}

void foo3(void *)
{
	static delay_stc stc;
	dly_init(&stc);
	static int no = 0, times = 0;

	while (stc.run)
	{
		switch (no)
		{
		case 0:
			no++;
			times++;
			//Serial.println(no);
			mydelay(1000, &stc);
			break;

		case 1:
			no = 0;
			times++;
			//Serial.println(no);
			mydelay(1000, &stc);
			break;

		default:
			no = 0;
			break;
		}

		if (times >= 4)
		{
			times = 0;
			Serial.println(F("\nFoo3'ten selamlar.\n"));
		}
	}
} */

void info(void *)
{
	Serial.print(F("Boş bellek = "));
	Serial.println(freeMemory());
	Serial.print("Size of hd = ");
	Serial.println(Task_node_size(hd));
	Serial.print("Size of lp = ");
	Serial.println(Task_node_size(lp));
	Serial.print("Size of head = ");
	Serial.println(Service_node_size(head));
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