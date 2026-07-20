/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: isr.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __ISR_H__
#define __ISR_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum 
{
	ISR_TYPE_IRQ = 0, 
	ISR_TYPE_IRQ_FAST
} Isr_Type ;

//--------------------------------------------------------------------------//

typedef enum
{
	ISR_PRI_0 = 0,
	ISR_PRI_1,
	ISR_PRI_8 = 8,
	ISR_PRI_10 = 10,
	ISR_PRI_14 = 14,
	ISR_PRI_15 = 15,
	ISR_PRI_31 = 0
	
} Vic_Isr_Pri;

//--------------------------------------------------------------------------//

typedef void (*PVF)(void);

//--------------------------------------------------------------------------//

EXTERN_C_ENTER

typedef enum
{
	ISR_ID_USER     =  1 ,

	ISR_ID_TIMER_0  =  4 ,
	
	ISR_ID_TIMER_1  =  5 ,

	ISR_ID_UART_0   =  6 ,
	
	ISR_ID_UART_1   =  7 ,

	ISR_ID_PWM1     =  8 ,

	ISR_ID_I2C_0    =  9 ,
	
	ISR_ID_SSP0_SPI = 10 ,
	
	ISR_ID_SSP1     = 11 ,

	ISR_ID_PLL      = 12 ,
 
	ISR_ID_RTC      = 13 ,

	ISR_ID_EINT_0   = 14 ,
	
	ISR_ID_EINT_1   = 15 ,
	
	ISR_ID_EINT_2   = 16 ,
	
	ISR_ID_EINT_3   = 17 ,

	ISR_ID_ADC_0    = 18 ,

	ISR_ID_I2C_1    = 19 ,
	
	ISR_ID_CAN      = 23 ,

	ISR_ID_TIMER_2  = 26 ,
	
	ISR_ID_TIMER_3  = 27 ,
	
	ISR_ID_UART_2   = 28 ,

	ISR_ID_UART_3   = 29 ,

	ISR_ID_I2C_2    = 30 ,
	
	// unused ISR channel
	ISR_ID_WDT			=	0,
	IRD_ID_Dbg_Com_Rx	=	2,
	ISR_ID_Dbg_Com_Tx	=	3,
	ISR_ID_BOD			=	20,
	ISR_ID_Etehrnet		=	21,
	ISR_ID_USB			=	22,
	ISR_ID_SD_MMC		=	24,
	ISR_ID_GP_DMA		=	25,
	ISR_ID_I2S			=	31,

} Isr_Number ;

EXTERN_C_LEAVE

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

EXTERN_C_ENTER

void isr_set_vector     ( Isr_Number vic_channel , Vic_Isr_Pri isr_pri , Isr_Type isr_type , PVF isr_func_addr );

void isr_set_fiq_channel( Isr_Number vic_channel );

EXTERN_C_LEAVE

#endif

