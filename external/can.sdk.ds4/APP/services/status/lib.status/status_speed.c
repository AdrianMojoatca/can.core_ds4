/*==========================================================================*/
// $Id: status_speed.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
Boolean status_set_speed (UInt8 value, Status_Speed status, Status_Src source)
{
	if(source == status_entry.updater[STATUS_SPEED])
	{
		switch(status)
		{
			case STATUS_SPEED_BYTE :			speed.byte = value 			;	break ;

			case STATUS_SPEED_IS_DETECTED :		speed.bit.is_detected = value   ;	break ;		
		}
				
		status_entry.is_valid[STATUS_SPEED] = TRUE	;

		return TRUE ;
	}

	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt16 status_get_speed (Status_Speed status)
{
	//if(status_entry.is_valid[STATUS_SPEED])
	{
		switch(status)
		{
			case STATUS_SPEED_BYTE :			return speed.byte ;
	
			case STATUS_SPEED_IS_DETECTED :		return speed.bit.is_detected ;
		}				
	}
	
	return -1 ;			
}

//--------------------------------------------------------------------------//

