#ifndef SYS_DEC_H
#define SYS_DEC_H

#include "task.h"

struct system
{
	Task_node * tasks;
	Task_node * tmp;
	bool pins [16];
}_atr_;

#endif