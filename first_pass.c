#include "first_pass.h"

boolean firstpass_line(char st[], int line){
    
    int res = add_string_data(st, line);
    if(res == 1){
      res = add_extern_data(st, line);
      if(res == 2)
        res = make_memory_of_command(st, line);
    }
    else if(res == 2){
      res = add_data_data(st, line);
      if(res == 2){
        res = make_memory_of_command_label(st, line);
        if(res == 2)
          res = make_extern_data(st, line);
      }
      if(res == 0)
        return FALSE;
      return TRUE;
    }
}
