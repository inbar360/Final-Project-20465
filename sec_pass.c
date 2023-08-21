#include "sec_pass.h"

boolean make_code_binary(FILE *from, struct Data_Table *data_head, int *IC, int *DC, int *counter) {
    struct Data_Table *curr_node;
    int line, errors, res;
    char st[MAX_LINE];
    for (line = 1; fgets(st, MAX_LINE, from) != NULL; line++) {
        printf("line: %d\n", line);
        curr_node = data_head;
        mark_label_entry(st, line, data_head, counter);
        while(curr_node && (getType(curr_node) != 'c') && getType(curr_node) != 'E'){
            curr_node = getNext(curr_node);
        }
        res = make_command(st, curr_node, line, data_head, counter);
        if(res == 1 &&  getType(curr_node) == 'E') {
            setType(curr_node, 'F'); /*stands for finished with this line and the command is marked entry*/
        }
        else if(res == 1)
            setType(curr_node, 'f'); /*stands for finished with this line*/
        if(res == 0)
          errors = 1;
    }

    if (errors) return FALSE;
    return TRUE;
}

