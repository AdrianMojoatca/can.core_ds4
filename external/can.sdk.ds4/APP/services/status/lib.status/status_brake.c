/*==========================================================================*/
// $Id: status_brake.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_private.h"
#include "bitfield.h"
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
Boolean status_set_brake (UInt8 value , Status_Brake status,  Status_Src source)
{
	if(source == status_entry.updater[STATUS_BRAKE] && status < STATUS_BRAKE_COUNT)
	{
		switch(status)
		{
			case STATUS_BRAKE_BYTE : 	brake.byte = value  ;	break ;

			default:					bitfield_wr (&brake.byte, REVERSE_BIT(status), REVERSE_BIT(status), value) ;	break ;
		}		
		
		status_entry.is_valid[STATUS_BRAKE] = TRUE	;

		return TRUE ;		
	}

	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt16 status_get_brake (Status_Brake status)
{
	if(/*status_entry.is_valid[STATUS_BRAKE] && */status < STATUS_BRAKE_COUNT)
	{
		switch(status)
		{
			case STATUS_BRAKE_BYTE :		return brake.byte ;

			default :						return bitfield_rd (&brake.byte, REVERSE_BIT(status), REVERSE_BIT(status)) ;
		}
	}
	return -1 ;

}

//--------------------------------------------------------------------------//


