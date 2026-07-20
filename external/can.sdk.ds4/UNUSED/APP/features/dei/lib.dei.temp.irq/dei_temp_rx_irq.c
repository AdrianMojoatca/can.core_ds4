/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_temp_rx_irq.c 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_temp_private.h"
#include "gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Dei_Temp_Bit dei_temp_get_bit( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------------------//

void dei_temp_rx_irq( void )
{
	Dei_Temp_Bit temp_bit = dei_temp_get_bit();

	if( temp_bit == DEI_TEMP_ERROR_BIT )
	{
		dei_temp_vars.rx_data = 0 ; 
		dei_temp_vars.rx_idx  = 0 ; 
	}

	if( temp_bit == DEI_TEMP_LOW_BIT )
	{
		dei_temp_vars.rx_data  = dei_temp_vars.rx_data << 1;			
		dei_temp_vars.rx_data &= 0xFE ;
			
		dei_temp_vars.rx_idx ++ ; 
	}
		
	if( temp_bit == DEI_TEMP_HIGH_BIT )
	{
		dei_temp_vars.rx_data  = dei_temp_vars.rx_data << 1;			
		dei_temp_vars.rx_data |= 0x01 ;
			
		dei_temp_vars.rx_idx ++ ; 
	}
												   
	if( temp_bit != DEI_TEMP_NO_BIT )
    {    
        if( dei_temp_vars.rx_idx == DEI_TEMP_RX_BIT_COUNT )
    	{
    		dei_temp_vars.rx_complete = TRUE ;
    		
    		tmr_irq_dis( dei_temp_vars.tx_tmr ); 
    
    		dei_temp_ext_disable();
    	}
    }
} 

//--------------------------------------------------------------------------------------//

Dei_Temp_Bit dei_temp_get_bit( void )
{
	UInt32 time_diff ;
	UInt32 time_fiq  ; 

	if( dei_temp_vars.edge == DEI_TEMP_RISING_EDGE )
	{
		dei_temp_vars.rx_time = T1TC ; 

		dei_temp_vars.edge = DEI_TEMP_NO_EDGE ;
		gpio_pl_rising_irq_ena(GPIO_PL_TEMP_SENSOR);
		gpio_pl_falling_irq_ena(GPIO_PL_TEMP_SENSOR);
	}

	if( dei_temp_vars.edge == DEI_TEMP_FALLING_EDGE )
	{
		time_fiq = 	T1TC ;

		time_diff = ( time_fiq > dei_temp_vars.rx_time ) ? time_fiq-dei_temp_vars.rx_time : (0xFFFFFFFF - dei_temp_vars.rx_time)+time_fiq ; 

		dei_temp_vars.edge = DEI_TEMP_NO_EDGE ;
		
		gpio_pl_rising_irq_ena(GPIO_PL_TEMP_SENSOR);
		gpio_pl_falling_irq_ena(GPIO_PL_TEMP_SENSOR);


		if( time_diff < DEI_TEMP_RX_LOW_MIN         ) return DEI_TEMP_ERROR_BIT ;
		if( time_diff < DEI_TEMP_RX_LOW_HIGH_BORDER ) return DEI_TEMP_LOW_BIT   ;
		if( time_diff < DEI_TEMP_RX_HIGH_MAX        ) return DEI_TEMP_HIGH_BIT  ;
		
		return DEI_TEMP_ERROR_BIT ; 
	}

	return DEI_TEMP_NO_BIT ;
} 

//--------------------------------------------------------------------------------------//
