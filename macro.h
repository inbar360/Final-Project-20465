#ifndef _MACRO_H
#define _MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Macro_Table table;

int preprocess(FILE *file, char *name);
void free_table(table **head);


#endif