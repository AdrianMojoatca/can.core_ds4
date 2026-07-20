/*==========================================================================*/
// $Id: wake.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _WAKE_H_
#define _WAKE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

typedef enum
{
	s_SLEEP,
	s_WAKE ,    
}Wake_Status ;

/*--------------------------------------------------------------------------*/

typedef void    (* Wake_Func )(Wake_Status);
typedef Boolean (* Wake_Ready)(void);

/*--------------------------------------------------------------------------*/

#ifdef PLATFORM_CM800
#if CM800_DEI_BRAND == 1
typedef enum
{
    CPU_FREQ_72MHZ , 
    CPU_FREQ_12MHZ ,
    CPU_FREQ_6MHZ  ,
    CPU_FREQ_4MHZ  ,
    CPU_FREQ_3MHZ  ,
    CPU_FREQ_2MHZ  ,
    CPU_FREQ_1MHZ  ,
    CPU_FREQ_0MHZ  
}Cpu_Freq;
#endif
#endif

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void    wake_init          (void);
void    wake_enter         (void);
void    wake_leave         (void);

#define wake_enter_flagged(dummy) wake_enter()
#define wake_leave_flagged(dummy) wake_leave()

void    wake_ck_enter      (void);
void    wake_ck_leave      (void);
void    wake_notify_wake   (void);
void    wake_register      (Wake_Func  wake_func);
void    wake_register_ready(Wake_Ready wake_func);
UInt32  wake_tc            (void);
void    wake_set_timeout1  (UInt16 timeout_in_ms);
void    wake_set_timeout2  (UInt16 timeout_in_ms);

#ifdef PLATFORM_CM800
#if CM800_DEI_BRAND == 1
void        wake_set_sleep_cpu_freq  (Cpu_Freq sleep_freq );
Cpu_Freq    wake_get_sleep_cpu_freq  (void);
Cpu_Freq    wake_get_current_cpu_freq(void);
#endif
#endif

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


#endif


