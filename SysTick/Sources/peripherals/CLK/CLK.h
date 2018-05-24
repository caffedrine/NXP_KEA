#ifndef CLK_H_
#define CLK_H_

#include "derivative.h"

//	  ____  _____ ____ _        _    ____      _  _____ ___ ___  _   _ ____
//	 |  _ \| ____/ ___| |      / \  |  _ \    / \|_   _|_ _/ _ \| \ | / ___|
//	 | | | |  _|| |   | |     / _ \ | |_) |  / _ \ | |  | | | | |  \| \___ \
//	 | |_| | |__| |___| |___ / ___ \|  _ <  / ___ \| |  | | |_| | |\  |___) |
//	 |____/|_____\____|_____/_/   \_\_| \_\/_/   \_\_| |___\___/|_| \_|____/

void Clk_Init();


//	  ____  _____ _____ ___ _   _ ___ _____ ___ ___  _   _ ____
//	 |  _ \| ____|  ___|_ _| \ | |_ _|_   _|_ _/ _ \| \ | / ___|
//	 | | | |  _| | |_   | ||  \| || |  | |  | | | | |  \| \___ \
//	 | |_| | |___|  _|  | || |\  || |  | |  | | |_| | |\  |___) |
//	 |____/|_____|_|   |___|_| \_|___| |_| |___\___/|_| \_|____/

void Clk_Init()
{
	ICS_C1|=ICS_C1_IRCLKEN_MASK; 			/* Enable the internal reference clock*/
#ifdef SKEAZ1284
	ICS_C3 = 0x90; 							/* Reference clock frequency = 31.25 kHz*/
#elif defined SKEAZN642
	ICS_C3 = 0x50; 							/* Reference clock frequency = 31.25 kHz*/
#endif
	while(!(ICS_S & ICS_S_LOCK_MASK));  	/* Wait for PLL lock, now running at 40 MHz (1280 * 31.25Khz) */		
    ICS_C2|=ICS_C2_BDIV(1)  ; 				/*BDIV=2, Bus clock = 20 MHz*/
	//ICS_S |= ICS_S_LOCK_MASK ; 				/* Clear Loss of lock sticky bit */
}

#endif /* CLK_H_ */
