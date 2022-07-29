#ifndef Tash_dec_h
#define Tash_dec_h

#include <Arduino.h>

#ifndef no_inline
#define INLINE inline
#else
#define INLINE
#endif // no_inline

#ifndef no_atr
#define _atr_ __attribute__((packed, aligned(1)))
#else
#define _atr_
#endif // no_atr

#ifndef DV
#define DV 100000
#endif

/*
  This variable, holds the elapsed time.
	  in loop() function
		  _time_ = millis() % DV
  */
u32 _time_;

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

enum _task_type_
{
	Basic_Task,
	Scheduled_Task,
	Endless_Task,
	Scheduled_Endless_Task,
	Stopped_Task
} _atr_;

struct Task
{
	void (*func)(void *argv);
	void *argv;
	u16 count;
	u8 type;
} _atr_;

struct Task_arg
{
	void *argv;
	u32 period;
	/*31 bit for last, 1 bit for turn.*/
	u32 last;
	// bool turn;
} _atr_;

struct Task_node
{
	Task task;
	struct Task_node *next;
	u16 id;
} _atr_;

struct delay_stc
{
	u32 delay_time;
	u32 begin;
	u8 no;
	_Bool run;
	_Bool open;
} _atr_;

typedef struct Task Task;
typedef struct Task_arg Task_arg;
typedef struct Task_node Task_node;
typedef struct delay_stc delay_stc;
typedef enum _task_type_ _task_type_;

#ifdef enable_hash

#ifndef type_hash
#define type_hash unsigned short
#endif

INLINE void _hash(type_hash *h, int x);
INLINE void hash_str(type_hash *h, char *s);

#else
u8 __id__;
#endif

INLINE Task Task_create(void (*func)(void *), void *argv, u16 count, _task_type_ type);
INLINE Task_arg *Task_arg_create(void *argv, u32 period);
INLINE bool check_time(Task_arg *targ);

INLINE u16 Task_node_add(Task_node **head, Task task);
INLINE u8 Task_node_size(Task_node *head);
INLINE _return_ Task_node_delete(Task_node **head, byte id);
INLINE _return_ Task_node_run(Task_node **head);
INLINE void Task_node_config(Task_node **head);
INLINE Task_node *Task_node_addr(Task_node *head, u8 id);
INLINE _return_ Task_node_change_type(Task_node *head, u8 id, _task_type_ type);

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
