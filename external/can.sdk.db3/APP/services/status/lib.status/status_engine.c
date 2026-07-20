/*==========================================================================*/
// $Id: status_engine.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
Boolean status_set_engine (UInt16 value, Status status, Status_Src source)
{	
	switch(status)
	{
		case STATUS_ENG_FUEL_LEVEL :
		{
			if(source == status_entry.updater[STATUS_ENG_FUEL_LEVEL])
			{
				
					engine.fuel_level = value ;
					status_entry.is_valid[STATUS_ENG_FUEL_LEVEL] = TRUE ;
				

				return TRUE ;
			}
		}
		break ;

		case STATUS_ENG_BAT_VOLT :
		{
			if(source == status_entry.updater[STATUS_ENG_BAT_VOLT])
			{
				
					engine.bat_volt = value ;
					status_entry.is_valid[STATUS_ENG_BAT_VOLT] = TRUE ;
				

				return TRUE ;
			}	
		}
		break ;

		case STATUS_ENG_RUN_TIME :
		{
			if(source == status_entry.updater[STATUS_ENG_RUN_TIME])
			{
				
					engine.run_time = value ;
					status_entry.is_valid[STATUS_ENG_RUN_TIME] = TRUE ;
				

				return TRUE ;
			}	
		}
		break ;

		case STATUS_ENG_RUNNING :
		{
			if(source == status_entry.updater[STATUS_ENG_RUNNING])
			{
				
					engine.running = value ;
					status_entry.is_valid[STATUS_ENG_RUNNING] = TRUE ;
				

				return TRUE ;
			}
		}
		break ;
		
        case STATUS_ENG_OEM_RS_EQUIPPED :
		{
			if(source == status_entry.updater[STATUS_ENG_OEM_RS_EQUIPPED])
			{
				
					engine.oem_rs_equipped = value ;
					status_entry.is_valid[STATUS_ENG_OEM_RS_EQUIPPED] = TRUE ;
				

				return TRUE ;
			}
		}
		break ;        

	}		
		
	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt32 status_get_engine (Status status)
{
	switch(status)
	{
		case STATUS_ENG_FUEL_LEVEL :
		{
			//if(status_entry.is_valid[STATUS_ENG_FUEL_LEVEL])
			{
				return engine.fuel_level ;
			}	
		}
		//break ;

		case STATUS_ENG_BAT_VOLT :
		{
			//if(status_entry.is_valid[STATUS_ENG_BAT_VOLT])
			{
				return engine.bat_volt ;
			}		
		}
		//break ;

		case STATUS_ENG_RUN_TIME :
		{
			//if(status_entry.is_valid[STATUS_ENG_RUN_TIME])
			{
				return engine.run_time ;
			}
		}
		//break ;

		case STATUS_ENG_RUNNING :
		{
			//if(status_entry.is_valid[STATUS_ENG_RUNNING])
			{
				return engine.running ;
			}
		}
		//break ;
        
		case STATUS_ENG_OEM_RS_EQUIPPED :
		{
			//if(status_entry.is_valid[STATUS_ENG_RUNNING])
			{
				return engine.oem_rs_equipped ;
			}
		}
		//break ;        
        
        
	}

	return -1 ;
}		
