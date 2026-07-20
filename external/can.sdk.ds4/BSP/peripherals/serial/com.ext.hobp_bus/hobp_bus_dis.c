/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_bus_dis.c 25922 2014-06-25 13:16:27Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_bus_private.h"
#include "wdog_v.h"

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

void hobp_bus_dis (void * arg)
{ 
  hobp_bus_rx_edge_dis() ;
 
  if(tmr_int_sta (hobp_bus_vars.tmr_tx))
    {
      hobp_bus_err_add(HOBP_BUS_ERR_TX_RUNNING) ;
      
      #if DBG_HOBP_BUS == 1
         hobp_bus_dbg_print("\nHobp Bus Tx Running" , NULL , NULL) ;
      #endif  
      
      while(tmr_int_sta (hobp_bus_vars.tmr_tx)) ;

    }

//  if(!os_pip_test_empty(hobp_bus_vars.pip_tx))
//    {
//      os_pip_flush(hobp_bus_vars.pip_tx) ;
//      
//      hobp_bus_err_add(HOBP_BUS_ERR_TX_NOT_EMPTY)  ;
//
//      #if DBG_HOBP_BUS == 1
//         hobp_bus_dbg_print("\nHobp Bus Tx Not Empty" , NULL , NULL) ;
//      #endif 
//    }
  
  if(tmr_int_sta (hobp_bus_vars.tmr_rx))
    {
      hobp_bus_err_add(HOBP_BUS_ERR_RX_RUNNING) ;

      #if DBG_HOBP_BUS == 1
         hobp_bus_dbg_print("\nHobp Bus Rx Running" , NULL , NULL) ;
      #endif 
      
      while(tmr_int_sta (hobp_bus_vars.tmr_rx)) ;
       
    }

  if(!q_empty(hobp_bus_vars.rx_q_pending))
    {
      hobp_bus_err_add(HOBP_BUS_ERR_RX_NOT_EMPTY) ;

      #if DBG_HOBP_BUS == 1
         hobp_bus_dbg_print("\nHobp Bus Not Empty" , NULL , NULL) ;
      #endif     
          
       while(!q_remove_c (hobp_bus_vars.rx_q_pending));

    }

  #if DBG_HOBP_BUS == 1
     hobp_bus_dbg_print("\nHobp Bus Dis" , NULL , NULL) ;
  #endif   
}

