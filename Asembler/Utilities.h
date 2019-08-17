/**
 * @file 	Utilities.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant methods for the Utilities process;
 */

#ifndef OPERATIONENCODER_H_
#define OPERATIONENCODER_H_

#include <stdbool.h>
#include <stdio.h>
#include "Types.h"

/**
 * @brief       Process an operation line on first transition
 * @param     	1. Current transition data
 * 				2. Label value (NULL if doesn't exist)
 * 				3. Does label exist
 */
void first_iteration_process_operation(iteration_data* transition, char* label, bool is_symbol);

/**
 * @brief       Process an operation line on second transition
 * @param     	1. Current transition data
 * 				2. Output files
 */
void second_iteration_process_operation(iteration_data* transition, compiler_output_files* p_output_files);

/**
 * @brief       Get all data of operation in line
 * @param     	1. Line information
 * 				2. Can use copy previous address method
 * 				3. Previous address method
 * @return 		Operation data
 */
decoded_operation* get_decoded_operation(iteration_data* transition);

/**
 * @brief       Gets the operation name from the line
 * @param     	Line information
 * @return 		Operation name
 */
char* get_operation_name(iteration_data* transition);

/**
 * @brief       Gets how many times we need to encode the operation
 * @param     	1. Current transition
 * 				2. Out - how many times we need to encode the operation
 * @return 		Is the result valid
 */
bool get_operation_times_counter(iteration_data* transition, int* times);

/**
 * @brief       Reads the operands from the line
 * @param     	1. Current transition data
 * 				2. Number of operands to read
 * 				3. Returned source operand
 * 				4. Returned target operand
 * @return 		Are operands valid
 */
bool get_all_operands(iteration_data* transition, int operands_count, char** source_operand, char** target_operand);

/**
 * @brief       Reads the next operand from the line
 * @param     		Current transition
 * @return 		Operand
 */
char* get_next_operand(iteration_data* transition);

/**
 * @brief       Gets the address method according to the operand
 * @param     	1. Current transition
 * 				2. Current operand
 * @return 		Method ID
 */
METHOD_ID get_address_method(iteration_data* transition, char* operand);

/**
 * @brief       Replaces the operand in case it is copy address
 * @param     	1. Current transition data
 * 				2. Current operand
 * 				3. Current operand address method
 * @return 		Is usage of Copy Address valid
 */
bool replace_operand_if_copy_address(iteration_data* transition, char** operand, METHOD_ID* operand_address_method);

/**
 * @brief       Checks whether the address methods used in the operation are authorized
 * @param     	Current decoded operation
 * @return 		Is authorized
 */
bool is_valid_operand_method(decoded_operation* current_operation);

/**
 * @brief       Calculates how many memory words are used to encode the operation
 * @param     	1. Current transition data
 * 				2. Decoded operation
 * @return 		How many memory words should be used to encode the operation
 */
int words_size_calculation(iteration_data* transition, decoded_operation* current_operation);

/**
 * @brief       Updates the transition with the first operand of the operation
 * @param     	1. Current transition data
 * 				2. Current decoded operation
 */
void update_transition_with_last_operation(iteration_data* transition, decoded_operation* decoded_operation);

/**
 * @brief       Encodes an operation into its output file
 * @param     	1. Current transition data
 * 				2. Decoded operation
 * 				3. Output files
 * @return 		Was the operation encoded successfully
 */
bool operands_final_process(iteration_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files);

/**
 * @brief       Encodes operands into output files
 * @param     	1. Current transition data
 * 				2. Decoded operation
 * 				3. Output files
 * @return 		Were operands encoded successfully
 */
bool encode_operands(iteration_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files);

/**
 * @brief       Encodes a direct operand
 * @param     	1. Current transition
 * 				2. Direct operand
 * 				3. Output files
 * @return 		Was operand encoded successfully
 */
bool encode_direct(iteration_data* transition, char* operand, compiler_output_files* output_files);

/**
 * @brief       Encodes a direct operand
 * @param     	1. Current transition
 * 				2. Direct operand
 * 				3. Output files
 * @return 		Was operand encoded successfully
 */
bool encode_index(iteration_data* transition, char* operand, compiler_output_files* output_files);

/**
 * @brief       Encodes register operands
 * @param     	1. Current transition
 * 				2. Source register
 * 				3. Target register
 * 				3. Output file
 * @return 		Were operands encoded successfully
 */
bool encode_registers(iteration_data* transition, char* source_register, char* target_register, FILE* p_file);

/**
 * @brief       Encodes a immediate number
 * @param     	1. Current transition
 * 				2. Immediate operand
 *              3. Output file
 * 				
 * @return 		Was operand encoded successfully
 */
bool encode_immediate(iteration_data* transition, char* operand, FILE* p_file);

/**
 * @brief       Searches for an operation name in a pre-defined list of valid operations
 * @param     	Operation name
 * @return 		If found returns machine operation information, otherwise NULL
 */
machine_operation_definition* search_machine_operation_in_list(char* operation);

/**
 * @brief       Initializes the operation list used for encoding
 * @return 		Did the initialization end
 */
bool init_operation_list();

/**
 * @brief       Adds an operation into the list
 * @param     	1. Operation name
 * 				2. Op code
 * 				3. Number of operands
 * @return 		Did add successfully
 */
bool add_operation_to_list(char* name, unsigned int code, int operands);

/**
 * @brief       Free memory list
 */
void free_operation_list();

#endif /* OPERATIONENCODER_H_ */
