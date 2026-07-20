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


#include "pts_ctl_private.h"
#include "q.h"

#include "cmd.h"
#include "timeout_f.h"
#include "config_pts_ctl.h"
#include "obd_can.h"
#include <string.h>


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef PTS_CTL_TIMEOUT
  #define  PTS_CTL_TIMEOUT 750  // ms
#endif
#ifndef PTS_CTL_PULSE_TIMEOUT
  #define  PTS_CTL_PULSE_TIMEOUT 2000  // ms
#endif

#define PTS_CTL_OBD_STATUS_TIMEOUT 100  // MS
#define PTS_CTL_REQ_TIMEOUT_INIT        (OBD_CAN_IGN_DELAY * 1000) + ( 2 * 1000) // obd delay + 2 sec
#define PTS_CTL_REQ_TIMEOUT_LOOP        (1 * 1000) 

#define TASK_PTS_CTL_DBG_STK   0x0180
#define TASK_PTS_CTL_DBG_NAME  "Pts_Ctl_Dbg"
#define TASK_PTS_CTL_DBG_PRI   100

#define PTS_CTL_DBG_Q_SIZE     20

//--------------------------------------------------------------------------//

typedef struct s_Pts_Ctl_Dbg_Q_Msg
{ 
  Q_Msg         q_base    ;
    
  StrConst    * text  ;
  
  UInt8 data[50]  ;
  
  UInt8 size    ;

} Pts_Ctl_Dbg_Q_Msg ;

//--------------------------------------------------------------------------//

typedef struct
{
  Q    q_free               ;
  Q    q_pending            ; 

  Tsk  tsk                  ;       
 
}Pts_Ctl_Dbg_Vars ;


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void pts_ctl_dbg_task(void * arg) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static UInt8 obd_can_status_count = NULL ;
static Pts_Ctl_Dbg_Vars pts_ctl_dbg_vars ;
Pts_Ctl_Vars pts_ctl_vars ;


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
     {INPUT_HANDBRAKE_STATUS_ON     , pts_ctl_request       } ,
     {INPUT_HANDBRAKE_STATUS_OFF    , pts_ctl_request_stop  } ,     
     {INPUT_SPEED_SET               , pts_ctl_speed_set     } ,
     {INPUT_SPEED_CLR               , pts_ctl_speed_clr     } ,
    
   } ;

  
  memset(&pts_ctl_vars , NULL , sizeof(Pts_Ctl_Vars)) ;
   
  #if DBG_PTS_CTL==1 
    pts_ctl_dbg_init() ;
  #endif   
  
  if(obd_can_config_set_enabled(OBD_CAN_SPEED , TRUE)) 
    {
      pts_ctl_vars.timeout_pts_pulse  = timeout_create(PTS_CTL_PULSE_TIMEOUT) ;
      pts_ctl_vars.timeout_pts        = timeout_f_create(PTS_CTL_TIMEOUT            , pts_ctl_pts_timeout) ; 
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

void pts_ctl_dbg_init(void)
{
  #if DBG_PTS_CTL == 1

  UInt8 n ;
  
  pts_ctl_dbg_vars.q_free    = q_create () ;
  pts_ctl_dbg_vars.q_pending = q_create () ;

  pts_ctl_dbg_vars.tsk = os_tsk_create (pts_ctl_dbg_task , TASK_PTS_CTL_DBG_PRI , TASK_PTS_CTL_DBG_STK , 0 , TASK_PTS_CTL_DBG_NAME) ;

  for (n = 0 ; n < PTS_CTL_DBG_Q_SIZE ; n++)
    {
      Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Pts_Ctl_Dbg_Q_Msg)) ;
      
      q_assign (pts_ctl_dbg_vars.q_free , q_msg) ;
      q_insert (pts_ctl_dbg_vars.q_free , q_msg) ;
    }
  
  os_tsk_start (pts_ctl_dbg_vars.tsk, 0) ;
  
  #endif
}

//--------------------------------------------------------------------------//
static void pts_ctl_dbg_task(void * arg)
{
  #if DBG_PTS_CTL == 1

  pts_ctl_dbg_print ("\nPts_Ctl Dbg Start" , NULL , NULL) ;

  for(;;)
    {
      
      Pts_Ctl_Dbg_Q_Msg * q_msg = (Pts_Ctl_Dbg_Q_Msg *) q_remove (pts_ctl_dbg_vars.q_pending, 10) ;
        
      if (q_msg)
        {
          UInt8  i  ;
          
          dbg_tx_request() ;

          if(q_msg->text != NULL)
            {
              dbg_tx_text(q_msg->text) ;
            }
        
          for (i = 0 ; i < q_msg->size ; i++)
            {
              dbg_tx_h8(q_msg->data[i]) ;
            }
          
          dbg_tx_release() ;

          q_return ((Q_Msg *) q_msg) ;
    
        }
    }
  

  #endif
}
//--------------------------------------------------------------------------//

void pts_ctl_dbg_print(StrConst * text ,  const UInt8 data[] , UInt8 size)
{  
  #if DBG_PTS_CTL == 1
  
  Pts_Ctl_Dbg_Q_Msg * q_msg = (Pts_Ctl_Dbg_Q_Msg *) q_remove_c (pts_ctl_dbg_vars.q_free);

  if (q_msg)
    {
      q_msg->text = text ;
  
      memcpy(&q_msg->data[0] , &data[0] , size)  ;
  
      q_msg->size = size ;

      q_insert (pts_ctl_dbg_vars.q_pending , (Q_Msg *) q_msg) ;
    }
  #endif

}
