#include <stdio.h>
#include "globals.h"
#include "utils.h"

char *strcat_name(char *file_name, char *to_add) {
	/* Allocating memory, of the sum of both given names + 1 characters, for new_name */
	char *new_name = (char *)malloc((strlen(file_name)+strlen(to_add)+1) * sizeof(char));
	
	/* Returns NULL if the memory allocation failed. */
	if (!new_name) return NULL;

	/* Copies the first given name to new_name, and adds the second name to new_name. */
	strcpy(new_name, file_name);
	strcat(new_name, to_add);
	/* Returns new_name. */
	return new_name;
}

int word_length(char *str, int start_idx) {
	/* Variable: i- an integer for the usage of the for loop. */
	int i;
	
	/* Going over the line until encountering either a white char, or the end of the line/file. */
	for(i = 0; str[i] && !IS_WHITE(str, i) && !END_CHAR(str, i); ++i);
	/* Returning i. */
	return i;
}

boolean equals(char *start, char *end, char *word) {
    int result;
    char temp = *end; /* Creates temp and sets it to ends value. */
    *end = '\0'; /* Sets the char to '\0'. */
    result = !strcmp(start, word); /* Sets result to whether the strings are equal. */
    *end = temp; /* Sets ends value back to it's original value. */
    return result ? TRUE : FALSE; /* Returns whether the strings are equal. */
}

