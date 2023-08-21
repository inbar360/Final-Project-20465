#include "pass_functions.h"

boolean is_label(char st[]) {
    int i = 0;

    while(i < 32 && !END_CHAR(st, i) && *(st + i) != ':' && !IS_WHITE(st, i)){
        if(!isalnum(*(st + i))) /* If the char is not a valid label-name char, return FALSE. */
            return FALSE;
        i++; /* Add 1 to i. */
    }

    if(*(st + i) == ':') return TRUE; /* If the char is ':' return TRUE, otherwise return FALSE. */
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

void make_binary(int n, int line, char *bits) {
    int abs_n, i, j;
    char bin[BITS];
    int carry = 1;

    if(n > 2047 || n < -2048) { /* If the integer is either too large or too small. */
        n > 2047 ? printf("Error on line %d: integer too large | maximum size is 2047", line) : printf("Error on line %d: integer too small | minimum size is -2048.\n", line);
 /* Add 1 to errors. */
        return; /* Return. */
    }

    abs_n = (n < 0) ? -n : n; /* Make negative positive/ keep positive positive - for two's complement. */ 
    i = BITS - 1; /* Because we use an array of length BITS, the index of the last cell would be (BITS-1), and of the first would be 0. */

    while(i >= 0) { /* While i is not negative. */
        bin[i] = (abs_n % 2) + '0'; /* To make this 0/1 */
        abs_n /= 2; /* Divide abs_n by 2. */
        i--; /* Subtract 1 from i. */
    }

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

    for(j = 0; j < 12; j++){ /* Set the given bits to the binary representation of n after using two's complement. */

        *(bits + j) = bin[j]; /* Change each cell to the corresponding cell in bin. */

    }



}



char *organization_type(char *op) {

    int is_num = 0;

    int flag = 0, i = 0, j;



    if((op[0] && op[1] && op[2] && op[3]) && (op[0] == '@') && (op[1] = 'r') && (op[2] <= '7') && (op[2] >= '0') && (IS_WHITE(op, 3) || END_CHAR(op, 3))){

        return "101";

    } /* If the operand is a register, return "101" (5 in binary) */

    j = 0;

    while(!END_CHAR(op, j) && !IS_WHITE(op, j)) { /* Check if the operand is a number. */

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

    char *command , *fir_op, *sec_op, ARE[2] = "00";

    int i = 0;
	int labelidx = 0;
    int j = 0;

    SKIP_WHITE(st, i); /* Get to the first non-white char of the line. */
	if (is_label(st)) {
		labelidx = i;
		while(*(st+labelidx) != ':')
			labelidx++;
		
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

    if(!END_CHAR(st, i)) /* If we have not yet reached the end of the line, set fir_op. */
        fir_op = organization_type(st + i);

    SKIP_NON_WHITE(st, i); /* Skipe the first operand. */
    SKIP_WHITE(st, i);

    if(!END_CHAR(st, i))
        sec_op = organization_type(st + i); /* Set sec_op using organization_type method. */

    if (!fir_op) { /* If fir_op is NULL, return NULL. */
        return NULL;
    }

	else if (!sec_op) {
		
		for(;j < 3; j++) { /* Add the first operand value to binary array. */
        	binary[j] = '0';
    	}
			
	}
	
	
    else {
    	for(;j < 3; j++) { /* Add the first operand value to binary array. */
        	binary[j] = sec_op[j];
    	}
	}
	
	printf("fir is: '%s', command is: '%s'\n", fir_op, command);
	
    for(;j < 7; j++) { /* Add command value to binary array. */
        binary[j] = command[j-3];

    }

    for(; j < 10; j++) { /* Add second operand value to binary array. */

        binary[j] = fir_op[j-7];

    }
    
    printf("bin: '%s'\n", binary);

    for(; j < 12; j++) { /* Add ARE value to binary array. */

        binary[j] = ARE[j-10];

    }
    
    printf("returning: '%s'\n", binary);
    return binary;

}

int make_memory_of_command_label(char *st, int line, struct Data_Table *data_head, int *IC, int *counter) {
    int errors_here = 0, length = 0, i = 0, white;
    char lab[LABEL_LENGTH];
    struct Data_Table *curr = data_head, *new;

    for(i = 0; i < *counter; i++) { /* Going to the next empty node */
        curr = getNext(curr);
    }

    SKIP_WHITE(st, i); /* Skipping white chars. */
    white = i;
    while((i - white) < 32 && !END_CHAR(st, i) && *(st + i) != ':' && !IS_WHITE(st, i)){
        if(!isalnum(*(st+i)))
            return 1; /* means its not a label */

        lab[i] = *(st + i); /* Set the cell in lab to the corresponding cell st. */
        i++; /* Add 1 to i. */
    }

    if(*(st + i) == ':') { /* If the char is ':' */
        if(*(st + (i + 1)) != ' ' || *(st + (i + 1)) != '\t') { /* If the next char is not a white char, print error. */
            printf("Error on line %d: no seperation between label and operands.\n", line);
            errors_here++; /* add 1 to errors_here, errors, and i. */

            i++;
        }
        else
            i += 2; /* Add 2 to i. */

        SKIP_WHITE(st, i); /* Skip all white chars. */
        if(opcode_in_binary(st + i) == NULL){
            return 2; /* The label is not for command */
        }
        length++; /* Add 1 to length. */

        if(strcmp(opcode_in_binary(st + i), "1111") == 0) /* The only command of 4 letters is "stop". */
            i += 4; /* Add 4 to i. */
        else
            i += 3; /* Add 3 to i. */

        if(*(st + i) != ' ' || *(st + i) != '\t' || *(st + i) != '\n'){
            printf("Error on line %d: no seperation between command and operands.\n", line);
            errors_here++; /* Add 1 to errors_here and errors. */

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

        if(*(st + i) != '\n') { /* If there's another operand, print error, and add to errors_here and errors. */
            printf("Error on line %d: too many operands.\n", line);
            errors_here++;

        }

        new = create_table();
        if(!new) { /* If ran into memory issues, add to errors_here = errors. */
            printf("Error: memory allocation failed.\n");
            errors_here++;

        }

        if(errors_here > 0) { /* If there was at least one errors, return 0. */
            free(new);
            return 0;
        }

        setData(new, lab); /* set new's data.*/
        setType(new, 'c'); /* type code  */
        setLength(new, length); /* Set new's length. */

        if(*counter == 0) { /* If the list is empty, set new's value to 100, and set data_head to new. */
            setValue(new, IC_INIT_VAL);
            data_head = new;
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
        printf("Error on line %d: no seperation between command and operands.\n", line);
        return 0;
    }
}



int make_memory_of_command(char *st, int line, struct Data_Table *data_head, int *IC, int *counter) {
    int errors_here = 0, length = 0, i;
    struct Data_Table *curr = data_head, *new;

    for(i = 0; i < *counter; i++) { /* Pointing to the next empty node. */
        curr = getNext(curr);
    }

    i = 0;
    SKIP_WHITE(st, i); /* Skip white chars. */
    if(opcode_in_binary(st + i) == NULL) {
        return 2; /* Return 2 if not a command. */
    }

    length++; /* Add 1 to length. */
    if(strcmp(opcode_in_binary(st + i), "1111") == 0)
        i += 5; /* Add 5 to i. */

    else
        i += 4; /* Add 4 to i. */

    if(*(st + i) != ' ' || *(st + i) != '\t' || *(st + i) != '\n') { /* If there's no seperation, add to erros_here + errors. */
        printf("Error in line %d: no seperation between label and operands.\n", line);
        errors_here++;
    }

    SKIP_WHITE(st, i); /* Skip white chars */
    if(*(st + i) != '\n') {
        length++; /* Add 1 to length. */
        SKIP_NON_WHITE(st, i); /* Skip first operand. */
        SKIP_WHITE(st, i); /* Skip white chars. */

        if(*(st + i) != '\n') {
            length++; /* Add 1 to length. */
            SKIP_NON_WHITE(st, i); /* Skip operand. */
            SKIP_WHITE(st, i); /* Skip white chars. */

            if(*(st + i) != '\n') {
                printf("Error on line %d: too many operands.\n", line); /* Print error, and add to errors_here + errors. */
                errors_here++;
    
            }
        }
    }

    new = create_table();
    if(!new) { /* If ran into memory issues, print and add to errors variables. */
        printf("Error: could not allocate mamory for data.\n");
        errors_here++;
    }
    if(errors_here > 0) { /* Return 0 if ran into any errors. */
        return 0;
    }

    setType(new, 'c');/* type code */
    setLength(new, length);

    if(*counter == 0) { /* If the list is empty set value to 100 (initial value), and data_head to new. */

        setValue(new, 100);

        data_head = new;

    }

    else{ /* If the list is not empty, set value based on IC, and set new to curr's next. */

        setValue(new, 100 + (*IC));

        setNext(curr, new);

    }

    (*IC) += length; /* Add to IC based on length, add 1 to counter. */

    (*counter)++;

    return 3; /* Return 3 if finished succesfully. */

}



int add_extern_data(char *st, int line, struct Data_Table *data_head, int *DC, int *counter) {

    int i = 0, errors_here = 0, l = 0, DC_here = *DC;

    int j = 0, in_list = 0, data_count = 0, m;

    int legal = 1;

    char name[LABEL_LENGTH];

    struct Data_Table *data = NULL, *data_temp = data;

    struct Data_Table *curr = data_head, *temp = data_head, *new;

    for(i = 0; i < *counter; i++){ /* Setting curr to the next empty node. */

        curr = getNext(curr);

    }

    SKIP_WHITE(st, i);

    if(is_label(st + i)) { /* If encountered a label definition, skip it. */

        while(*(st + i) != ':')

            i++;

    }



    if(*(st + ++i) != ' ' && *(st + i) != '\t') { /* If after the ':' there's no white char, print errors. */

        printf("Error in line %d: no seperation between label and command.\n", line);


        errors_here++;

    }

    else

        SKIP_WHITE(st,i); /* Skip white chars. */

    if(strncmp(st + i, ".extern", 7) == 0) {

        i += 7; /* Skip the .extern */

        if(*(st + i) != '\t' && *(st + i) != ' '){ /* Check if there is a space aftre the command. */

            printf("Error in line %d: no seperation between command and operands.\n", line);



            errors_here++;

        }

        else

            SKIP_WHITE(st, i); /* If there is space, skip it. */

        

        while (st[i] != '\n') {

            l = i;

            j = 0, in_list = 0, legal = 1;



            SKIP_NON_WHITE(st, l);



            while((j < *counter) && (in_list = 0)) { /* Going over the list until encountering the label name/ reached the end of the list. */

                if((strncmp(getData(temp), st + i, l-i) == 0) && (strlen(getData(temp)) == l-i)){

                    in_list = 1; /* If the label appears in the list, print error and add to errors variables. */

                    printf("Error in line %d: declared an existing internal label external.\n", line);

        

                    errors_here++;

                }

                j++; /* Add 1 to j, and set temp to the next node in the list. */

                temp = getNext(temp);

            }

            

            if((l-i) > LABEL_LENGTH) { /* If the label name is too long, print error, add to errors variables, and set legal to 0. */

                printf("Error in line %d: label length longer than maximum of 32 letter.\n", line);

    

                errors_here++;

                legal = 0;

            }



            for(m = i; m < l; m++) { /* start at the begiening of the current label and run until you reached its end */

                if(!isalnum(*(st + m))) {

                    printf("Error in line %d: label name is not legal.\n", line);

        

                    errors_here++;

                    legal = 0;

                    break;/* one error here is enough */

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



            i = l;

            SKIP_WHITE(st, i);

            if (errors_here > 0) {/* if there are errors dont make changes in the non automatic memory */

                free(new);

                free_data_table(&data);

                return 0;

            }



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

                data_head = data_temp;

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

        return 1;

    }

    else

        return 2; /* Return 2 if not extern. */

}



int mark_label_entry(char *st, int line, struct Data_Table *data_head, int *counter) { /* Second pass function */

    int i = 0, errors_here = 0, l;

    int j = 0, in_list = 0;

    Data_Table *curr = data_head, *temp = data_head; 

    SKIP_WHITE(st, i);

    if(is_label(st + i)){

        while(*(st + i) != ':')
            i++;
        
        if(*(st + ++i) != ' ' && *(st + i) != '\t') { /* Print error and add to errors variables. */
        	printf("Error in line %d: no seperation between label and command.\n", line);
        	errors_here++;
        	return 0;
    	}
    	SKIP_WHITE(st,i); /* Skip white chars. */
    }

    if(strncmp(st + i, ".entry", 6) == 0) { 

        i += 6; /* Skip ".entry" */

        if(*(st + i) != '\t' && *(st + i) != ' ') { /* Check if there is a space aftre the command */
            printf("Error in line %d: no seperation between command and operands.\n", line);
			errors_here++;
        }

        else
            SKIP_WHITE(st, i); /*if there is a space after the command skip it*/

        while(st[i] != '\n') {

            l = i;

            SKIP_NON_WHITE(st, l);

            j = 0, in_list = 0;
            while((j < *counter) && (in_list == 0)) { /* Checking if the label name appears in the list. */
                if((strncmp(getData(temp), st + i, l-i) == 0) && (strlen(getData(temp)) == l-i)) {
                    in_list = 1;
                    printf("Error in line %d: declared an existing internal label external.\n", line);
                    errors_here++;
                }

                temp = getNext(temp);
                j++;
            }

            if(l - i > 32){
                printf("Error in line %d: label length longer than maximum of 32 letter.\n", line);
                errors_here++;
            }

            for(i = 0; i < j; i++) { /* Setting curr to the node with the wanted name. */
                curr = getNext(curr);
            }

			if(getType(curr) == 'x') { /* If the label appears as external, print error. */
                printf("Error in line %d: tried to declare an external label an entry.\n", line);
				errors_here++;
            }

            if(errors_here > 0) /* If ran into errors, return 0. */
                return 0;

            if (!in_list) {
                printf("Error in line %d: tried declaring a non-existent label as entry.\n", line);
				errors_here++;
            }

            else if(getType(curr) == 'c')
                setType(curr, 'E');

            else 
                setType(curr, 'e');

            SKIP_WHITE(st, l);/*set l to the next word*/
            i = l; /*set i to the next word*/
        }

        return 1;
    }

    return 2;
}



int add_string_data(char *st, int line, struct Data_Table *data_head, int *DC, int *counter){/*adds labels of .string type*/

    Data_Table *curr = data_head, *new;

    char lab[LABEL_LENGTH], temp[12];

    char bin[MAX_MEMORY][BITS+1];

    int i, white, k, z;

    int errors_here = 0;

    int length = 0;

    for(i = 0; i < *counter; i++){ /*going to the next empty node*/

        curr = getNext(curr);

    }
    
    for (i = 0; i < MAX_MEMORY; i++) {
		bin[i][BITS] = '\0';
	}
	
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

        

        char* string = (char*) malloc(sizeof(char));

        if(*(st + (i + 1)) != ' ' || *(st + (i + 1)) != '\t'){/*check if there is a space after the label*/

            printf("Error on line %d: no seperation between label and operands.\n", line);

            errors_here++;



            i++;

        }

        else

            i += 2;

        SKIP_WHITE(st, i); /*go to the first word after the label*/

        if(strncmp(st+i, ".string", 7) == 0){/* check if command is string*/

            i += 7;

            if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space after the command*/

                printf("Error in line %d: no seperation bitween .string and the string.\n", line);

                errors_here++;

    

            }

            SKIP_WHITE(st, i);

            if(*(st + i) != '"'){ /*check if string is not marked first by " */

                printf("Error in line %d: no quotation marks in the beginning of the string.\n", line);

                errors_here++;

    

            }

            ++i;

            while(*(st + i) != '"' && *(st + i) != '\n'){ /*until you reach the end of string*/

                if(*(st + i) <= 31 || *(st + i) >= 128){ /*check if string char is printabble*/

                    printf("Error in line %d: non printable char in string.\n", line);

                    errors_here++;

        

                }

                *(string + length) = *(st + i); /*if add char to the char array*/

                length++;

                i++;

            }

            if(*(st + i) == '\n'){ /* check if there was no " terminator to the string*/

                printf("Error in line %d: no quotation marks in the end of the string.\n", line);

                errors_here++;

    

            }

            i++;

            SKIP_WHITE(st, i);

            if(*(st + i) != '\n'){

                printf("Error in line %d: to many operands in the declaration of \".string\".\n", line);

                errors_here++;

            }

            if(in_list(data_head, lab) == 1){

                printf("Error in line %d: label - '%s' declared multiple times.\n", line, lab);

                errors_here++;

    

            }

            new = create_table();

            if(!new){

                printf("Error: could not allocate mamory for data.\n");

                exit(1);

            }

            setType(new, 'n');

            setLength(new, length+1); /* plus 1 is for the NULL terminator */

            setData(new, lab);

            setNext(new, NULL);

            

            for(k = 0; k < length; k++){

                make_binary(*(string + k), line, temp);

                for(z = 0; z < 12; z++){

                     bin[k][z] = temp[z];

                }

            }

            for(z = 0; z < 12; z++)

                bin[k][z] = '0'; /*add null terminator*/

            

            setBinary(new, bin, length+1); /* plus 1 for null terminator */

            if(errors_here > 0){

                free(new);

                return 0;/* means that there were errors */

            }

            if(*counter == 0){

                setValue(new, 100);

                data_head = new;

            }

            else{

                setValue(new, 100+(*DC));

                setNext(curr, new);

            }

            *DC += length + 1;/*plus 1 is for the NULL terminator */

            (*counter)++;/* seccesfully added a data table node */

            return 3;/* means that it succesfully added the data */

        }

        return 2;/*means that the label is not for string */



    }

    else if(i - white == 32){

        printf("Error in line %d: label longer then the maximum length of 32.\n", line);

        return 0;

    }

    else{

        printf("Error in line %d: no ':' in the declaration of the label.\n", line);

        return 0;

    }

}





int add_data_data(char *st, int line, struct Data_Table *data_head, int *DC, int *counter){ /*adds labels of .data type */

    struct Data_Table* curr = data_head, *new;

    int i, errors_here = 0, minus = 0, k, white, z;

    int curr_num;

    int d_counter;

    int data_values[MAX_MEMORY];

    char lab[LABEL_LENGTH], temp[BITS];

    char bin[MAX_MEMORY][BITS+1];

    for(i = 0; i < *counter; i++){ /* going to the next empty node */

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

        if(*(st + (i + 1)) != ' ' || *(st + (i + 1)) != '\t'){

            printf("Error on line %d: no seperation bitween label and operands.\n", line);

            errors_here++;



            i++;

        }

        else

            i += 2;

        SKIP_WHITE(st, i);

        if(strncmp(st + i, ".data", 5) == 0){

            i += 5;

            if(*(st + i) != '\t' && *(st + i) != ' '){

                printf("Error in line %d: no seperation bitween ':' and .data.\n", line);

                errors_here++;

    

            }

            SKIP_WHITE(st, i);

            d_counter = 0;

            while(*(st + i) != '\n'){

                minus = 0;

                if(*(st + i) == '-'){

                    i++;

                    minus = 1;

                }

                curr_num = 0;

                while(*(st + i) != ' ' && *(st + i) != ',' && *(st + i) != '\t'){

                    if(*(st + i) > 57 || *(st + i) < 48){

                        printf("Error in line %d: operand is not of type int.\n", line);

                        errors_here++;

            

                        break;

                    }

                    curr_num = curr_num * 10 + (*(st + i) - '0');

                    i++;

                }

                if(minus == 1)

                    data_values[d_counter] = curr_num * -1;

                else

                    data_values[d_counter] = curr_num;

                SKIP_WHITE(st, i);

                if(*(st + i) != ','){

                    printf("Error in line %d: two operands not seperated by ','.\n", line);

                    errors_here++;

        

                }

                i++;

                SKIP_WHITE(st, i);

                d_counter++;

            }

            new = create_table();

            if(!new){

                printf("Error: could not allocate mamory for data.\n");

                errors_here++;

    

            }

            setType(new, 'n');

            setLength(new, d_counter);

            setData(new, lab);

            setNext(new, NULL);



            for(k = 0; k < d_counter; k++){

                make_binary(data_values[k], line, temp);

                for(z = 0; z < 12; z++){

                    bin[k][z] = temp[z];

                }

            }



            setBinary(new, bin, d_counter);

            if(errors_here > 0){

                free(new);

                return 0; /* means that there were errors */

            }

            if(*counter == 0){

                setValue(new, 100);

                data_head = new;

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

        printf("Error in line %d: no ':' in the declaration of the label.\n", line);
		return 0;
    }
	
}

int make_command(char *st, struct Data_Table *curr_node, int line, struct Data_Table *data_head, int *counter){ /* Second pass function */

    int i = 0, errors_here = 0, t;
    int l = 0, m = 0, only_num = 1, neg = 0;
    int in = 0, j = 0, k = 0, z;
    int num = 0, length;
    char *ARE, *number, *temp = NULL;
    char binary_word[12], temp_bin[12], bin[1024][BITS+1];
    char first_binary_word[12], second_binary_word[12];
    struct Data_Table *temp_data = data_head;
	printf("line %d is: \"%s\"\n", line, st);
	
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
		
		    else
		        i = length;

		    if(st[i] != ' ' && st[i] != '\t' && st[i] != '\n'){
		        printf("Error in line %d: no seperation between label and command.\n", line);
		        return 0;
		    }

		    SKIP_WHITE(st, i);
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

        temp = make_command_binary(st, temp);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        setBinary(curr_node, bin, 1);

        return 1;
    }

    else if(strncmp(st + i, "rts", 3) == 0){
        i += 3;/*go to the char after the command*/
        SKIP_WHITE(st, i);

        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/
            printf("///Error in line %d: operand found in an operand-less command.\n", line);
            return 0;/*wrong syntax*/
        }

        temp = make_command_binary(st, temp);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        setBinary(curr_node, bin, 1);

        return 1;
    }

    /*one operand commands with 3 and 5 organization type operands*/

    else if(strncmp(st + i, "not", 3) == 0 || strncmp(st + i, "clr", 3) == 0 || strncmp(st + i, "inc", 3) == 0 || strncmp(st + i, "dec", 3) == 0 || strncmp(st + i, "jmp", 3) == 0 || strncmp(st + i, "bne", 3) == 0 || strncmp(st + i, "red", 3) == 0 || strncmp(st + i, "jsr", 3) == 0){ /*3 and 5 */
    
        i += 3;/*go two chars after the command*/
        if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space aftre the command*/
            printf("Error in line %d: no seperation between comand and operands.\n", line);
            errors_here++;
        }

        SKIP_WHITE(st, i);
        if(*(st + i) == '\n'){
            printf("Error in line %d: no operands in a single operand function.\n", line);
            return 0;
        }

        l = 0;
        if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/

            SKIP_NON_WHITE(st, l);
            in = 0, j = 0;
            while(j < *counter && in != 1){
                if(strncmp(getData(temp_data), st + i, l) == 0)
                    in = 1;
                    
                else {
                    temp_data = getNext(temp_data);
                    j++;
                }
            }

            if(in != 1){
                errors_here++;
                printf("Error in line %d: reference to an undeclared label.\n", line);
            }

            else{ /* The label appears in the list. */
                make_binary(getValue(temp_data), line, temp_bin);
                for(k = 0; k < 10; k++)
                    temp_bin[k] = temp_bin[k + 2];

                ARE = (getType(temp_data) == 'x') ? "01" : "10"; /* If the label is extern, ARE is 01, else 10. */

                for(k = 10; k < BITS; k++)
                    temp_bin[k] = ARE[k - 10];

                for (k = 0; k < BITS; k++) {
                    binary_word[k] = temp_bin[k];
                }
            }
        }

        else { /* The wanted operand is a register. */

            make_binary(*(st + (i + 2)) - '0', line, temp_bin);

            for(; k < 5; k++)
                temp_bin[k] = temp_bin[k + 7];

            for(; k < 12; k++)
                temp_bin[k] = '0';

            for (k = 0; k < BITS; k++) {
                binary_word[k] = temp_bin[k];

            }

        }

        SKIP_NON_WHITE(st, i);
        SKIP_WHITE(st, i);

        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/
            printf("Error in line %d: multiple operand in a single operand function.\n", line);/*wrong syntax*/
            errors_here++;
        }

        if(errors_here > 0)
            return 0;
            
		temp = make_command_binary(st, temp);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        strcpy(bin[1], binary_word);
        setBinary(curr_node, bin, 2);

        return 1;
    }
    
    /*one operand commands with 1, 3 and 5 organization type operands*/
    else if(strncmp(st + i, "prn", 3) == 0){
		printf("prn!\n");
        i += 3;/*go two chars after the command*/
        if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space aftre the command*/
            printf("Error in line %d: no seperation between comand and operands.\n", line);
            errors_here++;
        }

        SKIP_WHITE(st, i);
        if(*(st + i) == '\n'){
            printf("Error in line %d: no operands in a single operand function.\n", line);
            return 0;
        }

        while(*(st + (i + m)) != '\t' && *(st + (i + m)) != ' ' && *(st + (i + m)) != '\n'){
			printf("cur line: '%s'\n", st+i+m);
            if(*(st + i) == '-' && m == 0)
                neg = 1;

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
            if(neg == 1)
                num = -num;
            make_binary(num, line, temp_bin);
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
                errors_here++;
                printf("Error in line %d: reference to an undeclared label.\n", line);
            }

            else{
                make_binary(getValue(temp_data), line, temp_bin);

                for(k = 0; k < 10; k++)
                    temp_bin[k] = temp_bin[k + 2];

                ARE = (getType(temp_data) == 'x') ? "01" : "10";

                for(k = 10; k < 12; k++)
                    temp_bin[k] = ARE[k - 10];

                for (k = 0; k < BITS; k++) {
                    binary_word[k] = temp_bin[k];
                }
            }
        }

        else{
            make_binary(*(st + (i + 2)) - '0', line, temp_bin);
            k = 0;
            
            for(; k < 5; k++)
                temp_bin[k] = temp_bin[k + 7];
                
            for(; k < 12; k++)
                temp_bin[k] = '0';

            for (k = 0; k < BITS; k++) {
                binary_word[k] = temp_bin[k];
            }
        }

        SKIP_NON_WHITE(st, i);

        SKIP_WHITE(st, i);

        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/

            printf("Error in line %d: multiple operand in a single operand function.\n", line);/*wrong syntax*/



            errors_here++;

        }

        if(errors_here > 0)

            return 0;
		printf("before make command binary!\n");
		temp = make_command_binary(st, bin[0]);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
		printf("after make command binary! bin[0] is: '%s' \n", bin[0]);
        strcpy(bin[1], binary_word);
        printf("before setBinary! bin[1] is: '%s'\n", bin[1]);
        setBinary(curr_node, bin, 2);
        printf("after setBinary!\n");

        return 1;

    }



    /*two operand commands with 3 organization type first operand and 3 and 5 organization type second operand*/



    else if(strncmp(st + i, "lea", 3) == 0){ 

        

        i += 3;/*go two chars after the command*/

        if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space aftre the command*/

            printf("Error in line %d: no seperation between command and operands.\n", line);



            errors_here++;

        }



        SKIP_WHITE(st, i);

        if(*(st + i) == '\n'){

            printf("Error in line %d: no operands in a two operand function.\n", line);



            return 0;

        }



        l = 0;

        while(*(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n')

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



            errors_here++;

            printf("Error in line %d: reference to an undeclared label.\n", line);

        }

        else{

            make_binary(getValue(temp_data), line, temp_bin);

            for(k = 0; k < 10; k++)

                temp_bin[k] = temp_bin[k + 2];

            ARE = (getType(temp_data) == 'x') ? "01" : "10";

            for(k = 10; k < 12; k++)

                temp_bin[k] = ARE[k - 10];



            for(k = 0; k < BITS; k++) {

                first_binary_word[k] = temp_bin[k];

            }

        }

        

        SKIP_NON_WHITE(st, i);

        SKIP_WHITE(st, i);

        if(*(st + i) == '\n'){

            printf("Error in line %d: only one operand in a two operand function.\n", line);



            return 0;

        }



        l = 0;

        if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/

            while(*(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n')

                l++;

            in = 0, j = 0;

            temp_data = data_head;

            while(j < *counter && in != 1){

                if(strncmp(getData(temp_data), st + i, l) == 0)

                    in = 1;

                else{

                    temp_data = getNext(temp_data);

                    j++;

                }

            }

            if(in != 1){

    

                errors_here++;

                printf("Error in line %d: reference to an undeclared label.\n", line);

            }

            else{

                make_binary(getValue(temp_data), line, temp_bin);

                for(k = 0; k < 10; k++)

                    temp_bin[k] = temp_bin[k + 2];

                ARE = (getType(temp_data) == 'x') ? "01" : "10";

                for(k = 10; k < BITS; k++)

                    temp_bin[k] = ARE[k - 10];



                for (k = 0; k < BITS; k++) {

                    second_binary_word[k] = temp_bin[k];

                }



            }

        }

        else{

            make_binary(*(st + (i + 2)) - '0', line, temp_bin);

            k = 0;

            for(; k < 5; k++)

                temp_bin[k] = temp_bin[k + 7];

            for(; k < 12; k++)

                temp_bin[k] = '0';



            for (k = 0; k < BITS; k++) {

                second_binary_word[k] = temp_bin[k];

            }

        }

        SKIP_NON_WHITE(st, i);

        SKIP_WHITE(st, i);

        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/

            printf("Error in line %d: more then two operands in a two operand function.\n", line);/*wrong syntax*/



            errors_here++;

        }

        if(errors_here > 0)

            return 0;
		
		temp = make_command_binary(st, temp);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        strcpy(bin[1], first_binary_word);
        strcpy(bin[2], second_binary_word);
        setBinary(curr_node, bin, 3);

        return 1;

    }



        /*two operand commands with 1, 3 and 5 organization type first operand and 3 and 5 organization type second operand*/



    else if(strncmp(st + i, "mov", 3) == 0 || strncmp(st + i, "add", 3) == 0 || strncmp(st + i, "sub", 3) == 0){ 

        i += 3;/*go two chars after the command*/

        if(*(st + i) != '\t' && *(st + i) != ' '){/*check if there is a space aftre the command*/

            printf("Error in line %d: no seperation bitween command and operands.\n", line);



            errors_here++;

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

                

            make_binary(num, line, temp_bin);

            for(z = 0; z < BITS; z++){

                if(z > 9)

                    temp_bin[z] = '0';

                else

                    temp_bin[z] = temp_bin[z + 2];

            }



            for (z = 0; z < BITS; z++) {

                first_binary_word[z] = temp_bin[z];

            }

        }



        else if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/

            while(*(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n')

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

    

                errors_here++;

                printf("Error in line %d: reference to an undeclared label.\n", line);

            }

            else{

                

                make_binary(getValue(temp_data), line, temp_bin);

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

            make_binary(*(st + (i + 2)) - '0', line, temp_bin);

            k = 0;

            for(; k < 5; k++)

                temp_bin[k] = temp_bin[k + 7];

            for(; k < BITS; k++)

                temp_bin[k] = '0';



            for (k = 0; k < BITS; k++) {

                first_binary_word[k] = temp_bin[k];

            }

            

        }

        

        SKIP_NON_WHITE(st, i);

        SKIP_WHITE(st, i);

        if(*(st + i) == '\n'){

            printf("Error in line %d: only one operand in a two operand function.\n", line);



            return 0;

        }



        l = 0;

        if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/

            while(*(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n')

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

    

                errors_here++;

                printf("Error in line %d: reference to an undeclared label.\n", line);

            }

            else{

                

                make_binary(getValue(temp_data), line, temp_bin);

                for(k = 0; k < 10; k++)

                    temp_bin[k] = temp_bin[k + 2];

                ARE = (getType(temp_data) == 'x') ? "01" : "10";

                for(k = 10; k < BITS; k++)

                    temp_bin[k] = ARE[k - 10];



                for (k = 0; k < BITS; k++) {

                    second_binary_word[k] = temp_bin[k];

                }



            }

        }

        else{

            

            make_binary(*(st + (i + 2)) - '0', line, temp_bin);

            k = 0;

            for(; k < 5; k++)

                temp_bin[k] = temp_bin[k + 7];

            for(; k < BITS; k++)

                temp_bin[k] = '0';



            for (k = 0; k < BITS; k++) {

                second_binary_word[k] = temp_bin[k];

            }



        }



        SKIP_NON_WHITE(st, i);

        SKIP_WHITE(st, i);

        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/

            printf("Error in line %d: more then two operands in a two operand function.\n", line);/*wrong syntax*/



            errors_here++;

        }

        if(errors_here > 0)

            return 0;
        
        temp = make_command_binary(st, temp);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        strcpy(bin[1], first_binary_word);
        strcpy(bin[2], second_binary_word);
        setBinary(curr_node, bin,3);

        return 1;

    }



        /*two operand commands with 1, 3 and 5 organization type first operand and 1, 3 and 5 organization type second operand*/



    else if(strncmp(st + i, "cmp", 3) == 0){

        i += 3; /*go two chars after the command*/

        if(*(st + i) != '\t' && *(st + i) != ' '){ /*check if there is a space aftre the command*/

            printf("Error in line %d: no seperation bitween command and operands.\n", line);



            errors_here++;

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

            int num = 0;

            for(z = 0; z < m; z++){

                if(neg == 1);

                else

                    num = num * 10 + (*(st + (i + z)) - '0');

            }

            if(neg == 1)

                num = -num;

                

            make_binary(num, line, temp_bin);

            for(z = 0; z < BITS; z++){

                if(z > 9)

                    temp_bin[z] = '0';

                else

                    temp_bin[z] = temp_bin[z + 2];

            }



            for (z = 0; z < BITS; z++) {

                first_binary_word[z] = temp_bin[z];

            }

            

        }

        else if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/

            while(*(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n')

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

    

                errors_here++;

                printf("Error in line %d: reference to an undeclared label.\n", line);

            }

            else{

                

                make_binary(getValue(temp_data), line, temp_bin);

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

            

            make_binary(*(st + (i + 2)) - '0', line, temp_bin);

            k = 0;

            for(; k < 5; k++)

                temp_bin[k] = temp_bin[k + 7];

            for(; k < BITS; k++)

                temp_bin[k] = '0';

            

            for (k = 0; k < BITS; k++) {

                first_binary_word[k] = temp_bin[k];

            }

            

        }

        

        SKIP_NON_WHITE(st, i);

        SKIP_WHITE(st, i);

        if(*(st + i) == '\n'){

            printf("Error in line %d: only one operand in a two operand function.\n", line);



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

            int num = 0;

            for(z = 0; z < m; z++){

                if(neg == 1);

                else

                    num = num * 10 + (*(st + (i + z)) - '0');

            }

            if(neg == 1)

                num = -num;

                

            make_binary(num, line, temp_bin);

            for(z = 0; z < BITS; z++){

                if(z > 9)

                    temp_bin[z] = '0';

                else

                    temp_bin[z] = temp_bin[z + 2];

            }



            for(z = 0; z < BITS; z++) {

                second_binary_word[z] = temp_bin[z];

            }

            

        }

        else if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0'){ /*check if operand is not a legal register*/

            while(*(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n')

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

    

                errors_here++;

                printf("Error in line %d: reference to an undeclared label.\n", line);

            }

            else{

                

                make_binary(getValue(temp_data), line, temp_bin);

                for(k = 0; k < 10; k++)

                    temp_bin[k] = temp_bin[k + 2];

                ARE = (getType(temp_data) == 'x') ? "01" : "10";

                for(k = 10; k < BITS; k++)

                    temp_bin[k] = ARE[k - 10];



                for(k = 0; k < BITS; k++) {

                    second_binary_word[k] = temp_bin[k];

                }



            }

        }

        else{

            

            make_binary(*(st + (i + 2)) - '0', line, temp_bin);

            k = 0;

            for(; k < 5; k++)

                temp_bin[k] = temp_bin[k + 7];

            for(; k < BITS; k++)

                temp_bin[k] = '0';

            

            for(k = 0; k < BITS; k++) {

                second_binary_word[k] = temp_bin[k];

            }

            

        }



        SKIP_NON_WHITE(st, i);

        SKIP_WHITE(st, i);

        if(*(st + i) != '\n'){/*if there are chars that arent white after the word*/

            printf("Error in line %d: more then two operands in a two operand function.\n", line);/*wrong syntax*/



            errors_here++;

        }

        if(errors_here > 0)

            return 0;

		temp = make_command_binary(st, temp);
        printf("temp is: '%s'\n", temp);
        strcpy(bin[0], temp);
        strcpy(bin[1], first_binary_word);
        strcpy(bin[2], second_binary_word);
        setBinary(curr_node, bin, 3);

        return 1;

    }

    return 2;  
}