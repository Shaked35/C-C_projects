/**
 * @file 	Utilities.c
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   This file Contains methods relevant to the operation line processing;
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Types.h"
#include "DocumentUtilities.h"
#include "Constans.h"
#include "Utilities.h"
#include "SymbolTable.h"
#include "Extern.h"


/* Global variables */
operation_information_node_ptr p_operation_head = NULL;

/**
 * @brief       First iteration process on operation 
 * @param      	1. Current transition data
 * 				2. Label value (NULL if doesn't exist)
 * 				3. Does label exist
 */
void first_iteration_process_operation(iteration_data* transition, char* label, bool is_symbol) {
	decoded_operation* p_decoded_operation = NULL;

	/* Checks if the line is valid for an operation */
	if (!is_valid_is_operation_line(transition->current_line_information)) {
		print_compiler_error("Line exceeds max length", transition->current_line_information);
		transition->is_compiler_error = true;
		return;
	}

	/* Check if there is a symbol in the beginning of line */
	if (is_symbol) {
		/* Checks if the symbol was defined in previous lines */
		symbol_node_ptr p_searched_symbol = search_symbol(label);

		if (p_searched_symbol == NULL) {

			/* Creates a new symbol and adds it to the symbol table, or die */
			symbol_node_ptr p_symbol = create_symbol(label, transition->IC, false, false, false);
			if (p_symbol != NULL) {
				add_symbol_to_list(p_symbol);
			} else {
				transition->is_runtimer_error = true;
				free(label);
				return;
			}
		} else {
			print_compiler_error("Each label/macro can be defined only once", transition->current_line_information);
			transition->is_compiler_error = true;
			free(label);
			return;
		}
	}

	/* Gets all data about the current operation */
	p_decoded_operation = get_decoded_operation(transition);

	if (p_decoded_operation != NULL) {
		/* Checks if the operands used in the operation are authorized */
		if (is_valid_operand_method(p_decoded_operation)) {

			/* Calculate the operation's size */
			transition->IC += words_size_calculation(transition, p_decoded_operation);
			/* Updates the transition with the source operand of the current operation */
			update_transition_with_last_operation(transition, p_decoded_operation);

		} else {
			print_compiler_error("Unauthorized address methods", transition->current_line_information);
			transition->is_compiler_error = true;
		}
	}
}

/**
 * @brief       Second iteration process on operation 
 * @param     	1. Current transition data
 * 				2. Output files
 */
void second_iteration_process_operation(iteration_data* transition, compiler_output_files* p_output_files) {
	/* Gets all data about the current operation */
	decoded_operation* p_decoded_operation = get_decoded_operation(transition);

	/* Encode the operation */
	operands_final_process(transition, p_decoded_operation, p_output_files);

	/* Updates the transition with the source operand of the current operation */
	update_transition_with_last_operation(transition, p_decoded_operation);
}


/**
 * @brief       Updates the transition with the first operand of the operation
 * @param     	1. Current transition data
 * 				2. Current decoded operation
 */
void update_transition_with_last_operation(iteration_data* transition, decoded_operation* decoded_operation) {

	if (decoded_operation->source_operand != NULL) {
		/* Updates the operand for copy-previous with the source operand of the current operation */
		replace_content(&transition->prev_operation_operand, decoded_operation->source_operand);
		transition->prev_operand_address_method = decoded_operation->source_operand_address_method;
	} else if (decoded_operation->target_operand != NULL) {
		/* Updates the operand for copy-previous with the target operand of the current operation */
		replace_content(&transition->prev_operation_operand, decoded_operation->target_operand);
		transition->prev_operand_address_method = decoded_operation->target_operand_address_method;
	} else {
		/*
		 * The current operation has no operands, therefore we cannot use copy previous in the
		 * next operation
		 */
		if (transition->prev_operation_operand != NULL) {
			free(transition->prev_operation_operand);
			transition->prev_operation_operand = NULL;
		}
	}
}

/**
 * @brief       Calculates how many memory words are used to encode the operation
 * @param     	1. Current transition data
 * 				2. Decoded operation
 * @return 		How many memory words should be used to encode the operation
 */
int words_size_calculation(iteration_data* transition, decoded_operation* current_operation) {
	/* We need to encode the operation */
	int size = OPERAION_MIN_WORD_SIZE;
	bool other_size = false;

	/* If both operands are registers they share the same memory word */
	if((current_operation->source_operand_address_method == DIRECT_REGISTER) &&
			(current_operation->target_operand_address_method == DIRECT_REGISTER)){
				size++;
				other_size = true;
			}
	/* If source_operand is a Index method */		
	if(current_operation->source_operand_address_method == INDEX_METHOD){
		/* If target is a register */	
		if(current_operation->target_operand_address_method == DIRECT_REGISTER){
			size++;
		}
		size+=2;
		other_size = true;
	}
	/* If target_operand is a Index method */
	if(current_operation->target_operand_address_method == INDEX_METHOD){
		/* If source is a register */	
		if(current_operation->source_operand_address_method == DIRECT_REGISTER){
			size++;
		}
		size+=2;
		other_size = true;
	}
	if(!other_size) {
		/* Each operand needs its own memory word */
		size += current_operation->operation->operands_number;
	}
	return size;
}

/**
 * @brief       Get all data of operation in line
 * @param     	Line information
 * @return 		Operation data
 */
decoded_operation* get_decoded_operation(iteration_data* transition) {
	decoded_operation* current_operation = NULL;
	machine_operation_definition* p_operation_information = NULL;

	/* Get operation name from line, or die */
	char* operation_name = get_operation_name(transition);
	if (transition->is_runtimer_error) {
		return NULL;
	}

	/* Find operation in machine operation list */
	p_operation_information = search_machine_operation_in_list(operation_name);

	/* The operation name isn't valid */
	if (p_operation_information == NULL) {
		print_compiler_error("Invalid operation name", transition->current_line_information);
		transition->is_compiler_error = true;
		return NULL;
	} else {
		char* source_operand = NULL;
		char* target_operand = NULL;

			/* Get the operands to encode in the operation */
		if (get_all_operands(transition, p_operation_information->operands_number, &source_operand, &target_operand)) {
			
			/* Gets the address methods to use in the encoding */
			METHOD_ID source_operand_address_method = get_address_method(transition, source_operand);
			METHOD_ID target_operand_address_method = get_address_method(transition, target_operand);

			/* No error occurred while trying to get address method */
			if ((source_operand_address_method != INVALID_ADDRESS_METHOD) && (target_operand_address_method != INVALID_ADDRESS_METHOD)) {
					current_operation = allocate_memory(sizeof(decoded_operation));
				if (current_operation == NULL) {
					transition->is_runtimer_error = true;
				} else {
					/* Initialize the operation according to data extracted from line */
					current_operation->operation = p_operation_information;
					current_operation->source_operand = source_operand;
					current_operation->source_operand_address_method = source_operand_address_method;
					current_operation->target_operand = target_operand;
					current_operation->target_operand_address_method = target_operand_address_method;
					current_operation->times = OPERATION_TIMES;
				}
				
			}
		
		}
	}

	return current_operation;
}


/**
 * @brief       Reads the operands from the line
 * @param     	1. Current transition data
 * 				2. Number of operands to read
 * 				3. Returned source operand
 * 				4. Returned target operand
 * @return 		Are operands valid
 */
bool get_all_operands(iteration_data* transition, int operands_count, char** source_operand, char** target_operand) {
	bool is_valid = true;

	switch (operands_count) {
		case NO_OPERANDS: {
			/* There aren't any operands */
			*source_operand = NULL;
			*target_operand = NULL;
			break;
		}
		case ONE_OPERAND: {
			/* Reads the target operand from the line */
			*source_operand = NULL;
			*target_operand = get_next_operand(transition);
			break;
		}
		case TWO_OPERANDS: {
			/* Reads the source operand from the line */
			*source_operand = get_next_operand(transition);

			if (transition->is_runtimer_error) {
				is_valid = false;
				break;
			}

			skip_all_spaces(transition->current_line_information);

			/* There must be a comma between operands */
			if (transition->current_line_information->line_str[transition->current_line_information->current_index] != OPERAND_SEPERATOR) {
				print_compiler_error("Missing comma between two operands", transition->current_line_information);
				transition->is_compiler_error = true;
				is_valid = false;
			} else {
				/* Reads the target operand from the line */
				transition->current_line_information->current_index++;
				*target_operand = get_next_operand(transition);
			}

			break;
		}
	}

	if (transition->is_runtimer_error) {
		is_valid = false;
	}
	/* Searches for tokens after operation */
	else if (is_valid && !is_end_of_data_in_line(transition->current_line_information)) {
		print_compiler_error("Ilegal operation line", transition->current_line_information);
		transition->is_compiler_error = true;
		is_valid = false;
	}

	/* The operands aren't valid. Releases memory allocated */
	if (!is_valid) {
		if (*source_operand != NULL) {
			free(*source_operand);
		}

		if (*target_operand != NULL) {
			free(*target_operand);
		}
	}

	return is_valid;
}

/**
 * @brief       Gets the operation name from the line
 * @param     	Line information
 * @return 		Operation name
 */
char* get_operation_name(iteration_data* transition) {
	char* result_operation_name;
	char* operation_name;
	int operation_name_length = 0;

	skip_all_spaces(transition->current_line_information);

	operation_name = transition->current_line_information->line_str + transition->current_line_information->current_index;

	/* Searches for the position of the operation inside the line */
	while (isalpha(transition->current_line_information->line_str[transition->current_line_information->current_index])) {
		(transition->current_line_information->current_index)++;
		operation_name_length++;
	}

	result_operation_name = allocate_string(operation_name_length);
	
	if (result_operation_name != NULL) {
		/* Copies the operation's name */
		strncpy(result_operation_name, operation_name, operation_name_length);
		result_operation_name[operation_name_length] = END_OF_STRING;
	} else {
		transition->is_runtimer_error = true;
	}

	return result_operation_name;
}

/**
 * @brief       Gets the address method according to the operand
 * @param     	1. Current transition
 * 				2. Current operand
 * @return 		Address method
 */
METHOD_ID get_address_method(iteration_data* transition, char* operand) {
	/* The operation has no source or target operand. Use IMMEDIATE method for encoding */
	char* temp_macro;
	if (operand == NULL) {
		return IMMEDIATE;
	} else {
		int operand_length = strlen(operand);
		if (operand_length > 0) {
			/* Operand starts with a # */
			if (operand[0] == IMMEDIATE_TOKEN) {
				int i = 1;

				if ((operand[1] == MINUS) || (operand[1] == PLUS)) {
					i++;

					if (operand_length == 1) {
						
						print_compiler_error("Immediate token # must be followed by a valid number", transition->current_line_information);
						transition->is_compiler_error = true;
						return INVALID_ADDRESS_METHOD;
					}
				}

				/* Check all operand's tokens are valid digits */
				for (; i < operand_length; i++) {
					if (!isdigit(operand[i])) {
						int j = 0;
						temp_macro = (char*)malloc(sizeof(char)*(operand_length));
						for(;i<operand_length; i++, j++){
							temp_macro[j] = operand[i];
						}
						/* Check if we already have this macro in the symbol table */
						if(!search_symbol(temp_macro)){
							print_compiler_error("Immediate token # must be followed by a valid number", transition->current_line_information);
							transition->is_compiler_error = true;
							return INVALID_ADDRESS_METHOD;
						}else
							break;	
					}
				}

				return IMMEDIATE;
			}
			/* The last length in the operand is equal to ']' */
			else if (operand[operand_length-1] == INDEX_METHOD_STR_END) {
				
				/* Check if this is a legal Index Method */
				int i = 0;
				/*Find the start index between [] */
				for (; i < operand_length-1 && operand[i] != INDEX_METHOD_STR_START; i++) {
				}
				/* If before the digit we don't have [ */
				if(operand[i] != INDEX_METHOD_STR_START){
					print_compiler_error("Ilegal Index Method line ", transition->current_line_information);
					transition->is_compiler_error = true;
					return INVALID_ADDRESS_METHOD;
				}
				i++;
				skip_all_spaces(transition->current_line_information);
				for(;operand[i] != INDEX_METHOD_STR_END;i++){
					if (!isdigit(operand[i])) {
						int j = 0;
						temp_macro = (char*)malloc(sizeof(char)*(operand_length));
						for(;operand[i] != INDEX_METHOD_STR_END ; i++, j++){
							temp_macro[j] = operand[i];
						}
						/* Check if we already have this macro in the symbol table & it's not a digit*/
						if(!search_symbol(temp_macro)){
							print_compiler_error("Index method must be followed by a valid number or macro", transition->current_line_information);
							transition->is_compiler_error = true;
							free(temp_macro);
							return INVALID_ADDRESS_METHOD;
						}else{
							free(temp_macro);
							break;
						}	
					}
				}

				return INDEX_METHOD;
			}
			/* The operand is legal register */
			else if (is_register(operand)) {
				return DIRECT_REGISTER;
			}
			/* The operand is a legal label */
			else if (is_valid_label(operand)) {
				return DIRECT;
			} else {
				print_compiler_error("Ilegal Operation line", transition->current_line_information);
				transition->is_compiler_error = true;
				return INVALID_ADDRESS_METHOD;
			}
		} else {
			print_compiler_error("Operation line cannot be empty", transition->current_line_information);
			transition->is_compiler_error = true;
			return INVALID_ADDRESS_METHOD;
		}
	}
}

/**
 * @brief       Checks whether the address methods used in the operation are authorized
 * @param     	Current decoded operation
 * @return 		Is authorized
 */
bool is_valid_operand_method(decoded_operation* current_operation) {
	/* This are invalid methods */
	if ((current_operation->source_operand_address_method == INVALID_ADDRESS_METHOD) ||
			(current_operation->target_operand_address_method == INVALID_ADDRESS_METHOD))
		return false;

	/*
	 * mov, add, and sub operation
	 * First operand: all methods are valid
	 * Second operand: only direct ,index and direct register are valid
	 */
	if ((strcmp(current_operation->operation->name, MOV) == 0) ||
		(strcmp(current_operation->operation->name, ADD) == 0) ||
		(strcmp(current_operation->operation->name, SUB) == 0)) {
		return ((current_operation->target_operand_address_method == DIRECT) ||
				(current_operation->target_operand_address_method == INDEX_METHOD) ||
				(current_operation->target_operand_address_method == DIRECT_REGISTER));
	}
	/*
	 * not, clr, inc, dec operation
	 * Only one operand.
	 * First operand: only direct ,index and direct register are valid
	 */
	else if ((strcmp(current_operation->operation->name, NOT) == 0) ||
			(strcmp(current_operation->operation->name, CLR) == 0) ||
			(strcmp(current_operation->operation->name, INC) == 0) ||
			(strcmp(current_operation->operation->name, DEC) == 0)) {
		return ((current_operation->target_operand_address_method == DIRECT) ||
				(current_operation->target_operand_address_method == INDEX_METHOD) ||
				(current_operation->target_operand_address_method == DIRECT_REGISTER));
	}
	/*
	 * lea operation
	 * First operand: only direct and index valid
	 * Second operand: only direct ,index and direct register are valid
	 */
	else if (strcmp(current_operation->operation->name, LEA) == 0) {
		return ((current_operation->source_operand_address_method == DIRECT)|| 
				(current_operation->target_operand_address_method == INDEX_METHOD)) &&
				((current_operation->target_operand_address_method == DIRECT) ||
				(current_operation->target_operand_address_method == DIRECT_REGISTER) ||
				(current_operation->target_operand_address_method == INDEX_METHOD));
	}
	/*
	 * jmp, bne, red operation
	 * Only one operand
	 * First operand: only direct and direct register are valid
	 */
	else if ((strcmp(current_operation->operation->name, JMP) == 0) ||
			   (strcmp(current_operation->operation->name, BNE) == 0) ||
			   (strcmp(current_operation->operation->name, RED) == 0)) {
		return ((current_operation->target_operand_address_method == DIRECT) || 
				(current_operation->target_operand_address_method == DIRECT_REGISTER));
	}
	/*
	 * jsr operation
	 * Only one operand
	 * First operand: only direct and direct register are valid
	 */
	else if (strcmp(current_operation->operation->name, JSR) == 0) {
		return ((current_operation->target_operand_address_method == DIRECT) || 
				(current_operation->target_operand_address_method == DIRECT_REGISTER));
	}
	/*
	 * cmp, prn, rts, stop operation
	 * The method is irrelevant or no operands given
	 */
	else {
		return true;
	}
}

/**
 * @brief       Encodes an operation into its output file
 * @param     	1. Current transition data
 * 				2. Decoded operation
 * 				3. Output files
 * @return 		Was the operation encoded successfully
 */
bool operands_final_process(iteration_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files) {
	encoded_operation coded_op;
	int i;

	/* Initlaizes the encoded operation with its' values */
	coded_op.bits.source_operand_address_method = p_decoded_operation->source_operand_address_method;
	coded_op.bits.target_operand_address_method = p_decoded_operation->target_operand_address_method;
	coded_op.bits.op_code = p_decoded_operation->operation->code;
	coded_op.bits.era = ABSOLUTE;
	coded_op.bits.rest = 0;

	/* Encode the operation */
	for (i = 1; i <= p_decoded_operation->times; i++) {
		print_encoding_to_file(transition->IC + START_ADDRESS, coded_op.value, output_files->ob_file);
		transition->IC++;

		if (p_decoded_operation->operation->operands_number > 0) {
			bool is_valid = encode_operands(transition, p_decoded_operation, output_files);

			if (!is_valid) {
				return is_valid;
			}
		}

	}

	return true;
}

/**
 * @brief       Encodes operands into output files
 * @param     	1. Current transition data
 * 				2. Decoded operation
 * 				3. Output files
 * @return 		Were operands encoded successfully
 */
bool encode_operands(iteration_data* transition, decoded_operation* p_decoded_operation, compiler_output_files* output_files) {
	bool is_valid;

	/* Both source and target operands are registers */
	if ((p_decoded_operation->source_operand_address_method == DIRECT_REGISTER) &&
			(p_decoded_operation->target_operand_address_method == DIRECT_REGISTER)) {
		is_valid = encode_registers(transition, p_decoded_operation->source_operand,
				p_decoded_operation->target_operand, output_files->ob_file);
		transition->IC++;
	} else {
		/* The operation has two operands */
		if(p_decoded_operation->source_operand)		
		if (p_decoded_operation->operation->operands_number == 2) {
			/* Encode the source operand */
			if (p_decoded_operation->source_operand_address_method == DIRECT) {
				is_valid = encode_direct(transition, p_decoded_operation->source_operand, output_files);
			} else if (p_decoded_operation->source_operand_address_method == INDEX_METHOD) {
				is_valid = encode_index(transition, p_decoded_operation->source_operand, output_files);
			} else if (p_decoded_operation->source_operand_address_method == DIRECT_REGISTER) {
				is_valid = encode_registers(transition, p_decoded_operation->source_operand, NULL, output_files->ob_file);
			} else if (p_decoded_operation->source_operand_address_method == IMMEDIATE) {
				is_valid = encode_immediate(transition, p_decoded_operation->source_operand, output_files->ob_file);
			} 

			transition->IC++;
		}

		/* Encode the target operand */
		if (p_decoded_operation->target_operand_address_method == DIRECT) {
			is_valid = encode_direct(transition, p_decoded_operation->target_operand, output_files);
		}else if (p_decoded_operation->target_operand_address_method == INDEX_METHOD) {
			is_valid = encode_index(transition, p_decoded_operation->target_operand, output_files);
		} else if (p_decoded_operation->target_operand_address_method == DIRECT_REGISTER) {
			is_valid = encode_registers(transition, NULL, p_decoded_operation->target_operand, output_files->ob_file);
		} else if (p_decoded_operation->target_operand_address_method == IMMEDIATE) {
			is_valid = encode_immediate(transition, p_decoded_operation->target_operand, output_files->ob_file);
		} 

		transition->IC++;
	}

	return is_valid;
}

/**
 * @brief       Encodes a direct operand
 * @param     	1. Current transition
 * 				2. Direct operand
 * 				3. Output files
 * @return 		Was operand encoded successfully
 */
bool encode_direct(iteration_data* transition, char* operand, compiler_output_files* output_files) {
	/* This a variable. Therefore it must be a symbol */
	symbol_node_ptr p_symbol = search_symbol(operand);
	if (p_symbol == NULL) {
		print_compiler_error("Using unknown symbol", transition->current_line_information);
		transition->is_compiler_error = true;
		return false;
	} else {
		operand_memory_word word;
		word.non_register_address.operand_address = p_symbol->current_symbol.address;
		/* The symbol is external. Encode the location it is used into the ext file */
		if (p_symbol->current_symbol.is_external) {
			word.non_register_address.era = EXTERNAL;

			create_extern_output_file_if_needed(output_files, transition->current_line_information->file_name);

			if (output_files->extern_file == NULL) {
				transition->is_runtimer_error  = true;
				return false;
			}
			
			write_extern_to_output_file(operand, transition->IC + START_ADDRESS, output_files->extern_file);
		} else {
			word.non_register_address.era = RELOCATABLE;
		}

		word.non_register_address.rest = 0;
		print_encoding_to_file(transition->IC + START_ADDRESS, word.value, output_files->ob_file);

		return true;
	}
}

/**
 * @brief       Encodes a index operand
 * @param     	1. Current transition
 * 				2. Index operand
 * 				3. Output files
 * @return 		Was operand encoded successfully
 */
bool encode_index(iteration_data* transition, char* operand, compiler_output_files* output_files) {
	/* This a variable. Therefore it must be a symbol */
	char* symbol = strtok(operand,"[");
	unsigned int index;
	char* temp_macro;
	symbol_node_ptr p_macro, p_symbol = search_symbol(symbol);
	temp_macro = strtok(NULL, "]");
	if (p_symbol == NULL) {
		print_compiler_error("Using unknown symbol", transition->current_line_information);
		transition->is_compiler_error = true;
		return false;
	} else {
		operand_memory_word word;
		/* Check if this is a macro */
		if(!isdigit(temp_macro[0])){
			p_macro = search_symbol(temp_macro);
			if(p_macro){
				index = p_macro->current_symbol.address;
			}
			else{
				print_compiler_error("Using unknown macro", transition->current_line_information);
				transition->is_compiler_error = true;
				return false;
			}
		/* This is a number */	
		}else{
			index = atoi(temp_macro);
		}
		/* operand_memory_word index_word;*/
		word.non_register_address.operand_address = p_symbol->current_symbol.address;
		index<<=2;
		/* The symbol is external. Encode the location it is used into the ext file */
		if (p_symbol->current_symbol.is_external) {
			word.non_register_address.era = EXTERNAL;

			create_extern_output_file_if_needed(output_files, transition->current_line_information->file_name);

			if (output_files->extern_file == NULL) {
				transition->is_runtimer_error  = true;
				return false;
			}
			write_extern_to_output_file(operand, transition->IC + START_ADDRESS, output_files->extern_file);
		} else {

			word.non_register_address.era = RELOCATABLE;
		}
		word.non_register_address.rest = 0;
		/*Address encode */
		print_encoding_to_file(transition->IC + START_ADDRESS, word.value, output_files->ob_file);
		transition->IC++;
		/*Index encode */
		print_encoding_to_file(transition->IC + START_ADDRESS, index, output_files->ob_file);
		return true;
	}
}
/**
 * @brief       Encodes register operands
 * @param     	1. Current transition
 * 				2. Source register
 * 				3. Target register
 * 				4. Output file
 * @return 		Were operands encoded successfully
 */
bool encode_registers(iteration_data* transition, char* source_register, char* target_register, FILE* p_file) {
	operand_memory_word word;

	/* The target register exists */
	if (source_register != NULL) {
		/* Encode the register's number */
		word.register_address.source_register_address = atoi(source_register + 1);
	} else {
		word.register_address.source_register_address = EMPTY_ADDRESS;
	}

	/* The target register exists */
	if (target_register != NULL) {
		/* Encode the register's number */
		word.register_address.target_register_address = atoi(target_register + 1);
	} else {
		word.register_address.target_register_address = EMPTY_ADDRESS;
	}

	word.register_address.era = ABSOLUTE;
	word.register_address.rest = 0;
	print_encoding_to_file(transition->IC + START_ADDRESS, word.value, p_file);

	return true;
}

/**
 * @brief       Encodes a immediate number
 * @param     	1. Current transition
 * 				2. Immediate operand
 * 				3. Output file
 * @return 		Was operand encoded successfully
 */
bool encode_immediate(iteration_data* transition, char* operand, FILE* p_file) {
	int number;
	char* temp_macro;
	operand_memory_word word;
	
	/* Check if this is a Macro */
	symbol_node_ptr p_searched_symbol;
	temp_macro = strtok(operand+1, " ");
	/* Is it a name of macro */ 
	p_searched_symbol = search_symbol(temp_macro);
	if(p_searched_symbol){
		number = p_searched_symbol->current_symbol.address;
	}
	/* Convert the number into a integer */
	else{
	number = atoi(operand + 1);
	}
	word.non_register_address.operand_address = number;
	word.non_register_address.era = ABSOLUTE;
	word.non_register_address.rest = 0;

	print_encoding_to_file(transition->IC + START_ADDRESS, word.value, p_file);

	return true;
}

/**
 * @brief       Searches for an operation name in a pre-defined list of valid operations
 * @param     	Operation name
 * @return 		If found returns machine operation information, otherwise NULL
 */
machine_operation_definition* search_machine_operation_in_list(char* operation) {
	operation_information_node_ptr p_current = p_operation_head;

	/* Scan the operation list and search for the operation */
	while (p_current != NULL) {
		if (strcmp(p_current->data.name, operation) == 0) {
			return &(p_current->data);
		}

		p_current = p_current->next;
	}

	return NULL;
}

/**
 * @brief       Initializes the operation list used for encoding
 * @return 		Did the initialization succeeded
 */
bool init_operation_list() {
	bool initialized = true;

	int op_code = 0;

	/* Initializes the operations */
	initialized &= add_operation_to_list(MOV, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(CMP, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(ADD, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(SUB, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(NOT, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(CLR, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(LEA, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(INC, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(DEC, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(JMP, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(BNE, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(RED, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(PRN, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(JSR, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(RTS, op_code++, NO_OPERANDS);
	initialized &= add_operation_to_list(STOP, op_code++, NO_OPERANDS);

	return initialized;
}

/**
 * @brief       Adds an operation into the list
 * @param     	1. Operation name
 * 				2. Op code
 * 				3. Number of operands
 * @return 		Did add successfully
 */
bool add_operation_to_list(char* name, unsigned int code, int operands) {
	bool added = false;

	operation_information_node_ptr p_new =
			(operation_information_node_ptr)allocate_memory(sizeof(operation_node_data));

	if (p_new != NULL) {
		p_new->data.name = name;
		p_new->data.code = code;
		p_new->data.operands_number = operands;
		p_new->next = NULL;

		/* first node input */
		if (p_operation_head == NULL) {
			p_operation_head = p_new;
		} else {
			p_new->next = p_operation_head;
			p_operation_head = p_new;
		}

		added = true;
	}

	return added;
}

/**
 * @brief     Free memory for operations list
 */
void free_operation_list() {
	operation_information_node_ptr p_cleaner_data = p_operation_head;

	/* Clean nodes until no more nodes */
	while (p_operation_head) {
		p_cleaner_data = p_operation_head;
		p_operation_head = p_operation_head->next;
		free (p_cleaner_data);
	}
	return;
}

/**
 * @brief       Reads the next operand from the line
 * @param     	Current transition
 * @return 		Operand
 */
char* get_next_operand(iteration_data* transition) {
	char* operand = NULL;
	int i, operand_end_index, operand_start_index, operand_length;

	skip_all_spaces(transition->current_line_information);

	operand_end_index  = operand_start_index = i = transition->current_line_information->current_index;

	/* Find operand position */
	for (;i < transition->current_line_information->line_length; i++) {
		if (!isspace(transition->current_line_information->line_str[i]) &&
				(transition->current_line_information->line_str[i] != OPERAND_SEPERATOR)) {
			operand_end_index = i;
		}
		else
		{
			break;
		}
	}

	operand_length = operand_end_index - operand_start_index + 1;

	operand = allocate_string(operand_length);

	if (operand == NULL) {
		transition->is_runtimer_error = true;
	} else {
		/* Copy operand */
		strncpy(operand, transition->current_line_information->line_str + operand_start_index, operand_length);
		operand[operand_length] = '\0';

		transition->current_line_information->current_index = operand_end_index + 1;
	}
	return operand;
}
