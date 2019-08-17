/**
 * @file 	Enums.h
 * @author 	Shaked Ahronoviz 302352037;
 * @date    Semester 2019B; 
 * @project Laboratory in Systems Programming;
 * @brief   In this file we have all of the relevant enums for the project
 */

#ifndef ENUMS_H_
#define ENUMS_H_

/* Operands IDs */
typedef enum {
	NO_OPERANDS = 0,
	ONE_OPERAND = 1,
	TWO_OPERANDS = 2
} OPERANDS_COUNT;

/* Methods IDs */
typedef enum {
	INVALID_ADDRESS_METHOD = -1,
	IMMEDIATE = 0,
	DIRECT = 1,
	INDEX_METHOD = 2,
	DIRECT_REGISTER = 3
} METHOD_ID;

/* ERA options */
typedef enum {
	ABSOLUTE = 0,
	EXTERNAL = 1,
	RELOCATABLE = 2
} ERA;


#endif /* ENUMS_H_ */
