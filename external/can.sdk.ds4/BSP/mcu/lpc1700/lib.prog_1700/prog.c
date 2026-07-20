/* prog.c */


/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/


#include "device.h"  
#include "nvfs_usr.h"
#include "prog.h"
#include "dbg.h"

#include "string.h"
#include "reset.h"
#include "os.h"



/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


#if DEBUG_PROG == 1
#define TRACE_DEI_PROG          dbg_tx_text
#else
#define TRACE_DEI_PROG(...)     ;
#endif


#define PROG_VERSION        2   // version of prog for XKEY compatibility

#define PROG_ERR_BUFF_SIZE  5  




//--------------------------------------------------------------------------//

typedef enum
{
    DEFAULT                      ,
    ENTER_LEAVE_EVENT            ,
    CHANGE_FEATURE_EVENT         ,
    CHANGE_OPTION_EVENT          ,
    PREPARE_CHANGE_FEATURE_EVENT

} Prog_Event ;

//--------------------------------------------------------------------------//

typedef struct s_Prog_Vars
{
    TskTimeout timeout           ;
    UInt8      feature_current   ;
    Boolean    do_change_feature ;
    Boolean    prog_enabled      ;
    Boolean    prog_entered      ;

    Prog_Func  exit_func         ;

} Prog_Vars ;

//--------------------------------------------------------------------------//

typedef struct s_Feature_Entry
{
    const UInt8 feature        ;
    const UInt8 option_number  ;
          UInt8 option_current ;
} Feature_Entry ;

//--------------------------------------------------------------------------//

typedef enum
{
   NO_PROG_ERR        ,
   ERR_PROG_READ      ,
   ERR_PROG_WRITE     ,
   ERR_PROG_CRC       ,
   
   
   ERR_PROG_TABLE_SIZE  ,
   ERR_PROG_TABLE_READ  ,
   ERR_PROG_TABLE_MATCH ,   
   

   ERR_PROG_COUNT 

}Prog_Error_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Prog_Err
{

  UInt16         count                    ;

  Prog_Error_Code code [PROG_ERR_BUFF_SIZE] ;

}Prog_Error ;

//--------------------------------------------------------------------------//



// COMPILE TIME CREATION OF DEFINE FOR TRUNK DEFAULT OPTION
#if FEAT_TRUNK_DEFAULT == 0
#define DEFAULT_TRUNK_OPTION	TRUNK_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 1
#define DEFAULT_TRUNK_OPTION	RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 2
#define DEFAULT_TRUNK_OPTION	LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 3
#define DEFAULT_TRUNK_OPTION	TAIL_GLASS_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 4
#define DEFAULT_TRUNK_OPTION	GAS_CAP_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 5
#define DEFAULT_TRUNK_OPTION	HEAD_LIGHTS_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 6
#define DEFAULT_TRUNK_OPTION	SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 7
#define DEFAULT_TRUNK_OPTION	WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 8
#define DEFAULT_TRUNK_OPTION	WINDOWS_VENT_OPTION
#endif

// COMPILE TIME CREATION OF DEFINE FOR AUX1 DEFAULT OPTION
#if FEAT_AUX1_DEFAULT == 0
#define DEFAULT_AUX1_OPTION		TRUNK_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 1
#define DEFAULT_AUX1_OPTION		RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 2
#define DEFAULT_AUX1_OPTION		LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 3
#define DEFAULT_AUX1_OPTION		TAIL_GLASS_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 4
#define DEFAULT_AUX1_OPTION		GAS_CAP_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 5
#define DEFAULT_AUX1_OPTION		HEAD_LIGHTS_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 6
#define DEFAULT_AUX1_OPTION		SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 7
#define DEFAULT_AUX1_OPTION		WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 8
#define DEFAULT_AUX1_OPTION		WINDOWS_VENT_OPTION
#endif

// COMPILE TIME CREATION OF DEFINE FOR AUX2 DEFAULT OPTION
#if FEAT_AUX2_DEFAULT == 0
#define DEFAULT_AUX2_OPTION		TRUNK_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 1
#define DEFAULT_AUX2_OPTION		RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 2
#define DEFAULT_AUX2_OPTION		LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 3
#define DEFAULT_AUX2_OPTION		TAIL_GLASS_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 4
#define DEFAULT_AUX2_OPTION		GAS_CAP_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 5
#define DEFAULT_AUX2_OPTION		HEAD_LIGHTS_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 6
#define DEFAULT_AUX2_OPTION		SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 7
#define DEFAULT_AUX2_OPTION		WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 8
#define DEFAULT_AUX2_OPTION		WINDOWS_VENT_OPTION
#endif

// COMPILE TIME CREATION OF DEFINE FOR AUX3 DEFAULT OPTION
#if FEAT_AUX3_DEFAULT == 0
#define DEFAULT_AUX3_OPTION		TRUNK_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 1
#define DEFAULT_AUX3_OPTION		RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 2
#define DEFAULT_AUX3_OPTION		LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 3
#define DEFAULT_AUX3_OPTION		TAIL_GLASS_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 4
#define DEFAULT_AUX3_OPTION		GAS_CAP_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 5
#define DEFAULT_AUX3_OPTION		HEAD_LIGHTS_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 6
#define DEFAULT_AUX3_OPTION		SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 7
#define DEFAULT_AUX3_OPTION		WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 8
#define DEFAULT_AUX3_OPTION		WINDOWS_VENT_OPTION
#endif

// COMPILE TIME CREATION OF DEFINE FOR AUX4 DEFAULT OPTION
#if FEAT_AUX4_DEFAULT == 0
#define DEFAULT_AUX4_OPTION		TRUNK_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 1
#define DEFAULT_AUX4_OPTION		RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 2
#define DEFAULT_AUX4_OPTION		LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 3
#define DEFAULT_AUX4_OPTION		TAIL_GLASS_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 4
#define DEFAULT_AUX4_OPTION		GAS_CAP_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 5
#define DEFAULT_AUX4_OPTION		HEAD_LIGHTS_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 6
#define DEFAULT_AUX4_OPTION		SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 7
#define DEFAULT_AUX4_OPTION		WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 8
#define DEFAULT_AUX4_OPTION		WINDOWS_VENT_OPTION
#endif










/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/



static Prog_Vars prog_vars ;

static const Feature_Entry feature_table_default[] =
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

#define FEATURE_NUM sizeof_array (feature_table_default)

static Feature_Entry feature_table[FEATURE_NUM] ;


static Prog_Error    prog_err ;

#define PROG_FEAT_SIZE      FEATURE_NUM * sizeof (Feature_Entry)   




#if DBG_PROG == 1
  
  #warning "DBG_PROG is Enable"
  
#endif

///*==========================================================================*/
///*                  F U N C T I O N   P R O T O T Y P E S                   */
///*==========================================================================*/
                
//void    prog_task                (void * tsk_arg)          ;
//void    prog_wake                (Wake_Status wake_status) ;

//void    prog_init_reset          (void) ;
//void    prog_reset               (void) ;

//void    prog_input_ign_on        (void * arg) ;
//void    prog_input_ign_off       (void * arg) ;
//void    prog_input_long_push     (void * arg) ;
//void    prog_input_push_press    (void * arg) ;
//void    prog_input_push_release  (void * arg) ;
//void    prog_input_change_option (void * arg) ;

//Boolean prog_init_nvfs           (void) ;
//Boolean prog_feat_table_read     (void) ;
//Boolean prog_feat_table_write    (void) ;

//void    prog_err_add             (Prog_Error_Code code) ;
//Boolean prog_err_save            (void)                 ; 

//extern Tsk           tsk_prog           ;
//extern Pip           pip_prog           ;
//extern Prog_Vars     prog_vars          ;
//extern Timeout       prog_ign_timeout   ;
//extern Feature_Entry feature_table   [] ;
//extern UInt8         xkey_feature    [] ;
//extern Prog_Error    prog_err           ;
//extern const UInt8   FEATURE_NUM        ;











//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
static Boolean prog_feat_table_content_match (void)
{     
  UInt8 lfeat = 0 ;
  UInt8 dfeat = 0 ;

  do
  {   
    if(feature_table[lfeat].feature == feature_table_default[dfeat].feature)
      {
        if(feature_table[lfeat].option_number == feature_table_default[dfeat].option_number)
          {
            lfeat++ ;
            if(lfeat == FEATURE_NUM)
              {
                return TRUE ;
              }
            dfeat = 0 ;
          }
        else
          {
            return FALSE ;
          }
      }
    else
      {
        dfeat++ ;
      }
  }
  while(dfeat < FEATURE_NUM) ;
  
  return FALSE ;  
}



static Boolean prog_err_save (void)           
{ 
  return nvfs_wr(nvfs_feat_error, &prog_err) == NVFS_OK ;  
}

static void prog_err_add (Prog_Error_Code code)
{  
  UInt8 ptr = prog_err.count %PROG_ERR_BUFF_SIZE ;
 
  prog_err.code[ptr] = code ;

  prog_err.count++ ;
  
  prog_err_save() ;
}




static Boolean prog_init_table (void)
{
  UInt16 size = 0 ;

  if(nvfs_rd_size(nvfs_feat_fw , &size) == NVFS_OK)
    {
      if(size == PROG_FEAT_SIZE)
        {
          if (nvfs_rd_s(nvfs_feat_fw , feature_table, sizeof(feature_table)) == NVFS_OK) // array
            {            
              if (prog_feat_table_content_match())
                {
                  #if DBG_PROG == 1
                    dbg_tx_text ("\nProg Feat Load") ; 
                  #endif
                  return TRUE ;
                } 
              else  
              {
                  prog_err_add(ERR_PROG_TABLE_MATCH) ;
                  
                  #if DBG_PROG == 1
                    dbg_tx_text ("\nProg Feat Match Fail") ; 
                  #endif 
              }                
            } 
          else  
          {
              prog_err_add(ERR_PROG_TABLE_READ) ;
              
              #if DBG_PROG == 1
                dbg_tx_text ("\nProg Feat Read Fail") ; 
              #endif 
          }            
        }
      else  
      {
          prog_err_add(ERR_PROG_TABLE_SIZE) ;
          
          #if DBG_PROG == 1
            dbg_tx_text ("\nProg Feat Size Fail") ; 
          #endif 
      }
  
    }   
  
  nvfs_delete(nvfs_feat_fw)  ;

  if(nvfs_wr_create(nvfs_feat_fw, &feature_table_default , PROG_FEAT_SIZE) == NVFS_OK)
    {         
      #if DBG_PROG == 1
        dbg_tx_text ("\nProg Feat Create") ;
      #endif
      
      return prog_init_table() ;
    }      
  
  #if DBG_PROG == 1
    dbg_tx_text ("\nProg Feat FAIL") ;
  #endif
  return FALSE ;
}

//--------------------------------------------------------------------------//

static Boolean prog_init_error (void)
{
  static Prog_Error cur_err ;
  
  UInt16 size = 0 ;

  if(nvfs_rd_size(nvfs_feat_error , &size) == NVFS_OK)
  {
      if((size == sizeof(cur_err)) && (nvfs_rd_s(nvfs_feat_error , &prog_err, sizeof(prog_err)) == NVFS_OK)) // NCC-DIRE002-017
      {
          #if DBG_PROG == 1
              dbg_tx_text ("\r\nProg Error Load") ;
          #endif
          return (TRUE) ;
      }
  }

  nvfs_delete(nvfs_feat_error)  ;

  switch (nvfs_wr_create(nvfs_feat_error, &cur_err , sizeof(Prog_Error)))
    {
      case NVFS_OK :  
        {         
           #if DBG_PROG == 1
                dbg_tx_text ("\r\nProg Error Create") ;
           #endif
           return prog_init_error() ;
        }  
    }      


  #if DBG_PROG == 1
    dbg_tx_text ("\r\nProg Error FAIL") ;
  #endif
  return FALSE ;
}



static Boolean prog_init_nvfs (void)
{
  return ( prog_init_error() && prog_init_table() );
}





//--------------------------------------------------------------------------//


static Boolean prog_feat_table_read (void)
{
  switch (nvfs_rd_s(nvfs_feat_fw , feature_table, sizeof(feature_table))) // array
    {
      case NVFS_OK : return TRUE ; 
    }   

    return FALSE ;

}


static Boolean prog_feat_table_write (void)
{
  switch (nvfs_wr(nvfs_feat_fw, &feature_table))
    {
      case NVFS_OK :  
        {          
          return TRUE ;
        }  
    }      

  return FALSE ;

}



static void prog_reset (void)
{
  nvfs_delete(nvfs_feat_fw) ;
}




//--------------------------------------------------------------------------//
// public APIs........


void prog_init_reset (void)
{

  reset_long_register(prog_reset) ;

}


Boolean prog_init(void)
{
  prog_vars.exit_func = NULL ;    
  
  if(!prog_init_nvfs())
    {
      #if DBG_PROG == 1

        dbg_tx_text("\r\nProg init nvfs fail");

      #endif

      return FALSE;
    }

  #if DBG_PROG == 1
  
    dbg_tx_text("\r\nProg init 933");
  
  #endif


  return TRUE;
}



SInt8 prog_retrieve_option (Feature_Name feature_name)
{
	UInt8 i ;

	for (i = 0; i < FEATURE_NUM; i++)
	{
		if (feature_table[i].feature == feature_name)
			return feature_table[i].option_current ;
	}
	return -1 ;
}


Boolean prog_set_option (UInt8 option_nb, Feature_Name feature_name)
{
    UInt8 i ;

#if DBG_PROG == 1
//  StrConst featstring[26][FEAT_COUNT] = 
//  {
//    {"FEAT_UNLOCK_DRIVER        " },
//    {"FEAT_RELOCK               " },
//    {"FEAT_AUTO_LOCK            " },
//    {"FEAT_AUTO_UNLOCK          " },
//    {"FEAT_TRUNK                " },
//    {"FEAT_AUX1                 " },
//    {"FEAT_AUX2                 " },
//    {"FEAT_AUX3                 " },
//    {"FEAT_AUX4                 " },
//    {"FEAT_RF_OUTPUT            " },
//    {"FEAT_HORN_CONFIRMATION    " },
//    {"FEAT_COMFORT_CLOSURE      " },
//    {"FEAT_OEM_SECURITY         " },
//    {"FEAT_KEY_IGN_ARM_DEFEAT   " },
//    {"FEAT_LIGHT_CONTROL        " },
//    {"FEAT_EXT_LIGHT_SUPERVISION" },
//    {"FEAT_HEATED_SEATS         " },
//    {"FEAT_DEFROSTER            " },
//    {"FEAT_SEATS_TEMP_CONTROL   " },
//    {"FEAT_SEATS_TEMP_LEVEL     " },
//    {"FEAT_CTL_DOORLOCK         " },
//    {"FEAT_SECURE_LOCK          " },
//    {"FEAT_CUSTOM1              " },
//    {"FEAT_CUSTOM2              " },
//    {"FEAT_CUSTOM3              " },
//    {"FEAT_CUSTOM4              " },
//  };
//
//  dbg_tx_crlf() ;
//  dbg_tx_text(&featstring[feature_name][0]) ;
//
  dbg_tx_text("\r\nset Option :") ;
  dbg_tx_u   (option_nb , 2)      ;

#endif


    for (i = 0; i < FEATURE_NUM; i++)
    {
        if (feature_table[i].feature == feature_name)
        {
            if(option_nb <= feature_table[i].option_number)
            {
							  if(feature_table[i].option_current != option_nb)
								{
									feature_table[i].option_current = option_nb ;

									prog_feat_table_write () ;
								}

                if(prog_vars.exit_func != NULL)
                {
                    prog_vars.exit_func() ;
                }
                return TRUE ;
            }
        }   
    }   
    return FALSE ;
}



//////////////////// remap //////////////////////////
#include "dei_feature.h"
#include "rf.h"


struct mapping
{
	Dei_Feat dei_feature;
	UInt8 dei_value;
	Feature_Name prog_feature;
	UInt8 prog_value;
};



// this table contains the mapping from dei feature/value to prog feature/value
// we can group them in the conditional based on whether the resulting prog feature exists
static const struct mapping mapping_table[] =
{
  { DEI_FEAT_DRIVER_DOOR_UNLOCK_PRIORITY, DEI_DRIVER_DOOR_UNLOCK_PRIORITY_ON,  FEAT_UNLOCK_DRIVER, UNLOCK_PRIORITY_DRIVER },
  { DEI_FEAT_DRIVER_DOOR_UNLOCK_PRIORITY, DEI_DRIVER_DOOR_UNLOCK_PRIORITY_OFF, FEAT_UNLOCK_DRIVER, UNLOCK_PRIORITY_ALL },

//  { DEI_FEAT_CONTROL_DOOR_LOCK_OPTION,1,FEAT_AUTO_LOCK,1 },
//  { DEI_FEAT_CONTROL_DOOR_LOCK_OPTION,1,FEAT_AUTO_UNLOCK,1 },
//  { DEI_FEAT_CONTROL_DOOR_LOCK_OPTION,2,FEAT_AUTO_UNLOCK,2 },
//  { DEI_FEAT_CONTROL_DOOR_LOCK_OPTION,3,FEAT_AUTO_LOCK,2 },
//  { DEI_FEAT_CONTROL_DOOR_LOCK_FUNC,2,FEAT_AUTO_UNLOCK,1 },
//  { DEI_FEAT_CONTROL_DOOR_LOCK_FUNC,3,FEAT_AUTO_LOCK,1 },

//  { DEI_FEAT_HORN_OUTPUT_TIMING,2,FEAT_HORN_CONFIRMATION,2 },
//  { DEI_FEAT_HORN_OUTPUT_TIMING,3,FEAT_HORN_CONFIRMATION,3 },
//  { DEI_FEAT_HORN_OUTPUT_TIMING,4,FEAT_HORN_CONFIRMATION,4 },
//  { DEI_FEAT_HORN_OUTPUT_TIMING,5,FEAT_HORN_CONFIRMATION,5 },

//  { DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE,1,FEAT_SECURE_LOCK,1 },
//  { DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE,2,FEAT_SECURE_LOCK,2 },
//  { DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE,3,FEAT_SECURE_LOCK,3 },
  { DEI_FEAT_CONTROL_DOOR_LOCK_OPTION, DEI_CTRL_DOOR_LOCK_OFF,      FEAT_CTL_DOORLOCK, CTL_DOORLOCK_DISABLE },
#if CTL_DOORLOCK_IGNITION_ENABLE == 1  
  { DEI_FEAT_CONTROL_DOOR_LOCK_OPTION, DEI_CTRL_DOOR_LOCK_IGNITION, FEAT_CTL_DOORLOCK, CTL_DOORLOCK_IGNITION },
#endif  
#if CTL_DOORLOCK_SPEED_ENABLE == 1  
  { DEI_FEAT_CONTROL_DOOR_LOCK_OPTION, DEI_CTRL_DOOR_LOCK_RPM,      FEAT_CTL_DOORLOCK, CTL_DOORLOCK_SPEED },
#endif 
#if SECURE_LOCK_SMART_ENABLE == 1
  { DEI_FEAT_SECURE_LOCK, DEI_SMART_SECURE_LOCK_ON,      FEAT_SECURE_LOCK, SECURE_LOCK_SMART },  
#endif  
#if SECURE_LOCK_ENABLE == 1
  { DEI_FEAT_SECURE_LOCK, DEI_SECURE_LOCK_ON,      FEAT_SECURE_LOCK, SECURE_LOCK_NORMAL },  
  { DEI_FEAT_SECURE_LOCK, DEI_SECURE_LOCK_OFF,      FEAT_SECURE_LOCK, SECURE_LOCK_DISABLE },  
#endif    
	
// do not map FEAT_COMFORT_CLOSURE
// do not map FEAT_CUSTOM1 .. CUSTOM4

	{ (Dei_Feat)0, 0, (Feature_Name)0, 0 }
};

#define OPTION_SUM ( 0 \
 + OPTION_TRUNK_ENABLE \
 + OPTION_RIGHT_SLIDE_ENABLE \
 + OPTION_LEFT_SLIDE_ENABLE \
 + OPTION_HATCH_ENABLE \
 + OPTION_WINDOWS_CLOSURE_ENABLE \
 + OPTION_HEAD_LIGHTS_ENABLE \
 + OPTION_SUNROOF_ENABLE \
 + OPTION_AUDIO_ENABLE \
 + OPTION_TIMED_ENABLE \
 + OPTION_PULSED_ENABLE \
/* + OPTION_LATCHED_ENABLE */\
 + OPTION_GARAGE_DOOR_ENABLE \
)


#if OPTION_SUM != 0
static const struct mapping auxmap_table[] =
{

#if OPTION_TRUNK_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_TRUNK, FEAT_TRUNK, TRUNK_OPTION },
#endif
#if OPTION_RIGHT_SLIDE_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_RIGHT_DOOR, FEAT_TRUNK, RIGHT_SLIDING_DOOR_OPTION },
#endif
#if OPTION_LEFT_SLIDE_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_LEFT_DOOR, FEAT_TRUNK, LEFT_SLIDING_DOOR_OPTION },
#endif
#if OPTION_HATCH_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_HATCH, FEAT_TRUNK, TAIL_GLASS_OPTION },
#endif
#if OPTION_WINDOWS_CLOSURE_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_WINDOW, FEAT_TRUNK, WINDOWS_CLOSURE_OPTION },
#endif
#if OPTION_HEAD_LIGHTS_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_LIGHTS, FEAT_TRUNK, HEAD_LIGHTS_OPTION },
#endif
#if OPTION_SUNROOF_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_SUNROOF, FEAT_TRUNK, SUNROOF_OPTION },
#endif
#if OPTION_AUDIO_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_AUDIO, FEAT_TRUNK, AUDIO_OPTION },
#endif
#if OPTION_TIMED_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_TIMED, FEAT_TRUNK, TIMED_OPTION },
#endif
#if OPTION_PULSED_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_PULSED, FEAT_TRUNK, PULSED_OPTION },
#endif
//#if OPTION_LATCHED_ENABLE == 1
//  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_LATCHED, FEAT_TRUNK, LATCHED_OPTION },
//#endif
#if OPTION_GARAGE_DOOR_ENABLE == 1
  { DEI_FEAT_AUX_1_ICON, DEI_AUX_ICON_GARAGE_DOOR, FEAT_TRUNK, GARAGE_DOOR_OPTION },
#endif

};

static const UInt8 aux_dei[]=
{
//  DEI_FEAT_CH2_ICON, -- do not remap
  DEI_FEAT_AUX_1_ICON,
  DEI_FEAT_AUX_2_ICON,
  DEI_FEAT_AUX_3_ICON,
//  DEI_FEAT_AUX_4_ICON
};

static const Feature_Name aux_prog[]=
{
//  FEAT_TRUNK,  -- do not remap
  FEAT_AUX1,
  FEAT_AUX2,
  FEAT_AUX3,
 // FEAT_AUX4
};

#endif



void my_prog_set(Feature_Name feat, unsigned value)
{
	prog_set_option(value,feat); // backwards
}


// overwrite based on current dei feature settings
static void apply_dei_mapping(void)
{
	const struct mapping *map;
#if OPTION_SUM != 0	
	unsigned i,val;
#endif	

	// again overwrite using values from feature dei
	for (map = mapping_table; map->dei_value; map++)
	{
		if (dei_feature_get(map->dei_feature) == map->dei_value)
		{
			my_prog_set(map->prog_feature, map->prog_value);
		}
	}
	
#if 0 // TP#7705 - DON'T REMAP AUX CHANNELS     (was OPTION_SUM != 0)	  
	// special case for all AUX channels
	for(i=0; i<sizeof(aux_dei); i++)
        {
                val = dei_feature_get(aux_dei[i]);
                for(map = auxmap_table; map->dei_value; map++)
                {
                    if(val == map->dei_value)
                        my_prog_set(aux_prog[i], map->prog_value);
                } 
        }
#endif				

#if FEAT_SECURE_LOCK_ENABLE           == 1		
        my_prog_set( FEAT_SECURE_LOCK,1); // 933 will always disable in digital firmware
#endif

#if FEAT_RF_OUTPUT_ENABLE             == 1		
        my_prog_set( FEAT_RF_OUTPUT,RF_TD2); // 933 will always disable in digital firmware 
#endif

}



void prog_remap(void)
{
	apply_dei_mapping();
}




