/*==========================================================================*/
// $Id: status_comfort.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
Boolean status_set_comfort (UInt8 value, Status status, Status_Src source)
{	
	switch(status)
	{
		case STATUS_DEFROST :
		{
			if(source == status_entry.updater[STATUS_DEFROST])
			{
				
					comfort2.defrost.state = value ;
					status_entry.is_valid[STATUS_DEFROST] = TRUE ;
				

				return TRUE ;
			}
		}
		break ;

		case STATUS_MIRROR_DRIVER :
		{
			if(source == status_entry.updater[STATUS_MIRROR_DRIVER])
			{
				
					comfort2.mirror.driver_state = value ;
					status_entry.is_valid[STATUS_MIRROR_DRIVER] = TRUE ;
				

				return TRUE ;
			}	
		}
		break ;

		case STATUS_MIRROR_PASS :
		{
			if(source == status_entry.updater[STATUS_MIRROR_PASS])
			{
				
					comfort2.mirror.passenger_state = value ;
					status_entry.is_valid[STATUS_MIRROR_PASS] = TRUE ;
				

				return TRUE ;
			}	
		}
		break ;

		case STATUS_SEAT_DRIVER :
		{
			if(source == status_entry.updater[STATUS_SEAT_DRIVER])
			{
				
					comfort2.seat.driver_state = value ;
					status_entry.is_valid[STATUS_SEAT_DRIVER] = TRUE ;
				

				return TRUE ;
			}
		}
		break ;

		case STATUS_SEAT_PASS :
		{
			if(source == status_entry.updater[STATUS_SEAT_PASS])
			{
				
					comfort2.seat.passenger_state = value ;
					status_entry.is_valid[STATUS_SEAT_PASS] = TRUE ;
				

				return TRUE ;
			}
		}
		break ;
	}		
		
	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt16 status_get_comfort (Status status)
{
	switch(status)
	{
		case STATUS_DEFROST :
		{
			//if(status_entry.is_valid[STATUS_DEFROST])
			{
				return comfort2.defrost.state ;
			}	
		}
		//break ;

		case STATUS_MIRROR_DRIVER :
		{
			//if(status_entry.is_valid[STATUS_MIRROR_DRIVER])
			{
				return comfort2.mirror.driver_state ;
			}		
		}
		//break ;

		case STATUS_MIRROR_PASS :
		{
			//if(status_entry.is_valid[STATUS_MIRROR_PASS])
			{
				return comfort2.mirror.passenger_state ;
			}
		}
		//break ;

		case STATUS_SEAT_DRIVER :
		{
			//if(status_entry.is_valid[STATUS_SEAT_DRIVER])
			{
				return comfort2.seat.driver_state ;
			}
		}
		//break ;

		case STATUS_SEAT_PASS :
		{
			//if(status_entry.is_valid[STATUS_SEAT_PASS])
			{
				return comfort2.seat.passenger_state ;
			}
		}
		//break ;
	}

	return -1 ;
}		
	
//--------------------------------------------------------------------------//

