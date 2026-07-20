/*==========================================================================*/
// $Id: status_rke.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
Boolean status_set_rke (UInt16 value , Status_Rke status,  Status_Src source)
{
	if(source == status_entry.updater[STATUS_RKE] && status < STATUS_RKE_COUNT)
	{
		switch(status)
		{
			case STATUS_RKE_WORD : 	rke.word = value  ;	break ;

			default:				bitfield_wr (rke.byte, REVERSE_BIT(status), REVERSE_BIT(status), value) ;	break ;
		}		
		
		status_entry.is_valid[STATUS_RKE] = TRUE	;

		return TRUE ;		
	}

	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt32 status_get_rke (Status_Rke status)
{
	if(/*status_entry.is_valid[STATUS_RKE] &&*/ status < STATUS_RKE_COUNT)
	{
		switch(status)
		{
			case STATUS_RKE_WORD :		return rke.word ;

			default :					return bitfield_rd (rke.byte, REVERSE_BIT(status), REVERSE_BIT(status)) ;
		}
	}
	return -1 ;

}

//--------------------------------------------------------------------------//


