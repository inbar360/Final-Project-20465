#include "macro.h"



#define COMP(name, op) (strcmp(name, op) == 0)



struct Macro_Table {

    char *name; /* The macro name. */

	char *value; /* The macro value. */

	struct Macro_Table *next; /* A pointer to the next macro in the list. */

};



struct Macro_Table *create_mtable() {

    struct Macro_Table *table = (struct Macro_Table *)malloc(sizeof(struct Macro_Table));

    if (!table) return NULL; /* If the created variable is NULL, meaning memory allocation errors, return NULL. */

    table->name = NULL;

    table->value = NULL;

    table->next = NULL;

    return table; /* Set all attributes to NULL and return the pointer. */

}



void setmName (struct Macro_Table *table, char *name) {

	if (!name) {

		table->name = NULL;

		return;

	}

	table->name = (char *)malloc((strlen(name)+1)*sizeof(char));

	if (!table->name) { /* If the memory allocation failed, exit. */

		printf("Error: Memory allocation failed.\n");

		exit(1);

	}

	strcpy(table->name, name); /* Copy the given name to table's name. */

}



void setmValue (struct Macro_Table *table, char *value) {

	table->value = (char *)malloc((strlen(value)+1)*sizeof(char));

	if (!table->value) { /* If the memory allocation failed, exit. */

		printf("Error: Memory allocation failed.\n");

		exit(1);

	}

    strcpy(table->value, value); /* Copy the given value to table's value. */

}



void setmNext (struct Macro_Table *table) {

    table->next = create_mtable(); /* Set table's next using the create_table method. */

}



char *getmName(struct Macro_Table *table) {

    return table->name; /* Returns the name. */

}



char *getmValue(struct Macro_Table *table) {

    return table->value; /* Returns the value. */

}



struct Macro_Table *getmNext (struct Macro_Table *table) {

    return table->next; /* Returns the pointer to the next macro. */

}



boolean name_exists(struct Macro_Table *head, char *name) {

    /* Going over the list, checking if the name already exists. */

	while (head && head->name) {

		if (strcmp(head->name, name) == 0) return TRUE;

		head = head->next;

	}



	/* Returning 0 if the name does not already exist in the list. */

	return FALSE;

}



char *find_macro_val(struct Macro_Table *head, char *name) {

    /* Going over the list, if the given name is equal to the macro name, return it's value. */

    while (head && head->name) {

        if (strcmp(head->name, name) == 0) return head->value;

        head = head->next; /* Point to the next macro. */

    }



    return NULL;

}



void free_mtable(struct Macro_Table **head) {

	/* Variable: next - used to save head's next each iteration of the while loop. */

	struct Macro_Table *next;

	/* Goes over the table starting with head.

	   Each iteration, sets next's pointer to heads values next, free's heads values attributes, 

	   then heads value itself, and finally sets heads value to point to next. */

	while(*head != NULL) {

		next = (*head)->next; 

		free((*head)->name);

		free((*head)->value);

		free(*head);

		*head = next;

	}

}



