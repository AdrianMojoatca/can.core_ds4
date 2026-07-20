/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_led_private.h 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

#ifndef __DEI_LED_PRIVATE_H__
#define __DEI_LED_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_led.h"
#include "out_fifo.h"
#include "vrtc.h"
#include "trace.h"
#include "config_gpio.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define DEI_LED_Q_SIZE			5
#define DEI_LED_TIMEOUT_SLEEP	10*60*60*24

#ifdef DEV_LPC_236x
#define DEI_LED_OUT(x)      	OUT_EXT_LED(x)
#endif

#ifndef DEBUG_DEI_LED
#define DEBUG_DEI_LED			0
#endif

#if DEBUG_DEI_LED == 1
#define TRACE_DEI_LED			TRACE
#else
#define TRACE_DEI_LED(...)		;
#endif



#define DEI_LED_RES_TIMEOUT     100

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void dei_led_sleep      ( void );
void dei_led_execute    ( void );
void dei_led_flash_sleep( void );
void dei_led_ctrl       ( Boolean status );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Vrtc     		dei_led_vrtc         ;
extern UInt32           dei_led_sleep_tmr    ; 
extern Out_Fifo 		dei_led_fifo         ;
extern Dei_Led_Pattern  dei_led_last_pattern ;
extern Res              dei_led_res          ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


