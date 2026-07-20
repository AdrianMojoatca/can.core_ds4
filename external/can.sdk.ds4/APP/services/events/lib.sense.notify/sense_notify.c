/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "status.h"
#include "sense_notify.h         " 
#include "sense_notify_init.c    "
#include "sense_notify_vars.c    " 
#include "sense_notify_keyin.c   "
#include "sense_notify_dbg_print.c"

  
#if STATUS_ALARM_ENABLE == 1                  
  #include "sense_notify_alarm.c   " 
#endif    
#if STATUS_BRAKE_ENABLE == 1                  
  #include "sense_notify_brake.c   " 
#endif  
#if STATUS_DOOR_ENABLE == 1                   
  #include "sense_notify_door.c    "
#endif 
#if STATUS_DOORLOCK_ENABLE == 1                    
  #include "sense_notify_doorlock.c"
#endif
#if STATUS_BRAKE_ENABLE == 1                        
  #include "sense_notify_hbrake.c  "  
#endif   
#if STATUS_DOOR_ENABLE == 1                
  #include "sense_notify_hood.c    "   
#endif  
#if STATUS_KEY_ENABLE == 1               
  #include "sense_notify_ign.c     "    
#endif
#if STATUS_RKE_ENABLE == 1                                  
  #include "sense_notify_rke.c     " 
#endif  
#if STATUS_RPM_ENABLE == 1                
  #include "sense_notify_rpm.c     "    
#endif
#if STATUS_RKE_ENABLE == 1                 
  #include "sense_notify_skd.c     "
#endif
#if STATUS_SPEED_ENABLE == 1                     
  #include "sense_notify_speed.c   " 
#endif
#if STATUS_DOOR_ENABLE == 1                    
  #include "sense_notify_trunk.c   "                   
#endif
#if STATUS_DOOR_ENABLE == 1    
  #include "sense_notify_door_trunk.c   "  
#endif    

#include "sense_notify_misc.c"

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

