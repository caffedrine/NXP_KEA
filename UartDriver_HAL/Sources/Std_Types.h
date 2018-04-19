/*! @file Std_Types.h
	@brief The data types definition.
*/

#ifndef _STD_TYPES_H
#define _STD_TYPES_H

/***************************************************************************//**
 * @addtogroup Std_Types
 * @brief The data type definition.
 * @{
*/

#include "stddef.h"
#include "Compiler.h"

/***
 * necessary for using within a C++ module
 */
#ifdef __cplusplus
extern "C" {
#endif

/*! \typedef signed char Int8
	\brief Signed 8-bit width data type definition.
*/
typedef signed char		Int8;

/*! \typedef unsigned char UInt8
	\brief Unsigned 8-bit width data type definition.
*/
typedef unsigned char	UInt8;

/*! \typedef signed short Int16
	\brief Signed 16-bit width data type definition.
*/
typedef signed short	Int16;

/*! \typedef unsigned short UInt16
	\brief Unsigned 16-bit width data type definition.
*/
typedef unsigned short	UInt16;

/*! \typedef signed long Int32
	\brief Signed 32-bit width data type definition.
*/
typedef signed long		Int32;

/*! \typedef unsigned long UInt32
	\brief Unsigned 32-bit width data type definition.
*/
typedef unsigned long	UInt32;

/*! \typedef signed long long Int64
	\brief Signed 64-bit width data type definition.
*/
typedef signed long	long Int64;

/*! \typedef unsigned long long UInt64
	\brief Unsigned 64-bit width data type definition.
*/
typedef unsigned long long UInt64;

/*! \typedef Int32 Std_SignedType
	\brief Signed standard data type definition.
*/
typedef Int32			Std_SignedType;

/*! \typedef UInt32 Std_UnsignedType
	\brief Unsigned standard data type definition.
*/
typedef UInt32			Std_UnsignedType;

/*! \typedef Int32 Std_ReturnType
	\brief Standard return data type definition.
*/
typedef Int32			Std_ReturnType;

/*! \typedef Int8 int8_t
	\brief Data type definition.
*/
typedef Int8	int8_t;

/*! \typedef UInt8 uint8_t
	\brief Data type definition.
*/
typedef UInt8	uint8_t;

/*! \typedef Int32 int32_t
	\brief Data type definition.
*/
typedef Int32	int32_t;

/*! \typedef UInt32 uint32_t
	\brief Data type definition.
*/
typedef UInt32	uint32_t;

/*! \typedef BOOL
	\brief The BOOL data type definition.
*/
typedef enum { 
	FALSE = 0,	/**< Defines the false code. */
	TRUE		/**< Defines the true code. */
} BOOL;

//typedef BOOL bool;

/*! \struct Int8Compl_DataType
	\brief The signed 8-bit width SIL data type definition.
*/
typedef struct {
	Int8 Value;			/**< Holds the value itself. */
	Int8 Complement;	/**< Holds the complement of the value. */
} Int8Compl_DataType;

/*! \struct UInt8Compl_DataType
	\brief The unsigned 8-bit width SIL data type definition.
*/
typedef struct {
	UInt8 Value;		/**< Holds the value itself. */
	UInt8 Complement;	/**< Holds the complement of the value. */
} UInt8Compl_DataType;

/*! \struct Int16Compl_DataType
	\brief The signed 16-bit width SIL data type definition.
*/
typedef struct {
	Int16 Value;		/**< Holds the value itself. */
	Int16 Complement;	/**< Holds the complement of the value. */
} Int16Compl_DataType;

/*! \struct UInt16Compl_DataType
	\brief The unsigned 16-bit width SIL data type definition.
*/
typedef struct {
	UInt16 Value;		/**< Holds the value itself. */
	UInt16 Complement;	/**< Holds the complement of the value. */
} UInt16Compl_DataType;

/*! \struct Int32Compl_DataType
	\brief The signed 32-bit width SIL data type definition.
*/
typedef struct {
	Int32 Value;		/**< Holds the value itself. */
	Int32 Complement;	/**< Holds the complement of the value. */
} Int32Compl_DataType;

/*! \struct UInt32Compl_DataType
	\brief The unsigned 32-bit width SIL data type definition.
*/
typedef struct {
	UInt32 Value;		/**< Holds the value itself. */
	UInt32 Complement;	/**< Holds the complement of the value. */
} UInt32Compl_DataType;

typedef UInt32Compl_DataType U32Sil_Type;

/*! \def Int8SIL_Write( Var, Val )
	\brief Macro to write a 8-bit width signed SIL variable.
	\def UInt8SIL_Write( Var, Val )
	\brief Macro to write a 8-bit width unsigned SIL variable.
	\def Int16SIL_Write( Var, Val )
	\brief Macro to write a SIL variable.
	\def UInt16SIL_Write( Var, Val )
	\brief Macro to write a SIL variable.
	\def Int32SIL_Write( Var, Val )
	\brief Macro to write a SIL variable.
	\def UInt32SIL_Write( Var, Val )
	\brief Macro to write a SIL variable.
*/
#define Int8SIL_Write( Var, Val )	Var.Value = (Int8)Val;   Var.Complement = (Int8)~Val
#define UInt8SIL_Write( Var, Val )	Var.Value = (UInt8)Val;  Var.Complement = (UInt8)~Val
#define Int16SIL_Write( Var, Val )	Var.Value = (Int16)Val;  Var.Complement = (Int16)~Val
#define UInt16SIL_Write( Var, Val )	Var.Value = (UInt16)Val; Var.Complement = (UInt16)~Val
#define Int32SIL_Write( Var, Val )	Var.Value = (Int32)Val;  Var.Complement = (Int32)~Val
#define UInt32SIL_Write( Var, Val )	Var.Value = (UInt32)Val; Var.Complement = (UInt32)~Val

/*! \def E_OK
	\brief The operation succeeded code.
	\def E_NOT_OK
	\brief The operation failed code.
	\def E_PENDING
	\brief The operation ongoing code.
	\def E_CANCELED
	\brief The operation cancel code.
*/
#define E_OK			((Std_ReturnType)0)
#define E_NOT_OK		((Std_ReturnType)1)
#define E_PENDING		((Std_ReturnType)2)
#define E_CANCELED		((Std_ReturnType)3)

/*! \def NULL_PTR
	\brief The null pointer.
	\def STD_ON
	\brief The pre-compiler switch.
	\def STD_OFF
	\brief The pre-compiler switch.
*/
#define NULL_PTR		((void*)0)
#define STD_ON			TRUE
#define STD_OFF			FALSE

/***
 * necessary for using within a C++ module
 */
#ifdef __cplusplus
}
#endif

/** @} (end addtogroup Std_Types) */
#endif	/* End include guard */
