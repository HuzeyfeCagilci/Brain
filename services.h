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

#define true 1
#define false 0

struct delay_struct
{
	int delay_time;
	int no;
	int ret;
	int begin;
	_Bool run;
	_Bool open;
};

struct Task
{
	void (*func)(void *);
	void *argv;
};

struct Task_node
{
	struct Task task;
	struct Task_node *next;
};

struct Service
{
	void (*func)(void *argv);
	void *argv;
	int loop_time;
	_Bool ok;
	_Bool stopped;
};

struct Service_node
{
	struct Service serv;
	struct Service_node *next;
};

typedef struct delay_struct delay_stc;
typedef struct Task Task;
typedef struct Task_node Task_node;
typedef struct Service Service;
typedef struct Service_node Service_node;

// Use this at the top of function.
inline void dly_init(delay_stc *stc);
// Use this where you want to delay.
inline void mydelay(int time, delay_stc *stc);
// Compare the time and run the func if is_it_time
inline void run(Service *srv);

Service_node *Service_node_init();
int Service_node_size(Service_node *head);
inline void Service_node_add(Service_node *head, Service serv);
inline void run_Service_node(Service_node *head);

inline void Task_node_add(Task_node *head, Task task);
Task Task_node_pop(Task_node **head);
inline void Task_node_run(Task_node **head);

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