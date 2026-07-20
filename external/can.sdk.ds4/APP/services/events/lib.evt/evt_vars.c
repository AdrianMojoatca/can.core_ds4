/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: evt_vars.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "evt_private.h"
#include "cmd.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

const Evt_Entry evt_table[]   =
{ 

    /*-------------------------------------------------------------------------*/
    /*BRAKE EVENT                                                              */
    /*-------------------------------------------------------------------------*/     
    {  SET_BRAKE_ON                             ,  INPUT_BRAKE_STATUS_PRESS      , 
    (Evt_Guard_F)evt_guard_brake_on             , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_brake_data_status          , (Evt_Set_A  )TRUE             }, 			
 
    {  SET_BRAKE_OFF                            , INPUT_BRAKE_STATUS_RELEASE     ,
    (Evt_Guard_F)evt_guard_brake_off            , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_brake_data_status          , (Evt_Set_A  )FALSE            }, 			
                                    
    /*-------------------------------------------------------------------------*/
    /*PARKING/NEUTRAL EVENT                                                    */
    /*-------------------------------------------------------------------------*/     
    {  SET_PRK_NTRL_ACTIVE                      , INPUT_PRK_NEUTRAL_STATUS_ON    ,
    (Evt_Guard_F)NULL                           , (Evt_Guard_R)0                 , 
    (Evt_Set_F  )NULL                           , (Evt_Set_A  )0                }, 			

    {  SET_PRK_NTRL_NOT_ACTIVE                  , INPUT_PRK_NEUTRAL_STATUS_OFF   ,
    (Evt_Guard_F)NULL                           , (Evt_Guard_R)0                 , 
    (Evt_Set_F  )NULL                           , (Evt_Set_A  )0                }, 			
                                   
    /*-------------------------------------------------------------------------*/
    /*HANDBRAKE EVENT                                                          */
    /*-------------------------------------------------------------------------*/         
    {	SET_HANDBRAKE_ON                        , INPUT_HANDBRAKE_STATUS_ON      ,
    (Evt_Guard_F)evt_guard_handbrake_on         , (Evt_Guard_R)TRUE              ,  
    (Evt_Set_F  )set_handbrake_data_status      , (Evt_Set_A  )TRUE             }, 			
   
    {	SET_HANDBRAKE_OFF                       ,  INPUT_HANDBRAKE_STATUS_OFF    ,
    (Evt_Guard_F)evt_guard_handbrake_off        , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_handbrake_data_status      , (Evt_Set_A  )FALSE            }, 			
  			
    /*-------------------------------------------------------------------------*/
    /*HOOD EVENT                                                               */
    /*-------------------------------------------------------------------------*/             
    {	SET_HOOD_OPEN                           ,  INPUT_HOOD_STATUS_OPEN        ,
    (Evt_Guard_F)evt_guard_hood_open            , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_hood_data_status           , (Evt_Set_A  )TRUE             }, 			
  
    {	SET_HOOD_CLOSE                          ,  INPUT_HOOD_STATUS_CLOSE       ,
    (Evt_Guard_F)evt_guard_hood_close           , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_hood_data_status           , (Evt_Set_A  )FALSE            }, 			
 			                               
    /*-------------------------------------------------------------------------*/
    /*TRUNK EVENT                                                              */
    /*-------------------------------------------------------------------------*/                 
    {	SET_TRUNK_OPEN                          ,  INPUT_TRUNK_STATUS_OPEN       ,
    (Evt_Guard_F)evt_guard_trunk_open           , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_trunk_data_status          , (Evt_Set_A  )TRUE             }, 			
  
    {  SET_TRUNK_CLOSE                          ,  INPUT_TRUNK_STATUS_CLOSE      ,
    (Evt_Guard_F)evt_guard_trunk_close          , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_trunk_data_status          , (Evt_Set_A  )FALSE            }, 			
  			                                   
    /*-------------------------------------------------------------------------*/
    /*IGNITION EVENT                                                           */
    /*-------------------------------------------------------------------------*/                 
    {	SET_IGN_ON                              ,  INPUT_IGN_STATUS_ON           ,
    (Evt_Guard_F)evt_guard_ign_on               , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_ign_data_status            , (Evt_Set_A  )TRUE             }, 			
  
    {	SET_IGN_OFF                             ,  INPUT_IGN_STATUS_OFF          ,
    (Evt_Guard_F)evt_guard_ign_off              , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_ign_data_status            , (Evt_Set_A  )FALSE            }, 			
 
			                              			                               
    /*-------------------------------------------------------------------------*/
    /*OTHER DOOR EVENT                                                         */
    /*-------------------------------------------------------------------------*/                     
    {	SET_OTHER_OPEN                            ,  INPUT_ALL_DOOR_STATUS_OPEN  , //INPUT_OTHER_DOOR_STATUS_OPEN
    (Evt_Guard_F)evt_guard_other_door_open      , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_other_door_data_status     , (Evt_Set_A  )TRUE             }, 			
  
    {	SET_OTHER_CLOSE                           , INPUT_ALL_DOOR_STATUS_CLOSE , //INPUT_OTHER_DOOR_STATUS_CLOSE
    (Evt_Guard_F)evt_guard_other_door_close     , (Evt_Guard_R)TRUE              , 
    (Evt_Set_F  )set_other_door_data_status     , (Evt_Set_A  )FALSE            }, 			
			                               
    
    /*-------------------------------------------------------------------------*/
    /*DRIVER DOOR EVENT                                                        */
    /*-------------------------------------------------------------------------*/                     
		

			                               
    /*-------------------------------------------------------------------------*/
    /*GLOWPLUG EVENT                                                           */
    /*-------------------------------------------------------------------------*/                     
    {	SET_GLOWPLUG_ON                         ,  INPUT_GLOWPLUG_STATUS_ON     ,
    (Evt_Guard_F)evt_guard_glowplug_off_inactive, (Evt_Guard_R)TRUE             , 
    (Evt_Set_F  )set_glowplug_off_data_status   , (Evt_Set_A  )FALSE           }, 			

    {	SET_GLOWPLUG_OFF                        , INPUT_GLOWPLUG_STATUS_OFF     ,
    (Evt_Guard_F)evt_guard_glowplug_off_active  , (Evt_Guard_R)TRUE             , 
    (Evt_Set_F  )set_glowplug_off_data_status   , (Evt_Set_A  )TRUE            }, 			

	/*------------------------------------------------------------------------*/
    /*OEM ALARM TRIGGER EVENT                                                 */
    /*------------------------------------------------------------------------*/                         		                               
    {  FN_ALARM_TRIGGERED                       , INPUT_SENSOR_TRIGGER         ,
    (Evt_Guard_F)NULL                           , (Evt_Guard_R)0               , 
    (Evt_Set_F  )NULL                           , (Evt_Set_A  )0              }, 			
 
 	/*------------------------------------------------------------------------*/
    /*KEY IN EVENT                                                            */
    /*------------------------------------------------------------------------*/                         		                                   		
    {	SET_KEY_IN                              ,  INPUT_KEY_IN_STATUS_ON      ,
    (Evt_Guard_F)NULL                           , (Evt_Guard_R)0               , 
    (Evt_Set_F  )NULL                           , (Evt_Set_A  )0              }, 			

    {	SET_KEY_OUT                             ,  INPUT_KEY_IN_STATUS_OFF     ,
    (Evt_Guard_F)NULL                           , (Evt_Guard_R)0               , 
    (Evt_Set_F  )NULL                           , (Evt_Set_A  )0              }, 			
    
    /*------------------------------------------------------------------------*/
    /*OEM LOCK EVENT                                                          */
    /*------------------------------------------------------------------------*/                         		                                   		
    {  SET_KEYLESS_LOCK                        ,  INPUT_OEM_LOCK               ,
    (Evt_Guard_F)get_rke_lock_use              , (Evt_Guard_R)RKE_ENABLE       , 
    (Evt_Set_F  )NULL                          , (Evt_Set_A  )0               }, 			
 			 
    /*------------------------------------------------------------------------*/
    /*OEM UNLOCK EVENT                                                        */
    /*------------------------------------------------------------------------*/                         		                                   		    
    {  SET_KEYLESS_UNLOCK                      ,  INPUT_OEM_UNLOCK             ,
    (Evt_Guard_F)get_rke_unlock_use            , (Evt_Guard_R)RKE_ENABLE       , 
    (Evt_Set_F  )NULL                          , (Evt_Set_A  )0               }, 
   
   	/*------------------------------------------------------------------------*/
    /*OEM ARM  EVENT                                                          */
    /*------------------------------------------------------------------------*/                         		                                   		        					 
    {  SET_KEYLESS_ARM                         ,  INPUT_OEM_ARM                , 
    (Evt_Guard_F)get_rke_lock_use              , (Evt_Guard_R)RKE_ENABLE       , 
    (Evt_Set_F  )NULL                          , (Evt_Set_A  )0               }, 			
 
    /*------------------------------------------------------------------------*/
    /*OEM UNLOCK EVENT                                                        */
    /*------------------------------------------------------------------------*/                         		                                   		        					     
    {  SET_KEYLESS_DISARM                      ,  INPUT_INVALID               , // DBALL is not yet using OEM_DISARM in the right sense so till it is sorted mapping to INPUT_INVALID
    (Evt_Guard_F)get_rke_unlock_use          , (Evt_Guard_R)RKE_ENABLE         , 
    (Evt_Set_F  )NULL                        , (Evt_Set_A  )0                 }, 
    
    /*------------------------------------------------------------------------*/
    /*OEM TRUNK RELEASE EVENT                                                 */
    /*------------------------------------------------------------------------*/                         		                                   		        					         
    {  SET_KEYLESS_TRUNK                       ,  INPUT_OEM_TRUNK              ,
    (Evt_Guard_F)get_rke_trunk_use           , (Evt_Guard_R)RKE_ENABLE         , 
    (Evt_Set_F  )NULL                        , (Evt_Set_A  )0                 }, 
   
    
    /*------------------------------------------------------------------------*/
    /*OEM PANIC EVENT                                                         */
    /*------------------------------------------------------------------------*/                         		                                   		        					             
    {  SET_KEYLESS_PANIC                       , INPUT_OEM_PANIC              ,
    (Evt_Guard_F)NULL                        , (Evt_Guard_R)0                 , 
    (Evt_Set_F  )NULL                        , (Evt_Set_A  )0                }, 			
                                                                       
    /*------------------------------------------------------------------------*/
    /*OEM AUX1 EVENT                                                          */
    /*------------------------------------------------------------------------*/                         		                                   		        					                 
    {  SET_KEYLESS_AUX1                        ,  INPUT_OEM_AUX1              ,
    (Evt_Guard_F)NULL                        , (Evt_Guard_R)0                 , 
    (Evt_Set_F  )NULL                        , (Evt_Set_A  )0                }, 			

    /*------------------------------------------------------------------------*/
    /*OEM AUX2 EVENT                                                          */
    /*------------------------------------------------------------------------*/                         		                                   		        					                     
    {  SET_KEYLESS_AUX2                        ,  INPUT_OEM_AUX2              ,
    (Evt_Guard_F)NULL                        , (Evt_Guard_R)0                 , 
    (Evt_Set_F  )NULL                        , (Evt_Set_A  )0                }, 			

    /*------------------------------------------------------------------------*/
    /*OEM AUX3 EVENT                                                          */
    /*------------------------------------------------------------------------*/                         		                                   		        					                     
    {  SET_KEYLESS_AUX3                        , INPUT_OEM_AUX3  			  ,
    (Evt_Guard_F)NULL                        , (Evt_Guard_R)0                 , 
    (Evt_Set_F  )NULL                        , (Evt_Set_A  )0                }, 			

    /*------------------------------------------------------------------------*/
    /*OEM AUX4 EVENT                                                          */
    /*------------------------------------------------------------------------*/                         		                                   		        					                     
    {  SET_KEYLESS_AUX4                        ,  INPUT_OEM_AUX4           	  ,
    (Evt_Guard_F)NULL                        , (Evt_Guard_R)0                 , 
    (Evt_Set_F  )NULL                        , (Evt_Set_A  )0                }, 			

    /*------------------------------------------------------------------------*/
    /*OEM RS START EVENT                                                      */
    /*------------------------------------------------------------------------*/                         		                                   		        					                         
    {  SET_KEYLESS_START                       ,  INPUT_OEM_RS_ON             ,
    (Evt_Guard_F)NULL                        , (Evt_Guard_R)0                 , 
    (Evt_Set_F  )NULL                        , (Evt_Set_A  )0                }, 			
    
    /*------------------------------------------------------------------------*/
    /*OEM RS STOP EVENT                                                       */
    /*------------------------------------------------------------------------*/                         		                                   		        					                             
    {	SET_KEYLESS_STOP                       ,   INPUT_OEM_RS_OFF           ,
    (Evt_Guard_F)NULL                        , (Evt_Guard_R)0                 , 
    (Evt_Set_F  )NULL                        , (Evt_Set_A  )0                }, 			

};

//--------------------------------------------------------------------------//

const UInt8 EVT_TABLE_SIZE     = sizeof_array (evt_table);


/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

