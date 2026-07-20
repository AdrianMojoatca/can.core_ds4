/*==========================================================================*/
// $Id: rftd2_update_support.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "rf.h"
#include "d2d.h"
#include "rftd2_private.h"
#include "prog.h"
#include "bitfield.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define SUPPORT_CHANGE ((rftd2_entry.support.byte[0] ^ support_send[1]) | (rftd2_entry.support.byte[1] ^ support_send[2]) | (rftd2_entry.support.byte[2] ^ support_send[3]))

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void rftd2_update_bytes(void)           ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static UInt8 support_send[4] ;
 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rftd2_update_support (void)
{
    UInt8 temp_support_send[4];

    if (prog_retrieve_option (FEAT_RF_OUTPUT) != RF_TD2)    // If RF_OUTPUT not already set, update
    {
        prog_set_option(RF_TD2, FEAT_RF_OUTPUT) ;
    }
    
       
        if(rftd2_update_support_change())
        {
            nvfs_wr (nvfs_rftd2_entry, &rftd2_entry) ;
        }

        // We don't want to update the global support_send[] bytes
        // so the SUPPORT_CHANGE bit will not be cleared
        temp_support_send[0] = SET_EXTENDED_STATUS_SUPPORT ;
        temp_support_send[1] = rftd2_entry.support.byte[0] ;
        temp_support_send[2] = rftd2_entry.support.byte[1] ;
        temp_support_send[3] = rftd2_entry.support.byte[2] ;
    
        
    d2d_tx_frame (&temp_support_send[0] , sizeof_array(support_send)) ;  
}

//--------------------------------------------------------------------------//

void rftd2_update_support_sync (void)
{
    if (prog_retrieve_option (FEAT_RF_OUTPUT) != RF_TD2)    // If RF_OUTPUT not already set, update
    {
        prog_set_option(RF_TD2, FEAT_RF_OUTPUT) ;
    }
    
       
        if(rftd2_update_support_change())
        {
            nvfs_wr (nvfs_rftd2_entry, &rftd2_entry) ;
        }
        
        support_send[0] = SET_EXTENDED_STATUS_SUPPORT ;
        support_send[1] = rftd2_entry.support.byte[0] ;
        support_send[2] = rftd2_entry.support.byte[1] ;
        support_send[3] = rftd2_entry.support.byte[2] ;
    
        
    d2d_tx_frame (&support_send[0] , sizeof_array(support_send)) ;  
}

//--------------------------------------------------------------------------//

Boolean rftd2_update_support_change (void)
{
    rftd2_update_bytes() ;

    return (!!SUPPORT_CHANGE) ;
}

Boolean rf_support_icon_bit(UInt8 num)
{
	return bitfield_bit(rf_support.icon.bit,num);
} 
//--------------------------------------------------------------------------//
static void rftd2_update_bytes(void)
{ // if nvfs memory support is true or rf icon suport is true set to true

		if(rf_support_icon_bit(ICON_LOCK))
			rftd2_entry.support.bit.lock = 1;
		if(rf_support_icon_bit(ICON_ARMED))
      rftd2_entry.support.bit.arm = 1;
		if(rf_support_icon_bit(ICON_IGNITION))
      rftd2_entry.support.bit.ignition = 1;
		if(rf_support_icon_bit(ICON_PARKING))
      rftd2_entry.support.bit.parking_light = 1;
		if(rf_support_icon_bit(ICON_HORN))
      rftd2_entry.support.bit.siren = 1;
		if(rf_support_icon_bit(ICON_VALET))
      rftd2_entry.support.bit.valet = 1;
		if(rf_support_icon_bit(ICON_SUCCESS))
      rftd2_entry.support.bit.is_running = 1;
		if(rf_support_icon_bit(ICON_PANIC))
      rftd2_entry.support.bit.panic = 1;
		
    if(rf_support_icon_bit(ICON_DOOR))
      rftd2_entry.support.bit.door = 1;
		if(rf_support_icon_bit(ICON_TRUNK))
      rftd2_entry.support.bit.trunk = 1;
		if(rf_support_icon_bit(ICON_HOOD))
      rftd2_entry.support.bit.hood = 1;
		if(rf_support_icon_bit(ICON_WARN))
      rftd2_entry.support.bit.warn_away = 1;
		if(rf_support_icon_bit(ICON_SHOCK))
      rftd2_entry.support.bit.shock_sensor = 1;
		if(rf_support_icon_bit(ICON_NOT_READY))
      rftd2_entry.support.bit.ready_mode = 1;

    #if STATUS_DTC_ENABLE == 1
      rftd2_entry.support.bit.dtc_code = (bitfield_rd(rf_support.icon.bit, ICON_DTC, ICON_DTC) | OBD_CAN_DTC_CODE_MASK) & obd_can_is_active() ;
    #else
		  if(rf_support_icon_bit(ICON_DTC))
        rftd2_entry.support.bit.dtc_code = 1;
    #endif

		if(rf_support_icon_bit(ICON_ALERT))
      rftd2_entry.support.bit.alert = 1;
		if(rf_support_icon_bit(ICON_SUCCESS))
      rftd2_entry.support.bit.eng_run_change = 1;
		if(rf_support_icon_bit(ICON_PANIC))
      rftd2_entry.support.bit.panic_change = 1;
		if(rf_support_icon_bit(ICON_RS_ARMED))
      rftd2_entry.support.bit.rs_arm = 1;
		if(rf_support_icon_bit(ICON_RS_HORN))
      rftd2_entry.support.bit.rs_siren = 1;
		if(rf_support_icon_bit(ICON_RS_PANIC))
      rftd2_entry.support.bit.rs_panic = 1;

    rftd2_entry.support.bit.support_change  = 1 ; // always supported, part of the protocol
}

//--------------------------------------------------------------------------//



