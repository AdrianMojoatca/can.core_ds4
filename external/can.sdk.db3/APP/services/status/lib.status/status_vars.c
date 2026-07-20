/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_vars.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

Status_Entry status_entry ;

#if STATUS_VIN_ENABLE == 1
Vin vin ;
#endif
#if STATUS_RKE_ENABLE == 1
Rke rke ;
#endif
#if STATUS_KEY_ENABLE == 1
Key key ;
#endif
#if STATUS_RPM_ENABLE == 1
Rpm rpm ;
#endif
#if STATUS_DOOR_ENABLE == 1
Door door ;
#endif
#if STATUS_SPEED_ENABLE == 1
Speed speed ;
#endif
#if STATUS_BRAKE_ENABLE == 1
Brake brake ;
#endif
#if STATUS_TRANS_ENABLE == 1
Trans trans ;
#endif
#if STATUS_ALARM_ENABLE == 1
Alarm alarm ;
#endif
#if STATUS_DOORLOCK_ENABLE == 1
Doorlock_Status doorlock_status ;
#endif
#if STATUS_ENGINE_ENABLE == 1
Engine engine ;
#endif
#if STATUS_COMFORT_ENABLE == 1
Comfort  comfort ;
Comfort2 comfort2 ;
#endif
#if STATUS_TEMPERATURE_ENABLE == 1
Temperature temperature ;
#endif
#if STATUS_REQ_INPUT_ENABLE == 1
Req_Input_Status req_input ;
#endif
#if STATUS_DTC_ENABLE == 1
Dtc dtc ;
#endif
#if STATUS_MILEAGE_ENABLE == 1
Mileage mileage ;
#endif
#if STATUS_TPMS_ENABLE == 1
Tpms tpms ;
#endif

Status_Light status_light ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
