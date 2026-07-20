/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_led_private.h 17934 2012-10-29 19:23:36Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEI_LED_PRIVATE_H__
#define __DEI_LED_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_led.h"
#include "out_fifo.h"
#include "vrtc_f.h"
#include "trace.h"
#include "config_gpio.h"
#include "brand.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define DEI_LED_Q_SIZE			5
#define DEI_LED_TIMEOUT_SLEEP		VRTC_1SEC*60*60			// check for 1 hour
#define DEI_LED_TIMEOUT_RESUME_PATT		VRTC_1SEC*2			// check for 2sec

#ifndef DEBUG_DEI_LED
#define DEBUG_DEI_LED			0
#endif

#if DEBUG_DEI_LED == 1
#define TRACE_DEI_LED			TRACE
#else
#define TRACE_DEI_LED(...)		;
#endif

#define DEI_LED_RES_TIMEOUT     100

typedef union uLED
{
		struct
		{
	    UInt8  byte[4];
		} s;
		UInt32 cur;
} uLed;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void dei_led_sleep      ( void );
void dei_led_execute    ( void );
void dei_led_resume_pattern_execute (void);
void dei_led_flash_sleep( void );
Dei_Led_Pattern get_led_last_pattern(void);
void dei_ss_sys_arming_save_led_pattern(Dei_Led_Pattern pattern);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Vrtc     		dei_led_vrtc         ;
extern Vrtc 				dei_led_temp_patt_vrtc;
extern UInt32           dei_led_sleep_tmr    ; 
extern Out_Fifo 		dei_led_fifo         ;
extern Dei_Led_Pattern  dei_led_last_pattern ;
extern uLed             uled_patt;
extern UInt8            led_pat_index;
extern Boolean 				dei_led_temp_patt;
extern Boolean 				dei_led_last_patt_confg;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


