/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_tx_one_shot.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Can_Tx_Status can_tx_one_shot (Can_Tx  can_tx , Can_Msg * can_msg, TskTimeout timeout)
{
  CRITICAL ;

  /// Cache address of CAN device and TX register bit definitions ///
  
        Can_IO_Base  io_base    =   can_tx->can_dev->io_base             ;
  const Can_Tx_Bit * can_tx_bit = & can_tx_bit_table [can_tx->can_tx_id] ;

  /// Check if TX buff is busy. If free, send message ///

  if ((io_base[CANSR] & can_tx_bit->cansr_tbs))
    {
      UInt16 evt_status ;

      /// Load TX buff with message and signal one-shot TX request ///
    
      can_msg_wr (io_base , can_msg , can_tx->can_tx_id) ;
      
      C_ENTER ;
        {
          io_base [CANIER] |= can_tx_bit->canier_tie ;          // Enable TX_READY
          io_base [CANCMR]  = can_tx_bit->cancmr_stb | 0x03 ;   // Select TX buff and start TX with abort flag set (one-shot mode)

        #define TX_IRQ_BITS (CAN_IRQ_TX_READY | CAN_IRQ_TX_BUS_ERROR | CAN_IRQ_TX_LOST_ARBITRATION)
		  evt_status = os_evt_wait (can_tx->evt_tx , TX_IRQ_BITS , TX_IRQ_BITS , timeout) ;
		#undef  TX_IRQ_BITS 

		  if(!evt_status) // if timed out do controller reset
		    {		  			  
			  can_ctl_reset (can_tx->can_dev) ; 		 
		    }
        }
      C_LEAVE ;

      if (evt_status & CAN_IRQ_TX_LOST_ARBITRATION) return CAN_TX_LOST_ARBITRATION ;
      if (evt_status & CAN_IRQ_TX_BUS_ERROR       ) return CAN_TX_BUS_ERROR        ;
	  if (!evt_status)                              return CAN_TX_TIMEOUT          ;

      return CAN_TX_OK ;
    }
  else
    {
      return CAN_TX_BUSY ;
    }
}

