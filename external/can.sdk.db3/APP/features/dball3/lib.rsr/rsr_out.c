/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_out.c 18996 2013-01-24 22:12:09Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
#include "prog.h"
#include "rf.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Rsr_Out rsr_out[RSR_OUT_COUNT] ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rsr_out_init (void)
{
  #if RSR_DISARM_ENA == 1
    rsr_out[RSR_DISARM_OUT].timeout = timeout_f_create (rsr_opt.unlock.dur	 , rsr_disarm_timeout) ;
  #endif  
  #if RSR_ARM_ENA == 1
    rsr_out[RSR_ARM_OUT].timeout 	= timeout_f_create (rsr_opt.lock.dur	 , rsr_arm_timeout) ;  
  #endif
  #if RSR_UNLOCK_ENA == 1
    rsr_out[RSR_UNLOCK_OUT].timeout = timeout_f_create (rsr_opt.unlock.dur   , rsr_unlock_timeout) ; 
  #endif
  #if RSR_LOCK_ENA == 1
    rsr_out[RSR_LOCK_OUT].timeout 	= timeout_f_create (rsr_opt.lock.dur     , rsr_lock_timeout) ; 
  #endif
  #if RSR_TRUNK_ENA == 1
    rsr_out[RSR_TRUNK_OUT].timeout 	= timeout_f_create (TRUNK_DURATION       , rsr_trunk_timeout) ;
  #endif
  
  rsr_out[RSR_PARKING_OUT].timeout	= timeout_f_create (RSR_PARK_FCT_LENGHT  , rsr_parking_timeout) ;  	  
}

//--------------------------------------------------------------------------//

void rsr_out_action (Rsr_Out_Action action, UInt16 offset, UInt16 on, UInt16 off, UInt8 pulse)
{	
  	if(rsr_out[action].state == RSR_OUT_STATE_RDY && on && pulse && prog_retrieve_option (FEAT_RF_OUTPUT) != NO_RF)
	{	
		rsr_out[action].time.offset = offset ;	
		rsr_out[action].time.set    = on ;
		rsr_out[action].time.clr	= off ;
		rsr_out[action].pulse	    = pulse ;

		rsr_out_ctl (action) ;
	}
}

//--------------------------------------------------------------------------//

Rsr_Out_State rsr_out_ctl (Rsr_Out_Action action)
{
	if(rsr_out[action].state == RSR_OUT_STATE_CLR)
	{
		
			if(--rsr_out[action].pulse)
			{
				rsr_out[action].state =	RSR_OUT_STATE_OFFSET ;
			}
		
	}
	
	switch (++rsr_out[action].state)
	{
		case RSR_OUT_STATE_RDY	 : break ;
		case RSR_OUT_STATE_OFFSET  : timeout_update(rsr_out[action].timeout, rsr_out[action].time.offset) ; break ;
		case RSR_OUT_STATE_SET	 : timeout_update(rsr_out[action].timeout, rsr_out[action].time.set) ; break ;
		case RSR_OUT_STATE_CLR	 : timeout_update(rsr_out[action].timeout, rsr_out[action].time.clr) ; break ;
		case RSR_OUT_STATE_DONE	 : return (rsr_out[action].state = RSR_OUT_STATE_RDY) ;
	}
	
	timeout_start (rsr_out[action].timeout) ;	
	
	return rsr_out[action].state ;
}



