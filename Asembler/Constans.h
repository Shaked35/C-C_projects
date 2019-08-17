/**
 * @file 	Constans.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant methods for the constans process;
 */

#ifndef CONSTS_H_
#define CONSTS_H_

#include <limits.h>

/* Output Assistant */
#define BASE_10 10
#define TARGET_BASE 4
#define TARGET_MEMORY_ADDRESS_WORD_LENGTH 4
#define TARGET_OPERATION_WORD_LENGTH 7
#define BASE_4_ZERO '0'
#define BASE_4_ONE '1'
#define BASE_4_TWO '2'
#define BASE_4_THREE '3'
#define SPECIAL_BASE_ZERO '*'
#define SPECIAL_BASE_ONE '#'
#define SPECIAL_BASE_TWO '%'
#define SPECIAL_BASE_THREE '!'

/* Output files separator */
#define COLUMN_SEPARATOR '\t'
#define HEADER_SEPARATOR ' '

/* Address definitions */
#define START_ADDRESS 100
#define EMPTY_ADDRESS 0

/* Label definitions */
#define LABEL_END_TOKEN ':'
#define LABEL_MAX_LENGTH 31
#define NEXT_LINE_SYMBOL '\n'
#define END_OF_STRING '\0'
#define DEFINE_SYMBOL  '='

/* A comment line */
#define COMMENT ';'

/* Data definition handling */
#define MINUS '-'
#define PLUS '+'
#define NUMBER_TOKEN_SEPERATOR ','
#define QUOTATION '\"'
#define STRING_DATA_END 0

/* Operations names */
#define MOV "mov"
#define CMP "cmp"
#define ADD "add"
#define SUB "sub"
#define NOT "not"
#define CLR "clr"
#define LEA "lea"
#define INC "inc"
#define DEC "dec"
#define JMP "jmp"
#define BNE "bne"
#define RED "red"
#define PRN "prn"
#define JSR "jsr"
#define RTS "rts"
#define STOP "stop"

/* Word bits sizes */
#define MEMORY_WORD 14
#define DATA_WORD_BITS_LENGTH MEMORY_WORD
#define OPERATION_OP_CODE_BITS_LENGTH 4
#define OPERATION_ADDRESS_METHOD_BITS_LENGTH 2
#define OPERATION_ERA_BITS_LENGTH 2
#define OPERAND_REGISTER_BITS_LENGTH 3
#define OPERAND_NON_REGISTER_BITS_LENGTH 12
#define REST_BITS_LENGTH 22
#define OPERATION_TIMES 1

/* Output files */
#define EXTERN_FILE_EXT ".ext"
#define CODE_FILE_EXT ".ob"
#define ENTRY_FILE_EXT ".ent"

/* Operation definitions */
#define MAX_LENGTH 100
#define OPERATION_LINE_MAX_LENGTH 80
#define OPERAND_SEPERATOR ','
#define IMMEDIATE_TOKEN '#'
#define INDEX_METHOD_STR_START '['
#define INDEX_METHOD_STR_END ']'
#define REGISTER_FIRST_TOKEN 'r'
#define REGISTERS_COUNT 8
#define OPERAION_MIN_WORD_SIZE 1
#define INVALID_OPEARTION_COUNTER -1

/* Behavior */
#define ENTRY_OPERATION ".entry"
#define STRING_OPERATION ".string"
#define DATA_OPERATION ".data"
#define EXTERN_OPERATION ".extern"
#define DEFINE_OPERATION ".define"

/* File Access Options */
#define READ_ONLY_MODE "r"
#define WRITE_MODE "w"

/* Input code file extension */
#define FILE_EXT ".as"

#endif /* CONSTS_H_ */
