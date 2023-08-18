#ifndef _MACRO_H
#define _MACRO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

typedef struct Mcro_Table Mcro_Table;

/* This function creates a new variable of type Macro_Table, and returns a pointer to it.
   returns: a pointer to the new variable, or NULL if ran into memory allocation issues. */
struct Macro_Table *create_table();

/* This function sets the name of the current table to the given name. */
void setName (struct Macro_Table *table, char *name);

/* This function sets the value of the current table to the given value.  */
void setValue (struct Macro_Table *table, char *value);

/* This function sets the next of the current table to a new variable of type Macro_Table using the create_table method. */
void setNext (struct Macro_Table *table);

/* This function returns the name of the current table. */
char *getName(struct Macro_Table *table);

/* This function returns the value of the current table. */
char *getValue(struct Macro_Table *table);

/* This function returns the next of the current table. */
struct Macro_Table *getNext (struct Macro_Table *table);

/* This function checks if the given name exists in the Macro_Table list, as a tables name. 
   returns: TRUE if found the name in the list, FALSE otherwise. */
boolean name_exists(struct Macro_Table *head, char *name);

/* This function finds the value of the table with the given name.
   returns: the value of the table with the wanted name. */
char *find_macro_val(struct Macro_Table *head, char *name);

/* This function frees each Macro_Table nodes attributes and then the node itself. */
void free_table(struct Macro_Table **head);

#endif