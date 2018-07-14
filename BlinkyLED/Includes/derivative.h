// Device specific configurations


#ifdef SKEAZ1284
	#include "SKEAZ1284.h"

	/* Translations for fixing compatibility issues */
	#define SIM_PINSEL	SIM_PINSEL_REG(SIM)

	#define FTMRH						FTMRE
	#define FTMRH_FSTAT 				FTMRE_FSTAT
	#define FTMRH_FSTAT_CCIF_MASK		FTMRE_FSTAT_CCIF_MASK
	#define FTMRH_FCLKDIV				FTMRE_FCLKDIV
	#define FTMRH_FCLKDIV_FDIVLCK_MASK	FTMRE_FCLKDIV_FDIVLCK_MASK
	#define FTMRH_FCLKDIV_FDIVLD_MASK	FTMRE_FCLKDIV_FDIVLD_MASK
	#define FTMRH_FCLKDIV_FDIV_MASK		FTMRE_FCLKDIV_FDIV_MASK
	#define FTMRH_FCLKDIV_FDIV			FTMRE_FCLKDIV_FDIV

#endif

#ifdef SKEAZN642
	#include "SKEAZN642.h"

	/* Translations for fixing compatibility issues */
	#define PUE0	PUEL
	#define PUE1	PUEH
	#define PUE2	PUEH		//! !!!

#endif
