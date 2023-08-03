
/* Assumptions:
	- The macro definition must end.
	- There are no nested macro definitions.
	- Macro can be called only in the first segment of the line. */

#include "macro.h"

#define MAX_LINE 80

#define IS_WHITE(string, index) (string[index] && (string[index] == ' ' || string[index] == '\t'))
#define END_CHAR(string, index) (string[index] == '\n' || string[index] == '\0')
#define SKIP_WHITE(string, index) \
        for (;string[(index)] && (IS_WHITE(string, index)); (++(index)));   
#define SKIP_NON_WHITE(string, index) \
		for (;string[(index)] && !IS_WHITE(string, index) && !END_CHAR(string, index); (++(index)));
#define REMOVE_FILE(file, name) \
		fclose(file); \
		remove(name);

typedef struct Macro_Table {
	char *name;
	char *value;
	struct Macro_Table *next;
} Macro_Table;



int word_length(char *str, int start_idx);

int save_lines_in_macro_table(FILE *file, Macro_Table **table);

int check_macro_name(char *name, Macro_Table *table);

char *get_macro_val(char *name, Macro_Table *table);

int preprocess(FILE *file, char *name) {
	
	/* Opening the file with the given name, and a file to which the lines will be printed. */
	FILE *to = NULL;
	char line[80];
	char *macro_name = NULL, *macro_val = NULL;
	char *name_of_new_file = name;
	int i = 0, end, macro_name_len, cur_len, extra, result = 0, saved = 1;
	Macro_Table *table = NULL, *head = NULL;
	strcat(name_of_new_file, ".am");
	to = fopen(name_of_new_file, "w+");
	/* if the file did not open, print error, close the file and exit. */
	if (!to) {
		printf("Error: could not open file.\n");
		REMOVE_FILE(to, name_of_new_file);
		return -1;
	}

	head = (Macro_Table *)malloc(sizeof(Macro_Table));
	table = (Macro_Table *)malloc(sizeof(Macro_Table));
	if (!head) {
		printf("Error: memory allocation failed.\n");
		REMOVE_FILE(to, name_of_new_file);
		return -1;
	}
	if (!table) {
		printf("Error: memory allocation failed.\n");
		free(head);
		REMOVE_FILE(to, name_of_new_file);
		return -1;
	}
	head = table;
	memset(line, '\0', 80);
	while(fgets(line, 80, file)) {
		i = 0;
		if (line[strlen(line)-1] != '\n' && !feof(file) && *line != '\n') {
			printf("Error: Line is too long, max %d characters.\n", MAX_LINE);
			REMOVE_FILE(to, name_of_new_file);
			return 0;
		}
		SKIP_WHITE(line, i);
		if (END_CHAR(line, i) || line[i] == ';') continue;
		else if(strncmp(line+i, "mcro", 4) == 0 && (IS_WHITE(line, i+4))) {
			i+=4;
			SKIP_WHITE(line, i);
			
			macro_name_len = word_length(line+i, i);
			extra = i;
			SKIP_NON_WHITE(line, extra);
			/* checking if there are other characters after mcro + macro name. */
			SKIP_WHITE(line, extra);
			if (*(line+extra) != '\0' && *(line+extra) != '\n') {
				printf("Error: Extraneous text after macro name.\n");
				REMOVE_FILE(to, name_of_new_file);
				return 0;
			}
			macro_name = (char *)calloc((macro_name_len+1), sizeof(char));
			if (!macro_name) {
				printf("Error: Memory allocation failed.\n");
				free(head);
				free(table);
				REMOVE_FILE(to, name_of_new_file);
				return -1;
			}
			strncpy(macro_name, line+i, macro_name_len);
			result = check_macro_name(macro_name, head);
			if (result == 1) {
				printf("Error: Macro name already exists.\n");
				REMOVE_FILE(to, name_of_new_file);
				return 0;
			}

			/* Allocating memory for tables attributes. */
			cur_len += end;
			table->value = (char *)malloc(sizeof(char));
			table->name = (char *)malloc((macro_name_len+1) * sizeof(char));
			table->next = (Macro_Table *)malloc(sizeof(Macro_Table));

			if (!(table->value)) {
				printf("Error: memory allocation failed.");
				free(macro_name);
				free(head);
				free(table);
				REMOVE_FILE(to, name_of_new_file);
				return -1;
			}

			if (!(table->name)) {
				printf("Error: memory allocation failed.");
				free(macro_name);
				free(head);
				free(table->value);
				free(table);
				REMOVE_FILE(to, name_of_new_file);
				return -1;
			}

			if(!(table->next)) {
				printf("Error: memory allocation failed.");
				free(macro_name);
				free(head);
				free(table->value);
				free(table->name);
				free(table);
				REMOVE_FILE(to, name_of_new_file);
				return -1;
			}

			/* By this point we know the macro name does not appear in the macro table, so we add it. */
			
			strcpy(table->name, macro_name);
			saved = save_lines_in_macro_table(file, &table);
			if (saved == 0) {
				REMOVE_FILE(to, name_of_new_file);
				return 0;
			}
			else if (saved == -1) {
				REMOVE_FILE(to, name_of_new_file);
				return -1;
			}

			/* pointing to the next node in the list and freeing macro_name. */
			table = table->next;
			free(macro_name);
		}

		else {
			end = i;
			SKIP_NON_WHITE(line, end);
			macro_name = (char *)malloc((end-i) * sizeof(char));
			if (!macro_name) {
				printf("Error: Memory allocation failed.\n");
				free(head);
				free(table);
				REMOVE_FILE(to, name_of_new_file);
				return -1;
			}
			strncpy(macro_name, (line+i), end-i);
			if ((macro_val = get_macro_val(macro_name, head)) != NULL) {
				fputs(macro_val, to);
			}
			
			else {
				fputs(line, to);
			}
			macro_name = NULL;
			free(macro_name);
		}
	}
	free_table(&head);
	/* closing the file. */
	fclose(to);
	return 1;
}

int word_length(char *str, int start_idx) {
	int i;
	for(i = 0; str[i] && !IS_WHITE(str, i) && !END_CHAR(str, i); ++i);
	return i;
}

int save_lines_in_macro_table(FILE *file, Macro_Table **table) {
	char line[MAX_LINE];
	int non_white = 0, white = 0, cur_val_len = 0, cur_val_pos = 0;

	fgets(line, 80, file);
	if (line[strlen(line)-1] != '\n' && !feof(file) && *line != '\n') {
		printf("Error: Line is too long, max %d characters.\n", MAX_LINE);
		return 0;
	}

	SKIP_WHITE(line, non_white);
	white = non_white;
	SKIP_NON_WHITE((line), white);
	if (END_CHAR(line, white)) --white;

	/* Macro definition must end, going over lines from the file until encountering "endmcro". */
	while(!(strncmp((line+non_white), "endmcro", (white-non_white)) == 0)) {
		cur_val_len += (strlen(line)-1);
		(*table)->value = (char *)realloc((*table)->value, cur_val_len * sizeof(char));
		if (!(*table)->value) { 
			printf("Error: Memory allocation failed.\n");\
			return -1;
		}
		strcpy((*table)->value + cur_val_pos, (line+1)); /* copying the line to the tables value at the first blank char. */
		cur_val_pos = cur_val_len;
	
		fgets(line, 80, file);
		if (line[strlen(line)-1] != '\n' && !feof(file) && *line != '\n') {
			printf("Error: Line is too long, max %d characters.\n", MAX_LINE);
			return 0;
		}
		non_white = 0;
		SKIP_WHITE(line, non_white);
		white = non_white;
		SKIP_NON_WHITE(line, white);
	}

	/* encountered "endmcro", checking if that line has extraneous characters, if not, add the macro value to the table. */
	white++;
	SKIP_WHITE(line, white);
	if (!END_CHAR(line, white)) {
			printf("Error: Extraneous text after \"endmcro\".\n");
			return 0;
	}
	/* does not have extraneous text after "endmcro", returning 1. */
	return 1;

}

int check_macro_name(char *name, Macro_Table *head) {
	/* going over the list, checking if the name already exists. */
	while (head && head->name) {
		if (strcmp(head->name, name) == 0) return 1;
		head = head->next;
	}

	/* returning 0 if the name does not already exist in the list. */
	return 0;
}

char *get_macro_val(char *name, Macro_Table *head) {
	/* going over the list until reaching  */
	while(head && head->name) {
		if(strcmp((head->name), name) == 0) return (head->value);
		head = head->next;
	}
	return NULL;
}

void free_table(Macro_Table **head) {
	Macro_Table *next;
	while(*head != NULL) {
		next = (*head)->next;
		free((*head)->name);
		free((*head)->value);
		free(*head);
		*head = next;
	}
}

char *strcat_name(char *file_name, char *to_add) {
	char *new_name = (char *)malloc((strlen(file_name)+strlen(to_add)+1) * sizeof(char));
	if (!new_name) return NULL;
	strcpy(new_name, file_name);
	strcat(new_name, ".as");
	return new_name;
}

	
