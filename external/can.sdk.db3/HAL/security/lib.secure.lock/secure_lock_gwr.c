/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: secure_lock_gwr.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "secure_lock_private.h"
#include "cmd.h"
#include "status.h"
#include "prog.h"
#include "bitfield.h"
#include "rf.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if STATUS_ALARM_ENABLE == 1
  #define ALARM_ARMED   (alarm.bit.equipped && alarm.bit.armed || alarm.bit.arming) 
  #define ALARM_UNKNOWN (!alarm.bit.equipped && !alarm.bit.disarmed)
#else
  #define ALARM_ARMED   FALSE
  #define ALARM_UNKNOWN TRUE
#endif

#define DOORS_LOCKED    (bitfield_rd(rf_data.icon.bit, ICON_LOCK, ICON_LOCK))

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void secure_lock_gwr_on( void *arg )
{	
	if (!secure_lock.gwr)
	{
		ATOMIC(secure_lock.gwr = TRUE) ;

		timeout_stop (secure_lock.timeout) ;

        #if DBG_SECURE_LOCK == 1
          dbg_tx_text ("\nSL: Gwr On") ;
        #endif
    
		if (rpm.word == 0 || !key.bit.ign)
		{
			switch(prog_retrieve_option (FEAT_SECURE_LOCK))		
		    {
		    	case SECURE_LOCK_NORMAL :
				{
					cmd_q_insert((Cmd_List)(INPUT_UNLOCK_DRIVER1 | INPUT_ON), COMMAND_INSERT_TIMEOUT);
				}
				break ;
			  #if SECURE_LOCK_SMART_ENABLE == 1 
		      	case SECURE_LOCK_SMART : 
				{
					#if DBG_SECURE_LOCK == 1
					  dbg_tx_text ("\nSL: disarm func") ;
					  if(ALARM_UNKNOWN)
					  {					 
					  	dbg_tx_text ("\nSL: alarm unkown!!!") ;
						dbg_tx_text ("\nSL: doorlock status:") ;
						dbg_tx_u (DOORS_LOCKED, 1) ;
					  }
					  else
					  {
					  	dbg_tx_text ("\nSL: alarm status:") ;
						dbg_tx_u (ALARM_ARMED, 1) ;
					  }
					#endif
					  	
					if(ALARM_ARMED || (ALARM_UNKNOWN && DOORS_LOCKED))
					{
						cmd_q_insert(INPUT_SECURE_DISARM, COMMAND_INSERT_TIMEOUT);
						ATOMIC(secure_lock.need_arm = TRUE) ;
						
						#if DBG_SECURE_LOCK == 1
						  dbg_tx_text ("\nSL: do disarm") ;
						#endif	
					}
				}
				break ;
			  #endif
		    }	
		}
	}	
}

#undef ALARM_ARMED	
#undef ALARM_UNKNOWN
#undef DOORS_LOCKED
//--------------------------------------------------------------------------//
void secure_lock_gwr_off( void *arg )
{    
    if (secure_lock.gwr)
	{
        timeout_stop(secure_lock.starter_timeout);

        #if DBG_SECURE_LOCK == 1
          dbg_tx_text ("\nSL: Gwr Off") ;
        #endif		
		secure_lock.gwr = FALSE ;
		secure_lock.need_arm = TRUE ;
		
		switch(prog_retrieve_option (FEAT_SECURE_LOCK))		
		 {
		 	case SECURE_LOCK_NORMAL :
			{
				timeout_update(secure_lock.timeout, SECURE_REARM_TIMEOUT_NORMAL);
			}
			break ;
		  #if SECURE_LOCK_SMART_ENABLE == 1 
		   	case SECURE_LOCK_SMART : 
			{
				timeout_update(secure_lock.timeout, SECURE_REARM_TIMEOUT_SMART);
			}
			break ;
		  #endif
		 }
				
		timeout_start (secure_lock.timeout) ;
	}
}
