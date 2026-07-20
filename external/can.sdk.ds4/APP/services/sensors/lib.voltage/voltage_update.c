/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: voltage_update.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "voltage_private.h"
#include "aid_dhp.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define VBAT_ADC_OFFSET  (0.4)

#define SAFE_VOLTAGE_LOW               (8.50)    //to support long range IVU (REF: 10393)
#define SAFE_VOLT_COUNT_THRESHOLD      10   //10 * voltage_sampling_period(=5) * 10 ms (per exec) = 500 ms
#define SAFE_VOLT_LOW_COUNT_THRESHOLD  2   //2 * voltage_sampling_period(=5) * 10 ms (per exec) = 100 ms
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void voltage_trace( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
#if DEBUG_VOLTAGE == 1
UInt16 trace_moment = 0;
#endif
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void safe_voltage_monitor(double volt);
/*--------------------------------------------------------------------------*/

volatile double voltage_in_check ;
Per_Exec_Wake voltage_update( void )
{
	double temp_voltage ;

	if( voltage_sampling_count++ >= voltage_sampling_period	)
	{
		temp_voltage = voltage_smooth( VOLTAGE_ADC_SCALING_FACTOR * adc_convert_min_max_avg (IN_BATT_VOLTAGE , VOLTAGE_ADC_FILTER_SIZE )  ) ;
		
		temp_voltage += VBAT_ADC_OFFSET;
		voltage_in_check = temp_voltage;
		safe_voltage_monitor(voltage_in_check);    //check the voltage if safe per second
#if DEBUG_VOLTAGE == 1		
		if(++trace_moment >= 20)    //20 * voltage_sampling_period(=5) * 10ms = 1 sec
		{
			set_voltage( temp_voltage );
			trace_moment = 0;
			voltage_trace();
		}
#endif		
		
		if( temp_voltage != get_voltage() )
		{			
			TRACE_ZONE
			(
				if( (temp_voltage > get_voltage()) &&  ((temp_voltage - get_voltage()) > VOLTAGE_TRACE_STEP) )
				{
					TRACE_DEI_VOLT(NULL , (void*)voltage_trace );
				}			
	
				if( (temp_voltage < get_voltage()) &&  ((get_voltage() - temp_voltage) > VOLTAGE_TRACE_STEP) )
				{
					TRACE_DEI_VOLT(NULL , (void*)voltage_trace );
				}			
			);

	    set_voltage( temp_voltage );

		}
		
		voltage_sampling_count = 0 ; 
	}
	
	return PER_EXEC_SLEEP ;		
}

/*--------------------------------------------------------------------------*/
void voltage_trace( void )
{
	//TRACE("\n\r[VOLTAGE %.02f]\n\r" , get_voltage() );
	#define FRACTION_MASK  0xFF
	UInt16 volt_tmp;
	volt_tmp = (get_voltage()*256.0)/1;   //convert to integer
#ifdef PLATFORM_933  
 	if(dbgclitx_room() < 200)
    return;
#endif
	
	TRACE("\n\r VOLTAGE[V]: ");
	TRACE("%d.", (volt_tmp>>8));
	volt_tmp = (volt_tmp & FRACTION_MASK) * 10;
	TRACE("%c", (volt_tmp>>8)+'0');
	volt_tmp = (volt_tmp & FRACTION_MASK) * 10;
	TRACE("%c\n\r", (volt_tmp>>8)+'0');
}

volatile UInt8 safe_volt_monitor_count = 0;
//--------------------------------------------------------------------------//
void safe_voltage_monitor(double volt)
{
	if(!sys_in_low_voltage_mode)
	{
		safe_volt_monitor_count = (volt < SAFE_VOLTAGE_LOW) ? (safe_volt_monitor_count + 1) : 0;
		if(safe_volt_monitor_count > SAFE_VOLT_LOW_COUNT_THRESHOLD)
		{
			safe_volt_monitor_count = 0;
			sys_in_low_voltage_mode = 1;
		}
	}
	else
	{
		safe_volt_monitor_count = (volt >= SAFE_VOLTAGE_LOW) ? (safe_volt_monitor_count + 1) : 0;
		if(safe_volt_monitor_count > SAFE_VOLT_COUNT_THRESHOLD)
		{
			safe_volt_monitor_count = 0;
			sys_in_low_voltage_mode = 0;
			master_auth_reset();
		}
	}
}

//--------------------------------------------------------------------------//
