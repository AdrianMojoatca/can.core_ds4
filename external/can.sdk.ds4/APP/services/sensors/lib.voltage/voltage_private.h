/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: voltage_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/
#ifndef __VOLTAGE_PRIVATE_H__
#define __VOLTAGE_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "per_exec.h"
#include "status_system.h"
#include "adc.h"
#include "config_gpio.h"

#include "trace.h"
#include "dbg.h"

//#include <stdio.h>
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

#define VOLTAGE_ADC_FILTER_SIZE			20
#define VOLTAGE_SMOOTH_FILTER_SIZE		5

#define VOLTAGE_TRACE_STEP				0.1

#ifdef DEV_LPC_236x
	#define VOLTAGE_ADC_SCALING_FACTOR		(5.03 /287.0)			
#else
	#define VOL_DIV_R1				226
	#define VOL_DIV_R2				1000
	#define VOLTAGE_ADC_SCALING_FACTOR		((VOL_DIV_R1 + VOL_DIV_R2)*3.3)/(VOL_DIV_R1*4096)
#endif

//#define VOLTAGE_ADC_SCALING_FACTOR		(1.5 /287.0)			

/*--------------------------------------------------------------------------*/

#ifndef DEBUG_VOLTAGE
#define DEBUG_VOLTAGE			0
#endif

#if DEBUG_VOLTAGE == 1
#define TRACE_DEI_VOLT			TRACE
#else
#define TRACE_DEI_VOLT(...)		;
#endif
/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void   		   voltage_smooth_init( UInt8 size );
double 		   voltage_smooth     ( double instant_value );
Per_Exec_Wake  voltage_update	  ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern UInt8 voltage_sampling_count  ; 
extern UInt8 voltage_sampling_period ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

