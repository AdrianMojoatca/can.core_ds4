/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_abd_lookup.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_private.h"
#include "isr.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define SAMPLE_ONCE 0
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Can_Abd_Status can_abd_lookup (Can_Abd_Def * const can_abd_def , TskTimeout timeout)
{	
  static Boolean do_one_time_init = TRUE ;
	
  Can_Abd_Status can_abd_status	;

  static const Can_IO_Base can_io_base_table [CAN_DEV_COUNT] =
    {
      CAN_BASE_0 , CAN_BASE_1
    } ;

  can_abd.io_base = can_io_base_table [can_abd_def->can_dev_id] ;

  can_dev_ena  (can_abd_def->can_dev_id) ;	    // Start CAN peripheral clock

  can_abd.io_base [CANMOD] |= 0x01 ;            // Disable CAN TX & RX operations.

  can_pclk_div (can_abd_def->bit_def.pclk_div) ;
  can_pin_map  (can_abd_def->can_pin_map) ;

  can_set_bitrate
  (
    can_abd.io_base ,
    can_abd_def->bit_def.brg ,
    can_abd_def->bit_def.tseg_1 ,
    can_abd_def->bit_def.tseg_2 ,
    can_abd_def->bit_def.sjw ,
    SAMPLE_ONCE
  ) ;

  can_listen_ena (can_abd.io_base) ;

  can_abd.io_base [CANMOD] &= ~(0x01) ;         // Enable CAN TX & RX operations.

  can_lut_bypass_ena () ;

  if (do_one_time_init)
    {
      CRITICAL;
	    
	  do_one_time_init = FALSE ;

	  C_ENTER ;
	    {
		  can_abd.evt = os_evt_create () ;
          isr_set_vector (ISR_ID_CAN , ISR_PRI_31 , ISR_TYPE_IRQ , can_abd_irq_service) ;
	    }  
	  C_LEAVE ;
    }
  
  can_abd.io_base [CANIER] |= (0x01 << 0) ;		// Set CAN bus RX interrupts
  can_abd.io_base [CANIER] |= (0x01 << 7) ;		// Set CAN bus error interrupts

  can_abd_test (&can_abd_status , timeout) ;

  can_lut_bypass_dis () ;

  can_abd.io_base [CANIER] &= ~(0x01 << 0) ;	// Clr CAN bus RX interrupts
  can_abd.io_base [CANIER] &= ~(0x01 << 7) ;	// Clr CAN bus error interrupts

  can_abd.io_base [CANMOD] |= 0x01 << 0 ;     	// Disable CAN TX & RX operations.(reset mode)

  can_dev_dis (can_abd_def->can_dev_id);			// Stop CAN peripheral clock

  return can_abd_status ;		
}
