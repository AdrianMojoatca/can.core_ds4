/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_rx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void can_rx_timeout_service (void * io_base) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean can_rx (Can_Rx  can_rx , Can_Msg * can_msg , TskTimeout timeout)
{
  CRITICAL ;

  /// Cache address of CAN device ///

  Can_IO_Base io_base = can_rx->can_dev->io_base ;

  /// Check if RX buff is full. If not, wait for msg to arrive. ///

  if (! (io_base[CANSR] & 0x01))
    {
      Boolean status ;

      os_timeout_arm_set (can_rx_timeout_service , (void *) io_base) ;  
        {
          C_ENTER ;
            {
              io_base [CANIER] |= 0x01 ;

              status = os_irq_wait (can_rx->irq_rx_ready , TRUE , timeout) ;
            }
          C_LEAVE ;
        }
      os_timeout_arm_clr () ;

      if (!status)
        {
          return FALSE ;
        }
    }

   /// RX buff is full, so load it into message buffer ///

   can_msg_rd (io_base , can_msg) ;

  return TRUE ;
}

//--------------------------------------------------------------------------//

static void can_rx_timeout_service (void * io_base)
{
  #define IO_BASE ((Can_IO_Base) io_base)

  IO_BASE [CANIER] &= ~ (UInt32) 0x01 ;

  #undef  IO_BASE
}

