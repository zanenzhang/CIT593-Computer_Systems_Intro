/***************************************************************************
 * file name   : assembler.c                                               *
 * author      : tjf & you                                                 *
 * description : This program will assemble a .ASM file into a .OBJ file   *
 *               This program will use the "asm_parser" library to achieve *
 *			     its functionality.										   * 
 *                                                                         *
 ***************************************************************************
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm_parser.h"

int main(int argc, char** argv) {

	char* filename = NULL ;					// name of ASM file
	char  program [ROWS][COLS] ; 			// ASM file line-by-line
	char  program_bin_str [ROWS][17] ; 		// instructions converted to a binary string
	unsigned short int program_bin [ROWS] ; // instructions in binary (HEX)
    unsigned short int addresses[ROWS];
    
    //Assumes the filename will always be after ./assembler as per the instructions
    if (argc > 1){             

        filename = argv[1];    //Get the filename after the assembler is called, i.e. argc is greater than 1 
        
        read_asm_file (filename, program ) ;    //Take the file and call the read_asm_file function to copy the ASM instructions into the 2D array

        int array_row = 0;
        char* instr = program[array_row];
        char* instr_bin_str = program_bin_str[array_row];
        
        //Loops through each line in the 2D array of .ASM directives
        while (program[array_row][0] != '\0'){
            
            parse_instruction (instr, instr_bin_str);       //Parse the instruction into a binary string
            
            program_bin[array_row] = str_to_bin (instr_bin_str);    //Converts the binary string into hex numbers
            
            array_row ++;                              //Resets the counter and array positions for the next append
            instr = program[array_row];             
            instr_bin_str = program_bin_str[array_row];
        }
        
        //Caps off with a NULL to mark the end of each string
        program[array_row][0] = '\0';          
        program_bin_str[array_row][0] = '\0';
        program_bin[array_row] = '\0';
        
        //Write the instructions in hex to an .obj file
        write_obj_file (filename, &program_bin[0]);        

    } else {

        //If only the assembler was called and a filename was not provided, print an error and return 1
        fprintf(stderr, "error1: usage: ./assembler <assembly_file.asm>") ;    
        return 1;
    }
    
}
