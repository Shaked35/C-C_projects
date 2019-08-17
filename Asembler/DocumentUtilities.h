/**
 * @file 	DocumentUtilities.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant methods for the DocumentUtilities process
 */

#ifndef ACCESSORIES_H_
#define ACCESSORIES_H_

#include <stdbool.h>
#include "Types.h"
#include "Enums.h"


/**
 * @brief       Checks if operand is a register
 * @param     	Operand
 * @return 		True if register, otherwise false
 */
bool is_register(char* operand);

/**
 * @brief       Prints a compiler error to the user
 * @param     	1. Message
 * 				2. Current line
 */
void print_compiler_error(char* message, line_data* info);

/**
 * @brief       Prints runtime error
 * @param     	Message
 */
void print_runtime_error(char* message);

/**
 * @brief       Allocates a string
 * @param     	Number of tokens in string
 * @return 		Allocated string
 */
char* allocate_string(int string_length);

/**
 * @brief       Allocates memory
 * @param     	Number of bytes to allocate
 * @return 		Pointer to number
 */
void* allocate_memory(int bytes);

/**
 * @brief       Skips all the spaces until next word
 * @param     	Line information
 */
void skip_all_spaces(line_data* info);

/**
 * @brief       Read next word from line
 * @param     	Current transition data
 * @return 		Extracted word
 */
char* get_next_word(iteration_data* transition);

/**
 * @brief       Skips the label
 * @param     	Line information
 */
void skip_label(line_data* info);

/**
 * @brief       Creates a line information structure
 * @param     	1. File name
 * 				2. Line number
 * 				3. Line content
 * @return 		Pointer to line information
 */
line_data* create_line_info(char* file_name, int line_number, char* line_str);

/**
 * @brief       Checks if this is an empty line or a comment
 * @param     	The line to check
 * @return 		Is this an empty line or a comment
 */
bool is_empty_or_comment(char* line);

/**
 * @brief       Checks if there are tokens that aren't spaces after current index
 * @param     	Line information
 * @return 		True if found non-space token, otherwise false
 */
bool is_end_of_data_in_line(line_data* info);

/**
 * @brief       Converts a number in base 10 to wanted base
 * @param     	1. Number in base 10
 * 				2. Wanted base
 * 				3. Minimal length of returned word
 * @return 		Number in wanted base
 */
char* convert_base10_to_target_base(unsigned int base10_number, int target_base, int minimal_returned_length);

/**
 * @brief       Checks if the string is a valid label
 * @param     	String to check
 * @return 		True if label, otherwise false
 */
bool is_valid_label(char* str);

/**
 * @brief       Checks if string is an operation name
 * @param     	String to check
 * @return 		True if string is an operation, otherwise false
 */
bool is_operation_name(char* str);

/**
 * @brief       Checks if this is a valid operation line
 * @param     	Line information
 * @return      bool: is it valid operation line
 */
bool is_valid_is_operation_line (line_data* info);

/**
 * @brief       Replaces the content of old string with current string
 * @param     	1. Old string
 * 				2. New string
 */
void replace_content(char** current_string, char* new_string);

/**
 * @brief       Creates a transition data structure
 * @return 		Basic transition data
 */
iteration_data* create_transition_data();

/**
 * @brief       Creates an output file from given file name
 * @param     	1. File name without extension
 * 				2. File extension
 * @return 		Reference to created file
 */
FILE* create_output_file(char* file_name_without_extension, char* extension);

/**
 * @brief       Checks if we've reached end of line
 * @param     	Line information
 * @return 		True if we've reached end of lines, otherwise false
 */
bool is_end_of_line(line_data* info);

/**
 * @brief       Writes encoding into output file
 * @param     	1. Address in base 10
 * 				2. Memory word value in base 10
 * 				3. Output file
 * @return      bool: is it the end of line
 */
void print_encoding_to_file(unsigned int ic, unsigned int value, FILE* p_file);

#endif /* UTILITIES_H_ */
