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
         
      rpm.word   = 0 ;
      key.byte   = 0 ;
      door.byte  = 0 ;
      trans.byte = 0 ;
      brake.byte = 0 ;
    
      req_input.status.byte  = 0x00 ;

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


//////////////// stubs for m2m items not supported by dball  //////////////////////////////////


// M2M inputs
#if STATUS_OIL_ALARM_ENABLE > 0
void sense_notify_oil_alarm(UInt8 on)
{
}
#endif
#if STATUS_TIRE_ALARM_ENABLE > 0
void sense_notify_tire_alarm(UInt8 on)
{
}

#endif
#if STATUS_SEAT_BELTS_ENABLE > 0

void sense_notify_seat_belts(UInt8 bitmap)
{
}
#endif
#if STATUS_SEAT_WEIGHTS_ENABLE > 0

void sense_notify_seat_weights(UInt8 bitmap)
{
}

#endif
#if STATUS_EV_BATTERY_LVL_ENABLE > 0

void sense_notify_ev_battery_lvl(UInt8 lvl)
{
}
#endif
#if STATUS_EV_RANGE_ENABLE > 0

void sense_notify_ev_range(UInt16 rangekm)
{
}
#endif
#if STATUS_OIL_LIFE_ENABLE > 0


void sense_notify_oil_life(UInt8 remaining)
{
}
#endif
#if STATUS_FUEL_LVL_ENABLE > 0

void sense_notify_fuel_lvl(UInt8 lvl)
{
}

#endif
#if STATUS_FUEL_CAP_ENABLE > 0
void sense_notify_fuel_cap(UInt8 litres)
{
}

#endif

#if STATUS_CABIN_TEMP_ENABLE > 0
void sense_notify_cabin_temp(SInt8 degC)
{
}

#endif

UInt8 tire_pressure_pending = 0;
#if STATUS_TIRE_PRESSURE_ENABLE > 0
void sense_notify_tire_pressure(int tirenumber, UInt8 psi)
{
}

#endif

#if STATUS_ODOMETER_ENABLE > 0
void sense_notify_odometer(UInt32 km)
{
}

#endif


#if STATUS_BATTERY_VOLTAGE_ENABLE > 0
void sense_notify_battery_voltage(UInt16 millivolts)
{
}

#endif


