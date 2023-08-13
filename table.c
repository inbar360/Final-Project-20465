#include "table.h"

struct Macro_Table {
    char *name; 
	char *value;
	struct Macro_Table *next;
};

struct Macro_Table *create_table() {
    struct Macro_Table *table = (struct Macro_Table *)malloc(sizeof(struct Macro_Table));
    if (!table) return NULL;
    table->name = NULL;
    table->value = NULL;
    table->next = NULL;
    return table;
}

void setName (struct Macro_Table *table, char *name) {
	printf("setting the name to: %s\n", name);
	table->name = (char *)malloc((strlen(name)+1)*sizeof(char));
	if (!table->name) {
		printf("Error: Memory allocation failed.\n");
		exit(1);
	}
	strcpy(table->name, name);
    printf("name is: \"%s\"\n", table->name);
}

void setValue (struct Macro_Table *table, char *value) {
	table->value = (char *)malloc((strlen(value)+1)*sizeof(char));
	if (!table->value) {
		printf("Error: Memory allocation failed.\n");
		exit(1);
	}
    strcpy(table->value, value);
}

void setNext (struct Macro_Table *table) {
    table->next = create_table();
}

char *getName(struct Macro_Table *table) {
    return table->name;
}

char *getValue(struct Macro_Table *table) {
    return table->value;
}

struct Macro_Table *getNext (struct Macro_Table *table) {
    return table->next;
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
    while (head && head->name) {
        if (strcmp(head->name, name) == 0) return head->value;
        head = head->next;
    }

    return NULL;
}

void free_table(struct Macro_Table **head) {
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
