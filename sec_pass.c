#include "second_pass.h"

boolean make_code_binary(FILE *from, struct Data_Table *data_head, int *IC, int *DC, int *counter) {
    struct Data_Table *curr_node;
    int i = 0, line, errors;
    char st[MAX_LINE];
    for (line = 1; fgets(st, MAX_LINE, from) != NULL; line++) {
        curr_node = data_head;
        mark_label_entry(st, line, data_head, counter);
        while(curr_node && (curr_node) != 'c' && getType(curr_node) != 'E'){
            curr_node = getNext(curr_node);
        }
        int res = make_command(st, curr_node, line, data_head, counter);
        if(res == 1) {
            setType(curr_node, 'f'); //stands for finished with this line
        }
        if(res == 0)
          errors = 1;
    }

    if (errors) return FALSE;
    return TRUE;
}

