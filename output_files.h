#include "globals.h"

#include "utils.h"

#include "tables.h"



/* This function creates the output files (except for the one the preprocessor creates),

   Given the name, the entries table, the externs table, the data table, the IC and the DC.

   Returns: TRUE if did not run into problems opening the files, FALSE otherwise. */

boolean create_files(char *name, int ic, int dc, struct Data_Table *data_table);