# BRAIN
## Task Scheduler
+ Executes tasks periodicly.
+ Keeps tasks in nodes.
+ Uses dynamic memory.
## Task Types
+ Basic Task
+ Endless Task
+ Scheduled Task
+ Scheduled Endless Task
## Usage
Add header files.
~~~C
#include <sys.h>	/* Includes 'System' structure and other header files. */
#include <funcs.h>	/* Includes ready-made functions. It is optional. */
~~~
Define a function.
~~~C
/* Definition of a function for task node
 *
 * It is a void function and returns nothing.
 * It takes a void* parameter. */
void foo(void *arg)
{
	/* Convert arg to int pointer. */
	int *p = (int *)arg;
	int i = *p;

	Serial.println(i);

	i = i * 2;
	*p = i;
}
~~~
~~~C
void setup()
{
	/* Config sys */
	Sys_setup();
~~~
Creating a scheduled task
~~~C
	/* Create a pointer to use in function.*/
	int *pointer = (int *)malloc(sizeof(int)); 
	*pointer = 10;

	Task_arg *task_arg = Task_arg_create(pointer, 1000);

	Task task = Task_create(foo, task_arg, 10, Scheduled_Task);
	Task_node_add(&System.tasks, task); /* Add the task to node */
}
~~~
