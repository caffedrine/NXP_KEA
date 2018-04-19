/*! @file Compiler.h
	@brief Compiler Specific Keyword Definition
*/

#ifndef _COMPILER_H
#define _COMPILER_H

/***
 * necessary for using within a C++ module
 */
#ifdef __cplusplus
extern "C" {
#endif


/**
@brief The compiler abstraction shall define the FUNC macro for the declaration and definition of
       functions, that ensures correct syntax of function declarations as required by a specific compiler.
@remarks Covers COMPILER001,COMPILER058
@remarks Implements DBASE03015
* @violates @ref Compiler_h_REF_1 A function should be used in
* preference to a function-like macro.
*/
#define FUNC(rettype, memclass) rettype

/**
@brief The compiler abstraction shall define the P2VAR macro for the declaration and definition of
       pointers in RAM, pointing to variables.
@remarks Covers COMPILER006
@remarks Implements DBASE03019
* @violates @ref Compiler_h_REF_1 A function should be used in
* preference to a function-like macro.
*/
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

/**
@brief The compiler abstraction shall define the P2CONST macro for the declaration and definition of
       pointers in RAM pointing to constants.
@remarks Covers COMPILER013
@remarks Implements DBASE03017
* @violates @ref Compiler_h_REF_1 A function should be used in
* preference to a function-like macro.
*/
#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype *

/**
@brief The compiler abstraction shall define the CONSTP2VAR macro for the declaration and definition
       of constant pointers accessing variables.
@remarks Covers COMPILER031
@remarks Implements DBASE03014
* @violates @ref Compiler_h_REF_1 A function should be used in
* preference to a function-like macro.
*/
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype * const

/**
@brief The compiler abstraction shall define the CONSTP2CONST macro for the declaration and
       definition of constant pointers accessing constants.
@remarks Covers COMPILER032
@remarks Implements DBASE03013
* @violates @ref Compiler_h_REF_1 A function should be used in
* preference to a function-like macro.
*/
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype * const

/**
@brief The compiler abstraction shall define the P2FUNC macro for the type definition of pointers
       to functions.
@remarks Covers COMPILER039
@remarks Implements DBASE03018
* @violates @ref Compiler_h_REF_1 A function should be used in
* preference to a function-like macro.
*/
#define P2FUNC(rettype, ptrclass, fctname) rettype (*fctname)

/**
@brief The compiler abstraction shall define the CONST macro for the declaration and definition of
       constants.
@remarks Covers COMPILER023
@remarks Implements DBASE03012
* @violates @ref Compiler_h_REF_1 A function should be used in
* preference to a function-like macro.
*/
#define CONST(consttype, memclass) const consttype

/**
@brief The compiler abstraction shall define the VAR macro for the declaration and definition of
       variables.
@remarks Covers COMPILER026
@remarks Implements DBASE03022
* @violates @ref Compiler_h_REF_1 A function should be used in
* preference to a function-like macro.
*/
#define VAR(vartype, memclass) vartype

#define __ramfunc	__attribute__((long_call, section(".data")))
#define _nop		__NOP

/***
 * necessary for using within a C++ module
 */
#ifdef __cplusplus
	}
#endif


#endif