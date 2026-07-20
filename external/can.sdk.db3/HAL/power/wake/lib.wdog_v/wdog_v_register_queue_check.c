/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v_register_queue_check.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <string.h>
 
#include "wdog_v_private.h"
 
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
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void wdog_v_register_queue_check (Wdog_v_Queue_Check_Func func, char *source)
{
	CRITICAL ;

	C_ENTER ;
    {
		/*add_to_list :*/
        {
          	Queue_Check_Entry * queue_check_entry = (Queue_Check_Entry *) os_mem_request (sizeof (Queue_Check_Entry)) ;
    
          	queue_check_entry -> func = func ;
          	queue_check_entry -> next = queue_check_list ;

			// We have to limit the size of the source string
    		memset ( queue_check_entry->source, ' ', sizeof(queue_check_entry->source));
			if (strlen(source) > sizeof(queue_check_entry->source))
			{
				memcpy(queue_check_entry->source, source, sizeof(queue_check_entry->source));
			}
			else
			{
				memcpy(queue_check_entry->source, source, strlen(source));
			}

          	queue_check_list = queue_check_entry ;
        }
    }
  C_LEAVE ;
}


