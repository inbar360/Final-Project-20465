
/* Assumptions:
	- The macro definition must end.
	- There are no nested macro definitions.
	- There are no multiple definition */

#include "macro.h"

#define MAX_LINE 80

#define IS_WHITE(string, index) (string[index] == ' ' || string[index] == '\t')

#define SKIP_WHITE(string, index) \
        for (;string[(index)] && (IS_WHITE(string, index)); (++(index)));
        
#define SKIP_NON_WHITE(string, index) \
		for (;string[(index)] && (!IS_WHITE(string, index)); (++(index)));

typedef struct Macro_Table {
	char *name;
	char *value;
	struct Macro_Table *next;
} Macro_Table;



int word_length(char *str, int start_idx);

int save_lines_in_macro_table(FILE *file, Macro_Table **table);

int main(int argc, char* argv[]) {
	
	FILE *file = fopen(argv[1], "r");
	FILE *to = fopen("file.am", "w+");
	char line[80];
	int i = 0, end, macro_name_len, cur_len, extra;
	Macro_Table *table = NULL, *head = NULL;
	if (!file) {
		printf("Error: could not open file.\n");
		exit(1);
	}
	if (!to) {
		printf("Error: could not open file.\n");
		fclose(file);
		exit(1);
	}

	head = (Macro_Table *)malloc(sizeof(Macro_Table));
	table = (Macro_Table *)malloc(sizeof(Macro_Table));
	if (!head) {
		printf("Error: memory allocation failed.\n");
		fclose(file);
		fclose(to);
		exit(1);
	}
	if (!table) {
		printf("Error: memory allocation failed.\n");
		free(head);
		fclose(file);
		fclose(to);
		exit(1);
	}
	head = table;
	
	while(fgets(line, 80, file)) {
		if (line[strlen(line)-1] != '\n' && !feof(file) && *line != '\n') {
			printf("Error: Line is too long, max %d characters.\n", MAX_LINE);
			break;
		}
		SKIP_WHITE(line, i); 
		if(strncmp(line+i, "mcro", 4) == 0 && (line[i+1] == ' ' || line[i+1] == '\t')) {
			i++;
			SKIP_WHITE(line, i);
			end = word_length(line+i, i);
			macro_name_len = end-i;
			SKIP_NON_WHITE(line, i);
			/* checking if there are other characters after mcro + macro name. */
			extra = i;
			SKIP_WHITE(line, extra);
			if (*(line+extra) != '\0' && *(line+extra) != '\n') {
				printf("Error: Extraneous text after macro name.\n");
				break;
			}

			/* Allocating memory for tables attributes. */
			cur_len += end;
			table->value = (char *)malloc(sizeof(char));
			table->name = (char *)malloc(macro_name_len * sizeof(char));
			table->next = (Macro_Table *)malloc(sizeof(Macro_Table));

			if (!(table->value)) {
				printf("Error: memory allocation failed.");
				free(head);
				free(table);
				fclose(file);
				fclose(to);
			}

			if (!(table->name)) {
				printf("Error: memory allocation failed.");
				free(head);
				free(table->value);
				free(table);
				fclose(file);
				fclose(to);
			}

			if(!(table->next)) {
				printf("Error: memory allocation failed.");
				free(head);
				free(table->value);
				free(table->name);
				free(table);
				fclose(file);
				fclose(to);
			}

			strncpy(table->name, line+i, macro_name_len);
			

		}
	}
	fclose(file);
	fclose(to);
	return 0;
}

int word_length(char *str, int start_idx) {
	int i;
	for(i = start_idx; str[i] && !IS_WHITE(str, i); ++i);
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

	/* Macro definition must end, going over lines from the file until encountering "endmcro". */
	while(!(strncmp((line+non_white), "endmcro", (white-non_white)) == 0)) {
		fgets(line, 80, file);
		if (line[strlen(line)-1] != '\n' && !feof(file) && *line != '\n') {
			printf("Error: Line is too long, max %d characters.\n", MAX_LINE);
			return 0;
		}
		non_white = 0;
		SKIP_WHITE(line, non_white);
		white = non_white;
		SKIP_NON_WHITE(line, white);
		cur_val_len += strlen(line);
		(*table)->value = realloc((*table)->value, cur_val_len * sizeof(char));
		if (!(*table)->value) { 
			printf("Error: Memory allocation failed.\n");\
			return 0;
		}
		strcpy((*table)->value + cur_val_pos, line); /* copying the line to the tables value at the first blank char. */
		cur_val_pos = cur_val_len;
	}

	/* encountered "endmcro", checking if that line has extraneous characters, if not, add the macro value to the table. */
	SKIP_WHITE(line, white);
	if (*(line+white) != '\0' && *(line+white) != '\n') {
			printf("Error: Extraneous text after \"endmcro\".\n");
			return 0;
	}

	/* does not have extraneous text after "endmcro", returning 1. */
	return 1;

}

	
