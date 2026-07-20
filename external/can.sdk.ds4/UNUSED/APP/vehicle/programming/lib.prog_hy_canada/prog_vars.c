/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id$
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
#ifdef PLATFORM_CM800
   // Task variables not required
#else

    Tsk tsk_prog ;
    Pip pip_prog ;

    Timeout prog_ign_timeout ;

#endif

Prog_Vars prog_vars ;

  Feature_Entry feature_table_default[] =
  {
  //  {FEATURE                , OPTION NUMBER   , OPTION CURRENT}
      {FEAT_RUNTIME  		  , RUNTIME_NUM - 1        	    , RUNTIME1                     	, TRUE},  
    #if FEAT_RF_OUTPUT_ENABLE             == 1
      {FEAT_RF_OUTPUT         , 3               			, FEAT_RF_OUTPUT_VALUE			, FALSE},    
    #endif                        
    #if FEAT_UNLOCK_DRIVER_ENABLE         == 1        
      {FEAT_UNLOCK_DRIVER     , 2               			, FEAT_UNLOCK_DRIVER_VALUE		, FALSE},  
    #endif                                                                                                                                   
    #if FEAT_TRUNK_ENABLE                 == 1
      {FEAT_TRUNK             , AUX_OPTION_NUM-1			, DEFAULT_TRUNK_OPTION			, FALSE},     
    #endif                                                            
    #if FEAT_AUX1_ENABLE                  == 1                                         
      {FEAT_AUX1              , AUX_OPTION_NUM-1			, DEFAULT_AUX1_OPTION			, FALSE},  
    #endif                                                            
    #if FEAT_AUX2_ENABLE                  == 1                                         
      {FEAT_AUX2              , AUX_OPTION_NUM-1			, DEFAULT_AUX2_OPTION			, FALSE},  
    #endif                                                            
    #if FEAT_AUX3_ENABLE                  == 1                                         
      {FEAT_AUX3              , AUX_OPTION_NUM-1			, DEFAULT_AUX3_OPTION			, FALSE},  
    #endif                                                            
    #if FEAT_AUX4_ENABLE                  == 1             
      {FEAT_AUX4              , AUX_OPTION_NUM-1			, DEFAULT_AUX4_OPTION			, FALSE},
    #endif                       
    #if FEAT_HEATED_SEATS_ENABLE          == 1
      {FEAT_HEATED_SEATS      , 2               			, FEAT_HEATED_SEATS_VALUE		, FALSE},  
    #endif                                  
    #if FEAT_DEFROSTER_ENABLE             == 1
      {FEAT_DEFROSTER         , 2               			, FEAT_DEFROSTER_VALUE			, FALSE}, 
    #endif                                                  
    #if FEAT_SEATS_TEMP_CONTROL_ENABLE    == 1
      {FEAT_SEATS_TEMP_CONTROL, 4               			, FEAT_SEATS_TEMP_CONTROL_VALUE	, FALSE},                                          
    #endif 
    #if FEAT_SEATS_TEMP_LEVEL_ENABLE      == 1
      {FEAT_SEATS_TEMP_LEVEL  , HEATED_SEATS_LEVEL_NUM 		, FEAT_SEATS_TEMP_LEVEL_VALUE	, FALSE},                                          
    #endif                                    
    #if FEAT_CTL_DOORLOCK_ENABLE          == 1
      {FEAT_CTL_DOORLOCK 	  , CTL_DOORLOCK_NUM_PLUS_ONE-1	, FEAT_CTL_DOORLOCK_VALUE		, FALSE},  
    #endif
    #if FEAT_SECURE_LOCK_ENABLE           == 1
      {FEAT_SECURE_LOCK 	  , SECURE_LOCK_NUM_PLUS_ONE-1	, FEAT_SECURE_LOCK_VALUE		, FALSE},  
    #endif                                                                                                                                           
    #if FEAT_CUSTOM2_ENABLE == 1
      {FEAT_CUSTOM2 		  , CUSTOM2_NUM_PLUS_ONE-1		, FEAT_CUSTOM2_DEFAULT_VALUE	, FALSE},  
    #endif                                                                    
    #if FEAT_CUSTOM3_ENABLE == 1
      {FEAT_CUSTOM3 		  , CUSTOM3_NUM_PLUS_ONE-1		, FEAT_CUSTOM3_DEFAULT_VALUE	, FALSE},  
    #endif                                                                        
    #if FEAT_CUSTOM4_ENABLE == 1
      {FEAT_CUSTOM4 		  , CUSTOM4_NUM_PLUS_ONE-1		, FEAT_CUSTOM4_DEFAULT_VALUE	, FALSE},  
    #endif                                                                        

} ;

const UInt8 FEATURE_NUM = sizeof_array (feature_table_default) ;

Feature_Entry feature_table[FEATURE_NUM] ;

Prog_Error    prog_err ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
