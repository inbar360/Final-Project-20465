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



int word_length(char *str) {

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



void base64print(FILE *obj, char bin[]) {

	char base_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int num = 0, i;

	char first, second;



	for (i = 0; i < 6; i++) {

        num <<= 1; /* Shifts the number left by 1 position */

        num |= (bin[i] - '0'); /* Set the least significant bit based on the bin array */

    }

	first = base_chars[num]; /* Setting first to the char representing the first 6 bits. */

	num = 0; /* Setting num to 0 for the second group of 6 bits. */



	for (i = 0; i < 6; i++) {

		num <<=1; /* Shifts the number left by 1 position. */

		num |= (bin[i+6] - '0'); /* Set the least significant bit based on the bin array. */

	}

	second = base_chars[num]; /* Setting second to the char representing the last 6 bits. */

	fprintf(obj, "%c%c", first, second); /* Printing both chars to the object file. */

}

int label_length(char *st) {
	int l = 0;
	while (l < 32 && st[l] && isalnum(st[l])) {
		l++;
	}
	
	return l;
}



void print_nodes(struct Data_Table *head) {

	struct Data_Table *curr = head;

	

	while(curr) {

		printf("curr val=%d, len=%d, type='%c', data='%s'\n", getValue(curr), getLength(curr), getType(curr), getData(curr));

		curr = getNext(curr);

	}

}