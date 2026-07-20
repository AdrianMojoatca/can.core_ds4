/*==========================================================================*/
// $Id: wake_clk_setup.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
#include "wake_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
// PLL Feed Seqence -------------------------------------------------------------

// This function performs the sequence atomically, and can be called whether the
// FIQ or IRQ are currently enabled or disabled; the state of FIQ or IRQ will not 
// be affected upon exit.

// This function can be called from assembly or C.


/*--------------------------------------------------------------------------*/

// PLL Feed Seqence

void crt0_pll_feed(void)
{
	ATOMIC
	(
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	)
}

/*--------------------------------------------------------------------------*/

// Clock Setup

void crt0_clock_setup(void)
{
	crt0_pll_feed();
		
	SCS = SCS_Val;									//Enable main oscillator
		
	if( (SCS_Val & OSCEN) != 0 )
		{
			while((SCS & OSCSTAT)==0)     // wait for oscillator ready to use
			;  
		}
		
	CLKSRCSEL = CLKSRCSEL_Val;    	// select board crystal for PLL clock
		
	PLLCFG = PLLCFG_Val;						// PLL configuration value				
	crt0_pll_feed();
		
	PLLCON = PLLCON_PLLE;         	// Enable PLL
	crt0_pll_feed();
}
			
/*--------------------------------------------------------------------------*/


void PLL_Lock_Wait (void)
{
	while((PLLSTAT & PLLSTAT_PLOCK) == 0)		 // Wait until PLL Locked 
	;
}

/*--------------------------------------------------------------------------*/

void crt0_pll_connect (void)
{
	while(((PLLSTAT & (PLLSTAT_M|PLLSTAT_N))^(PLLCFG_Val)) != 0 )
	;
			
	CCLKCFG = CCLKCFG_Val;
			
	USBCLKCFG = USBCLKCFG_Val;
	
	PLLCON = PLLCON_PLLE|PLLCON_PLLC;         // connect PLL
			
	crt0_pll_feed();
}

/*--------------------------------------------------------------------------*/
			
			
//  Setup Peripheral Clock
void finish_clock_setup (void )
{			
	PCLKSEL0 = PCLKSEL0_Val;
	
	PCLKSEL1 = PCLKSEL1_Val;
	
	PCONP = PCONP_Val;
	
}

/*--------------------------------------------------------------------------*/

void wake_clock_init (void )
{			
	crt0_clock_setup();	
	PLL_Lock_Wait();
	crt0_pll_connect();
	finish_clock_setup();
}
