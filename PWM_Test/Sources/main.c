#include "Cpu.h"
#include "Events.h"
#include "PWM1.h"
#include "PwmLdd1.h"
#include "TU1.h"

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

void PWM_Driver_Init(uint32_t periodUS)
{
	(void)PwmLdd1_Init(NULL);
}

int main()
{
	PWM_Driver_Init(1000);

	PWM1_Enable();

	int i, j;
	while(1)
	{
		for(i=0; i<=65535; i++)
		{
			PWM1_SetRatio16(i);
			for(int j=0; j <= 10; j++);
		}

		for(i=65534; i>=1; i--)
		{
			PWM1_SetRatio16(i);
			for(int j=0; j <= 10; j++);
		}
	}
}
