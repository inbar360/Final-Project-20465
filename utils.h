#ifndef _UTILS_H
#define _UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

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

/* Given a string str and a starting index, 
   this function returns the length of the word starting at that index. */
int word_length(char *str, int start_idx);

/* This function checks whether two words are equal,
   The given "word", and the string starting with "start" and ending with the char before "end".
   returns: TRUE if the words are equal, FALSE otherwise. */
boolean equals(char *start, char *end, char *word);

#endif