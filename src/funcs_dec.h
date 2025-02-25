#ifndef FUNCS_DEC_H
#define FUNCS_DEC_H

#include <Arduino.h>

int freeRam();
void blink(void *arg);
void info(void *);
void print_task_node(void *argv);
void print_task(void *argv);
_return_ set_blink(byte pin, u32 period, u8 count, _task_type_ type);

extern void *__data_end;
extern void *__bss_end;

struct blink_struct
{
	byte led;
	_Bool ok;
} _atr_;

typedef struct blink_struct blink_stc;

#endif // FUNCS_DEC_H