#include "first_pass.h"



boolean firstpass_line(char st[], int line, struct Data_Table **head, int *IC, int *DC, int *counter){

    int res;
    printf("fp line is: '%s', is head null: %d\n", st, !head);
    
    if (is_label(st)) {

    	res = add_string_data(st, line, &head, DC, counter);
    	printf("after add_string_data res=%d, counter = %d\n", res, *counter);
    	if(res == 2){

      		res = add_data_data(st, line, &head, DC, counter);
      		printf("after add_data_data res=%d, counter=%d\n", res, *counter);

      		if(res == 2){

        		res = make_memory_of_command_label(st, line, &head, IC, counter);
        		printf("after make_memory_of_command_label res=%d, counter=%d\n", res, *counter);

        		if(res == 2) {

          			res = add_extern_data(st, line, &head, DC, counter);
          			printf("after add_extern_data res=%d, counter=%d\n", res, *counter);
        		}

      		}

      

    	}
    }
    	

    else {

      res = add_extern_data(st, line, &head, DC, counter);
      printf("after add_extern_data res=%d, counter = %d\n", res, *counter);

      if(res == 2) {

        res = make_memory_of_command(st, line, &head, IC, counter);
        printf("after make_memory_of_command res=%d, counter = %d\n", res, *counter);
      }

    }
    

    if(res == 3 || res == 2)

        return TRUE;

    return FALSE;

}