// Device specific configurations
#ifndef _DERIVATIVE_H_
#define _DERIVATIVE_H_


/*
#define SKEAZ1284
//*/ #define SKEAZN642

#ifdef SKEAZ1284
	#include "SKEAZ1284.h"
	// PINSEL translation
	#define SIM_PINSEL	SIM_PINSEL_REG(SIM)

	// Translations
	#define FTMRH						FTMRE
	#define FTMRH_FSTAT 				FTMRE_FSTAT
	#define FTMRH_FSTAT_CCIF_MASK		FTMRE_FSTAT_CCIF_MASK
	#define FTMRH_FCLKDIV				FTMRE_FCLKDIV
	#define FTMRH_FCLKDIV_FDIVLCK_MASK	FTMRE_FCLKDIV_FDIVLCK_MASK
	#define FTMRH_FCLKDIV_FDIVLD_MASK	FTMRE_FCLKDIV_FDIVLD_MASK
	#define FTMRH_FCLKDIV_FDIV_MASK		FTMRE_FCLKDIV_FDIV_MASK
	#define FTMRH_FCLKDIV_FDIV			FTMRE_FCLKDIV_FDIV

#elif defined SKEAZN642
	#include "SKEAZN642.h"
#endif

//	  ____ _       _           _     __             _   _
//	 / ___| | ___ | |__   __ _| |   / _|_   _ _ __ | |_(_) ___  _ __  ___
//	| |  _| |/ _ \| '_ \ / _` | |  | |_| | | | '_ \| __| |/ _ \| '_ \/ __|
//	| |_| | | (_) | |_) | (_| | |  |  _| |_| | | | | |_| | (_) | | | \__ \
//	 \____|_|\___/|_.__/ \__,_|_|  |_|  \__,_|_| |_|\__|_|\___/|_| |_|___/
//



#endif //_DERIVATIVE_H_
