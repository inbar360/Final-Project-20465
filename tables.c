#include "tables.h"

#include "utils.h"



/**** Structs definitions: ****/

struct Data_Table{

    int value, length;

    char type;

    char data[LABEL_LENGTH];

    char binary[MAX_MEMORY][BITS+1];

    struct Data_Table *next;

};



/**** Create_Table method: ****/



struct Data_Table *create_table() {

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



/**** Data_Table methods: ****/



void setValue(struct Data_Table *table, int value) {

    table->value = value;

}



void setLength(struct Data_Table *table, int length) {

    table->length = length;

}



void setType(struct Data_Table *table, char type) {

    table->type = type;

}



void setData(struct Data_Table *table, char data[]) {

    int i;

    for (i = 0; i < strlen(data); i++) {

        table->data[i] = data[i];

    }

}



void setBinary(struct Data_Table *table, char binary[1024][BITS+1], int len) {

    int i;
    printf("bin[0] = %s, bin[1] = %s\n", binary[0], binary[1]);

    for (i = 0; i < len; i++) {
    	printf("binary[i]: '%s', %d, %d\n", binary[i], strlen(binary[i]), !table);
    	table->binary[i][BITS] = '\0';
    	

        strcpy(table->binary[i], binary[i]);
        printf("after: '%s'\n", table->binary[i]);

    }

}



void setNext(struct Data_Table *table, struct Data_Table *add) {

    if (!add) table->next = NULL;

    table->next = add;

}



int getValue(struct Data_Table *table) {

    return table->value;

}



int getLength(struct Data_Table *table) {

    return table->length;

}



char getType(struct Data_Table *table) {

    return table->type;

}



char *getData(struct Data_Table *table) {

    return table->data;

}



char (*getBinary(struct Data_Table *table))[BITS+1] {

    return table->binary;

}



struct Data_Table *getNext(struct Data_Table *table) {

    return table->next;

}



/**** Free Table method: ****/



void free_data_table(struct Data_Table **head) {

    struct Data_Table *next;



    while (*head != NULL) {

        next = (*head)->next;

        free(*head);

        *head = next;

    }

}