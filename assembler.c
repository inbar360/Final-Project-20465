#include "macro.h"

int process_file(char *name);

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

int process_file(char *file_name) {
    char *name = strcat_name(file_name, ".as");
    FILE *file = NULL;
    int prepro;
    if (!name) {
    	printf("Error: Memory allocation failed.\n");
    	exit(1);
    }
    
    file = fopen(name, "r");
    
    if (!file) {
    	printf("Error: Could not open file %s\n", name);
    	free(name);
    	return 0;
    }
    prepro = preprocess(file, file_name);
    if(prepro == -1) {
   		fclose(file);
   		free(name);
    	exit(1);
    }
    else if (prepro == 0) {
    	fclose(file);
    	free(name);
    	return 0;
    }
    
    free(name);
    fclose(file);
    return 1;
    
}