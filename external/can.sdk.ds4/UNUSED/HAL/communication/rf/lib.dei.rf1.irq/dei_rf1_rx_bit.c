/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf1_rx_bit.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf1_private.h"
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

//--------------------------------------------------------------------------------------//

Dei_Rf1_Bit dei_rf1_rx_bit( void )
{
	UInt32 time_diff ;
	UInt32 time_fiq  ; 

	Dei_Rf1_Edge edge_type = dei_rf1_ext_status();

	if( edge_type == DEI_RF1_FALLING_EDGE )
	{
		dei_rf1_vars.rx_time = dei_rf1_vars.rx_time_stamp;

		dei_rf1_ext_enable_rising();
	}

	if( edge_type == DEI_RF1_RISING_EDGE )
	{
		dei_rf1_ext_enable_falling();

		time_fiq = 	dei_rf1_vars.rx_time_stamp;

		time_diff = ( time_fiq > dei_rf1_vars.rx_time ) ? time_fiq-dei_rf1_vars.rx_time : (0xFFFFFFFF - dei_rf1_vars.rx_time)+time_fiq ; 

		if( time_diff < dei_rf1_vars.rx_time_low_min  ) return DEI_RF1_ERROR_BIT ;
		if( time_diff < dei_rf1_vars.rx_time_low_max  ) return DEI_RF1_LOW_BIT   ;
		if( time_diff < dei_rf1_vars.rx_time_high_max ) return DEI_RF1_HIGH_BIT  ;
		if( time_diff < dei_rf1_vars.rx_time_start_min)	return DEI_RF1_ERROR_BIT ;
		if( time_diff < dei_rf1_vars.rx_time_start_max) return DEI_RF1_START_BIT ;
		
		return DEI_RF1_ERROR_BIT ; 
	}

	return DEI_RF1_NO_BIT ;
} 

//--------------------------------------------------------------------------------------//

