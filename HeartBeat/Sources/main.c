#include "derivative.h" /* include peripheral declarations SKEAZ128M4 */
#include "FTM.h"
#include "PWT.h"
#include "clocks.h"

void init_clk_FEE_40MHz(void);

int main(void)
{
	init_clks_FEE_40MHz(); 	/* KEA128 clks FEE, 8MHz xtal: core 40 MHz, bus 20MHz */
	init_FTM(); 			/* Enable bus clock to FTM1,2 prescaled by 128 */
	init_FTM1_ch1_PWM(); 	/* PTE7 output, to blue LED */
	init_FTM2_ch1_OC(); 	/* PTH1 output, to green LED & J2_10 */
	init_FTM2_ch5_IC(); 	/* PTB5 input; connect J2_5 and J2_10  */

	init_PWT(); 			/* PTD5 input */
	start_FTM_counters();

	for ( ;; )
	{
									/* Poll to look for timed I/O flags */
		output_compare_FTM2_ch1(); 	/* If output compare match: */

									/*   then toggle pin, clear flag, reload timer */
		input_capture_FTM2_ch5(); 	/* If input captured: clear flag, read timer */

		pulse_width_timer_PWT(); 	/* If two falling edges captured: */
									/*   then clear flag, read pulse width value */
	}
}
