#include "services.h"
#include "services.c"

void blink(void *led);
void sr_read(void *argv);

int led = LED_BUILTIN;

Service_node *head; // = {&blink, &led, 1000, 1, 0};
Task_node *hd;

void setup()
{
	Serial.begin(9600);
	pinMode(led, OUTPUT);
	Serial.println("Jello");

	head = Service_node_init();

	Service_node_add(head, {&blink, &led, 1000, 1, 0});
	//char yazi[] PROGMEM = {"Huzeyfe"};

	//Service_node_add(head, {&yaz, (void *)yazi, 2000, 1, 0});
	char ya[] PROGMEM = {"Bir kere yazılacak."};

	Service_node_add(head, {&sr_read, NULL, 1000, 1, 0});

	hd = Task_node_init();
	Task_node_add(hd, {&yaz, &ya});
}

void loop()
{
	milisn = millis() % DV;
	run_Service_node(head);
	Task_node_run(&hd);
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

const char sr[] = "Serial acik.";
int servicess;
const char yazi[] = "Huzeyfe";

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
}
