#include <stdlib.h>
#include "services.h"

inline void dly_init(delay_stc *stc)
{
	if (!stc->open)
		stc->run = true;
	else if ((milisn > stc->begin && milisn - stc->begin >= stc->delay_time) || (milisn < stc->begin && milisn - stc->begin + DV >= stc->delay_time))
	{
		stc->run = true;
		stc->delay_time = 0;
		stc->open = false;
	}
}

inline void mydelay(int time, delay_stc *stc)
{
	stc->run = false;
	stc->delay_time = time;
	stc->begin = milisn;
	stc->open = true;
}

inline void run(Service *srv)
{
	if (srv->stopped)
		return;

	if (is_it_time(milisn, srv->loop_time, srv->ok))
	{
		srv->ok = false;
		(*srv->func)(srv->argv);
	}
	else if (isnt_it_time(milisn, srv->loop_time, srv->ok))
	{
		srv->ok = true;
	}
}

inline void Service_node_add(Service_node *head, Service serv)
{
	if (head == 0)
		return;

	while (head->next != 0)
	{
		head = head->next;
	}

	head->serv = serv;
	head->next = (Service_node *)malloc(sizeof(Service_node));
	head->next->next = 0;
}

Service_node *Service_node_init()
{
	Service_node *head = (Service_node *)malloc(sizeof(Service_node));
	head->next = 0;
	return head;
}

int Service_node_size(Service_node *head)
{
	int ret=0;
	while(head->next!=0)
	{
		head=head->next;
		ret++;
	}
	return ret;
}

inline void run_Service_node(Service_node *head)
{
	while (head->next != 0)
	{
		run(&head->serv);
		head = head->next;
	}
}

Task_node *Task_node_init()
{
	Task_node *head = (Task_node *)malloc(sizeof(Task_node));
	head->next = 0;
	return head;
}

inline void Task_node_add(Task_node *head, Task task)
{
	if (head == 0)
		return;

	while (head->next != 0)
	{
		head = head->next;
	}

	head->task = task;
	head->next = (Task_node *)malloc(sizeof(Task_node));
	head->next->next = 0;
}

Task Task_node_pop(Task_node **head)
{
	Task_node *tmp = *head, *prev;
	*head = tmp->next;
	Task ret=tmp->task;
	free(tmp);
	return ret;
}

inline void Task_node_run(Task_node **head)
{
	while ((*head)->next != 0)
	{
		Task task = Task_node_pop(head);
		task.func(task.argv);
	}
}

#ifdef Long_time

ISR(TIMER1_COMPA_vect)
{
	cli();

	second++;

	if (second == 60)
	{
		minute++;
		second = 0;
	}

	if (minute == 60)
	{
		hour++;
		minute = 0;
	}

	Serial.println((int)second);

	sei();
}

#endif