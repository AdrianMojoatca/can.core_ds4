/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_sensor_type_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#ifdef PLATFORM_933
  #include "sense_rs.h"
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

void dei_ss_sensor_type_init( void )
{
	static const Notify_Pld_Sensor_Type  sensor_icon[] = 
	{
		NOTIFY_PLD_SENSOR_NONE      , 
    	NOTIFY_PLD_SENSOR_SHOCK     , 
    	NOTIFY_PLD_SENSOR_FIELD     , 
    	NOTIFY_PLD_SENSOR_TILT      ,
    	NOTIFY_PLD_SENSOR_GLASS     ,
    	NOTIFY_PLD_SENSOR_ULTRA 
	};

#ifdef PLATFORM_CM800

	notify_send( NOTIFY_SENSOR1_TYPE  , sensor_icon[ dei_feature_get( DEI_FEAT_SENSOR_TYPE )-1 ] ) ; 

	ss_sensor = sensor_create( SS_NPC_COUT_TRIGGER , DEI_SS_VRTC_TIMEOUT_NPC_TRIGGER ,(dei_feature_get( DEI_FEAT_NUISANCE_PREVENTION )==1), \
	 						   SS_NPC_COUNT_WARN   , DEI_SS_VRTC_TIMEOUT_NPC_WARN    , TRUE);

#else

	notify_send( NOTIFY_SENSOR1_TYPE  , sensor_icon[ 1 ] ) ; 
	notify_send( NOTIFY_SENSOR2_TYPE  , sensor_icon[ 3 ] ) ; 
	notify_send( NOTIFY_SENSOR3_TYPE  , sensor_icon[ 0 ] ) ; //dei_feature_get( DEI_FEAT_SENSOR_TYPE )-1 ] ) ; 

	// Initialization for Shock Sensor
	ss_sensor[0] = sensor_create( SS_NPC_COUT_TRIGGER , DEI_SS_VRTC_TIMEOUT_NPC_TRIGGER ,(dei_feature_get( DEI_FEAT_NUISANCE_PREVENTION )==1), \
	 						   SS_NPC_COUNT_WARN   , DEI_SS_VRTC_TIMEOUT_NPC_WARN    , TRUE);
	
	// Initialization for Tilt Sensor
	ss_sensor[1] = sensor_create( SS_NPC_COUT_TRIGGER , DEI_SS_VRTC_TIMEOUT_NPC_TRIGGER ,(dei_feature_get( DEI_FEAT_NUISANCE_PREVENTION )==1), \
	 						   SS_NPC_COUNT_WARN   , DEI_SS_VRTC_TIMEOUT_NPC_WARN    , TRUE);
	
	
	// Initialization for unknown sensor
	ss_sensor[2] = sensor_create( SS_NPC_COUT_TRIGGER , DEI_SS_VRTC_TIMEOUT_NPC_TRIGGER ,(dei_feature_get( DEI_FEAT_NUISANCE_PREVENTION )==1), \
	 						   SS_NPC_COUNT_WARN   , DEI_SS_VRTC_TIMEOUT_NPC_WARN    , TRUE);
								 
  if(ext_sensor_trigger_configured)  // make sure initialized after sense_rs_pi_init(core_pre_init)
	{
		ss_sensor_ext = sensor_create( SS_NPC_COUT_TRIGGER , DEI_SS_VRTC_TIMEOUT_NPC_TRIGGER ,(dei_feature_get( DEI_FEAT_NUISANCE_PREVENTION )==1), \
	 						   SS_NPC_COUNT_WARN   , DEI_SS_VRTC_TIMEOUT_NPC_WARN    , TRUE);
	}
								 
#endif
}


//--------------------------------------------------------------------------//

