/*==========================================================================*/
// $Id: rftd2_update.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "rf.h"
#include "d2d.h"
#include "rftd2_private.h"
#include "bitfield.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean rftd2_updated(void)           ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rftd2_update (Rftd2_Update_Type type)
{
	UInt8 data [4] ;
	
	if(rftd2_updated() || type == RFTD2_UPDATE_ALWAYS)
	{
		
			data[0] = SET_EXTENDED_STATUS  ;
			data[1] = rftd2.status.byte[0] ;
			data[2] = rftd2.status.byte[1] ;
			data[3] = rftd2.status.byte[2] ;
		

		d2d_tx_frame (&data[0] , sizeof_array(data)) ;
	}
}

//--------------------------------------------------------------------------//
static Boolean rftd2_updated(void)
{
  #define IS_BYTE_CHANGE(z) (rftd2.prev_status.byte[z] ^ rftd2.status.byte[z])
	
	Boolean is_change = FALSE ;
	
	  rftd2.status.byte[0] =
	  rftd2.status.byte[1] =
	  rftd2.status.byte[2] = 0;
	
	
	  if(rf_data_icon_bit(ICON_LOCK))
		  rftd2.status.bit.lock     		= 1;
		if(rf_data_icon_bit(ICON_ARMED))
		  rftd2.status.bit.arm      		= 1;
		if(rf_data_icon_bit(ICON_IGNITION))
		  rftd2.status.bit.ignition 		= 1;
		if(rf_data_icon_bit(ICON_PARKING))
		  rftd2.status.bit.parking_light  = 1;
		if(rf_data_icon_bit(ICON_HORN))
		  rftd2.status.bit.siren    		= 1;
		if(rf_data_icon_bit(ICON_VALET))
		  rftd2.status.bit.valet    		= 1;
		if(rf_data_icon_bit(ICON_SUCCESS))
		  rftd2.status.bit.is_running  	= 1;
		if(rf_data_icon_bit(ICON_PANIC))
		  rftd2.status.bit.panic    		= 1;

    if(rf_data_icon_bit(ICON_DOOR))		
		  rftd2.status.bit.door      		= 1;
		if(rf_data_icon_bit(ICON_TRUNK))
		  rftd2.status.bit.trunk     		= 1;
		if(rf_data_icon_bit(ICON_HOOD))
		  rftd2.status.bit.hood      		= 1;
		if(rf_data_icon_bit(ICON_WARN))
		  rftd2.status.bit.warn_away		= 1;
		if(rf_data_icon_bit(ICON_SHOCK))
		  rftd2.status.bit.shock_sensor   = 1;
		if(rf_data_icon_bit(ICON_NOT_READY))
		  rftd2.status.bit.ready_mode     = 1;

    if(rf_data_icon_bit(ICON_DTC))		
		  rftd2.status.bit.dtc_code       = 1;
		if(rf_data_icon_bit(ICON_ALERT))
		  rftd2.status.bit.alert          = 1;
		if(rf_data_icon_bit(ICON_RS_ARMED))
		  rftd2.status.bit.rs_arm      	= 1;
		if(rf_data_icon_bit(ICON_RS_HORN))
		  rftd2.status.bit.rs_siren    	= 1;
		if(rf_data_icon_bit(ICON_RS_PANIC))
		  rftd2.status.bit.rs_panic    	= 1;
		
		rftd2.status.bit.eng_run_change = rftd2.status.bit.is_running ^ rftd2.prev_status.bit.is_running ;
		rftd2.status.bit.panic_change   = (rftd2.status.bit.panic | rftd2.status.bit.rs_panic) ^ rftd2.prev_status.bit.panic ;	
		rftd2.status.bit.support_change	= rftd2_update_support_change () ;

		is_change = !!(IS_BYTE_CHANGE(0)|IS_BYTE_CHANGE(1)|IS_BYTE_CHANGE(2)) ;
		if((rftd2.status.bit.siren | rftd2.status.bit.rs_siren) && !(rftd2.prev_status.bit.siren | rftd2.prev_status.bit.rs_siren))
		{
			d2d_tx (AL_ALARM_ALERT) ;
		}
		rftd2.prev_status.byte[0] = rftd2.status.byte[0] ;
		rftd2.prev_status.byte[1] = rftd2.status.byte[1] ;
		rftd2.prev_status.byte[2] = rftd2.status.byte[2] ;
	
	return is_change ;

  #undef IS_BYTE_CHANGE
}

//--------------------------------------------------------------------------//



