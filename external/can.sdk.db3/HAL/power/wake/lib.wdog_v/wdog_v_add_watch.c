/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v_add_watch.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void format_source_name(char *source, char *formatted_source);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

vWatch wdog_v_add_watch (char *source, UInt16 line, UInt8 timeout_in_seconds)
{
	CRITICAL ;

	Boolean 	already_instantiated 	= FALSE;
   	vWatch_List * entry					= NULL;
	vWatch		instance 				= NULL;
 	char 		source_[WDOG_V_MAX_RESET_SOURCE_SIZE];
		
	// In case of error, this function returns NULL
	// We don't log errors, we are too low-level; it's the caller's responsability

	// Sanity checks

	if (timeout_in_seconds < WDOG_V_WATCH_MIN_PERIOD) timeout_in_seconds = WDOG_V_WATCH_MIN_PERIOD;
	if (timeout_in_seconds > WDOG_V_WATCH_MAX_PERIOD) timeout_in_seconds = WDOG_V_WATCH_MAX_PERIOD;

	// ------------------------------------------------------------------------
	// The source size is limited to WDOG_V_MAX_RESET_SOURCE_SIZE bytes because
	// we want to save it to NVFS and we don't want to use up too much space.
	// ------------------------------------------------------------------------

	format_source_name(source, source_);

	// ------------------------------------------------------------------------
	// Search the linked list to see if the instance was already created before
	// In that case, we don't allocate memory, we simply reactivate it.
	// An instance can be recognized from the data pair SOURCE:LINE.
	// ------------------------------------------------------------------------

	for (entry = vwatch_list ; entry ; entry = entry->next)
	{
		if (entry->instance->line == line && 
				memcmp(	entry->instance->source, source_, WDOG_V_MAX_RESET_SOURCE_SIZE) == 0) break;
	}
	if (!entry)
	{
		// This is a new instance - memory must be allocated 
		instance = (vWatch) os_mem_request (sizeof *instance) ;
	}
	else
	{
		// This instance was previously instantiated, let's just reinitialize it
		instance = entry->instance;
		already_instantiated = TRUE;
	}

	// ------------------------------------------------------------------------
	// Initialize the instance
	// ------------------------------------------------------------------------

	if (instance != NULL)
	{
		// Protect this because the list is global and different tasks could call us at the same time
		C_ENTER ;
	    {
			// Identify our virtual watchdog instance, for debugging/logging purposes
			if (!already_instantiated)
			{
				memcpy(instance->source, source_, WDOG_V_MAX_RESET_SOURCE_SIZE);
				instance->line = line;
			}
				
			// Instance settings
			instance->timeout_in_seconds = timeout_in_seconds;
			instance->active             = TRUE;
			instance->timestamp          = wdog_v_ticks;

			// Add new instance to watch list
			if (!already_instantiated)
			{
				entry = (vWatch_List *) os_mem_request (sizeof (vWatch_List)) ;
		      	entry -> instance = instance ;
		       	entry -> next = vwatch_list ;
		       	vwatch_list = entry ;
			}
	    }
		C_LEAVE ;
	}

	return instance;
}

//--------------------------------------------------------------------------//

static void format_source_name(char *source, char *formatted_source)
{
	int 		i = strlen(source);
	int			j;

	// Formatted source name must be padded with white spaces
	memset(formatted_source, ' ', WDOG_V_MAX_RESET_SOURCE_SIZE);

	// Formatted source name must not contain extensions (.c, .h, .cpp, .hpp, etc)
	for ( j=i-1; j>0 && source[j] != '.'; j--);
	if (j > 0) i = j;

	// Original source name is short, just copy it
	if (i <= WDOG_V_MAX_RESET_SOURCE_SIZE)
	{
		memcpy ( formatted_source, source, i);
	}

	// Original source name too long, keep X first bytes and X last bytes
	else
	{
		j = WDOG_V_MAX_RESET_SOURCE_SIZE / 2;
		memcpy ( formatted_source, source, j);
		memcpy ( &formatted_source[j], &source[i-j], j);
	}
}

#if 0
// Let's keep thig debug function in here until we are satisfied that
// the library is stable - 
void wdog_v_show_vwatch(void)
{
	char          src[9];
	UInt32        line = 0;
	UInt8         i = 0;
   	vWatch_List * entry					= NULL;

	for (i=0, entry = vwatch_list ; entry ; entry = entry->next, i++)
	{
	  line = entry->instance->line;
	  memcpy ( src, entry->instance->source, 8 );
	  src[8] = 0;
	  dbg_tx_text("INSTANCE #"); 
	  dbg_tx_h8(i);
	  dbg_tx_text("\r\n");
	  dbg_tx_text(" source " );
	  dbg_tx_text(src);
	  dbg_tx_text("\r\n");
	  dbg_tx_text(" line   " );
	  dbg_tx_h16(line);
	  dbg_tx_text("\r\n");
	}
}
#endif

