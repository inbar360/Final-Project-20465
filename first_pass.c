#include "first_pass.h"







boolean firstpass_line(char st[], int line, struct Data_Table **head, int *IC, int *DC, int *counter){

    int res, res1 = 0;



    if (is_label(st)) {



    	res = add_string_data(st, line, &head, DC, counter);



    	if(res == 2){



      		res = add_data_data(st, line, &head, DC, counter);





      		if(res == 2){



        		res = make_memory_of_command_label(st, line, &head, IC, counter);



        		if(res == 2) {



          			res = add_extern_data(st, line, &head, DC, counter);



        		}



      		}



      



    	}



    }



    	



    else{



      res = add_extern_data(st, line, &head, DC, counter);



      if(res == 2) {



        res = make_memory_of_command(st, line, &head, IC, counter);  



      }



    }

    

    if (res == 2) {

    	res1 = add_string_data(st, line, &head, DC, counter);

    	if (res1 != 4 && res1 != 0) 

    		res1 = add_data_data(st, line, &head, DC, counter);

    }

    

    if((res == 3 || res == 2) && res1 != 4)

        return TRUE;



    return FALSE;



}

