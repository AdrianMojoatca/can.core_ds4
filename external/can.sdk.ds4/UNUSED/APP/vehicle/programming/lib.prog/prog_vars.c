/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog_vars.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "prog_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
#if	defined PLATFORM_CM800 || defined PLATFORM_933		//def PLATFORM_CM800
   // Task variables not required
#else

    Tsk tsk_prog ;
    Pip pip_prog ;

    Timeout prog_ign_timeout ;

#endif

Prog_Vars prog_vars ;

  const Feature_Entry feature_table_default[] =
  {
  //  {FEATURE                , OPTION NUMBER   , OPTION CURRENT}
    #if FEAT_RF_OUTPUT_ENABLE             == 1
      {FEAT_RF_OUTPUT         , 3               , FEAT_RF_OUTPUT_VALUE},    
    #endif                        
    #if FEAT_UNLOCK_DRIVER_ENABLE         == 1        
      {FEAT_UNLOCK_DRIVER     , 2               , FEAT_UNLOCK_DRIVER_VALUE},  
    #endif                                                                 
    #if FEAT_RELOCK_ENABLE                == 1   
      {FEAT_RELOCK            , 2               , FEAT_RELOCK_VALUE       },  
    #endif 
    #if FEAT_AUTO_LOCK_ENABLE             == 1
      {FEAT_AUTO_LOCK         , 2               , FEAT_AUTO_LOCK_VALUE}, 
    #endif                          
    #if FEAT_AUTO_UNLOCK_ENABLE           == 1
      {FEAT_AUTO_UNLOCK       , 2               , FEAT_AUTO_UNLOCK_VALUE},     
    #endif                                                                     
    #if FEAT_TRUNK_ENABLE                 == 1
      {FEAT_TRUNK             , AUX_OPTION_NUM-1, DEFAULT_TRUNK_OPTION},     
    #endif                                                            
    #if FEAT_AUX1_ENABLE                  == 1                                         
      {FEAT_AUX1              , AUX_OPTION_NUM-1, DEFAULT_AUX1_OPTION},  
    #endif                                                            
    #if FEAT_AUX2_ENABLE                  == 1                                         
      {FEAT_AUX2              , AUX_OPTION_NUM-1, DEFAULT_AUX2_OPTION},  
    #endif                                                            
    #if FEAT_AUX3_ENABLE                  == 1                                         
      {FEAT_AUX3              , AUX_OPTION_NUM-1, DEFAULT_AUX3_OPTION},  
    #endif                                                            
    #if FEAT_AUX4_ENABLE                  == 1             
      {FEAT_AUX4              , AUX_OPTION_NUM-1, DEFAULT_AUX4_OPTION},
    #endif
    #if FEAT_HORN_CONFIRMATION_ENABLE     == 1    
      {FEAT_HORN_CONFIRMATION , 5               , FEAT_HORN_CONFIRMATION_VALUE},     
    #endif                    
    #if FEAT_COMFORT_CLOSURE_ENABLE       == 1
      {FEAT_COMFORT_CLOSURE   , 3               , FEAT_COMFORT_CLOSURE_VALUE},                                     
    #endif                                                           
    #if FEAT_OEM_SECURITY_ENABLE          == 1
      {FEAT_OEM_SECURITY      , 2               , FEAT_OEM_SECURITY_VALUE},
    #endif                                  
    #if FEAT_KEY_IGN_ARM_DEFEAT_ENABLE    == 1
      {FEAT_KEY_IGN_ARM_DEFEAT, 2               , FEAT_KEY_IGN_ARM_DEFEAT_VALUE},       
    #endif                                    
    #if FEAT_LIGHT_CONTROL_ENABLE         == 1
      {FEAT_LIGHT_CONTROL     , 2               , FEAT_LIGHT_CONTROL_VALUE},       
    #endif                                    
    #if FEAT_EXT_LIGHT_SUPERVISION_ENABLE == 1
      {FEAT_EXT_LIGHT_SUPERVISION, 2            , FEAT_EXT_LIGHT_SUPERVISION_VALUE}, 
    #endif                                    
    #if FEAT_HEATED_SEATS_ENABLE          == 1
      {FEAT_HEATED_SEATS      , 2               , FEAT_HEATED_SEATS_VALUE},  
    #endif                                  
    #if FEAT_DEFROSTER_ENABLE             == 1
      {FEAT_DEFROSTER         , 2               , FEAT_DEFROSTER_VALUE}, 
    #endif                                                  
    #if FEAT_SEATS_TEMP_CONTROL_ENABLE    == 1
      {FEAT_SEATS_TEMP_CONTROL, 4               , FEAT_SEATS_TEMP_CONTROL_VALUE},                                          
    #endif 
    #if FEAT_SEATS_TEMP_LEVEL_ENABLE      == 1
      {FEAT_SEATS_TEMP_LEVEL, HEATED_SEATS_LEVEL_NUM , FEAT_SEATS_TEMP_LEVEL_VALUE},                                          
    #endif                                    
    #if FEAT_CTL_DOORLOCK_ENABLE          == 1
      {FEAT_CTL_DOORLOCK , CTL_DOORLOCK_NUM_PLUS_ONE-1, FEAT_CTL_DOORLOCK_VALUE},  
    #endif
    #if FEAT_SECURE_LOCK_ENABLE           == 1
      {FEAT_SECURE_LOCK , SECURE_LOCK_NUM_PLUS_ONE-1, FEAT_SECURE_LOCK_VALUE},  
    #endif                                                                      
    #if FEAT_CUSTOM1_ENABLE == 1
      {FEAT_CUSTOM1 , CUSTOM1_NUM_PLUS_ONE-1, FEAT_CUSTOM1_DEFAULT_VALUE},  
    #endif                                                                      
    #if FEAT_CUSTOM2_ENABLE == 1
      {FEAT_CUSTOM2 , CUSTOM2_NUM_PLUS_ONE-1, FEAT_CUSTOM2_DEFAULT_VALUE},  
    #endif                                                                    
    #if FEAT_CUSTOM3_ENABLE == 1
      {FEAT_CUSTOM3 , CUSTOM3_NUM_PLUS_ONE-1, FEAT_CUSTOM3_DEFAULT_VALUE},  
    #endif                                                                        
    #if FEAT_CUSTOM4_ENABLE == 1
      {FEAT_CUSTOM4 , CUSTOM4_NUM_PLUS_ONE-1, FEAT_CUSTOM4_DEFAULT_VALUE},  
    #endif                                                                        

} ;

const UInt8 FEATURE_NUM = sizeof_array (feature_table_default) ;

Feature_Entry feature_table[FEATURE_NUM] ;

Prog_Error    prog_err ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
