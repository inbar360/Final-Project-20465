#include "utils.h"

#include "preprocessor.h"

#include "macro.h"

#include "sec_pass.h"

#include "output_files.h"

#include "first_pass.h"

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

    	printf("name of file: \"%s.as\"\n", argv[i]);

        printf("%s\n", process_file(argv[i]) ? "File process finished succesfully!\n" : "File process failed.\n");

    }



    return 0;

}



static boolean process_file(char *file_name) {

    char *name = strcat_name(file_name, ".as");

    FILE *file = NULL, *am = NULL;

    struct Macro_Table *head = NULL;

    struct Data_Table *data_head = NULL, *curr;

    char *new_name = strcat_name(file_name, ".am"); /* Using strcat_name function from "utils.c" to create the new name. */

    boolean prepro, secpass, firpass = TRUE;

    char st[MAX_LINE];

    int IC = 0, DC = 0, line, counter = 0;



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



    prepro = preprocess(file, new_name, &head, am); /* Calling preprocessor. */

    if (!prepro) {

        printf("Spreading of macros into %s.am did not work.\n", file_name);

        REMOVE_FILE(am, new_name);

        free(new_name);

        free(name);

        fclose(file);

        free_mtable(&head);

        return FALSE;

    }

    

    printf("Preprocessor worked!\n");

    

    rewind(am);

    for (line = 1; fgets(st, MAX_LINE, am) != NULL; line++) {

        if(!firstpass_line(st, line, &data_head, &IC, &DC, &counter)) /* call firstpass_line for each line of the file. */

            firpass = FALSE;

    }

	printf("First pass of \"%s\" %s\n", new_name, firpass ? "finished succesfully!" : "failed.");

    rewind(am); /* Before the second pass, set the file position to the beginning of the file. */

	curr = data_head;

	while(curr != NULL){

		if(getType(curr) == 'c');

		else 

			setValue(curr, getValue(curr) + IC);

		curr = getNext(curr);

	}

	

    secpass = make_code_binary(am, &data_head, &IC, &DC, &counter);

    

    if (!firpass) {

        free(new_name);

        free_mtable(&head);

        free_data_table(&data_head);

        free(name);

        fclose(file);

        fclose(am);

        return FALSE;

    }



    else if (!secpass) {

        printf("Second pass of file \"%s\" did not work.\n", new_name);

        free(new_name);

        free_mtable(&head);

        free_data_table(&data_head);

        free(name);

        fclose(file);

        fclose(am);

        return FALSE;

    }

	

    else {

    	printf("Second pass of file \"%s\" finished succesfully!\n", new_name);

        if(!create_files(file_name, IC, DC, data_head)) {

        	printf("Creating output files for \"%s\" failed.\n", new_name);

            free(new_name);

            free_mtable(&head);

            free_data_table(&data_head);

            free(name);

            fclose(file);

            fclose(am);

            return FALSE;

        }

    }



    free(new_name);

    free_mtable(&head);

    free_data_table(&data_head);

    free(name);

    fclose(file);

    fclose(am);

    return TRUE;

    

}