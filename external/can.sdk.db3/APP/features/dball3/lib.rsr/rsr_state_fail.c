/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_state_fail.c 32362 2015-06-16 18:52:36Z jean-francois.dube $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
#include "cmd.h"
#include "rf.h"
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
void rsr_state_fail(void)
{
  static UInt8 retry_count ;

  Rsr_State last_state = rsr_get_last_state() ;

  rsr_vars.idle_mode = FALSE ; 

  rsr_vars.runtime_remaining = 0 ; 
  
  #if RSR_STARTER_ENA == 1
    ;
  #endif
  #if RSR_ACC_ENA == 1
    RSR_ACC_CLR   ;
  #endif
  #if RSR_IGN_ENA == 1
    RSR_IGN_CLR   ;
  #endif

  if (last_state == RSR_STATE_START)
	  {
      while(!cmd_q_insert (INPUT_RS_STARTER , RSR_CMD_INSERT_TIMEOUT)) ;	// starter off 
	  }
  
  if (last_state != RSR_STATE_GWR)
	  {
	    while(!cmd_q_insert (INPUT_RS_IGNITION , RSR_CMD_INSERT_TIMEOUT)) ;	// ign off
	    while(!cmd_q_insert (INPUT_RS_ACCESSORY, RSR_CMD_INSERT_TIMEOUT)) ;	// acc off
	  }
	
  while(!cmd_q_insert     (INPUT_GWR, RSR_CMD_INSERT_TIMEOUT)) ;	// gwr off, no delay for quick disabling of DAC output
  
  if (rsr_feature_enabled(RS_FEAT_START_ATTEMPTS))
    {
	    rsr_opt.retry_count = rsr_feature_get(RS_FEAT_START_ATTEMPTS);
    }
  if ((rsr_vars.err_code == RSR_ERR_NO_RPM) && (++retry_count < rsr_opt.retry_count))
    {      
      rsr_vars.err_code = RSR_NO_ERR ;
      
      os_tsk_wait(rsr_opt.retry_timeout * RETRY_TIMEBASE) ;
      
      rsr_parking_pattern (RSR_PRK_CONSTANT) ;

      rsr_set_state (RSR_STATE_DISARM)  ;

      return ;
    }

  retry_count = 0 ;
 
  rsr_parking_pattern (RSR_PRK_STOP) ;
  
  rsr_err_add    (rsr_vars.err_code) ;

  rsr_err_save   () ;

  rf_sound_set   ((Rf_Sound_Def)0, FALSE)  ;
  rf_icon_set  	 (ICON_TIME    , FALSE)  ;
  rf_icon_clr    (ICON_SUCCESS , TRUE)  ;

  rsr_set_state  (RSR_STATE_ARM) ;

  timeout_start  (rsr_vars.fail_timeout) ;
}

//--------------------------------------------------------------------------//

void rsr_fail_timeout(void * arg)
{
  if(rpm.word == 0)
    {
      rsr_parking_flash(RSR_PARK_ERR_TIMEOUT, rsr_vars.err_code) ;
    }
}
