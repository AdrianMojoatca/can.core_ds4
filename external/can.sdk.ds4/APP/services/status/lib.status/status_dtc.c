/*==========================================================================*/
// $Id: status_dtc.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
Boolean status_set_dtc (UInt16 value , Status_Dtc status,  Status_Src source)
{
	if(source == status_entry.updater[STATUS_DTC] && status <= STATUS_DTC_COUNT)
	{
		switch(status)
		{
			case STATUS_DTC_COUNT :	dtc.count = value ;	return TRUE ;

			default:
			{
				if(status < STATUS_DTC_SIZE)
				{
					dtc.code._16_[status] = value ;
					status_entry.is_valid[STATUS_DTC] = TRUE ;
					return TRUE ;
				}
			}	
			break ;
		}		
	}

	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt32 status_get_dtc (Status_Dtc status)
{
	if(/*status_entry.is_valid[STATUS_DTC] &&*/ status <= STATUS_DTC_COUNT)
	{
		switch(status)
		{
			case STATUS_DTC_COUNT :		return dtc.count ;

			default :					
			{
				if(status < STATUS_DTC_SIZE)
				{
					return dtc.code._16_[status] ;
				}
			}
			break ;
		}
	}
	return -1 ;
}	

//--------------------------------------------------------------------------//


