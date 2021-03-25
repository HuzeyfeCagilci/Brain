#ifndef SERVICES_C
#define SERVICES_C

#include <stdlib.h>
#include "services.h"

void dly_init(delay_stc *stc)
{
	if (!stc->open)
		stc->run = True;
	else if ((milisn > stc->begin && milisn - stc->begin >= stc->delay_time) || (milisn < stc->begin && milisn - stc->begin + DV >= stc->delay_time))
	{
		stc->run = True;
		stc->delay_time = 0;
		stc->open = False;
	}
}

void mydelay(int time, delay_stc *stc)
{
	stc->run = False;
	stc->delay_time = time;
	stc->begin = milisn;
	stc->open = True;
}

void run(Service *srv)
{
	if (srv->stopped)
		return;

	if (is_it_time(milisn, srv->loop_time, srv->ok))
	{
		srv->ok = False;
		(*srv->func)(srv->argv);
	}
	else if (isnt_it_time(milisn, srv->loop_time, srv->ok))
	{
		srv->ok = True;
	}
}

void Service_node_add(Service_node *head, Service serv)
{
	static int id = 0;
	if (head == 0)
		return;

	while (head->next != 0)
	{
		head = head->next;
	}

	head->serv = serv;
	head->id = id++;
	head->next = (Service_node *)malloc(sizeof(Service_node));
	head->next->next = 0;
	head->next->id = -1;
}

void Service_node_delete(Service_node **head, int id)
{
	Service_node *tmp = *head, *prev;

	if (tmp != 0 && tmp->id == id)
	{
		*head = tmp->next;
		free(tmp);
	}

	else
	{
		while (tmp != 0 && tmp->id != id)
		{
			prev = tmp;
			tmp = tmp->next;
		}

		if (tmp->next == 0)
			return;

		prev->next = tmp->next;

		free(tmp);
	}
}

Service_node *Service_node_init()
{
	Service_node *head = (Service_node *)malloc(sizeof(Service_node));
	head->next = 0;
	return head;
}

int Service_node_size(Service_node *head)
{
	int ret = 0;
	while (head->next != 0)
	{
		head = head->next;
		ret++;
	}
	return ret;
}

void Service_node_run(Service_node *head)
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

byte Task_node_size(Task_node *head)
{
	byte ret = 0;
	while (head->next != 0)
	{
		head = head->next;
		ret++;
	}
	return ret;
}

int Task_node_add(Task_node *head, Task task)
{
	static byte id = 0;
	if (head == 0)
		return -1;

	while (head->next != 0)
	{
		head = head->next;
	}

	head->id = id++;
	head->task = task;
	head->next = (Task_node *)malloc(sizeof(Task_node));
	head->next->next = 0;
	head->next->id = -1;

	return head->id;
}

void Task_node_delete(Task_node **head, int id)
{
	Task_node *tmp = *head, *prev;

	if (tmp != 0 && tmp->id == id)
	{
		*head = tmp->next;
		free(tmp);
	}

	else
	{
		while (tmp != 0 && tmp->id != id)
		{
			prev = tmp;
			tmp = tmp->next;
		}

		if (tmp->next == 0)
			return;

		prev->next = tmp->next;

		free(tmp);
	}
}

Task Task_node_pop(Task_node **head)
{
	Task_node *tmp = *head, *prev;
	*head = tmp->next;
	Task ret = tmp->task;
	free(tmp);
	return ret;
}

void Task_node_run(Task_node **head)
{
	while ((*head)->next != 0)
	{
		Task task = Task_node_pop(head);
		task.func(task.argv);
	}
}

void Task_node_loop_run(Task_node *head)
{
	while (head->next != 0)
	{
		head->task.func(head->task.argv);
		head = head->next;
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
#endif //SERVICES_C
