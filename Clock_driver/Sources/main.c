#include "derivative.h"

#include "NVIC/nvic.h"
#include "CLK/CLK.h"
#include "DbgConsole/DbgConsole.h"

static int i = 0;

int main(void)
{
	Clk_Init(CLK_FREQUENCY_16_MHZ);


	SIM_SOPT |= SIM_SOPT_CLKOE_MASK;		/* Enable Bus clock on PTH2 */
	SIM_SOPT |= SIM_SOPT_BUSREF(0b111); 	/* Bus clock output on PTH2 divided by 128 */


	for ( ;; )
	{
	}

	return 0;
}
