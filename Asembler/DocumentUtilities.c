/**
 * @file 	DocumentUtilities.c
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   This file Contains utils methods used to process assembly line
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Constans.h"
#include "Types.h"
#include "DocumentUtilities.h"


/**
 * @brief       Read next word from line
 * @param     	Current transition data
 * @return 		The next word without spaces
 */
char* get_next_word(iteration_data* transition)
{
	char* word;
	int i, word_end_index, word_start_index, word_length;

	skip_all_spaces(transition->current_line_information);

	word_end_index  = word_start_index = i = transition->current_line_information->current_index;

	/* Find word position */
	for (;i < transition->current_line_information->line_length; i++) {
		if (!isspace(transition->current_line_information->line_str[i])) {
			word_end_index = i;
		} else {
			break;
		}
	}

	word_length = word_end_index - word_start_index + 1;

	word = allocate_string(word_length);

	if (word == NULL) {
		transition->is_runtimer_error = true;
	} else {
		strncpy(word, transition->current_line_information->line_str + word_start_index, word_length);
		(word)[word_length] = END_OF_STRING;

		transition->current_line_information->current_index = word_end_index + 1;
	}

	return word;
}

/**
 * @brief       Skips all the spaces until next char
 * @param     	Line information
 */
void skip_all_spaces(line_data* info) {
	int index = info->current_index;

	/* Scans all spaces */
	while ((index < info->line_length) && isspace(info->line_str[index])) {
		index++;
	}

	info->current_index = index;
}

/**
 * @brief       Creates a line information structure
 * @param     	1. File name
 * 				2. line_number: line index
 * 				3. Line content
 * @return 		Pointer to line information
 */
line_data* create_line_info(char* file_name, int line_number, char* line_str) {
	line_data* info = (line_data*)allocate_memory(sizeof(line_data));

	/* Initialize information*/
	if (info != NULL) {
		info->current_index = 0;
		info->file_name = file_name;
		info->line_number = line_number;
		info->line_str = line_str;
		info->line_length = strlen(line_str);
	}

	return info;
}

/**
 * @brief       Checks if this is an empty line or a comment
 * @param     	The line to check
 * @return 		Is this an empty line or a comment
 */
bool is_empty_or_comment(char* line) {
	int i;
	int length = strlen(line);

	/* Check's there is a token that isn't a space */
	for (i = 0; i < length; i++) {
		if (!isspace(line[i])) {
			return line[i] == COMMENT;
		}
	}

	return true;
}

/**
 * @brief       Converts a number in base 10 to wanted base
 * @param     	1. Number in base 10
 * 				2. Wanted base
 * 				3. Minimal length of returned word
 * @return 		Number in wanted base
 */
char* convert_base10_to_target_base(unsigned int base10_number, int target_base, int minimal_returned_length) {
	char* result = NULL;
	char* current_token = NULL;
	int result_length;
	int remainder = 0;
	
	/* char outPut;*/
	result = allocate_string(1);

	if (result == NULL) {
		print_runtime_error("Could not allocate memory. Exit program");
	} else {
		result[0] = END_OF_STRING;
		result_length = 1;
	}
	while (base10_number != 0) {
		int temp;

		temp = base10_number / target_base;
		remainder = base10_number - temp * target_base;
		base10_number = temp;

		current_token = allocate_string(result_length);
		if (current_token != NULL) {
			current_token[0] = '0'+remainder;
			current_token[1] = END_OF_STRING;

			strcat(current_token, result);

			free(result);
			result = current_token;
		} else {
			if (result != NULL) {
				free(result);
			}

			return NULL;
		}
	}

	/* Making sure that the returned result is with the right length and if not, add 0 to the right */
	result_length = strlen(result);

	/* Add zeros to the left if necessary */
	while (result_length < minimal_returned_length) {
		current_token = allocate_string(result_length);
		current_token[0] = '0';
		current_token[1] = END_OF_STRING;

		strcat(current_token, result);

		free(result);
		result = current_token;
		result_length++;
	}

	return result;
}

/**
 * @brief       Allocates a string
 * @param     	Number of tokens in string
 * @return 		Allocated string
 */
char* allocate_string(int string_length) {
	return (char*)allocate_memory(sizeof(char) * (string_length + 1));
}

/**
 * @brief       Allocates memory
 * @param     	Number of bytes to allocate
 * @return 		Pointer to number
 */
void* allocate_memory(int bytes) {
	void* result = (void*)malloc(bytes);

	if (result == NULL) {
		print_runtime_error("Could not allocate memory. Exit program");
	}

	return result;
}

/**
 * @brief       Checks if there are tokens that aren't spaces after current index
 * @param     	Line information
 * @return 		True if found non-space token, otherwise false
 */
bool is_end_of_data_in_line(line_data* info) {
	bool is_end = true;
	int i;

	/* Searches non-space token */
	for (i = info->current_index; i < info->line_length; i++) {
		if (!isspace(info->line_str[i])) {
			is_end = false;
			break;
		}
	}

	return is_end;
}

/**
 * @brief       Skips the label
 * @param     	Line information
 */
void skip_label(line_data* info) {
	/* Search the label end token */
	char* partial_line = strchr(info->line_str, LABEL_END_TOKEN);
	if (partial_line != NULL) {
		info->current_index = partial_line - info->line_str + 1;
	}
}

/**
 * @brief       Checks if the string is a valid label
 * @param     	String to check
 * @return 		True if label, otherwise false
 */
bool is_valid_label(char* str) {
	int i;
	int len = strlen(str);

	/* The first letter in the label isn't a number & string is shorter then maximum length (excluding the end of label token)*/
	if (!isalpha(str[0]) || len > LABEL_MAX_LENGTH) {
		return false;
	}

	/* The rest are alpha numeric*/
	for (i = 1; i < len; i++) {
		if (!isalnum(str[i])) {
			return false;
		}
	}

	/* All was fine */
	return !is_operation_name(str) && !is_register(str);
}

/**
 * @brief       Checks if string is an operation name
 * @param     	String to check
 * @return 		True if string is an operation, otherwise false
 */
bool is_operation_name(char* str) {
	return (strcmp(str, MOV) == 0) || (strcmp(str, CMP) == 0) ||
			(strcmp(str, ADD) == 0) || (strcmp(str, SUB) == 0) ||
			(strcmp(str, NOT) == 0) || (strcmp(str, CLR) == 0) ||
			(strcmp(str, LEA) == 0) || (strcmp(str, INC) == 0) ||
			(strcmp(str, DEC) == 0) || (strcmp(str, JMP) == 0) ||
			(strcmp(str, BNE) == 0) || (strcmp(str, RED) == 0) ||
			(strcmp(str, PRN) == 0) || (strcmp(str, JSR) == 0) ||
			(strcmp(str, RTS) == 0) || (strcmp(str, STOP) == 0);
}

/**
 * @brief       Checks if this is a valid operation line
 * @param     	1. Line information
 */
bool is_valid_is_operation_line (line_data* info) {

	return info->line_length < OPERATION_LINE_MAX_LENGTH;
}

/**
 * @brief       Replaces the content of old string with current string
 * @param     	1. Old string
 * 				2. New string
 */
void replace_content(char** current_string, char* new_string) {
	char* temp = allocate_string(strlen(new_string));

	if (*current_string != NULL) {
		free(*current_string);
	}

	*current_string = temp;
	strcpy(*current_string, new_string);
}

/**
 * @brief       Creates a transition data structure
 * @return 		Basic transition data
 */
iteration_data* create_transition_data() {
	iteration_data* transition = (iteration_data*)allocate_memory(sizeof(iteration_data));

	/* Initialize */
	if (transition != NULL) {
		transition->IC = EMPTY_ADDRESS;
		transition->DC = EMPTY_ADDRESS;
		transition->prev_operation_operand = NULL;
		transition->is_compiler_error = false;
		transition->is_runtimer_error = false;
		transition->current_line_information = NULL;
	}

	return transition;
}

/**
 * @brief       Creates an output file from given file name
 * @param     	1. File name without extension
 * 				2. File extension
 * @return 		Reference to created file
 */
FILE* create_output_file(char* file_name_without_extension, char* extension) {
	FILE* p_output_file = NULL;
	int file_name_length = strlen(file_name_without_extension) + strlen(extension);

	char* file_name = allocate_string(file_name_length);
	
	if (file_name != NULL) {
		/* Creates code file output name */
		strcpy(file_name, file_name_without_extension);
		strcat(file_name, extension);
		file_name[file_name_length] = END_OF_STRING;
		p_output_file = fopen(file_name, WRITE_MODE);

		free(file_name);

		if (!p_output_file) {
			print_runtime_error("Cannot create output file");
		}
	}

	return p_output_file;
}

/**
 * @brief       Checks if we've reached end of line
 * @param     	Line information
 * @return 		True if we've reached end of lines, otherwise false
 */
bool is_end_of_line(line_data* info) {
	return info->current_index == info->line_length;
}

/**
 * @brief       Checks if operand is a register
 * @param     	Operand
 * @return 		True if register, otherwise false
 */
bool is_register(char* operand) {
	int digit;
	int length = strlen(operand);

	if ((length != 2) || (operand[0] != REGISTER_FIRST_TOKEN) || (!isdigit(operand[1]))) {
		return false;
	}

	digit = atoi(operand + 1);

	return (digit < REGISTERS_COUNT) && (digit >= 0);
}

/**
 * @brief       Prints a compiler error to the user
 * @param     	1. Error essage
 * 				2. Current line data
 */
void print_compiler_error(char* message, line_data* info) {
	fprintf(stderr, "Error: %s, File %s Line %d \n", message, info->file_name, info->line_number);
}

/**
 * @brief       Prints runtime error
 * @param     	Message
 */
void print_runtime_error(char* message) {
	fprintf(stderr, "Error: %s \n", message);
}

/**
 * @brief       Writes encoding into output file
 * @param     	1. Address in base 10
 * 				2. Memory word value in base 10
 * 				3. Output file
 */
void print_encoding_to_file(unsigned int address, unsigned int value, FILE* p_file) {
	char* base4_value;
	char* outPut;
	int i;
	/* Print Address base value */
	outPut = convert_base10_to_target_base(address, BASE_10, TARGET_MEMORY_ADDRESS_WORD_LENGTH);
	printf("%s ",outPut);
	fputs(outPut, p_file);
	fputc(COLUMN_SEPARATOR, p_file);

	free(outPut);

	/* Print operation base value */
	outPut = base4_value = convert_base10_to_target_base(value, TARGET_BASE, TARGET_OPERATION_WORD_LENGTH);
	/* Convert base value to special value */
	printf(": %d \n",value);
	for(i=0;i<sizeof(base4_value);i++)
	if(base4_value[i] == BASE_4_ZERO){
		outPut[i] = SPECIAL_BASE_ZERO;}
	else if(base4_value[i] == BASE_4_ONE){
		outPut[i] = SPECIAL_BASE_ONE;
	}
	else if(base4_value[i] == BASE_4_TWO){
		outPut[i] = SPECIAL_BASE_TWO;
	}
	else if(base4_value[i] == BASE_4_THREE){
		outPut[i] = SPECIAL_BASE_THREE;
	}			
	fputs(outPut, p_file);

	free(base4_value);

	fputc(NEXT_LINE_SYMBOL, p_file);
}
