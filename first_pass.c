#include "first_pass.h"

boolean firstpass_line(char st[], int line, struct Data_Table *head, int *IC, int *DC, int *counter){
    
    int res = add_string_data(st, line, head, DC, counter);
    if(res == 1){
      res = add_extern_data(st, line, head, DC, counter);
      if(res == 2)
        res = make_memory_of_command(st, line, head, IC, counter);
    }
    else if(res == 2){
      res = add_data_data(st, line, head, DC, counter);
      if(res == 2){
        res = make_memory_of_command_label(st, line, head, IC, counter);
        if(res == 2)
          res = add_extern_data(st, line, head, DC, counter);
      }
      
    }

    if(res == 3 || res == 2)
        return TRUE;
    return FALSE;
}
