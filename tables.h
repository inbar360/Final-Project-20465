#ifndef _TABLES_H

#define _TABLES_H

#include "globals.h"



typedef struct Data_Table Data_Table;



#define LABEL_LENGTH 32



/***** Data_Table methods: *****/



/* This function creates a variable of type Data_Table and returns a pointer to it.

   returns: a pointer to the new variable, or NULL if ran into memory allocation errors. */

struct Data_Table *create_table();



/* This function sets the value of the current table to the given value. */

void setValue(struct Data_Table *table, int value);



/* This function sets the length of the current table to the given length. */

void setLength(struct Data_Table *table, int length);



/* This function sets the type of the current table to the given type. */

void setType(struct Data_Table *table, char type);



/* This function sets the data of the current table to the given data. */

void setData(struct Data_Table *table, char data[]);



/* This function sets the array of strings of the current table to the given array. */

void setBinary(struct Data_Table *table, char binary[1024][BITS+1], int len);



/* This function sets the next of the current table to the data_table variable add. */

void setNext(struct Data_Table *table, struct Data_Table *add);



/* This function returns the value of the current table. */

int getValue(struct Data_Table *table);



/* This function returns the length of the current table. */

int getLength(struct Data_Table *table);



/* This function returns the type of the current table. */

char getType(struct Data_Table *table);



/* This function returns the data of the current table. */

char *getData(struct Data_Table *table);



/* This function returns the machine words of the current table. */

char (*getBinary(struct Data_Table *table))[BITS];



/* This function returns the next of the current table. 

   returns NULL if ran into memory allocation issues. */

struct Data_Table *getNext(struct Data_Table *table);



/* This function frees each Data_Table nodes attributes, and then the node itself. */

void free_data_table(struct Data_Table **head);



#endif