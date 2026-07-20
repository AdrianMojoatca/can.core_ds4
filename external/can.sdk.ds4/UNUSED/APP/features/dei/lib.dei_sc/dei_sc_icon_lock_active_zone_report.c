/*==========================================================================*/
// $Id: dei_sc_icon_lock_active_zone_report.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Sc_Azr
{
    Vrtc vrtc  ;
}Dei_Sc_Azr;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_sc_icon_lock_active_zone_report_execute( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Sc_Azr  dei_sc_azr_vars ; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_sc_icon_lock_active_zone_report_init( void )
{
	dei_sc_azr_vars.vrtc = vrtc_f_create( (Exec_Func)dei_sc_icon_lock_active_zone_report_execute );
}	

//--------------------------------------------------------------------------//

Boolean dei_sc_icon_lock_active_zone_report( void )
{    
    vrtc_stop( dei_sc_azr_vars.vrtc );

	vrtc_update( dei_sc_azr_vars.vrtc , DEI_SC_VRTC_TIMEOUT_LOCK_ZONE_ACTIVE ) ; 

	vrtc_start( dei_sc_azr_vars.vrtc );

	return TRUE;
}

//--------------------------------------------------------------------------//

void dei_sc_icon_lock_active_zone_report_execute( void )
{
	dei_sc_tx( DEI_SC_ACK_LOCK_WITH_ACTIVE_ZONE_REPORT , DEI_SC_PAGGING_TX , 100 );
}

//--------------------------------------------------------------------------//

Boolean  dei_sc_icon_active_zone( Notify_Pld_Zone_Report zone )
{
    dei_sc_info_set( DEI_SC_INFO_DOOR_STATUS    , !!(zone & NOTIFY_PLD_ZONE_DOOR    ) );
    dei_sc_info_set( DEI_SC_INFO_TRUNK_STATUS   , !!(zone & NOTIFY_PLD_ZONE_TRUNK   ) );
    dei_sc_info_set( DEI_SC_INFO_HOOD_STATUS    , !!(zone & NOTIFY_PLD_ZONE_HOOD    ) );
	dei_sc_info_set( DEI_SC_INFO_IGNITION_STATUS, !!(zone & NOTIFY_PLD_ZONE_IGN     ) );
	dei_sc_info_set( DEI_SC_INFO_SENSOR1_NPC    , !!(zone & NOTIFY_PLD_ZONE_S1_NPC  ) );
	dei_sc_info_set( DEI_SC_INFO_SENSOR2_NPC    , !!(zone & NOTIFY_PLD_ZONE_S2_NPC  ) );
	dei_sc_info_set( DEI_SC_INFO_SENSOR3_NPC    , !!(zone & NOTIFY_PLD_ZONE_S3_NPC  ) );
    
    return dei_sc_tx( DEI_SC_ACK_LOCK_WITH_ACTIVE_ZONE_REPORT , DEI_SC_PAGGING_TX , 100 );
}
