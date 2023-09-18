#include "globals.h"



#include "tables.h"



#include "utils.h"



#define FIVE 5

#define SIX 6

#define SEVEN 7

#define EIGHT 8

#define NINE 9

#define TEN 10

#define ELEVEN 11

#define BASE_DEFAULT 100

#define FIRST_NUMBER 48

#define LAST_NUMBER 57

#define FIRST_UPCASE_LETTER 65

#define LAST_UPCASE_LETTER 90

#define FIRST_LOWCASE_LETTER 97

#define LAST_LOWCASE_LETTER 122

#define PRINTABLE_CHAR_MIN 31

#define PRINTABLE_CHAR_MAX 128

#define SMALLEST_NUM -2048

#define BIGGEST_NUM 2047



/* This function checks if the given string is a valid label name.

   Returns: TRUE if the given string is a valid label name, FALSE otherwise. */

boolean is_label(char st[]);



/* This function checks if the given string is a label name that appears in the data table.

   Returns: TRUE if the given string appears in the list, FALSE otherwise. */

boolean in_list(struct Data_Table *dt, char str[]);



/* This function returns the binary equivalent of the decimal representation of the command.

   Returns: NULL if not a command. */

char *opcode_in_binary(char *op);



/* This function creates the binary representation of the given number (n).

   Returns: the binary representation if the number is legal, NULL otherwise. */

char *make_binary(int n, int line);



/* This function check the operand given in the string op.

   Returns: the type of operand, NULL if not a legal operand. */

char *organization_type(char *op);



/* This function creates the binary machine word for the command, two operands and ARE.

   Returns: the binary machine word, NULL if ran into errors. */

char *make_command_binary(char *st, char binary[BITS+1]);



/* This function saves label memory.

   Returns: 3 if done succesfully, 2 if not a legal label, 1 if not a label, 0 if ran into errors. */

int make_memory_of_command_label(char *st, int line, struct Data_Table ***data_head, int *IC, int *counter);



/* This function creates a command name without label place in data table. */

int make_memory_of_command(char *st, int line, struct Data_Table ***data_head, int *IC, int *counter);



/* This function creates an extern variable for data table and marks that it's extern. */

int add_extern_data(char *st, int line, struct Data_Table ***data_head, int *DC, int *counter);



/* This function creates an entry variable for data table and marks that it's entry. */

int mark_label_entry(char *st, int line, struct Data_Table ***data_head, int *counter);





int add_string_data(char *st, int line, struct Data_Table ***data_head, int *DC, int *counter);







int add_data_data(char *st, int line, struct Data_Table ***data_head, int *DC, int *counter);







int make_command(char *st, struct Data_Table **curr_node, int line, struct Data_Table ***data_head, int *counter);