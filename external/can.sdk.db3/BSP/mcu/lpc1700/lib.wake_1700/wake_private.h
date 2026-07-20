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
#include "dbg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


#define CLOCK_SETUP          1
#define SCS_Val              0x00000021
#define CLKSRCSEL_Val        0x00000001
#define PLLCFG_Val           0x0000000B
#define CCLKCFG_Val          0x00000003
#define USBCLKCFG_Val        0x00000005
#define PCLKSEL0_Val         0x00000000
#define PCLKSEL1_Val         0x00000000

#ifndef PLATFORM_933
#define PCONP_Val            0x00000000
#else
#define PCONP_Val            0x0200020A
#endif


#define OSCRANGE             (1<<4)          // Oscillator Range Select
#define OSCEN                (1<<5)          // Main oscillator Enable
#define OSCSTAT              (1<<6)          // Main Oscillator Status
#define PLLCON_PLLE          (1<<0)          // PLL Enable
#define PLLCON_PLLC          (1<<1)          // PLL Connect
#define PLLSTAT_M            (0x7FFF<<0)     // PLL M Value
#define PLLSTAT_N            (0xFF<<16)      // PLL N Value
#define PLLSTAT_PLOCK        (1<<26)         // PLL Lock Status

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
} Wake_Vars ;

/*--------------------------------------------------------------------------*/

#define WAKE_TIMEOUT1                1000      // for 1 minute , the value is used after a wake_leave() call
#define WAKE_TIMEOUT2                100       // for 1 second , the value is used after the systems is waking up                                               

/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Per_Exec_Wake wake_state        (void);
void          wake_service      (Wake_Status wake_status);
Boolean       wake_service_ready(void);
void          wake_cpu_wake     (void);
void          wake_cpu_sleep    (void); 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Wake_Entry* wake_list ;
extern Wake_Vars   wake_vars ;
extern Wake_Entry* wake_list_ready;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
	
	void	crt0_pll_feed			(void);

//--------------------------------------------------------------------------//

#endif



