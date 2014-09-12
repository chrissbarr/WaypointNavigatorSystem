/*
 * debug.h
 *
 * Created: 6/09/2014 5:02:23 PM
 *  Author: Chris
 */ 

#include "usart.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

void debug_print(const char* debugString);
void debug_printi(int debugNum);
void debug_printf(float debugNum);