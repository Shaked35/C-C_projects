/**
 * @file 	Types.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant detypes for the project;
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <stdbool.h>
#include <stdio.h>
#include "Enums.h"
#include "Constans.h"


/*
 * A symbol in the code.
 * Can be an instruction or an operation.
 */
typedef struct
{
	char* name;
	int is_instruction;
	int is_external;
	unsigned int address;
	bool is_macro;
} symbol;

/* A symbol node in the symbol table */
typedef struct symbol_node* symbol_node_ptr;
typedef struct symbol_node {
	symbol current_symbol;
	symbol_node_ptr next;
} symbol_node;


/**
 * Data definition
 * A data is defined of a value (char or number) and an address
 */
typedef struct data {
	union {
		struct {
			unsigned int number : DATA_WORD_BITS_LENGTH;
			unsigned int rest : REST_BITS_LENGTH;
		} bits;
		unsigned int value;
	} encoded_data;
	unsigned int address;
	unsigned int index;
	bool is_macro;
} definition_data;

/**
 *  Data Table Node 
 * */
typedef struct node_data* data_node_ptr;
typedef struct node_data {
	definition_data current_data;
	data_node_ptr next;
} node_data;

/**
 *  Contains the definition of the machine operation as instructed in the manual 
 * */
typedef struct {
	char* name;
	unsigned int code;
	unsigned int operands_number;
} machine_operation_definition;

/**
 *  Machine operation definitions node 
 * */
typedef struct operation_node* operation_information_node_ptr;
typedef struct operation_node {
	machine_operation_definition data;
	operation_information_node_ptr next;
} operation_node_data;

/**
 *  Definition of a machine operation 
 * */
typedef union {
	struct {
		unsigned int era : OPERATION_ERA_BITS_LENGTH;
		unsigned int target_operand_address_method : OPERATION_ADDRESS_METHOD_BITS_LENGTH;
		unsigned int source_operand_address_method : OPERATION_ADDRESS_METHOD_BITS_LENGTH;
		unsigned int op_code : OPERATION_OP_CODE_BITS_LENGTH;
		unsigned int rest : REST_BITS_LENGTH;
	} bits;
	unsigned int value;
} encoded_operation;


/**
 *  Contains data of decoded operation 
 * */
typedef struct {
	machine_operation_definition* operation;
	int times;
	char* source_operand;
	char* target_operand;
	METHOD_ID source_operand_address_method;
	METHOD_ID target_operand_address_method;
} decoded_operation;

/**
 *  Presentation of operand memory word
 * */
typedef union {
	struct {
		unsigned int era : OPERATION_ERA_BITS_LENGTH;
		unsigned int target_register_address : OPERAND_REGISTER_BITS_LENGTH;
		unsigned int source_register_address : OPERAND_REGISTER_BITS_LENGTH;
		unsigned int rest : REST_BITS_LENGTH;
	} register_address;
	struct {
		unsigned int era: OPERATION_ERA_BITS_LENGTH;
		unsigned int operand_address : OPERAND_NON_REGISTER_BITS_LENGTH;
		unsigned int rest : REST_BITS_LENGTH;
	} non_register_address;
	unsigned int value;
} operand_memory_word;

/**
 *  Contains output files 
 * */
typedef struct {
	FILE* extern_file;
	FILE* entry_file;
	FILE* ob_file;
} compiler_output_files;


/**
 *  Contains data for each line 
 * */
typedef struct line_data {
	char* line_str;
	int line_length;
	int current_index;
	int line_number;
	char* file_name;
} line_data;


/**
 *  Contains iteration data 
 * */
typedef struct {
	char* prev_operation_operand;
	METHOD_ID prev_operand_address_method;
	unsigned int IC;
	unsigned int DC;
	bool is_compiler_error;
	bool is_runtimer_error;
	line_data* current_line_information;
} iteration_data;

#endif /* TYPES_H_ */
