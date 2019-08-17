/**
 * @file 	Entry.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant methods for the .entry process
 */

#ifndef ENTERY_H_
#define ENTERY_H_

#include "Types.h"

/**
 * @brief       Processes an entry line in the second transition
 * @param     	Line information 
 */
void first_iteration_process_entry(iteration_data* transition);

/**
 * @brief       Processes an entry line in the second transition
 * @param     	1. Line information
 * 				2. Output files
 */
void second_iteration_process_entry(iteration_data* transition, compiler_output_files* output_files);

/**
 * @brief       Write the entry into the entry output file
 * @param     	1. Entry name
 * 				2. Entry definition address
 * 				3. Output file
 */
void write_entry_to_output_file(char* entry_name, unsigned int address, FILE* output_file);

/**
 * @brief       Creates the entry file if it doesn't exists
 * @param     	1. Output files
 * 				2. File name without extension
 */
void create_entry_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension);

#endif /* ENTERY_H_ */
