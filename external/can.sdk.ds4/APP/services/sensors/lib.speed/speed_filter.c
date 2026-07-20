/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: speed_filter.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "speed_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Speed_Filter
{
    UInt8   size ;
    UInt8   index;
    UInt32* table;
    Boolean is_non_zero;
}Speed_Filter;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Speed_Filter speed_filter_vars; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void speed_filter_init( UInt8 size )
{
	speed_filter_vars.size =  size ;
		
	speed_filter_vars.table = (UInt32*)os_mem_request( sizeof(UInt32) * size ) ; 
	
	speed_filter_reset();		
}

//--------------------------------------------------------------------------//

UInt32 speed_filter_update( UInt32 instant )
{
	UInt32 value = 0 ; 

	UInt8  i ;
		
	speed_filter_vars.table[ speed_filter_vars.index ] = instant ; 
	
	speed_filter_vars.index = (speed_filter_vars.index + 1)%speed_filter_vars.size ; 

	if( !speed_filter_vars.index || (speed_filter_vars.size == 1) ) speed_filter_vars.is_non_zero = TRUE ;	
	
	for( i = 0 ; i < speed_filter_vars.size ; i ++ ) value += speed_filter_vars.table[i] ; 
	
	return speed_filter_vars.is_non_zero ? value / speed_filter_vars.size  : 0 ; 
}

//--------------------------------------------------------------------------//

void speed_filter_reset( void )
{
	UInt8 i ;
	
	for( i = 0 ; i < speed_filter_vars.size ; i ++ )
	{
		speed_filter_vars.table[i] = 0 ; 
	}
	
	speed_filter_vars.index = 0;			

	speed_filter_vars.is_non_zero = FALSE ; 
}

//--------------------------------------------------------------------------//

