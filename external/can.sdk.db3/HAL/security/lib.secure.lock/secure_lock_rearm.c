/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: secure_lock_rearm.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
	  #define ALARM_DISARMED (alarm.bit.equipped && !alarm.bit.armed && !alarm.bit.arming)
	  #define ALARM_UNKNOWN  (!alarm.bit.equipped && !alarm.bit.disarmed)  // if no alarm bit are set ignore alarm state, use lock bit only
	#else
	  #define ALARM_DISARMED FALSE
	  #define ALARM_UNKNOWN  TRUE
	#endif
	
	#if STATUS_DOORLOCK_ENABLE == 1
	  #define DOORS_LOCKED 	(doorlock_status.bit.is_support? doorlock_status.bit.lock : bitfield_rd(rf_data.icon.bit, ICON_LOCK, ICON_LOCK)) 
	#else
	  #define DOORS_LOCKED	(bitfield_rd(rf_data.icon.bit, ICON_LOCK, ICON_LOCK))
	#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void secure_lock_rearm(void * arg)
{
  if(!secure_lock.door_open)
  {
    if(secure_lock.gwr || rpm.word == 0 || !key.bit.ign)
	{		
		switch(prog_retrieve_option (FEAT_SECURE_LOCK))		
	    {
	    	case SECURE_LOCK_NORMAL :
			{
				cmd_q_insert((Cmd_List)(INPUT_LOCK1 | INPUT_ON), COMMAND_INSERT_TIMEOUT);
			}
			break ;
		  #if SECURE_LOCK_SMART_ENABLE == 1 
	      	case SECURE_LOCK_SMART : 
			{			
				#if DBG_SECURE_LOCK == 1
				  dbg_tx_text ("\nSL: rearm func") ;
				  if(ALARM_UNKNOWN)
				  {
				  	dbg_tx_text ("\nSL: alarm unkown!!!") ;
					dbg_tx_text ("\nSL: doorlock status:") ;
					dbg_tx_u (DOORS_LOCKED, 1) ;
				  }
				  else
				  {
				  	dbg_tx_text ("\nSL: alarm status:") ;
					dbg_tx_u (!ALARM_DISARMED, 1) ;
					dbg_tx_text ("\nSL: doorlock status:") ;
					dbg_tx_u (DOORS_LOCKED, 1) ;
				  }
				#endif
								
				if(DOORS_LOCKED && (ALARM_DISARMED || (ALARM_UNKNOWN && secure_lock.need_arm)))
				{			
					ATOMIC(secure_lock.need_arm = FALSE) ;

					cmd_q_insert(INPUT_SECURE_ARM, COMMAND_INSERT_TIMEOUT);	

					#if DBG_SECURE_LOCK == 1
					  dbg_tx_text ("\nSL: do arm") ;
					#endif
				}
			}
			break ;
		  #endif
	    }			
	}  
  }  
  else
  {
    #if DBG_SECURE_LOCK == 1
      dbg_tx_text ("\nSL: Rearm FAIL Door Open") ;
    #endif  
  }  

}
#undef DOORS_LOCKED
#undef ALARM_DISARMED
#undef ALARM_UNKNOWN
