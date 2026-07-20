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
	ISR_PRI_0 = 29,     // higher priority than OS for FIQ
	ISR_PRI_31 = 31,    // interrupt priority for IRQ	
	ISR_PRI_15 = 31,
	ISR_PRI_8 = 31,
	ISR_PRI_10 = 31,
	ISR_PRI_1 = 31,
	ISR_PRI_14 = 31
	
} Isr_Pri;

//--------------------------------------------------------------------------//

typedef void (*PVF)(void);

// fast interrupt handling vectors
extern UInt32 ISR_Handler_0;



//--------------------------------------------------------------------------//

EXTERN_C_ENTER

typedef enum
{
	ISR_ID_PENDSV		=	-2,
	
	ISR_ID_TIMER_0  =  1 ,
	
	ISR_ID_TIMER_1  =  2 ,

	ISR_ID_TIMER_2  = 3 ,
	
	ISR_ID_TIMER_3  = 4 ,
	
	ISR_ID_UART_0   =  5 ,
	
	ISR_ID_UART_1   =  6 ,

	ISR_ID_UART_2   = 7 ,

	ISR_ID_UART_3   = 8 ,

	ISR_ID_PWM1     =  9 ,

	ISR_ID_I2C_0    =  10 ,
	
	ISR_ID_I2C_1    = 11 ,

	ISR_ID_I2C_2    = 12 ,

	ISR_ID_SPI 		=  13 ,
	
	ISR_ID_SSP0     = 14 ,

	ISR_ID_SSP1		=  15 ,

	ISR_ID_PLL      = 16 ,
 
	ISR_ID_RTC      = 17 ,

	ISR_ID_EINT_0   = 18 ,
	
	ISR_ID_EINT_1   = 19 ,
	
	ISR_ID_EINT_2   = 20 ,
	
	ISR_ID_EINT_3   = 21 ,

	ISR_ID_ADC_0    = 22 ,
	
	ISR_ID_CAN      = 25 ,

	
	// unused ISR channel
  ISR_ID_NMI          = 	-14 , // Non Maskable Interrupt
  ISR_ID_MEM_MANAGE   = 	-12 , // Cortex-M3 Memory Management Interrupt
  ISR_ID_BUS_FAULT    = 	-11 , // Cortex-M3 Bus Fault Interrupt
  ISR_ID_USAGE_FAULT  = 	-10 , // Cortex-M3 Usage Fault Interrupt
  ISR_ID_SVCALL       = 	-5  , // Cortex-M3 SV Call Interrupt
  ISR_ID_DBG_MONITOR  = 	-4  , // Cortex-M3 Debug Monitor Interrupt
  ISR_ID_SYS_TICK     = 	-1  , // Cortex-M3 System Tick Interrupt
	ISR_ID_WDT				=	0,
	ISR_ID_BOD				=	23,
	ISR_ID_USB				=	24,
	ISR_ID_GP_DMA			=	26,
	ISR_ID_I2S				=	27,
	ISR_ID_Etehrnet		=	28,
	ISR_ID_RITINT			=	29,
	ISR_ID_PWM				= 30,
	ISR_ID_QENC				=	31,
	ISR_ID_PLL1				=	32,
	ISR_ID_USB_Act		=	33,
	ISR_ID_CAN_ACT		=	34

} Isr_Number ;


#ifdef DEV_LPC_176x
	#define MEMMAP			0x400FC040
#else
	#define MEMMAP			0xE01FC040
#endif
EXTERN_C_LEAVE

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void isr_init         (void) ;

void nvic_irq_enable   (Isr_Number irq_num , Boolean enable) ;
void nvic_set_priority (Isr_Number irq_num , UInt8  irq_priority) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

EXTERN_C_ENTER

void isr_set_vector ( Isr_Number irq_num , Isr_Pri irq_pri , Isr_Type irq_type , PVF irq_func_addr );

void isr_set_fiq_channel( Isr_Number irq_num );

void isr_set_fiq_pri( Isr_Number irq_num );

void isr_reset_fiq_pri( Isr_Number irq_num );

EXTERN_C_LEAVE

/*==========================================================================*/

#endif

