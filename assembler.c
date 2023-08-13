#include "utils.h"
#include "macro.h"
#include "table.h"

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
    FILE *file = NULL, *to = NULL;
    struct Macro_Table *head = NULL;
    char *new_name = strcat_name(file_name, ".am");
    int prepro;

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
    	printf("Error: Could not open file %s\n", name);
    	free(name);
        free(new_name);
    	return FALSE;
    }

    to = fopen(new_name, "w+");
    if(!to) {
        printf("Error: Could not open file %s\n", new_name);
        free(new_name);
        free(name);
        fclose(file);
        return FALSE;
    }
    prepro = preprocess(file, file_name, &head, to);
    if (!prepro) {
        printf("Did not work");
        REMOVE_FILE(to, new_name);
        free(new_name);
        free(name);
        fclose(file);
        free_table(&head);
        return FALSE;
    }
    
    free(new_name);
    free_table(&head);
    free(name);
    fclose(file);
    fclose(to);
    return TRUE;
    
}