/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_temp_private.h 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/
#ifndef __DEI_TEMP_PRIVATE_H__
#define __DEI_TEMP_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_temp.h"
#include "isr.h"
#include "irq_user.h"
#include "fiq.h"
#include "wake.h"
#include "trace.h"
#include "tmr_irq.h"
#include "status_system.h"
#include "per_exec.h"
#include "vrtc_f.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#ifndef DEBUG_DEI_TEMP
#define DEBUG_DEI_TEMP			0
#endif

#if DEBUG_DEI_TEMP == 1
#define TRACE_DEI_TEMP			TRACE
#else
#define TRACE_DEI_TEMP(...)		;
#endif
//--------------------------------------------------------------------------//


#define DEI_TEMP_CMD_TIMEOUT	  200

#define DEI_TEMP_PIN			  	(1u << GPIO_PL_PIN_NUM(GPIO_PL_TEMP_SENSOR))				//(1u<<16)
#define DEI_TEMP_OUT(x)			  (x ? (gpio_pl_set(GPIO_PL_TEMP_SENSOR)) : (gpio_pl_clr(GPIO_PL_TEMP_SENSOR)))			//(x ? (FIO0SET = (1u << 16)) : (FIO0CLR = (1u << 16)))  //P0.16

#define DEI_TEMP_REQUEST		    0x59
#define DEI_TEMP_REQUEST_HANDSHAKE  0xA6
#define DEI_TEMP_RESPONSE_HANDSHAKE 0xD9

#define DEI_TEMP_BASE_TIME_WAKE		72

//For Tx//
#define DEI_TEMP_TX_START_BIT_PERIOD		DEI_TEMP_BASE_TIME_WAKE*12*1000
#define DEI_TEMP_TX_BIT_LOW_PERIOD			DEI_TEMP_BASE_TIME_WAKE*4 *1000
#define DEI_TEMP_TX_BIT_HIGH_PERIOD			DEI_TEMP_BASE_TIME_WAKE*8 *1000						
#define DEI_TEMP_TX_BIT_PREAMBLE_PERIOD		DEI_TEMP_BASE_TIME_WAKE*1 *1000	
#define DEI_TEMP_TX_BIT_STOP_PERIOD			DEI_TEMP_BASE_TIME_WAKE*520							

#define DEI_TEMP_RX_FRAME_TIMEOUT			DEI_TEMP_BASE_TIME_WAKE*400*1000							
#define DEI_TEMP_TX_BIT_COUNT               8
#define DEI_TEMP_TX_PREAMBLE_BIT_COUNT      5
#define DEI_TEMP_TX_FRAME_COUNT				4

#define DEI_TEMP_RX_LOW_HIGH_BORDER			DEI_TEMP_BASE_TIME_WAKE*6 *1000
#define DEI_TEMP_RX_HIGH_MAX				DEI_TEMP_BASE_TIME_WAKE*10*1000
#define DEI_TEMP_RX_LOW_MIN			    	DEI_TEMP_BASE_TIME_WAKE*2*1000
#define DEI_TEMP_RX_BIT_COUNT               8

#define DEI_TEMP_ERR_MAX                    5


#define DEI_TEMP_TIMEOUT_TEMP_REQUEST		10*100
#define DEI_TEMP_TIMEOUT_WAKE               VRTC_1SEC*60*60 //Wakeup on every hour to get the  temperature

// order of #define is critical
#define DEI_TEMP_HANDSHAKE_INIT0            0
#define DEI_TEMP_HANDSHAKE_INIT1            1
#define DEI_TEMP_HANDSHAKE_INIT2            2
#define DEI_TEMP_HANDSHAKE_PASS             3
#define DEI_TEMP_HANDSHAKE_FAIL             4                   


//--------------------------------------------------------------------------//

typedef enum
{
	DEI_TEMP_FALLING_EDGE , 
	DEI_TEMP_RISING_EDGE	 ,
	DEI_TEMP_NO_EDGE
}Dei_Temp_Edge ;

//--------------------------------------------------------------------------//

typedef enum
{
	DEI_TEMP_LOW_BIT  , 
	DEI_TEMP_HIGH_BIT ,
	DEI_TEMP_ERROR_BIT,
	DEI_TEMP_START_BIT,

	DEI_TEMP_NO_BIT
}Dei_Temp_Bit;

//--------------------------------------------------------------------------//

typedef enum
{
    DEI_TEMP_STATE_PREAMBLE      ,
    DEI_TEMP_STATE_TX_START_BIT  ,  
    DEI_TEMP_STATE_TX_DATA       ,
    DEI_TEMP_STATE_RX_TIMEOUT
}Dei_Temp_State ;

//--------------------------------------------------------------------------// 



typedef struct s_Dei_Temp_Vars
{
	Boolean         tx_bit         ;
	Boolean         tx_bit_complete;	
	UInt8           tx_data        ;
	UInt8           tx_copy_data   ;
	UInt8           tx_idx         ;
	UInt8           tx_frame_count ;
    Dei_Temp_State  tx_state       ; 
    
		
	Tmr_Irq	tx_tmr         ;
	
	Boolean rx_complete    ;
    Boolean rx_error       ;
	UInt8   rx_data        ;
	UInt8   rx_idx         ;
	UInt32  rx_time        ;

    UInt8   rx_err_count   ;

	UInt16  timeout_request      ;
    Vrtc    timeout_sensor_update;

	Dei_Temp_Edge edge ; 

    UInt8 tempHandshakeStatus ;
		
}Dei_Temp_Vars ;

//--------------------------------------------------------------------------//




/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void     	  dei_temp_ext_disable  ( void );
void     	  dei_temp_ext_enable   ( void );
Dei_Temp_Edge dei_temp_ext_status   ( void );
Per_Exec_Wake dei_temp_request      ( void );
void          dei_temp_wake         ( Wake_Status wake_status );

void          dei_temp_usr_irq      ( void );
void 		  dei_temp_tx_irq       ( void );
void 		  dei_temp_rx_irq       ( void );
void 		  dei_temp_rx_fiq       ( void );
UInt8         dei_temp_cast         ( UInt8 sensor_data );
void          dei_temp_wake_for_read( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Dei_Temp_Vars dei_temp_vars ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

