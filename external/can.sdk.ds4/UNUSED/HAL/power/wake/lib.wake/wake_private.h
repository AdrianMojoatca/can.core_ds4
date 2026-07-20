/*==========================================================================*/
// $Id: wake_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _WAKE_PRIVATE_H_
#define _WAKE_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
#include "wake.h"
#include "per_exec.h"
#include "gpio_wake.h"
#include "config_gpio.h"
#include "dbg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//---------------------------------------------//

typedef struct s_Wake_Entry
{
    Wake_Func wake_func ;

    struct s_Wake_Entry * next ;

} Wake_Entry ;

//---------------------------------------------//
typedef struct s_Wake_Vars
{   
    UInt16   timeout     ;
    UInt16   timeout1    ;
    UInt16   timeout2    ;
    UInt8    counter1    ;
    UInt8    counter2    ;
    UInt32   tc          ;
    Boolean  do_wake ;        
    
    #ifdef PLATFORM_CM800
	#if CM800_DEI_BRAND == 1
        Cpu_Freq sleep_freq  ;
        Cpu_Freq current_freq;
	#endif
    #endif
    
} Wake_Vars ;

/*--------------------------------------------------------------------------*/

#define WAKE_TIMEOUT1                1000      // for 1 minute , the value is used after a wake_leave() call
#define WAKE_TIMEOUT2                100       // for 1 second , the value is used after the systems is waking up                                               

/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Per_Exec_Wake wake_state        (void);

#ifdef PLATFORM_CM800
#if CM800_DEI_BRAND == 1
Per_Exec_Wake wake_state1       (void);
#endif
#endif

void          wake_service      (Wake_Status wake_status);
Boolean       wake_service_ready(void);

void wake_cpu_sleep( void ) ;
void wake_cpu_wake ( void ) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Wake_Entry* wake_list ;
extern Wake_Vars   wake_vars ;
extern Wake_Entry* wake_list_ready;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#endif



