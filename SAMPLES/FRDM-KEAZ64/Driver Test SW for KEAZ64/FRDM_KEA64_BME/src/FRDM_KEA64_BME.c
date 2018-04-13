
/********************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************//*
*
* @file FRDM_KEA64_BME.c
*
* @author Freescale
*
* @version 0.0.1
*
* @brief This demo compares the bit manipulation using simple C code and BME.
* 		 It has examples for simple read-modify-write operation on one bit
* 		 and bit field using simple C and BME.
* @note  the compiler optimization level will affect the code execution time,
*        it is recommended that high level optimization for speed be used.
********************************************************************************/

#include "ics.h"
#include "uart.h"
#include "bme.h"
#include "printf.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define GPIO_ALIAS_OFFSET       0x000F0000L                                          /* Alias GPIO base address to 0x4000F000 which can be used for BME BFI/UBFX function  */
#define GPIOB_PDOR_ALIAS        (((uint32_t)&GPIOB->PDOR)-GPIO_ALIAS_OFFSET)         /* Alias GPIO base address to 0x4000F000 which can be used for BME BFI/UBFX function  */

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/
uint32_t BME_LogicOPwithC(void);
uint32_t BME_LogicOPwithBME(void);
uint32_t BME_BFIwithC(uint32_t *pAddr, uint8_t u8BitPos, uint8_t u8FieldWidth, uint32_t u32Data);
uint32_t BME_BFIwithBME(void);

/******************************************************************************
* Global Definition
******************************************************************************/
#ifdef __GNUC__
//#define __ramfunc __attribute__((section (".data")))
#define __ramfunc __attribute__((section(".text")))
#else
#define __ramfunc /* Define this if you use a different compiler*/
#endif

/******************************************************************************
* Global functions
******************************************************************************/

/********************************************************************/
int main (void)
{
    /* Perform processor initialization */
	ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;		/* ICS in FLL engaged internal mode*/
	ICS_set.bdiv=0;
	ICS_Init(&ICS_set);		             	/* Initialization of Core clock at 48 MHz, Bus clock at 24 MHz */

	/* UART Initialization */
	UART_ConfigType UART_Config={{0}};

	UART_Config.sctrl1settings.bits.bM=0;  	/* 8 bit mode*/
	UART_Config.sctrl1settings.bits.bPe=0;	/* No hardware parity generation or checking*/
	UART_Config.bSbns=0;					/* One stop bit*/
	UART_Config.sctrl2settings.bits.bRe=1;	/* Enable Receiver*/
	UART_Config.sctrl2settings.bits.bTe=1;	/* Enable Transmitter*/
	UART_Config.u32SysClkHz = 24000000;   	/* Bus clock in Hz*/
	UART_Config.u32Baudrate = 115200;     	/* UART baud rate */

	UART_Init(UART2,&UART_Config);			/*Initialization of UART utilities*/

    printf("\nRunning the FRDM_KEA64_BME project.\n\r");

    /* Start to test bit operations with normal C and BME */
    printf("Logic operation in C takes %d ticks.\n\r", (int)BME_LogicOPwithC());
    printf("Logic operation with BME takes %d ticks\n\r", (int)BME_LogicOPwithBME());

    /*!
     * Configure PTG0 to PTG3 as output pin
     */
    GPIOB->PDDR |= (0xF << 16);             /* Configure PTG0 to PTG3 as output pins */
    GPIOB->PIDR &= ~(0xF << 16);            /* Note: Must clear the PIDR bit in order to read correct data from the pin */


    printf("Bit field operation in C takes %d ticks.\n\r", (int)BME_BFIwithC((uint32_t*)&GPIOB->PDOR,16,4-1, 5<<16));  /* (Bit width - 1) as 3rd parameter */
    printf("Bit field operation with BME takes %d ticks.\n\r", (int)BME_BFIwithBME());
    printf("Test completed.\n\r");

    return 0;
}

/*****************************************************************************//*!
*
* @brief perform logic operation in C.
*
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__ramfunc uint32_t BME_LogicOPwithC(void)
{
    uint32_t    u32PortVal = 0;
    uint32_t    u32LogicOPTicks;

    /* Configure PTA1 as output pin */
    GPIOA->PDDR |= 0x02;
    GPIOA->PIDR &= ~0x02;           		/* Note: Must clear the PIDR bit in order to read correct data at the pin */

    /* Initialize the system tick and count the time of operations */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    SysTick->VAL = 0x0;						/* Clear current timer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);

    /* Normal read modify and write sequence to toggle a GPIO */
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;

    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;

    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;

    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;

    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;

    /* Measure Ticks */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks);
}


/*****************************************************************************//*!
*
* @brief perform logic operation via BME .
*
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__ramfunc uint32_t BME_LogicOPwithBME(void)
{
    uint32_t    u32LogicOPTicks;
    /* Configure PTA1 as output pin */
    GPIOA->PDDR |= 0x02;
    GPIOA->PIDR &= ~0x02;           		/* Note: must clear the PIDR bit in order to read correct data from the pin */

    /* Initialize the system tick and count the time of operations */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    SysTick->VAL = 0x0;						/* clear current timer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);

    /* BME sequence to toggle a GPIO */
    BME_XOR(&GPIOA->PDOR) = 0x02;
    BME_XOR(&GPIOA->PDOR) = 0x02;
    BME_XOR(&GPIOA->PDOR) = 0x02;
    BME_XOR(&GPIOA->PDOR) = 0x02;
    BME_XOR(&GPIOA->PDOR) = 0x02;

    /* Measure ticks */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks);
}

/*****************************************************************************//*!
*
* @brief perform bit field operation with normal C code .
*
* @param[in] pAddr          pointer to destination address of a 32-bit data.
* @param[in] u8BitPos       bit field position of a 32-bit data to be operated.
* @param[in] u8FieldWidth   bit field width-1 of a 32-bit data to be replaced.
* @param[in] u32Data        32-bit data containing the bit field to be inserted to
*                           the corresponding bit field of 32-bit data.
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__ramfunc uint32_t BME_BFIwithC(uint32_t *pAddr, uint8_t u8BitPos, uint8_t u8FieldWidth, uint32_t u32Data)
{
    uint32_t    u32RegVal;
    uint32_t    u32Mask;
    uint32_t    u32LogicOPTicks;

    /* Initialize the system tick and count the time of operations */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    SysTick->VAL = 0x0;						/* clear current timer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);

    /* Bit Field Manipulation code */
    u32RegVal = *pAddr;
    u32Mask = ((1 << (u8FieldWidth+1)) - 1) << u8BitPos;
    u32RegVal = (u32RegVal & ~u32Mask)|((u32Data) & u32Mask);
    *pAddr = u32RegVal;

    /* Measure ticks */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks);
}


/*****************************************************************************//*!
*
* @brief perform bit field operation with BME.
*       Perform bit field operation and emulate GPIO mask.
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__ramfunc uint32_t BME_BFIwithBME(void)
{

    uint32_t    u32LogicOPTicks;
    uint32_t    u32Data = (0x5 << 16);
    uint32_t    u32Addr = GPIOB_PDOR_ALIAS;

    /* Configure PTG0 to PTG3 as output pin */
    GPIOB->PDDR |= (0xF << 16);             /* Configure PTG0 to PTG3 as output pins */
    GPIOB->PDOR =  0;                       /* Output 0 to PTG0 to PTG3  */
    GPIOB->PIDR &= ~(0xF << 16);            /* Note: must clear the PIDR bit in order to read correct data from the pin */

    /* Initialize the system tick and count the time of operations */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    SysTick->VAL = 0x0;						/* Clear current timer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);


    BME_BITFIELD_INSERT(u32Addr,16,4) =  u32Data;  /* Write 5 to bit 19..16 */

    /* Measure ticks */
    u32LogicOPTicks = SysTick->VAL;

    /* Check if the data field is inserted correctly (canceled)*/
    	/*
    if(u32Data != GPIOB->PDIR)
    {
        printf("Error: bitfield insertion with BME failed!\n");
    }
    	 */
    return (SysTick->LOAD - u32LogicOPTicks);
}
