/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __WDOG_V_H__
#define __WDOG_V_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "wake.h"
#include "nvfs.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define WDOG_V_VERSION  1

#ifndef VIRTUAL_WATCH_TIMEOUT

    #define VIRTUAL_WATCH_TIMEOUT  5

#else

    #if VIRTUAL_WATCH_TIMEOUT > 8 ||  VIRTUAL_WATCH_TIMEOUT < 1
        #error set VIRTUAL_WATCH_TIMEOUT > 0 and < WDT timeout (8 seconds)
    #endif

#endif


// Make sure the reset preamble function never runs longer than this
#ifndef PREAMBLE_TIMEOUT

   #define PREAMBLE_TIMEOUT        1000    // Value in milliseconds

#else

    #if PREAMBLE_TIMEOUT < 1000 ||  PREAMBLE_TIMEOUT > 10000
      #error set PREAMBLE_TIMEOUT > 1000 and < 10000
    #endif
    
#endif


//#define WDOG_V_DONT_RESET_ON_MEM_CHECK_FAILED
//#define WDOG_V_ONLY_LOG
//#define WDOG_V_DONT_USE

// Use this macro to embed code within a 'virtual watch' instance
// which will cause a reset if the embedded code takes too long to complete.
// * Code   = C code to embed within the watch

#define VIRTUAL_WATCH(code) { \
                                vWatch _vwatch_; \
                                _vwatch_ = wdog_v_add_watch(__MODULE__, __LINE__, VIRTUAL_WATCH_TIMEOUT); \
                                if (_vwatch_ == NULL) \
                                { \
                                    wdog_v_force_reset(WDOG_V_RESET_HEAP_CHECK,__MODULE__,__LINE__); \
                                } \
                                code ; \
                                wdog_v_release_watch(_vwatch_); \
                            }

// Use this macro to embed code within a 'virtual watch' instance
// which will cause a reset if the embedded code takes too long to complete.
// * Code    = C code to embed within the watch
// * Timeout = custom timeout in seconds 

#define VIRTUAL_WATCH_WITH_CUSTOM_TIMEOUT(code,timeout) { \
                                vWatch _vwatch_; \
                                _vwatch_ = wdog_v_add_watch(__MODULE__, __LINE__, timeout); \
                                if (_vwatch_ == NULL) \
                                { \
                                    wdog_v_force_reset(WDOG_V_RESET_HEAP_CHECK,__MODULE__,__LINE__); \
                                } \
                                code ; \
                                wdog_v_release_watch(_vwatch_); \
                            }



// Special version of VIRTUAL_WATCH when the start and end points are not in the same function
#define VIRTUAL_WATCH_START(period,_vwatch_,id) _vwatch_ = wdog_v_add_watch(id, 0, period); \
                                        if (_vwatch_ == NULL) \
                                        { \
                                            wdog_v_force_reset(WDOG_V_RESET_HEAP_CHECK, id, 0); \
                                        } \

#define VIRTUAL_WATCH_END(_vwatch_) wdog_v_release_watch(_vwatch_);

// Just a simple macro to make code easier to read (ex. 'Timeout = 5 SECONDS;')
#define SECONDS                     * 1

// A Virtual Watch instance
// This needs to be part of the public interface because the VIRTUAL_WATCH macro refers to it
typedef struct s_vWatch *           vWatch;

// This is a typedef for a function that can be registered as preamble to a force reset/
// Only 1 function can be registered.
// If such a function is registered, it will be called before a force reset and will
// be informed of the type of force reset, as well as a context flag to know if the reset
// has been forced from within a task or an Irq.
typedef void                        (* Wdog_v_Preamble_Func)(SInt16 reset_type, Boolean Irq) ;

// This is a typedef for functions that can be registered to check for queues.
// The virtual library will cyclically poll all registered functions, and force a reset
// if any of them returns FALSE;
// The implementation of the queue check logic is left to the library which has registered.
typedef Boolean                     (* Wdog_v_Queue_Check_Func)(void) ;

// Types of resets forced by the virtual watchdog library
typedef enum 
{
  WDOG_V_RESET_NORMAL = 0,
  WDOG_V_RESET_TIMEOUT,
  WDOG_V_RESET_WATCH_TIMEOUT,
  WDOG_V_RESET_STK_CHECK,
  WDOG_V_RESET_HEAP_CHECK,
  WDOG_V_RESET_MEM_CHECK,
  WDOG_V_RESET_DATA_ABORT,
  WDOG_V_RESET_PREFETCH_ABORT,
  WDOG_V_RESET_UNDEFINED_INSTRUCTION,
  WDOG_V_RESET_SCHEDULED,
  WDOG_V_RESET_QUEUE_CHECK,
  WDOG_V_RESET_FIRMWARE
} e_Wdog_v_Reset_Type;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
EXTERN_C_ENTER

void    wdog_v_init                 (void);
void    wdog_v_init_reset_on_sleep  (void);
void    wdog_v_force_reset          (e_Wdog_v_Reset_Type reset_type, char *source, UInt16 line);
void    wdog_v_register_preamble    (Wdog_v_Preamble_Func func);
void    wdog_v_register_queue_check (Wdog_v_Queue_Check_Func func, char *source);
vWatch  wdog_v_add_watch            (char *source, UInt16 line, UInt8 timeout_in_seconds);
Boolean wdog_v_release_watch        (vWatch instance);
void    wdog_v_log_reset_type       (void);
Boolean wdog_v_wake_check_is_active (void);
void    wdog_v_wake_check_activate  (Boolean active);

EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/


/*==========================================================================*/

#endif

