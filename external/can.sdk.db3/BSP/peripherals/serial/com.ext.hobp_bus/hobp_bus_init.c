/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_bus_init.c 25922 2014-06-25 13:16:27Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "fiq.h"
#include "irq_user.h"
#include "hobp_bus_private.h"
#include "wdog_v.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define HOBP_BUS_RX_SIZE         10
#define HOBP_BUS_TX_SIZE         5 // response is transmit 2x with 7 bytes 

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean  hobp_bus_init_rx            (UInt8 size) ;
static Boolean  hobp_bus_init_tx            (UInt8 size) ;   
static Boolean  hobp_bus_rx_free_q_ok       (void);
static Boolean  hobp_bus_tx_free_q_ok       (void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

int hobp_bus_init (void)           
{ 
  #define NVFS hobp_bus_init_nvfs ()
  #define RX   hobp_bus_init_rx   (HOBP_BUS_RX_SIZE)
  #define TX   hobp_bus_init_tx   (HOBP_BUS_TX_SIZE)
  
  if(NVFS && RX && TX)
    {     
      hobp_bus_set_baudrate(HOBP_BUS_SPEED) ;
      
      hobp_bus_vars.tsk           = os_tsk_create (hobp_bus_task , TASK_HOBP_BUS_PRI , TASK_HOBP_BUS_STK , 0 , TASK_HOBP_BUS_NAME) ;
      
      
      
      fiq_register        (hobp_bus_fiq_service    ) ;
      irq_user_register   (hobp_bus_user_service   ) ;
      wake_register       (hobp_bus_wake           ) ;

      wdog_v_register_queue_check(hobp_bus_rx_free_q_ok    , "HOBP_BUS_RX");
      wdog_v_register_queue_check(hobp_bus_tx_free_q_ok    , "HOBP_BUS_TX");

      #if DBG_HOBP_BUS == 1
        hobp_bus_dbg_init() ;
        hobp_bus_dbg_print("\nHobp_Bus Init Done" , NULL, NULL) ; 
        ; 
      #endif
      
      os_tsk_start        (hobp_bus_vars.tsk    , 0) ;
			#ifdef PLATFORM_404 
				PULL_12V_ISO1_SET;
			#endif
      return TRUE ;
    }

  return FALSE ;

  #undef NVFS
  #undef RX
  #undef TX
}

//--------------------------------------------------------------------------//
static Boolean hobp_bus_init_rx(UInt8 size)
{
  hobp_bus_vars.tmr_rx      = tmr_create   () ;

  if(hobp_bus_vars.tmr_rx)
    {
      UInt8 n ;

      hobp_bus_vars.rx_q_free   = q_create     () ;
      hobp_bus_vars.rx_q_pending= q_create     () ;
    
      for (n = 0 ; n < size ; n++)
        {
          Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Hobp_Bus_Rx_Q_Msg)) ;
            
          if(!q_msg)
            {
              return FALSE ;
            }
          
          q_assign (hobp_bus_vars.rx_q_free , q_msg) ;
          q_insert (hobp_bus_vars.rx_q_free , q_msg) ;
        }
      return TRUE ;
    }
  return FALSE ;
}

//--------------------------------------------------------------------------//
static Boolean hobp_bus_init_tx(UInt8 size)
{  
  hobp_bus_vars.tmr_tx       = tmr_create   ()      ;  
  
  if(hobp_bus_vars.tmr_tx)
    {
      UInt8 n ;

      hobp_bus_vars.tx_q_free    = q_create () ;
      hobp_bus_vars.tx_q_pending = q_create () ;
      
//      hobp_bus_vars.pip_tx       = os_pip_create(size)  ;
    
      for (n = 0 ; n < size ; n++)
        {
          Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Hobp_Bus_Tx_Q_Msg)) ;
            
          if(!q_msg)
            {
              return FALSE ;
            }
          
          q_assign (hobp_bus_vars.tx_q_free , q_msg) ;
          q_insert (hobp_bus_vars.tx_q_free , q_msg) ;
        }
      return TRUE ;
    }
  
  return FALSE ;

  //return (hobp_bus_vars.tmr_tx && hobp_bus_vars.pip_tx) ;
}

//--------------------------------------------------------------------------//

// This will be called by wdog_v to check if a free q still has available free messages;
// A free q should never be empty - that can only happen if q_return() is not used properly.
static Boolean hobp_bus_rx_free_q_ok (void)
{   
    return !q_empty(hobp_bus_vars.rx_q_free);
}
//--------------------------------------------------------------------------//

static Boolean hobp_bus_tx_free_q_ok (void)
{   
    return !q_empty(hobp_bus_vars.tx_q_free);
}
//--------------------------------------------------------------------------//

