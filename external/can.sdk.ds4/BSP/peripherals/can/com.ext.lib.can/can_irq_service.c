/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_irq_service.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void can_irq_service_device (Can_Dev can_dev) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_irq_service (void)
{
  #ifdef DEV_LPC_176x
    Can_Dev_Id dev_id = CAN_DEV_1 ;
  #else
    Can_Dev_Id dev_id = CAN_DEV_0 ;
  #endif

  do
    {
      if (can_dev_table [dev_id])
        {
          can_irq_service_device (can_dev_table [dev_id]) ;
        }
    }
  while (++dev_id < CAN_DEV_COUNT) ;

}

//--------------------------------------------------------------------------//

static void can_irq_service_device (Can_Dev can_dev)
{
  UInt32 bus_status = can_dev->io_base [CANICR]              ;
  UInt32 irq_status = can_dev->io_base [CANIER] & bus_status ;

  #define BIT(z) ((UInt32) 1 << (z))

  /*service_lost_arbitration :*/
    {
      if (bus_status & BIT (6))
        {
          can_dev->irq_arbitration_cache = TRUE ;
        }
    }

  /*service_rx :*/
    {    
      can_irq_service_rx (can_dev, bus_status) ;          
    }

  /*service_tx :*/
    {
      Boolean irq_arbitration_cache_clr = FALSE ;

      if (can_dev->irq_arbitration_cache)
        {
          bus_status |= BIT (6) ;
        }

      #define SERVICE_TX(z1,z2)                                         \
                                                                        \
      if (irq_status & BIT (z1))                                        \
        {                                                               \
          can_irq_service_tx (can_dev->tx [CAN_TX_##z2] , bus_status) ; \
                                                                        \
          irq_arbitration_cache_clr = TRUE ;                            \
        }

      SERVICE_TX (0x01,0)
      SERVICE_TX (0x09,1)
      SERVICE_TX (0x0A,2)

      #undef  SERVICE_TX
    
      if (irq_arbitration_cache_clr)
        {
          can_dev->irq_arbitration_cache = FALSE ;
        }
    }

  /*service_bus_error :*/
    {
      if (irq_status & BIT (7))
        {
          can_irq_service_bus_error (can_dev) ;
        }
    }

  /*service_bus_off :*/
    {
      if (irq_status & BIT (2))
        {
          can_irq_service_bus_off (can_dev) ;
        }
    }

  /*service_data_overrun :*/
    {
      if (irq_status & BIT (3))
        {
          can_irq_service_overrun (can_dev) ;
        }
    }

  #undef  BIT
}

