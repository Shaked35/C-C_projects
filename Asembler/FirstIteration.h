/**
 * @file 	FirstIteration.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant methods for the first Iteration process;
 */

#ifndef FIRSTITERATION_H_
#define FIRSTITERATION_H_

#include <stdio.h>
#include <stdbool.h>
#include "Types.h"

/**
 * @brief       Executes the first Iteration of the assembly compiler
 * @param     	1. Input file
 * 				2. File name
 * @return 		Was Iteration successful
 */
bool first_iteration_execute(FILE* p_file, char* file_name, unsigned int* IC, unsigned int* DC);

/**
 * @brief       Processes a line according to its' type
 * @param     	Line information
 */
void first_iteration_process_line(iteration_data* transition);

#endif /* FIRSTTRANSITION_H_ */
