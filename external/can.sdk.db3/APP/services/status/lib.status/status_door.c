/*==========================================================================*/
// $Id: status_door.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_private.h"

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
Boolean status_set_door (UInt8 value , Status_Door status,  Status_Src source)
{
	if(source == status_entry.updater[STATUS_DOOR] && status < STATUS_DOOR_COUNT)
	{
		switch(status)
		{
			case STATUS_DOOR_BYTE :		door.byte = value ;		break ;

			default:					bitfield_wr (&door.byte, REVERSE_BIT(status), REVERSE_BIT(status), value) ;	break ;
		}		
		
		status_entry.is_valid[STATUS_DOOR] = TRUE	;

		return TRUE ;
	}

	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt16 status_get_door (Status_Door status)
{
	if(/*status_entry.is_valid[STATUS_DOOR] && */status < STATUS_DOOR_COUNT)
	{
		switch(status)
		{
			case STATUS_DOOR_BYTE :		return door.byte ;

			default :					return bitfield_rd (&door.byte, REVERSE_BIT(status), REVERSE_BIT(status)) ;
		}
	}
	return -1 ;
}	

//--------------------------------------------------------------------------//


