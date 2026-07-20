/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk_flash.c 1106 2010-04-27 13:30:39Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_can_private.h"

#ifdef PLATFORM_CM800
	#include "fob_ctrl.h"
#endif
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

//--------------------------------------------------------------------------//

void sense_rs_can_init( Sense_Rs_Can_Cfg *cfg )
{			    
    sense_rs_can_cfg = cfg ? cfg : (Sense_Rs_Can_Cfg *)&sense_rs_can_default_cfg ;
            
    sense_rs_can_oem_active    = TRUE  ; 
    sense_rs_can_brake_skip    = FALSE ;
	
	#ifdef PLATFORM_CM800
		sense_rs_can_cfg->oem_lock_timeout = fob_ctrl_get_tbp() / 10 ; 
	#endif
    
    sense_rs_can_driver_door_init ();
    sense_rs_can_other_doors_init (); 
    sense_rs_can_all_doors_init   ();   
	sense_rs_can_key_in_init      ();
	sense_rs_can_ign_init         ();
	sense_rs_can_rpm_init         ();
	sense_rs_can_speed_init       ();	    
    sense_rs_can_hood_init        ();
    sense_rs_can_trunk_init       ();    
    sense_rs_can_handbrake_init   ();    
    sense_rs_can_brake_init       ();
    sense_rs_can_glowplug_init    ();
	sense_rs_can_oem_disable_init ();
	sense_rs_can_oem_lock_init    ();
    sense_rs_can_oem_unlock_init  ();
    sense_rs_can_oem_trunk_init   ();
    sense_rs_can_oem_autolock_init();
	sense_rs_can_skd_init         ();
	sense_rs_can_neutral_init     ();
	
	TRACE("\r\nSENSE RS CAN INIT\r\n");	 
}



//--------------------------------------------------------------------------//






 


