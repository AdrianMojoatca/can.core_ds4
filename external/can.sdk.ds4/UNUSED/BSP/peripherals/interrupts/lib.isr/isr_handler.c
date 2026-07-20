/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: isr_handler.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
// fast interrupt handling vectors
extern UInt32 const isr_fast_0[];

// Slow interrupt handling vectors
extern UInt32 const isr_slow_0[];

PVF isr_user_func_vector;
PVF isr_user_func[32];

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void isr_set_vector (Isr_Number vic_channel , Vic_Isr_Pri isr_pri , Isr_Type isr_type , PVF isr_func_addr)
{
	UInt32 vic_chan_bit = (1ul << vic_channel) ;
	VICIntSelect &= ~ vic_chan_bit ;               //  ISR mode select 
	
	(& VICVectPriority0) [vic_channel] = isr_pri;  // Setup priority of this ISR (0 to 15)
	isr_user_func[vic_channel] = isr_func_addr ;  // copy user_func address to array in RAM 
		
	// Passing address of slow or fast ISR in VICVectAddr0-31
	if(isr_type == ISR_TYPE_IRQ)
	{
		(& VICVectAddr0 ) [vic_channel] = (UInt32)(isr_slow_0 + (vic_channel*3)) ;  // Pass address of slow interrupt vector in to VIC slot 
	}
	else
	{
		(& VICVectAddr0 ) [vic_channel] = (UInt32)(isr_fast_0 + (vic_channel*3)) ;  // Pass address of fast interrupt vector in to VIC slot 
	}
	
	VICIntEnable |= vic_chan_bit ;                     // Enable the ISR source
}


//--------------------------------------------------------------------------//

void isr_set_fiq_channel( Isr_Number vic_channel )
{
	UInt32 vic_chan_bit = (1ul << vic_channel) ;
	
  VICIntSelect |=   vic_chan_bit ;               // FISR mode select

	(& VICVectPriority0) [vic_channel] = ISR_PRI_0   ;  // Setup priority of this FIQ to 0

  VICIntEnable |= vic_chan_bit ;                     // Enable the FIQ source
}

//--------------------------------------------------------------------------//

