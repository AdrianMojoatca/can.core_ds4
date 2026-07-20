/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_status_save.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
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

//--------------------------------------------------------------------------//

void dei_ss_status_save( Wake_Status wake_status )
{  	
static Dei_Ss_Status status ;
	
	if(wake_status == s_SLEEP)
	{
		ATOMIC
			(
				status.arm   = get_ss_arm()  ;//ss.armed ; 
				status.valet = get_ss_valet();//ss.valet ;

				status.trigger_active = get_ss_trigger();      //ss.trigger_active ; 
				status.trigger_source = get_ss_last_trigger(); //ss.last_trigger   ;

				status.event_memory[0]= get_ss_memory_trigger_event(0);//ss.event_memory[0];
				status.event_memory[1]= get_ss_memory_trigger_event(1);//ss.event_memory[1];
#ifdef PLATFORM_933   //DS4 carries 4 history values
				status.event_memory[2]= get_ss_memory_trigger_event(2);//ss.event_memory[0];
				status.event_memory[3]= get_ss_memory_trigger_event(3);//ss.event_memory[1];
#endif		
			)

		nvfs_wr( nvfs_ss_stat ,&status ) ;    
	}
}

//--------------------------------------------------------------------------//


