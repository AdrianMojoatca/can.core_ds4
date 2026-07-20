/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_task.c 32362 2015-06-16 18:52:36Z jean-francois.dube $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
#include "wake.h"
#include "status.h"
#include "rf.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void rsr_shutdown     (void) ;
static void rsr_smachine     (void) ;
static void rsr_exit         (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/  

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rsr_task (void * tsk_arg)
{
  rsr_set_state (RSR_STATE_STOP) ;

  for(;;)
    {
	    os_flg_clr (rsr_vars.flg_start) ;
      os_flg_clr (rsr_vars.flg_stop)  ; 

	    os_flg_wait_set (rsr_vars.flg_start, 0) ;

	    rsr_vars.err_code = RSR_NO_ERR ;

	    wake_enter() ;
	   				
		  os_pte_sync (rsr_vars.pte) ;  

		  do	
        {
          rsr_shutdown       () ;
          rsr_vars.callback  () ;
				  rsr_timing         () ;
	      } 
		  while (rsr_vars.state != RSR_STATE_STOP) ;
	    
	  wake_leave() ;
	}
}

//--------------------------------------------------------------------------//

static void rsr_shutdown (void)
{ 
  if(rsr_vars.err_code == RSR_NO_ERR)
    {     
      if (os_flg_state (rsr_vars.flg_stop))
    	{
    	  rsr_vars.err_code   = RSR_ERR_RKE_STOP ;
    	}
      else if ((brake.bit.brake == 1))
    	{ 
    	  rsr_vars.err_code   = RSR_ERR_BRAKE  ;                                         
    	}
	  else if (door.bit.hood)
    	{ 
    	  rsr_vars.err_code   = RSR_ERR_HOOD  ;                                         
    	}
      else if (rsr_vars.valet_mode)
    	{ 
    	  rsr_vars.err_code   = RSR_ERR_MANUAL  ;  // neutral safety code                                       
    	}
      else if (rsr_vars.running.min >= rsr_opt.runtime)
    	{
    	  rsr_vars.err_code   = RSR_ERR_RUNTIME ;
    	}
      // conditional
	    #if STATUS_DTC_ENABLE == 1
        else if(rsr_opt.dtc_check && dtc.count)
          {
		        rsr_vars.err_code   = RSR_ERR_DTC ;
          }
	    #endif
	    #if STATUS_TRANS_ENABLE == 1
        else if (rsr_opt.trans_check && !(trans.bit.park && trans.bit.neutral))
          {
            rsr_vars.err_code   = RSR_ERR_TRANS ;
          }
	    #endif

      if(rsr_vars.err_code != RSR_NO_ERR)
        {                            
          rsr_set_state(RSR_STATE_FAIL) ; 
        }
    }
}   

//--------------------------------------------------------------------------//

void rsr_notify_start (Boolean success)
{

  rf_icon_clr  (ICON_STARTED , FALSE)  ;
  
  rf_sound_set ((Rf_Sound_Def)(success? rsr_opt.runtime : 0), FALSE)  ;

  if(success)
    {
      rf_icon_set  (ICON_TIME    , FALSE)  ;
      rf_icon_set  (ICON_SUCCESS , TRUE)   ;     
    }
  else
    {
      rf_icon_clr  (ICON_SUCCESS , TRUE)  ;
    }
}

