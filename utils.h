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
		
char *strcat_name(char *file_name, char *to_add);
int word_length(char *str, int start_idx);
boolean equals(char *start, char *end, char *word);

#endif