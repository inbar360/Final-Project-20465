#ifndef _GLOBALS_H

#define _GLOBALS_H

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>



#define MAX_LINE 81

#define TWO 2

#define THREE 3

#define FOUR 4

#define BITS 12

#define MAX_MEMORY 1024

#define LABEL_LENGTH 32

#define MAX 2047

#define MIN -2048

#define IC_INIT_VAL 100

#define SKIP_REST_OF_LINE(file, c) \
	for(;c && c != '\n'; c = fgetc(file));

typedef enum {

  FALSE = 0, TRUE = 1

} boolean;



typedef enum registers{

	r0 = 0,

	r1,

	r2,

	r3,

	r4,

	r5,

	r6,

	r7,

	NON_REG = -1

} reg;



#endif