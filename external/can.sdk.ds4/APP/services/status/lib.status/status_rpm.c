/*==========================================================================*/
// $Id: status_rpm.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
Boolean status_set_rpm (UInt16 value, Status_Rpm status, Status_Src source)
{
	if(source == status_entry.updater[STATUS_RPM])
	{	
		switch(status)
		{
			case STATUS_RPM_WORD : 		rpm.word = value ; 		break ;

			case STATUS_RPM_HIGH :		rpm.byte[1] = value ;	break ;

			case STATUS_RPM_LOW :		rpm.byte[0] = value ;	break ;
		}

		status_entry.is_valid[STATUS_RPM] = TRUE ;

		return TRUE ;
	}

	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt32 status_get_rpm (Status_Rpm status)
{
	//if(status_entry.is_valid[STATUS_RPM])
	{

		switch(status)
		{
			case STATUS_RPM_WORD :		return rpm.word ;
			
			case STATUS_RPM_HIGH :		return rpm.byte[1] ;	

			case STATUS_RPM_LOW :		return rpm.byte[0] ;	
		}
	}
	
	return -1 ;	
}

//--------------------------------------------------------------------------//

