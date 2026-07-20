/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_start.c 33314 2015-08-21 13:22:01Z simon.demers $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
#include "prog.h"
#include "led_usr.h"
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

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void rsr_start_toggle (void * arg)
{
  if(os_flg_state (rsr_vars.flg_start))
    {
      rsr_stop (0) ;
    }
  else
    {
      rsr_start(0) ;
    }
}

void rsr_start (void * arg)
{
  Boolean idle_mode_detected_but_disable = FALSE ;  
  
  if (!rsr_vars.gwr_is_detect && prog_retrieve_option (FEAT_RF_OUTPUT) == NO_RF)
    {
      prog_set_option(RF_TD, FEAT_RF_OUTPUT) ;
    }
  
  if (prog_retrieve_option (FEAT_RF_OUTPUT) == NO_RF)
    {
      led_usr_op_err(RSR_DISPLAY_ERR_RFTD_NOT_ENA ,1) ;
    }
  else
    {      

       #if RSR_IDLE_MODE_ENABLE == 0
      
         if((rpm.word > 300))
         {
           idle_mode_detected_but_disable = TRUE  ; 
         }
      
       #endif
         
         
      if(!idle_mode_detected_but_disable)
      {     
        rsr_input_init()                     ;  //re-read the inputs in case mode changed changed
       
        rf_icon_clr  (ICON_STARTED  , FALSE)  ;
     
        rf_icon_set  (ICON_TIME     , FALSE)  ;
        rf_icon_set  (ICON_SUCCESS , FALSE)   ;
        
        rsr_opt.runtime = DEFAULT_RSR_RUNTIME ;
      
        if (rsr_feature_enabled(RS_FEAT_REMOTE_START_RUNTIME))
        {
          rsr_opt.runtime = rsr_feature_get(RS_FEAT_REMOTE_START_RUNTIME);
        }
        
        rsr_vars.runtime_remaining = rsr_opt.runtime  ;  
        
        
        
        rf_sound_set ((Rf_Sound_Def)rsr_opt.runtime, TRUE)  ;

        os_flg_clr (rsr_vars.flg_stop)  ;
        os_flg_set (rsr_vars.flg_start) ;
        
        
      }
    }
}


