/*==========================================================================*/
// $Id: status_tpms.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
Boolean status_set_tpms (UInt8 value, Status_TPMS status, Status_Src source)
{
	if(source == status_entry.updater[STATUS_TPMS])
	{	
		switch(status)
		{
			case STATUS_TPMS_LIGHT : 			tpms.light_status = value ; 	break ;

			case STATUS_TPMS_FRONT_DRIVER :		tpms.front_driver = value ;		break ;

			case STATUS_TPMS_FRONT_PASSENGER :	tpms.front_passenger = value ;	break ;

			case STATUS_TPMS_REAR_DRIVER :		tpms.rear_driver = value ;		break ;

			case STATUS_TPMS_REAR_PASSENGER :	tpms.rear_passenger = value ;	break ;
		}

		status_entry.is_valid[STATUS_TPMS] = TRUE ;

		return TRUE ;
	}

	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt16 status_get_tpms (Status_TPMS status)
{
	//if(status_entry.is_valid[STATUS_TPMS])
	{

		switch(status)
		{
			case STATUS_TPMS_LIGHT : 			return(tpms.light_status) ; 

			case STATUS_TPMS_FRONT_DRIVER :		return(tpms.front_driver) ;	

			case STATUS_TPMS_FRONT_PASSENGER :	return(tpms.front_passenger) ;

			case STATUS_TPMS_REAR_DRIVER :		return(tpms.rear_driver) ;	

			case STATUS_TPMS_REAR_PASSENGER :	return(tpms.rear_passenger) ;	
		}
	}
	
	return -1 ;	
}

//--------------------------------------------------------------------------//

