#include "utils.h"
#include "preprocessor.h"
#include "macro.h"
#include "second_pass.h"
#include "output_files.h"
#include "tables.h"

static boolean process_file(char *name);

int main(int argc, char *argv[]) {
    int i;
    if (argc == 1) {
        printf("Usage: %s file1 file2 ...\n", argv[0]);
        return 0;
    }
    /* processing the files, starting with 1 because the first argument is not a file name.*/
    for (i = 1; i < argc; i++) {
    	printf("%s\n", argv[i]);
        printf("%d\n", process_file(argv[i]));
    }

    return 0;
}

static boolean process_file(char *file_name) {
    char *name = strcat_name(file_name, ".as");
    FILE *file = NULL, *am = NULL;
    struct Macro_Table *head = NULL;
    struct Data_Table *data_head = NULL;
    char *new_name = strcat_name(file_name, ".am"); /* Using strcat_name function from "utils.c" to create the new name. */
    boolean prepro, secpass;
    int ic = 0, dc = 0;

    if (!new_name) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    if (!name) {
    	printf("Error: Memory allocation failed.\n");
        free(new_name);
    	exit(1);
    }
    
    file = fopen(name, "r");
    if (!file) {
    	printf("Error: Could not open file %s.\n", name);
    	free(name);
        free(new_name);
    	return FALSE;
    }

    am = fopen(new_name, "w+"); /* Opening the file for writing and reading. */
    if(!am) {
        printf("Error: Could not open file %s.\n", new_name);
        free(new_name);
        free(name);
        fclose(file);
        return FALSE;
    }

    prepro = preprocess(file, new_name, &head, am);
    if (!prepro) {
        printf("Spreading of macros into %s.am did not work.\n", file_name);
        REMOVE_FILE(am, new_name);
        free(new_name);
        free(name);
        fclose(file);
        free_table(&head);
        return FALSE;
    }
    /* FIRST PASS HERE
        - in function first_pass:
            1. string
            2. if returned 1 - make data
            3. ..... */

    rewind(am); /* Before the second pass, set the file position to the beginning of the file. */
    secpass = sec_pass(am, new_name, data_head, &ic);
    if (!secpass) {
        printf("Second pass of file \"%s\" did not work.\n", new_name);
    }

    else {
        create_files(file_name, ic, dc, data_head);
    }

    free(new_name);
    free_table(&head);
    free_data_table(&data_head);
    free(name);
    fclose(file);
    fclose(am);
    return TRUE;
    
}