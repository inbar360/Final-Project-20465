#ifndef _MACRO_H
#define _MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "utils.h"
#include "table.h"

#define MAX_LINE 81
typedef struct Macro_Table Macro_Table;

boolean preprocess(FILE *file, char *name, struct Macro_Table **head, FILE *to);


#endif