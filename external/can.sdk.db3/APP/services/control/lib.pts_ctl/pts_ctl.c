/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: pts_ctl.c 32500 2015-06-29 19:12:39Z jean-francois.dube $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "obd_can.h"
#include "status.h"
#include "led_usr.h"
#include "pts_ctl.h"

#include "pts_ctl_init.c "
#include "pts_ctl_dbg_print.c"
#include "pts_ctl_vars.c     " 

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static UInt8 obd_can_status_count = NULL ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void pts_ctl_ena        (void) 
{
  pts_ctl_vars.is_disabled = TRUE ;

  #if DBG_PTS_CTL == 1
    pts_ctl_dbg_print ("\nPts_Ctl Ena" , NULL , NULL) ; 
  #endif     
}
//--------------------------------------------------------------------------//
void pts_ctl_dis        (void)
{
  pts_ctl_vars.is_disabled = FALSE ;
  
  #if DBG_PTS_CTL == 1
    pts_ctl_dbg_print ("\nPts_Ctl Dis" , NULL , NULL) ; 
  #endif     
  
  
}
//--------------------------------------------------------------------------//
void pts_ctl_obd_status_timeout(void * arg)
{
  #if DBG_PTS_CTL == 1
    pts_ctl_dbg_print ("\nPts_Ctl Timeout Count= " , &obd_can_status_count , 1) ; 
  #endif    
 if(++obd_can_status_count < 5)
   {
     pts_ctl_on(arg) ; 
   }  
 else
  {
    led_usr_op_err(LED_USR_PTS_CTL_DISABLE , 5) ;      
  } 
}
//--------------------------------------------------------------------------//
void pts_ctl_on(void * arg)
{   
  Obd_Can_Req_Status obd_status = obd_can_ctrl_request_status(OBD_CAN_SPEED) ;

  if(obd_status == OBD_CAN_SUCCESS)
    {
      if(!pts_ctl_vars.is_disabled)
        {
          if(speed.bit.is_detected || (speed.byte != NULL))
            {
              #if DBG_PTS_CTL == 1
                pts_ctl_dbg_print ("\nPts_Ctl ON Speed Detect" , NULL , NULL) ; 
              #endif   
            }
          else if(pts_ctl_vars.func_on && pts_ctl_vars.func_off)
            {
              if( timeout_test(pts_ctl_vars.timeout_pts_pulse))
                {
                  pts_ctl_vars.func_on() ;
			            timeout_start(pts_ctl_vars.timeout_pts_pulse) ;
			            timeout_start(pts_ctl_vars.timeout_pts) ;
                  #if DBG_PTS_CTL == 1
                    pts_ctl_dbg_print ("\nPts_Ctl ON OK" , NULL , NULL) ; 
                  #endif 
                }
                
                #if DBG_PTS_CTL == 1
                else
                  {
                    pts_ctl_dbg_print ("\n***Pts_Ctl Timeout Pulse Not Over" , NULL , NULL) ; 
                  }
                #endif 

              return ;
            }
          else
            {
              #if DBG_PTS_CTL == 1
                pts_ctl_dbg_print ("\nPts_Ctl ON  FAIL: No Func" , NULL , NULL) ; 
              #endif              
            }             
        }
      else
        {
          #if DBG_PTS_CTL == 1
            pts_ctl_dbg_print ("\nPts_Ctl ON  FAIL: Is Disable" , NULL , NULL) ; 
          #endif          
        }
    }
    else
    {
       timeout_start(pts_ctl_vars.timeout_obd_status) ;
	          
	   #if DBG_PTS_CTL == 1
          pts_ctl_dbg_print ("\nPts_Ctl ON  FAIL: OBD Status= " , &obd_status , 1) ; 
        #endif    
     }

}
//--------------------------------------------------------------------------//
void pts_ctl_off(void * arg)
{
  timeout_stop(pts_ctl_vars.timeout_pts) ;

  if(!pts_ctl_vars.is_disabled)
    {
      if(pts_ctl_vars.func_off && pts_ctl_vars.func_on)
        {
          pts_ctl_vars.func_off() ;

          #if DBG_PTS_CTL == 1
            pts_ctl_dbg_print ("\nPts_Ctl Off" , NULL , NULL) ; 
          #endif   
          
          return ;
        }
      else
        {
          #if DBG_PTS_CTL == 1
            pts_ctl_dbg_print ("\nPts_Ctl Off FAIL: No Func" , NULL , NULL) ; 
          #endif              
        }
    }
  else
    {
      #if DBG_PTS_CTL == 1
        pts_ctl_dbg_print ("\nPts_Ctl OFF FAIL: Is Disable" , NULL , NULL) ; 
      #endif          
    }
}
//--------------------------------------------------------------------------//
void pts_ctl_pts_timeout(void * arg)
{
  #if DBG_PTS_CTL == 1
    pts_ctl_dbg_print ("\nPts_Ctl Pts TIMEOUT" , NULL , NULL) ; 
  #endif  
  
  pts_ctl_off(arg) ;
}
//--------------------------------------------------------------------------//
void pts_ctl_reg_func         (Pts_Ctl_Cmd cmd, Pts_Ctl_Func func)
{
  switch (cmd)
    {
      case PTS_CTL_CMD_ON  : ATOMIC(pts_ctl_vars.func_on  = func); break ;
      case PTS_CTL_CMD_OFF : ATOMIC(pts_ctl_vars.func_off = func); break ;
    }   

  #if DBG_PTS_CTL == 1
  switch (cmd)
    {
      case PTS_CTL_CMD_ON  : pts_ctl_dbg_print ("\nPts_Ctl Reg Func ON  SET" , NULL , NULL) ; break ;
      case PTS_CTL_CMD_OFF : pts_ctl_dbg_print ("\nPts_Ctl Reg Func OFF SET" , NULL , NULL) ; break ;
    } 
  #endif     
    
}
//--------------------------------------------------------------------------//
void pts_ctl_request (void * arg)
{
  Obd_Can_Req_Status obd_status = obd_can_ctrl_request_status(OBD_CAN_SPEED) ;
 
  if(obd_status == OBD_CAN_ONGOING)
    {
      #if DBG_PTS_CTL == 1
        pts_ctl_dbg_print ("\nPts_Ctl Obd ongoing..." , NULL , NULL) ; 
     #endif 
    }
	else  
	{     
      obd_can_tx(OBD_CAN_SPEED, OBD_CAN_REQ_TYPE_ON_DEMAND, FALSE) ;

      #if DBG_PTS_CTL == 1
        pts_ctl_dbg_print ("\nPts_Ctl Request" , NULL , NULL) ; 
      #endif 
	}
   
  timeout_update(pts_ctl_vars.timeout_obd_req , PTS_CTL_REQ_TIMEOUT_INIT) ;
  timeout_start (pts_ctl_vars.timeout_obd_req)                            ;
}
//--------------------------------------------------------------------------//
void pts_ctl_request_stop (void * arg)
{
  timeout_stop(pts_ctl_vars.timeout_obd_req) ;

  #if DBG_PTS_CTL == 1
    pts_ctl_dbg_print ("\nPts_Ctl Request Stop" , NULL , NULL) ; 
  #endif 

}
//--------------------------------------------------------------------------//
void pts_ctl_gwr_on   (void * arg)
{
  if(!pts_ctl_vars.gwr)
    {
      pts_ctl_vars.gwr  = TRUE ;
     
      obd_can_status_count  = 0 ;
      
      speed.bit.is_detected = FALSE ;
      
      pts_ctl_request     (arg) ;
      pts_ctl_request_stop(arg) ;
     
      #if DBG_PTS_CTL == 1
        pts_ctl_dbg_print ("\nPts_Ctl Gwr On" , NULL , NULL) ; 
      #endif      
  }
}
//--------------------------------------------------------------------------//
void pts_ctl_gwr_off   (void * arg)
{
  if(pts_ctl_vars.gwr)
    {
      pts_ctl_vars.gwr      = FALSE ;
          
      #if DBG_PTS_CTL == 1
        pts_ctl_dbg_print ("\nPts_Ctl Gwr Off" , NULL , NULL) ; 
      #endif      
  }
}
//--------------------------------------------------------------------------//
void pts_ctl_speed_set   (void * arg)
{  
//  if(pts_ctl_vars.gwr)
//    {
//      pts_ctl_vars.speed_detect = speed.byte ;
//    }
  
  #if DBG_PTS_CTL == 1
    pts_ctl_dbg_print ("\nPts_Ctl Speed Set Detect= " , &speed.byte , 1) ; 
  #endif  

}
//--------------------------------------------------------------------------//
void pts_ctl_speed_clr   (void * arg)
{
  
  #if DBG_PTS_CTL == 1
    pts_ctl_dbg_print ("\nPts_Ctl Speed Clr" , NULL , NULL) ; 
  #endif  
}

