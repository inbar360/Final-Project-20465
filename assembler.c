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

int process_file(char *name) {
    FILE *file = fopen(name,"r");
    int prepro;
    if (!file) {
    	printf("Error: Could not open file %s\n", name);
    	return 0;
    }
    prepro = preprocess(file, name);
    if(prepro == -1) {
   		fclose(file);
    	exit(1);
    }
    else if (prepro == 0) {
    	fclose(file);
    	return 0;
    }
    
    fclose(file);
    return 1;
    
}