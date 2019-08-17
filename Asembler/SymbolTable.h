/**
 * @file 	SymbolTable.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant methods for the symbel process;
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_
#include <stdbool.h>
#include "Types.h"


/**
 * @brief       Adds a new symbol to the list
 * @param     	The new symbol
 */
void add_symbol_to_list(symbol_node_ptr p_new_symbol);

/**
 * @brief       Creates a new symbol node
 * @param     	1. Symbol name
 * 				2. IC address
 * 				3. Is external
 * 				4. Is instruction
 * @return  		New symbol node
 */
symbol_node_ptr create_symbol(char* name, int address, bool is_extern, bool is_instruction, bool is_global);

/**
 * @brief       Search input symbol in symbol table
 * @param     	Symbol name
 * @return 		Current symbol node
 */
symbol_node_ptr search_symbol(char* symbol_name);

/**
 * @brief       Updates symbol's address after first transition ends
 * @param     	IC value
 */
void symbol_address_reloaded(int ic_length);

/**
 * @brief       Free symbol list from memory
 */
void free_symbol_list();

#endif /* SYMBOLTABLE_H_ */
