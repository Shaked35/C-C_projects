/**
 * @file 	Extern.c
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   This file Contains methods relevant to .extern line processing;
 */

#include <stdlib.h>
#include <stdio.h>
#include "DocumentUtilities.h"
#include "SymbolTable.h"
#include "Extern.h"
#include "Types.h"
#include "Constans.h"

/**
 * @brief       First iteration process on extern 
 * @param     	1. Current transition
 */
void first_iteration_process_extern(iteration_data* transition) {
	symbol_node_ptr p_symbol = NULL;

	char* extern_name = get_next_word(transition);

	/* If we have an extern*/
	if (extern_name) {
		symbol_node_ptr p_searched_symbol = search_symbol(extern_name);

		/* Extern does not exists in symbol table */
		if (!p_searched_symbol) {
			/* Create new symbol in table or die */
			
			p_symbol = create_symbol(extern_name, EMPTY_ADDRESS, true, true, false);
			if (p_symbol) {
				add_symbol_to_list(p_symbol);

				/* Make sure that the line does not contain left overs */
				if (!is_end_of_data_in_line(transition->current_line_information)) {
					print_compiler_error("Invalid tokens after extern definition", transition->current_line_information);
					transition->is_compiler_error = true;
					return;
				}
			}
			/* Could not allocat memory, die*/
			else {
				transition->is_runtimer_error = true;
				free(extern_name);
				return;
			}
		}
		/* Extern is already in table */
		else {
			print_compiler_error("Each extern can be defined only once", transition->current_line_information);
			transition->is_compiler_error = true;
			return;
		}
	}
}

/**
 * @brief       Write the extern into the extern output file
 * @param     	1. Extern name
 * 				2. Extern usage address
 * 				3. Output file
 */
void write_extern_to_output_file(char* extern_operand, unsigned int current_address, FILE* output_file) {
	char* output;

	/* Add current extern to output file */
	fputs(extern_operand, output_file);
	fputc(COLUMN_SEPARATOR, output_file);

	/* Add current entry address to output file */
	output = convert_base10_to_target_base(current_address, BASE_10, TARGET_MEMORY_ADDRESS_WORD_LENGTH);
	fputs(output, output_file);
	free(output);

	/* New line */
	fputc(NEXT_LINE_SYMBOL, output_file);
}

/**
 * @brief       Creates the entry file if it doesn't exists
 * @param     	1. Output files
 * 				2. File name without extension
 */
void create_extern_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension) {
	if (output_files->extern_file == NULL) {
		output_files->extern_file = create_output_file(file_name_without_extension, EXTERN_FILE_EXT);
	}
}



