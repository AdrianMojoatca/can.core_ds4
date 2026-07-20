/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: adc.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __ADC_H__
#define __ADC_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// ADC's number of bits of resolution
#define	ADC_BITS_RESOLUTION 	10		
// Maximum value that we can read from ADC converter register
#define	ADC_REG_MAX_VALUE		(((UInt16)1 << (ADC_BITS_RESOLUTION))-1u)	

typedef UInt8 Adc_Enable_Mask ;

//--------------------------------------------------------------------------//

typedef enum // Hardware dependant ordering
{
  ADC_CHANNEL_0 ,
  CHANNEL0 = ADC_CHANNEL_0,  // DBALL firmware uses CHANNEL0
  ADC_CHANNEL_1 ,
  ADC_CHANNEL_2 ,
  ADC_CHANNEL_3 ,
  ADC_CHANNEL_4 ,
  ADC_CHANNEL_5 ,
  ADC_CHANNEL_6 ,
  ADC_CHANNEL_7 ,
// ADC_NO_CHANNEL is equivalent to channel 0 but this value is only to force a delay before the very first conversion 
  ADC_NO_CHANNEL  

} Adc_Channel ;

//--------------------------------------------------------------------------//

typedef enum // Hardware dependant ordering
{
  ADC_ENA_0 = (1U << ADC_CHANNEL_0) ,
  ADC_ENA_1 = (1U << ADC_CHANNEL_1) ,
  ADC_ENA_2 = (1U << ADC_CHANNEL_2) ,
  ADC_ENA_3 = (1U << ADC_CHANNEL_3) ,
  ADC_ENA_4 = (1U << ADC_CHANNEL_4) ,
  ADC_ENA_5 = (1U << ADC_CHANNEL_5) ,
  ADC_ENA_6 = (1U << ADC_CHANNEL_6) ,
  ADC_ENA_7 = (1U << ADC_CHANNEL_7) 

} Adc_Enable_Bits ;

//--------------------------------------------------------------------------//

typedef enum
{
  ADC_DIV_CCLK_1  = 1 ,
  ADC_DIV_CCLK_2  = 2 ,
  ADC_DIV_CCLK_4  = 0 ,
  ADC_DIV_CCLK_8  = 3

} Adc_CCLK_Divider ;

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

void   adc_init                (Adc_CCLK_Divider cclk_divider , UInt8 pclk_divider , Adc_Enable_Mask adc_enable_mask) ;

UInt16 adc_convert             (Adc_Channel adc_channel) ;

double adc_convert_min_max_avg (Adc_Channel adc_channel , UInt8 filter_size) ;

EXTERN_C_LEAVE

/*==========================================================================*/

#endif

