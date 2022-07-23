#ifndef Tash_dec_h
#define Tash_dec_h

#include <Arduino.h>

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
#ifndef _atr_
#define _atr_ __attribute__((packed, aligned(1)))
#endif // _atr_
#else
#define _atr_
#endif // attr

#ifndef DV
#define DV 100000
#endif

/*
  This variable, holds the elapsed time.
	  in loop() function
		  _time_ = millis() % DV
  */
uint64_t _time_;
uint8_t __id__;

/* _time_ is the remainder after millis() is divided by DV.
 * Unless this method, there will be overflow errors.
 * If you want microseconds use this instead of it:
 * _time_ = micros() % DV; */
#ifndef UPT
#define UPT (_time_ = millis() % DV)
#endif

typedef enum
{
	Success,
	Malloc_fail,
	Cannot_find,
	Pin_is_busy,
	Other_fail
} _return_;

typedef enum
{
	Basic_Task,
	Scheduled_Task,
	Endless_Task,
	Scheduled_Endless_Task,
	Stopped_Task
} _task_type_;

struct Task
{
	void (*func)(void *argv);
	void *argv;
	uint16_t count;
	_task_type_ type;
} _atr_;

struct Task_arg
{
	void *argv;
	uint64_t period;
	uint64_t last;
	bool turn;
} _atr_;

struct Task_node
{
	Task task;
	struct Task_node *next;
	uint8_t id;
} _atr_;

struct delay_stc
{
	uint64_t delay_time;
	uint64_t begin;
	uint8_t no;
	_Bool run;
	_Bool open;
} _atr_;

typedef struct Task Task;
typedef struct Task_arg Task_arg;
typedef struct Task_node Task_node;
typedef struct delay_stc delay_stc;

INLINE Task Task_create(void (*func)(void *), void *argv, uint16_t count, _task_type_ type);
INLINE Task_arg *Task_arg_create(void *argv, uint64_t period);
INLINE bool check_time(Task_arg *targ);

INLINE uint8_t Task_node_add(Task_node **head, Task task);
INLINE uint8_t Task_node_size(Task_node *head);
INLINE _return_ Task_node_delete(Task_node **head, byte id);
INLINE _return_ Task_node_run(Task_node **head);
INLINE void Task_node_config(Task_node **head);
INLINE Task_node * Task_node_addr(Task_node *head, uint8_t id);
INLINE _return_ Task_node_change_type(Task_node *head, uint8_t id, _task_type_ type);

/* Use this at the top of function. */
INLINE void dly_init(delay_stc *stc);
/* Use this where you want to delay. */
INLINE void mydelay(long time, delay_stc *stc);

/* Example for using delay_stc
void foo(void *)
{
	static delay_stc stc;
	dly_init(&stc);			// if stc.open == 1 , stc.run will be 0

	while(stc.run)
	{
		switch (stc.no)
		{
		case 0:
			stc.no++;
			Serial.println(stc.no);
			mydelay(1000, &stc);
			break;

		case 1:
			stc.no=0;
			Serial.println(stc.no);
			mydelay(1000, &stc);
			break;

		default: 
			stc.no = 0;
			break;
		}
	}
}
*/

#endif // Tash_dec_h
