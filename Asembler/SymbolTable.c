/**
 * @file 	SymbolTable.c
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   This file Contains methods relevant to working with the symbol table;
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "DocumentUtilities.h"
#include "Types.h"
#include "Constans.h"


/* SymbolTable head and tail */
symbol_node_ptr p_symbol_head = NULL;
symbol_node_ptr p_symbol_tail = NULL;

/**
 * @brief       Adds a new symbol to the list
 * @param     	The new symbol
 */
void add_symbol_to_list(symbol_node_ptr p_new_symbol) {
	/* List is empty */
	if (p_symbol_head == NULL) {
		p_symbol_head = p_new_symbol;
		p_symbol_tail = p_new_symbol;
	} else {
		p_symbol_tail->next = p_new_symbol;
		p_symbol_tail = p_new_symbol;
	}
}

/**
 * @brief       Creates a new symbol node
 * @param     	1. Symbol name
 * 				2. IC address
 * 				3. Is external
 * 				4. Is instruction
 * @return  		New symbol node
 */
symbol_node_ptr create_symbol(char* name, unsigned int address, bool is_extern, bool is_instruction, bool is_global) {
	symbol_node_ptr p_node = (symbol_node_ptr)allocate_memory(sizeof(symbol_node));
	/* Copy data into node */
	if (p_node != NULL) {
		p_node->current_symbol.name = name;
		p_node->current_symbol.is_external = is_extern;
		p_node->current_symbol.is_instruction = is_instruction;
		p_node->current_symbol.is_macro= is_global;
		p_node->current_symbol.address = address;
		p_node->next = NULL;
		
	}

	return p_node;
}

/**
 * @brief       Search input symbol in symbol table
 * @param     	Symbol name
 * @return 		Current symbol node
 */
symbol_node_ptr search_symbol(char* symbol_name) {
	symbol_node_ptr p_current = p_symbol_head;

	/* Scan symbol table */
	while ((p_current != NULL) && (strcmp(p_current->current_symbol.name, symbol_name) != 0)) {
		p_current = p_current->next;
	}

	return p_current;
}

/**
 * @brief       Updates symbol's address after first transition ends
 * @param     	IC value
 */
void symbol_address_reloaded(int ic_length) {
	symbol_node_ptr p_current = p_symbol_head;
	while (p_current != NULL) {
		/* The symbol isn't external */
		if (!p_current->current_symbol.is_external && !(p_current->current_symbol.is_macro)) {
			/* The symbol is an instruction (.data or .string) */
			if (p_current->current_symbol.is_instruction) {
				p_current->current_symbol.address += ic_length + START_ADDRESS;
			} else {
				p_current->current_symbol.address += START_ADDRESS;
			}
		}

		p_current = p_current->next;
	}
}
	

/**
 * @brief       Free symbol list from memory
 */
void free_symbol_list() {
	symbol_node_ptr p_cleaner_data = p_symbol_head;

	/* Clean nodes until no more nodes */
	while (p_symbol_head) {
		p_cleaner_data = p_symbol_head;
		p_symbol_head = p_symbol_head->next;
		free (p_cleaner_data);
	}
	return;
}
