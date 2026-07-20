/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_private.h 32240 2015-06-12 12:31:31Z simon.demers $
/*==========================================================================*/

#ifndef __D2D_VALET_PRIVATE_H__
#define __D2D_VALET_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d_valet.h"
#include "d2d_switch.h"

#include "wake.h"
#include "tmr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define BUTN_RELEASED  0
#define BUTN_DEBOUNCE  1  // temporary state
#define BUTN_PRESSED   2

#define DEBOUNCE_NONE    0
#define DEBOUNCE_PRESS   1
#define DEBOUNCE_RELEASE 2
#define DEBOUNCE_TIMEOUT 3

#define LED_STAT_NONE 		0
#define LED_STAT_OFF		  1
#define LED_STAT_ON			  2
enum
{
    VALET_LED_IN_USE_NONE    ,
    VALET_LED_IN_USE_ENABLED ,
    VALET_LED_IN_USE_ACTIVE  ,       //D4 LED flashing
    VALET_LED_IN_USE_INACTIVE,       //D4 LED OFF
};

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/


void    d2d_valet_reset(void) ;

void d2d_valet_butn_p0_service(void);
void d2d_valet_butn_p1_service(void);


void d2d_valet_butn_irq(D2d_Valet_Port *port, UInt8 c);
void d2d_valet_msg_timeout_handler(D2d_Valet_Port *port);

void d2d_valet_wake (Wake_Status wake_status);

void d2d_valet_butn_p4_service(void);
void check_valet_switch_state(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Wake_Status d2d_valet_wake_status;
extern D2d_Valet_Port *d2d_valet_port;
extern UInt32 d2d_valet_offset_time;
extern UInt32 _d2d_valet_pulse_time;
extern UInt8 d2d_valet_led_table_index;

extern UInt8 valet_switch_unstable;
extern UInt32 valet_switch_tstamp, valet_switch_tstamp_pre;
extern UInt8 valet_led_in_use;


/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

