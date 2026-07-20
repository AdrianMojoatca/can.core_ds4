/*==========================================================================*/
// $Id: dei_sc_icon_warning.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
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

Boolean dei_sc_icon_warning( Notify_Pld_Sensor_Warn warn )
{      
    dei_sc_info_set( DEI_SC_INFO_DOOR_TRIGGERED        , !!(warn&NOTIFY_PLD_TRIG_DOOR )) ; 
    dei_sc_info_set( DEI_SC_INFO_HOOD_TRIGGERED        , !!(warn&NOTIFY_PLD_TRIG_HOOD )) ; 
    dei_sc_info_set( DEI_SC_INFO_TRUNK_TRIGGERED       , !!(warn&NOTIFY_PLD_TRIG_TRUNK)) ; 
    dei_sc_info_set( DEI_SC_INFO_IGNITION_TRIGGERED    , !!(warn&NOTIFY_PLD_TRIG_IGN  )) ; 
	
    dei_sc_info_set( DEI_SC_INFO_SENSOR1_TRIGGERED     , !!(warn&NOTIFY_PLD_TRIG_S1  )) ; 
    dei_sc_info_set( DEI_SC_INFO_SENSOR2_TRIGGERED     , !!(warn&NOTIFY_PLD_TRIG_S2  )) ; 
    dei_sc_info_set( DEI_SC_INFO_SENSOR3_TRIGGERED     , !!(warn&NOTIFY_PLD_TRIG_S3  )) ; 

    return dei_sc_tx( DEI_SC_ACK_ALARM_WARNING_EVENT , DEI_SC_PAGGING_TX , 100 ) ; 
}

//--------------------------------------------------------------------------//

