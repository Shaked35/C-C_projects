/**
 * @file 	Data.c
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   This file Contains methods relevant to .string and .data line processing;
 */

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Types.h"
#include "DocumentUtilities.h"
#include "Data.h"
#include "Constans.h"
#include "SymbolTable.h"

/* DATA head and tail */
data_node_ptr p_data_head = NULL;
data_node_ptr p_data_tail = NULL;


/**
 * @brief       First iteration process on data
 * @param     	1. Line information
 * 				2. Label value
 * 				3. Type of data (.string, .data, .define)
 * 				4. Does a symbol exists
 * 				5. Is it a macro
 */
void first_iteration_process_data(iteration_data* transition, char* label, char* data_type, bool is_symbol, bool is_macro) {
	
	symbol_node_ptr p_symbol;
	/*It is a macro line */
	if(is_macro){
		int value;
		label = get_macro_label(transition);
		get_next_number(transition, &value, &is_macro);
		p_symbol = create_symbol(label, value, false, false, true);
		if (p_symbol != NULL) {
				add_symbol_to_list(p_symbol);
			} else {
				transition->is_runtimer_error = true;
				free(label);
				return;
			}
		return;	
	}
	if (is_symbol) {
		/* Check if symbol exists in symbol table*/
		symbol_node_ptr p_searched_symbol = search_symbol(label);
		/* If symbol does not exist in symbol table, create it and add it */
		if (p_searched_symbol == NULL) {
				if((strcmp(data_type, STRING_OPERATION) == 0)){
					p_symbol = create_symbol(label, transition->DC++, false, true, false);
				}else{
					p_symbol = create_symbol(label, transition->DC, false, true, false);
				}
			if (p_symbol != NULL) {
				add_symbol_to_list(p_symbol);
			} else {
				transition->is_runtimer_error = true;
				free(label);
				return;
			}

		} else {
			print_compiler_error("Each label can be defined only once", transition->current_line_information);
			transition->is_compiler_error = true;
			return;
		}
	}

	skip_all_spaces(transition->current_line_information);

	/*
	 * Extract data according to type
	 */
	if (is_end_of_data_in_line(transition->current_line_information)) {
		print_compiler_error("Invalid data line", transition->current_line_information);
		transition->is_compiler_error = true;
	}
	/* This is a string initialization */
	else if (strcmp(data_type, STRING_OPERATION) == 0) {
		process_string(transition);
	}
	/* This is a numeric data */
	else {
		process_numbers(transition);
	}
}

/**
 * @brief       Adds a string to the data list
 * @param     	A character of the string
 * @return 		Was the add successful
 */
bool add_string_data_to_list(char data, unsigned int address) {
	data_node_ptr p_data = (data_node_ptr)allocate_memory(sizeof(node_data));

	/* if we managed to create a new node, insert values and add to list */
	if (p_data != NULL) {
		p_data->current_data.encoded_data.bits.number = data;
		p_data->current_data.encoded_data.bits.rest = EMPTY_ADDRESS;
		p_data->current_data.address = address;
		p_data->next = NULL;

		add_data_node_to_list(p_data);

		return true;
	} else {
		return false;
	}
}

/**
 * @brief       Processes for string data
 * @param     	Line information
 */
void process_string(iteration_data* transition) {
	/* Is it a valid string line" */
	if (transition->current_line_information->line_str[transition->current_line_information->current_index] != QUOTATION) {
		print_compiler_error("A string must start with a '\"' token", transition->current_line_information);
		transition->is_compiler_error = true;
	} else {
		bool success;

		/* Skip quotation mark */
		transition->current_line_information->current_index++;

		/* Keep scanning till end of line */
		while (!is_end_of_line(transition->current_line_information)) {
			char token = transition->current_line_information->line_str[transition->current_line_information->current_index];
		
			if (token == NEXT_LINE_SYMBOL) {
				print_compiler_error("A string must end with a '\"' token", transition->current_line_information);
				transition->is_compiler_error = true;
				break;
			} else if (token == QUOTATION) {
				transition->current_line_information->current_index++;
				break;
			} else if (token != QUOTATION) {
				success = add_string_data_to_list(token, transition->DC);

				if (!success) {
					transition->is_runtimer_error = true;
					return;
				} else {
					transition->DC++;
				}
			}

			transition->current_line_information->current_index++;
		}
		/* Add string to list or die */
		
		success = add_string_data_to_list(STRING_DATA_END, transition->DC);
		if (!success) {
			transition->is_runtimer_error = true;
			return;
		}


		if (!is_end_of_data_in_line(transition->current_line_information)) {
			print_compiler_error("Invalid tokens after .string instruction", transition->current_line_information);
			transition->is_compiler_error = true;
		}
	}
}

/**
 * @brief       Adds a numeric data to the list
 * @param     	1. A number
 *              2. Address
 *              3. Index in array
 * @return 		Was the add successful
 */
bool add_numeric_data_to_list(int number, unsigned int address, unsigned int index, bool is_macro) {
	data_node_ptr p_data = (data_node_ptr)allocate_memory(sizeof(node_data));

	/* if we managed to create a new node, insert values and add to list */
	if (p_data != NULL) {
		p_data->current_data.encoded_data.bits.number = number;
		p_data->current_data.encoded_data.bits.rest = EMPTY_ADDRESS;
		p_data->current_data.address = address;
		p_data->current_data.index = index;
		p_data->current_data.is_macro = is_macro;
		p_data->next = NULL;
		add_data_node_to_list(p_data);

		return true;
	} else {
		return false;
	}
}

/**
 * @brief       Processes numbers definition
 * @param     	Current transition data
 */
void process_numbers(iteration_data* transition) {
	bool should_process_next_number = true;
	bool success;
	bool is_macro = false;
	unsigned int index = 0;

	skip_all_spaces(transition->current_line_information);

	/* Make sure .data contains numbers */
	if (is_end_of_data_in_line(transition->current_line_information)) {
		print_compiler_error("Invalid .data definition. Missing numbers.", transition->current_line_information);
		transition->is_compiler_error = true;
	}

	/* Processes all numbers in line */
	while (!is_end_of_line(transition->current_line_information) && should_process_next_number) {
		char* partial_line = NULL;
		int number;
		
		if (get_next_number(transition, &number, &is_macro)) {
				success = add_numeric_data_to_list(number, transition->DC++,index, is_macro);
			
			if (!success) {
				transition->is_runtimer_error = true;
				return;
			}

			/* Search the next ',' */
			partial_line = strchr(transition->current_line_information->line_str + transition->current_line_information->current_index, NUMBER_TOKEN_SEPERATOR);

			/* Found ',' and scan another number */
			if (partial_line != NULL) {
				transition->current_line_information->current_index = partial_line - transition->current_line_information->line_str + 1;
				should_process_next_number = true;
				index++;
			} else {
				skip_all_spaces(transition->current_line_information);
				should_process_next_number = false;
			}
		} else {
			should_process_next_number = false;
		}
	}
	
	/* If it's not end of line or we need to get another number, throw an error */
	if (!is_end_of_line(transition->current_line_information) || should_process_next_number) {
		print_compiler_error(".data syntax is invalid", transition->current_line_information);
		transition->is_compiler_error = true;
	}
}

/**
 * @brief       Gets the next number from the .data line
 * @param     	1. Current transition data
 * 				2. Pointer to the resulted number
 * 				3. Is macro
 * @return 		bool - Is number valid
 */
bool get_next_number(iteration_data* transition, int* number, bool* is_macro) {
	char* temp_macro;
	skip_all_spaces(transition->current_line_information);

	/* Reaches end of line while expecting a number */
	if (is_end_of_line(transition->current_line_information)) {
		print_compiler_error("Expected number definition", transition->current_line_information);
		transition->is_compiler_error = true;
	} else {
		int number_start_index = transition->current_line_information->current_index;
		int number_end_index = transition->current_line_information->current_index;

		/* First token of the number is a minus or a plus */
		if ((transition->current_line_information->line_str[number_start_index] == MINUS) ||
				transition->current_line_information->line_str[number_start_index] == PLUS) {
			number_end_index++;
		}
		/* Makes sure the next token is a digit */
		if (!is_end_of_line(transition->current_line_information) &&
				!isdigit(transition->current_line_information->line_str[number_end_index])) {
					int i;
					symbol_node_ptr p_searched_symbol;
					temp_macro = (char*)malloc(OPERATION_LINE_MAX_LENGTH);
					/* Check if this is a macro */	
					for(i=0;(transition->current_line_information->line_str[number_end_index]!=NUMBER_TOKEN_SEPERATOR) && 
					(transition->current_line_information->line_str[number_end_index]!=NEXT_LINE_SYMBOL) && 
					(transition->current_line_information->line_str[number_end_index]!=HEADER_SEPARATOR);i++,number_end_index++)	{
						temp_macro[i] = transition->current_line_information->line_str[number_end_index];
					}
					temp_macro[i] = END_OF_STRING;
					/* Check if this is a legal macro */
					p_searched_symbol = search_symbol(temp_macro);
					if(!p_searched_symbol){
						print_compiler_error("A number must have digits in it or macro value", transition->current_line_information);
						transition->is_compiler_error = true;
						return false;
					}else{
						transition->current_line_information->current_index = number_end_index + 1;
						/* Insert a macro number */
						*number = p_searched_symbol->current_symbol.address;
						*is_macro = false;
						free(temp_macro);
						return true;
					}
		} else if (!is_end_of_line(transition->current_line_information)){
			char* number_string = NULL;
			/* Finds all digits */
			while (isdigit(transition->current_line_information->line_str[number_end_index])) {
				number_end_index++;
			}

			/* Last token wasn't part of the number */
			number_end_index--;

			/* Copying the number from the line into a new memory space, or die */
			number_string = allocate_string(number_end_index - number_start_index + 1);
			if (!number_string) {
				transition->is_runtimer_error = true;
				return false;
			}

			strncpy(number_string, transition->current_line_information->line_str + number_start_index, number_end_index - number_start_index + 1);
			number_string[number_end_index - number_start_index + 1] = END_OF_STRING;
			*number = atoi(number_string);

			free(number_string);

			transition->current_line_information->current_index = number_end_index + 1;

			return true;
		}
	}
	return false;
}

/**
 * @brief       Read next label from line
 * @param     	Current transition data
 * @return 		Extracted label
 */
char* get_macro_label(iteration_data* transition)
{
	char* label;
	int i, label_end_index, label_start_index, label_length, symbol_end_index;

	skip_all_spaces(transition->current_line_information);

	label_end_index  = label_start_index = i = transition->current_line_information->current_index;

	/* Find label position */
	for (;i < transition->current_line_information->line_length; i++) {
		if ((!isspace(transition->current_line_information->line_str[i])) && ((transition->current_line_information->line_str[i])!=DEFINE_SYMBOL)) {
			label_end_index = i;
		} else {
			break;
		}
	}

	label_length = label_end_index - label_start_index + 1;

	label = allocate_string(label_length);

	if (label == NULL) {
		transition->is_runtimer_error = true;
	} else {
		/* Copy label */
		strncpy(label, transition->current_line_information->line_str + label_start_index, label_length);
		(label)[label_length] = END_OF_STRING;

		transition->current_line_information->current_index = label_end_index + 1;
	}
	/* Skip '=' */
	if (is_end_of_data_in_line(transition->current_line_information)) {
		print_compiler_error("Missing data after .define", transition->current_line_information);
		transition->is_compiler_error = true;
	}
	else{
		for (;i < transition->current_line_information->line_length; i++) {
		if ((transition->current_line_information->line_str[i])!=DEFINE_SYMBOL) {
			symbol_end_index = i;
		} else {
			break;
		}
	}
	transition->current_line_information->current_index = symbol_end_index + 2;
	}
	return label;
}

/**
 * @brief       Writes all data definitions into an output file
 * @param     	Output file
 */
void write_data_to_output_file(FILE* output_file) {
	data_node_ptr p_current_data = p_data_head;

	while (p_current_data != NULL) {
		definition_data data = p_current_data->current_data;
		print_encoding_to_file(data.address, data.encoded_data.value, output_file);
		p_current_data = p_current_data->next;
	}
}

/**
 * @brief       Updates data addresses according to code block size
 * @param     	Code block size
 */
void data_address_reloaded(int ic_length) {
	data_node_ptr p_current = p_data_head;

	while (p_current != NULL ) {
		if(!(p_current->current_data.is_macro)){
			p_current->current_data.address += ic_length + START_ADDRESS;
		}
		p_current = p_current->next;
	}
}

/**
 * @brief       Adds new data found in code to the list
 * @param     	A new data node
 */
void add_data_node_to_list(data_node_ptr p_new_data) {
	/* This is the first data */
	if (p_data_head == NULL) {
		p_data_head = p_new_data;
		p_data_tail = p_new_data;
	} else {
		/* Inserts the data in the end */
		p_data_tail->next = p_new_data;
		p_data_tail = p_new_data;
		
	}
}

/**
 * @brief      Free memory list
 */
void free_data_node_list() {
	data_node_ptr p_cleaner_data = p_data_head;

	/* Clean nodes until no more nodes */
	while (p_data_head) {
		p_cleaner_data = p_data_head;
		p_data_head = p_data_head->next;
		free (p_cleaner_data);
	}
	return;
}