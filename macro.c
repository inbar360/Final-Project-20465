#include "macro.h"

#define MAX_LINE 81
#define MCRO_LENGTH 4

int linepos = 0;

static int process_line(FILE *from, char *line, struct Macro_Table **table, FILE *to, struct Macro_Table *head);

boolean preprocess(FILE *file, char *name, struct Macro_Table **head, FILE *to) {

    struct Macro_Table *table = create_table();
    char line[MAX_LINE];
    int process = TRUE;
    *head = table;

    while (process != EOF && process) {
        process = process_line(file, line, &table, to, *head);
    }

    /* returns if reached the end of the file, or found a line with too many characters. */
    return process == EOF; 
}

char *get_macro_value(FILE *from, struct Macro_Table **table);

static int process_line(FILE *from, char *line, struct Macro_Table **table, FILE *to, struct Macro_Table *head) {

    int start = 0, end = 0, temp = 0;
    char *mcr_name = NULL, *mcr_value = NULL;
    if (fgets(line, MAX_LINE, from) == NULL) return EOF;
    linepos++;
	printf("line is: \"%s\"\n", line);
	line[MAX_LINE-1] = '\0';
    if (line[strlen(line)-1] != '\n' && !feof(from) && *line != '\n') {
        printf("Error: Length of line %d is longer than %d characters.\n", linepos, MAX_LINE);
        return FALSE;
    }

    SKIP_WHITE(line, start);
    end = start;
    SKIP_NON_WHITE(line, end);
    if (line[start] == ';' || END_CHAR(line, start)) return TRUE;

    if (equals(&line[start], &line[end], "mcro")) {
    	printf("started with mcro\n");
        start = end;
        SKIP_WHITE(line, start);
        end = start;
        SKIP_NON_WHITE(line, end);
        temp = end;
        SKIP_WHITE(line, temp);
        if(!END_CHAR(line, temp)) {
            printf("Error: Extranous text after macro name at line %d.\n", linepos);
            return FALSE;
        }
        mcr_name = (char *)malloc(end-start+1);
        if (!mcr_name) {
        	printf("Error: Memory allocation failed.\n");
        	return FALSE;
        }
        printf("before name\n");
        strncpy(mcr_name, line+start, end-start);
        mcr_name[end-start] = '\0';
        printf("after name, \"%s\"\n", mcr_name);
        if (name_exists(head, mcr_name)) {
            printf("Error: Macro name already exists at line %d.\n", linepos);
            free(mcr_name);
            return FALSE;
        }
		printf("before setting the name\n");
        setName((*table), mcr_name);
        printf("after setting the name\n");
        mcr_value = get_macro_value(from, table);
        if (mcr_value == NULL) {
            free(mcr_name);
            return FALSE;
        }
            
        setValue(*table, mcr_value);

        free(mcr_name);
        free(mcr_value);
        setNext(*table);
        *table = getNext(*table);
        if (!(*table)) {
            printf("Error: Memory allocation failed.\n");
            free(mcr_name);
            return FALSE;
        }
        printf("ended with mcro\n");
    }

    else {
        mcr_name = (char *)malloc(end-start+1);
        strncpy(mcr_name, line+start, end-start);
        mcr_name[end-start] = '\0';
        
        if(name_exists(head, mcr_name)) {
            mcr_value = find_macro_val(head, mcr_name);
            fputs(mcr_value, to);
        }

        else 
            fputs(line, to);

        free(mcr_name);
    }

    return TRUE;
}

char *get_macro_value(FILE *from, struct Macro_Table **table) {
    char *val = (char *)malloc(sizeof(char));
    static char line[MAX_LINE];
    int start = 0, end = 0, curlen = 1, curpos = 0;
    if (!val) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    fgets(line, MAX_LINE, from);
    linepos++;
    line[MAX_LINE-1] = '\0';
    if (line[strlen(line)-1] != '\n' && !feof(from) && *line != '\n') {
        printf("Error: Length of line %d is longer than %d characters.\n", linepos, MAX_LINE);
        return NULL;
    }

    SKIP_WHITE(line, start);
    end = start;
    SKIP_NON_WHITE(line, end);

    /* By the instructions, macro definition must end, therefore, no need to check for eof. */
    while(!equals(&line[start], &line[end], "endmcro")) {
        val[curlen-1] = '\0';
        if (!(line[start] == ';' || END_CHAR(line, start))) {
        	printf("len val, line: %d %d\n", strlen(val), strlen(line));
        	curlen = strlen(val) + strlen(line);
        	printf("val %d, curlen %d, val is: \"%s\"\n", strlen(val), curlen, val);
            val = (char *)realloc(val, curlen*sizeof(char));
            memcpy(val+curpos, line+1, strlen(line));
            curpos += strlen(line)-1;
            printf("val cur is: \"%s\"\n", val);
        } 

        fgets(line, MAX_LINE, from);
        linepos++;
        line[MAX_LINE-1] = '\0';
        if (line[strlen(line)-1] != '\n' && !feof(from) && *line != '\n') {
            printf("Error: Length of line %d is longer than %d characters.\n", linepos ,MAX_LINE);
            return NULL;
        }

        start = 0;
        SKIP_WHITE(line, start);
        end = start;
        SKIP_NON_WHITE(line, end);
    }

    return val;
}