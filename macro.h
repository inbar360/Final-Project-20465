#ifndef _MACRO_H
#define _MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80
typedef struct Macro_Table Macro_Table;

int preprocess(FILE *file, char *name, Macro_Table **head);
void free_table(Macro_Table **head);
char *strcat_name(char *file_name, char *to_add);

#endif