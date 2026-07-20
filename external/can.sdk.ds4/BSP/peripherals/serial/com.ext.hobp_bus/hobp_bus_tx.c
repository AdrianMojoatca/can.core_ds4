/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_bus_tx.c 25922 2014-06-25 13:16:27Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_bus_private.h"
#include <string.h>

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
Boolean hobp_bus_tx(UInt8  * data , UInt8 size)
{      
  Boolean status = FALSE ;   
  
  if(size < HOBP_BUS_DATA_SIZE)
    {
      Hobp_Bus_Tx_Q_Msg * tx_q_msg = (Hobp_Bus_Tx_Q_Msg *) q_remove_c (hobp_bus_vars.tx_q_free) ;
      
      if(tx_q_msg)
        {
          tx_q_msg->msg.size = size  ;
          
          memcpy(&tx_q_msg->msg.data[0] , data , size) ;

          q_insert (hobp_bus_vars.tx_q_pending , (Q_Msg *) tx_q_msg) ;

          hobp_bus_set_flag(HOBP_BUS_TX_BYTE_DONE) ;
          
          q_return ((Q_Msg *) tx_q_msg) ;
          
          status = TRUE ;

          #if DBG_HOBP_BUS == 1
            hobp_bus_dbg_print ("\nTx             :" , data, size ) ;
          #endif

        }
      else
        {
          #if DBG_HOBP_BUS == 1
            hobp_bus_dbg_print ("\nTx no q free   " , NULL , NULL) ;
          #endif
        }
    }
  else
    {
      #if DBG_HOBP_BUS == 1
        hobp_bus_dbg_print ("\nTx Size Fail   :" , &size, 1 ) ;
      #endif
    }
  
  

  
  
  
  
  
  
  
  
  
  

//    {
//      UInt8 i ;
//    
//      for(i = 0 ; i < size ; i++)
//        {
//          int rx_data ;
//          
//          if(!os_pip_insert(hobp_bus_vars.pip_tx , data[i] , HOBP_BUS_TX_TIMEOUT_2MS ))
//            {      
//              os_pip_flush(hobp_bus_vars.pip_tx) ;
//              
//              hobp_bus_err_add(HOBP_BUS_ERR_TX_PIP_FULL)  ;
//              
//              return FALSE ;
//            }
//
//        }
//      
//      //if(!tmr_int_sta (hobp_bus_vars.tmr_tx))
//        {
//          hobp_bus_set_flag(HOBP_BUS_TX_BYTE_DONE) ;
//        }
//
//      status =  TRUE ;
//    }


  return status ;
}


