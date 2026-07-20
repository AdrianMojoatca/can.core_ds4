/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk_flash.c 1106 2010-04-27 13:30:39Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_can_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void sense_other_doors( Boolean instant_level );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Ied  sense_rs_ied_door ; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_other_doors_init( void )
{
     
}            

//--------------------------------------------------------------------------//

Boolean sense_rs_other_doors_can( Boolean instant_level )
{
    sense_other_doors( instant_level );                

    return get_other_door_status();
}    

//--------------------------------------------------------------------------//

void sense_other_doors( Boolean instant_level )
{
    Boolean old_status =  get_other_door_status();

    set_other_door_data_status( instant_level );

	if( old_status != get_other_door_status() )
    {
        door.bit.front_pass  = get_other_door_status();
		door.bit.rear_driver = get_other_door_status();
		door.bit.rear_pass   = get_other_door_status();
					
	    TRACE_SENSE_RS_CAN("\n\r[OTHER DOOR %s]\n\r" , get_other_door_status() ? "OPEN":"CLOSE" );	              

		if( get_other_door_status() )
		{
			sense_rs_can_door_open_data();
		}
		else
		{
			sense_rs_can_door_close_data();
		}
   }
		
}

//--------------------------------------------------------------------------//

							

