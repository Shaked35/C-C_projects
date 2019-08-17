/**
 * @file 	FirstIteration.c 
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   This file contains all of the process of the first Iteration run;
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "Constans.h"
#include "Data.h"
#include "Utilities.h"
#include "DocumentUtilities.h"
#include "Entry.h"
#include "Enums.h"
#include "Extern.h"
#include "FirstIteration.h"
#include "Types.h"
#include "SymbolTable.h"


/* Global variables */
METHOD_ID last_operand_method;
bool can_use_copy_previous = false;
/**
 * @brief       Processes a line according to its' type
 * @param     	First Transition information
 */
void first_iteration_process_line(iteration_data* transition) {
	char* label = NULL;
	char* line_type = NULL;
	bool is_symbol = false;
	char* partial_line;
	skip_all_spaces(transition->current_line_information);

	/* Find label or macro */
	if ((partial_line = strchr(transition->current_line_information->line_str, LABEL_END_TOKEN)) != NULL) {

		/* Find label position */
		int start_label = transition->current_line_information->current_index;
		int label_length = partial_line - transition->current_line_information->line_str - start_label;
		label = allocate_string(label_length);
		strncpy(label, transition->current_line_information->line_str + start_label, label_length);
		label[label_length] = END_OF_STRING;

		/* The label is valid */
		if (is_valid_label(label)) {
			is_symbol = true;
			transition->current_line_information->current_index += label_length + 1;
		} else {
			print_compiler_error("Invalid label definition", transition->current_line_information);
			transition->is_compiler_error = true;
			return;
		}
	}

	/* Read line type */
	line_type = get_next_word(transition);

	/* Handle line type */
	if (line_type == NULL) {
		print_compiler_error("Invalid line", transition->current_line_information);
		transition->is_compiler_error = true;
	}
	/* Macro type */
	else if (strcmp(line_type, DEFINE_OPERATION) == 0) {
		first_iteration_process_data(transition, label, line_type, is_symbol,true);
	}
	/* Extern type */
	else if (strcmp(line_type, EXTERN_OPERATION) == 0) {
		first_iteration_process_extern(transition);
	}
	/* Entry type */
	else if (strcmp(line_type, ENTRY_OPERATION) == 0) {
		first_iteration_process_entry(transition);
	}
	/* Data type process*/
	else if ((strcmp(line_type, DATA_OPERATION) == 0) || (strcmp(line_type, STRING_OPERATION) == 0)) {
		first_iteration_process_data(transition, label, line_type, is_symbol,false);
	}
	/* Operation type */
	else  {
		transition->current_line_information->current_index -= strlen(line_type);
		first_iteration_process_operation(transition, label, is_symbol);
	}

	if (line_type != NULL) {
		free(line_type);
	}
}

/**
 * @brief       Executes the first Iteration of the assembly compiler
 * @param     	1. Input file
 * 				2. File name
 * @return 		true if iteration process successed
 */
bool first_iteration_execute(FILE* assembler_input_file, char* file_name_without_extension, unsigned int* IC, unsigned int* DC) {
	/* Creates the first iteration data */
	iteration_data* transition = create_transition_data();
	int line_number = 0;
	bool success = true;

	if (transition == NULL) {
		return false;
	}

	/* IC-DC: Initialization */
	transition->IC = EMPTY_ADDRESS;
	transition->DC = EMPTY_ADDRESS;

	/* Runs until end of file */
	while (!feof(assembler_input_file) && !transition->is_runtimer_error) {
		char line[MAX_LENGTH];

		line_number++;

		/* Reads new line  */
		if (fgets(line, MAX_LENGTH + 1, assembler_input_file)) {

			/* This isn't an empty line or a comment */
			if (!is_empty_or_comment(line)) {
				line_data* info = create_line_info(file_name_without_extension, line_number, line);

				/* Process the line */
				if (info != NULL) {
					transition->current_line_information = info;

					first_iteration_process_line(transition);
					success &= !(transition->is_compiler_error);

					free(info);
				} else {
					transition->is_runtimer_error = true;
				}
			}
		}
	}

	/* Transition succeed */
	if (!transition->is_compiler_error && !transition->is_runtimer_error) {
		/* Update the data address according to the code length */
		data_address_reloaded(transition->IC);
		symbol_address_reloaded(transition->IC);

		*IC = transition->IC;
		*DC = transition->DC;
		/*IC-DC final logs */
		printf("IC: %d, DC: %d\n",transition->IC,transition->DC);
	}

	if (transition->prev_operation_operand != NULL) {
		free(transition->prev_operation_operand);
	}

	free(transition);

	return success & !transition->is_runtimer_error;
}


