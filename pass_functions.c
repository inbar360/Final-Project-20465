#include "globals.h"
#include "pass_functions.h"

Data_Table *data_head;
Ent_Table *ent_head;
Ext_Table *ext_head;
int IC = 0, DC = 0, COUNTER = 0, ERRORS = 0;

int is_label(file, line) {
    int i = 0;
    while(i < 32 && !IS_WHITE(st, i) && !END_CHAR(st, i) && *(st + i) != ':'){
        if(!isalnum(*(st+i))) 
            return 0;
        i++;
    }
    if(*(st + i) == ':') return 1;
}

int in_list(struct Data_Table *dt, char str[]){
    struct Data_Table *dt1= dt;
    while(dt1 != NULL){
        if(strcmp(str, getDataData(dt1)) == 0){
            return 1;
        }
        dt1 = getDataNext(dt1);
    }
    return 0;
}

char *opcode_in_binary(char* op){
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
    
    char bin[BITS]; // +1 for the null terminator
    int i = BITS - 1;

    if(n > MAX || n < MIN){
        n > MAX ? printf("Error int line %d: integer to large | maximum size is 2047", line) : printf("Error int line %d: integer to small | minimum size is -2048", line);
        ERRORS++; 
        return;
    }
    int abs_n = (n < 0) ? -n : n;//make negative positive
    
    // Convert to binary
    while(i >= 0) {
        bin[i] = (abs_n % 2) + '0';
        abs_n /= 2;
        i--;
    }
    
    // Handle negative numbers by calculating two's complement
    if (n < 0) {
        // Invert the BITS
        for (int j = 0; j < BITS; j++) {
            bin[j] = (bin[j] == '0') ? '1' : '0';
        }
        
        // Add 1 to the result
        int carry = 1;
        for (int j = BITS - 1; j >= 0; j--) {
            if (bin[j] == '0' && carry == 1) {
                bin[j] = '1';
                carry = 0;
            } else if (bin[j] == '1' && carry == 1) {
                bin[j] = '0';
            }
        }
    }
    
    for(int j = 0; j < 12; j++){
        *(bits + j) = bin[j];
    }
}


char *organization_type(char op[]){
    /*int 0;*/
    int is_num = 0;
    int is_label = 0, i = 0;
    if(op[0] && op[1] && op[2] && op[0] == '@' && op[1] = 'r' && op[2] <= '7' && op[2] >= '0' && (op[3] == '\n' || IS_WHITE(op, 3))) {
        return "101";
    }
    
    while(op[i] != '\n' && !IS_WHITE(op, i)){
        if(op[i] < '0' || op[1] > '9')
            is_num = 1;
    }
    if(is_num = 0)
        return "001";
    
    while(op[i] != '\n' && op[i] != '\t' && op[i] != ' '){
        if((op[i] > 57 && op[i] < 65) || op[i] < 48 || (op[i] > 90 && *(st + i) < 97) || op[i] > 122)
            flag = 1;
    }
    if(i > 32){
        printf("Error: non legal operand");
        ERRORS++;
        return NULL;
    }
    if(flag == 1){
        printf("Error: non legal operand");
        ERRORS++;
    }
    return "011";
}

char *make_ARE(char st[]){ /* line after labes st */
    int i = 0, fir_op = 0, sec_op = 0;
    int length_op = 0, j;
    char type_fir = 0, type_sec = 0;
    Data_Table *temp = data_head;
    SKIP_WHITE(st, i);
    i += 5;
    SKIP_WHITE(st, i);
    if(st[i] != '\n'){
        fir_op = 1;
        if(strcmp(organization_type(st + sizeof(char) * i), "101") == 0 || strcmp(organization_type(st + sizeof(char) * i), "001") == 0){
            type_fir = 0;
        }
        j = i;
        while(!IS_WHITE(st, j) && st[j] != '\n' && length_op < 32){
            length_op++;
            j++;
        }
        if(length_op == 32){
            printf("Error: unrecognized operator");
            ERRORS++;
            return NULL;
        }
        
        while(strncmp(getDataData(temp), st + (sizeof(char) * i), length_op) != 0 && getDataNext(temp) != NULL)
            temp = getDataNext(temp);
        if(strncmp(getDataData(temp), st + (sizeof(char) * i), length_op) == 0)
            type_fir = getDataType(temp);
        else {
            printf("Erorr: reference to an undeclared label");
            ERRORS++;
            return NULL;
        }
        i += length_op;
    }
    else{
        return "00";
    }
    SKIP_WHITE(st, i);
    if(st[i] != '\n'){
        sec_op = 1;
        if(strcmp(organization_type(st + sizeof(char) * i), "101") == 0 || strcmp(organization_type(st + sizeof(char) * i), "001") == 0){
            type_sec = 0;
        }
        length_op = 0, j = i;
        while(!IS_WHITE(st, j) && st[j] != '\n' && length_op < 32){
            length_op++;
            j++;
        }
        if(length_op == LABEL_LENGTH){
            printf("Error: unrecognized operator");
            ERRORS++;
            return NULL;
        }
        temp = data_head;
        while(strncmp(getDataData(temp), st + (sizeof(char) * i), length_op) != 0 && getDataNext(temp) != NULL)
            temp = getDataNext(temp);
        if(strncmp(getDataData(temp), st + (sizeof(char) * i), length_op) == 0)
            type_sec = getDataType(temp);
        else{
            printf("Erorr: reference to an undeclared label");
            ERRORS++;
            return NULL;
        }
        i += length_op;
    }
    return type_fir == 'e' || type_sec == 'e' ? "01" : type_fir == 'r' || type_sec == 'r' ? "10" : "00";
    
}

char[] make_command_binary(text of line){
    char[] binary = char[12], command = char[4], fir_op = char[3], sec_op = char[3], ARE = char[2];
    

}

int make_memory_of_command(char st[]) { /* file string st */
    int errors_here = 0, length = 0;
    Data_Table *curr = date_head, *new;
    char lab[] = char[32];
    int i = 0, white;
    for(i = 0; i < IC - 1; i++){ //going to the next empty node
        curr = getDataNext(curr);
    }
    
    SKIP_WHITE(st, i);
    white = i;
    /**** Can probably use is_label here ****/
    while(i - white < 32 && *(st + i) != '\0' && *(st + i) != ':' && *(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n'){
        if((*(st + i) > 57 && *(st + i) < 65) || *(st + i) < 48 || (*(st + i) > 90 && *(st + i) < 97) || *(st + i) > 122)
            return 1; //means its not a label
        lab[i] = *(st + i);
        i++;
    }
    if(*(st + i) == ':') {
        if(!IS_WHITE(st, i+1)) {
            printf("Error: no seperation bitween label and operands");
            errors_here++;
            ERRORS++;
            i++;
        }
        else
            i += 2;
        SKIP_WHITE(st, i);
        if(opcode_in_binary(st + i) == NULL) {
            return 2;//means the label is not for command
        }
        length++;
        if(strcmp(opcode_in_binary(st + i), "1111") == 0)
            i += 4;
        else
            i += 3;
        /**** לא ממש הבנתי את השורה למטה זה לא אמור להיות וגם? ****/
        if(*(st + i) != ' ' || *(st + i) != '\t' || *(st + i) != '\n'){
            printf("Error: no seperation bitween label and operands");
            errors_here++;
            ERRORS++;
        }
        SKIP_WHITE(st, i);
        if(*(st + i) != '\n')
            length++;
        while(!IS_WHITE(st, i) || *(st + i) != '\n')
            i++;
        SKIP_WHITE(st, i);
        if(*(st + i) != '\n')
            length++;
        while(!IS_WHITE(st, i) || *(st + i) != '\n')
            i++;
        SKIP_WHITE(st, i);
        if(*(st + i) != '\n') {
            printf("Error: too many operands");
            errors_here++;
            ERRORS++;
        }
        new = create_data_table();
        if(!new){
            printf("Error: could not allocate mamory for data")
            errors_here++;
            ERRORS++;
        }
        if(errors_here > 0){
            return 0;
        }
        setDataData(new, lab);
        setDataType(new, 'c');
        setDataLength(new, length);
        if(COUNTER == 0){
            setDataValue(new, 100);
            data_head = new;
        }
        else{
            setDataValue(new, 100+IC);
            setDataNext(curr, new);
        }
        IC += length;
        COUNTER++;
        return 3; //done succesfully
    }

}
/* file string st */
int add_string_data(char st[], int counter, int line){//adds labels of .string type
    Data_Table *curr = data_head, *new;
    char lab[] = char[32];
    int i, white;
    int errors_here = 0;
    int start, end, length = 0;
    char *string;
    for(i = 0; i < DC - 1; i++){ //going to the next empty node
        curr = getDataNext(curr);
    }

    SKIP_WHITE(st, i);
    white = i;
    /* using is_label before this maybe */
    while(i - white < 32 && *(st + i) != '\0' && *(st + i) != ':' && *(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n'){
        if((*(st + i) > 57 && *(st + i) < 65) || *(st + i) < 48 || (*(st + i) > 90 && *(st + i) < 97) || *(st + i) > 122)
            return 1; //means its not a label
        lab[i - white] = *(st + i);
        i++;
    }
    if(*(st + i) == ':'){
        string = (char *)malloc(sizeof(char));
        if (!string) {
            printf("Error: Memory allocation failed.\n");
            exit(1);
        }
        if(!IS_WHITE(st, i+1)){
            printf("Error: no seperation bitween label and operands");
            errors_here++;
            ERRORS++;
            i++;
        }
        else
            i += 2;
        SKIP_WHITE(st, i);
        if(strncmp(st+i, ".string", 7) == 0) {
            i += 7;
            if(*(st + i) != '\t' && *(st + i) != ' '){
                printf("Error in line %d: no seperation bitween .string and the string", line);
                errors_here++;
                ERRORS++;
            }
            SKIP_WHITE(st, i);
            if(*(st + i) != '"') {
                printf("Error in line %d: no quotation marks in the beginning of the string", line);
                errors_here++;
                ERRORS++;
            }
            start = ++i;
            while(*(st + i) != '"' && *(st + i) != '\n'){
                if(*(st + i) <= 31 || *(st + i) >= 128){
                    printf("Error in line %d: non printable char in string", line);
                    errors_here++;
                    ERRORS++;
                }
                *(string + length) = *(st + i);
                length++;
                i++;
            }
            if(*(st + i) == '\n'){
                printf("Error in line %d: no quotation marks in the end of the string", line);
                errors_here++;
                ERRORS++;
            }
            end = i++;
            SKIP_WHITE(st, i);
            if(*(st + i) != '\n'){
                printf("Error in line %d: to many operands in the declaration of .string", line);
                errors_here++;
                ERRORS;
            }
            if(in_list(data_head, lab) == 1){
                printf("Error in line %d: label - '%s' declared multiple times", line, lab);
                errors_here++;
                ERRORS++;
            }
            new = create_data_table();
            if(!new) {
                printf("Error: Memory allocation failed.\n");
                errors_here++;
                ERRORS++;
            }
            setDataType(new, 'n');
            setDataLength(new, length);
            setDataData(new, lab);
            new.next = NULL;
            if(!new.binary) {
                printf("Error: could not allocate mamory for data")
                errors_here++;
                ERRORS++;
            }
            for(int k = 0; k < length; k++){
                char temp[12];
                make_binary(*(string + k), temp);
                for(int z = 0; z < 12; z++){
                     new.binary[k][z] = temp[z];
                }
            }
            if(errors_here > 0){
                return 0;//means that there were errors
            }
            if(counter == 0){
                new.value = 100;
                data_head = new_data;
            }
            else{
                new.value = 100 + DC;
                curr->next = new_data;
            }
            DC += length;
            counter++;
            return 3;//means that it succesfully added the data
        }
        return 2;//means that the label is not for string

    }
    else if(i - white == 32){
        printf("///Error in line %d: label longer then the maximum length of 32///", line);
        return 0;
    }
    else{
        printf("///Error in line %d: no ':' in the declaration of the label", line);
        return 0;
    }
}


int add_data_data(file string s1, int counter, int line){//adds labels of .data type
    Data_Table* curr = &date_head;
    for(i = 0; i < counter - 1; i++){ //going to the next empty node
        curr = &(curr->next);
    }
    int data_values[] = int[1024];
    int i = 0;
    char[] lab = char[32];
    SKIP_WHITE(st, i);
    int white = i;
    while(i - white < 32 && *(st + i) != '\0' && *(st + i) != ':' && *(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n'){
        if((*(st + i) > 57 && *(st + i) < 65) || *(st + i) < 48 || (*(st + i) > 90 && *(st + i) < 97) || *(st + i) > 122)
            return 1;         //printf("///Error in line %d: label name can only contain numbers and English letters (lower and upper case)///", line);
        lab[i - white] = *(st + i);
        i++;
    }
    if(*(st + i) == ':'){
        if(*(st + (i + 1)) != ' ' || *(st + (i + 1)) != '\t'){
            printf("Error: no seperation bitween label and operands");
            errors_here++;
            ERRORS++;
            i++;
        }
        else
            i += 2;
        SKIP_WHITE(st, i);
        if(strncmp(st + i, ".data", 5) == 0){
            i += 5;
            if(*(st + i) != '\t' && *(st + i) != ' '){
                printf("///Error in line %d: no seperation bitween ':' and .data///", line);
                errors_here++;
                ERRORS++;
            }
            SKIP_WHITE(st, i);
            int d_counter = 0;
            while(*(st + i) != '\n'){
                int minus = 0;
                if(*(st + i) == '-'){
                    i++
                    minus = 1;
                }
                int curr_num = 0, digits = 0;
                while(*(st + i) != ' ' && *(st + i) != ',' && *(st + i) != '\t'){
                    if(*(st + i) > 57 || *(st + i) < 48){
                        printf("///Error in line %d, character "d": operand is not of type int///", line, i);
                        errors_here++;
                        ERRORS++;
                        stop;
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
                    printf("///Error in line %d: two operands not seperated by ','///", line);
                    errors_here++;
                    ERRORS++;
                }
                i++;
                SKIP_WHITE(st, i);
                d_counter++;
            }
            Data_Table new;
            if(!new){
                printf("Error: could not allocate mamory for data")
                errors_here++;
                ERRORS++;
            }
            new.type = 'n';
            new.length = d_counter;
            new.data = lab;
            new.next = NULL;
            if(!new.binary){
                printf("Error: could not allocate mamory for data")
                errors_here++;
                ERRORS++;
            }
            for(int k = 0; k < d_counter; k++){
                char temp[12];
                make_binary(data_values[k], temp);
                for(int z = 0; z < 12; z++){
                     new.binary[k][z] = temp[z];
                }
            }
            if(errors_here > 0){
                return 0;//means that there were errors
            }
            if(counter == 0){
                new.value = 100;
                data_head = new_data;
            }
            else{
                new.value = 100 + DC;
                curr->next = new_data;
            }
            DC += length;
            counter++;
            return 3;//success
        }
        return 2;//not for data

    }
    else if(i - white == 32){
        printf("///Error in line %d: label longer then the maximum length of 32///", line);
    }
    else{
        printf("///Error in line %d: no ':' in the declaration of the label", line);
    }
}


int is_value_command(char* st, int line){
    int i = 0;
    while(i < 32 && *(st + i) != '\0' && *(st + i) != ':' && *(st + i) != ' ' && *(st + i) != '\t' && *(st + i) != '\n'){
        if((*(st + i) > 57 && *(st + i) < 65) || *(st + i) < 48 || (*(st + i) > 90 && *(st + i) < 97) || *(st + i) > 122)
            printf("///Error in line %d: label name can only contain numbers and English letters (lower and upper case)///", line);
            return 0;
        i++;
    }
    if(*(st + i) == ':'){
        i += 2;
        SKIP_WHITE(st, i);
        
        //no operand commands//
        if(strncmp(st + i, "stop", 4) == 0){
            i += 4;//go to the char after the command
            SKIP_WHITE(st, i);
            if(*(st + i) != '\n'){//if there are chars that arent white after the word
                printf("///Error in line %d: operand found in an operand-less command", line);
                return 0;//wrong syntax
            }
             
            return 1;
        }

        else if(strncmp(st + i, "rts", 3) == 0){
            i += 3;//go to the char after the command
            SKIP_WHITE(st, i);
            if(*(st + i) != '\n'){//if there are chars that arent white after the word
                printf("///Error in line %d: operand found in an operand-less command", line);
                return 0;//wrong syntax
            }
            return 1;
        }

        //one operand commands//
        else if(strncmp(st + i, "not", 3) == 0){ //3 and 5 
            i += 3;//go two chars after the command
            if(*(st + i) != '\t' && *(st + i) != ' ')//check if there is a space aftre the command
                return 0;
            SKIP_WHITE(st, i);
            if(*(st + i) != '@' || *(st + (i + 1)) != 'r' || *(st + (i + 2)) > '7' ||  *(st + (i + 2)) < '0') //check if operand is not a legal register
                if()
            i += 2;
            SKIP_WHITE(st, i);
            if(*(st + i) != '\n')//if there are chars that arent white after the word
                return 0;//wrong syntax
            return 1;
        }

        else{
            printf("///Error in line %d: undefined command", line);
            
        }

    }
    else if(i == 32){
        printf("///Error in line %d: label longer then the maximum length of 32///", line);
    }
    else{
        printf("///Error in line %d: no ':' in the declaration of the label", line);
    }

    
        
}



