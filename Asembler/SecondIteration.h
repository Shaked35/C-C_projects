/**
 * @file 	SecondIteration.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant methods for the second Iteration process;
 */

#ifndef SECONDITERATION_H_
#define SECONDITERATION_H_

#include <stdio.h>
#include "Types.h"

/**
 * @brief       Executes the second Iteration
 * @param     	1. Input file handle
 * 				2. Name of input file
 * 				3. Code length in memory word
 * 				4. Data length in memory word
 */
void second_iteration_execute(FILE* p_file, char* file_name_without_extension, unsigned int previous_transition_ic, unsigned int previous_transition_dc);

/**
 * @brief       Processes a line in input file
 * @param     	1. Current transition data
 * 				2. Output files
 */
void second_iteration_process_line(iteration_data* transition, compiler_output_files* output_files);

/**
 * @brief       Writes first line of ob file
 * @param     	1. IC value
 * 				2. DC value
 * 				3. Ob file handle
 */
void write_code_and_data_size_to_output_file(unsigned int ic, unsigned int dc, FILE* output_file);

#endif /* SECONDTRANSITION_H_ */
