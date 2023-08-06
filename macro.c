/* Assumptions:
	- The macro definition must end.
	- There are no nested macro definitions.
	- Macro can be called only in the first segment of the line. */

#include "macro.h"

#define MAX_LINE 80
#define MCRO_LENGTH 4

#define IS_WHITE(string, index) (string[index] && (string[index] == ' ' || string[index] == '\t'))
#define END_CHAR(string, index) (string[index] == '\n' || string[index] == '\0')
#define SKIP_WHITE(string, index) \
        for (;string[(index)] && (IS_WHITE(string, index)); (++(index)));   
#define SKIP_NON_WHITE(string, index) \
		for (;string[(index)] && !IS_WHITE(string, index) && !END_CHAR(string, index); (++(index)));
#define REMOVE_FILE(file, name) \
		fclose(file); \
		remove(name);

struct Macro_Table {
	char *name; 
	char *value;
	struct Macro_Table *next;
};


/* This function receives a part of a line, and is used to determine a length of a word (for memory allocation) */
int word_length(char *str, int start_idx);

/* This function saves the lines in between the name of the macro and the "endmcro" flag into the macro table.
   Returns -1 if had errors such as memory allocation failing, 0 if came across errors such as too many character,
   and 1 if saved the lines in the macro table with no errors. */
int save_lines_in_macro_table(FILE *file, Macro_Table **table);

/* This function checks if the recieved name exists in the macro table.
   Returns 1 if the name does exist, and 0 otherwise. */
int check_macro_name(char *name, Macro_Table *table);

/* This function goes over the table until identifying the name in the table,
   if the name does not exist, the function returns NULL, if the name does exist,
   the function returns a pointer to the value of the macro with the given name. */
char *get_macro_val(char *name, Macro_Table *table);

int preprocess(FILE *file, char *name, Macro_Table **head) {
	
	FILE *to = NULL;
	char line[MAX_LINE];
	char *macro_name = NULL, *macro_val = NULL;
	char *name_of_new_file = strcat_name(name, ".am");
	int i = 0, end = 0, macro_name_len, extra, result = 0, saved = 1;
	Macro_Table *table = NULL;

	/* If the strcat_name function returned NULL, meaning, memory allocation failed, return -1. */
	if (!name_of_new_file) {
		printf("Error: Memory allocation failed.\n");
		return -1;
	}

	/* Open the new file with the name_of_new_file for writing. */
	to = fopen(name_of_new_file, "w+");
	/* if the file did not open, print error, close the file and exit. */
	if (!to) {
		printf("Error: could not open file.\n");
		REMOVE_FILE(to, name_of_new_file);
		free(name_of_new_file);
		return -1;
	}

	/* Allocating memory for heads value, table */
	table = (Macro_Table *)malloc(sizeof(Macro_Table));
	/* If either of them is NULL, return -1. */
	if (!table) {
		printf("Error: memory allocation failed.\n");
		REMOVE_FILE(to, name_of_new_file);
		free(name_of_new_file);
		return -1;
	}

	/* Assinging heads value to table, using memset to assign all chars of line to '\0'. */
	*head = table;
	memset(line, '\0', MAX_LINE);

	/* Going over the lines of the file in a while loop, each time assingning line the characters of the current line of the file. */
	while(fgets(line, MAX_LINE, file)) {

		/* If the table is NULL (meaning we found one macro definition), allocate memory. */
		if (!table) {
			table = (Macro_Table *)malloc(sizeof(Macro_Table));
			if (!table) { /* If the memory allocation failed, return -1. */
				printf("Error: Memory allocation failed.\n");
				REMOVE_FILE(to, name_of_new_file);
				free(name_of_new_file);
				if (macro_name) free(macro_name);
				table = NULL;
				free_table(head);
				return -1;
			}
		}
		/* Set i to 0 (because we need to go over the line from the start), and if the line is too long, return 0. */
		i = 0;
		if (line[strlen(line)-1] != '\n' && !feof(file) && *line != '\n') {
			printf("Error: Line is too long, max %d characters.\n", MAX_LINE);
			REMOVE_FILE(to, name_of_new_file);
			free(name_of_new_file);
			free(table);
			return 0;
		}
		/* Set i as the index of the first non_white character, and if it's an empty line/comment line, skip it. */
		SKIP_WHITE(line, i);
		if (END_CHAR(line, i) || line[i] == ';') continue;
		/* If encountered "mcro", and right after is a white char. */
		else if(strncmp(line+i, "mcro", MCRO_LENGTH) == 0 && (IS_WHITE(line, i+MCRO_LENGTH))) {
			/* Skipe the "mcro" and the white chars right after. */
			i+=MCRO_LENGTH;
			SKIP_WHITE(line, i);
			/* Assign the length of the macro name to the macro_name_len variable. */
			macro_name_len = word_length(line+i, i);
			extra = i;
			SKIP_NON_WHITE(line, extra);
			/* Checking if there are other characters after mcro + macro name. */
			SKIP_WHITE(line, extra);
			if (*(line+extra) != '\0' && *(line+extra) != '\n') {
				printf("Error: Extraneous text after macro name.\n");
				REMOVE_FILE(to, name_of_new_file);
				free(name_of_new_file);
				free(table);
				return 0;
			}

			/* Allocating memory  */
			macro_name = (char *)malloc((macro_name_len+1) * sizeof(char));
			if (!macro_name) {
				printf("Error: Memory allocation failed.\n");
				free_table(head);
				free(table);
				REMOVE_FILE(to, name_of_new_file);
				free(name_of_new_file);
				free(table);
				return -1;
			}
			/* Setting the last char of macro_name to '\0' and copying the rest of the chars from the line. */
			macro_name[macro_name_len] = '\0';
			strncpy(macro_name, line+i, macro_name_len);
			result = check_macro_name(macro_name, *head);
			/* If the macro_name does exist, write error and return 0. */
			if (result == 1) {
				printf("Error: Macro name already exists.\n");
				REMOVE_FILE(to, name_of_new_file);
				free(name_of_new_file);
				free(macro_name);
				free(table);
				return 0;
			}

			/* Allocating memory for tables attributes. */
			table->value = NULL;
			table->name = (char *)malloc((strlen(macro_name)+1) * sizeof(char));
			table->next = NULL;
			/* If either of the attributes allocation failed, return -1. */

			if (!(table->name)) {
				printf("Error: memory allocation failed2.");
				free(macro_name);
				free_table(head);
				free(table);
				REMOVE_FILE(to, name_of_new_file);
				free(name_of_new_file);
				return -1;
			}

			/* By this point we know the macro name does not appear in the macro table, so we add it. */
			strcpy(table->name, macro_name);
			saved = save_lines_in_macro_table(file, &table);
			/* If the macro value was not saved, if saved is 0 return 0, if it's -1 return -1. */
			if (saved == 0) {
				REMOVE_FILE(to, name_of_new_file);
				free(name_of_new_file);
				free(macro_name);
				return 0;
			}
			else if (saved == -1) {
				REMOVE_FILE(to, name_of_new_file);
				free(name_of_new_file);
				free(macro_name);
				free_table(head);
				free(table->name);
				free(table);
				return -1;
			}

			/* Saved the macro value, pointing to the next node in the list and freeing macro_name. */
			table = table->next;
			free(macro_name);
		}

		/* If the first word is not "mcro", allocate memory for macro_name. */
		else {
			end = i;
			SKIP_NON_WHITE(line, end);
			macro_name = (char *)malloc((end-i+1) * sizeof(char));
			if (!macro_name) {
				printf("Error: Memory allocation failed.\n");
				free_table(head);
				free(table->name);
				free(table);
				REMOVE_FILE(to, name_of_new_file);
				free(name_of_new_file);
				return -1;
			}

			/* Assign the last char to '\0', and copy the first word from line to macro_name. */
			macro_name[end-i] = '\0';
			strncpy(macro_name, (line+i), end-i);
			/* If macro_val is not null, meaning the macro name does exist in the table,
			   print the macro value to the wanted file. */
			if ((macro_val = get_macro_val(macro_name, *head)) != NULL) {
				fputs(macro_val, to);
			}
			/* The macro_val is null, meaning the line does not feature a macro name,
			   print the line to the wanted file. */
			else {
				fputs(line, to);
			}
			/* Free macro_name. */
			free(macro_name);
		}
	}
	/* Freeing name_of_new_file and closing the file. Returning 1 meaning there were no errors. */
	free(name_of_new_file);
	fclose(to);
	return 1;
}

int word_length(char *str, int start_idx) {
	/* Variable: i- an integer for the usage of the for loop. */
	int i;
	
	/* Going over the line until encountering either a white char, or the end of the line/file. */
	for(i = 0; str[i] && !IS_WHITE(str, i) && !END_CHAR(str, i); ++i);
	/* Returning i. */
	return i;
}

int save_lines_in_macro_table(FILE *file, Macro_Table **table) {
	/* Variables: 
		- line- an array of characters of length 80 (MAX_LINE) which will store the value of each line. 
		- non_white- an integer used to save the index of the next non_white character.
		- white- an integer used to save the index of the next white character.
		- cur_val_len- an integer used to save the current length of the macro value.
		- cur_val_pos- an integer used to save the index of the characters of the macro value that are free. */
	char line[MAX_LINE];
	int non_white = 0, white = 0, cur_val_len = 0, cur_val_pos = 0;

	/* Receieves a line of 80 characters max, if the line is longer than 80, returns 0. */
	fgets(line, MAX_LINE, file);
	if (line[strlen(line)-1] != '\n' && !feof(file) && *line != '\n') {
		printf("Error: Line is too long, max %d characters.\n", MAX_LINE);
		return 0;
	}

	/* Uses non_white to point to the start of the first word, 
	   and white to point to the first character after the word. */
	SKIP_WHITE(line, non_white);
	white = non_white;
	SKIP_NON_WHITE((line), white);
	/* Macro definition must end, going over lines from the file until encountering "endmcro". */
	while(!(strncmp((line+non_white), "endmcro", (white-non_white)) == 0)) {
		/* Adding to cur_val_len each iteration the length of the line -1 (because we skip the '\t'). */
		cur_val_len += (strlen(line)-1);
		if (!(*table)->value) {
			(*table)->value = (char *)malloc((cur_val_len+1) * sizeof(char));
			if (!(*table)->value) { 
				printf("Error: Memory allocation failed.\n");\
				return -1;
			}
			(*table)->value[cur_val_len] = '\0';
		}
		/* Reallocating more memory to the tables value, if the allocation fails, returns -1. */
		(*table)->value = (char *)realloc((*table)->value, (cur_val_len+1) * sizeof(char));
		if (!(*table)->value) { 
			printf("Error: Memory allocation failed.\n");\
			return -1;
		}
		
		/* After the memory reallocation, copying the line to the table, and setting pos to the new length. */
		(*table)->value[cur_val_len] = '\0';
		strcpy((*table)->value + cur_val_pos, (line+1)); /* copying the line to the tables value at the first blank char. */
		cur_val_pos = cur_val_len;

		/* Get a new line from the file, and return 0 if it's too long. */
		fgets(line, MAX_LINE, file);
		if (line[strlen(line)-1] != '\n' && !feof(file) && *line != '\n') {
			printf("Error: Line is too long, max %d characters.\n", MAX_LINE);
			return 0;
		}

		/* Set non_white to 0, then skip all the white characters, 
		   set white to non_white and skip all the non_white characters. */
		non_white = 0;
		SKIP_WHITE(line, non_white);
		white = non_white;
		SKIP_NON_WHITE(line, white);
	}

	/* Encountered "endmcro", checking if that line has extraneous characters. */
	SKIP_WHITE(line, white);
	if (!END_CHAR(line, white)) {
		printf("Error: Extraneous text after \"endmcro\".\n");
		return 0;
	}
	/* Does not have extraneous text after "endmcro", returning 1. */
	return 1;

}

int check_macro_name(char *name, Macro_Table *head) {
	/* Going over the list, checking if the name already exists. */
	while (head && head->name) {
		if (strcmp(head->name, name) == 0) return 1;
		head = head->next;
	}

	/* Returning 0 if the name does not already exist in the list. */
	return 0;
}

char *get_macro_val(char *name, Macro_Table *head) {
	/* Going over the list until reaching  */
	while(head && head->name) {
		if(strcmp((head->name), name) == 0) return (head->value);
		head = head->next;
	}
	return NULL;
}

void free_table(Macro_Table **head) {
	/* Variable: next - used to save head's next each iteration of the while loop. */
	Macro_Table *next;
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

char *strcat_name(char *file_name, char *to_add) {
	/* Allocating memory, of the sum of both given names + 1 characters, for new_name */
	char *new_name = (char *)malloc((strlen(file_name)+strlen(to_add)+1) * sizeof(char));
	
	/* Returns NULL if the memory allocation failed. */
	if (!new_name) return NULL;

	/* Copies the first given name to new_name, and adds the second name to new_name. */
	strcpy(new_name, file_name);
	strcat(new_name, to_add);
	/* Returns new_name. */
	return new_name;
}