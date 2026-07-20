/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEI_RF_PRIVATE_H__
#define __DEI_RF_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf.h"
#include "trace.h"
#include "tmr_irq.h"
#include "wake.h"
#include "fiq.h"
#include "config_gpio.h"
#include "800_gpio.h"
#include "irq_user.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//---------------------------------------------------------------------------//


#ifndef DEBUG_DEI_RF
#define DEBUG_DEI_RF			0
#endif

#if DEBUG_DEI_RF == 1
#define TRACE_DEI_RF			TRACE
#else
#define TRACE_DEI_RF(...)		;
#endif

//---------------------------------------------------------------------------//

typedef void (*Dei_Rf_State )( void );

//---------------------------------------------------------------------------//

typedef struct s_Dei_Rf
{    
	//for rx// 
    UInt32              rx_data0                       ;
    UInt32              rx_data1                       ;
    Boolean             rx_crc0                        ;
    Boolean             rx_crc1                        ;
    UInt8 		        rx_bit_index                   ;
    UInt8               rx_bit_count                   ;            
    UInt32              rx_time                        ;
    Dei_Rf_State        rx_state                       ;
    Irq                 rx_irq                         ;
    Boolean             rx_complete                    ;
    Dei_Rf_Antenna_Type antenna_type                   ;	
	UInt32              time_rx_high_min               ;
	UInt32              time_rx_high_max               ;
	UInt32              time_rx_low_max                ;
    UInt32              rx_delay                       ;	
    UInt8               rx_id_table[ DEI_RF_ID_COUNT ] ;
    UInt32              rx_time_stamp                  ;

    
    //for tx//
    UInt32             tx_data0                         ;
    UInt32             tx_data1                         ;
    UInt32             tx_copy_data0                    ;
    UInt32             tx_copy_data1                    ;    
    UInt16             tx_bit_index                     ;
    UInt8              tx_bit_count                     ;
    UInt8              tx_frame_index                   ;
    UInt8              tx_frame_count                   ;    
    UInt8              tx_page_count                    ;     
    Boolean            tx_pagging                       ;
    Dei_Rf_State       tx_state                         ;   
    Tmr_Irq            tx_tmr                           ;
    Irq                tx_irq                           ;
    Boolean            tx_complete                      ; 

    Boolean            wake                             ;
    
}Dei_Rf;

//---------------------------------------------------------------------------//

#define DEI_RF_CK_72MHZ						72
#define DEI_RF_CK_12MHZ						12
#define DEI_RF_CK_6MHZ						6
#define DEI_RF_CK_4MHZ						4
#define DEI_RF_CK_3MHZ						3
#define DEI_RF_CK_2MHZ						2
#define DEI_RF_CK_1MHZ						1

#define	DEI_RF_RX_HIGH_MIN                  300
#define	DEI_RF_RX_HIGH_MAX                  480
#define DEI_RF_RX_LOW_MAX                   800

#define DEI_RF_RX_NORMAL_BIT_COUNT          50
#define DEI_RF_RX_PROGRAMMING_BIT_COUNT     56


//----------------------------------------------------------------------------//

#define DEI_RF_TIME_BASE_WAKE						72

#define DEI_RF_TX_1MS                               1000*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_OFFSET_START                      30000*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_PREAMBLE_ON_PERIOD1               1000*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_PREAMBLE_ON_PERIOD2               500*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_PREAMBLE_OFF_PERIOD               500*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_BIT_PERIOD                        160*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_BIT_LOW_PERIOD                    440*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_BIT_HIGH_PERIOD                   240*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_WAKEUP_ON_PERIOD                  160*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_WAKEUP_OFF_PERIOD                 840*DEI_RF_TIME_BASE_WAKE

#define DEI_RF_TX_INTERFRAME_SPACE_NORMAL           120*1000*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_INTERFRAME_SPACE_PROGRAMMING      65*1000*DEI_RF_TIME_BASE_WAKE

#define DEI_RF_TX_PAGGING_INTERFRAME_OFFSET         115*1000*DEI_RF_TIME_BASE_WAKE
#define DEI_RF_TX_PAGGING_INTERPAGE_OFFSET          335*1000*DEI_RF_TIME_BASE_WAKE

//---------------------------------------------------------------------------//

#define DEI_RF_TX_NORMAL_BIT_COUNT                  63   
#define DEI_RF_TX_PROGRAMMING_BIT_COUNT             47 
#define DEI_RF_TX_WAKEUP_BIT_COUNT                  960     

//---------------------------------------------------------------------------//

#define DEI_RF_TX_NORMAL_FRAME_COUNT                6
#define DEI_RF_TX_PROGRAMMING_FRAME_COUNT           6
#define DEI_RF_TX_PAGGING_FRAME_COUNT               3
#define DEI_RF_TX_PAGGING_PAGE_COUNT                9

//---------------------------------------------------------------------------//
                                               
#define DEI_RF_EXT_ENABLE()					    PINSEL4  |=   0x00400000  ; EXTINT  = 0x02
#define DEI_RF_EXT_DISABLE()                    PINSEL4  &= (~0x00400000) ; EXTINT  = 0x02  

//--------------------------------------------------------------------------//

#define DEI_RF_TMR(x)                           tmr_irq_update( dei_rf.tx_tmr , x , TRUE );tmr_irq_ena( dei_rf.tx_tmr )

//--------------------------------------------------------------------------//

#define DEI_RF_TX_ON()                          OUT_TX_RF(TRUE)  //FIO0SET |= ((UInt32)1 << 15)
#define DEI_RF_TX_OFF()                         OUT_TX_RF(FALSE) //FIO0CLR |= ((UInt32)1 << 15)

//--------------------------------------------------------------------------//


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void    dei_rf_usr_irq( void );
void    dei_rf_rx_irq ( void ) ;
void    dei_rf_tx_irq ( void );

void    dei_rf_rx_state_start ( void );
void    dei_rf_tx_state_offset( void );


void    dei_rf_wake  ( Wake_Status wake_status ) ; 

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Dei_Rf               dei_rf ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

