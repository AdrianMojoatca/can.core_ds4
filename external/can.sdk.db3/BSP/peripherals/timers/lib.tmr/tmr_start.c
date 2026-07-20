/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tmr_start.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/
/* UInt8 tmr_start (Tmr tmr, UInt32 tick_count, Boolean use_fiq_tc)
   
   Description: Will set the timer with tick_count valye and enable it
                Parameters: tmr: Timer variable
				   		    tick_count: timeout in cycles for tmr 
						    use_fiq_tc: tmr called from fiq or irq service
			                  		    1: from fiq
									    0: from irq	  
	Return: 1 if timer update was successfull
		    0  if timer update to cycles after delta subtraction is less than MIN_CYCLES_TMR_UPDATE
		    		   			   
   UInt8 tmr_start_fiq_rx (Tmr tmr, UInt32 tick_count)
   
   Description: (to be always called from fiq_service and used to remove 2*delta during recieve sequence
                Will set the timer with tick_count valye and enable it
                Parameters: tmr: Timer variable
				   		    tick_count: timeout in cycles for tmr 
	Return: 1 if timer update was successfull
		   0  if timer update to cycles after delta subtraction is less than MIN_CYCLES_TMR_UPDATE
*/
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "tmr_private.h"
#include "fiq.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
// value of 133 cycles for Level1 optimization and 114 cycles for level 3 optimization measured
// with DBALL HW ver 3.53. Hence 2*CPU_CLOCK(72) = 144 was chosen as a safe minimum value
#define MIN_CYCLES_TMR_UPDATE	 144  

// 1 usec (1*72 = 72) , resolution of tmr_update to be removed from tick_count
#define CYCLES_RESOLUTION	 72  


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
UInt8 tmr_start (Tmr tmr, UInt32 tick_count, Boolean use_fiq_tc)
{
    UInt8 ret_val = 1;
	UInt32 fiqtc = fiq_get_tc(FIQ_TC_TIMER_1);
	SInt32  delta;
	UInt32 tic;
	if (use_fiq_tc)
	{
		tic = T1TC;
        // to check is timer variable has a wraparound after 0xFFFFFFFF
		if( tic < fiqtc )
		{
			delta = tick_count - (0xFFFFFFFF - fiqtc + tic) - CYCLES_RESOLUTION;
		}
		else
		{
			delta = tick_count -(tic - fiqtc) - CYCLES_RESOLUTION;
		}
	}
	else
	{
		delta = tick_count;
	}
	// minimum value of TxMR to be set for timer to trigger is 144 cycles
	if(delta < MIN_CYCLES_TMR_UPDATE)
	{
		delta = MIN_CYCLES_TMR_UPDATE;
		ret_val = 0;
	}
	TxMR = TxTC + delta;
	TxIR = (1u << tmr->tmr_index) ;
 	TxMCR |= (1u << (tmr->tmr_index * 3)) ;	

	return ret_val;
}
UInt8 tmr_start_fiq_rx (Tmr tmr, UInt32 tick_count)
{
    UInt8 ret_val = 1;
	UInt32 fiqtc = fiq_get_tc(FIQ_TC_TIMER_1);
	SInt32  delta;
	UInt32 tic;
	tic = T1TC;
    // to check is timer variable has a wraparound after 0xFFFFFFFF
	if( tic < fiqtc )
	{
		delta = tick_count - 2 * (0xFFFFFFFF - fiqtc + tic)  - CYCLES_RESOLUTION;
	}
	else
	{
		delta = tick_count - 2 * (tic - fiqtc) - CYCLES_RESOLUTION;
	}
	// minimum value of TxMR to be set for timer to trigger is 144 cycles
	if(delta < MIN_CYCLES_TMR_UPDATE)
	{
		delta = MIN_CYCLES_TMR_UPDATE;
		ret_val = 0;
	}
	TxMR = TxTC + delta;
	TxIR = (1u << tmr->tmr_index) ;
 	TxMCR |= (1u << (tmr->tmr_index * 3)) ;	

	return ret_val;
}
