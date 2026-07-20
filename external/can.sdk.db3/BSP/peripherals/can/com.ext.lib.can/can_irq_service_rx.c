/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_irq_service_rx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define BIT(z) ((UInt32) 1 << (z))

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean service_rx_data_external (Can_Dev can_dev) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_irq_service_rx (Can_Dev can_dev , UInt32 dev_icr)
{

  if (dev_icr & BIT (CAN_RIE))
    {
      Boolean is_rx_overun_suspected = FALSE ;

      if (can_dev->irq_service_rx)
        {
          is_rx_overun_suspected = service_rx_data_external (can_dev) ;
        }
      else
        {
          os_irq_set_i (can_dev->rx[0]->irq_rx_ready) ;
          can_dev->io_base [CANIER] &= ~ (UInt32) 0x01 ;
        }

      if (is_rx_overun_suspected)
        {
          static Boolean do_log_once = TRUE ;

          can_ctl_reset (can_dev) ;     // Reset CAN controller 

          if(do_log_once)               // log error only once to avoid commiting to nvfs too often
            {
              do_log_once = FALSE ;
              can_irq_func (can_dev->can_dev_id, CAN_IRQ_ERRATA) ;
            }
        }
    }
}

//--------------------------------------------------------------------------//

static Boolean service_rx_data_external (Can_Dev can_dev)
{
  can_dev->irq_service_rx (can_dev->io_base) ;                        // Process first  message, callee issues RBB command

  if (!!(can_dev->io_base[CANGSR] & BIT (CAN_RBS)))
    {
      can_dev->irq_service_rx (can_dev->io_base) ;                    // Process second message, callee issues RBB command
                                                    
      return TRUE ;
    }

  return FALSE ;
}

#undef BIT

