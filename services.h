/* Author: Huzeyfe Çağılcı
 * Version: 1.1
 */

/*  
'x' is an integer that keeps elapsed time in miliseconds.
	We will use the 'milisn' variable for 'x'.
	'y' is an integer that keeps the loop time.
	'z' is a boolean variable.

	For 'x', we will use remainder after 'millis()' is divided by 'DV'.
	For 'y', we will use 'Task::loop_time' variable.
	For 'z', we will use 'Task::ok' variable.
*/

#include <Arduino.h>
#ifndef SERVICES_H
#define SERVICES_H

#define is_it_time(x, y, z) (x % y == 0 && z)
#define isnt_it_time(x, y, z) (x % y != 0 && !z)

/*
milisn = millis() % DV;
	milisn is the remainder after millis() is divided by DV.
	Unless this method, there will be overflow errors.
*/
#define DV 10000

/*
This variable, holds the elapsed time.
	in loop() function
		milisn=millis()%DV
*/
int milisn;

#define True 1
#define False 0

/* Example for using delay_stc
void foo(void *)
{
	static delay_stc stc;
	dly_init(&stc);			// if stc.open == 1 , stc.run will be 0
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
*/

#define atrr
#ifdef atrr
#define _atr_(x) __attribute__(x)
#else
#define _atr_(x)
#endif

/*
Use this to allow delay in functions within the loop
	dly_init(delay_stc *stc)
*/
struct delay_struct
{
	int delay_time;
	int no;
	int begin;
	_Bool run;
	_Bool open;
}_atr_((packed, aligned(1)));

struct Task
{
	void (*func)(void *);
	void *argv;
}_atr_((packed, aligned(1)));

struct Task_node
{
	byte id;
	struct Task task;
	struct Task_node *next;
}_atr_((packed, aligned(1)));

struct Service
{
	void (*func)(void *argv);
	void *argv;
	int loop_time;
	_Bool ok;
	_Bool stopped;
}_atr_((packed, aligned(1)));

struct Service_node
{
	byte id;
	struct Service serv;
	struct Service_node *next;
}_atr_((packed, aligned(1)));

typedef struct delay_struct delay_stc;
typedef struct Task Task;
typedef struct Task_node Task_node;
typedef struct Service Service;
typedef struct Service_node Service_node;

// Use this at the top of function.
void dly_init(delay_stc *stc);
// Use this where you want to delay.
void mydelay(int time, delay_stc *stc);
// Compare the time and run the func if is_it_time
void run(Service *srv);
// Every Task_node / Service_node has an id.
byte __id__;

Service_node *Service_node_init();
byte Service_node_size(Service_node *head);
byte Service_node_add(Service_node *head, Service serv);
void Service_node_delete(Service_node **head, int id);
void Service_node_run(Service_node *head);

Task_node *Task_node_init();
Task Task_node_pop(Task_node **head);
byte Task_node_size(Task_node *head);
byte Task_node_add(Task_node *head, Task task);
void Task_node_delete(Task_node **head, int id);
void Task_node_run(Task_node **head);
void Task_node_loop_run(Task_node *head);

#ifdef Long_time

volatile char second = 0;
volatile char minute = 0;
volatile char hour = 0;

inline void _timer1_set_up(int hz)
{
	cli();
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1 = 0;	//initialize counter value to 0
	// set compare match register
	OCR1A = (16 * (1000000)) / (hz * 1024) - 1; // = (16*10^6) / (1*1024) - 1 (must be <65536)
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12 and CS10 bits for 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei();
}

#define timer1_set_up() _timer1_set_up(1)

#endif
#endif //SERVICES_H
