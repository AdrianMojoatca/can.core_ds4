/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf1_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/
#ifndef __DEI_RF1_PRIVATE_H__
#define __DEI_RF1_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf1.h"
#include "isr.h"
#include "irq_user.h"
#include "fiq.h"
#include "wake.h"
#include "trace.h"
#include "tmr_irq.h"
#include "cmd.h"
#include "nvfs_usr.h"
#include "nvram_usr.h"
#include "cmd.h"
#include "notify.h"
#include "exec.h"
#include "status_system.h"
#include "config_gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#ifndef DEBUG_DEI_RF1
#define DEBUG_DEI_RF			0
#endif

#if DEBUG_DEI_RF1 == 1
#define TRACE_DEI_RF1			TRACE
#else
#define TRACE_DEI_RF1(...)		;
#endif


#define DEI_RF1_HHU_MAX			 4

//--------------------------------------------------------------------------//

typedef enum
{
	DEI_RF1_FALLING_EDGE , 
	DEI_RF1_RISING_EDGE	 ,
	DEI_RF1_NO_EDGE
}Dei_Rf1_Edge ;

//--------------------------------------------------------------------------//

typedef enum
{
	DEI_RF1_LOW_BIT  , 
	DEI_RF1_HIGH_BIT ,
	DEI_RF1_ERROR_BIT,
	DEI_RF1_START_BIT,

	DEI_RF1_NO_BIT
}Dei_Rf1_Bit;

//--------------------------------------------------------------------------//

#define DEI_RF1_TX_UNLOCK		0x02
#define DEI_RF1_TX_CAR_FINDER	0x04
#define DEI_RF1_TX_START		0x05
#define DEI_RF1_TX_STOP			0x0C
#define DEI_RF1_TX_ERROR        0xEE 

#define DEI_RF1_RX_SHORT		0x9B
#define DEI_RF1_RX_LONG			0xBB

#define DEI_RF1_CMD_TIMEOUT		200

//--------------------------------------------------------------------------//

typedef struct s_Dei_Rf1_Vars
{
	UInt32  rx_time ;
	UInt32	rx_data ;
	UInt8   rx_idx  ;
	Boolean rx_start;
	Boolean rx_prg  ;
	UInt32  rx_last_data;
	Boolean rx_complete ;

	UInt32  rx_time_low_min  ;
	UInt32  rx_time_low_max  ; 
	UInt32  rx_time_high_max ;
	UInt32  rx_time_start_min;
	UInt32  rx_time_start_max;
	UInt32  rx_time_stamp    ;
	

	Tmr_Irq rx_tmr;

											
	Boolean tx_bit         ;
	Boolean tx_bit_complete;
	Boolean tx_complete    ;
	Boolean tx_in_progress ;
	Boolean tx_request     ;
	UInt32  tx_data        ;
	UInt32  tx_copy_data   ;
	UInt8   tx_idx         ;
	UInt8   tx_frame_count ;
		
	Tmr_Irq	tx_tmr         ;     
	
	UInt32  hhu_sn_used ;	
	UInt8   hhu_id_used ;
	
	Q       exec_q_free ;


}Dei_Rf1_Vars ;

//--------------------------------------------------------------------------//

typedef struct s_Dei_Rf1_Hhu
{
	UInt32 id[DEI_RF1_HHU_MAX];
	UInt8  idx ;
}Dei_Rf1_Hhu ;	 

//--------------------------------------------------------------------------//

typedef struct s_Dei_Rf1_Notify_Table
{
	Notify nf  ; 

	Notify_Func func ; 

}Dei_Rf1_Notify_Table;

//--------------------------------------------------------------------------//

#define DEI_RF1_BASE_TIME_WAKE				72

#define DEI_RF1_RX_LOW_MIN_WAKE				DEI_RF1_BASE_TIME_WAKE*260
#define DEI_RF1_RX_LOW_MAX_WAKE				DEI_RF1_BASE_TIME_WAKE*660
#define DEI_RF1_RX_HIGH_MAX_WAKE			DEI_RF1_BASE_TIME_WAKE*1320
#define DEI_RF1_RX_START_MIN_WAKE           DEI_RF1_BASE_TIME_WAKE*3800
#define DEI_RF1_RX_START_MAX_WAKE           DEI_RF1_BASE_TIME_WAKE*4200

#define DEI_RF1_RX_TIMEOUT                  DEI_RF1_BASE_TIME_WAKE*1000*1000

#define DEI_RF1_TX_HALF_BIT_TIMEOUT         DEI_RF1_BASE_TIME_WAKE*320
#define DEI_RF1_TX_FIRST_FRAME_TIMEOUT      DEI_RF1_BASE_TIME_WAKE*1000*20 //20ms
#define DEI_RF1_TX_NEXT_FRAME_TIMEOUT       DEI_RF1_BASE_TIME_WAKE*1000*5  //5ms

#define DEI_RF1_TX_FRAME_MAX				6

//--------------------------------------------------------------------------//

//VIPER
//S011101011

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void         dei_rf1_rx_irq      ( void );
void         dei_rf1_rx_tmr_irq  ( void );
void         dei_rf1_tx_irq      ( void );
void         dei_rf1_usr_irq     ( void );
Dei_Rf1_Bit  dei_rf1_rx_bit      ( void );
Boolean      dei_rf1_rx_hhu_check( void );
Boolean      dei_rf1_rx_hhu_add  ( void );
Boolean      dei_rf1_nvfs_init   ( void );
void         dei_rf1_nvfs_delete ( void );
void         dei_rf1_nvfs_update ( void );
void         dei_rf1_tx_frame    (UInt8 cmd);


void         dei_rf1_ext_enable_falling( void );
void         dei_rf1_ext_enable_rising ( void );
void         dei_rf1_ext_disable       ( void );
Dei_Rf1_Edge dei_rf1_ext_status        ( void );


void         dei_rf1_notify_init            ( void );
void 	     dei_rf1_notify_rs_on           ( void );
void 	     dei_rf1_notify_rs_off          ( void );
void 	     dei_rf1_notify_unlock          ( void );
void 	     dei_rf1_notify_car_finder      ( void );
void 	     dei_rf1_notify_programming_on  ( void );
void 	     dei_rf1_notify_programming_off ( void );
void 	     dei_rf1_notify_remote_delete   ( void );
void 	     dei_rf1_notify_rs_error		( void );
void         dei_rf1_notify_lock            ( void );


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Dei_Rf1_Vars dei_rf1_vars ;
extern Dei_Rf1_Hhu  dei_rf1_hhu  ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

