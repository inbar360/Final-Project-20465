#ifndef _SEC_PASS_H
#define _SEC_PASS_H

#include "globals.h"
#include "tables.h"
#include "utils.h"
#include "pass_functions.h"


boolean make_code_binary(FILE *from, struct Data_Table *data_head, int *IC, int *DC, int *counter);

#endif