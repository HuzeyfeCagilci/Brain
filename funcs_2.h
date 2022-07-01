#ifndef FUNCS_2_H
#define FUNCS_2_H

#include <Arduino.h>
#include "funcs_1.h"
#include "sys.h"

_return_ set_blink(byte pin, uint64_t period, uint8_t count, _task_type_ type)
{
	if (System.pins[pin])
		return Pin_is_busy;

	pinMode(pin, OUTPUT);

	blink_stc *bs = (blink_stc *)malloc(sizeof(blink_stc));

	if (bs == 0)
		return Malloc_fail;

	bs->led = pin;

	Task_node_add(&System.tasks, Task_create(blink, Task_arg_create(bs, period), count, type));

	return Success;
}

#endif