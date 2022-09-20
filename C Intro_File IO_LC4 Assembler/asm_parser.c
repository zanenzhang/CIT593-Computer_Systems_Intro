 /***************************************************************************
 * file name   : asm_parser.c                                              *
 * author      : tjf & you                                                 *
 * description : the functions are declared in asm_parser.h                *
 *               The intention of this library is to parse a .ASM file     *
 *			        										               * 
 *                                                                         *
 ***************************************************************************
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm_parser.h"

/*Declaring all functions so that future calls from other functions will not result in any errors */
int parse_add (char* instr, char* instr_bin_str) ;  
int parse_mul (char* instr, char* instr_bin_str) ;
int parse_sub (char* instr, char* instr_bin_str) ;
int parse_div (char* instr, char* instr_bin_str) ;
int parse_and (char* instr, char* instr_bin_str) ;
int parse_not (char* instr, char* instr_bin_str) ;
int parse_or (char* instr, char* instr_bin_str) ;
int parse_xor (char* instr, char* instr_bin_str) ;
unsigned short int str_to_bin (char* instr_bin_str) ;
int write_obj_file (char* filename, unsigned short int program_bin[ROWS] ) ;
unsigned short int endian_swap (unsigned short int flip_number) ;
char* convert_imm_binary(unsigned int imm_result) ;
int parse_imm(char* instr, char* instr_bin_str) ;
int parse_code(char* instr_bin_str) ;
int parse_address(char* instr, char* instr_bin_str) ;
int parse_data(char* instr_bin_str) ;

/* to do - implement all the functions in asm_parser.h */

//This function is to read an ASM file into a 2D array
int read_asm_file (char * filename, char program[ROWS][COLS]){
    
    FILE *src_file ;
    
    src_file = fopen(filename, "r");  //Opens the file passed into the function in read mode
    
    if (src_file == NULL){
        
        //If the file fails to open, i.e. the file does not exist, show an error message and return 2
        printf("error2: read_asm_file() failed \n") ;   
        return 2;
    }
       
    char buffer[255];    
    int row = 0;    
    int col;
    
    //Loops through the asm file and copies the instructions (ASCII) into the 2D program array
    while (fgets(buffer, 255, src_file) != NULL) {
        
        col = 0;
        
        for (int i = 0; buffer[i] != '\0'; i++){

            //To ignore the new line characters at the end, replace with a NULL and exit the current loop
            if (buffer[i] == '\n'){          
                
                program[row][col] = '\0' ;
                
                break;
             
            //Likewise, for the extra credit question, this ignores comments by replacing with a NULL and exit the current loop   
            } else if (buffer[i] == ';') {
                
                program[row][col] = '\0' ;
                
                break;
                
            } else {
                
                program[row][col] = buffer[i] ; //Otherwise if it is not a comment or new line, copy over the character to the 2D array
            }
            
            col ++;            
        } 
        row ++;
    }
    
    col = 0;
    program[row][col] = '\0' ;  //Cap off with a NULL
    
    return (0);
}

//This function is to parse the ASM instruction and convert into a binary equivalent
int parse_instruction (char* instr, char* instr_bin_str){
    
    char *token;
    const char delim[] = ", ";
    
    token = strtok(instr, delim); //Takes the first token of the instruction string
    
    if (token == NULL){
        
        printf("error3: parse_instruction() failed"); //Prints an error and returns 3 if there is no token
        
        return 3;
    } 

    if (strcmp(token, "ADD") == 0){       //If the directive is an addition, run the corresponding parse steps

       parse_add(token, instr_bin_str);

    } else if (strcmp(token, "MUL") == 0) {   //If the directive is a multiplication, run the corresponding parse steps

       parse_mul(token, instr_bin_str);

    } else if (strcmp(token, "SUB") == 0) {      //If the directive is a subtraction, run the corresponding parse steps

       parse_sub(token, instr_bin_str);

    } else if (strcmp(token, "DIV") == 0) {       //If the directive is a division, run the corresponding parse steps

       parse_div(token, instr_bin_str);

    } else if (strcmp(token, "AND") == 0) {       //If the directive is a logical and, run the corresponding parse steps

       parse_and(token, instr_bin_str);

    } else if (strcmp(token, "NOT") == 0) {    //If the directive is a logical not, run the corresponding parse steps

       parse_not(token, instr_bin_str);

    } else if (strcmp(token, "OR") == 0) {     //If the directive is a logical or, run the corresponding parse steps

       parse_or(token, instr_bin_str);

    } else if (strcmp(token, "XOR") == 0) {     //If the directive is a logical exclusive or, run the corresponding parse steps

       parse_xor(token, instr_bin_str);
        
    } else if (strcmp(token, ".CODE") ==0){         //If the directive is .CODE
        
        parse_code(instr_bin_str);
        
    } else if (strcmp(token, ".ADDR") ==0){         //If the directive is .ADDR

        parse_address(token, instr_bin_str);
        
    } else if (strcmp(token, ".DATA") ==0) {
            
        parse_data(instr_bin_str);              
    }
    
    return 0;
}

//To parse the assembly instruction for an addition directive after the directive is identified 
int parse_add (char* token, char* instr_bin_str){

    char opcode[] = "0001";      //Opcode for addition (arithmetic) 
    int instr_position = 0;
    const char delim[] = ", ";    //To split the tokens by a space or a comma    
    
    if (token == NULL){            //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_add() failed");
        return 4;
        
    } else {
        
        for (int i = 0; i < 4; i++){     //Loop through the opcode and append to the binary instruction string

            instr_bin_str[instr_position] = opcode[i];
            instr_position++;
        }    
        
        instr_bin_str[instr_position] = '\0';  //Cap off with a NULL
    }
    
    char* instr_bin_ptr = &instr_bin_str[instr_position];
    
    token = strtok(NULL, delim);     //Get the next token, which will be a register number
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);       //Parse out the register number
    }
    
    token = strtok(NULL, delim);      //Get the next token, which will be a register number
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);        //Parse out the register number
    }
    
    token = strtok(NULL, delim);          //Get the next token, which could be an IMM5 or a register
    
    if ((token[0] == 'R') && (token[1] != '\0')){
        
        parse_reg('0', instr_bin_ptr);             //Appends the sub-opcode if it is an addition with only registers
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);       //If it is a register value, parse the register
    
    } else {
        
        parse_imm(token, instr_bin_str);        //Otherwise, parse the IMM5 and append to the binary instruction string
        
    }
   
    return 0;    
}

//To parse the assembly instruction for a multiplication directive after the directive is identified 
int parse_mul (char* token, char* instr_bin_str){
    
    char opcode[] = "0001";      //Opcode for multiplication (arithmetic)
    int instr_position = 0;
    const char delim[] = ", ";
    
    if (token == NULL){               //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_mul() failed");   
        return 4;
        
    } else {
        
        for (int i = 0; i < 4; i++){           //Loop through the opcode and append to the binary instruction string

            instr_bin_str[instr_position] = opcode[i];
            instr_position++;
        }     
        
        instr_bin_str[instr_position] = '\0';
    }
    
    char* instr_bin_ptr = &instr_bin_str[instr_position];
    
    token = strtok(NULL, delim);     //Get the remaining tokens and parse for registers
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    parse_reg('1', instr_bin_ptr);              //Appends the sub-opcode
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    return 0;
    
}

//To parse the assembly instruction for a subtraction directive after the directive is identified
int parse_sub (char* token, char* instr_bin_str){
    
    char opcode[] = "0001";
    int instr_position = 0;
    const char delim[] = ", ";
    
    if (token == NULL){                     //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_sub() failed");
        return 4;
        
    } else {
        
        for (int i = 0; i < 4; i++){                   //Loop through the opcode and append to the binary instruction string

            instr_bin_str[instr_position] = opcode[i];
            instr_position++;
        }     
        
        instr_bin_str[instr_position] = '\0';
    }
    
    char* instr_bin_ptr = &instr_bin_str[instr_position];
    
    token = strtok(NULL, delim);                //Get the remaining tokens and parse for registers
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    parse_reg('2', instr_bin_ptr);              //Appends the sub-opcode
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    return 0;
    
}

//To parse the assembly instruction for a division directive after the directive is identified
int parse_div (char* token, char* instr_bin_str){
    
    char opcode[] = "0001";
    int instr_position = 0;
    const char delim[] = ", ";
    
    if (token == NULL){           //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_div() failed");
        return 4;
        
    } else {
        
        for (int i = 0; i < 4; i++){         //Loop through the opcode and append to the binary instruction string

            instr_bin_str[instr_position] = opcode[i];
            instr_position++;
        }     
        instr_bin_str[instr_position] = '\0';
    }
    
    char* instr_bin_ptr = &instr_bin_str[instr_position];
    
    token = strtok(NULL, delim);     //Get the remaining tokens and parse for registers
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    parse_reg('3', instr_bin_ptr);                //Appends the sub-opcode
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    return 0;
}

//To parse the assembly instruction for an and directive after the directive is identified
int parse_and (char* token, char* instr_bin_str){
    
    char opcode[] = "0101";
    int instr_position = 0;
    const char delim[] = ", ";
    
    if (token == NULL){                   //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_and() failed");
        return 4;
        
    } else {
        
        for (int i = 0; i < 4; i++){          //Loop through the opcode and append to the binary instruction string

            instr_bin_str[instr_position] = opcode[i];
            instr_position++;
        }     
        instr_bin_str[instr_position] = '\0';
    }
    
    char* instr_bin_ptr = &instr_bin_str[instr_position];
    
    token = strtok(NULL, delim);            //Get the remaining tokens and parse for registers
     
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    token = strtok(NULL, delim); 
    
    if ((token[0] == 'R') && (token[1] != '\0')){
        
        parse_reg('0', instr_bin_ptr);                   //Appends the sub-opcode if it is not an AND with IMM5
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    
    } else {
        
        parse_imm(token, instr_bin_str);
        
    }
   
    return 0;    
}

//To parse the assembly instruction for a not directive after the directive is identified
int parse_not (char* token, char* instr_bin_str){
    
    char opcode[] = "0101";
    int instr_position = 0;
    const char delim[] = ", ";
    
    if (token == NULL){              //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_not() failed");
        return 4;
        
    } else {
        
        for (int i = 0; i < 4; i++){           //Loop through the opcode and append to the binary instruction string

            instr_bin_str[instr_position] = opcode[i];
            instr_position++;
        }     
        instr_bin_str[instr_position] = '\0';
    }
    
    char* instr_bin_ptr = &instr_bin_str[instr_position];
    
    token = strtok(NULL, delim);               //Get the remaining tokens and parse for registers
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    parse_reg('1', instr_bin_ptr);            //Appends the sub-opcode
    
    return 0;  
}

//To parse the assembly instruction for an or directive after the directive is identified
int parse_or (char* token, char* instr_bin_str){
    
    char opcode[] = "0101";
    int instr_position = 0;
    const char delim[] = ", ";
    
    if (token == NULL){                 //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_or() failed");
        return 4;
        
    } else {
        
        for (int i = 0; i < 4; i++){              //Loop through the opcode and append to the binary instruction string

            instr_bin_str[instr_position] = opcode[i];
            instr_position++;
        }     
        instr_bin_str[instr_position] = '\0';
    }
    
    char* instr_bin_ptr = &instr_bin_str[instr_position];
    
    token = strtok(NULL, delim);                 //Get the remaining tokens and parse for registers
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    parse_reg('2', instr_bin_ptr);                //Appends the sub-opcode
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    return 0;  
}

//To parse the assembly instruction for an exclusive or directive after the directive is identified
int parse_xor (char* token, char* instr_bin_str){
    
    char opcode[] = "0101";
    int instr_position = 0;
    const char delim[] = ", ";
    
    if (token == NULL){                     //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_xor() failed");
        return 4;
        
    } else {
        
        for (int i = 0; i < 4; i++){             //Loop through the opcode and append to the binary instruction string

            instr_bin_str[instr_position] = opcode[i];
            instr_position++;
        }     
        instr_bin_str[instr_position] = '\0';
    }
    
    char* instr_bin_ptr = &instr_bin_str[instr_position];
    
    token = strtok(NULL, delim);               //Get the remaining tokens and parse for registers
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    parse_reg('3', instr_bin_ptr);               //Appends the sub-opcode
    
    token = strtok(NULL, delim);    
    
    if (token[1] != '\0'){
        
        char reg_num = token[1];
        
        parse_reg(reg_num, instr_bin_ptr);
    }
    
    return 0;  
}

//To parse the assembly instruction for an IMM5 (Addition or AND)
int parse_imm(char* token, char* instr_bin_str){
        
    if (token == NULL){                     //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_add_imm() failed");
        return 4;
    }         

    char imm_array[6];
    char* endPtr;
    unsigned int imm_result;
    int position;
    int x;

    if ( (token[0] == 'x') || (token [0] == 'X') ){             //Checks if the first character is an 'x' to signal a hex number

        position = 1;
        x = 0;
        while (token[position] != '\0'){               //Gets all digits of the hex number and places into a char array
            imm_array[x] = token[position];
            position ++;
            x++ ;
        }
        imm_array[x] = '\0';                      //Capps off the char array with a NULL

        int base = 16;
        imm_result = strtol(imm_array, &endPtr, base);       //Converts the char array into a unsigned int

    } else if ( token[0] == '#' ) {      //Checks if the first character is an '#' to signal a decimal number

        position = 1;
        x = 0;
        while (token[position] != '\0'){             //Gets all digits of the decimal number and places into a char array
            imm_array[x] = token[position];
            position ++;
            x++ ;
        }
        imm_array[x] = '\0';           //Capps off the char array with a NULL
        int base = 10;
        imm_result = strtol(imm_array, &endPtr, base);        //Converts the char array into a unsigned int

    } else if ( (token[1] == 'x') || (token [1] == 'X') ) {     //Checks if the second character is an 'x', as in 0x to signal a hex number

        position = 2;
        x = 0;
        while (token[position] != '\0'){     //Gets all digits of the hex number and places into a char array
            imm_array[x] = token[position];
            position ++;
            x++ ;
        }
        imm_array[x] = '\0';                    //Capps off the char array with a NULL

        int base = 16;
        imm_result = strtol(imm_array, &endPtr, base);        //Converts the char array into a unsigned int
    }            

    char bin_array[6];
    unsigned int bogey = 16;    
    int difference;
    int counter = 0;
    position = 0;

    //Converts the unsigned int into a binary string
    while (counter < 5){            //Since the immediate value is limited to 5 binary digits

        difference = imm_result - bogey;            

        if ( difference >= 0){            //Goes from the MSB and checks if it can fit into the IMM number
            bin_array[position] = '1';       //If yes, the MSB has a 1, otherwise it has a 0, then loop again
            imm_result = imm_result - bogey;

        } else {

            bin_array[position] = '0';                      
        }

        bogey = bogey / 2;          //Move the MSB over one place to the right and adjust the binary value
        position++;
        counter++;
    }

    bin_array[position] = '\0';       //Cap off the binary string
    
    int instr_position = 0;
    
    while (instr_bin_str[instr_position] != '\0'){          //Move the instruction binary string to the end
        
        instr_position ++;
    }
    
    instr_bin_str[instr_position] = '1';          //Since the IMM5 sub opcode starts with a '1', first append a '1'
    instr_position++;
    
    position = 0;
    
     while (bin_array[position] != '\0'){         //If the IMM5 has not reached the end of the array

        instr_bin_str[instr_position] = bin_array[position];      //Copy the binary values over to the instruction binary string
         
        instr_position++;             //Move through the remaining binary digits
        position++;
    }

    instr_bin_str[instr_position] = '\0';          //Cap off the binary string with a NULL
    
    return 0;
}

//To parse the assembly instruction registers and register numbers
int parse_reg (char reg_num, char* instr_bin_str) {
    
    //Binary values for the register numbers
    char reg_code0[] = "000";
    char reg_code1[] = "001";
    char reg_code2[] = "010";
    char reg_code3[] = "011";
    char reg_code4[] = "100";
    char reg_code5[] = "101";
    char reg_code6[] = "110";
    char reg_code7[] = "111";
    
    int instr_position = 0;
    
    while (instr_bin_str[instr_position] != '\0'){    //Move the instruction binary string to the end
        
        instr_position ++;
    }
    
    switch(reg_num) {             //Switch and case logic as instructed
            
        case '0':            
            for (int i = 0; i < 3; i++){
                
                instr_bin_str[instr_position] = reg_code0[i];       //If R0, copy over the binary value for 0
                instr_position++;
            }
            
            instr_bin_str[instr_position] = '\0';
            break;

        case '1':            
            for (int i = 0; i < 3; i++){
                
                instr_bin_str[instr_position] = reg_code1[i];   //If R1, copy over the binary value for 1
                instr_position++;
            }
            
            instr_bin_str[instr_position] = '\0';
            break;

        case '2':            
            for (int i = 0; i < 3; i++){
                instr_bin_str[instr_position] = reg_code2[i];      //If R2, copy over the binary value for 2
                instr_position++;
            }
            instr_bin_str[instr_position] = '\0';
            break;

        case '3':            
            for (int i = 0; i < 3; i++){
                instr_bin_str[instr_position] = reg_code3[i];       //If R3, copy over the binary value for 3
                instr_position++;
            }
            instr_bin_str[instr_position] = '\0';
            break;
        
        case '4':            
            for (int i = 0; i < 3; i++){
                instr_bin_str[instr_position] = reg_code4[i];       //If R4, copy over the binary value for 4
                instr_position++;
            }
            instr_bin_str[instr_position] = '\0';
            break;
        
        case '5':
            for (int i = 0; i < 3; i++){
                instr_bin_str[instr_position] = reg_code5[i];       //If R5, copy over the binary value for 5
                instr_position++;
            }
            instr_bin_str[instr_position] = '\0';
            break;
            
        case '6':
            for (int i = 0; i < 3; i++){
                instr_bin_str[instr_position] = reg_code6[i];        //If R6, copy over the binary value for 6
                instr_position++;
            }
            instr_bin_str[instr_position] = '\0';
            break;
            
        case '7':
            for (int i = 0; i < 3; i++){
                instr_bin_str[instr_position] = reg_code7[i];        //If R7, copy over the binary value for 7
                instr_position++;
            }
            instr_bin_str[instr_position] = '\0';
            break;
            
        default:
            printf("error5: parse_reg() failed");
            return 5;
    }
    
    return 0;    
}

//To convert binary numbers to strings of unsigned integers
unsigned short int str_to_bin (char* instr_bin_str){
    
    char* end_Ptr;

    unsigned int result;
    int base = 2;

    result = strtol(instr_bin_str, &end_Ptr, base);          //Uses strtol as instructed to perform the conversion

    if(*end_Ptr){
        
        printf("error6: str_to_bin() failed");      //Prints an error message if the end_ptr is NULL
        return 6;      
    } 
    
    return result;
}

//To write the hex values of ASM instructions into .obj format that can be used by the LC4
int write_obj_file (char* filename, unsigned short int program_bin[ROWS] ) {
    
    char *token;
    const char delim[] = ". ";            //To split the tokens by period or space
    char new_ending[] = ".obj";          
    FILE* fp ;
    
    token = strtok(filename, delim);        //Separates the file format from the name,
    
    token = strcat(token, new_ending);       //Swaps out the .asm for .obj
    
    fp = fopen(filename, "wb");
    
    if (fp == NULL){                  //If the file pointer is a NULL, print an error message and return 7
        
        printf("error7: write_obj_file() failed");
        return 7;
    }
    
    unsigned short int test_position = 0;
    unsigned short int hex_compare = 0xCADE;
    unsigned short int position = 0;
    
    //If the program does not start with a .CODE, then add .CODE and .ADDR manually to the start of the .OBJ file
    if (program_bin[0] != hex_compare){            
        
        unsigned short int header = 0xCADE ;
        header = endian_swap(header);    
        fwrite(&header, sizeof(unsigned short int), 1, fp);

        unsigned short int address = 0x0000 ;
        address = endian_swap(address); 
        fwrite(&address, sizeof(unsigned short int), 1, fp);
    
        while (program_bin[position] != '\0'){              //To count the number of instructions as per the requirements

            position ++;    
        }

        position = endian_swap(position);              //Performs a lower and upper bit swap due to endianness
        fwrite(&position, sizeof(unsigned short int), 1, fp);        //Write the number of instructions in hex code into the .obj file    
    
        position = 0;
        unsigned short int instr_program;
        while (program_bin[position] != '\0'){          //Grabs each set of hex instruction and swaps the higher and lower bits due to endianess
        
            instr_program = program_bin[position];

            instr_program = endian_swap(instr_program);

            fwrite(&instr_program, sizeof(unsigned short int), 1, fp);    //Writes each instruction in hex code into the .obj file

            position ++;
        }
        
    } else if (program_bin[0] == hex_compare) {          //Otherwise if the hex instructions already start with .CODE already, no need to duplicate
        
        position = 0;
        unsigned short int instr_program;

        while (program_bin[position] != '\0'){

            if (program_bin[position] == hex_compare){         //If the current hex instruction is a .CODE directive

                instr_program = program_bin[position];         
                instr_program = endian_swap(instr_program);                //Performs the lower and higher bit swap due to endianness
                fwrite(&instr_program, sizeof(unsigned short int), 1, fp);     //Writes the .CODE into the .OBJ file

                position ++;                //Moves to the next hex instruction (.ADDR)

                instr_program = program_bin[position];        
                instr_program = endian_swap(instr_program);                  //Performs the lower and higher bit swap due to endianness
                fwrite(&instr_program, sizeof(unsigned short int), 1, fp);      //Writes the .ADDR into the .OBJ file

                position ++;                 //Moves to the next step, which will be writing the # of instructions

                unsigned short int new_counter = position;
                unsigned short int temp_position = 0;

                while ( (program_bin[new_counter] != '\0') && (program_bin[new_counter] != hex_compare)){              //To count the number of instructions as per the requirements

                    temp_position ++;   
                    new_counter++;                 //Essentially adjusts the instruction count of the current .CODE block by 2 to account 
                }

                temp_position = endian_swap(temp_position);              //Performs a lower and upper bit swap due to endianness
                fwrite(&temp_position, sizeof(unsigned short int), 1, fp);        //Write the number of instructions in hex code into the .obj file                 

            } else {          

                instr_program = program_bin[position];

                instr_program = endian_swap(instr_program);                  //Swaps the lower and higher bits due to endianness

                fwrite(&instr_program, sizeof(unsigned short int), 1, fp);    //Writes each instruction in hex code into the .obj file

                position ++;
            }
        }
    }
    
    fclose(fp);              //Closes the file pointer
    
    return 0;
}

//To perform a swap of lower and higher bits due to endianness
unsigned short int endian_swap (unsigned short int flip_number){
    
    unsigned short int copy_one = flip_number >> 8 ;           //Shifts the hex number right by two digits (8 bits)
    
    unsigned short int copy_two = flip_number << 8 ;           //Shifts the hex number left by two digits (8 bits)
    
    unsigned short int swapped_final = copy_one | copy_two ;      //Performs an OR operation to complete the swap
    
    return swapped_final;
}

//To write the .CODE directive into a binary string that will be converted to hex for the .OBJ file
int parse_code(char* instr_bin_str){
    
    char code_array[] = "0xCADE";
    
    unsigned int code_result;      
    int base = 16;
    code_result = strtol(code_array, NULL, 0);       //Converts the char array into a unsigned int
    
    char bin_array[17];
    unsigned int bogey = 32768;    
    int difference;
    int counter = 0;
    int position = 0;

    //Converts the unsigned int into a binary string
    while (counter < 16){            //Since the hex number will result in a 16 bit binary

        difference = code_result - bogey;            

        if ( difference >= 0){            //Goes from the MSB and checks if it can fit into the original hex value
            bin_array[position] = '1';       //If yes, the MSB has a 1, otherwise it has a 0, then loop again
            code_result = code_result - bogey;

        } else {

            bin_array[position] = '0';                      
        }

        bogey = bogey / 2;          //Move the MSB over one place to the right and adjust the binary value
        position++;
        counter++;
    }

    bin_array[position] = '\0';       //Cap off the binary string
    
    int instr_position = 0;
    
    while (instr_bin_str[instr_position] != '\0'){          //Move the instruction binary string to the end
        
        instr_position ++;
    }
    
    position = 0;
    
     while (bin_array[position] != '\0'){         //If the binary string has not reached the end of the array

        instr_bin_str[instr_position] = bin_array[position];      //Copy the binary values over to the instruction binary string
         
        instr_position++;             //Move through the remaining binary digits
        position++;
    }

    instr_bin_str[instr_position] = '\0';          //Cap off the binary string with a NULL
    
    return 0;
}

//To write the .ADDR location into a binary string that will be converted to hex for the .OBJ file
int parse_address(char* instr, char* instr_bin_str){
    
    const char delim[] = ", ";    //To split the tokens by a space or a comma    
    char* token = instr;
    
    if (token == NULL){            //If the token is NULL, return 4 and print an error message
        
        printf("error4: parse_address() failed");
        return 4;
    } 
    
    token = strtok(NULL, delim);     //Get the next token, will be the address for .ADDR
        
    char address_array[7];
    unsigned int address_result; 
    char* endPtr;
    int x;
    int position;
    
    if ( (token[0] == 'x') || (token [0] == 'X') ){             //Checks if the first character is an 'x' to signal a hex number

        position = 1;
        x = 0;
        while (token[position] != '\0'){               //Gets all digits of the hex number and places into a char array
            address_array[x] = token[position];
            position ++;
            x++ ;
        }
        address_array[x] = '\0';                      //Caps off the char array with a NULL

        int base = 16;
        address_result = strtol(address_array, &endPtr, base);       //Converts the char array into a unsigned int

    } else if ( token[0] == '#' ) {      //Checks if the first character is an '#' to signal a decimal number

        position = 1;
        x = 0;
        while (token[position] != '\0'){             //Gets all digits of the decimal number and places into a char array
            address_array[x] = token[position];
            position ++;
            x++ ;
        }
        address_array[x] = '\0';           //Capps off the char array with a NULL
        int base = 10;
        address_result = strtol(address_array, &endPtr, base);        //Converts the char array into a unsigned int

    } else if ( (token[1] == 'x') || (token [1] == 'X') ) {     //Checks if the second character is an 'x', as in 0x to signal a hex number

        position = 2;
        x = 0;
        while (token[position] != '\0'){     //Gets all digits of the hex number and places into a char array
            address_array[x] = token[position];
            position ++;
            x++ ;
        }
        address_array[x] = '\0';                    //Capps off the char array with a NULL

        int base = 16;
        address_result = strtol(address_array, &endPtr, base);        //Converts the char array into a unsigned int
    }        
     
    address_array[position] = '\0';
    
    char bin_array[17];
    unsigned int bogey = 32768;    
    int difference;
    int counter = 0;
    
    position = 0;

    //Converts the unsigned int into a binary string
    while (counter < 16){            //Since the hex number will result in a 16 bit binary

        difference = address_result - bogey;            

        if ( difference >= 0){            //Goes from the MSB and checks if it can fit into the original hex value
            bin_array[position] = '1';       //If yes, the MSB has a 1, otherwise it has a 0, then loop again
            address_result = address_result - bogey;

        } else {

            bin_array[position] = '0';                      
        }

        bogey = bogey / 2;          //Move the MSB over one place to the right and adjust the binary value
        position++;
        counter++;
    }

    bin_array[position] = '\0';       //Cap off the binary string
    
    int instr_position = 0;
    
    while (instr_bin_str[instr_position] != '\0'){          //Move the instruction binary string to the end
        
        instr_position ++;
    }
    
    position = 0;
    
     while (bin_array[position] != '\0'){         //If the binary string has not reached the end of the array

        instr_bin_str[instr_position] = bin_array[position];      //Copy the binary values over to the instruction binary string
         
        instr_position++;             //Move through the remaining binary digits
        position++;
    }

    instr_bin_str[instr_position] = '\0';          //Cap off the binary string with a NULL
        
    return 0;
}

//To write the .DATA directive into a binary string that will be converted to hex for the .OBJ file
int parse_data(char* instr_bin_str){
    
    char code_array[] = "0xDADA";
    
    unsigned int code_result;      
    int base = 16;
    code_result = strtol(code_array, NULL, 0);       //Converts the char array into a unsigned int
    
    char bin_array[17];
    unsigned int bogey = 32768;    
    int difference;
    int counter = 0;
    int position = 0;

    //Converts the unsigned int into a binary string
    while (counter < 16){            //Since the hex number will result in a 16 bit binary

        difference = code_result - bogey;            

        if ( difference >= 0){            //Goes from the MSB and checks if it can fit into the original hex value
            bin_array[position] = '1';       //If yes, the MSB has a 1, otherwise it has a 0, then loop again
            code_result = code_result - bogey;

        } else {

            bin_array[position] = '0';                      
        }

        bogey = bogey / 2;          //Move the MSB over one place to the right and adjust the binary value
        position++;
        counter++;
    }

    bin_array[position] = '\0';       //Cap off the binary string
    
    int instr_position = 0;
    
    while (instr_bin_str[instr_position] != '\0'){          //Move the instruction binary string to the end
        
        instr_position ++;
    }
    
    position = 0;
    
     while (bin_array[position] != '\0'){         //If the binary string has not reached the end of the array

        instr_bin_str[instr_position] = bin_array[position];      //Copy the binary values over to the instruction binary string
         
        instr_position++;             //Move through the remaining binary digits
        position++;
    }

    instr_bin_str[instr_position] = '\0';          //Cap off the binary string with a NULL
    
    return 0;
}