/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tacho_filter.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "tacho_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Tacho_Filter
{
    UInt8   size ;
    UInt8   index;
    UInt32* table;
    Boolean is_non_zero;
}Tacho_Filter;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Tacho_Filter tacho_filter_vars; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void tacho_filter_init( UInt8 size )
{
	tacho_filter_vars.size =  size ;
		
	tacho_filter_vars.table = (UInt32*)os_mem_request( sizeof(UInt32) * size ) ; 
	
	tacho_filter_reset();		
}

//--------------------------------------------------------------------------//

UInt32 tacho_filter_update( UInt32 instant )
{
	UInt32 value = 0 ; 

	UInt8  i ;
		
	tacho_filter_vars.table[ tacho_filter_vars.index ] = instant ; 
	
	tacho_filter_vars.index = (tacho_filter_vars.index + 1)%tacho_filter_vars.size ; 

	if( !tacho_filter_vars.index || (tacho_filter_vars.size == 1) ) tacho_filter_vars.is_non_zero = TRUE ;	
	
	for( i = 0 ; i < tacho_filter_vars.size ; i ++ ) value += tacho_filter_vars.table[i] ; 
	
	return tacho_filter_vars.is_non_zero ? value / tacho_filter_vars.size  : 0 ; 
}

//--------------------------------------------------------------------------//

void tacho_filter_reset( void )
{
	UInt8 i ;
	
	for( i = 0 ; i < tacho_filter_vars.size ; i ++ )
	{
		tacho_filter_vars.table[i] = 0 ; 
	}
	
	tacho_filter_vars.index = 0;			

	tacho_filter_vars.is_non_zero = FALSE ; 
}

//--------------------------------------------------------------------------//

