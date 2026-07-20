/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: reg_table_build.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "reg_private.h"
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

//--------------------------------------------------------------------------//
//! \desc  Create the Reg_Table object used for registering
//!        Use this method if you want to derive from Reg_Table a new data type 

//! \assume -none- 

//! \notes  -none-

//! \param  atomic     Specify how to ensure the atomicity of read/write/modify operations
//!                    Use any combination of flags :
//!                      - REG_ATOMIC_USE_RES : if you want to ensure atomicity by using a Res object
//!                      - REG_ATOMIC_USE_IRQ : if you want to ensure atomicity by disabling the IRQ 
//!                      - REG_ATOMIC_USE_FIQ : if you want to ensure atomicity by disabling the FIQ 

//! \param  size       The size of memory block allocated for new object

//! \return \b Reg_Table    \b non-NULL if the object was sucessfully created
//!                         \b NULL if the method fails 

//--------------------------------------------------------------------------//

Reg_Table reg_table_build( Reg_Atomic atomic , UInt8 size )
{
    Reg_Table reg_table = (Reg_Table)os_mem_request( size ) ;
	
	if( reg_table )
	{     
    	reg_table->next  = NULL ;     
    	    	
    	reg_atomic_init( reg_table , atomic );
	}

    return reg_table ;
}

//--------------------------------------------------------------------------//
