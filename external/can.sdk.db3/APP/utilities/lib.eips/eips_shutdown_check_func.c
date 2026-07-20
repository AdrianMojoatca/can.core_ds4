/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: eips_shutdown_check_func.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "eips_private.h"
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
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void eips_shutdown_check_func (void * arg)
{
  if(eips_vars.check_timeout == EIPS_ENGINE_START_CHECK_TIMEOUT)
    {
      if(eips_vars.rpm_status) //i can move to previous check but i want to make sure   every check is valid
        {           
          ATOMIC(eips_vars.fail_status = TRUE) ;
        
          cmd_q_insert((Cmd_List)(INPUT_EIPS_SHUTDOWN_FAIL | INPUT_ON), COMMAND_INSERT_TIMEOUT);
        
          eips_vars.check_timeout = EIPS_ENGINE_STOP_CHECK_TIMEOUT ;
        
          timeout_update(eips_vars.timeout_shutdown_check , eips_vars.check_timeout * EIPS_SEC) ;
        
          timeout_start (eips_vars.timeout_shutdown_check) ;
        
          #if DBG_EIPS == 1
            dbg_tx_text("\r\n\nEIPS 30 Sec Fail Timeout Start...")  ;
          #endif    
        }
    }
  else
    {
      cmd_q_insert(INPUT_EIPS_SHUTDOWN_FAIL, COMMAND_INSERT_TIMEOUT);
    
      #if DBG_EIPS == 1
        dbg_tx_text("\r\n\nEIPS 30 Sec Fail Timeout Done")  ;
      #endif    
    }  
}

