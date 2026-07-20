/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: secure_lock_starter.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "secure_lock_private.h"
#include "prog.h"
#include "cmd.h"
#include "status.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Boolean secure_lock_starter ; 

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void secure_lock_starter_on( void *arg )
{
	if(secure_lock.gwr && !secure_lock_starter)
	{
		secure_lock_starter = TRUE ;
        
        #if DBG_SECURE_LOCK == 1
          dbg_tx_text ("\nSL: Starter On") ;
        #endif	        
        
	}	
}
//--------------------------------------------------------------------------//
void secure_lock_starter_off( void *arg )
{
	if(secure_lock_starter)
	{
		secure_lock_starter = FALSE ;

		switch(prog_retrieve_option (FEAT_SECURE_LOCK))		
	    {
	    	case SECURE_LOCK_NORMAL :
			{
				timeout_start(secure_lock.starter_timeout);//cmd_q_insert((Cmd_List)(INPUT_LOCK1 | INPUT_ON), COMMAND_INSERT_TIMEOUT);
			}
			break ;
		  #if SECURE_LOCK_SMART_ENABLE == 1 
	      	case SECURE_LOCK_SMART : 
			{					
				timeout_start(secure_lock.starter_timeout);
                timeout_update(secure_lock.timeout, SECURE_REARM_RUN_TIMEOUT);	
				timeout_start (secure_lock.timeout) ;	
			}
			break ;
		  #endif
	    }
        #if DBG_SECURE_LOCK == 1
          dbg_tx_text ("\nSL: Starter Off") ;
        #endif

    }	
}
//--------------------------------------------------------------------------//
void secure_lock_starter_timeout(void * arg)
{
  if(!secure_lock.door_open)
    {
      if (secure_lock.gwr)
      {
        cmd_q_insert((Cmd_List)(INPUT_LOCK1 | INPUT_ON), COMMAND_INSERT_TIMEOUT);
        
        #if DBG_SECURE_LOCK == 1
          dbg_tx_text ("\nSL: Starter Timeout Lock") ;
        #endif     
      }     
    }
  else
  {
    #if DBG_SECURE_LOCK == 1
      dbg_tx_text ("\nSL: Starter Timeout FAIL Door Open") ;
    #endif      
  }

 
}
