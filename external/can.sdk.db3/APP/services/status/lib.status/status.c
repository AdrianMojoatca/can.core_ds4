/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status.h          " 
#include "status_source.c   " 
#include "status_vars.c     "  
#if STATUS_ALARM_ENABLE == 1                  
  #include "status_alarm.c    "  
#endif 
#if STATUS_BRAKE_ENABLE == 1                
  #include "status_brake.c    " 
#endif
#if STATUS_COMFORT_ENABLE == 1                  
  #include "status_comfort.c  "
#endif   
#if STATUS_DOOR_ENABLE == 1                
  #include "status_door.c     "
#endif
#if STATUS_DOORLOCK_ENABLE == 1                   
  #include "status_doorlock.c " 
#endif
#if STATUS_DTC_ENABLE == 1                  
  #include "status_dtc.c      "  
#endif
#if STATUS_ENGINE_ENABLE == 1                 
  #include "status_engine.c   "  
#endif 
#if STATUS_KEY_ENABLE == 1                
  #include "status_key.c      " 
#endif
#if STATUS_MILEAGE_ENABLE == 1                  
  #include "status_mileage.c  "    
#endif
#if STATUS_RKE_ENABLE == 1               
  #include "status_rke.c      "  
#endif
#if STATUS_RPM_ENABLE == 1                 
  #include "status_rpm.c      "   
#endif                
#if STATUS_SPEED_ENABLE == 1                  
  #include "status_speed.c    "
#endif
#if STATUS_TEMPERATURE_ENABLE == 1                   
  #include "status_temp.c     " 
#endif
#if STATUS_TPMS_ENABLE == 1                  
  #include "status_tpms.c     "
#endif
#if STATUS_TRANS_ENABLE ==1                  
  #include "status_trans.c    "    
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
