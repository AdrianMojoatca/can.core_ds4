/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <string.h>

#include "wdog_v_private.h"
#include "wdog.h"
#include "dbg.h"
#include "nvfs_usr.h"
#include "sys.h"
#ifdef DEV_LPC_236x
	#include "nvram_usr.h"
#endif
 
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
                              
static void 	add_to_error_log		(SInt16 type);
static void 	init_unused_mem			(void);
static void 	force_reset_on_sleep	(Wake_Status wake_status);
static Boolean 	init_nvfs_data			(void);
static void 	clear_source			(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void wdog_v_init ()
{
#ifndef WDOG_V_DONT_USE

	// Read or create the NVFS variable
	init_nvfs_data();

	// Check previous reset - if we caused it, add the reset info in the NVFS error log.
	wdog_v_log_reset_type();

	// Prepare the reset code for a real hardware WDT reset
	WDOG_V_RESET_APP_CODE = WDOG_V_RESET_TIMEOUT ;

	// Initialize unused memory to check for corruption later
	init_unused_mem();

	// Start the high-priority virtual watchdog monitoring task
  	wdog_v_tsk = os_tsk_create (wdog_v_task, TASK_WDOG_V_PRI , TASK_WDOG_V_STK , 0 , TASK_WDOG_V_NAME) ;
  	os_tsk_start (wdog_v_tsk, 0) ;

	// Start the low-priority WDT kicking task
  	wdt_tsk = os_tsk_create (wdt_task, TASK_WDT_PRI, TASK_WDT_STK , 0 , TASK_WDT_NAME) ;
  	os_tsk_start (wdt_tsk, 0) ;

	// Initialize and enable the hardware WDT
#ifndef WDOG_V_ONLY_LOG
	// When using JTAG breakpoints, disable the WDT; otherwise, MCU will reset during breakpoints
	#if DBG_JTAG == 0
		wdog_init( WDOG_CLK_MODE_PCLK , 0x2000000 );	// approx 8 seconds @ 72 MHz
		wdog_enable();
	#endif
#endif

	// Register to system ticks - will be used for timestamping the virtual watch instances
	// Important! do this after the WDT is setup, in case the service routines block before we have time to setup the WDT
	sys_timer_register (wdog_v_tick , 1) ;

    wdog_v_kick_idle_init();	

#endif
}

//--------------------------------------------------------------------------//

void wdog_v_init_reset_on_sleep (void)
{
#ifndef WDOG_V_DONT_USE

	// IMPORTANT! This function is decoupled from wdog_v_init because it must be
	//            called as soon as possible to ensure we are the 1st entity
	//            to use wake_register (the 1st entity will be the last one called
	//			  in the linked list.

	// Read or create the NVFS variable
	init_nvfs_data();

	// Register to wake to force reset upon sleep - if this feature is enabled
	if (wdog_v_nvfs_entry.reset_on_sleep != 0)
	{
		wake_register (force_reset_on_sleep);
	}

#endif
}

//--------------------------------------------------------------------------//


// Maybe this function should be in its own source file
// But originally it was a static function only called by init
// Now we added a debug mode where force_reset() can call this function...
void wdog_v_log_reset_type(void)
{
#if DBG_WDOG_V == 1
    static char source[WDOG_V_MAX_RESET_SOURCE_SIZE+1];        
#endif

    e_Wdog_v_Reset_Type type;

	// Check the reset source using register RSIR
	// We cannot use WDMOD because it's already been cleared by the bootloader
	UInt8 reset_source = RSIR;	  

	// Clear RSIR
	RSIR = 0xFF;

#if DBG_WDOG_V == 1
	dbg_tx_text("\r\n***** WDOG_V: RSIR = "); dbg_tx_h16(reset_source); dbg_tx_text("\r\n");
#endif

	// Only BIT(2) will be raised for a true watchdog reset
	if (reset_source != 0x04) 
	{
#if DBG_WDOG_V == 1
		dbg_tx_text("\r\n***** WDOG_V: Not a watchdog reset, resuming...\r\n");
#endif
		return;
	}

	type = (e_Wdog_v_Reset_Type) WDOG_V_RESET_APP_CODE;

#if DBG_WDOG_V == 1
	dbg_tx_text("\r\n***** WDOG_V: RESET_TYPE = "); dbg_tx_h16(type); dbg_tx_text("\r\n");
#endif

	//
	// Show some information on the debug port and add reset to log if needed
	//
#if DBG_WDOG_V == 1
	memcpy ( source, (char *)WDOG_V_RESET_SOURCE, WDOG_V_MAX_RESET_SOURCE_SIZE);
	source[WDOG_V_MAX_RESET_SOURCE_SIZE] = 0;
#endif

	switch(type)
	{
	  	case WDOG_V_RESET_TIMEOUT:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_TIMEOUT\r\n");
#endif
			clear_source();
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_WATCH_TIMEOUT:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_WATCH_TIMEOUT\r\n");
			dbg_tx_text("***** C Module: ");
			dbg_tx_text(source);
			dbg_tx_text("\r\n***** Line #: ");
			dbg_tx_u16(WDOG_V_RESET_LINE);
			dbg_tx_text("\r\n");
#endif
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_STK_CHECK:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_STACK_CHECK\r\n");
			dbg_tx_text("***** Task: ");
			dbg_tx_text(source);
			dbg_tx_text("\r\n");
#endif
			WDOG_V_RESET_LINE = 0;
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_HEAP_CHECK:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_HEAP_CHECK\r\n");
#endif
			clear_source();
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_MEM_CHECK:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_MEM_CHECK\r\n");
#endif
			clear_source();
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_DATA_ABORT:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_DATA_ABORT\r\n");
			dbg_tx_text("***** Addr: ");
			dbg_tx_text(source);
			dbg_tx_text("\r\n");
#endif
			WDOG_V_RESET_LINE = 0;
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_PREFETCH_ABORT:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_PREFETCH_ABORT\r\n");
#endif
			clear_source();
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_UNDEFINED_INSTRUCTION:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_UNDEFINED_INSTRUCTION\r\n");
#endif
			clear_source();
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_SCHEDULED:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_SCHEDULED\r\n");
#endif
			clear_source();
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_QUEUE_CHECK:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_QUEUE_CHECK\r\n");
			dbg_tx_text("***** Source: ");
			dbg_tx_text(source);
			dbg_tx_text("\r\n");
#endif
			WDOG_V_RESET_LINE = 0;
			add_to_error_log(type);
			break;

	  	case WDOG_V_RESET_FIRMWARE:
#if DBG_WDOG_V == 1
			dbg_tx_text("\r\n***** WDOG_V_RESET_FIRMWARE\r\n");
			dbg_tx_text("***** C Module: ");
			dbg_tx_text(source);
			dbg_tx_text("\r\n***** Line #: ");
			dbg_tx_u16(WDOG_V_RESET_LINE);
			dbg_tx_text("\r\n");
#endif
			add_to_error_log(type);
			break;


		default:
			break;
	}
}

//--------------------------------------------------------------------------//

static Boolean init_nvfs_data (void)
{
	static Wdog_v_NVFS_Entry curr_entry ;  

	// Try to read the NVFS data
	switch (nvfs_rd (nvfs_wdog_v_data , &wdog_v_nvfs_entry))
    {
      case NVFS_OK : return (TRUE) ;  
    }   

	// Oops, data does not exits, create it with default values
	curr_entry.reset_on_sleep = 0;
	curr_entry.err_count = 0;
	switch (nvfs_wr_create (nvfs_wdog_v_data, &curr_entry , sizeof (Wdog_v_NVFS_Entry)))
    {
	  // Important! Leave this usr_commit: if wdog_v causes a reset, we will never go to sleep and commit would never happen
      case NVFS_OK : return nvram_usr_commit() && init_nvfs_data() ;
    }      

  	return FALSE ;
}

//--------------------------------------------------------------------------//

static UInt8 reset_on_sleep_counter = 0;

static void force_reset_on_sleep(Wake_Status wake_status)
{
	if (wake_status == s_SLEEP)
	{
		if (++reset_on_sleep_counter >= wdog_v_nvfs_entry.reset_on_sleep)
		{
			reset_on_sleep_counter = 0;
			wdog_v_force_reset(WDOG_V_RESET_SCHEDULED, "", 0);
		}
	}
}

//--------------------------------------------------------------------------//

static void init_unused_mem(void)
{
    MemInfo mem_info ;    
	UInt32 *p;

	// Fill unused memory with a known pattern which can be checked for corruption later
    os_mem_info( &mem_info );
	for (p=(UInt32 *)mem_info.addr_next; p < mem_info.addr_max; p++)
	{
		*p = 0xDEADBEEF;
	}
}

//--------------------------------------------------------------------------//

static void add_to_error_log(SInt16 type)
{
	int i;

	if (wdog_v_nvfs_entry.err_count == WDOG_V_MAX_ERRORS)
	{
		// Too many log entries already, make room
		for (i=0; i<WDOG_V_MAX_ERRORS-1; i++)
		{
			wdog_v_nvfs_entry.err[i].type = wdog_v_nvfs_entry.err[i+1].type;
			memcpy(	wdog_v_nvfs_entry.err[i].source, wdog_v_nvfs_entry.err[i+1].source, WDOG_V_MAX_RESET_SOURCE_SIZE);
			wdog_v_nvfs_entry.err[i].line = wdog_v_nvfs_entry.err[i+1].line;
		}
		wdog_v_nvfs_entry.err_count = WDOG_V_MAX_ERRORS - 1;
	}

	// Add the error entry in the log
	wdog_v_nvfs_entry.err[wdog_v_nvfs_entry.err_count].type = type;
	memcpy(	wdog_v_nvfs_entry.err[wdog_v_nvfs_entry.err_count].source, 
			(char *)WDOG_V_RESET_SOURCE, WDOG_V_MAX_RESET_SOURCE_SIZE);
	wdog_v_nvfs_entry.err[wdog_v_nvfs_entry.err_count].line = WDOG_V_RESET_LINE;
	wdog_v_nvfs_entry.err_count++;

	// Commit to NVFS
  	switch (nvfs_wr (nvfs_wdog_v_data, &wdog_v_nvfs_entry))
    {
	  // Important! Leave this usr_commit: if wdog_v causes a reset, we will never go to sleep and commit would never happen
      case NVFS_OK : nvram_usr_commit () ;
    }      
}

//--------------------------------------------------------------------------//

static void clear_source(void)
{
	// Not all forced reset types have the following info; clear it for a cleaner log
	memset ((char *)WDOG_V_RESET_SOURCE, ' ', WDOG_V_MAX_RESET_SOURCE_SIZE);
	WDOG_V_RESET_LINE = 0;
}

