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
#include "fiq_private.h"
#include "tmr_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

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
	timer1_init () ; 

	timer2_init () ;

	timer3_init () ;
}

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



