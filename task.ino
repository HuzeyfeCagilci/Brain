#include "services.h"
#include "services.c"

void blink(void *led);
void sr_read(void *argv);

int led = LED_BUILTIN;

Service_node *head; // = {&blink, &led, 1000, 1, 0};
Task_node *hd;
Task_node *lp;

void setup()
{
	Serial.begin(9600);
	pinMode(led, OUTPUT);
	Serial.println("Jello");

	head = Service_node_init();

	Service_node_add(head, {&blink, &led, 1000, 1, 0});

	char ya[] PROGMEM = {"Bir kere yazılacak."};

	hd = Task_node_init();
	lp = Task_node_init();
	Task_node_add(hd, {&yaz, &ya});
	Task_node_add(lp, {&foo, NULL});
	Serial.print("Size of hd = "); Serial.println(Task_node_size(hd));
	Serial.print("Size of lp = "); Serial.println(Task_node_size(lp));
	Serial.print("Size of head = "); Serial.println(Service_node_size(head));
}

void loop()
{
	milisn = millis() % DV;
	Service_node_run(head);
	Task_node_run(&hd);
	Task_node_loop_run(lp);
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

void foo(void *)
{
	static delay_stc stc;
	dly_init(&stc);
	static int id=0;

	while(stc.run)
	{	
		switch (id)
		{
		case 0:
			id++;
			Serial.println(id);
			mydelay(1000, &stc);
			break;

		case 1:
			id=0;
			Serial.println(id);
			mydelay(1000, &stc);
			break;

		default : id = 0;
			break;
		}
	}
}

const char sr[] = "Serial acik.";
int servicess;
const char yazi[] = "Huzeyfe";

/*
void sr_read(void *argv)
{
	if (Serial.available())
	{
		Serial.readString();
		Task_node_add(hd, {&yaz, (void *)sr});
		servicess = Service_node_size(head);
		Task_node_add(hd, {&yaz_i, (void *)&servicess});
		//Service_node_add(head, {&yaz, (void *)yazi, 2000, 1, 0});
		Service_node_add(head, {&yaz, (void *)yazi, 1000, 1, 0});
	}
}*/