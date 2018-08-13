#ifndef CLK_H_
#define CLK_H_

#include "derivative.h"

#define CLK_FREQUENCY_32_MHZ    32000000  //HZ
#define CLK_FREQUENCY_16_MHZ    16000000  //HZ
#define CLK_FREQUENCY_8_MHZ      8000000  //HZ

//	  ____  _____ ____ _        _    ____      _  _____ ___ ___  _   _ ____
//	 |  _ \| ____/ ___| |      / \  |  _ \    / \|_   _|_ _/ _ \| \ | / ___|
//	 | | | |  _|| |   | |     / _ \ | |_) |  / _ \ | |  | | | | |  \| \___ \
//	 | |_| | |__| |___| |___ / ___ \|  _ <  / ___ \| |  | | |_| | |\  |___) |
//	 |____/|_____\____|_____/_/   \_\_| \_\/_/   \_\_| |___\___/|_| \_|____/

void Clk_Init(uint32_t u32frequency);


//	  ____  _____ _____ ___ _   _ ___ _____ ___ ___  _   _ ____
//	 |  _ \| ____|  ___|_ _| \ | |_ _|_   _|_ _/ _ \| \ | / ___|
//	 | | | |  _| | |_   | ||  \| || |  | |  | | | | |  \| \___ \
//	 | |_| | |___|  _|  | || |\  || |  | |  | | |_| | |\  |___) |
//	 |____/|_____|_|   |___|_| \_|___| |_| |___\___/|_| \_|____/

void Clk_Init(uint32_t u32frequency) {

	if (u32frequency == CLK_FREQUENCY_32_MHZ)   // 32 MHZ
	{
		/* the following code segment demonstrates setting the ICS to FEI mode using the factory
		 trim value. The resulting ICSOUT frequency is fint_ft*1024/BDIV. */
		ICS_C2 = 0x20; // BDIV=divide by 2 – use default until clock dividers configured
		ICS_C1 = 0x04; // internal reference clock as source to FLL
		while ((ICS_S & ICS_S_LOCK_MASK) == 0); // wait for FLL to lock
		SIM_BUSDIV = 0x00000000; // core clock = ICSOUT and bus clock = core clock/2 //32MHZ=00   16MHZ=01
		ICS_C2 = 0x00; // BDIV=divide by 1 – allows max core and bus clock frequencies
	}

	if (u32frequency == CLK_FREQUENCY_16_MHZ)  // 16 MHZ
	{
		/* the following code segment demonstrates setting the ICS to FEI mode using the factory
		 trim value. The resulting ICSOUT frequency is fint_ft*1024/BDIV.*/
		ICS_C2 = 0x20; // BDIV=divide by 2 – use default until clock dividers configured
		ICS_C1 = 0x04; // internal reference clock as source to FLL
		while ((ICS_S & ICS_S_LOCK_MASK) == 0); // wait for FLL to lock
		SIM_BUSDIV = 0x00000001; // core clock = ICSOUT and bus clock = core clock/2 //32MHZ=00   16MHZ=01
		ICS_C2 = 0x00; // BDIV=divide by 1 – allows max core and bus clock frequencies
	}

	if (u32frequency == CLK_FREQUENCY_8_MHZ)  // 8MHZ
	{
		/* the following code segment demonstrates setting the ICS to FEI mode using the factory
		 trim value. The resulting ICSOUT frequency is fint_ft*1024/BDIV.*/
		ICS_C2 = 0x20; // BDIV=divide by 2 – use default until clock dividers configured
		ICS_C1 = 0x04; // internal reference clock as source to FLL
		while ((ICS_S & ICS_S_LOCK_MASK) == 0); // wait for FLL to lock
		SIM_BUSDIV = 0x00000001; // core clock = ICSOUT and bus clock = core clock/2 //32MHZ=00   16MHZ=01
		ICS_C2 = 0x20; // BDIV=divide by 2
	}

}

#endif /* CLK_H_ */
