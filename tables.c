#include "tables.h"
#include "utils.h"

/**** Structs definitions: ****/
struct Data_Table{
    int value, length;
    char type;
    char data[LABEL_LENGTH];
    char binary[MAX_MEMORY][BITS];
    struct Data_Table *next;
}Data_Table;

struct Ent_Table{
    int value;
    char data[LABEL_LENGTH];
    struct Ent_Table *next;
}Ent_Table;

struct Ext_Table{
    int value;
    char data[LABEL_LENGTH];
    struct Ext_Table *next;
}Ext_Table;

/**** Create_Table methods: ****/

struct Data_Table *create_data_table() {
    struct Data_Table *table = (struct Data_Table *)malloc(sizeof(struct Data_Table));
    if (!table) return NULL;
    table->length = 0;
    table->value = 0;
    table->type = '\0';
    memset(table->data, '\0', LABEL_LENGTH);
    memset(table->binary, 0, sizeof(table->binary));
    table->next = NULL;
    return table;
}

struct Ent_Table *create_ent_table() {
    struct Ent_Table *table = (struct Ent_Table *)malloc(sizeof(struct Ent_Table));
    if (!table) return NULL;
    table->value = 0;
    memset(table->data, '\0', LABEL_LENGTH);
    table->next = NULL;
    return table;
}

struct Ext_Table *create_ext_table() {
    struct Ext_Table *table = (struct Ext_Table *)malloc(sizeof(struct Ext_Table));
    if (!table) return NULL;
    table->value = 0;
    memset(table->data, '\0', LABEL_LENGTH);
    table->next = NULL;
    return table;
}

/**** Data_Table methods: ****/

void setDataValue(struct Data_Table *table, int value) {
    table->value = value;
}

void setDataLength(struct Data_Table *table, int length) {
    table->length = length;
}

void setDataType(struct Data_Table *table, char type) {
    table->type = type;
}

void setDataData(struct Data_Table *table, char data[]) {
    int i;
    for (i = 0; i < LABEL_LENGTH; i++) {
        table->data[i] = data[i];
    }
}

void setDataBinary(struct Data_Table *table, char **binary) {
    int i;

    for (i = 0; i < strlen(binary); i++) {
        strcpy(table->binary[i], binary[i]);
    }
}

void setDataNext(struct Data_Table *table) {
    table->next = create_data_table();
}

int getDataValue(struct Data_Table *table) {
    return table->value;
}

int getDataLength(struct Data_Table *table) {
    return table->length;
}

char getDataType(struct Data_Table *table) {
    return table->type;
}

char *getDataData(struct Data_Table *table) {
    return table->data;
}

char **getDataBinary(struct Data_Table *table) {
    return table->binary;
}

struct Data_Table *getDataNext(struct Data_Table *table) {
    return table->next;
}

/**** Ent_Table methods: ****/

void setEntValue(struct Ent_Table *table, int value) {
    table->value = value;
}

void setEntData(struct Ent_Table *table, char data[]) {
    int i;
    for (i = 0; i < LABEL_LENGTH; i++) {
        table->data[i] = data[i];
    }
}

void setEntNext(struct Ent_Table *table) {
    table->next = create_ent_table();
}

int getEntValue(struct Ent_Table *table) {
    return table->value;
}

char *getEntData(struct Ent_Table *table) {
    return table->data;
}

struct Ent_Table *getEntNext(struct Ent_Table *table) {
    return table->next;
}

/**** Ext_Table methods: ****/

void setExtValue(struct Ext_Table *table, int value) {
    table->value = value;
}

void setExtData(struct Ext_Table *table, char data[]) {
    int i;
    for (i = 0; i < LABEL_LENGTH; i++) {
        table->data[i] = data[i];
    }
}

void setExtNext(struct Ext_Table *table) {
    table->next = create_ext_table();
}

int getExtValue(struct Ext_Table *table) {
    return table->value;
}

char *getExtData(struct Ext_Table *table) {
    return table->data;
}

struct Ext_Table *getExtNext(struct Ext_Table *table) {
    return table->next;
}

/**** Free Table methods: ****/

void free_data_table(struct Data_Table **head) {
    int i;
    struct Data_Table *next;

    while (*head != NULL) {
        next = (*head)->next;
        for (i = 0; i < strlen((*head)->binary); i++) {
            free((*head)->binary[i]);
        }
        *head = next;
    }
}

void free_ent_table(struct Ent_Table **head) {
	/* Variable: next - used to save head's next each iteration of the while loop. */
	struct Ent_Table *next;
	/* Goes over the table starting with head.
	   Each iteration, sets next's pointer to heads values next, free's heads values attributes, 
	   then heads value itself, and finally sets heads value to point to next. */
	while(*head != NULL) {
		next = (*head)->next; 
		free((*head)->value);
		free(*head);
		*head = next;
	}
}

void free_ext_table(struct Ext_Table **head) {
	/* Variable: next - used to save head's next each iteration of the while loop. */
	struct Ext_Table *next;
	/* Goes over the table starting with head.
	   Each iteration, sets next's pointer to heads values next, free's heads values attributes, 
	   then heads value itself, and finally sets heads value to point to next. */
	while(*head != NULL) {
		next = (*head)->next; 
		free((*head)->value);
		free(*head);
		*head = next;
	}
}
