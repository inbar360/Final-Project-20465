#ifndef _PREPROCESS_H

#define _PREPROCESS_H



#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "globals.h"

#include "utils.h"

#include "macro.h"



#define MAX_LINE 81



boolean preprocess(FILE *file, char *name, struct Macro_Table **head, FILE *to);





#endif