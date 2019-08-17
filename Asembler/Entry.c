/**
 * @file 	Entry.c
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   This file Contains methods relevant to .entry line processing
 */

#include <stdlib.h>
#include <stdio.h>
#include "Constans.h"
#include "Entry.h"
#include "Types.h"
#include "SymbolTable.h"
#include "DocumentUtilities.h"


/**
 * @brief       First iteration process on entry 
 * @param     	Line information
 */
void first_iteration_process_entry(iteration_data* transition) {
	char* entry_name = get_next_word(transition);
	/* If entry was found*/
	if (entry_name != NULL) {
		/* Is entry valid? */
		if (is_valid_label(entry_name)) {
			/* Are we at line end?, if so, its an error, and if not, we are done */
			if (!is_end_of_data_in_line(transition->current_line_information)) {
				print_compiler_error("Invalid tokens in end of entry definition", transition->current_line_information);
				transition->is_compiler_error = true;
			}
		}
		/* Throw compiler error */
		else {
			print_compiler_error("Entry name must be a valid label", transition->current_line_information);
			transition->is_compiler_error = true;
		}
	}
	/* Throw runtime error */
	else if (!transition->is_runtimer_error) {
		print_compiler_error("Missing entry name", transition->current_line_information);
		transition->is_compiler_error = true;
	}
}

/**
 * @brief       Second iteration process on entry 
 * @param     	1. Line information
 * 				2. Output files
 */
void second_iteration_process_entry(iteration_data* transition, compiler_output_files* output_files) {
	char* entry_name = get_next_word(transition);
	
	/* Search for the entry inside the symbol table */
	symbol_node_ptr p_symbol = search_symbol(entry_name);
	/* Entry doesn't exists in symbol table*/
	if (p_symbol == NULL) {
		print_compiler_error("Invalid entry definition. Label doesn't exists.", transition->current_line_information);
		transition->is_compiler_error = true;
	} else {
		create_entry_output_file_if_needed(output_files, transition->current_line_information->file_name);

		if (output_files->entry_file == NULL) {
			transition->is_runtimer_error = true;
			return;
		}
		write_entry_to_output_file(entry_name, p_symbol->current_symbol.address, output_files->entry_file);
	}
}

/**
 * @brief       Write the entry into the entry output file
 * @param     	1. Entry name
 * 				2. Entry definition address
 * 				3. Output file
 */
void write_entry_to_output_file(char* entry_name, unsigned int address, FILE* output_file) {
	char* output;

	/* Add current entry to output file */
	fputs(entry_name, output_file);
	fputc(COLUMN_SEPARATOR, output_file);

	/* Add current entry address to output file */
	output = convert_base10_to_target_base(address, BASE_10, TARGET_MEMORY_ADDRESS_WORD_LENGTH);
	fputs(output, output_file);
	free(output);

	/* New file */
	fputc(NEXT_LINE_SYMBOL, output_file);
}

/**
 * @brief       Creates the entry file if it doesn't exists
 * @param     	1. Output files
 * 				2. File name without extension
 */
void create_entry_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension) {
	if (output_files->entry_file == NULL) {
		output_files->entry_file = create_output_file(file_name_without_extension, ENTRY_FILE_EXT);
	}
}
