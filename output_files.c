#include "output_files.h"

/* This function creates the entry file and writes into it. */
static boolean create_ent(char *name, struct Data_Table *head);

/* This function creates the extern file and writes into it. */
static boolean create_ext(char *name, struct Data_Table *head);

/* This function creates the object file and writes into it. */
static boolean create_ob(char *name, int ic, int dc, struct Data_Table *head);

boolean create_files(char *name, int ic, int dc, struct Data_Table *data_table) {
    return create_ent(name, data_table) && create_ext(name, data_table) && create_ob(name, ic, dc, data_table);
}

static boolean create_ent(char *name, struct Data_Table *head) {
    FILE *ent;
    struct Data_Table *curr = head;
    char *new_name = strcat_name(name, ".ent"); /* Using strcat_name function from "utils.c" to create the new name. */

    /* By the instructions, the .ent file should not exist if the list is empty.
       Before opening the file, check if the list is empty, if it is, return TRUE without opening the file. */
    if (curr == NULL) {
        free(new_name);
        return TRUE;
    }

    ent = fopen(new_name, "w"); /* Opening the file for writing. */

    if (!ent) {
        printf("Error: Could not open file %s.\n", new_name);
        free(new_name);
        return FALSE;
    }

    do { /* We know the head is not NULL */
        if (getType(curr) == 'e' || getType(curr) == 'F') { /* If the current type is 'e', meaning entry, print the name + value. */
            fprintf(ent, "%s %d\n", getData(curr), getValue(curr));
        }
        curr = getNext(curr); /* Set curr to the next in the list. */
    } while(curr); /* While curr is not NULL. */

    free(new_name);
    fclose(ent);
    return TRUE; /* Free the new name, close the file and return TRUE. */
}

static boolean create_ext(char *name, struct Data_Table *head) {
    FILE *ext;
    struct Data_Table *curr = head;
    char *new_name = strcat_name(name, ".ext"); /* Using strcat_name function from "utils.c" to create the new name. */

    /* By the instructions, the .ext file should not exist if the list is empty.
       Before opening the file, check if the list is empty, if it is, return TRUE without opening the file. */
    if (curr == NULL) {
        free(new_name);
        return TRUE;
    }

    ext = fopen(new_name, "w"); /* Open the file for writing. */

    if (!ext) {
        printf("Error: Could not open file %s.\n", new_name);
        free(new_name);
        return FALSE;
    }

    do { /* We know the head is not NULL. */
        if (getType(curr) == 'x') { /* If the current type is 'x', meaning extern, printf the name + value. */
            fprintf(ext, "%s %d", getData(curr), getValue(curr));
        }
        curr = getNext(curr); /* Set curr to the next in the list. */
    } while(curr); /* While curr is not NULL. */

    free(new_name);
    fclose(ext);
    return TRUE; /* Free new name, close the file and return TRUE. */
}

static boolean create_ob(char *name, int ic, int dc, struct Data_Table *head) {
    FILE *ob;
    char *new_name = strcat_name(name, ".ob"), (*data)[BITS]; /* Using strcat_name function from "utils.c" to create the new name. */
    int i;
    struct Data_Table *curr = head;

    ob = fopen(new_name, "w"); /* Opening the file for writing. */

    if (!ob) {
        printf("Error: Could not open file %s.\n", new_name);
        free(new_name);
        return FALSE;
    }

    /* Print the IC and DC as the headline for the object file. */
    fprintf(ob, "%d %d\n", ic, dc); 

    while (curr) { /* While curr is not NULL. */
        data = getBinary(curr); /* Get the binary into data. */
        for (i = 0; i < getLength(curr); i++) { /* Go over the data. */
            if (getType(curr) == 'f' || getType(curr) == 'F')
                base64print(ob, data[i]); /* Print the data in base64. */
        }
        curr = getNext(curr); /* Set curr to the next in the list. */
    }

    curr = head;
    while (curr) {
        data = getBinary(curr);
        for (i = 0; i < getLength(curr); i++) { /* Go over the data. */
            if (getType(curr) == 's' || getType(curr) == 'e' || getType(curr) == 'd' || getType(curr) == 'x')
                base64print(ob, data[i]); /* Print the data in base64. */
        }
        curr = getNext(curr); /* Set curr to the next in the list. */
    }

    free(new_name); 
    fclose(ob);
    return TRUE; /* Free new name, close the file, and return TRUE. */
}
