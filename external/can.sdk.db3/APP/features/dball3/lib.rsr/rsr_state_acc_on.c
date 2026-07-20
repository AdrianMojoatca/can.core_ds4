/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_state_acc_on.c 23620 2013-11-15 14:12:00Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
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

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void rsr_state_acc_on(void)
{
	if((RSR_ENGINE_RUNNING || (TACHLESS && rsr_vars.ign) || RSR_CUR_TIME >= RSR_ACC_TO_IGN_TIMEOUT) && cmd_q_insert (INPUT_RS_IGNITION | INPUT_ON, RSR_CMD_INSERT_TIMEOUT))//if(rsr_vars.sec >= RSR_GWR_ON_DELAY && cmd_q_insert (INPUT_IGNITION | INPUT_ON, RSR_CMD_INSERT_TIMEOUT))
    {             
	  #if RSR_IGN_ENA == 1
	    RSR_IGN_SET ;
	  #endif

      rsr_set_state   (RSR_STATE_IGN) ;  
	  
	  if(rsr_feature_get(RS_FEAT_DIESEL_START_DELAY))
	    {
	  	  rsr_notify_start (TRUE) ; // give start notification now to avoid timeout on start confirmation
		}	
    }
}


