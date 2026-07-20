/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include "d2d.h"
#include "timeout_f.h"
#include "config_status.h"
#include "status.h"


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define rke_update_timeout  6000
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void sense_notify_init (void)
{
    
    
    static const Cmd_Array1 cmd_array[] = 
      {
        {INPUT_GWR  		| INPUT_ON      ,sense_notify_gwr_on                         } ,   
        {INPUT_GWR                          ,sense_notify_gwr_off                        } ,         
        {INPUT_STARTER		| INPUT_ON      ,sense_notify_fake_rpm_on                    } ,   
        {INPUT_STARTER                      ,sense_notify_fake_rpm_off                   } ,   
        {INPUT_RS_STARTER   | INPUT_ON      ,sense_notify_fake_rpm_on                    } ,   
        {INPUT_RS_STARTER                   ,sense_notify_fake_rpm_off                   } ,  
	  
      } ;	
       
      timeout_unlock = timeout_f_create(rke_update_timeout ,sense_notify_rke_timeout) ;  
      rpm.word   = 0 ;
      key.byte   = 0 ;
      door.byte  = 0 ;
      trans.byte = 0 ;
      brake.byte = 0 ;
    
      req_input.status.byte  = 0x00 ;
			
			status_set_source(STATUS_SOURCE_1,ALL_STATUS);
      status_set_trans(0,STATUS_TRANS_BYTE,STATUS_SOURCE_1);  // iniitalize with no bit set,  first sense_notify_trans() will cause a change
			

	  if( d2d_get_mask(SET_DRIVER_OPEN)     && d2d_get_mask(SET_DRIVER_CLOSE)        && d2d_get_ena(SET_DRIVER_OPEN)     && d2d_get_ena(SET_DRIVER_CLOSE)        )
	  {
		req_input.support.bit.driver         = TRUE;
	  }
	  
      if( d2d_get_mask(SET_OTHER_OPEN)      && d2d_get_mask(SET_OTHER_CLOSE)         && d2d_get_ena(SET_OTHER_OPEN)      && d2d_get_ena(SET_OTHER_CLOSE)         )
	  {
		  req_input.support.bit.other        = TRUE;
	  }
                                           
      if( d2d_get_mask(SET_TRUNK_OPEN)      && d2d_get_mask(SET_TRUNK_CLOSE)         && d2d_get_ena(SET_TRUNK_OPEN)      && d2d_get_ena(SET_TRUNK_CLOSE)  	     )
	  {
		  req_input.support.bit.trunk        = TRUE;
	  }
	  
      if( d2d_get_mask(SET_HOOD_OPEN)       && d2d_get_mask(SET_HOOD_CLOSE)          && d2d_get_ena(SET_HOOD_OPEN)       && d2d_get_ena(SET_HOOD_CLOSE)   	     )
	  {
		  req_input.support.bit.hood         = TRUE;
	  }
                                           
      if( d2d_get_mask(SET_BRAKE_ON)        && d2d_get_mask(SET_BRAKE_OFF)           && d2d_get_ena(SET_BRAKE_ON)        && d2d_get_ena(SET_BRAKE_OFF)    	     )
	  {
		  req_input.support.bit.brake        = TRUE;
	  }
	  
      if( d2d_get_mask(SET_KEY_IN)          && d2d_get_mask(SET_KEY_OUT)             && d2d_get_ena(SET_KEY_IN)          && d2d_get_ena(SET_KEY_OUT)      		 )
	  {
		  req_input.support.bit.key_in       = TRUE;
	  }
      
      if( d2d_get_mask(SET_PRK_NTRL_ACTIVE) && d2d_get_mask(SET_PRK_NTRL_NOT_ACTIVE) && d2d_get_ena(SET_PRK_NTRL_ACTIVE) && d2d_get_ena(SET_PRK_NTRL_NOT_ACTIVE) )
	  {
		  req_input.support.bit.park_neutral = TRUE;
	  }	

      if( d2d_get_mask(SET_HANDBRAKE_ON)    && d2d_get_mask(SET_HANDBRAKE_OFF)       && d2d_get_ena(SET_HANDBRAKE_ON)    && d2d_get_ena(SET_HANDBRAKE_OFF)		 )
	  {
		  req_input.support.bit.handbrake    = TRUE;
	  }	    
    
     CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
      
     #if DBG_SENSE == 1
        sense_notify_dbg_init () ;
        sense_notify_dbg_print("\nSense Notify Init Done" , NULL, NULL);      
      #endif    
      
     sense_notify_rke_idle();
}

static cmd_insert(unsigned cmd)
{
	cmd_q_insert(cmd,COMMAND_INSERT_TIMEOUT);
}


// M2M inputs
#if STATUS_OIL_ALARM_ENABLE > 0
void sense_notify_oil_alarm(UInt8 on)
{
	if(!status_get_oil_alarm() != !on)
	{
		status_set_oil_alarm(on);
		cmd_insert(INPUT_OIL_ALARM);
	}
}
#endif
#if STATUS_TIRE_ALARM_ENABLE > 0
void sense_notify_tire_alarm(UInt8 on)
{
	if(!status_get_tire_alarm() != !on)
	{
		status_set_tire_alarm(on);
	  cmd_insert(INPUT_TIRE_ALARM);
	}
}

#endif
#if STATUS_SEAT_BELTS_ENABLE > 0

void sense_notify_seat_belts(UInt8 bitmap)
{
	if(status_get_seat_belts() != bitmap)
	{
		status_set_seat_belts(bitmap);
		cmd_insert(INPUT_SEAT_BELTS); // value changed
	}
}
#endif
#if STATUS_SEAT_WEIGHTS_ENABLE > 0

void sense_notify_seat_weights(UInt8 bitmap)
{
	if(status_get_seat_weights() != bitmap)
	{
		status_set_seat_weights(bitmap); // value changed
		cmd_insert(INPUT_SEAT_WEIGHTS);
	}
}

#endif
#if STATUS_EV_BATTERY_LVL_ENABLE > 0

void sense_notify_ev_battery_lvl(UInt8 lvl)
{
	 if(status_get_ev_battery_lvl() != lvl)
	 {
		 status_set_ev_battery_lvl(lvl);
		 cmd_insert(INPUT_EV_BATTERY_LVL);
	 }
}
#endif
#if STATUS_EV_RANGE_ENABLE > 0

void sense_notify_ev_range(UInt16 rangekm)
{
	 if(status_get_ev_range() != rangekm)
	 {
		 status_set_ev_range(rangekm);
		 cmd_insert(INPUT_EV_RANGE);
	 }
}
#endif
#if STATUS_OIL_LIFE_ENABLE > 0


void sense_notify_oil_life(UInt8 remaining)
{
	 if(status_get_oil_life() != remaining)
	 {
		 status_set_oil_life(remaining);
		 cmd_insert(INPUT_OIL_LIFE);
	 }
}
#endif
#if STATUS_FUEL_LVL_ENABLE > 0

void sense_notify_fuel_lvl(UInt8 lvl)
{
	 if(status_get_fuel_lvl() != lvl)
	 {
		 status_set_fuel_lvl(lvl);
		 cmd_insert(INPUT_FUEL_LVL);
	 }
}

#endif
#if STATUS_FUEL_CAP_ENABLE > 0
void sense_notify_fuel_cap(UInt8 litres)
{
	 if(status_get_fuel_cap() != litres)
	 {
		 status_set_fuel_cap(litres);
		 cmd_insert(INPUT_FUEL_CAP);
	 }
}

#endif

#if STATUS_CABIN_TEMP_ENABLE > 0
void sense_notify_cabin_temp(SInt8 degC)
{
	 if(status_get_cabin_temp() != degC)
	 {
		 status_set_cabin_temp(degC);
		 cmd_insert(INPUT_CABIN_TEMP);
	 }
}

#endif

UInt8 tire_pressure_pending = 0;
#if STATUS_TIRE_PRESSURE_ENABLE > 0
void sense_notify_tire_pressure(int tirenumber, UInt8 psi)
{
	 status_set_tire_pressure(tirenumber,psi);
	
	 // don't worry about filtering lack of kPa change
	 // instead, worry not to flood cmd queue in case many tires are reported
	 if((tire_pressure_pending & 1) == 0) 
	 {
		 tire_pressure_pending|=1;
	   cmd_insert(INPUT_TIRE_PRESSURE);
	 }
}

#endif

#if STATUS_ODOMETER_ENABLE > 0
void sense_notify_odometer(UInt32 km)
{
	 if(status_get_odometer() != km)
	 {
		 status_set_odometer(km);
		 cmd_insert(INPUT_ODOMETER);
	 }
}

#endif


#if STATUS_BATTERY_VOLTAGE_ENABLE > 0
void sense_notify_battery_voltage(UInt16 millivolts)
{
	 if(status_get_battery_voltage() != millivolts)
	 {
		 status_set_battery_voltage(millivolts);
		 cmd_insert(INPUT_BATTERY_VOLTAGE);
	 }
}

#endif

// STATUS_SPEED_ENABLE was already defined for d2d


#if STATUS_TRANS_ENABLE
void sense_notify_trans(Trans value)
{
   SInt16 old = status_get_trans(STATUS_TRANS_BYTE);
   status_set_trans((UInt8)value.byte,STATUS_TRANS_BYTE, STATUS_SOURCE_1);  // Status_Src argument is 0 for now, see sense_notify_init() 
	 
   if(old != status_get_trans(STATUS_TRANS_BYTE))
	 {
     cmd_insert(INPUT_TRANS_CHANGE);
	 }
}
#endif

