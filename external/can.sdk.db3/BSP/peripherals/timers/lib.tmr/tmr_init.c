/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tmr_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr.h"
#include "fiq.h"
#include "fiq_private.h"
#include "gpio.h"
#include "tmr_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
#ifdef PLATFORM_404
static void timer0_init (void) ;
#endif
static void timer1_init (void) ;
static void timer2_init (void) ;
static void timer3_init (void) ;



/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void tmr_init (void)
{
#ifdef PLATFORM_404
	timer0_init () ;
#endif
	
	timer1_init () ; 

	timer2_init () ;

	timer3_init () ;
}

#ifdef PLATFORM_404
/*==========================================================================*/
static void timer0_init (void)
{
	/// Setup TIMER-0 for 1 ms IRQ. For testing, we assume 72 MHz clock and divider is 1 ///
 
  PCONP |= (1u << 1) ;

  /// Setup TIMER-0 for 1 ms IRQ. For testing, we assume 72 MHz clock and divider is 1 ///

  PCLKSEL0 &= ~(3u << 2) ;
  PCLKSEL0 |=  (4u << 2) ;    // Divide CPU clock by 1

	T0PR   = 0x00000000 ;       // Set prescaler to divide by 1

	//Match compare registers **NOT IN USE
 // T0MR0  = 72000 - 1  ;       // Set match limit
 // T0MCR |= 0x03 ;             // On match 0, generate IRQ and reset timer counter 
  
	PINSEL3 |= 3<<20; // set capture on CAP0.0 which is P1[26]
	T0CTCR = 0x00; // 1:0 = 11 counter mode: TC incremended on CAP from bit 3:2 on both edges
									// 3:2 = 00 capture on CAP0.0 for TIMER0
	T0CCR =((0<<0)|(0<<1)|(0<<2));// Daniel.G - CAP0RE, CAP0FE, CAP0I - capture rising edge, falling edge, generate interrupt on capture event
																//not capturing rising edge currently(bit 0 = 0)
																//initialized as not capturing any edges and no interrupt generated. This gets controlled by bypass library if there is need.
  T0TCR  = 0x01 ;             // Enable TIMER-0

																																										//than 	fiq_service_vector(); by means of isr_set_fiq_channel (ISR_ID_TIMER_0) ;
	isr_set_fiq_channel (ISR_ID_TIMER_0) ;	
	
	timer_base_addr[3] = TMR0_BASE_ADDR ;	
}


//--------------------------------------------------------------------------//


#endif
/*==========================================================================*/
static void timer1_init (void)
{
	#ifdef DEV_LPC_236x
	PCONP |= (1u << 2) ;		// POWER the TIMER

	PCLKSEL0 &= ~(3u << 4) ;
	PCLKSEL0 |=  (1u << 4) ;    // Divide CPU clock by 1
	#endif

	T1PR   = 0x00000000 ;       // Set prescaler to divide by 1
       
	T1TCR  = 0x01 ;             // Enable TIMER

	isr_set_fiq_channel (ISR_ID_TIMER_1) ;	
	
	timer_base_addr[0] = TMR1_BASE_ADDR ;	
}

/*==========================================================================*/
static void timer2_init (void)
{
	#ifdef DEV_LPC_236x
	PCONP |= (1u << 22) ;		// POWER the TIMER

	PCLKSEL1 &= ~(3u << 12) ;
	PCLKSEL1 |=  (1u << 12) ;    // Divide CPU clock by 1
	#endif

	T2PR   = 0x00000000 ;       // Set prescaler to divide by 1
       
	T2TCR  = 0x01 ;             // Enable TIMER

	isr_set_fiq_channel (ISR_ID_TIMER_2) ;	

	timer_base_addr[1] = TMR2_BASE_ADDR ;
}

/*==========================================================================*/
static void timer3_init (void)
{
	#ifdef DEV_LPC_236x
	PCONP |= (1u << 23) ;		// POWER the TIMER

	PCLKSEL1 &= ~(3u << 14) ;
	PCLKSEL1 |=  (1u << 14) ;    // Divide CPU clock by 1
	#endif

	T3PR   = 0x00000000 ;       // Set prescaler to divide by 1
       
	T3TCR  = 0x01 ;             // Enable TIMER

	isr_set_fiq_channel (ISR_ID_TIMER_3) ;	

	timer_base_addr[2] = TMR3_BASE_ADDR ;
}



