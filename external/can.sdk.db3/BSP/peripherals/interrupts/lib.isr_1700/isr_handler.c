/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: isr_handler.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "isr_private.h"
#include "fiq.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
PVF isr_user_func_vector;
PVF isr_user_func[64];

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

// values in nvic_vector_table must be odd when the user routine irq_func_addr contains Thumb instruction

#define THUMB_PVF(x) ((PVF)(1+(UInt32)(x)))
volatile UInt32 temporary_variable;

void isr_set_vector (Isr_Number irq_num , Isr_Pri irq_pri , Isr_Type irq_type , PVF irq_func_addr)
{
	nvic_set_priority( irq_num, irq_pri );
	
	
	// Setting NVIC vector table for slow interrupt handler address 
	if( irq_type == ISR_TYPE_IRQ )
	{
		temporary_variable = (UInt32)(&ISR_Handler_0) + (int)irq_num * 8;
		

		ATOMIC_ALL (nvic_vector_table [irq_num + 16] =(temporary_variable));   // 12 bytes for each prolog
		isr_user_func[irq_num + 16] = irq_func_addr ;  // copy user_func address to array in RAM 
		temporary_variable++;
	}
	else
	{
		ATOMIC_ALL (nvic_vector_table [irq_num + 16] = (UInt32)(irq_func_addr)) ;
	}
	
	nvic_irq_enable( irq_num, TRUE );
}


/*==========================================================================*/

void isr_set_fiq_channel( Isr_Number irq_num )
{
	nvic_set_priority( irq_num, ISR_PRI_0 );
	
  ATOMIC_ALL ( nvic_vector_table [irq_num + 16] = (UInt32)fiq_service_vector ) ;

	nvic_irq_enable( irq_num, TRUE );
	
}

/*==========================================================================*/

void nvic_set_vector (Isr_Number irq_num , PVF irq_handler)
{
  ATOMIC_ALL (nvic_vector_table [irq_num + 16] = (UInt32)irq_handler) ;
}

/*==========================================================================*/

void isr_set_fiq_pri( Isr_Number irq_num )
{
	nvic_set_priority( irq_num, 31 );
}

/*==========================================================================*/

void isr_reset_fiq_pri( Isr_Number irq_num )
{
	nvic_set_priority( irq_num, 29 );
}

