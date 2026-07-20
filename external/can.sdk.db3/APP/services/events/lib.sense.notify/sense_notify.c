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

void sense_notify_parking_on (void) 
{
  if(!status_light.parking)
    {     
      status_light.parking = TRUE ;

      cmd_q_insert(INPUT_PARKING_STATUS_ON, COMMAND_INSERT_TIMEOUT);
     
      #if DBG_SENSE == 1
              
        sense_notify_dbg_print("\nSense Parking On" , NULL , NULL);
      
      #endif

    }   
}

//--------------------------------------------------------------------------------

void sense_notify_parking_off (void) 
{ 
  if(status_light.parking) 
    {
      status_light.parking = FALSE ;
      
      cmd_q_insert(INPUT_PARKING_STATUS_OFF, COMMAND_INSERT_TIMEOUT);
                  
      #if DBG_SENSE == 1
       
        sense_notify_dbg_print("\nSense Parking Off" , NULL , NULL);
      
      #endif
    }  
}
//--------------------------------------------------------------------------------
void sense_notify_hazard_on (void) 
{
  if(!status_light.hazard)
    {     
      status_light.hazard = TRUE ;

      cmd_q_insert(INPUT_HAZARD_STATUS_ON, COMMAND_INSERT_TIMEOUT);
     
      #if DBG_SENSE == 1
              
        sense_notify_dbg_print("\nSense Hazard On" , NULL , NULL);
      
      #endif

    }   
}

//--------------------------------------------------------------------------------

void sense_notify_hazard_off (void) 
{ 
  if(status_light.hazard) 
    {
      status_light.hazard = FALSE ;
      
      cmd_q_insert(INPUT_HAZARD_STATUS_OFF, COMMAND_INSERT_TIMEOUT);
                  
      #if DBG_SENSE == 1
       
        sense_notify_dbg_print("\nSense Hazard Off" , NULL , NULL);
      
      #endif
    }  
}
//--------------------------------------------------------------------------------
void sense_notify_lo_beam_on (void) 
{
  if(!status_light.lo_beam)
    {     
      status_light.lo_beam = TRUE ;

      cmd_q_insert(INPUT_LO_BEAM_STATUS_ON, COMMAND_INSERT_TIMEOUT);
     
      #if DBG_SENSE == 1
              
        sense_notify_dbg_print("\nSense Lo Beam On" , NULL , NULL);
      
      #endif

    }   
}

//--------------------------------------------------------------------------------

void sense_notify_lo_beam_off (void) 
{ 
  if(status_light.lo_beam) 
    {
      status_light.lo_beam = FALSE ;
      
      cmd_q_insert(INPUT_LO_BEAM_STATUS_OFF, COMMAND_INSERT_TIMEOUT);
                  
      #if DBG_SENSE == 1
       
        sense_notify_dbg_print("\nSense Lo Beam Off" , NULL , NULL);
      
      #endif
    }  
}
//--------------------------------------------------------------------------------
void sense_notify_hi_beam_on (void) 
{
  if(!status_light.hi_beam)
    {     
      status_light.hi_beam = TRUE ;

      cmd_q_insert(INPUT_HI_BEAM_STATUS_ON, COMMAND_INSERT_TIMEOUT);
     
      #if DBG_SENSE == 1
              
        sense_notify_dbg_print("\nSense Hi Beam On" , NULL , NULL);
      
      #endif

    }   
}

//--------------------------------------------------------------------------------
void sense_notify_hi_beam_off (void) 
{ 
  if(status_light.hi_beam) 
    {
      status_light.hi_beam = FALSE ;
      
      cmd_q_insert(INPUT_HI_BEAM_STATUS_OFF, COMMAND_INSERT_TIMEOUT);
                  
      #if DBG_SENSE == 1
       
        sense_notify_dbg_print("\nSense Hi Beam Off" , NULL , NULL);
      
      #endif
    }  
}

//--------------------------------------------------------------------------------
void sense_notify_door_skip(Boolean do_skip)
{
  if(sense_notify_door_skip_flag != do_skip)
  {
    sense_notify_door_skip_flag = do_skip;
    cmd_q_insert(do_skip?INPUT_DOOR_SKIP_SET:INPUT_DOOR_SKIP_CLR,COMMAND_INSERT_TIMEOUT);
      
    #if DBG_SENSE == 1
      sense_notify_dbg_print("\nsense_notify: door skip changed to " , &do_skip , 1);
    #endif
  }
}

//--------------------------------------------------------------------------------
void sense_notify_brake_skip(Boolean do_skip)
{
  if(sense_notify_brake_skip_flag != do_skip)
  {
    sense_notify_brake_skip_flag = do_skip;
    cmd_q_insert(do_skip?INPUT_BRAKE_SKIP_SET:INPUT_BRAKE_SKIP_CLR,COMMAND_INSERT_TIMEOUT);
      
    #if DBG_SENSE == 1
      sense_notify_dbg_print("\nsense_notify: brake skip changed to " , &do_skip , 1);
    #endif
  }
}

//--------------------------------------------------------------------------------
void sense_notify_ignition_skip(Boolean do_skip)
{
  if(sense_notify_ignition_skip_flag != do_skip)
  {
    sense_notify_ignition_skip_flag = do_skip;
    cmd_q_insert(do_skip?INPUT_IGNITION_SKIP_SET:INPUT_IGNITION_SKIP_CLR,COMMAND_INSERT_TIMEOUT);
      
    #if DBG_SENSE == 1
      sense_notify_dbg_print("\nsense_notify: ign skip changed to " , &do_skip , 1);
    #endif
  }
}
