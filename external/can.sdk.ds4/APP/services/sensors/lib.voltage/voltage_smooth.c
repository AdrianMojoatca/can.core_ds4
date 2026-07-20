/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: voltage_smooth.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "voltage_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef struct s_Voltage_Smooth
{
    UInt8   size ;
    UInt8   index;
    double *table;
    Boolean is_non_zero;
}Voltage_Smooth;
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void voltage_smooth_reset( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Voltage_Smooth  voltage_smooth_vars;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void voltage_smooth_init( UInt8 size )
{
	voltage_smooth_vars.size =  size ;
		
	voltage_smooth_vars.table = (double*)os_mem_request( sizeof(double) * size ) ; 
	
	voltage_smooth_reset();		
}

/*--------------------------------------------------------------------------*/

double voltage_smooth( double instant )
{
	double value = 0 ; 

	UInt8  i ;
		
	voltage_smooth_vars.table[ voltage_smooth_vars.index ] = instant ; 
	
	voltage_smooth_vars.index = (voltage_smooth_vars.index + 1)%voltage_smooth_vars.size ; 

	if( !voltage_smooth_vars.index || (voltage_smooth_vars.size == 1) ) voltage_smooth_vars.is_non_zero = TRUE ;	
	
	for( i = 0 ; i < voltage_smooth_vars.size ; i ++ ) value += voltage_smooth_vars.table[i] ; 
	
    //volt_smooth_tmp = voltage_smooth_vars.is_non_zero ? value / voltage_smooth_vars.size  : 0 ; 
	return voltage_smooth_vars.is_non_zero ? value / voltage_smooth_vars.size  : instant ; 
}

/*--------------------------------------------------------------------------*/

void voltage_smooth_reset( void )
{
	UInt8 i ;
	
	for( i = 0 ; i < voltage_smooth_vars.size ; i ++ )
	{
		voltage_smooth_vars.table[i] = 0 ; 
	}
	
	voltage_smooth_vars.index = 0;			

	voltage_smooth_vars.is_non_zero = FALSE ; 
}

/*--------------------------------------------------------------------------*/

