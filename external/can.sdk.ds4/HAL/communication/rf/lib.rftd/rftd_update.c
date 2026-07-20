/*==========================================================================*/
// $Id: rftd_update.c 33406 2015-08-26 21:25:10Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "rf.h"
#include "rftd_private.h"
#include "bitfield.h"
#include "d2d.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void update_bytes(void)           ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rftd_update (void)
{
  CRITICAL ;

  C_ENTER ;
    {
      UInt8 data [5] ;

			update_bytes() ;

      // pass TO D2D

      data[0] = SET_2WAY_STATUS;
      data[1] = rftd.byte1.byte;
      data[2] = rftd.byte2.byte;
      data[3] = rftd.byte3.byte;
      data[4] = rftd.byte4.byte;

      d2d_tx_frame (&data[0] , sizeof_array(data)) ;
    }
  C_LEAVE ;
}

//--------------------------------------------------------------------------//
Boolean rf_data_icon_bit(UInt8 num)
{
	return bitfield_bit(rf_data.icon.bit,num);
}

static void update_bytes(void)
{
	rftd.byte1.byte =
	rftd.byte2.byte =
	rftd.byte3.byte = 0;

  rftd.byte4.byte = rf_data.sound ;

	
// byte1	
	if(rf_data_icon_bit(ICON_LOCK))
		rftd.byte1.bit.lock = 1;
	
  if(rf_data_icon_bit(ICON_ARMED))
    rftd.byte1.bit.armed    = 1;
	
  if(rf_data_icon_bit(ICON_IGNITION))
    rftd.byte1.bit.ignition = 1;
	
  if(rf_data_icon_bit(ICON_PARKING))
    rftd.byte1.bit.parking  = 1;
	
	if(rf_data_icon_bit(ICON_VALET))
    rftd.byte1.bit.valet    = 1;

  if(rf_data_icon_bit(ICON_HORN))
    rftd.byte1.bit.siren    = 1;

	if(rf_data_icon_bit(ICON_PANIC))
    rftd.byte1.bit.panic    = 1;
	
	if(rf_data_icon_bit(ICON_SUCCESS))
    rftd.byte1.bit.started  = 1;

	
// byte2
	if(rf_data_icon_bit(ICON_DOOR))
    rftd.byte2.bit.door      = 1;

	if(rf_data_icon_bit(ICON_TRUNK))
    rftd.byte2.bit.trunk     = 1;

	if(rf_data_icon_bit(ICON_HOOD))
    rftd.byte2.bit.hood      = 1;

	if(rf_data_icon_bit(ICON_WARN))
    rftd.byte2.bit.warn      = 1;
    
	if(rf_data_icon_bit(ICON_SHOCK))
    rftd.byte2.bit.shock     = 1;

	if((rf_data_icon_bit(ICON_OTP)) || (rf_data_icon_bit(ICON_TIME)))
    rftd.byte2.bit.time      = 1;

	if(rf_data_icon_bit(ICON_TEMP))
    rftd.byte2.bit.temp      = 1;

	if(rf_data_icon_bit(ICON_TEMP_TYPE))
    rftd.byte2.bit.temp_type = 1;

// byte3
	if(rf_data_icon_bit(ICON_NOT_READY))
    rftd.byte3.bit.not_ready = 1;


	
  bitfield_wr(rf_data.icon.bit , ICON_TIME , ICON_TIME , FALSE ) ;	// clear non-permanent bits time and temp
  
}

//--------------------------------------------------------------------------//



