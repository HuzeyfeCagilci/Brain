/* Author: Huzeyfe Çağılcı
 * Version: 1.4
 */

#include <Arduino.h>
#ifndef TASK_DECLARATIONS_H
#define TASK_DECLARATIONS_H

/*
 'x' is an integer that keeps elapsed time in miliseconds.
	 We will use the '_time_' variable for 'x'.
	 'y' is an integer that keeps the loop time.
	 'z' is a boolean variable.

	 For 'x', we will use remainder after 'millis()' is divided by 'DV'.
	 For 'y', we will use 'Task::loop_time' variable.
	 For 'z', we will use 'Task::ok' variable.
 */
#define is_it_time(x, y, z) (x % y == 0 && z)
/* "NOT" type of is_it_time macro. */
#define isnt_it_time(x, y, z) (x % y != 0 && !z)

/*
 _time_ = millis() % DV;
	 _time_ is the remainder after millis() is divided by DV.
	 Unless this method, there will be overflow errors.
	 If you want microseconds use this instead of it:
	_time_ = micros() % DV;

 */
#define DV 100000

/*
 This variable, holds the elapsed time.
	 in loop() function
		 _time_ = millis () %DV
 */
long _time_;

// Every Task_node / Service_node has an id.
byte __id__;

/* Sometimes, there may be an error about "True" or "False". */

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

#ifdef _inline_
#ifndef INLINE
#define INLINE inline
#endif // INLINE
#else
#ifndef INLINE
#define INLINE
#endif // INLINE
#endif // _inline_

#ifdef atrr
#define _atr_ __attribute__((packed, aligned(1)))
#else
#define _atr_
#endif // attr

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
} _atr_;

struct Task
{
	void (*func)(void *);
	void *argv;
} _atr_;

struct Task_node
{
	byte id;
	struct Task task;
	struct Task_node *next;
} _atr_;

struct Service
{
	void (*func)(void *argv);
	void *argv;
	int loop_time;
	_Bool ok;
	_Bool stopped;
} _atr_;

struct Service_node
{
	byte id;
	struct Service serv;
	struct Service_node *next;
} _atr_;

typedef struct delay_struct delay_stc;
typedef struct Task Task;
typedef struct Task_node Task_node;
typedef struct Service Service;
typedef struct Service_node Service_node;

typedef enum
{
	Success,
	Malloc_fail,
	Cannot_find
} Node_return;

// Use this at the top of function.
INLINE void dly_init(delay_stc* stc);
// Use this where you want to delay.
INLINE void mydelay(int time, delay_stc* stc);
// delete the last node and return it
INLINE Task node_pop(Task_node **head);
// Compare the time and run the func if is_it_time
INLINE void run(Service *srv);
INLINE Task_node *Task_node_init();
INLINE Service_node *Service_node_init();
INLINE byte Service_node_size(Service_node *head);

INLINE byte Task_node_add(Task_node **head, Task task);
INLINE byte Service_node_add(Service_node **head, Service serv);
INLINE byte Task_node_size(Task_node *head);
INLINE byte Service_node_size(Service_node *head);

INLINE void Task_node_run(Task_node **head);
// pop last node and run
INLINE void Service_node_run(Service_node *head);
// don't pop, just run
INLINE void Task_node_loop_run(Task_node *head);

INLINE Node_return Task_node_delete(Task_node **head, byte id);
INLINE Node_return Service_node_delete(Service_node **head, byte id);

#ifdef Long_time

volatile char second = 0;
volatile char minute = 0;
volatile char hour = 0;

INLINE void _timer1_set_up(int hz);

#define timer1_set_up() _timer1_set_up(1)

#endif
#endif // TASK_DECLARATIONS_H
