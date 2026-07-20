/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __WDOG_V_PRIVATE_H__
#define __WDOG_V_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "wdog_v.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//------------------------------------------------------------------------//
// Application Watchdog Reset Code Addr
extern  unsigned Image$$ER_RAM_APP_WDOG_RESET_ID$$Base ;
#define WDOG_RESET_APP_CODE_ADDR ((unsigned) (& Image$$ER_RAM_APP_WDOG_RESET_ID$$Base))   

//------------------------------------------------------------------------//
// Virtual Watchdog Reset Source Addr
extern  char * Image$$ER_RAM_APP_VWDOG_RESET_SOURCE$$Base ;
#define WDOG_V_RESET_SOURCE_ADDR ((char *) (& Image$$ER_RAM_APP_VWDOG_RESET_SOURCE$$Base))   

//------------------------------------------------------------------------//
// Virtual Watchdog Reset Line Addr
extern  unsigned Image$$ER_RAM_APP_VWDOG_RESET_LINE$$Base ;
#define WDOG_V_RESET_LINE_ADDR ((unsigned) (& Image$$ER_RAM_APP_VWDOG_RESET_LINE$$Base))   

//------------------------------------------------------------------------//


// <h>  TASK Configuration
// <h>  Task 'wdog_v' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_WDOG_V_NAME    "wdog_v"
#define TASK_WDOG_V_PRI     60001
#define TASK_WDOG_V_STK     0x00C0

// <h>  Task 'wdt' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_WDT_NAME   "wdt"
#define TASK_WDT_PRI    400
#define TASK_WDT_STK    0x0040
// </h>

// Locations in RAM where we can store info about a forced reset.
// These locations are defined in the scatter file and will not be reset
// by the C startup code.
#define WDOG_V_RESET_APP_CODE *(volatile SInt16 *)(WDOG_RESET_APP_CODE_ADDR)
#define WDOG_V_RESET_SOURCE    (volatile char *)  (WDOG_V_RESET_SOURCE_ADDR)
#define WDOG_V_RESET_LINE     *(volatile SInt16 *)(WDOG_V_RESET_LINE_ADDR)

// Monitoring settings
#define WATCH_CHECK_RATE        1000    // Value in milliseconds
#define WDT_KICK_RATE           1000
#define MEM_CHECK_RATE          2000
#define ARM_ERROR_CHECK_RATE    100
#define QUEUE_CHECK_RATE        10000

#define MONITOR_HEAP_WARNING    95      // Value in %
#define MONITOR_HEAP_FAILURE    99

// Limits of the error log in NVFS
#define WDOG_V_MAX_ERRORS               3
#define WDOG_V_MAX_RESET_SOURCE_SIZE    8

// Limits of the virtual watch 
#define WDOG_V_WATCH_MIN_PERIOD 1
#define WDOG_V_WATCH_MAX_PERIOD 60    

// Virtual watchdog error log
typedef struct s_Wdog_v_Error
{
    UInt8   type;
    char    source[WDOG_V_MAX_RESET_SOURCE_SIZE];
    UInt16  line;

} Wdog_v_Error;

// Virtual watchdog NVFS entry
typedef struct s_Wdog_v_NVFS_Entry
{
    UInt8           reset_on_sleep;
    UInt8           err_count;
    Wdog_v_Error    err[WDOG_V_MAX_ERRORS];

} Wdog_v_NVFS_Entry ;

// Virtual watch entity
struct s_vWatch
{
    char    source[WDOG_V_MAX_RESET_SOURCE_SIZE] ;
    UInt16  line ;
    UInt8   timeout_in_seconds ;
    Boolean active ;
    UInt32  timestamp ;

};

// Linked list of virtual watch entities
typedef struct s_vWatch_List
{
    vWatch                  instance ;
    struct s_vWatch_List *  next ;

} vWatch_List;

// List of registered queue check functions
typedef struct s_Queue_Check_Entry
{
    char                            source[WDOG_V_MAX_RESET_SOURCE_SIZE];
    Wdog_v_Queue_Check_Func         func ;
    struct s_Queue_Check_Entry *    next ;

} Queue_Check_Entry ;


typedef struct s_Wdog_Preamble_Entry
{
    Wdog_v_Preamble_Func            func ;
    struct s_Wdog_Preamble_Entry*   next ;

} Wdog_Preamble_Entry ;

/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void wdog_v_task        (void);
void wdt_task           (void);
void wdog_v_tick        (void);

void wdog_v_preamble_execute(SInt16 reset_type, Boolean irq);

#ifdef WDOG_V_IDLE_MODE_ACTIVE

    void wdog_v_kick_idle_init( void );

#else

    #define  wdog_v_kick_idle_init()   ;

#endif

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern vWatch_List *            vwatch_list ;

extern Tsk                      wdog_v_tsk ;
extern Tsk                      wdt_tsk ;

extern UInt32                   wdog_v_ticks ;

extern volatile Boolean         data_abort_flag ;
extern volatile Boolean         prefetch_abort_flag ;
extern volatile Boolean         undefined_instruction_flag ;
extern volatile UInt32          exception_addr;

extern Wdog_v_NVFS_Entry        wdog_v_nvfs_entry ;

extern Wdog_Preamble_Entry*     preamble_list     ;

extern UInt32                   preamble_func_running_since;

extern Queue_Check_Entry *      queue_check_list;

extern const NVFS_Entry_Name    nvfs_wdog_v_data ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

