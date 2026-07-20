/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_dev_create.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr.h"
#include "can_private.h"

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

Can_Dev can_dev_create (Can_Dev_Id can_dev_id , UInt8 brg , UInt8 tseg_1 , UInt8 tseg_2 , UInt8 sjw , Boolean sample_3_times , Can_Pin_Map pin_map)
{
  CRITICAL ;

  Can_Dev can_dev ;

  /*init_can_dev_object :*/
    {
      can_dev = (Can_Dev) os_mem_request (sizeof (struct s_Can_Dev)) ;

      /*init_rx :*/
        {
          UInt8 index = 0 ;
      
          do
            {
              can_dev->rx [index] = NULL ;
            }
          while (++index < sizeof_array (can_dev->rx)) ;

          can_dev->irq_service_rx        = NULL  ;
          can_dev->irq_arbitration_cache = FALSE ;
        }

      /*init_tx :*/
        {
          UInt8 index = 0 ;
      
          do
            {
              can_dev->tx [index] = NULL ;
            }
          while (++index < sizeof_array (can_dev->tx)) ;
        }
    
      /*init_io_base :*/
        {
          static const Can_IO_Base can_io_base_table [CAN_DEV_COUNT] =
            {
              CAN_BASE_0 ,CAN_BASE_1
            } ;
    
          can_dev->io_base    = can_io_base_table [can_dev_id] ;

          can_dev->can_dev_id = can_dev_id ;
        }
    }

  C_ENTER ;
    {
      static Boolean do_one_time_init = TRUE ;

      /*init_can_module :*/
        {
          can_pin_map (pin_map)    ;

          can_dev_ena (can_dev_id) ;

          can_set_bitrate (can_dev->io_base , brg , tseg_1 , tseg_2 , sjw , sample_3_times) ;

          can_dev->io_base [CANMOD] |=  (0x01 << 3) ;              // CAN-TX number determines TX priority, not identifier
          can_dev->io_base [CANMOD] &= ~(0x01 << 0) ;              // Enable CAN TX & RX operations.

          can_dev->io_base [CANIER] |= (0x01 << CAN_DOIE) ;	 	 //	DATA OVERRUN IRQ enabled
		  can_dev->io_base [CANIER] |= (0x01 << CAN_ALIE) ;		 // ARBITRATION IRQ enabled
		  can_dev->io_base [CANIER] |= (0x01 << CAN_EIE) ;		 // ERROR WARNING IRQ enabled
        }

      if (do_one_time_init)
        {
          do_one_time_init = FALSE ;

          isr_set_vector (ISR_ID_CAN , ISR_PRI_1 , ISR_TYPE_IRQ , can_irq_service) ;
        }

      can_dev_table [can_dev_id] = can_dev ;
    }
  C_LEAVE ;

  return can_dev ;
}

