/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_state_ign_on.c 24015 2013-12-17 22:09:54Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
#include "status.h"
#include "cmd.h"

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
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void rsr_state_ign_on(void)
{
  UInt32 ign_to_start_delay = RSR_IGN_TO_START_TIMEOUT ;

  if(rsr_feature_enabled(RS_FEAT_DIESEL_START_DELAY))
	{
	  ign_to_start_delay += rsr_feature_get(RS_FEAT_DIESEL_START_DELAY) * 1000; // msec timebase
	}

  if(RSR_ENGINE_RUNNING || rsr_vars.idle_mode)
    {  
      rsr_parking_pattern (RSR_PRK_CONSTANT) ;
      
      rsr_set_state   (RSR_STATE_RUN) ;

      rsr_notify_start(TRUE) ;
    }

  #define CMD_INSERT (cmd_q_insert (INPUT_RS_ACCESSORY, RSR_CMD_INSERT_TIMEOUT) && cmd_q_insert (INPUT_RS_STARTER | INPUT_ON, RSR_CMD_INSERT_TIMEOUT))
   else if((RSR_CUR_TIME >= ign_to_start_delay) && CMD_INSERT) 
    {
      rsr_parking_pattern (RSR_PRK_STOP) ;

	  #if RSR_ACC_ENA == 1
	    RSR_ACC_CLR ;
      #endif
	  #if RSR_STARTER_ENA == 1
        ;
      #endif  
      rsr_set_state   (RSR_STATE_START)    ;
    } 
  #undef CMD_INSERT
}


