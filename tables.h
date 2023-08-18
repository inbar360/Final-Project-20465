#include "globals.h"

typedef struct Data_Table Data_Table;
typedef struct Ent_Table Ent_Table;
typedef struct Ext_Table Ext_Table;

#define LABEL_LENGTH 32

/***** Data_Table methods: *****/

/* This function creates a variable of type Data_Table and returns a pointer to it.
   returns: a pointer to the new variable, or NULL if ran into memory allocation errors. */
struct Data_Table *create_data_table();

/* This function sets the value of the current table to the given value. */
void setDataValue(struct Data_Table *table, int value);

/* This function sets the length of the current table to the given length. */
void setDataLength(struct Data_Table *table, int length);

/* This function sets the type of the current table to the given type. */
void setDataType(struct Data_Table *table, char type);

/* This function sets the data of the current table to the given data. */
void setDataData(struct Data_Table *table, char data[]);

/* This function sets the array of strings of the current table to the given array. */
void setDataBinary(struct Data_Table *table, char **binary);

/* This function sets the next of the current table to a new variable of type Data_Table using the create_data_table method. */
void setDataNext(struct Data_Table *table);

/* This function returns the value of the current table. */
int getDataValue(struct Data_Table *table);

/* This function returns the length of the current table. */
int getDataLength(struct Data_Table *table);

/* This function returns the type of the current table. */
char getDataType(struct Data_Table *table);

/* This function returns the data of the current table. */
char *getDataData(struct Data_Table *table);

/* This function returns the machine words of the current table. */
char **getDataBinary(struct Data_Table *table);

/* This function returns the next of the current table. 
   returns NULL if ran into memory allocation issues. */
struct Data_Table *getDataNext(struct Data_Table *table);

/* This function frees each Data_Table nodes attributes, and then the node itself. */
void free_data_table(struct Data_Table **head);

/***** Ent_Table methods: *****/

/* This function creates a variable of type Ent_Table and returns a pointer to it.
   returns: a pointer to the new variable, or NULL if ran into memory allocation errors. */
struct Ent_Table *create_ent_table();

/* This function sets the value of the current table to the given value. */
void setEntValue(struct Ent_Table *table, int value);

/* This function sets the data of the current table to the given data. */
void setEntData(struct Ent_Table *table, char data[]);

/* This function sets the next of the current table to a new variable of type Ent_Table using the create_ent_table method. */
void setEntNext(struct Ent_Table *table);

/* This function returns the value of the current table. */
int getEntValue(struct Ent_Table *table);

/* This function returns the data of the current table. */
char *getEntData(struct Ent_Table *table);

/* This function returns the next of the current table. 
   returns NULL if ran into memory allocation issues. */
struct Ent_Table *getEntNext(struct Ent_Table *table);

/* This function frees each Ent_Table nodes attributes, and then the node itself. */
void free_ent_table(struct Ent_Table **head);

/***** Ext_Table methods: *****/

/* This function creates a variable of type Ext_Table and returns a pointer to it.
   returns: a pointer to the new variable, or NULL if ran into memory allocation errors. */
struct Ext_Table *create_ext_table();

/* This function sets the value of the current table to the given value. */
void setExtValue(struct Ext_Table *table, int value);

/* This function sets the data of the current table to the given data. */
void setExtData(struct Ext_Table *table, char data[]);

/* This function sets the next of the current table to a new variable of type Ext_Table using the create_ext_table method. */
void setExtNext(struct Ext_Table *table);

/* This function returns the value of the current table. */
int getExtValue(struct Ext_Table *table);

/* This function returns the data of the current table. */
char *getExtData(struct Ext_Table *table);

/* This function returns the next of the current table. 
   returns NULL if ran into memory allocation issues. */
struct Ext_Table *getExtNext(struct Ext_Table *table);

/* This function frees each Ext_Table nodes attributes, and then the node itself. */
void free_ext_table(struct Ext_Table **head);