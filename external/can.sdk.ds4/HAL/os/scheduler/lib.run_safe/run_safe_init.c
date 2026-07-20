/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: run_safe_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "run_safe_private.h"
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
static Boolean is_init = FALSE ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void run_safe_init (Run_Safe_Func func)           
{ 
  if(func != NULL)
    {            
        run_safe_entry.func             = func       ;
    }
  else
    {
        run_safe_entry.func             = run_safe_shutdown ;       
    }
    
  run_safe_entry.run_safe_state   = ACTIVE     ;
  run_safe_entry.gwr_state        = NOT_ACTIVE ;
	
  if (!is_init)
	{     
	  run_safe_entry.run_safe_timeout = timeout_f_create (RUN_SAFE_TIMEOUT, run_safe_timeout_elapsed) ;
	  
	  cmd_lut_insert ((Cmd_List)(INPUT_GWR | INPUT_ON) , run_safe_gwr_on  , EXEC_MODE_NORMAL) ;   
	  cmd_lut_insert ((Cmd_List)(INPUT_GWR           ) , run_safe_gwr_off , EXEC_MODE_NORMAL) ;
      
      #if DBG_RUN_SAFE == 1
        dbg_tx_text("\r\nRunSafe Init") ;
      #endif         
	}
  else
    {
      #if DBG_RUN_SAFE == 1
        dbg_tx_text("\r\nRunSafe Already Init") ;
      #endif         
    }
  

}

