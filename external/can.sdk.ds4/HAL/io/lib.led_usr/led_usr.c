/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: led_usr.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/


#include "display.h"
#include "wake.h"
#include "config_gpio.h"

#include "led_usr_private.h "               
#include "cmd.h"
#include "timeout_f.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define T_ON  400
#define T_OFF 400

#define ER_T_ON    200   //follow display error timing
#define ER_T_OFF   400
#define ER_T_PAUSE 800

#define IGNITION_ON_TIMEOUT     10000

#define DETECT_LED_FLASH_FAST_TIME              100
#define DETECT_LED_FLASH_SLOW_TIME              100 * 10
//--------------------------------------------------------------------------//
enum 
{
  NO_FLASH       = 0     ,
  LOCK_FLASH     = 1     ,
  UNLOCK_FLASH   = 2     ,
  TRUNK_FLASH    = 3     ,
  AUX1_FLASH     = 4     ,
  AUX2_FLASH     = 5     ,
  AUX3_FLASH     = 6     ,

  START_ON_FLASH    = 65535 ,
  GWR_FLASH         = 65535 ,

  FLASH_COUNT 

};




/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void led_usr              (UInt8 count)    ;
static void led_usr_timeout_start(UInt32 timeout) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


Led_Usr_Vars led_usr_vars ;

static const Cmd_Array1 cmd_array[] = 
{
  {INPUT_GWR               | INPUT_ON , led_usr_gwr_on      },
  {INPUT_GWR                          , led_usr_gwr_off     },
  {INPUT_LOCK1             | INPUT_ON , led_usr_lock        },
  {INPUT_LOCK2             | INPUT_ON , led_usr_lock        },
  {INPUT_LOCK3             | INPUT_ON , led_usr_lock        },
  {INPUT_UNLOCK_DRIVER1    | INPUT_ON , led_usr_unlock      },     
  {INPUT_UNLOCK_DRIVER2    | INPUT_ON , led_usr_unlock      },             
  {INPUT_UNLOCK_DRIVER3    | INPUT_ON , led_usr_unlock      },     
  {INPUT_UNLOCK_ALL1       | INPUT_ON , led_usr_unlock      },     
  {INPUT_UNLOCK_ALL2       | INPUT_ON , led_usr_unlock      },     
  {INPUT_UNLOCK_ALL3       | INPUT_ON , led_usr_unlock      },

  {INPUT_TRUNK             | INPUT_ON , led_usr_trunk       },

  {INPUT_AUX1              | INPUT_ON , led_usr_aux1        },
  {INPUT_AUX2              | INPUT_ON , led_usr_aux2        },
  {INPUT_AUX3              | INPUT_ON , led_usr_aux3        },

  {INPUT_IGNITION          | INPUT_ON , led_usr_rs_ign_on   },
  {INPUT_IGNITION                     , led_usr_rs_ign_off  },
  {INPUT_RS_IGNITION       | INPUT_ON , led_usr_rs_ign_on   },
  {INPUT_RS_IGNITION                  , led_usr_rs_ign_off  },
  {INPUT_STARTER           | INPUT_ON , led_usr_rs_start_on },
  {INPUT_STARTER                      , led_usr_rs_start_off},
  {INPUT_RS_STARTER        | INPUT_ON , led_usr_rs_start_on },
  {INPUT_RS_STARTER                   , led_usr_rs_start_off},
  {INPUT_KEY2GO_LOG_DONE              , led_usr_key2go_led_done},      

} ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void led_usr_init (void)
{
  if(!led_usr_vars.is_init)
    {
      led_usr_vars.timeout = timeout_f_create(NULL , led_usr_timeout) ;

      CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);  
    }

  led_usr_vars.is_init = TRUE ;
  
}


void led_usr_detect           (Led_Usr_Detect led_usr) 
{ 
  switch (led_usr)
  {    
    case LED_USR_DETECT_INIT            : display_once (RED_LED  , 0)                                                             ; break ;
    case LED_USR_DETECT_BP_SKIP         : display_once (BOTH_LED , 0)                                                             ; break ;
    case LED_USR_DETECT_GREEN_ON        : display_once (GREEN_LED, 0)                                                             ; break ;    
                                                                                                                                  ;
    case LED_USR_DETECT_CAN1            : display_flash(BOTH_LED , 65535, DETECT_LED_FLASH_FAST_TIME, DETECT_LED_FLASH_FAST_TIME) ; break ;
    case LED_USR_DETECT_CAN2            : display_flash(GREEN_LED, 65535, DETECT_LED_FLASH_FAST_TIME, DETECT_LED_FLASH_FAST_TIME) ; break ;
    case LED_USR_DETECT_FLASH_RED_FAST  : display_flash(RED_LED  , 65535, DETECT_LED_FLASH_FAST_TIME, DETECT_LED_FLASH_FAST_TIME) ; break ;     
    case LED_USR_DETECT_FLASH_RED_SLOW  : display_flash(RED_LED  , 65535, DETECT_LED_FLASH_SLOW_TIME, DETECT_LED_FLASH_SLOW_TIME) ; break ;
    case LED_USR_DETECT_FLASH_GREEN_SLOW: display_flash(GREEN_LED, 65535, DETECT_LED_FLASH_SLOW_TIME, DETECT_LED_FLASH_SLOW_TIME) ; break ;  
    
    case LED_USR_DETECT_KEY2GO          : 
    {
      if(!led_usr_vars.is_init)
      {
        led_usr_init() ;
      }          
      cmd_q_insert (INPUT_KEY2GO_LOG_DONE , COMMAND_INSERT_TIMEOUT)                           ; 
      
      break ;   
    }
       

    case LED_USR_DETECT_DONE            : display_once (GREEN_LED, 3000)                                                          ; break ;
    case LED_USR_DETECT_NO_BP_DONE      : display_once (BOTH_LED , 3000)                                                          ; break ;
    case LED_USR_DETECT_NO_VIN_DONE     : display_once (RED_LED  , 3000)                                                          ; break ;
  }   
}
//--------------------------------------------------------------------------//
void led_usr_key2go_led_done (void * arg) 
{
  display_flash(BOTH_LED , 65535, DETECT_LED_FLASH_SLOW_TIME, DETECT_LED_FLASH_SLOW_TIME) ;
}
//--------------------------------------------------------------------------//
void led_usr_gwr_on (void * arg) 
{
  if(!led_usr_vars.gwr)
    {
      led_usr_vars.gwr = TRUE  ;
      
      wake_enter () ;

      display_flash (GREEN_LED, GWR_FLASH, T_ON, T_OFF) ;
    }
}

//--------------------------------------------------------------------------//
  
void led_usr_gwr_off(void * arg) 
{
  if(led_usr_vars.gwr)
    {
      led_usr_vars.gwr = FALSE  ;

      timeout_stop(led_usr_vars.timeout) ;
      
      display_flash (GREEN_LED, NO_FLASH, T_ON, T_OFF) ;   
      
      wake_leave () ; 
    }
}

//--------------------------------------------------------------------------//

void led_usr_lock   (void * arg) 
{
  led_usr(LOCK_FLASH)  ;
}

//--------------------------------------------------------------------------//

void led_usr_unlock (void * arg) 
{
  led_usr (UNLOCK_FLASH) ;
}

//--------------------------------------------------------------------------//

void led_usr_trunk  (void * arg) 
{
  led_usr (TRUNK_FLASH) ;
}

//--------------------------------------------------------------------------//
void led_usr_aux1  (void * arg) 
{
  led_usr (AUX1_FLASH) ;
}

//--------------------------------------------------------------------------//

void led_usr_aux2  (void * arg) 
{
  led_usr (AUX2_FLASH) ;
}

//--------------------------------------------------------------------------//

void led_usr_aux3  (void * arg) 
{
  led_usr (AUX3_FLASH) ;
}
//--------------------------------------------------------------------------//
void led_usr_detect_err   (Led_Usr_Detect_Err_Code err_code , UInt8 count )
{
  display_error(err_code , count) ;
}
//--------------------------------------------------------------------------//
void led_usr_op_err   (Led_Usr_Op_Err_Code err_code , UInt8 count )
{
  display_error(err_code , count) ;

  if(led_usr_vars.gwr)
    {
      led_usr_timeout_start (count *(err_code  * (ER_T_ON + ER_T_OFF) + ER_T_PAUSE)) ;
    }
}
//--------------------------------------------------------------------------//
static void led_usr(UInt8 count)
{
  display_flash (BOTH_LED, count, T_ON, T_OFF) ;

  if(led_usr_vars.gwr)
    {
      led_usr_timeout_start (count  * (T_ON + T_OFF)) ;
    }
}
//--------------------------------------------------------------------------//
static void led_usr_timeout_start(UInt32 timeout)
{
  timeout_update(led_usr_vars.timeout , timeout) ;

  timeout_start(led_usr_vars.timeout)            ;
}

//--------------------------------------------------------------------------//

void led_usr_timeout  (void * arg) 
{
  if(led_usr_vars.gwr)
    {
      display_flash (GREEN_LED, GWR_FLASH, T_ON, T_OFF) ;
    }
}
//--------------------------------------------------------------------------//
void led_usr_rs_ign_on    (void * arg)
{
  if(led_usr_vars.gwr && !led_usr_vars.ign)
    {
      led_usr_vars.ign = TRUE ;

      LED_RED_SET ;
      led_usr_timeout_start (IGNITION_ON_TIMEOUT) ;
    }
}
//--------------------------------------------------------------------------//
void led_usr_rs_ign_off   (void * arg)
{
  if(led_usr_vars.ign)
    {
      LED_RED_CLR ;
         
      led_usr_vars.ign = FALSE ;
      
      timeout_stop(led_usr_vars.timeout) ;

      if(led_usr_vars.gwr)
        {
          display_flash (GREEN_LED, GWR_FLASH, T_ON, T_OFF) ;  
        }
        
    }
}
//--------------------------------------------------------------------------//
void led_usr_rs_start_on  (void * arg)
{
  if(led_usr_vars.gwr)
    {
      LED_RED_CLR ;
      
      display_flash (GREEN_LED, START_ON_FLASH, T_ON/4, T_OFF/4) ;
    }
}
//--------------------------------------------------------------------------//
void led_usr_rs_start_off (void * arg)
{  
  LED_RED_CLR ;

  if(led_usr_vars.gwr)
    {
      display_flash (GREEN_LED, GWR_FLASH, T_ON, T_OFF) ;
    }

  

}

