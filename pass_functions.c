#include "pass_functions.h"


boolean is_label(char st[]) {
    int i = 0, j = 0;
    SKIP_WHITE(st, j);
   
    while(i < 32 && !END_CHAR(st, j+i) && *(st + j + i) != ':' && !IS_WHITE(st, j+i)){
        if(!isalnum(*(st + j + i))) /* If the char is not a valid label-name char, return FALSE. */
            return FALSE;
        i++; /* Add 1 to i. */
    }


    if(*(st + j + i) == ':') return TRUE; /* If the char is ':' return TRUE, otherwise return FALSE. */
    return FALSE;
}


boolean in_list(struct Data_Table *dt, char str[]) {
    struct Data_Table *dt1= dt;


    while(dt1 != NULL) { /* While dt1 is not NULL */
        if(strcmp(str, getData(dt1)) == 0) { /* If the string appears in the list, return TRUE. */
            return TRUE;
        }
        dt1 = getNext(dt1); /* Set dt1 to the next in the list. */
    }
    return FALSE; /* Return FALSE if did not encounter str in the list. */
}


char *opcode_in_binary(char *op) {
    if (strlen(op) < 3) return NULL;


    if(strncmp(op, "mov", 3) == 0)
        return "0000";


    if(strncmp(op, "cmp", 3) == 0)
        return "0001";


    if(strncmp(op, "add", 3) == 0)
        return "0010";


    if(strncmp(op, "sub", 3) == 0)
        return "0011";


    if(strncmp(op, "not", 3) == 0)
        return "0100";


    if(strncmp(op, "clr", 3) == 0)
        return "0101";


    if(strncmp(op, "lea", 3) == 0)
        return "0110";


    if(strncmp(op, "inc", 3) == 0)
        return "0111";


    if(strncmp(op, "dec", 3) == 0)
        return "1000";


    if(strncmp(op, "jmp", 3) == 0)
        return "1001";


    if(strncmp(op, "bne", 3) == 0)
        return "1010";


    if(strncmp(op, "red", 3) == 0)
        return "1011";


    if(strncmp(op, "prn", 3) == 0)
        return "1100";


    if(strncmp(op, "jsr", 3) == 0)
        return "1101";


    if(strncmp(op, "rts", 3) == 0)
        return "1110";


    if(strncmp(op, "stop", 4) == 0)
        return "1111";


    return NULL;
}


char *make_binary(int n, int line) {
    int abs_n, i, j;
    char bin[BITS], *toRet = (char *)malloc(BITS+1);
    int carry = 1;
   
    if(!toRet) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
    toRet[BITS] = '\0';
    printf("val = %d, line = %d\n", n, line);
   
    if(n > 2047 || n < -2048) { /* If the integer is either too large or too small. */
        n > 2047 ? printf("Error on line %d: integer too large | maximum size is 2047", line) : printf("Error on line %d: integer too small | minimum size is -2048.\n", line);
 /* Add 1 to errors. */
        return NULL; /* Return. */
    }


    abs_n = (n < 0) ? -n : n; /* Make negative positive/ keep positive positive - for two's complement. */
    i = BITS - 1; /* Because we use an array of length BITS, the index of the last cell would be (BITS-1), and of the first would be 0. */


    while(i >= 0) { /* While i is not negative. */
        bin[i] = (abs_n % 2) + '0'; /* To make this 0/1 */
        abs_n /= 2; /* Divide abs_n by 2. */
        i--; /* Subtract 1 from i. */
    }
    printf("after while loop bin = '%s'\n", bin);
    if (n < 0) { /* If n is negative. */
        for (j = 0; j < BITS; j++) { /* Invert the bits */
            bin[j] = (bin[j] == '0') ? '1' : '0'; /* If the bit was 0 change to 1, if it was 1 change to 0. */
        }
       
        /* Add 1 to the result */
        for (j = BITS - 1; j >= 0; j--) {
            if (bin[j] == '0' && carry == 1) { /* Find the first empty space (bit '0') to place '1'. */
                bin[j] = '1';
                carry = 0;
            } else if (bin[j] == '1' && carry == 1) { /* If we've not yet found an empty space, change to '0'. */
                bin[j] = '0';
            }
        }
    }


    for (j = 0; j < BITS; j++) {
        *(toRet+j) = bin[j];
    }
    printf("toRet '%s', bin '%s'\n", toRet, bin);
    return toRet;






}






char *organization_type(char *op) {


    int is_num = 0;


    int flag = 0, i = 0, j;






    if((op[0] && op[1] && op[2] && op[3]) && (op[0] == '@') && (op[1] = 'r') && (op[2] <= '7') && (op[2] >= '0') && (IS_WHITE(op, 3) || END_CHAR(op, 3) || op[3] == ',')){


        return "101";


    } /* If the operand is a register, return "101" (5 in binary) */


    j = 0;


    while(!END_CHAR(op, j) && !IS_WHITE(op, j) && op[j] != ',') { /* Check if the operand is a number. */


        if(j == i && op[j] == '-');


        else if (op[j] < '0' || op[j] > '9') {


            is_num = 1;


        }


        j++; /* Add 1 to j. */


    }


    if(is_num == 0) /* If the operand is a valid number, return "001" (1 in binary) */


        return "001";


   


    while(!END_CHAR(op, i) && !IS_WHITE(op, i)) { /* Check if the operand is a valid label name. */


        if(!isalnum(op[i])) /* If found a non-valid label char, mark flag as 1. */


            flag = 1;


        i++; /* Add 1 to i. */


    }






    if(i > LABEL_LENGTH) { /* If the length of the operand is larger than 32, print error, add 1 to errors and return NULL. */


        printf("Error: non legal operand.\n");
        return NULL;


    }


    if(flag == 1) { /* If the label name is not valid, print error. */


        printf("Error: non legal operand.\n");
        return NULL;


    }


    return "011";


}






char *make_command_binary(char *st, char binary[BITS+1]) {


    char *command, *fir_op = NULL, *sec_op = NULL, ARE[2] = "00";


    int i = 0, k, alloc_fir = 0, alloc_sec = 0, is_sec = 0;
    int labelidx = 0;
    int j = 0, comma = 0;
   
    SKIP_WHITE(st, i); /* Get to the first non-white char of the line. */
    if (is_label(st)) {
        labelidx = i;
        while(*(st+labelidx) != ':')
            labelidx++;
       
        labelidx++;
        SKIP_WHITE(st, labelidx);
        command = opcode_in_binary(st + labelidx);
    }
   
    else
        command = opcode_in_binary(st + i); /* Use opcode_in_binary function to save the wanted chars. */
   
    if (!command) {
        printf("Error: undefined command name.\n");
        return NULL;
    }
   
    if (labelidx != 0)
        i = labelidx+1;
       
    SKIP_NON_WHITE(st, i); /* Skip the first word. */


    SKIP_WHITE(st, i);


    if(!END_CHAR(st, i)){ /* If we have not yet reached the end of the line, set fir_op. */
        fir_op = organization_type(st + i);
    }


    while(*(st + i) != ',' && !IS_WHITE(st, i) && !END_CHAR(st, i))
        i++; /* Skip the first operand. */
    SKIP_WHITE(st, i);
    if (*(st+i) != ',' && *(st+i) != '\n') {
        printf("Error: no comma between two operands.\n");
        return NULL;
    }
    if(*(st+i) == ',') {
        i++;
        SKIP_WHITE(st, i);
        comma = 1;
    }


    if(!END_CHAR(st, i)){
            sec_op = organization_type(st + i); /* Set sec_op using organization_type method. */
    }
    else{
        if (comma)
            printf("Error: comma after operand with nothing beyond it.\n");
    }
   
    if(!fir_op) {
        fir_op = (char *)malloc(3);
        if (!fir_op) {
            printf("Error: memory allocation failed.\n");
            exit(1);
        }
        sec_op = (char *)malloc(3);
        if (!sec_op) {
            printf("Error: memory allocation failed.\n");
            free(fir_op);
            exit(1);
        }
        alloc_fir = 1, alloc_sec = 1;
        for(k = 0; k < 3; k++)
            *(fir_op + k) = '0';
        for(k = 0; k < 3; k++)
            *(sec_op + k) = '0';
    }
    else if (!sec_op) {
        sec_op = (char *)malloc(3);
        if (!sec_op) {
            printf("Error: memory allocation failed.\n");
            exit(1);
        }
        alloc_sec = 1;
        for(k = 0; k < 3; k++)
            *(sec_op + k) = '0';
    }
    else
        is_sec = 1;
     for(;j < 3; j++) { /* Add command value to binary array. */
        if(!is_sec)
            binary[j] = *(sec_op + j);
        else
            binary[j] = *(fir_op + j);


    }
    for(;j < 7; j++) { /* Add command value to binary array. */
        binary[j] =*(command + j - 3);


    }


    for(; j < 10; j++) { /* Add second operand value to binary array. */
        if(!is_sec)
            binary[j] = *(fir_op + j - 7);
        else
            binary[j] = *(sec_op + j - 7);
    }


    for(; j < 12; j++) { /* Add ARE value to binary array. */


        binary[j] = ARE[j-10];


    }
   
    printf("returning: '%s'\n", binary);
    if (alloc_fir == 1) free(fir_op);
    if (alloc_sec == 1) free(sec_op);
    return binary;


}






int make_memory_of_command_label(char *st, int line, struct Data_Table ***data_head, int *IC, int *counter) {
    int length = 0, i = 0, white;
    char lab[LABEL_LENGTH];
    struct Data_Table *curr = **data_head, *new;


    for(i = 0; i < (*counter -1); i++) { /* Going to the next empty node */
        curr = getNext(curr);
    }
    i = 0;
    SKIP_WHITE(st, i); /* Skipping white chars. */
    white = i;
    while((i - white) < 32 && !END_CHAR(st, i) && *(st + i) != ':' && !IS_WHITE(st, i)){
        if(!isalnum(*(st+i)))
            return 1; /* means its not a label */


        lab[i] = *(st + i); /* Set the cell in lab to the corresponding cell st. */
        i++; /* Add 1 to i. */
    }


    if(*(st + i) == ':') { /* If the char is ':' */
        if(*(st + (i + 1)) != ' ' && *(st + (i + 1)) != '\t') { /* If the next char is not a white char, print error. */
            printf("Error on line %d: no seperation between label and operands.\n", line);
            return 0;
        }
        else
            i++; /* Add 2 to i. */


        SKIP_WHITE(st, i); /* Skip all white chars. */
        if(opcode_in_binary(st + i) == NULL){
            return 2; /* The label is not for command */
        }
        length++; /* Add 1 to length. */


        if(strcmp(opcode_in_binary(st + i), "1111") == 0) /* The only command of 4 letters is "stop". */
            i += 4; /* Add 4 to i. */
        else
            i += 3; /* Add 3 to i. */


        if(*(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n'){
            printf("Error on line %d: no seperation between command and operands.\n", line);
            return 0;
        }


        SKIP_WHITE(st, i); /* Skip the white chars. */
        if(*(st + i) != '\n') /* If we've not reached the end of the line, add 1 to length. */
            length++;


        SKIP_NON_WHITE(st, i);
        SKIP_WHITE(st, i);


        if(*(st + i) != '\n') /* Add 1 to length if there's another operand. */
            length++;


        SKIP_NON_WHITE(st, i);
        SKIP_WHITE(st, i);


        if(*(st + i) != '\n') { /* If there's another operand, print error.*/
            printf("Error on line %d: too many operands.\n", line);
            return 0;


        }


        new = create_table();
        if(!new) { /* If ran into memory issues, exit. */
            printf("Error: memory allocation failed.\n");
            exit(1);
        }


        setData(new, lab); /* set new's data.*/
        setType(new, 'c'); /* type code  */
        setLength(new, length); /* Set new's length. */
        printf("counter = %d\n", *counter);
        if(*counter == 0) { /* If the list is empty, set new's value to 100, and set data_head to new. */
            setValue(new, IC_INIT_VAL);
            **data_head = new;
        }


        else { /* If the list is not empty, set the value of new based on IC, and set new as curr's next. */
            setValue(new, 100+(*IC));
            setNext(curr, new);
        }
        (*IC) += length; /* Add to IC the amount of operands of the line. */
        (*counter)++; /* Add 1 to counter, indicating that we have at least one data_table variable in the list. */
        return 3; /* done succesfully */
    }


    else {
        printf("line '%s', label = %d\n", st+white, is_label(st+white));
        if (is_label(st+white)) {
            printf("Error in line %d: no ':' in the declaration of the label.\n", line);
            return 0;
        }
       
        return 2;
    }
}






int make_memory_of_command(char *st, int line, struct Data_Table ***data_head, int *IC, int *counter) {
    int length = 0, reg_count, i;
    struct Data_Table *curr = **data_head, *new;


    for(i = 0; i < (*counter -1); i++) { /* Pointing to the next empty node. */
        curr = getNext(curr);
    }


    i = 0;
    SKIP_WHITE(st, i); /* Skip white chars. */
    if(opcode_in_binary(st + i) == NULL) {
        return 2; /* Return 2 if not a command. */
    }


    length++; /* Add 1 to length. */
    if(strcmp(opcode_in_binary(st + i), "1111") == 0)
        i += 4; /* Add 4 to i. */


    else
        i += 3; /* Add 3 to i. */


    if(*(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n') { /* If there's no seperation, return 0. */
        printf("Error in line %d: no seperation between label and operands.\n", line);
        return 0;
    }


    SKIP_WHITE(st, i); /* Skip the white chars. */
        if(*(st + i) != '\n'){ /* If we've not reached the end of the line, add 1 to length. */
            length++;
            if(*(st + i) && *(st + i  + 1) && *(st + i  + 2) && *(st + i  + 3) && (*(st + i) == '@') && (*(st + i  + 1)= 'r') && (*(st + i  + 1) <= '7') && (*(st + i  + 2) >= '0') && (IS_WHITE(st, i + 3) || END_CHAR(st, i + 3))){
                reg_count = 1;
            }
            else if(*(st + i) && *(st + i  + 1) && *(st + i  + 2) && *(st + i  + 3) && (*(st + i) == '@') && (*(st + i  + 1)= 'r') && (*(st + i  + 2) <= '7') && (*(st + i  + 2) >= '0') && !IS_WHITE(st, i + 3) && !END_CHAR(st, i + 3) && *(st+i+3) != ','){
                printf("Error in line %d: no separation between register and a non legal operand", line);
                return 0;
            }
        }


        SKIP_NON_WHITE(st, i);
        SKIP_WHITE(st, i);


        if(*(st + i) != '\n'){ /* Add 1 to length if there's another operand. */
     if(*(st + i) && *(st + i  + 1) && *(st + i  + 2) && *(st + i  + 3) && (*(st + i) == '@') && (*(st + i  + 1)= 'r') && (*(st + i  + 2) <= '7') && (*(st + i  + 2) >= '0') && (IS_WHITE(st, i + 3) || END_CHAR(st, i + 3))){
        if(reg_count == 1);/*dont add to length, if there are two register they will share the binary word*/
            reg_count = 2;  
}


else if(*(st + i) && *(st + i  + 1) && *(st + i  + 2) && *(st + i  + 3) && (*(st + i) == '@') && (*(st + i  + 1)= 'r') && (*(st + i  + 2) <= '7') && (*(st + i  + 2) >= '0') && !IS_WHITE(st, i + 3) && !END_CHAR(st, i + 3)){
printf("Error in line %d: no separation between register and a non legal operand", line);
return 0;
}
else
    length++;
        }


        SKIP_NON_WHITE(st, i);
        SKIP_WHITE(st, i);


        if(*(st + i) != '\n') { /* If there's another operand, print error.*/
            printf("Error on line %d: too many operands.\n", line);
            return 0;


        }


    new = create_table();
    if(!new) { /* If ran into memory issues exit. */
        printf("Error: could not allocate memory for data.\n");
        exit(1);
    }


    setType(new, 'c');/* type code */
    setLength(new, length);


    if(*counter == 0) { /* If the list is empty set value to 100 (initial value), and data_head to new. */


        setValue(new, 100);


        **data_head = new;


    }


    else{ /* If the list is not empty, set value based on IC, and set new to curr's next. */


        setValue(new, 100 + (*IC));


        setNext(curr, new);


    }


    (*IC) += length; /* Add to IC based on length, add 1 to counter. */


    (*counter)++;


    return 3; /* Return 3 if finished succesfully. */


}









int add_extern_data(char *st, int line, struct Data_Table ***data_head, int *DC, int *counter) {

    int i = 0, l = 0, DC_here = *DC;

    int j = 0, in_list = 0, data_count = 0, m;

    int legal = 1;

    char name[LABEL_LENGTH];

    struct Data_Table *data = NULL, *data_temp = data;

    struct Data_Table *curr = **data_head, *temp = **data_head, *new;

    for(i = 0; i < (*counter -1); i++){ /* Setting curr to the next empty node. */

        curr = getNext(curr);

    }

    SKIP_WHITE(st, i);

    if(is_label(st + i)) { /* If encountered a label definition, skip it. */

        while(*(st + i) != ':' && *(st + i) != '\n')
            i++;
    if(*(st + i) == '\n'){
	printf("Error in line %d: no ':' between label and code", line);
	return 0;
}
        if(*(st + ++i) != ' ' && *(st + i) != '\t') { /* If after the ':' there's no white char, print errors. */
		    printf("Error in line %d: no seperation between label and command.\n", line);
			return 0;

		}

		else
		    SKIP_WHITE(st,i); /* Skip white chars. */
    }
 if(*(st + i) == '\n'){
	printf("Error in line %d: found nothing after label", line);
	return 0;
}

    if(strncmp(st + i, ".extern", 7) == 0) {

        i += 7; /* Skip the .extern */

        if(*(st + i) != '\t' && *(st + i) != ' '){ /* Check if there is a space aftre the command. */

            printf("Error in line %d: no seperation between command and operands.\n", line);
			return 0;
        }

        else

            SKIP_WHITE(st, i); /* If there is space, skip it. */

        
       if(*(st + i) == '\n'){
	  printf("Error in line %d: no operands after comand.\n", line);
		return 0;
     }
        while (st[i] != '\n') {

            l = i;

            j = 0, in_list = 0, legal = 1;



            while(*(st + l) != ' ' && *(st + l) != '\t' && *(st + l) != '\n' && *(st + l) != ',')
                l++;



            while((j < *counter) && (in_list = 0)) { /* Going over the list until encountering the label name/ reached the end of the list. */

                if((strncmp(getData(temp), st + i, l-i) == 0) && (strlen(getData(temp)) == l-i)){

                    in_list = 1; /* If the label appears in the list, print error and add to errors variables. */

                    printf("Error in line %d: declared an existing internal label external.\n", line);
					return 0;

                }

                j++; /* Add 1 to j, and set temp to the next node in the list. */

                temp = getNext(temp);

            }

            

            if((l-i) > LABEL_LENGTH) { /* If the label name is too long, print error, add to errors variables, and set legal to 0. */

                printf("Error in line %d: label length longer than maximum of 32 letter.\n", line);
				return 0;
            }



            for(m = i; m < l; m++) { /* start at the begiening of the current label and run until you reached its end */

                if(!isalnum(*(st + m))) {

                    printf("Error in line %d: label name is not legal.\n", line);
					return 0;

                }

            }

            

            new = create_table();

            if (!new) {

                printf("Error: Memory allocation failed.\n");
                exit(1);

            }



            if(legal == 1) { /* If the label name is legal, set as new's data. */

                for(m = i; m < l; m++)

                    name[m-i] = *(st + m);

                setData(new, name);

            }


			SKIP_WHITE(st, l);/*set l to the next word*/
			if(*(st + l) != ',' && *(st + l) != '\n'){
				printf("Error in line %d: no ',' between operands.\n", line);
				return 0;
			}
			l = *(st + l) == ',' ? l + 1: l;
			SKIP_WHITE(st, l);
            i = l; /*set i to the next word*/
            
            setType(new, 'x');/* x stand for extern, needed for the function mark_label_entry and make_command */

            setLength(new, 1);/* its length is only 1 because its value is not in this file and we can only asign a type and base*/

            setValue(new, 100 + DC_here++);/* add one to DC_here*/

            if (data_count == 0) 

                data = new;

            else {

                setNext(data_temp, new);

                data_temp = getNext(data_temp); 

            }

            data_count++; /* Add 1 to counter, and return 1. */

        }   



        data_temp = data;

        for (i = 0; i < data_count; i++) {

            if (*counter == 0) {

                **data_head = data_temp;

                (*counter)++;

            }

            else {

                setNext(curr, data_temp);

                curr = getNext(curr);

                (*counter)++;

            }

            data_temp = getNext(data_temp);

        }



        *DC = DC_here;

        return 3;

    }

    else

        return 2; /* Return 2 if not extern. */

}










int mark_label_entry(char *st, int line, struct Data_Table ***data_head, int *counter) { /* Second pass function */

    int i = 0, l;

    int j = 0, in_list = 0;

    Data_Table *curr = **data_head, *temp = **data_head; 

    SKIP_WHITE(st, i);

    if(is_label(st + i)){

        while(*(st + i) != ':')
            i++;
        
        if(*(st + ++i) != ' ' && *(st + i) != '\t') { /* Print error and add to errors variables. */
        	printf("Error in line %d: no seperation between label and command.\n", line);
        	return 0;
    	}
    	SKIP_WHITE(st,i); /* Skip white chars. */
    }

    if(strncmp(st + i, ".entry", 6) == 0) { 

        i += 6; /* Skip ".entry" */

        if(*(st + i) != '\t' && *(st + i) != ' ') { /* Check if there is a space aftre the command */
            printf("Error in line %d: no seperation between command and operands.\n", line);
			return 0;
        }
        
        else
            SKIP_WHITE(st, i); /*if there is a space after the command skip it*/
       if(*(st + i) == '\n'){
	  printf("Error in line %d: no operands after comand.\n", line);
			return 0;
	}
	
        while(st[i] != '\n') {

            l = i;
	j = 0, in_list = 0;
             while(*(st + l) != ' ' && *(st + l) != '\t' && *(st + l) != '\n' && *(st + l) != ',')
                l++;

            
            while((j < *counter) && (in_list == 0)) { /* Checking if the label name appears in the list. */
                if((strncmp(getData(temp), st + i, l-i) == 0)) 
                    in_list = 1;
                temp = getNext(temp);
                j++;
            }

            if(l - i > 32){
                printf("Error in line %d: label length longer than maximum of 32 letter.\n", line);
                return 0;
            }

            for(i = 0; i < j - 1; i++) { /* Setting curr to the node with the wanted name. */
                curr = getNext(curr);
            }

			if(getType(curr) == 'x') { /* If the label appears as external, print error. */
                printf("Error in line %d: tried to declare an external label an entry.\n", line);
				return 0;
            }

            if (!in_list) {
                printf("Error in line %d: tried declaring a non-existent label as entry.\n", line);
				return 0;
            }

            else if(getType(curr) == 'c')
                setType(curr, 'E');

            else 
                setType(curr, 'e');

            SKIP_WHITE(st, l);/*set l to the next word*/
	if(*(st + l) != ',' && *(st +  l) != '\n'){
		printf("Error in line %d: no ',' between operands.\n", line);
		return 0;
	}
	l = *(st + l) == ',' ? l + 1: l;
SKIP_WHITE(st, l);
            i = l; /*set i to the next word*/
        }

        return 3;
    }
    return 2;
}










int add_string_data(char *st, int line, struct Data_Table ***data_head, int *DC, int *counter){/*adds labels of .string type*/


    Data_Table *curr = **data_head, *new;


    char lab[LABEL_LENGTH], *temp;


    char bin[MAX_MEMORY][BITS+1], *string;


    int i, white, k, z;


    int length = 0;
   
    printf("before the first for loop.\n");
    for(i = 0; i < (*counter -1); i++){ /*going to the next empty node*/
        printf("null? %d\n", !curr);
        curr = getNext(curr);
    }
   
    printf("after the first for loop\n");
    for (i = 0; i < MAX_MEMORY; i++) {
        bin[i][BITS] = '\0';
    }
    printf("after the for loop.\n");
    i = 0;


    SKIP_WHITE(st, i);


    white = i;


    while(i - white < 32 && *(st + i) != '\0' && *(st + i) != ':' && *(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n'){/*next two lines check if string is a labal*/


        if((*(st + i) > 57 && *(st + i) < 65) || *(st + i) < 48 || (*(st + i) > 90 && *(st + i) < 97) || *(st + i) > 122)


            return 1; /*means its not a label*/


        lab[i - white] = *(st + i);


        i++;


    }


    if(*(st + i) == ':'){ /*check if the the label is legaly terminated with ':'*/


       
        printf("noticed it's a label!\n");
        string = (char*) malloc(sizeof(char));
        if (!string) {
            printf("Error: Memory allocation failed.\n");
            exit(1);
        }


        if(*(st + (i + 1)) != ' ' && *(st + (i + 1)) != '\t'){/*check if there is a space after the label*/


            printf("Error on line %d: no seperation between label and operands.\n", line);
            free(string);
            return 0;


        }


        else


            i++;


        SKIP_WHITE(st, i); /*go to the first word after the label*/


        if(strncmp(st+i, ".string", 7) == 0){/* check if command is string*/


            i += 7;


            if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space after the command*/


                printf("Error in line %d: no seperation bitween .string and the string.\n", line);
                free(string);
                return 0;
            }


            SKIP_WHITE(st, i);
            printf("before check: '%s'\n", st+i);
            if(*(st + i) != '"'){ /*check if string is not marked first by " */


                printf("Error in line %d: no quotation marks in the beginning of the string.\n", line);
                free(string);
                return 0;


   


            }


            ++i;


            while(*(st + i) != '"' && *(st + i) != '\n'){ /*until you reach the end of string*/


                if(*(st + i) <= 31 || *(st + i) >= 128){ /*check if string char is printabble*/


                    printf("Error in line %d: non printable char in string.\n", line);
                    free(string);
                    return 0;


       


                }


                *(string + length) = *(st + i); /*if add char to the char array*/


                length++;


                i++;


            }


            if(*(st + i) == '\n'){ /* check if there was no " terminator to the string*/


                printf("Error in line %d: no quotation marks in the end of the string.\n", line);
                free(string);
                return 0;


            }


            i++;


            SKIP_WHITE(st, i);


            if(*(st + i) != '\n'){


                printf("Error in line %d: to many operands in the declaration of \".string\".\n", line);
                free(string);
                return 0;


            }


            if(in_list(**data_head, lab) == 1){


                printf("Error in line %d: label - '%s' declared multiple times.\n", line, lab);
                free(string);
                return 0;
            }


            new = create_table();


            if(!new){


                printf("Error: could not allocate memory for data.\n");
                free(string);
                exit(1);


            }


            setType(new, 'n');


            setLength(new, length+1); /* plus 1 is for the NULL terminator */


            setData(new, lab);


            setNext(new, NULL);


           


            for(k = 0; k < length; k++){


                temp = make_binary(*(string + k), line);


                for(z = 0; z < 12; z++){


                     bin[k][z] = temp[z];


                }
                free(temp);


            }


            for(z = 0; z < 12; z++)


                bin[k][z] = '0'; /*add null terminator*/


           


            setBinary(new, bin, length+1); /* plus 1 for null terminator */
            if((*counter) == 0){


                setValue(new, 100);


                **data_head = new;
            }


            else{


                setValue(new, 100+(*DC));


                setNext(curr, new);


            }


            *DC += length + 1;/*plus 1 is for the NULL terminator */


            (*counter)++;/* seccesfully added a data table node */
            free(string);
            return 3;/* means that it succesfully added the data */


        }
       
        free(string);
        return 2;/*means that the label is not for string */
    }


    else if(i - white == 32){


        printf("Error in line %d: label longer than the maximum length of 32.\n", line);


        return 0;


    }


    else{
        if (is_label(st+white)) {
            printf("Error in line %d: no ':' in the declaration of the label.\n", line);
            return 0;
        }
       
        return 2;
    }


}










int add_data_data(char *st, int line, struct Data_Table ***data_head, int *DC, int *counter){ /*adds labels of .data type */


    struct Data_Table* curr = **data_head, *new;


    int i, minus = 0, k, white, z;


    int num, m = 0;


    int d_counter;


    int data_values[MAX_MEMORY];


    char lab[LABEL_LENGTH], *temp;


    char bin[MAX_MEMORY][BITS+1], *number;


    for(i = 0; i < (*counter -1); i++){ /* going to the next empty node */


        curr = getNext(curr);


    }


    for (i = 0; i < MAX_MEMORY; i++) {
        bin[i][BITS] = '\0';
    }


    i = 0;


    SKIP_WHITE(st, i);


    white = i;


    while(i - white < 32 && *(st + i) != '\0' && *(st + i) != ':' && *(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n'){


        if((*(st + i) > 57 && *(st + i) < 65) || *(st + i) < 48 || (*(st + i) > 90 && *(st + i) < 97) || *(st + i) > 122)


            return 1;  


        lab[i - white] = *(st + i);


        i++;


    }


    if(*(st + i) == ':'){
        printf("figured out label\n");
        if(*(st + (i + 1)) != ' ' && *(st + (i + 1)) != '\t'){


            printf("Error on line %d: no seperation between label and operands.\n", line);
            return 0;


        }
       
        else


            i++;


        SKIP_WHITE(st, i);


        if(strncmp(st + i, ".data", 5) == 0){
            printf("figured out .data\n");
            i += 5;


            if(*(st + i) != '\t' && *(st + i) != ' '){


                printf("Error in line %d: no seperation between ':' and .data.\n", line);
                return 0;
            }


            SKIP_WHITE(st, i);


            d_counter = 0;


            while(*(st + i) != '\n'){


                minus = 0;


                if(*(st + i) == '-'){


                    i++;


                    minus = 1;


                }


                num = 0;
                m = 0;
                while(*(st + (i + m)) != '\t' && *(st + (i + m)) != ' ' && *(st + (i + m)) != '\n' && *(st + (i + m)) != ','){
                    m++;
                }
               
                number = (char *)malloc(m+1); /* +1 for null terminator. */
                if (!number) {
                    printf("Error: Memory allocation failed.\n");
                    exit(1);
                }
           
                number[m] = '\0';
                strncpy(number, st+i, m);
                num = atoi(number);
                free(number);
                printf("num is: %d\n", num);
                if(minus == 1)


                    data_values[d_counter] = num * -1;


                else


                    data_values[d_counter] = num;
                i += m;
                SKIP_WHITE(st, i);
               
                if (*(st+i) == '\n') {
                    d_counter++;
                    break;
                }
               
                if(*(st + i) != ','){


                    printf("Error in line %d: two operands not seperated by ','.\n", line);
                    return 0;
                }


                i++;


                SKIP_WHITE(st, i);


                d_counter++;


            }


            new = create_table();


            if(!new){


                printf("Error: could not allocate mamory for data.\n");
                return 0;
            }


            setType(new, 'n');


            setLength(new, d_counter);


            setData(new, lab);


            setNext(new, NULL);






            for(k = 0; k < d_counter; k++){


                temp = make_binary(data_values[k], line);


                for(z = 0; z < 12; z++){


                    bin[k][z] = temp[z];


                }
                free(temp);
            }






            setBinary(new, bin, d_counter);


            if(*counter == 0){


                setValue(new, 100);


                **data_head = new;


            }


            else{


                setValue(new, 100+(*DC));


                setNext(curr, new);


            }


            *DC += d_counter;


            (*counter)++;


            return 3; /* success */


        }


        return 2; /* not for data */






    }


    else if(i - white == 32){


        printf("Error in line %d: label longer then the maximum length of 32.\n", line);
        return 0;
    }


    else{
        if (is_label(st+white)) {
            printf("Error in line %d: no ':' in the declaration of the label.\n", line);
            return 0;
        }
       
        return 2;
    }
   
}


int make_command(char *st, struct Data_Table **curr_node, int line, struct Data_Table ***data_head, int *counter){ /* Second pass function */


    int i = 0, t;
    int l = 0, m = 0, only_num = 1, neg = 0, g;
    int in = 0, j = 0, k = 0, z, reg_count = 0;
    int num = 0, length;
    char *ARE, *number, *temp = NULL;
    char binary_word[BITS+1], *temp_bin, bin[1024][BITS+1];
    char first_binary_word[BITS+1], second_binary_word[BITS+1];
    struct Data_Table *temp_data = **data_head;
    printf("line %d is: \"%s\"\n", line, st);
    first_binary_word[BITS] = '\0', second_binary_word[BITS] = '\0', binary_word[BITS] = '\0';
    for (i = 0; i < MAX_MEMORY; i++) {
        bin[i][BITS] = '\0';
    }
    i = 0;
   
    SKIP_WHITE(st, i);
    if(*(st + i) != '.') {
        length = i;
        if (is_label(st+i)) {
            while((*(st + length) != ':') && (length < (33 + i))){
                length++;
            }


            if(st[length] != ':'){
                printf("Error in line %d: label is too long, max %d.\n", line, LABEL_LENGTH);
                return 0;
            }
        /***********    NOTICE HERE YOU HAVE PROBLEM WITH SEPERATION    ***********/
            else
                i = length;
           
            i++;
            if(st[i] != ' ' && st[i] != '\t' && st[i] != '\n'){
                printf("Error in line %d: no seperation between label and command.\n", line);
                return 0;
            }


            SKIP_WHITE(st, i);
            printf("after skipwhite: '%s'\n", st+i);
        }
    }


    /*no operand commands*/


    if(strncmp(st + i, "stop", 4) == 0){
        i += 4;/*go to the char after the command */
        SKIP_WHITE(st, i);
        if(*(st + i) != '\n'){/*if there are chars that arent white after the word */
            printf("///Error in line %d: operand found in an operand-less command.\n", line);
            return 0;/*wrong syntax*/
        }


        temp = make_command_binary(st, bin[0]);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        setBinary(*curr_node, bin, 1);


        return 1;
    }


    else if(strncmp(st + i, "rts", 3) == 0){
        i += 3;/*go to the char after the command*/
        SKIP_WHITE(st, i);


        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/
            printf("///Error in line %d: operand found in an operand-less command.\n", line);
            return 0;/*wrong syntax*/
        }


        temp = make_command_binary(st, bin[0]);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        setBinary(*curr_node, bin, 1);


        return 1;
    }


    /*one operand commands with 3 and 5 organization type operands*/


    else if(strncmp(st + i, "not", 3) == 0 || strncmp(st + i, "clr", 3) == 0 || strncmp(st + i, "inc", 3) == 0 || strncmp(st + i, "dec", 3) == 0 || strncmp(st + i, "jmp", 3) == 0 || strncmp(st + i, "bne", 3) == 0 || strncmp(st + i, "red", 3) == 0 || strncmp(st + i, "jsr", 3) == 0){ /*3 and 5 */
   
        i += 3;/*go two chars after the command*/
        if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space aftre the command*/
            printf("Error in line %d: no seperation between comand and operands.\n", line);
            return 0;
        }


        SKIP_WHITE(st, i);
        if(*(st + i) == '\n'){
            printf("Error in line %d: no operands in a single operand function.\n", line);
            return 0;
        }


        l = i;
        if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/


            while(*(st + l) == '\n' && *(st + l) == '\t' && *(st + l) == ' ')
				l++;
            printf("l=%d, i=%d line: '%s'\n", l-i, i, st+i);
            in = 0, j = 0;
            while(j < *counter && in != 1){
                if(strncmp(getData(temp_data), st + i, l-i) == 0)
                    in = 1;
                   
                else {
                    temp_data = getNext(temp_data);
                    j++;
                }
            }


            if(in != 1){
                printf("Error in line %d: reference to an undeclared label.\n", line);
                return 0;
            }
           
            else{ /* The label appears in the list. */
                printf("noticed it's in\n");
                temp_bin = make_binary(getValue(temp_data), line);
                for(k = 0; k < 10; k++)
                    temp_bin[k] = temp_bin[k + 2];


                ARE = (getType(temp_data) == 'x') ? "01\0" : "10\0"; /* If the label is extern, ARE is 01, else 10. */
                printf("ARE = %s\n", ARE);
                for(k = 10; k < BITS; k++)
                    temp_bin[k] = ARE[k - 10];


                for (k = 0; k < BITS; k++) {
                    binary_word[k] = temp_bin[k];
                }
                printf("binary_word = '%s'\n", binary_word);
                free(temp_bin);
            }
        }


        else { /* The wanted operand is a register. */


            temp_bin = make_binary(*(st + (i + 2)) - '0', line);


            for(; k < 5; k++)
                temp_bin[k] = temp_bin[k + 7];


            for(; k < 12; k++)
                temp_bin[k] = '0';


            for (k = 0; k < BITS; k++) {
                binary_word[k] = temp_bin[k];


            }
            free(temp_bin);
        }


        SKIP_NON_WHITE(st, i);
        SKIP_WHITE(st, i);


        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/
            printf("Error in line %d: multiple operand in a single operand function.\n", line);/*wrong syntax*/
            return 0;
        }
        printf("before make_command_binary\n");
        temp = make_command_binary(st, bin[0]);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        strcpy(bin[1], binary_word);
        setBinary(*curr_node, bin, 2);


        return 1;
    }
   
    /*one operand commands with 1, 3 and 5 organization type operands*/
    else if(strncmp(st + i, "prn", 3) == 0){
        printf("prn!\n");
        i += 3;/*go two chars after the command*/
        if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space aftre the command*/
            printf("Error in line %d: no seperation between comand and operands.\n", line);
            return 0;
        }


        SKIP_WHITE(st, i);
        if(*(st + i) == '\n'){
            printf("Error in line %d: no operands in a single operand function.\n", line);
            return 0;
        }


        while(*(st + (i + m)) != '\t' && *(st + (i + m)) != ' ' && *(st + (i + m)) != '\n'){
            printf("cur line: '%s'\n", st+i+m);
            if(*(st + i) == '-' && m == 0);
            else if(*(st + (i + m)) < '0' || *(st + (i + m)) > '9')
                    only_num = 0;


            m++;
        }
        printf("reached here with only_num %d\n", only_num);
        if(only_num == 1) {
            number = (char *)malloc(m+1); /* +1 for null terminator. */
            if (!number) {
                printf("Error: Memory allocation failed.\n");
                exit(1);
            }
           
            number[m] = '\0';
            strncpy(number, st+i, m);
            num = atoi(number);
            free(number);
   
            temp_bin = make_binary(num, line);
            printf("finished make_binary!\n");
            for(z = 0; z < 12; z++){
                if(z > 9)
                    temp_bin[z] = '0';
                else
                    temp_bin[z] = temp_bin[z + 2];
            }


            for (k = 0; k < BITS; k++) {
                binary_word[k] = temp_bin[k];
            }
            printf("after the for loops!\n");
            free(temp_bin);
        }


        else if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/
            t = i;
            while(*(st + t) != ' ' && *(st + t) != '\t' && *(st + t) != '\n') {
                l++;
                t++;
            }


            in = 0, j = 0;


            while(j < *counter && in != 1){
                if(strncmp(getData(temp_data), st + i, l) == 0)
                    in = 1;
                else{
                    temp_data = getNext(temp_data);
                    j++;
                }
            }


            if(in != 1){
                printf("Error in line %d: reference to an undeclared label.\n", line);
                return 0;
            }


            else{
                temp_bin = make_binary(getValue(temp_data), line);


                for(k = 0; k < 10; k++)
                    temp_bin[k] = temp_bin[k + 2];


                ARE = (getType(temp_data) == 'x') ? "01" : "10";


                for(k = 10; k < 12; k++)
                    temp_bin[k] = ARE[k - 10];


                for (k = 0; k < BITS; k++) {
                    binary_word[k] = temp_bin[k];
                }
                free(temp_bin);
            }
        }


        else{
            temp_bin = make_binary(*(st + (i + 2)) - '0', line);
            k = 0;
           
            for(; k < 5; k++)
                temp_bin[k] = temp_bin[k + 7];
               
            for(; k < 12; k++)
                temp_bin[k] = '0';


            for (k = 0; k < BITS; k++) {
                binary_word[k] = temp_bin[k];
            }
            free(temp_bin);
        }


        SKIP_NON_WHITE(st, i);


        SKIP_WHITE(st, i);


        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/


            printf("Error in line %d: multiple operand in a single operand function.\n", line);/*wrong syntax*/
            return 0;


        }
       
        printf("before make command binary!\n");
        temp = make_command_binary(st, bin[0]);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        printf("after make command binary! bin[0] is: '%s' \n", bin[0]);
        bin[1][BITS] = '\0';
        strcpy(bin[1], binary_word);
        printf("before setBinary! bin[1] is: '%s'\n", bin[1]);
        setBinary(*curr_node, bin, 2);
        printf("after setBinary!\n");


        return 1;


    }






    /*two operand commands with 3 organization type first operand and 3 and 5 organization type second operand*/






    else if(strncmp(st + i, "lea", 3) == 0){


       


        i += 3;/*go two chars after the command*/


        if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space aftre the command*/


            printf("Error in line %d: no seperation between command and operands.\n", line);
            return 0;


        }






        SKIP_WHITE(st, i);


        if(*(st + i) == '\n'){


            printf("Error in line %d: no operands in a two operand function.\n", line);
            return 0;


        }






        l = 0;


        while(*(st + i + l) != ' ' && *(st + i + l) != '\t' && *(st + i + l) != '\n' && *(st + i + l) != ',')


            l++;


        in = 0, j = 0;


        while(j < *counter && in != 1){


            if(strncmp(getData(temp_data), st + i, l) == 0)


                in = 1;


            else{


                temp_data = getNext(temp_data);


                j++;


            }


        }


        if(in != 1){




            printf("Error in line %d: reference to an undeclared label.\n", line);
            return 0;


        }


        else{


            temp_bin = make_binary(getValue(temp_data), line);


            for(k = 0; k < 10; k++)


                temp_bin[k] = temp_bin[k + 2];


            ARE = (getType(temp_data) == 'x') ? "01" : "10";


            for(k = 10; k < 12; k++)


                temp_bin[k] = ARE[k - 10];






            for(k = 0; k < BITS; k++) {


                first_binary_word[k] = temp_bin[k];


            }
            free(temp_bin);
        }


        temp_data = **data_head;


        while(*(st + i) != ',' && !IS_WHITE(st, i) && !END_CHAR(st, i))
    i++;


        SKIP_WHITE(st, i);


        if(*(st + i) == '\n'){


            printf("Error in line %d: only one operand in a two operand function.\n", line);
            return 0;


        }


        if (*(st+i) != ',') {
            printf("Error in line %d: missing comma between the two operands.\n", line);    
        }
	   else 
		i++;
	SKIP_WHITE(st, i);
	  

        l = 0;


        if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/


            while(*(st + i + l) != ' ' && *(st + i + l) != '\t' && *(st + i + l) != '\n' && *(st +i + l) != ',')


                l++;


            in = 0, j = 0;


            temp_data = **data_head;


            while(j < *counter && in != 1){


                if(strncmp(getData(temp_data), st + i, l) == 0)


                    in = 1;


                else{


                    temp_data = getNext(temp_data);


                    j++;


                }


            }


            if(in != 1){


                printf("Error in line %d: reference to an undeclared label.\n", line);
                return 0;


            }


            else{


                temp_bin = make_binary(getValue(temp_data), line);


                for(k = 0; k < 10; k++)


                    temp_bin[k] = temp_bin[k + 2];


                ARE = (getType(temp_data) == 'x') ? "01" : "10";


                for(k = 10; k < BITS; k++)


                    temp_bin[k] = ARE[k - 10];






                for (k = 0; k < BITS; k++) {


                    second_binary_word[k] = temp_bin[k];


                }
                free(temp_bin);




            }


        }


        else{


            temp_bin = make_binary(*(st + (i + 2)) - '0', line);


            k = 0;


            for(; k < 5; k++)


                temp_bin[k] = temp_bin[k + 7];


            for(; k < 12; k++)


                temp_bin[k] = '0';






            for (k = 0; k < BITS; k++) {


                second_binary_word[k] = temp_bin[k];


            }
            free(temp_bin);
        }


        SKIP_NON_WHITE(st, i);


        SKIP_WHITE(st, i);


        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/


            printf("Error in line %d: more then two operands in a two operand function.\n", line);/*wrong syntax*/
            return 0;


        }
       
        temp = make_command_binary(st, bin[0]);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        bin[1][BITS] = '\0';
        strcpy(bin[1], first_binary_word);
        printf("bin[1] is: %s\n", bin[1]);
        strcpy(bin[2], second_binary_word);
        setBinary(*curr_node, bin, 3);


        return 1;


    }






        /*two operand commands with 1, 3 and 5 organization type first operand and 3 and 5 organization type second operand*/






    else if(strncmp(st + i, "mov", 3) == 0 || strncmp(st + i, "add", 3) == 0 || strncmp(st + i, "sub", 3) == 0){
        printf("noticed the command!\n");
        i += 3;/*go two chars after the command*/


        if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space aftre the command*/


            printf("Error in line %d: no seperation bitween command and operands.\n", line);


            return 0;


        }






        SKIP_WHITE(st, i);


        if(*(st + i) == '\n'){


            printf("Error in line %d: no operands in a two operand function.\n", line);
            return 0;


        }






        l = 0, m = 0, only_num = 1, neg = 0;


        while(*(st + (i + m)) != '\t' && *(st + (i + m)) != ' ' && *(st + (i + m)) != '\n'){
           
            if(*(st + i) == '-' && m == 0)


                neg = 1;


            else if(*(st + (i + m)) < '0' || *(st + (i + m)) > '9')


                    only_num = 0;


            m++;


        }


        if(only_num == 1){


            number = (char *)malloc(m+1); /* +1 for null terminator. */
            if (!number) {
                printf("Error: Memory allocation failed.\n");
                exit(1);
            }
           
            number[m] = '\0';
            strncpy(number, st+i, m);
            num = atoi(number);
            free(number);


            if(neg == 1)


                num = -num;
            printf("num=%d\n", num);
               


            temp_bin = make_binary(num, line);


            for(z = 0; z < BITS; z++){


                if(z > 9)


                    temp_bin[z] = '0';


                else


                    temp_bin[z] = temp_bin[z + 2];


            }






            for (z = 0; z < BITS; z++) {


                first_binary_word[z] = temp_bin[z];


            }
            free(temp_bin);
        }






        else if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/
            l = 0;
            while(*(st + i + l) != ' ' && *(st + i + l) != '\t' && *(st + i + l) != '\n' && *(st + i + l) != ',')
                l++;
            in = 0, j = 0;


            while(j < *counter && in != 1){


                if(strncmp(getData(temp_data), st + i, l) == 0)


                    in = 1;


                else{


                    temp_data = getNext(temp_data);


                    j++;


                }


            }


            if(in != 1){


                printf("Error in line %d: reference to an undeclared label.\n", line);
                return 0;


            }


            else{


               
                temp_bin = make_binary(getValue(temp_data), line);


                for(k = 0; k < 10; k++)


                    temp_bin[k] = temp_bin[k + 2];


                ARE = (getType(temp_data) == 'x') ? "01" : "10";


                for(k = 10; k < BITS; k++)


                    temp_bin[k] = ARE[k - 10];






                for (k = 0; k < BITS; k++) {


                    first_binary_word[k] = temp_bin[k];


                }
            }


        }


        else{
            printf("noticed its a register, now make_binary\n");
            reg_count++;
            temp_bin = make_binary(*(st + (i + 2)) - '0', line);
            printf("temp_bin = '%s'\n", temp_bin);
            printf("after make_binary\n");
            k = 0;


            for(; k < 5; k++)


                temp_bin[k] = temp_bin[k + 7];


            for(; k < BITS; k++)


                temp_bin[k] = '0';






            for (k = 0; k < BITS; k++) {


                first_binary_word[k] = temp_bin[k];
               
            }
            free(temp_bin);
           


        }
        printf("after first operand! first = %s\n", first_binary_word);
       
        while(*(st + i) != ',' && !IS_WHITE(st, i) && !END_CHAR(st, i))
    i++;
        SKIP_WHITE(st, i);


        if(*(st + i) == '\n'){


            printf("Error in line %d: only one operand in a two operand function.\n", line);
            return 0;


        }


        if (*(st+i) != ',') {
            printf("Error in line %d: missing comma between the two operands.\n", line);    
        }
       
        i++;
        SKIP_WHITE(st, i);
        l = 0;
        temp_data = **data_head;


        if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/


            while(*(st + i + l) != ' ' && *(st + i + l) != '\t' && *(st + i + l) != '\n' && *(st + i + l) != ',')


                l++;


            in = 0, j = 0;


           


            while(j < *counter && in != 1){


                if(strncmp(getData(temp_data), st + i, l) == 0)


                    in = 1;


                else{


                    temp_data = getNext(temp_data);


                    j++;


                }


            }


            if(in != 1){
                printf("Error in line %d: reference to an undeclared label.\n", line);
                return 0;


            }


            else{


               


                temp_bin = make_binary(getValue(temp_data), line);


                for(k = 0; k < 10; k++)


                    temp_bin[k] = temp_bin[k + 2];


                ARE = (getType(temp_data) == 'x') ? "01" : "10";


                for(k = 10; k < BITS; k++)


                    temp_bin[k] = ARE[k - 10];






                for (k = 0; k < BITS; k++) {


                    second_binary_word[k] = temp_bin[k];
                   
                }
                free(temp_bin);




            }


        }


        else{


           
            reg_count++;
            temp_bin = make_binary(*(st + (i + 2)) - '0', line);


            k = 0;


            for(; k < 5; k++)


                temp_bin[k] = temp_bin[k + 7];


            for(; k < BITS; k++)


                temp_bin[k] = '0';






            for (k = 0; k < BITS; k++) {


                second_binary_word[k] = temp_bin[k];
               
            }


            free(temp_bin);


        }


        printf("second = %s\n", second_binary_word);


        SKIP_NON_WHITE(st, i);


        SKIP_WHITE(st, i);


        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/


            printf("Error in line %d: more then two operands in a two operand function.\n", line);/*wrong syntax*/
            return 0;
        }
       
        temp = make_command_binary(st, bin[0]);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
       if(reg_count == 2){
    for(g = 5; g < 10; g++)
        first_binary_word[g] = second_binary_word[g - 5]; /*concat the binary words
of the registers*/
    strcpy(bin[1], first_binary_word);
    printf("bin[1] = %s, first = %s, second = %s\n", bin[1], first_binary_word, second_binary_word);
    setBinary(*curr_node, bin, 2);
return 1;
       }
        strcpy(bin[1], first_binary_word);
        printf("bin[1] = %s, first = %s, second = %s\n", bin[1], first_binary_word, second_binary_word);
        strcpy(bin[2], second_binary_word);
        printf("bin[1] = %s\n", bin[1]);
        setBinary(*curr_node, bin,3);


        return 1;


    }






        /*two operand commands with 1, 3 and 5 organization type first operand and 1, 3 and 5 organization type second operand*/






    else if(strncmp(st + i, "cmp", 3) == 0){


        i += 3; /*go two chars after the command*/


        if(*(st + i) != '\t' && *(st + i) != ' '){ /*check if there is a space aftre the command*/


            printf("Error in line %d: no seperation between command and operands.\n", line);
            return 0;
        }






        SKIP_WHITE(st, i);


        if(*(st + i) == '\n'){


            printf("Error in line %d: no operands in a two operand function.\n", line);
            return 0;


        }






        l = 0, m = 0, only_num = 1, neg = 0;


        while(*(st + (i + m)) != '\t' && *(st + (i + m)) != ' ' && *(st + (i + m)) != '\n'){


            if(*(st + i) == '-')


                neg = 1;


            else if(*(st + (i + m)) < '0' || *(st + (i + m)) > '9')


                    only_num = 0;


            m++;


        }


        if(only_num == 1){
            num = 0;


            for(z = 0; z < m; z++){


                if(neg == 1);


                else


                    num = num * 10 + (*(st + (i + z)) - '0');


            }


            if(neg == 1)


                num = -num;


               


            temp_bin = make_binary(num, line);


            for(z = 0; z < BITS; z++){


                if(z > 9)


                    temp_bin[z] = '0';


                else


                    temp_bin[z] = temp_bin[z + 2];


            }






            for (z = 0; z < BITS; z++) {


                first_binary_word[z] = temp_bin[z];
               
            }


            free(temp_bin);


        }


        else if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/
            l = 0;
            while(*(st + i + l) != ' ' && *(st + i + l) != '\t' && *(st + i + l) != '\n' && *(st + i + l) != ',')
                l++;


            in = 0, j = 0;






            while(j < *counter && in != 1){


                if(strncmp(getData(temp_data), st + sizeof(char) * i, l) == 0)


                    in = 1;


                else{


                    temp_data = getNext(temp_data);


                    j++;


                }


            }


            if(in != 1){
                printf("Error in line %d: reference to an undeclared label.\n", line);
                return 0;


            }


            else{


               


                temp_bin = make_binary(getValue(temp_data), line);


                for(k = 0; k < 10; k++)


                    temp_bin[k] = temp_bin[k + 2];


                ARE = (getType(temp_data) == 'x') ? "01" : "10";


                for(k = 10; k < BITS; k++)


                    temp_bin[k] = ARE[k - 10];






                for (k = 0; k < BITS; k++) {


                    first_binary_word[k] = temp_bin[k];
                   
                }


                free(temp_bin);


            }


        }


        else{


           
            reg_count++;
            temp_bin = make_binary(*(st + (i + 2)) - '0', line);


            k = 0;


            for(; k < 5; k++)


                temp_bin[k] = temp_bin[k + 7];


            for(; k < BITS; k++)


                temp_bin[k] = '0';


           


            for (k = 0; k < BITS; k++) {


                first_binary_word[k] = temp_bin[k];
               
            }


            free(temp_bin);


        }


       


        while(*(st + i) != ',' && !IS_WHITE(st, i) && !END_CHAR(st, i))
    i++;


        SKIP_WHITE(st, i);


        if(*(st + i) == '\n'){


            printf("Error in line %d: only one operand in a two operand function.\n", line);
            return 0;


        }


        if (*(st+i) != ',') {
            printf("Error in line %d: missing comma between the two operands.\n", line);    
        }
       
        i++;
        SKIP_WHITE(st, i);
        printf("line after comma: %s\n", st+i);
        l = 0, m = 0, only_num = 1, neg = 0;
        temp_data = **data_head;


        while(*(st + (i + m)) != '\t' && *(st + (i + m)) != ' ' && *(st + (i + m)) != '\n'){


            if(*(st + i) == '-')


                neg = 1;


            else if(*(st + (i + m)) < '0' || *(st + (i + m)) > '9')


                    only_num = 0;


            m++;


        }


        if(only_num == 1){
            num = 0;


            for(z = 0; z < m; z++){


                if(neg == 1);


                else


                    num = num * 10 + (*(st + (i + z)) - '0');


            }


            if(neg == 1)


                num = -num;


               


            temp_bin = make_binary(num, line);


            for(z = 0; z < BITS; z++){


                if(z > 9)


                    temp_bin[z] = '0';


                else


                    temp_bin[z] = temp_bin[z + 2];


            }






            for(z = 0; z < BITS; z++) {


                second_binary_word[z] = temp_bin[z];
               
            }


            free(temp_bin);


        }


        else if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/
            l = 0;
            while(*(st + i + l) != ' ' && *(st + i + l) != '\t' && *(st + i + l) != '\n' && *(st + i + l) != ',')
                l++;


            in = 0, j = 0;


            while(j < *counter && in != 1){


                if(strncmp(getData(temp_data), st + i, l) == 0)


                    in = 1;


                else{


                    temp_data = getNext(temp_data);


                    j++;


                }


            }


            if(in != 1){
                printf("Error in line %d: reference to an undeclared label.\n", line);
                return 0;


            }


            else{


               


                temp_bin = make_binary(getValue(temp_data), line);


                for(k = 0; k < 10; k++)


                    temp_bin[k] = temp_bin[k + 2];


                ARE = (getType(temp_data) == 'x') ? "01" : "10";


                for(k = 10; k < BITS; k++)


                    temp_bin[k] = ARE[k - 10];






                for(k = 0; k < BITS; k++) {


                    second_binary_word[k] = temp_bin[k];
                   
                }


                free(temp_bin);


            }


        }


        else{


           
            reg_count++;
            temp_bin = make_binary(*(st + (i + 2)) - '0', line);


            k = 0;


            for(; k < 5; k++)


                temp_bin[k] = temp_bin[k + 7];


            for(; k < BITS; k++)


                temp_bin[k] = '0';


           


            for(k = 0; k < BITS; k++) {


                second_binary_word[k] = temp_bin[k];
               
            }


            free(temp_bin);


        }




        printf("line: '%s'\n", st+i);
        SKIP_NON_WHITE(st, i);
        SKIP_WHITE(st, i);
        printf("line: '%s'\n", st+i);
        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/


            printf("Error in line %d: more then two operands in a two operand function.\n", line);/*wrong syntax*/
            return 0;


        }


        temp = make_command_binary(st, bin[0]);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        if(reg_count == 2){
            for(g = 5; g < 10; g++)
                first_binary_word[g] = second_binary_word[g - 5]; /*concat the binary words
    of the registers*/
            strcpy(bin[1], first_binary_word);
            setBinary(*curr_node, bin, 2);
            return 1;
        }
       
        bin[1][BITS] = '\0';
        strcpy(bin[1], first_binary_word);
        strcpy(bin[2], second_binary_word);
        setBinary(*curr_node, bin, 3);


        return 1;


    }


    return 2;  
}