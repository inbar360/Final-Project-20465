#include "globals.h"

#include "tables.h"

#include "utils.h"



/* This function checks if the given string is a valid label name.

   Returns: TRUE if the given string is a valid label name, FALSE otherwise. */

boolean is_label(char st[]);



boolean in_list(struct Data_Table *dt, char str[]);



char *opcode_in_binary(char *op);



void make_binary(int n, int line, char *bits);



char *organization_type(char *op);



char *make_command_binary(char *st, char binary[12]);



int make_memory_of_command_label(char *st, int line, struct Data_Table *data_head, int *IC, int *counter);



int make_memory_of_command(char *st, int line, struct Data_Table *data_head, int *IC, int *counter);



int add_extern_data(char *st, int line, struct Data_Table *data_head, int *DC, int *counter);



int mark_label_entry(char *st, int line, struct Data_Table *data_head, int *counter);



int add_string_data(char *st, int line, struct Data_Table *data_head, int *DC, int *counter);



int add_data_data(char *st, int line, struct Data_Table *data_head, int *DC, int *counter);



int make_command(char *st, struct Data_Table *curr_node, int line, struct Data_Table *data_head, int *counter);