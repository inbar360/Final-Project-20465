#ifndef _UTILS_H

#define _UTILS_H

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "globals.h"

#include "tables.h"



#define IS_WHITE(string, index) (string[index] && (string[index] == ' ' || string[index] == '\t'))

#define END_CHAR(string, index) (string[index] == '\n' || string[index] == '\0')

#define SKIP_WHITE(string, index) \
      for (;string[(index)] && (IS_WHITE(string, index)); (++(index)));   

#define SKIP_NON_WHITE(string, index) \
		for (;string[(index)] && !IS_WHITE(string, index) && !END_CHAR(string, index); (++(index)));

#define REMOVE_FILE(file, name) \

		fclose(file); \

		remove(name);



/* This function receives a file name and a string to add, 

   creates a new variable that includes a combination of both.

   returns: NULL if ran into allocation errors, otherwise the new string. */

char *strcat_name(char *file_name, char *to_add);



/* Given a string str, 

   this function returns the length of the first word. */

int word_length(char *str);



/* This function checks whether two words are equal,

   The given "word", and the string starting with "start" and ending with the char before "end".

   returns: TRUE if the words are equal, FALSE otherwise. */

boolean equals(char *start, char *end, char *word);



/* This function receives a char array of length 12, the binary representation,

   Prints the base64 words into the object file. */

void base64print(FILE *obj, char bin[]);



void print_nodes(struct Data_Table *head);



#endif