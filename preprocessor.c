/* Assumptions:
    - Macro definitions must end.
    - There are no nested macro definitons.
    - Macro reference could only be the first word of the line.
    - If a macro is being referenced, the rest of the line is either empty, or white characters. */

#include "preprocessor.h"

#define MAX_LINE 81
#define MCRO_LENGTH 4

int linepos = 0;

/* This function processes a single line from the given file,
   writes the result into the output file.
   returns: FALSE if encountered an error, EOF if reached the end of the file, and TRUE otherwise. */
static int process_line(FILE *from, char *line, struct Macro_Table **table, FILE *to, struct Macro_Table *head, char *name);

boolean preprocess(FILE *file, char *name, struct Macro_Table **head, FILE *to) {
    /* Creating the macro table, and setting heads value to table. */
    struct Macro_Table *table = create_table();
    char line[MAX_LINE];
    int process = TRUE;
    boolean errors = FALSE;
    *head = table;

    /* Calling the process_line function. */
    while (process != EOF) {
        process = process_line(file, line, &table, to, *head, name);
        if (!process) errors = TRUE;
    }

    /* returns if reached the end of the file and found errors, or have not found errors. */
    return !errors; 
}

/* Given a file, and the macro table, this function goes over lines from the file,
   until encountering "endmcro".
   returns: the macro value, or NULL in case of an error. */
char *get_macro_value(FILE *from, struct Macro_Table **table, char *name);

static int process_line(FILE *from, char *line, struct Macro_Table **table, FILE *to, struct Macro_Table *head, char *name) {

    int start = 0, end = 0, temp = 0;
    char *mcr_name = NULL, *mcr_value = NULL, c;
    if (fgets(line, MAX_LINE, from) == NULL) return EOF; /* Returns EOF if reached the end of the file. */
    linepos++; /* Add 1 to linepos. */
	line[MAX_LINE-1] = '\0'; /* Setting the last char of line to '\0'. */
    if (line[strlen(line)-1] != '\n' && !feof(from) && *line != '\n') {
        printf("Error: Length of line %d in file \"%s\" is longer than %d characters.\n", linepos, name, MAX_LINE);
        c = fgetc(from);
        SKIP_REST_OF_LINE(from, c);
        return FALSE;
    }
    /* If the line is not too long, reach the first char that is not white, meaning the start of the first word, or the end of the line. */
    SKIP_WHITE(line, start);
    end = start;
    SKIP_NON_WHITE(line, end);
    if (line[start] == ';' || END_CHAR(line, start)) return TRUE; /* If the line is either empty or a comment, return TRUE without printing it to the output file. */
    /* If encountered "mcro" */
    if (equals(&line[start], &line[end], "mcro")) {
        start = end;
        SKIP_WHITE(line, start);
        end = start;
        SKIP_NON_WHITE(line, end);
        temp = end;
        SKIP_WHITE(line, temp);
        if(!END_CHAR(line, temp)) { /* If there are other chars after the name of macro, return FALSE. */
            printf("Error: Extranous text after macro name at line %d in file \"%s\".\n", linepos, name);
            return FALSE;
        }
        mcr_name = (char *)malloc(end-start+1); /* Allocate the amount of chars + 1 for '\0'. */
        if (!mcr_name) { /* Return FALSE in case of allocation failing. */
        	printf("Error: Memory allocation failed.\n");
        	return FALSE;
        }
        strncpy(mcr_name, line+start, end-start); 
        mcr_name[end-start] = '\0'; /* Copy the amount of chars to mcr_name, and set last char to '\0'. */
        if (name_exists(head, mcr_name)) { /* If the macro name already exists in table, return FALSE. */
            printf("Error: Macro name already exists at line %d in file \"%s\".\n", linepos, name);
            free(mcr_name);
            return FALSE;
        }
        setName((*table), mcr_name); /* Set the current macro name to mcr_name. */
        mcr_value = get_macro_value(from, table, name); /* Get the value of the macro. No need to allocate new memory because the function returns a variable of type char *, which is not freed inside the function. */
        if (mcr_value == NULL) { /* If mcr_value is NULL return FALSE. */
        	free(getName(*table));
        	setName((*table), NULL);
        	free(mcr_name);
            return FALSE;
        } 
        setValue(*table, mcr_value); /* Sets the value of the current macro to mcr_value. */

        /* Frees the variables we allocated memory for. */
        free(mcr_name);
        free(mcr_value);
        setNext(*table); /* Allocates memory for the tables next */
        *table = getNext(*table);
        if (!(*table)) { /* If the value of table is NULL, returns FALSE. */
            printf("Error: Memory allocation failed.\n");
            return FALSE;
        }
    }

    else { /* If did not encounter "mcro", check if the first word is a macro reference */
        mcr_name = (char *)malloc(end-start+1); /* Allocate the amount of chars + 1 for '\0' */
        strncpy(mcr_name, line+start, end-start); /* Copy the amount of chars to mcr_name. */
        mcr_name[end-start] = '\0'; /* Set the last char to '\0' */
        
        if(name_exists(head, mcr_name)) { /* If the name exists in the macro table, set mcr_value to it's value, and put in the output file. */
            mcr_value = find_macro_val(head, mcr_name); /* Assuming there are no other significant characters in the same line. */
            fputs(mcr_value, to);
        }

        else /* If the name is not a macro name, put the line in the output file. */
            fputs(line, to);

        free(mcr_name); /* At the end, free mcr_name. */
    }

    return TRUE; /* Return TRUE if did not run into any issues. */
}

char *get_macro_value(FILE *from, struct Macro_Table **table, char *name) {
    char *val = (char *)malloc(sizeof(char)), c;
    static char line[MAX_LINE];
    boolean errors = FALSE;
    int start = 0, end = 0, curlen = 1, curpos = 0;
    if (!val) { /* If memory allocation failed, return NULL. */
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    fgets(line, MAX_LINE, from); /* Get a line from the file into the variable line. */
    linepos++; /* Add 1 to the linepos. */
    line[MAX_LINE-1] = '\0'; /* Set the last char of line to '\0' */
    if (line[strlen(line)-1] != '\n' && !feof(from) && *line != '\n') { /* If the line is longer than 80 chars, not including '\n', return NULL. */
        printf("Error: Length of line %d in file \"%s\" is longer than %d characters.\n", linepos, name, MAX_LINE);
        c = fgetc(from);
        SKIP_REST_OF_LINE(from, c); /* Skip the rest of the line. */
        errors = TRUE;
    }
    /* Set start to the index of the first non white char and end to the index of the first char after the first word. */
    SKIP_WHITE(line, start);
    end = start;
    SKIP_NON_WHITE(line, end);

    /* By the instructions, macro definition must end, therefore, no need to check for eof. */
    while(!equals(&line[start], &line[end], "endmcro")) {
        val[curlen-1] = '\0'; /* Each time, set the last char of val to '\0'. */
        if (!(line[start] == ';' || END_CHAR(line, start))) { /* If the line isn't a comment nor an empty line. */
        	curlen = strlen(val) + strlen(line); /* Add to curlen. */
            val = (char *)realloc(val, curlen*sizeof(char)); /* Allocate more memory. */
            memcpy(val+curpos, line+1, strlen(line)); /* Copy the chars of the line to val, except for the first char. */
            curpos += strlen(line)-1; /* Add to curpos. */
        } 

        fgets(line, MAX_LINE, from); /* Get another line. */
        linepos++; /* Add 1 to linepos and set the last char to '\0'. */
        line[MAX_LINE-1] = '\0';
        if (line[strlen(line)-1] != '\n' && !feof(from) && *line != '\n') { /* If the line is longer than 80 chars, not including '\n', return NULL. */
            printf("Error: Length of line %d in file \"%s\" is longer than %d characters.\n", linepos, name, MAX_LINE);
            c = fgetc(from);
            SKIP_REST_OF_LINE(from, c); /* Skip the rest of the line. */
            errors = TRUE;
        }
        start = 0; /* Set start to 0, then set it to the index of the first non white char. */
        SKIP_WHITE(line, start);
        end = start; /* Set end to the index of the first char after the first word. */
        SKIP_NON_WHITE(line, end);
    }
	
	if(errors) {
		free(val);
		return NULL;
	}
    return val; /* Return the val. */
}