/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: voltage_init.c 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "temperature_private.h"
#include "vrtc_f.h"

#include "aid_dhp.h"
#include "sensor_proto.h"

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

/*--------------------------------------------------------------------------*/

//#define GMTRUNK_DEMO   // enable this to get Magnum Demo analog trunk button feature.  NOT for general use!

#ifdef GMTRUNK_DEMO

#include "nvfs.h"
#include "per_exec.h"
#include "cmd.h"
#include "adc_private.h"


// valid button press is between 2.6V and 2.9V
#define GMTRUNK_ADCHI ((2900 * 0x1000)/3300)
#define GMTRUNK_ADCLO ((2600 * 0x1000)/3300)

UInt16 gmtrunk_adc = 0;
UInt8  gmtrunk_skipped = 0;
UInt8  gmtrunk_period = 0;

void gmtrunk_monitor(void)
{
UInt16 val;

  if(++gmtrunk_skipped < gmtrunk_period)
    return;
  gmtrunk_skipped = 0;

  val = adc_convert(IN_TEMP_SENSOR);
  if(val >= GMTRUNK_ADCLO && val < GMTRUNK_ADCHI)
  {
    if(gmtrunk_adc >= GMTRUNK_ADCHI)
    {// this is the falling edge
// unconditional:    if(get_trunk_status() || prg_out_shadow_read(1+PRG_OUT_STS_PROX))
      {
        cmd_q_rs_insert( INPUT_RF_TRUNK | INPUT_RF_EXT , 100 );
      }	
    }
  }
  gmtrunk_adc = val;
}

void gmtrunk_init(void)
{
UInt8 b;

  if(nvfs_rd_at("GMTRUNK_PERIOD",0,&gmtrunk_period,1)==1 && (gmtrunk_period > 0))
  {
    adc_init (ADC_DIV_CCLK_8 , 4 , IN_TEMP_SENSOR_ENA) ;
    per_exec_register((Per_Exec_Func)gmtrunk_monitor);
  }
}


#endif

void temperature_init( void )
{
	static Boolean do_one_time = TRUE ;
	
  if(do_one_time)
	{
		TRACE("\n\rAnalog tempearature sensor intialized\n\r");

#ifdef GMTRUNK_DEMO		
		gmtrunk_init();
#endif		
		adc_init (ADC_DIV_CCLK_8 , 4 , IN_TEMP_SENSOR_ENA) ;

		temperature_sampling_count  = 0 ; 
		temperature_sampling_period = 5 ;

		do_one_time = FALSE ;
		tempsens_cap = 0;
	}
	
}

/*--------------------------------------------------------------------------*/
void tempsens_cap_polling(void)
{
	  UInt8 slot,chan;
	
	  temperature_update();
		if(get_temp_sensor_value())   // init of status_sense was all 0, check temperature probe first
		{
			tempsens_cap |= TEMPSENS_PROB_AVAIL;
		}

	  // NGSS?
		set_temp_sensor_value(0);  // initialize
		slot = get_temp_sensor_db_num();
		chan = get_temp_chanl_num();
		if(slot && chan && aid_db[slot].id)
		{
				sensor_proto_tx(PLD_E0, aid_db[slot].id, chan, 0, 0);
		}
		os_tsk_wait(500);
	
		if(get_temp_sensor_value())   // init of status_sense was all 0
			tempsens_cap |= TEMPSENS_NGSS_AVAIL;
	
	if(tempsens_cap)
		set_temp_sensor_present(TRUE);
}

/*--------------------------------------------------------------------------*/
