/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: voltage_update.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "temperature_private.h"
#include "aid_dhp.h"
#include "sensor_proto.h"
#include "string.h"
#include "status_system.h"

#include "vrtc_f.h"
#include "system_type.h"
#include "nvfs_usr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
static void temperature_trace( void );
/*--------------------------------------------------------------------------*/
void temperature_update( void )
{
	SInt32 curr_temp, adc_inst_val;
	UInt8 slot,chan;
		
	if( (tempsens_config == TEMPSENS_PROBE && (tempsens_cap & TEMPSENS_PROB_AVAIL) == TEMPSENS_PROB_AVAIL && !ble_siren_temp_avail )
		  || !tempsens_cap   // at first polling for capability
		  || (tempsens_config == TEMPSENS_NONE && (tempsens_cap & TEMPSENS_PROB_AVAIL) == TEMPSENS_PROB_AVAIL)    // temp probe is preferred
	    || ((tempsens_config == TEMPSENS_NGSS && (tempsens_cap & TEMPSENS_NGSS_AVAIL) != TEMPSENS_NGSS_AVAIL) && (tempsens_cap & TEMPSENS_PROB_AVAIL) == TEMPSENS_PROB_AVAIL)   // temp probe is a backup
	    || ((tempsens_config == TEMPSENS_WSS && (tempsens_cap & TEMPSENS_WSS_AVAIL) != TEMPSENS_WSS_AVAIL) && (tempsens_cap & TEMPSENS_PROB_AVAIL) == TEMPSENS_PROB_AVAIL)   // temp probe is a backup
	  )  
	{
		adc_inst_val = adc_convert_min_max_avg (IN_TEMP_SENSOR , TEMPERATURE_ADC_FILTER_SIZE );
		curr_temp = ((((((((TEMP_SCALING_FACTOR_A * adc_inst_val) >> 10) +TEMP_SCALING_FACTOR_B) * adc_inst_val)>>10) + TEMP_SCALING_FACTOR_C)* adc_inst_val) >> 10) + TEMP_SCALING_FACTOR_D;

		last_temp_read = curr_temp ;
				
		if(curr_temp< (-40<<8))
			curr_temp = (-40<<8);	   // less tha -40C will be read as -40C
		
		set_temp_sensor_value((curr_temp + (40<<8) + 0x80)>>8);
	}
	else
	{
		if( (tempsens_config == TEMPSENS_NGSS && ((tempsens_cap & TEMPSENS_NGSS_AVAIL) == TEMPSENS_NGSS_AVAIL) && !ble_siren_temp_avail)  
		    || ((tempsens_config == TEMPSENS_NONE) && ((tempsens_cap & TEMPSENS_NGSS_AVAIL) == TEMPSENS_NGSS_AVAIL))
        || ((tempsens_config == TEMPSENS_PROBE && (tempsens_cap & TEMPSENS_PROB_AVAIL) != TEMPSENS_PROB_AVAIL) && (tempsens_cap & TEMPSENS_NGSS_AVAIL) == TEMPSENS_NGSS_AVAIL)   // NGSS is a backup
		    || ((tempsens_config == TEMPSENS_WSS && (tempsens_cap & TEMPSENS_WSS_AVAIL) != TEMPSENS_WSS_AVAIL) && (tempsens_cap & TEMPSENS_NGSS_AVAIL) == TEMPSENS_NGSS_AVAIL)   // NGSS is a backup
		  )
		{
				slot = get_temp_sensor_db_num();
				chan = get_temp_chanl_num();
				if(slot && chan && aid_db[slot].id)
				{
						sensor_proto_tx(PLD_E0, aid_db[slot].id, chan, 0, 0);
				}
		}
	}
	
		
	TRACE_ZONE
	(
			TRACE_DEI_TEMP(NULL , (void*)temperature_trace );
	);

}

/*--------------------------------------------------------------------------*/

static void temperature_trace( void )
{
	#define SHIFT_MASK ((1<<8)-1)
	SInt32 trace_temp;
	
 	if(dbgclitx_room() < 200)
    return;
  
	TRACE("\n\r TEMPERATURE: ");
	if(last_temp_read >= 0)
		trace_temp = last_temp_read;
	else
	{
		trace_temp = (- last_temp_read);
		TRACE("  -");
	}
	  
		TRACE("%d" , (trace_temp>>8) /*get_temperature()*/ );
		// mask integer
		trace_temp = (((trace_temp & SHIFT_MASK)*10)>>8);
		TRACE(".%c\n\r", trace_temp+'0' /*get_temperature()*/ );
	
	  //TRACE("ADC Value : %d", adc_inst_val);
}

//--------------------------------------------------------------------------//
