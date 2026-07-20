/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: pts_ctl_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "pts_ctl_private.h"
#include "cmd.h"
#include "timeout_f.h"
#include "core_contract_db3_pts_ctl_config.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define PTS_CTL_OBD_STATUS_TIMEOUT 100  // MS
#define PTS_CTL_REQ_TIMEOUT_INIT        (OBD_CAN_IGN_DELAY * 1000) + ( 2 * 1000) // obd delay + 2 sec
#define PTS_CTL_REQ_TIMEOUT_LOOP        (1 * 1000) 
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void pts_ctl_init (void)
{
 static const Cmd_Array1 cmd_array[] = 
   {
     {INPUT_GWR_ON                  , pts_ctl_gwr_on        } , 
     {INPUT_GWR_OFF                 , pts_ctl_gwr_off       } ,
     {INPUT_IGNITION  | INPUT_ON    , pts_ctl_request       } , 
     {INPUT_IGNITION                , pts_ctl_request_stop  } ,

     {INPUT_PTS_ON                  , pts_ctl_on            } ,
     {INPUT_PTS_OFF                 , pts_ctl_off           } ,     
     {INPUT_BRAKE_STATUS_PRESS      , pts_ctl_request       } ,
     {INPUT_BRAKE_STATUS_RELEASE    , pts_ctl_request_stop  } ,    
     {INPUT_SPEED_SET               , pts_ctl_speed_set     } ,
     {INPUT_SPEED_CLR               , pts_ctl_speed_clr     } , 
    
   } ;

  
  memset(&pts_ctl_vars , NULL , sizeof(Pts_Ctl_Vars)) ;
   
  #if DBG_PTS_CTL==1 
    pts_ctl_dbg_init() ;
  #endif   
  
  if(obd_can_config_set_enabled(OBD_CAN_SPEED , TRUE)) 
    {
      pts_ctl_vars.timeout_pts_pulse  = timeout_create((UInt32)PTS_CTL_CFG_PTS_CTL_PULSE_TIMEOUT) ;
      pts_ctl_vars.timeout_pts        = timeout_f_create((UInt32)PTS_CTL_CFG_PTS_CTL_TIMEOUT, pts_ctl_pts_timeout) ; 
      pts_ctl_vars.timeout_obd_status = timeout_f_create(PTS_CTL_OBD_STATUS_TIMEOUT , pts_ctl_obd_status_timeout) ; 
      pts_ctl_vars.timeout_obd_req    = timeout_f_create(PTS_CTL_REQ_TIMEOUT_INIT   , pts_ctl_request) ; 
      
  
      CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);  

      #if DBG_PTS_CTL == 1
        pts_ctl_dbg_print ("\nPts_Ctl Init OK" , NULL , NULL) ; 
      #endif  
    } 
  else
    {
      #if DBG_PTS_CTL == 1
        pts_ctl_dbg_print ("\nPts_Ctl Init Fail" , NULL , NULL) ; 
      #endif  

    }

}
