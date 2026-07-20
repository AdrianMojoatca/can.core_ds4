/*==========================================================================*/
// $Id: status_temp.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
Boolean status_set_temp (UInt8 value, Status status, Status_Src source)
{	
	switch(status)
	{
		case STATUS_TEMP_IN :
		{
			if(source == status_entry.updater[STATUS_TEMP_IN])
			{
				
					temperature.in = value ;
					temperature.in_ena = TRUE ;
					status_entry.is_valid[STATUS_TEMP_IN] = TRUE ;
				

				return TRUE ;
			}
		}
		break ;

		case STATUS_TEMP_OUT :
		{
			if(source == status_entry.updater[STATUS_TEMP_OUT])
			{
				
					temperature.out = value ;
					temperature.out_ena = TRUE ;
					status_entry.is_valid[STATUS_TEMP_OUT] = TRUE ;
				

				return TRUE ;
			}	
		}
		break ;

		case STATUS_TEMP_ENG :
		{
			if(source == status_entry.updater[STATUS_TEMP_ENG])
			{
				
					temperature.eng_coolant = value ;
					temperature.coolant_ena = TRUE ;
					status_entry.is_valid[STATUS_TEMP_ENG] = TRUE ;
				

				return TRUE ;
			}	
		}
		break ;
		
		case STATUS_TEMP_SENS :
		{
			if(source == status_entry.updater[STATUS_TEMP_SENS])
			{
				
					temperature.sensor = value ;
					temperature.sensor_ena = TRUE ;
					status_entry.is_valid[STATUS_TEMP_SENS] = TRUE ;
				

				return TRUE ;
			}	
		}
		break ;
		
	}		
		
	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt16 status_get_temp (Status status)
{
	switch(status)
	{
		case STATUS_TEMP_IN :
		{
			//if(status_entry.is_valid[STATUS_ENG_FUEL_LEVEL])
			{
				return temperature.in ;
			}	
		}
		//break ;

		case STATUS_TEMP_OUT :
		{
			//if(status_entry.is_valid[STATUS_ENG_BAT_VOLT])
			{
				return temperature.out ;
			}		
		}
		//break ;

		case STATUS_TEMP_ENG :
		{
			//if(status_entry.is_valid[STATUS_TEMP_ENG])
			{
				return temperature.eng_coolant ;
			}
		}
		//break ;
		
		case STATUS_TEMP_SENS :
		{
			//if(status_entry.is_valid[STATUS_TEMP_ENG])
			{
				return temperature.sensor ;
			}
		}
		//break ;
		
	}

	return -1 ;
}		
	
//--------------------------------------------------------------------------//

