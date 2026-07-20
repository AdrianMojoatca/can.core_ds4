/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk.h 1106 2010-04-27 13:30:39Z florin.olariu $
/*==========================================================================*/

#ifndef __SENSE_RS_CAN_H__
#define __SENSE_RS_CAN_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

typedef struct s_Sense_Rs_Can_Cfg
{
    UInt8 brake_debounce      ;
	UInt8 neutral_debounce    ; 
    UInt8 handbrake_debounce  ; 	
        
    UInt32 oem_skip_timeout   ;
	UInt32 oem_lock_timeout   ;

    Boolean speed_shutdown_on ;

}Sense_Rs_Can_Cfg ;     

/*--------------------------------------------------------------------------*/

#define SENSE_RS_CAN_BRAKE_DEBOUNCE		    5
#define SENSE_RS_CAN_NEUTRAL_DEBOUNCE		5
#define SENSE_RS_CAN_HANDBRAKE_DEBOUNCE		/*35*/ 3

#define SENSE_RS_CAN_OEM_SKIP_TIMEOUT       50  //for 500 mseconds
#define SENSE_RS_CAN_OEM_LOCK_TIMEOUT       50  //for 500 mseconds

/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

void     sense_rs_can_init        	  ( Sense_Rs_Can_Cfg *cfg );

Boolean  sense_rs_driver_door_can 	  ( Boolean instant_level );
Boolean  sense_rs_other_doors_can 	  ( Boolean instant_level );
Boolean  sense_rs_hood_can        	  ( Boolean instant_level );
Boolean  sense_rs_trunk_can       	  ( Boolean instant_level );
Boolean  sense_rs_ign_can         	  ( Boolean instant_level );
Boolean  sense_rs_key_in_can      	  ( Boolean instant_level );
Boolean  sense_rs_brake_can       	  ( Boolean instant_level );
Boolean  sense_rs_handbrake_can   	  ( Boolean instant_level );
Boolean  sense_rs_glowplug_off_can	  ( Boolean instant_level );
Boolean  sense_rs_neutral_can         ( Boolean instant_level );

void 	 sense_rs_rpm_can         	  ( UInt16 new_rpm );
void     sense_rs_can_rpm_fake_enter  ( UInt16 rpm_fake);
void 	 sense_rs_can_rpm_fake_leave  ( void );
UInt16 	 sense_rs_can_get_real_rpm	  ( void );
void     sense_rs_speed_can			  ( UInt32 new_speed );

Boolean  sense_rs_oem_lock_can    	  ( Boolean instant_level );
Boolean  sense_rs_oem_autolock_can	  ( Boolean instant_level );
Boolean  sense_rs_oem_unlock_can  	  ( Boolean instant_level );
Boolean  sense_rs_oem_trunk_can   	  ( Boolean instant_level );
Boolean  sense_rs_skd_can			  ( Boolean instant_level );

void     sense_rs_oem_disable     	  ( UInt32 ten_msec_ticks );

void     sense_rs_can_brake_force_on  ( void );
void     sense_rs_can_brake_force_off ( void );
void 	 sense_rs_can_brake_skip_enter( void );
void 	 sense_rs_can_brake_skip_leave( void );
Boolean  sense_rs_can_brake_get_status( void );
void     sense_rs_can_set_dl_status   ( Boolean status );
Boolean  sense_rs_can_get_dl_status   ( void );

#endif


