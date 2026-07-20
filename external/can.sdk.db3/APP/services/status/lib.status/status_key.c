/*==========================================================================*/
// $Id: status_key.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
Boolean status_set_key (UInt8 value , Status_Key status,  Status_Src source)
{
	if(source == status_entry.updater[STATUS_KEY] && status < STATUS_KEY_COUNT)
	{
		switch(status)
		{
			case STATUS_KEY_BYTE : 	key.byte = value  ;	break ;

			default:				bitfield_wr (&key.byte, REVERSE_BIT(status), REVERSE_BIT(status), value) ;	break ;
		}		
		
		status_entry.is_valid[STATUS_KEY] = TRUE	;

		return TRUE ;		
	}

	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt16 status_get_key (Status_Key status)
{
	if(/*status_entry.is_valid[STATUS_KEY] && */status < STATUS_KEY_COUNT)
	{
		switch(status)
		{
			case STATUS_KEY_BYTE :		return key.byte ;

			default :					return bitfield_rd (&key.byte, REVERSE_BIT(status), REVERSE_BIT(status)) ;
		}
	}
	return -1 ;

}

//--------------------------------------------------------------------------//


