/**
 * @file 	Extern.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   This file Contains methods relevant to .extern line processing;
 */

#ifndef EXTERN_H_
#define EXTERN_H_

#include <stdio.h>
#include "Types.h"


/**
 * @brief       Creates the entry file if it doesn't exists
 * @param     	1. Output files
 * 				2. File name without extension
 */
void create_extern_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension);

/**
 * @brief       Process the extern definition in first transition.
 * 				Adds it into the symbol table
 * @param     	Current transition
 */
void first_iteration_process_extern(iteration_data* transition);

/**
 * @brief       Write the extern into the extern output file
 * @param     	1. Extern name
 * 				2. Extern usage address
 * 				3. Output file
 */
void write_extern_to_output_file(char* extern_operand, unsigned int current_address, FILE* output_file);

#endif /* EXTERN_H_ */
