#include "second_pass.h"

/* This function receives a line from the file and processes it. */
boolean process_line_secpass(char *line, struct Data_Table *data, int *ic, int linepos);

boolean sec_pass(FILE *from, char *name, struct Data_Table *data, int *ic) {
    char line[MAX_LINE];
    boolean errors = FALSE, pass = TRUE;
    int linepos = 1; /* Set ic to 0, and linepos to 1. */
    *ic = 0;

    while (fgets(line, MAX_LINE, from) != NULL) { /* While EOF is not reached. */
        pass = process_line_secpass(line, data, ic, linepos);
        if (!pass) { /* If errors were found, set errors to TRUE, and print name of file + line. */
            errors = TRUE;
            printf(" in file: \"%s\", at line: %d.\n", name, linepos); /* In case of errors, print the name of file, and line of error. */
        }
        linepos++;
    }
    /* פה חסר קריאה לפונקציה של קידוד */
    /* returns if reached the end of the file and found errors, or have not found errors. */
    return !errors;
}

boolean save_entry(struct Data_Table *data, int linepos, char *line);

boolean process_line_secpass(char *line, struct Data_Table *data, int *ic, int linepos) {
    char *first_word, *label_name;
    boolean saved_entry;
    int i, label = 0;
    SKIP_WHITE(line, i); /* Set i at the index of the first non-white char of the line. */

    if(is_label(line+i)) {
        label = 1; /* If the first word is a label definition, turn on the label flag. */
        label_name = (char *)malloc(word_length(line+i)+1); /* Allocate memory for the name of the label. */
        if (!label_name) { /* If ran into memory isues, exit. */
            printf("Error: Memory allocation failed.\n");
            exit(1);
        }
        strncpy(label_name, line+i, word_length(line+i)); /* Copy the label name from the line. */
        label_name[word_length(line+i)] = '\0';
        SKIP_NON_WHITE(line, i); /* Because the variable will not change outside of this function, we skip the label. */
    }

    SKIP_WHITE(line, i);
    first_word = (char *)malloc(word_length(line+i)+1);
    if (!first_word) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
    strncpy(first_word, line+i, word_length(line+i));
    first_word[word_length(line+i)] = '\0';

    if (!strcmp(first_word, ".entry")) { /* If the directive is .entry */
        if (label) {
            printf("Warning: Label \"%s\" at the start of .entry, line %d is being ignored.\n", label_name, linepos);
            free(label_name);
        }

        saved_entry = save_entry(data, linepos, line+i);
        if (saved_entry == EOF) {
            free(first_word);
            exit(1);
        }

        if (!saved_entry) {
            free(first_word);
            return FALSE;
        }

        free(first_word);
        return TRUE;
    }

    if (label) 
        free(label_name);
    
    if (line[i] == '.') { /* If the first pass we took care of all other directives. */
        free(first_word);
        return TRUE;
    }

    /* Missing return statement. Need to encode operands and set new IC. */
}

boolean save_entry(struct Data_Table *data, int linepos, char *line) {
    struct Data_Table *curr = data;
    int i = 0;
    char *label;
    while (1) {
        curr = data;
        SKIP_NON_WHITE(line, i);
        SKIP_WHITE(line, i);
        if (line[i] == ',') i++;
        SKIP_WHITE(line, i);

        if (line[i] == '\n') break;

        label = (char *)malloc(word_length(line+i) + 1);
        if (!label) {
            printf("Error: Memory allocation failed.\n");
            return EOF;
        }
        strncpy(label, line+i, word_length(line+i));
        label[word_length(line+i)] = '\0';

        while (curr) {
            if(strcmp(label, getData(curr)) == 0) {
                if (getType(curr) == 'x') {
                    printf("Error: Label \"%s\" appears as both extern and entry, ", label);
                    free(label);
                    return FALSE;
                }
                setType(curr, 'e');
                break;
            }
            curr = getEntNext(curr);
            
        }
        free(label);
    }

    return TRUE;
}