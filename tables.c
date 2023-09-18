#include "tables.h"

#include "utils.h"



/**** Structs definitions: ****/

struct Data_Table{

    int value, length;

    char type;

    char *data;

    char *binary[MAX_MEMORY];

    struct Data_Table *next;

};



/**** Create_Table method: ****/



struct Data_Table *create_table() {

    struct Data_Table *table;

    int i;

    table = (struct Data_Table *)malloc(sizeof(struct Data_Table));

    

    if (!table) return NULL;

    table->length = 0;

    table->value = 0;

    table->type = '\0';

    table->data = NULL;

    for (i = 0; i < MAX_MEMORY; i++) {

    	table->binary[i] = NULL;

    }

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



void setData(struct Data_Table *table, char *data) {

    if (!data) {

		table->data = NULL;

		return;

	}

	table->data = (char *)malloc((strlen(data)+1)*sizeof(char));

	if (!table->data) { /* If the memory allocation failed, exit. */

		printf("Error: Memory allocation failed.\n");

		exit(1);

	}

	strcpy(table->data, data); /* Copy the given name to table's name. */

}



void setBinary(struct Data_Table *table, char binary[1024][BITS+1], int len) {

    int i;

    

    for (i = 0; i < len; i++) {

    	table->binary[i] = (char *)malloc(BITS+1);

    	if(!table->binary[i]) {

    		printf("Error: Memory allocation failed.\n");

    		exit(1);

    	}

    	table->binary[i][BITS] = '\0';

        strcpy(table->binary[i], binary[i]);

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



char **getBinary(struct Data_Table *table) {

    return table->binary;

}



struct Data_Table *getNext(struct Data_Table *table) {

    return table->next;

}



/**** Free Table method: ****/



void free_data_table(struct Data_Table **head) {

    struct Data_Table *next;

    int i;



    while (*head != NULL) {

    	i = 0;

    

        next = (*head)->next;

        free((*head)->data);

        while ((*head)->binary[i]) {

        	free((*head)->binary[i]);

        	i++;

        }

         

        free(*head);

        *head = next;

    }

}





/**** Other functions that are used regarding the data table: ****/

boolean in_list(struct Data_Table *dt, char *str) {

    struct Data_Table *dt1= dt;



    while(dt1 && dt1->data) { /* While dt1 is not NULL */

        if(strcmp(dt1->data, str) == 0) { /* If the string appears in the list, return TRUE. */

            return TRUE;

        }

        dt1 = dt1->next; /* Set dt1 to the next in the list. */

    }

    return FALSE; /* Return FALSE if did not encounter str in the list. */

}

