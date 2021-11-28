#ifndef Tash_h
#define Tash_h

#include <stdlib.h>
#include "task_dec.h"

INLINE Task Task_create(void (*func)(void *), void *argv, uint16_t count, _task_type_ type)
{
	Task task = {func, argv, count, type};
	return task;
}

INLINE Task_arg *Task_arg_create(void *argv, uint64_t period)
{
	Task_arg *task_arg = (Task_arg *)malloc(sizeof(Task_arg));

	if (task_arg == 0)
		return NULL;

	task_arg->argv = argv;
	task_arg->period = period;
	task_arg->last = 0;
	task_arg->turn = 1;

	return task_arg;
}

INLINE bool check_time(Task_arg *targ)
{
	if (targ->last <= _time_)
	{
		if (targ->turn)
		{
			if (_time_ - targ->last >= targ->period)
			{
				targ->turn = false;
				targ->last = _time_;
				return true;
			}
		}
		else
		{
			if (_time_ - targ->last < targ->period)
			{
				targ->turn = true;
				return false;
			}
		}
	}
	else
	{
		if (targ->turn)
		{
			if (_time_ + (DV - targ->last) >= targ->period)
			{
				targ->turn = false;
				targ->last = _time_;
				return true;
			}
		}
		else
		{
			if (_time_ + (DV - targ->last) < targ->period)
			{
				targ->turn = true;
				return false;
			}
		}
	}

	return false;
}

INLINE uint8_t Task_node_add(Task_node **head, Task task)
{
	Task_node *tmp = *head, *new_node = (Task_node *)malloc(sizeof(Task_node));

	if (new_node == 0)
		return 0;

	while (tmp != 0)
	{
		if (tmp->id == __id__)
		{
			__id__++;
		}
		tmp = tmp->next;
	}

	new_node->task = task;
	new_node->next = *head;
	new_node->id = ++__id__;
	*head = new_node;
	return new_node->id;
}

INLINE uint8_t Task_node_size(Task_node *head)
{
	uint8_t i = 0;
	while (head != 0)
	{
		head = head->next;
		i++;
	}

	return i;
}

INLINE _return_ Task_node_delete(Task_node **head, byte id)
{
	Task_node *tmp;

	if ((*head)->id == id)
	{
		if (*head == NULL)
			return Cannot_find;

		tmp = *head;
		*head = tmp->next;

		switch (tmp->task.type)
		{
		case Basic_Task:
			free(tmp->task.argv);
			break;

		case Scheduled_Task:
			free(((Task_arg *)tmp->task.argv)->argv);
			free(tmp->task.argv);

		default:
			break;
		}
		free(tmp);

		return Success;
	}

	tmp = *head;
	Task_node *prev;

	while (tmp->id != id)
	{
		if (tmp == NULL)
			return Cannot_find;
		prev = tmp;
		tmp = tmp->next;
	}

	prev->next = tmp->next;

	switch (tmp->task.type)
	{
	case Basic_Task:
		free(tmp->task.argv);
		break;

	case Scheduled_Task:
		free(((Task_arg *)tmp->task.argv)->argv);
		free(tmp->task.argv);

	default:
		break;
	}
	free(tmp);

	return Success;
}

INLINE _return_ Task_node_run(Task_node **head)
{
	Task_node *node = *head;

	_return_ ret = Success;

	while (node != 0)
	{
		switch (node->task.type)
		{
		case Basic_Task:
			node->task.func(node->task.argv);
			node->task.count--;
			break;

		case Scheduled_Task:
			if (check_time((Task_arg *)node->task.argv))
			{
				node->task.func(((Task_arg *)node->task.argv)->argv);
				node->task.count--;
			}
			break;

		case Endless_Task:
			node->task.func(node->task.argv);
			break;

		case Scheduled_Endless_Task:
			if (check_time((Task_arg *)node->task.argv))
				node->task.func(((Task_arg *)node->task.argv)->argv);
			break;

		default:
			// Serial.println(F("default"));

			break;
		}

		if (node->task.count <= 0)
		{
			ret = Task_node_delete(head, node->id);
			/*if (Task_node_delete(head, node->id) == Success)
			{
				Serial.print(F("Deleted Task_node whose id is "));
				Serial.println(node->id);
			}*/
			// info(NULL);
			// print_task_node(*head);
		}

		node = node->next;
	}

	return ret;
}

INLINE void Task_node_config(Task_node **head)
{
	(*head)->id = 0;
	(*head)->next = 0;
}

INLINE void dly_init(delay_stc *stc)
{
	if (!stc->open)
		stc->run = true;
	else if ((_time_ > stc->begin && _time_ - stc->begin >= stc->delay_time) || (_time_ < stc->begin && _time_ - stc->begin + DV >= stc->delay_time))
	{
		stc->run = true;
		stc->delay_time = 0;
		stc->open = false;
	}
}

INLINE void mydelay(long time, delay_stc *stc)
{
	stc->run = false;
	stc->delay_time = time;
	stc->begin = _time_;
	stc->open = true;
}

#endif // Tash_h
