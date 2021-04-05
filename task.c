#ifndef TASK_C
#define TASK_C

#include <stdlib.h>
#include "task.h"

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

Service_node *Service_node_init()
{
	Service_node *head = (Service_node *)malloc(sizeof(Service_node));
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

byte Service_node_size(Service_node *head)
{
	byte ret = 0;
	while (head->next != 0)
	{
		head = head->next;
		ret++;
	}
	return ret;
}

void Task_node_run(Task_node **head)
{
	while ((*head) != 0)
	{
		Task task = node_pop(head);
		task.func(task.argv);
	}
}

void Task_node_loop_run(Task_node *head)
{
	while (head != 0)
	{
		head->task.func(head->task.argv);
		head = head->next;
	}
}

void Service_node_run(Service_node *head)
{
	while (head != 0)
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

Task Task_node_pop(Task_node **head)
{
	Task_node *tmp = *head, *prev;
	*head = tmp->next;
	Task ret = tmp->task;
	free(tmp);
	return ret;
}

byte Task_node_add(Task_node **head, Task task)
{
	Task_node *new_node = (Task_node *)malloc(sizeof(Task_node));
	if (new_node == 0)
		return Malloc_fail;
	new_node->task = task;
	new_node->next = *head;
	new_node->id = ++__id__;
	*head = new_node;
	return new_node->id;
}

byte Service_node_add(Service_node **head, Service serv)
{
	Service_node *new_node = (Service_node *)malloc(sizeof(Service_node));
	if (new_node == 0)
		return Malloc_fail;
	new_node->serv = serv;
	new_node->next = *head;
	new_node->id = ++__id__;
	*head = new_node;
	return new_node->id;
}


Node_return Task_node_delete(Task_node **head, byte id)
{
	Task_node *tmp;

	if((*head)->id==id)
	{
		if(*head==NULL) return Cannot_find;
		tmp=*head;
		*head=tmp->next;
		free(tmp);
		return Success;
	}

	tmp=*head;
	Task_node *prev;

	while(tmp->id != id)
	{
		if(tmp==NULL) return Cannot_find;
		prev=tmp;
		tmp=tmp->next;
	}

	prev->next=tmp->next;
	free(tmp);

	return Success;
}

Node_return Service_node_delete(Service_node **head, byte id)
{
	Service_node *tmp;

	if((*head)->id==id)
	{
		if(*head==NULL) return Cannot_find;
		tmp=*head;
		*head=tmp->next;
		free(tmp);
		return Success;
	}

	tmp=*head;
	Service_node *prev;

	while(tmp->id != id)
	{
		if(tmp==NULL) return Cannot_find;
		prev=tmp;
		tmp=tmp->next;
	}

	prev->next=tmp->next;
	free(tmp);

	return Success;
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
