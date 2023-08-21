l#include "second_pass.h"

void make_code_binary(FILE *from, struct Data_Table *data_head) {
    struct Data_Table *curr_node;
    int i = 0, line;
    char st[MAX_LINE];
    for (line = 1; fgets(st, MAX_LINE, from) != NULL; line++) {
        curr_node = data_head;
        mark_label_entry(st, line);
        while(curr_node && (curr_node) != 'c' && getType(curr_node) != 'C'){
            curr_node = getNext(curr_node);
        }
        int res = make_command(st, curr_node, line);
        if(res == 1) {
            setType(curr_node, 'f'); //stands for finished with this line
            return 1;
        }
        if(res == 0)
        return 0;
    }
}

