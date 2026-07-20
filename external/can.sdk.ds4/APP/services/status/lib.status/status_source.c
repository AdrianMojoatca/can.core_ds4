/*==========================================================================*/
// $Id: status_source.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_private.h"
#include "prg_out.h"
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
Boolean status_set_source (Status_Src source , Status status)
{
	if(status == ALL_STATUS && source)
	{
	 	UInt8 list ;
		for (list = 0 ; list < ALL_STATUS ; list++)
		{
			status_entry.updater[list] = source ;	
		}
	}
	else if (status < ALL_STATUS && source)
	{
		status_entry.updater[status] = source ;
	}
	else
	{
	 	return FALSE ;
	}

	return TRUE ;
}

//--------------------------------------------------------------------------//

Status_Src status_get_source (Status status)
{
	if (status < ALL_STATUS)
	{
		return status_entry.updater[status] ;
	}

	return (Status_Src)-1 ;
}

//--------------------------------------------------------------------------//


////////////////// M2M extensions ////////////////////
#if STATUS_OIL_ALARM_ENABLE > 0
static Boolean oil_alarm = 0;
void status_set_oil_alarm(Boolean alarm)
{
	oil_alarm = alarm;
}
Boolean status_get_oil_alarm(void)
{
	return oil_alarm;
}
#endif

#if STATUS_TIRE_ALARM_ENABLE > 0
static Boolean tire_alarm = 0;
void status_set_tire_alarm(Boolean alarm)
{
	tire_alarm = alarm;
}
Boolean status_get_tire_alarm(void)
{
	return tire_alarm;
}
#endif

#if STATUS_SEAT_BELTS_ENABLE > 0
static UInt8 seat_belts = 0;
void status_set_seat_belts(UInt8 bitmap)
{
	seat_belts = bitmap;
}
UInt8 status_get_seat_belts(void)
{
	return seat_belts;
}
#endif

#if STATUS_SEAT_WEIGHTS_ENABLE > 0
static UInt8 seat_weights = 0;
void status_set_seat_weights(UInt8 bitmap)
{
	seat_weights = bitmap;
}
UInt8 status_get_seat_weights(void)
{
	return seat_weights;
}
#endif

#if STATUS_EV_BATTERY_LVL_ENABLE > 0
static UInt8 ev_battery_lvl = 0;
void status_set_ev_battery_lvl(UInt8 lvl)
{
	  ev_battery_lvl = lvl;
}
UInt8 status_get_ev_battery_lvl(void)
{
	return ev_battery_lvl;
}
#endif
#if STATUS_EV_RANGE_ENABLE > 0
static UInt16 ev_range = 0;
void status_set_ev_range(UInt16 rangekm)
{
	  ev_range = rangekm;
}
UInt8 status_get_ev_range(void)
{
	return ev_range;
}
#endif
#if STATUS_OIL_LIFE_ENABLE > 0
static UInt8 oil_life = 0;
void status_set_oil_life(UInt8 remaining)
{
	  oil_life = remaining;
}
UInt8 status_get_oil_life(void)
{
	return oil_life;
}
#endif
#if STATUS_FUEL_LVL_ENABLE > 0
static UInt8 fuel_lvl = 0;
void status_set_fuel_lvl(UInt8 lvl)
{
	  fuel_lvl = lvl;
}
UInt8 status_get_fuel_lvl(void)
{
	return fuel_lvl;
}
#endif
#if STATUS_FUEL_CAP_ENABLE > 0
static UInt8 fuel_cap = 0;
void status_set_fuel_cap(UInt8 litres)
{
	  fuel_cap = litres;
}
UInt8 status_get_fuel_cap(void)
{
	return fuel_cap;
}
#endif

#if STATUS_CABIN_TEMP_ENABLE > 0
static SInt8 cabin_temp = 0;
void status_set_cabin_temp(SInt8 degC)
{
	  cabin_temp = degC;
}
SInt8 status_get_cabin_temp(void)
{
	return cabin_temp;
}
#endif

#if STATUS_TIRE_PRESSURE_ENABLE > 0
#define MAX_WHEELS 18
static int tire_count = 0;
static UInt8 tire_pressure[MAX_WHEELS]; // front-before-back, inner-before-outer, left-before-right
void status_set_tire_pressure(int tire, UInt8 psi)
{
	if(tire < MAX_WHEELS)
	{
	  if(tire >= tire_count)
		  tire_count = 1+tire; // highest tire number seen so far
	  tire_pressure[tire] = psi;
  } 
}
int status_get_tire_pressure(int tire)
{
	if(tire < 0) // do they ask for length?
	  return tire_count;	
	return (tire < MAX_WHEELS) ? tire_pressure[tire] : 0;
}
#endif

// this is a status unconditionally supported by core
Boolean status_get_kib_state(void)
{
	return prg_out_shadow_read(1+PRG_OUT_FOB); // show the last thing we asked for
}

#if STATUS_ODOMETER_ENABLE > 0
static UInt32 odometer_km = 0;
UInt32 status_get_odometer(void)
{
	return odometer_km;
}
void status_set_odometer(UInt32 km)
{
	odometer_km = km;
}
#endif

#if STATUS_BATTERY_VOLTAGE_ENABLE > 0
static UInt16 battery_millivolts = 0;
UInt16 status_get_battery_voltage(void)
{
	return battery_millivolts;
}
void status_set_battery_voltage(UInt16 millivolts)
{
	battery_millivolts = millivolts;
}
#endif

#if STATUS_SPEED_ENABLE > 0

UInt16 status_get_m2m_speed(void)
{
	return status_get_speed(STATUS_SPEED_BYTE);
}

#endif

