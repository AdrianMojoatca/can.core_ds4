/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_tx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_private.h"
#include "wdog_v.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void can_tx_timeout_service (void * can_tx) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean can_tx (Can_Tx can_tx , Can_Msg * can_msg , TskTimeout timeout)
{
  CRITICAL ;

  /// Cache address of CAN device and TX register bit definitions ///

        Can_IO_Base  io_base    =   can_tx->can_dev->io_base             ;
  const Can_Tx_Bit * can_tx_bit = & can_tx_bit_table [can_tx->can_tx_id] ;

  /// Check if TX buff is busy. If yes, wait until free. ///

  if (! (io_base[CANSR] & can_tx_bit->cansr_tbs))
    {
      Boolean status ;

      os_timeout_arm_set (can_tx_timeout_service , can_tx) ;
        {
          //C_ENTER ;
            //{
              io_base [CANIER] |= can_tx_bit->canier_tie ;

			  // Place a virtual watch here in case of BUSOFF error.
			  //   BUSOFF is not handled by the firmware; 
			  //   If it happens, TX may never complete, 'evt_tx' may never be set; we'd be stuck here.
			  //   Ideally, we should handle BUSOFF, but it's still unclear how to do it properly.

			  if(timeout == NULL) // add protection if timeout == NULL, we don't want to wait forever.
              {
                timeout = 1 ;
              }
              
              //VIRTUAL_WATCH
			  //(
				
                status = os_evt_wait (can_tx->evt_tx , CAN_IRQ_TX_READY , CAN_IRQ_TX_READY , timeout) ;
			  //);
			  
            //}
          //C_LEAVE ;
        }
      os_timeout_arm_clr () ;

      if (!status)
        {
           C_ENTER ;
             {
               io_base [CANCMR] = can_tx_bit->cancmr_stb | 0x02 ; // Select TX buff and signal TX abort
             }
           C_LEAVE ;

          return FALSE ;
        }
    }

   /// TX buff is free, so load it with message and signal TX request ///

   can_msg_wr (io_base , can_msg , can_tx->can_tx_id) ;
  
   C_ENTER ;
     {
       io_base [CANCMR] = can_tx_bit->cancmr_stb | 0x01 ; // Select TX buff and start TX
     }
   C_LEAVE ;

  return TRUE ;
}

//--------------------------------------------------------------------------//

static void can_tx_timeout_service (void * can_tx)
{
  #define CAN_TX ((Can_Tx) can_tx)

  ATOMIC(CAN_TX->can_dev->io_base [CANIER] &= ~ can_tx_bit_table [CAN_TX->can_tx_id].canier_tie) ;

  #undef  CAN_TX
}

