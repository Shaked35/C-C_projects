/***********************************************************************
 * Asembler Project (C) 2019B by Shaked Ahronoviz                      *                        
 *                                                                     *    
 * This is the final project in the course: Laboratory in              *
 * Systems Programming.                                                *
 * This program is an Asembler for Assembly code that should           *
 * given in .as file by the client.                                    *
 *                                                                     *
 **********************************************************************/
/**
 * @file 	Asembler.c
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   This program will try to compile an assembler code file,
 * In case of errors in file it will print them out;
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DocumentUtilities.h"
#include "FirstIteration.h"
#include "SecondIteration.h"
#include "Constans.h"
#include "SymbolTable.h"
#include "Data.h"
#include "Utilities.h"


/** 
 * @brief  Cleaning memory method
*/
void memory_cleaner() {
	free_data_node_list();
	free_symbol_list();
	return;
}

/* Start compailer process */
int main(int argc, char* argv[]) {
	int i;
	char* curr_file;
	FILE* p_file;

	/* No File */
	if (argc == 1) {
		print_runtime_error("Please insert a file");
		exit(0);
	}
	
	/* Initialize operation list */
	if (!init_operation_list()) {
		free_operation_list();
		print_runtime_error("Initializes operation process failed");
		exit(0);
	}

	/* Asembler process for each as file */
	for (i=1; i < argc; i++) {
		unsigned int ic;
		unsigned int dc;

		/* Get file path */
		curr_file = allocate_string(strlen(argv[i])+strlen(FILE_EXT));
		strcpy (curr_file, argv[i]);
		strcat (curr_file, FILE_EXT);

		/* Try to open the file */
		p_file = fopen(curr_file, READ_ONLY_MODE);
		/* If this file isn't in the correct format print an error*/
		if (p_file == NULL) {
			print_runtime_error("Illegal as file");
		} else {
			/* Start first iteration */
			bool should_continue = first_iteration_execute(p_file, curr_file, &ic, &dc);
			/* Start the next iteration file */
			if (should_continue) {
				rewind(p_file);
				second_iteration_execute(p_file, argv[i], ic, dc);
			} 

			memory_cleaner();
			fclose(p_file);
		}

		free(curr_file);
	}
	free_operation_list();

	exit(0);
}
