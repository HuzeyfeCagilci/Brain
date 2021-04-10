#ifndef TASK_H
#define TASK_H

#include <stdlib.h>
#include "task_declarations.h"

INLINE void dly_init(delay_stc *stc)
{
	if (!stc->open)
		stc->run = True;
	else if ((_time_ > stc->begin && _time_ - stc->begin >= stc->delay_time) || (_time_ < stc->begin && _time_ - stc->begin + DV >= stc->delay_time))
	{
		stc->run = True;
		stc->delay_time = 0;
		stc->open = False;
	}
}

INLINE void mydelay(int time, delay_stc *stc)
{
	stc->run = False;
	stc->delay_time = time;
	stc->begin = _time_;
	stc->open = True;
}

INLINE void run(Service *srv)
{
	if (srv->stopped)
		return;

	if (is_it_time(_time_, srv->loop_time, srv->ok))
	{
		srv->ok = False;
		(*srv->func)(srv->argv);
	}
	else if (isnt_it_time(_time_, srv->loop_time, srv->ok))
	{
		srv->ok = True;
	}
}

INLINE Service_node *Service_node_init()
{
	Service_node *head = (Service_node *)malloc(sizeof(Service_node));
	head->next = 0;
	return head;
}

INLINE byte Task_node_size(Task_node *head)
{
	byte ret = 0;
	while (head->next != 0)
	{
		head = head->next;
		ret++;
	}
	return ret;
}

INLINE byte Service_node_size(Service_node *head)
{
	byte ret = 0;
	while (head->next != 0)
	{
		head = head->next;
		ret++;
	}
	return ret;
}

INLINE void Task_node_run(Task_node **head)
{
	while ((*head) != 0)
	{
		Task task = node_pop(head);
		task.func(task.argv);
	}
}

INLINE void Task_node_loop_run(Task_node *head)
{
	while (head != 0)
	{
		head->task.func(head->task.argv);
		head = head->next;
	}
}

INLINE void Service_node_run(Service_node *head)
{
	while (head != 0)
	{
		run(&head->serv);
		head = head->next;
	}
}

INLINE Task_node *Task_node_init()
{
	Task_node *head = (Task_node *)malloc(sizeof(Task_node));
	head->next = 0;
	return head;
}

INLINE Task Task_node_pop(Task_node **head)
{
	Task_node *tmp = *head, *prev;
	*head = tmp->next;
	Task ret = tmp->task;
	free(tmp);
	return ret;
}

INLINE byte Task_node_add(Task_node **head, Task task)
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

INLINE byte Service_node_add(Service_node **head, Service serv)
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


INLINE Node_return Task_node_delete(Task_node **head, byte id)
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

INLINE Node_return Service_node_delete(Service_node **head, byte id)
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

INLINE void _timer1_set_up(int hz)
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

#endif
#endif // TASK.H
