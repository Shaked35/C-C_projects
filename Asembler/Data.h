/**
 * @file 	Data.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant methods for the .string & .data process
 */

#ifndef DATA_H_
#define DATA_H_
#include <stdio.h>
#include "Types.h"


/**
 * @brief       Processes a data initialization line
 * @param     	1. Line information
 * 				2. Current DC value
 * 				3. Label value
 * 				4. Type of data (.string, .data)
 * 				5. Does a symbol exists
 */
void first_iteration_process_data(iteration_data* transition, char* label, char* type, bool is_symbol, bool is_macro);

/**
 * @brief       Processes a string
 * @param     	1. Line information
 * 				2. Current DC value
 */
void process_string(iteration_data* transition);

/**
 * @brief       Processes numbers definition
 * @param     	1. Line information
 * 				2. Current DC value
 */
void process_numbers(iteration_data* transition);

/**
 * @brief       Gets the next number from the .data line
 * @param     	1. Current transition data
 * 				2. Pointer to the resulted number
 * @return 		Is number valid
 */
bool get_next_number(iteration_data* transition, int* number, bool* is_macro);

/**
 * @brief       Adds new data found in code to the list
 * @param     	A new data node
 */
void add_data_node_to_list(data_node_ptr p_new_data);

/**
 * @brief       Adds a string to the data list
 * @param     	A character of the string
 * @return 		Was the add successful
 */
bool add_string_data_to_list(char token, unsigned int address);

/**
 * @brief       Adds a numeric data to the list
 * @param     	1. A number
 *              2. Address
 *              3. Index in array
 * @return 		Was the add successful
 */
bool add_numeric_data_to_list(int number, unsigned int address, unsigned int index, bool is_macro);

/**
 * @brief       Updates data addresses according to code block size
 * @param     	Code block size
 */
void data_address_reloaded(int ic_length);

/**
 * @brief       Writes all data definitions into an output file
 * @param     	Output file
 */
void write_data_to_output_file(FILE* p_file);

/**
 * @brief       Free memory list
 */
void free_data_node_list();



char* get_macro_label(iteration_data* transition);

#endif /* DATA_H_ */
