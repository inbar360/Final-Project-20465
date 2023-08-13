#ifndef _TABLE_H
#define _TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

typedef struct Mcro_Table Mcro_Table;

struct Macro_Table *create_table();
void setName (struct Macro_Table *table, char *name);
void setValue (struct Macro_Table *table, char *value);
void setNext (struct Macro_Table *table);
char *getName(struct Macro_Table *table);
char *getValue(struct Macro_Table *table);
struct Macro_Table *getNext (struct Macro_Table *table);
boolean name_exists(struct Macro_Table *head, char *name);
char *find_macro_val(struct Macro_Table *head, char *name);
void free_table(struct Macro_Table **head);

#endif