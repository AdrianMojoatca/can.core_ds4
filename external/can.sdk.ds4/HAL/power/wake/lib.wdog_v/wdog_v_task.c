/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v_task.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <string.h>

#include "wdog_v_private.h"
#include "wdog.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// These are the subtasks performed by the main virtual watchdog task.
// These enums must match the 'periods' array for the gte below.
enum e_Monitor_Subtasks
{
  SUBTASK_VWATCH_CHECK = 0,
  SUBTASK_MEM_CHECK,
  SUBTASK_ARM_ERROR_CHECK,
  SUBTASK_QUEUE_CHECK,

  SUBTASK_COUNT
};

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void monitor_stk         (void);
static void monitor_heap		(void);
static void monitor_unused_mem	(void);
static char hex_nibble          (UInt8 nibble);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Gte gte = NULL;

// Use GTE for scheduling subtasks
// The order of the periods must match enum 'e_Monitor_Subtasks'
static TskTimeout periods[SUBTASK_COUNT] = { 
	WATCH_CHECK_RATE,
	MEM_CHECK_RATE,
	ARM_ERROR_CHECK_RATE,
	QUEUE_CHECK_RATE
};

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void wdog_v_task (void)
{
#ifndef WDOG_V_DONT_USE

	vWatch_List *	curr 		= NULL;
	vWatch 			instance 	= NULL;
	Boolean			done_once	= FALSE;

	wdog_v_ticks = 0;

	instance = (vWatch) os_mem_request (sizeof *instance) ;

	gte = os_gte_create ( periods, SUBTASK_COUNT );

	for(;;)
	{
		UInt16 timeout_bits = os_gte_wait(gte);

		// ------------------------------------------------------------------
		// Virtual watch checks
		// All active instances of the virtual watch created with macro 
		// VIRTUAL_WATCH are checked on a periodic basis.
		// ------------------------------------------------------------------

		if ((1u << SUBTASK_VWATCH_CHECK) & timeout_bits)
		{
			for (curr = vwatch_list ; curr ; curr = curr->next)
			{	
				ATOMIC(memcpy(instance, curr->instance, sizeof *instance));
	
				if (instance->active)
				{
					UInt32 interval;

					if (wdog_v_ticks < instance->timestamp) 
					{
						// Handle rollover of system ticks
						interval = (wdog_v_ticks + (0xFFFFFFFF - instance->timestamp));
					}
					else
					{
						interval = wdog_v_ticks - instance->timestamp;
					}
					if ( interval >= instance->timeout_in_seconds * 1000)
					{
						wdog_v_force_reset(WDOG_V_RESET_WATCH_TIMEOUT, instance->source, instance->line);
					}
				}
			}
		}

		// ------------------------------------------------------------------
		// Task stacks, Heap & unused memory checks
		// ------------------------------------------------------------------

		if ((1u << SUBTASK_MEM_CHECK) & timeout_bits)
		{
			monitor_stk () ;
            monitor_heap();
			monitor_unused_mem();
		}

		// ------------------------------------------------------------------
		// ARM error checks (data abort, prefetch abort, undef instruction)
		// ------------------------------------------------------------------

		if ((1u << SUBTASK_ARM_ERROR_CHECK) & timeout_bits)
		{
			if (data_abort_flag)
			{
				// Format exception address in HEX as the source
				char source[WDOG_V_MAX_RESET_SOURCE_SIZE];
				UInt32 addr = exception_addr;
				UInt8  i;
				for (i=0; i<WDOG_V_MAX_RESET_SOURCE_SIZE; i++)
				{
					source[WDOG_V_MAX_RESET_SOURCE_SIZE-i-1] = hex_nibble(addr & 0x0000000F);
					addr >>= 4;
				}
				wdog_v_force_reset(WDOG_V_RESET_DATA_ABORT, source, 0);
			}
			if (prefetch_abort_flag)
			{
				wdog_v_force_reset(WDOG_V_RESET_PREFETCH_ABORT, "", 0);
			}
			if (undefined_instruction_flag)
			{
				wdog_v_force_reset(WDOG_V_RESET_UNDEFINED_INSTRUCTION, "", 0);
			}
		}

		// ------------------------------------------------------------------
		// Queue checks
		// ------------------------------------------------------------------

		if ((1u << SUBTASK_QUEUE_CHECK) & timeout_bits)
		{
			Queue_Check_Entry * curr ;
		
			for (curr = queue_check_list ; curr ; curr = curr->next)
			{
				if (!curr->func())
				{
					wdog_v_force_reset(WDOG_V_RESET_QUEUE_CHECK, curr->source, 0);
				}
			}
		}
	}

#endif
}

//--------------------------------------------------------------------------//

void wdt_task (void)
{
#ifndef WDOG_V_DONT_USE

	// Periodically kick the hardware watchdog.
	for (;;)
	{
		wdog_kick();
		os_tsk_wait(WDT_KICK_RATE);
	}

#endif
}

//--------------------------------------------------------------------------//

static void monitor_stk( void )
{
    Tsk     tsk      ;
    StkInfo stk_info ;
  	TskInfo tsk_info ;

	// Iterate through all tasks and check the integrity of the fences    
    for (os_tsk_list_init (& tsk) ; tsk ; os_tsk_list_next (& tsk))
    {     
  		os_tsk_info (tsk , &tsk_info) ;
        os_stk_info (tsk , &stk_info) ;        

        if( !stk_info.fence_ok_beg || !stk_info.fence_ok_end)
        {            
			wdog_v_force_reset(WDOG_V_RESET_STK_CHECK, (char *)tsk_info.tsk_name, 0);
        }        
    }
}

//--------------------------------------------------------------------------//

static void monitor_heap( void )
{        
	UInt16 heap_size;
	UInt16 heap_used;
	UInt16 heap_usage;

    MemInfo mem_info ;    

    os_mem_info( &mem_info );
        
	heap_size = (UInt16)((UInt32)mem_info.addr_max - (UInt32)mem_info.addr_min);
	heap_used = (UInt16)((UInt32)mem_info.addr_next - (UInt32)mem_info.addr_min);
	heap_usage = (heap_used * 100) / heap_size;

#ifdef WDOG_V_DONT_RESET_ON_MEM_CHECK_FAILED

    if ( heap_usage >= MONITOR_HEAP_WARNING )
    {
		TRACE("\r\n***** HEAP CHECK WARNING *****\r\n");
		for (;;)
		{
			os_tsk_wait(50);
			OUT_LED_GREEN( FALSE ) ;
			OUT_LED_RED( FALSE ) ;
			os_tsk_wait(50);
			OUT_LED_RED( TRUE ) ;
		}
	}

#else

    if ( heap_usage >= MONITOR_HEAP_FAILURE )
    {
		wdog_v_force_reset(WDOG_V_RESET_HEAP_CHECK, "", 0);
    }

#endif

}

//--------------------------------------------------------------------------//

static void monitor_unused_mem(void)
{
    MemInfo mem_info ;    
    MemInfo mem_info_2 ;    
	UInt32 *p;

    os_mem_info( &mem_info );

	for (p=(UInt32 *)mem_info.addr_next; p < mem_info.addr_max; p++)
	{
		if (*p != 0xDEADBEEF)
		{
			// An error has been detected; it may be because someone has called os_mem_request() during our loop
			os_mem_info ( &mem_info_2 );
			if (mem_info_2.addr_next != mem_info.addr_next)
			{
				// Simply defer the memory check - the situation we just detected should be a rare
				// occurence in an embedded environment. The important thing is to avoid false positives.
				return;
			};

#ifdef WDOG_V_DONT_RESET_ON_MEM_CHECK_FAILED

			TRACE("\r\n***** UNUSED MEMORY CHECK FAILED *****\r\n");
			for (;;)
			{
				os_tsk_wait(50);
				OUT_LED_GREEN( FALSE ) ;
				OUT_LED_RED( FALSE ) ;
				os_tsk_wait(50);
				OUT_LED_RED( TRUE ) ;
			}

#else

			wdog_v_force_reset(WDOG_V_RESET_MEM_CHECK, "", 0);

#endif

		}
	}
}

//--------------------------------------------------------------------------//

static char hex_nibble( UInt8 nibble )
{
	if (nibble < 10) return '0' + nibble;
	return 'A' + (nibble - 10);
}

