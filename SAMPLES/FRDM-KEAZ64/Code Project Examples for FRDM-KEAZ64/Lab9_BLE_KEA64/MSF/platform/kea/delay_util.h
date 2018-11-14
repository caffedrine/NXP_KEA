/****************************************************************************
 * @file     delay_util.h
 * @brief    busy-loop delays for Kinetis MCUs 
 * @version  1
 * @date     6. May 2014
 *
 * @note     This file is included into msf.h.
 *
 ******************************************************************************/
#ifndef MSF_DELAY_UTIL_H
#define MSF_DELAY_UTIL_H


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup group_globals  
* @{ 
*/

/** Convenience inline functions for very short busy-loop waiting
 * IMPORTANT: the results will be incorrect if compiler optimizations are not enabled.
 * For example, the inline functions are not "inlined" but called which results in longer delays.
*/
#if		F_CPU == 4000000
    /* 1 clock is 1/4 us */
    /** Delay for 1 us
     * always_inline attribute makes sure it is inlined even if optimisations are off or Os (for size)
     */
	static inline void MSF_DELAY_1US(void) __attribute__((always_inline, unused));
    static inline void MSF_DELAY_1US(void)
    {
        // Need 4 clocks
        __asm__ __volatile__ (
				"PUSH {r0} " "\n\t"			// 2 clocks
				"POP {r0} " "\n\t"			// 2 clocks
		);
    }

    /** Delay for 5 us
     */
    static inline void MSF_DELAY_5US(void) __attribute__((always_inline, unused));
    static inline void MSF_DELAY_5US(void)
    {
        // need 20 clocks
        // For N repetitions the loop takes 3*N-1 clocks
        // plus 5 clocks the other code;
    	// 3 * 5 - 1 = 14 + 5 = 19 + 1 nop to get 20
         __asm__ __volatile__ (
			"PUSH {r0}" "\n\t"			// 2 clocks
            "MOV r0,#5" "\n\t"         // 1 clock
            "1: SUB r0, #1" "\n\t"	// 1 clock
			"BNE 1b"  "\n\t"		// 2 clocks if branches, 1 if not; 1b means local label backwards
			"POP {r0}"              // 2 clocks
        	"NOP" "\n\t"     		// 1 clock
		);
    }

#elif     F_CPU == 8000000
    /* 1 clock is 1/8 us */ 
    /** Delay for 1 us
     * always_inline attribute makes sure it is inlined even if optimisations are off or Os (for size)
     */  
	static inline void MSF_DELAY_1US(void) __attribute__((always_inline, unused));
    static inline void MSF_DELAY_1US(void)
    {        
        // Need 8 clocks
        __asm__ __volatile__ (				
				"PUSH {r0} " "\n\t"			// 2 clocks
				"POP {r0} " "\n\t"			// 2 clocks
                "PUSH {r0} " "\n\t"			// 2 clocks 
				"POP {r0} " "\n\t"			// 2 clocks                
		);
    }
    
    /** Delay for 5 us
     */ 
    static inline void MSF_DELAY_5US(void) __attribute__((always_inline, unused));
    static inline void MSF_DELAY_5US(void)
    {       
        // need 40 clocks
        // For N repetitions the loop takes 3*N-1 clocks
        // plus 5 clocks the other code;
    	// 3 * 12 - 1 = 35 + 5 = 40
         __asm__ __volatile__ (				
			"PUSH {r0}" "\n\t"			// 2 clocks
            "MOV r0,#12" "\n\t"         // 1 clock
            "1: SUB r0, #1" "\n\t"	// 1 clock
			"BNE 1b"  "\n\t"		// 2 clocks if branches, 1 if not; 1b means local label backwards
			"POP {r0}"              // 2 clocks			
		);
    }

#elif   ((F_CPU == 20900000) || (F_CPU == 20970000))
    /** Delay for 1 us
     */  
    static inline void MSF_DELAY_1US(void) __attribute__((always_inline, unused));
    static inline void MSF_DELAY_1US(void)
    {     
        // Need 21 clocks   
        // For N repetitions the loop takes 3*N-1 clocks
        // plus 5 clocks the other code;
        // 3 * 5 -1 = 14 + 5 = 19 -> add 2 nops
         __asm__ __volatile__ (				
			"NOP" "\n\t"     // 1 clock
            "NOP" "\n\t"	
            "PUSH {r0}" "\n\t"			// 2 clocks
            "MOV r0,#5" "\n\t"         // 1 clock
            "1: SUB r0, #1" "\n\t"	// 1 clock
			"BNE 1b"  "\n\t"		// 2 clocks if branches, 1 if not
			"POP {r0}"              // 2 clocks			
		);
    }
    
    /** Delay for 5 us
     */ 
    static inline void MSF_DELAY_5US(void) __attribute__((always_inline, unused));
    static inline void MSF_DELAY_5US(void)
    {     
        // Need 104.5 clocks   
        // For N repetitions the loop takes 3*N-1 clocks
        // plus 5 clocks the other code;
        // 3 * 33  -1 = 98 + 5 = 103 -> add 2 nops
         __asm__ __volatile__ (				
			"NOP" "\n\t"     // 1 clock
            "NOP" "\n\t"	
            "PUSH {r0}" "\n\t"			// 2 clocks
            "MOV r0,#33" "\n\t"         // 1 clock
            "1: SUB r0, #1" "\n\t"	// 1 clock
			"BNE 1b"  "\n\t"		// 2 clocks if branches, 1 if not
			"POP {r0}"              // 2 clocks			
		);
    }

#elif  F_CPU == 41943040
    
    /** Delay for 1 us
     */ 
    static inline void MSF_DELAY_1US(void) __attribute__((always_inline, unused));
    static inline void MSF_DELAY_1US(void)
    {     
        // Need 42 clocks   
        // For N repetitions the loop takes 3*N-1 clocks
        // plus 5 clocks the other code;
        // 3 * 12 -1 = 35 + 5 = 40 -> add 2 nops
         __asm__ __volatile__ (				
			"NOP" "\n\t"     // 1 clock
            "NOP" "\n\t"	
            "PUSH {r0}" "\n\t"			// 2 clocks
            "MOV r0,#12" "\n\t"         // 1 clock
            "1: SUB r0, #1" "\n\t"	// 1 clock
			"BNE 1b"  "\n\t"		// 2 clocks if branches, 1 if not
			"POP {r0}"              // 2 clocks			
		);
    }
    
     /** Delay for 5 us
     */
    static inline void MSF_DELAY_5US(void) __attribute__((always_inline, unused));
    static inline void MSF_DELAY_5US(void)
    {     
        // Need 210 clocks   
        // For N repetitions the loop takes 3*N-1 clocks
        // plus 5 clocks the other code;
        // 3 * 68  -1 = 203 + 5 = 208 -> add 2 nops
         __asm__ __volatile__ (				
			"NOP" "\n\t"     // 1 clock
            "NOP" "\n\t"	
            "PUSH {r0}" "\n\t"			// 2 clocks
            "MOV r0,#68" "\n\t"         // 1 clock
            "1: SUB r0, #1" "\n\t"	// 1 clock
			"BNE 1b"  "\n\t"		// 2 clocks if branches, 1 if not
			"POP {r0}"              // 2 clocks			
		);
    }
    
#elif  F_CPU == 48000000
     
    /** Delay for 1 us
     */    
    static inline void MSF_DELAY_1US(void) __attribute__((always_inline));
    static inline void MSF_DELAY_1US(void)
    {     
        // Need 48 clocks   
        // For N repetitions the loop takes 3*N-1 clocks
        // plus 5 clocks the other code;
        // 3 * 14 -1 = 41 + 5 = 46 -> add 2 nops
         __asm__ __volatile__ (				
			"NOP" "\n\t"     // 1 clock
            "NOP" "\n\t"	
            "PUSH {r0}" "\n\t"			// 2 clocks
            "MOV r0,#14" "\n\t"         // 1 clock
            "1: SUB r0, #1" "\n\t"	// 1 clock
			"BNE 1b"  "\n\t"		// 2 clocks if branches, 1 if not
			"POP {r0}"              // 2 clocks			
		);
    }
    
    /** Delay for 5 us
     */  
    static inline void MSF_DELAY_5US(void) __attribute__((always_inline));
    static inline void MSF_DELAY_5US(void)
    {       
        // need 240 clocks
        // For N repetitions the loop takes 3*N-1 clocks
        // plus 5 clocks the other code;
        // 3*78 = 234 + 5 = 239 -> add 1 nop
         __asm__ __volatile__ (	
         	"NOP" "\n\t"     // 1 clock			
			"PUSH {r0}" "\n\t"			// 2 clocks
            "MOV r0,#78" "\n\t"         // 1 clock
            "1: SUB r0, #1" "\n\t"	// 1 clock
			"BNE 1b"  "\n\t"		// 2 clocks if branches, 1 if not
			"POP {r0}"              // 2 clocks			
		);
    }
      
#else
    #warning Short delay functions not defined for current F_CPU. 
#endif 
/** @}*/

#ifdef __cplusplus
}
#endif

#endif  /* MSF_DELAY_UTIL_H */
