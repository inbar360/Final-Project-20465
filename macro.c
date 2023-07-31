
/* Assumptions:
	- The macro definition must end.
	- There are no nested macro definitions.
	- There are no multiple definition */

#include "macro.h"

#define MAX_LINE 80

#define SKIP_WHITE(string, index) \
        for (;string[(index)] && (string[index] == ' ' || string[index] == '\t'); (++(index)));
        
#define SKIP_NON_WHITE(string, index) \
		for (;string[(index)] && (string[index] != ' ' && string[index] != '\t'); (++(index)));

typedef struct Macro_Table {
	char *name;
	char *value;
	struct Macro_Table *next;
} Macro_Table;

/*int save_lines_in_macro_table(*/

int word_length(char *str, int start_idx);

int main(int argc, char* argv[]) {
	
	FILE *file = fopen(argv[1], "r");
	FILE *to = fopen("file.am", "w+");
	char a[80];
	int i = 0, len;
	Macro_Table *table, *head;
	if (!file || !to) {
		printf("Error: could not open file.\n");
	}
	
	while(fgets(a, 80, file)) {
		if (a[strlen(a)-1] != '\n' && !feof(file) && *a != '\n') {
			printf("Error: Line is too long, max %d characters.\n", MAX_LINE);
			break;
		}
		SKIP_WHITE(a, i);
		if(strncmp(a+i, "mcro", 4) == 0 && (a[i+1] == ' ' || a[i+1] == '\t')) {
			i++;
			SKIP_NON_WHITE(a, len);
		}
	}
	fclose(file);
	return 0;
}

int word_length(char *str, int start_idx) {
	for
}

	
