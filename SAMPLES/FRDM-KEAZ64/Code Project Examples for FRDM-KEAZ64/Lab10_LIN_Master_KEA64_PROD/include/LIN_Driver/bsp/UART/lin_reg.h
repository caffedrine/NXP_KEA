/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup UART_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      bsp/UART/lin_reg.h
*
* @author    FPT Software
*
* @brief     UART register definition on S08, S12, S12X
*
******************************************************************************/

/******************************************************************************
 *
 * History:
 *
 * 20101027		v1.0	First version
 *
 *****************************************************************************/

#ifndef LIN_REG_H         /* Prevent duplicated includes */
#define LIN_REG_H

#include "lin_cfg/lin_hw_cfg.h"

/*************************************/
/******   Common Register struct *****/
/*************************************/

/**
* @var typedef unsigned char tU08
* unsigned 8 bits definition
*/
typedef unsigned char   tU08;   /**< unsigned 8 bit definition */

/**
* @var typedef unsigned char tU16
* unsigned 16 bits definition
*/
typedef unsigned short    tU16;   /**< unsigned 16 bit definition */

/**
* @var typedef unsigned long tU32
* unsigned 32 bits definition
*/
typedef unsigned long   tU32;   /**< unsigned 32 bit definition */

/**
* @var typedef unsigned char tS08
* signed 32 bits definition
*/
typedef signed char     tS08;   /**< signed 8 bit definition */

/**
* @var typedef signed int      tS16
* signed 16 bits definition
*/
typedef signed short      tS16;   /**< signed 16 bit definition */

/**
* @var typedef signed long      tS32
* signed 32 bits definition
*/
typedef signed long     tS32;   /**< signed 32 bit definition */

/**
* @var uREG08 tREG08
* register 8 bits
*/

/**
* @union uREG08
* register 8 bits
*/
typedef union uREG08    	/**< 8 bit register with byte and bit access*/
  {
  tU08  byte;           	/**< access whole register e.g. var.byte   = 0xFF;*/
  struct
    {
    tU08 _0 :1;				/**< bit 0 */
    tU08 _1 :1;				/**< bit 1 */
	tU08 _2 :1;				/**< bit 2 */
    tU08 _3 :1;				/**< bit 3 */
    tU08 _4 :1;				/**< bit 4 */
    tU08 _5 :1;				/**< bit 5 */
    tU08 _6 :1;				/**< bit 6 */
    tU08 _7 :1;				/**< bit 7 */
    }bit;					/**< access bit at a time  e.g. var.bit._7 = 1;   */
  }tREG08;


/**
* @var uREG16 tREG16
* register 16 bits
*/

/**
* @union uREG16
* register 16 bits
*/
typedef union uREG16   		/**< 16 bit register with word and byte access*/
  {
  tU16 word;           		/**< access whole word */
  struct
    {
    tREG08 msb;				/**< msb byte */
    tREG08 lsb;				/**< lsb byte */
    }byte;					/**< access byte at a time */
  }tREG16;

/*************************************/
/*******   UART Register struct *******/
/*************************************/


/**
* @union tUARTBDH
* UART baud rate higher byte
*/
typedef union  uUARTBDH
{
  tU08 byte;				/**< access register as byte */
  struct
  {
    tU08 sbr8  :1;   		/**< msb baud rate select bits: bit 8 */
    tU08 sbr9  :1;			/**< msb baud rate select bits: bit 9 */
    tU08 sbr10 :1;			/**< msb baud rate select bits: bit 10 */
    tU08 sbr11 :1;			/**< msb baud rate select bits: bit 11 */
    tU08 sbr12 :1;			/**< msb baud rate select bits: bit 12 */
    tU08 sbns  :1; 			/**< Stop Bit Number Select */
    tU08 rxegie :1;     	/**< RxD Input Active Edge Interrupt Enable (for RXEDGIF) */
    tU08 lbkdie :1;     	/**< LIN Break Detect Interrupt Enable (for LBKDIF) */
  }bit;						/**< access register as bits */
}tUARTBDH;

/**
* @union tUARTBDL
* UART baud rate lower byte
*/
typedef union uUARTBDL
{
  tU08 byte;				/**< access register as byte */
  struct
  {
    tU08 sbr0  :1;     		/**< lsb baud rate select bits: bit 0 */
    tU08 sbr1  :1;			/**< lsb baud rate select bits: bit 1 */
    tU08 sbr2  :1;			/**< lsb baud rate select bits: bit 2 */
    tU08 sbr3  :1;			/**< lsb baud rate select bits: bit 3 */
    tU08 sbr4  :1;			/**< lsb baud rate select bits: bit 4 */
    tU08 sbr5  :1;			/**< lsb baud rate select bits: bit 5 */
    tU08 sbr6  :1;			/**< lsb baud rate select bits: bit 6 */
    tU08 sbr7  :1;			/**< lsb baud rate select bits: bit 7 */
  }bit;						/**< access register as bits */
}tUARTBDL;

/**
* @union tUARTCR1
* UART control register 1
*/
typedef union uUARTCR1
{
  tU08  byte;				/**< access register as byte */
  struct
  {
    tU08 pt      :1;     	/**< parity type */
    tU08 pe      :1;     	/**< parity enable */
    tU08 ilt     :1;     	/**< idle line type */
    tU08 wake    :1;     	/**< wake up by address mark/idle */
    tU08 m       :1;     	/**< mode select */
    tU08 rsrc    :1;     	/**< receiver source */
    tU08 UARTswai :1;     	/**< stop in wait mode  */
    tU08 loops   :1;     	/**< loop mode/single wire mode enable */
  }bit;						/**< access register as bits */
}tUARTCR1;

/**
* @var uUARTCR2 tUARTCR2
*/
/**
* @union uUARTCR2
* * UART Control Register 2
*/
typedef union uUARTCR2
{
  tU08   byte;				/**< access register as byte */
  struct
  {
    tU08 sbk   :1;      	/**< send break character */
    tU08 rwu   :1;      	/**< receiver wake-up control */
    tU08 re    :1;      	/**< receiver enable */
    tU08 te    :1;      	/**< transmitter enable */
    tU08 ilie  :1;      	/**< idle line interrupt enable */
    tU08 rie   :1;      	/**< receiver interrupt enable   */
    tU08 tcie  :1;      	/**< transmit complete interrupt enable */
    tU08 tie   :1;      	/**< transmit interrupt enable   */
  }bit;						/**< access register as bits */
}tUARTCR2;

/**
* @var uUARTSR1 tUARTSR1
*/
/**
* @union uUARTSR1
* UART Status Register 1
*/
typedef union uUARTSR1
{
  tU08   byte;				/**< access register as byte */
  struct
  {
    tU08 pf    :1;      	/**< parity error flag  */
    tU08 fe    :1;      	/**< framing error flag */
    tU08 nf    :1;      	/**< noise error flag */
    tU08 orf   :1;      	/**< overrun error flag - 'or' in specification */
    tU08 idle  :1;      	/**< idle line detected flag */
    tU08 rdrf  :1;      	/**< receive data register full flag */
    tU08 tc    :1;      	/**< transmit complete flag */
    tU08 tdre  :1;      	/**< transmit data register empty flag */
  }bit;						/**< access register as bits */
}tUARTSR1;


/**
* @var uUARTSR2 tUARTSR2
* UART status register 2
*/
/**
* @union uUARTSR2
* UART status register 2
*/
typedef union uUARTSR2
{
  tU08 byte;				/**< access register as byte */
  struct
  {
    tU08 raf    :1;     	/**< receiver active flag */
    tU08 lbkde  :1;     	/**< LIN Break Detection Enable */
    tU08 brk13  :1;     	/**< Break Character Generation Length */
    tU08 rwuid  :1;     	/**< Receive Wake Up Idle Detect */
    tU08 rxinv  :1;     	/**< Receive Data Inversion */
    tU08        :1; 		/**< bit reserved area */
    tU08 rxedgif:1;     	/**< RxD Pin Active Edge Interrupt Flag */
    tU08 lbkdif :1;     	/**< LIN Break Detect Interrupt Flag */
  }bit;						/**< access register as bits */
}tUARTSR2;

/**
* @var uUARTCR3 tUARTCR3
*/
/**
* @union uUARTCR3
* UART control register 3
*/
typedef union uUARTCR3
{
  tU08 byte;				/**< access register as byte */
  struct
  {
    tU08 peie     :1;        /**< Parity Error Interrupt Enable */
    tU08 feie     :1;        /**< Framing Error Interrupt Enable */
    tU08 neie     :1;        /**< Noise Error Interrupt Enable */
    tU08 orie     :1;        /**< Overrun Interrupt Enable */
    tU08 txinv    :1;        /**< Transmit Data Inversion */
    tU08 txdir    :1;        /**< TxD Pin Direction in Single-Wire Mode */
    tU08 t8       :1;        /**< Ninth Data Bit for Transmitter */
    tU08 r8       :1;        /**< Ninth Data Bit for Receiver */
  }bit;						 /**< access register as bits */
}tUARTCR3;

/**
* @var uUARTD tUARTD
*/
/**
* @union uUARTD
* UART data
*/
typedef union uUARTD
{
  tU08   byte;				/**< access register as byte */
  struct
  {
    tU08 r0_t0 :1;      	/**< Receive/Transmit Data Bit 0 */
    tU08 r1_t1 :1;      	/**< Receive/Transmit Data Bit 1 */
    tU08 r2_t2 :1;      	/**< Receive/Transmit Data Bit 2 */
    tU08 r3_t3 :1;      	/**< Receive/Transmit Data Bit 3 */
    tU08 r4_t4 :1;      	/**< Receive/Transmit Data Bit 4 */
    tU08 r5_t5 :1;      	/**< Receive/Transmit Data Bit 5 */
    tU08 r6_t6 :1;      	/**< Receive/Transmit Data Bit 6 */
    tU08 r7_t7 :1;      	/**< Receive/Transmit Data Bit 7 */
  }bit;						/**< access register as bits */
}tUARTD;
/**
* @var uUARTDRH tUARTDRH
*/
/**
* @union uUARTDRH
* UART Data Register hight
*/
typedef union uUARTDRH
{
  tU08   byte;				/**< access register as byte */
  struct
  {
    tU08       :6;      	/**< not used */
    tU08 t8    :1;      	/**< transmit bit 8 */
    tU08 r8    :1;      	/**< receive bit 8   */
  }bit;						/**< access register as bits */
}tUARTDRH;
/**
* @struct tUART
* UART data structure
*/
typedef struct 				 /**< UART data structure  */
{
	  volatile tUARTBDH uartbdh;   /**< UART baud rate high register  */
	  volatile tUARTBDL uartbdl;   /**< UART baud rate low register   */
	  volatile tUARTCR1 uartcr1;   /**< UART control register 1       */
	  volatile tUARTCR2 uartcr2;   /**< UART control register 2 */
	  volatile tUARTSR1 uartsr1;   /**< UART status register 1 */
	  volatile tUARTSR2 uartsr2;   /**< UART status register 2 */
	  volatile tUARTCR3 uartcr3;   /**< UART control register 3 */
	  volatile tUARTD   uartd;     /**< UART data register */
}tUART;

	#define UARTBDH_RXEDGIE_MASK         64				/**< UART baud rate register high bit mask: RxD input active edge interrupt enable */
	#define UARTBDH_LBKDIE_MASK          128				/**< UART baud rate register high bit mask: LIN break detected interrupt enable */





/* UARTCR1 - UART Control Register 1 */

	#define UARTCR1_PT_MASK                1          	/**< UART control register 1 bit mask: parity type*/
	#define UARTCR1_PE_MASK                2          	/**< UART control register 1 bit mask: parity enable*/
	#define UARTCR1_ILT_MASK               4          	/**< UART control register 1 bit mask: idle line type select*/
	#define UARTCR1_WAKE_MASK              8          	/**< UART control register 1 bit mask: receiver wakeup method select*/
	#define UARTCR1_M_MASK                 16          	/**< UART control register 1 bit mask: 8 bit or 9 bit mode select*/
	#define UARTCR1_RSRC_MASK              32          	/**< UART control register 1 bit mask: receiver source select*/
	#define UARTCR1_UARTSWAI_MASK           64          	/**< UART control register 1 bit mask: UART stops in wait mode*/
	#define UARTCR1_LOOPS_MASK             128          	/**< UART control register 1 bit mask: loop mode select*/



/* UARTACR2 - UART Alternative Control Register 2 */


/* UARTCR2 - UART Control Register 2 */
	#define UARTCR2_SBK_MASK                1         	/**< UART control register 2 bit mask: send break */
	#define UARTCR2_RWU_MASK                2         	/**< UART control register 2 bit mask: receiver wakeup control */
	#define UARTCR2_RE_MASK                 4         	/**< UART control register 2 bit mask: receiver enable */
	#define UARTCR2_TE_MASK                 8         	/**< UART control register 2 bit mask: transmitter enable */
	#define UARTCR2_ILIE_MASK               16         	/**< UART control register 2 bit mask: idle line interrupt enable*/
	#define UARTCR2_RIE_MASK                32         	/**< UART control register 2 bit mask: receiver interrupt enable */
	#define UARTCR2_TCIE_MASK               64         	/**< UART control register 2 bit mask: transmission complete interrupt enable */
	#define UARTCR2_TIE_MASK                128         	/**< UART control register 2 bit mask: transmit interrupt enable */


/* UARTSR1 - UART Status Register */
	#define UARTSR1_PF_MASK                 1         	/**< UART status register 1 bit mask: parity error flag */
	#define UARTSR1_FE_MASK                 2         	/**< UART status register 1 bit mask: framing error flag */
	#define UARTSR1_NF_MASK                 4         	/**< UART status register 1 bit mask: noise flag */
	#define UARTSR1_OR_MASK                 8         	/**< UART status register 1 bit mask: receiver overrun flag */
	#define UARTSR1_IDLE_MASK               16         	/**< UART status register 1 bit mask: idle line flag */
	#define UARTSR1_RDRF_MASK               32         	/**< UART status register 1 bit mask: receive data register full flag */
	#define UARTSR1_TC_MASK                 64         	/**< UART status register 1 bit mask: transmission complete flag*/
	#define UARTSR1_TDRE_MASK               128         	/**< UART status register 1 bit mask: transmit data register empty flag */

/* UARTSR2 - UART Status Register 2 */




	#define UARTSR2_RAF_MASK                  1          /**< UART status register 2 bit mask: receiver active flag */
	#define UARTSR2_LBKDE_MASK                2          /**< UART status register 2 bit mask: LIN break detection enable */
	#define UARTSR2_BRK13_MASK                4          /**< UART status register 2 bit mask: break character length */
	#define UARTSR2_RWUID_MASK                8          /**< UART status register 2 bit mask: receive wakeup idle detect */
	#define UARTSR2_RXINV_MASK                16         /**< UART status register 2 bit mask: receive data inversion */
	#define UARTSR2_RXEDGIF_MASK              64         /**< UART status register 2 bit mask: RxD pin active edge interrupt flag */
	#define UARTSR2_LBKDIF_MASK               128        /**< UART status register 2 bit mask: LIN break detect interrupt flag */




/* UARTCR3 - UART Control Register 3 */
	#define UARTCR3_PEIE_MASK               1            /**< UART control register 3 bit mask: parity error interrupt enable */
	#define UARTCR3_FEIE_MASK               2            /**< UART control register 3 bit mask: framing error interrupt enable */
	#define UARTCR3_NEIE_MASK               4            /**< UART control register 3 bit mask: noise error interrupt enable */
	#define UARTCR3_ORIE_MASK               8            /**< UART control register 3 bit mask: overrun interrupt enable */
	#define UARTCR3_TXINV_MASK              16           /**< UART control register 3 bit mask: transmit data inversion */
	#define UARTCR3_TXDIR_MASK              32           /**< UART control register 3 bit mask: TxD pin direction in single-wire mode */
	#define UARTCR3_T8_MASK                 64           /**< UART control register 3 bit mask: Ninth data bit for transmitter */
	#define UARTCR3_R8_MASK                 128          /**< UART control register 3 bit mask: ninth data bit for receiver */

/* UARTD - UART Data Register */
	#define UARTD_R0_T0_MASK                1            /**< UART data register bit mask: bit 0 */
	#define UARTD_R1_T1_MASK                2            /**< UART data register bit mask: bit 1 */
	#define UARTD_R2_T2_MASK                4            /**< UART data register bit mask: bit 2 */
	#define UARTD_R3_T3_MASK                8            /**< UART data register bit mask: bit 3 */
	#define UARTD_R4_T4_MASK                16           /**< UART data register bit mask: bit 4 */
	#define UARTD_R5_T5_MASK                32           /**< UART data register bit mask: bit 5 */
	#define UARTD_R6_T6_MASK                64           /**< UART data register bit mask: bit 6 */
	#define UARTD_R7_T7_MASK                128          /**< UART data register bit mask: bit 7 */


/*--------------------------------------------------------------------------------*/
/*CongTH*/
typedef struct UART_MemMap1 {
  uint8_t BDH;                                     /**< UART Baud Rate Register: High, offset: 0x0 */
  uint8_t BDL;                                     /**< UART Baud Rate Register: Low, offset: 0x1 */
  uint8_t C1;                                      /**< UART Control Register 1, offset: 0x2 */
  uint8_t C2;                                      /**< UART Control Register 2, offset: 0x3 */
  uint8_t S1;                                      /**< UART Status Register 1, offset: 0x4 */
  uint8_t S2;                                      /**< UART Status Register 2, offset: 0x5 */
  uint8_t C3;                                      /**< UART Control Register 3, offset: 0x6 */
  uint8_t D;                                       /**< UART Data Register, offset: 0x7 */
} volatile UART_Ptr;


#endif /* Lin register */

/**
* @}
*/
