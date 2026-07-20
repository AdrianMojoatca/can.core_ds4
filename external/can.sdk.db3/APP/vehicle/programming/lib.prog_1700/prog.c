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
  UInt8 feature        ;
  UInt8 option_number  ;
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



#define PROG_DEFAULT_TO_AUX_OPTION(default_index) ((UInt8)((default_index) + 2))

#define DEFAULT_TRUNK_OPTION PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_TRUNK_DEFAULT)
#define DEFAULT_AUX1_OPTION  PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_AUX1_DEFAULT)
#define DEFAULT_AUX2_OPTION  PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_AUX2_DEFAULT)
#define DEFAULT_AUX3_OPTION  PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_AUX3_DEFAULT)
#define DEFAULT_AUX4_OPTION  PROG_DEFAULT_TO_AUX_OPTION(PROG_CFG_FEAT_AUX4_DEFAULT)










/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/



static Prog_Vars prog_vars ;

static Feature_Entry feature_table_default[FEAT_COUNT] ;
static UInt8 FEATURE_NUM = 0 ;

static Feature_Entry feature_table[FEAT_COUNT] ;


static Prog_Error    prog_err ;

static UInt8 prog_aux_option_count (void)
{
  return (UInt8)(PROG_CFG_OPTION_TRUNK_ENABLE
      + PROG_CFG_OPTION_RIGHT_SLIDE_ENABLE
      + PROG_CFG_OPTION_LEFT_SLIDE_ENABLE
      + PROG_CFG_OPTION_TAIL_GLASS_ENABLE
      + PROG_CFG_OPTION_WINDOWS_CLOSURE_ENABLE
      + PROG_CFG_OPTION_HEAD_LIGHTS_ENABLE
      + PROG_CFG_OPTION_SUNROOF_CLOSURE_ENABLE
      + PROG_CFG_OPTION_AUDIO_ENABLE
      + PROG_CFG_OPTION_TIMED_ENABLE
      + PROG_CFG_OPTION_PULSED_ENABLE
      + PROG_CFG_OPTION_GARAGE_DOOR_ENABLE) ;
}

static UInt8 prog_custom1_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CUSTOM1_OPT2_ENABLE
      + (UInt8)PROG_CFG_CUSTOM1_OPT3_ENABLE
      + (UInt8)PROG_CFG_CUSTOM1_OPT4_ENABLE
      + (UInt8)PROG_CFG_CUSTOM1_OPT5_ENABLE) ;
}

static UInt8 prog_custom2_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CUSTOM2_OPT2_ENABLE
      + (UInt8)PROG_CFG_CUSTOM2_OPT3_ENABLE
      + (UInt8)PROG_CFG_CUSTOM2_OPT4_ENABLE
      + (UInt8)PROG_CFG_CUSTOM2_OPT5_ENABLE) ;
}

static UInt8 prog_custom3_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CUSTOM3_OPT2_ENABLE
      + (UInt8)PROG_CFG_CUSTOM3_OPT3_ENABLE
      + (UInt8)PROG_CFG_CUSTOM3_OPT4_ENABLE
      + (UInt8)PROG_CFG_CUSTOM3_OPT5_ENABLE) ;
}

static UInt8 prog_custom4_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CUSTOM4_OPT2_ENABLE
      + (UInt8)PROG_CFG_CUSTOM4_OPT3_ENABLE
      + (UInt8)PROG_CFG_CUSTOM4_OPT4_ENABLE
      + (UInt8)PROG_CFG_CUSTOM4_OPT5_ENABLE) ;
}

static UInt8 prog_heated_seats_level_count (void)
{
  return (UInt8)((UInt8)PROG_CFG_HEATED_SEATS_LEVEL_HIGH_ENABLE
      + (UInt8)PROG_CFG_HEATED_SEATS_LEVEL_MED_ENABLE
      + (UInt8)PROG_CFG_HEATED_SEATS_LEVEL_LOW_ENABLE) ;
}

static UInt8 prog_ctl_doorlock_option_count (void)
{
  return (UInt8)(1
      + (UInt8)PROG_CFG_CTL_DOORLOCK_IGNITION_ENABLE
      + (UInt8)PROG_CFG_CTL_DOORLOCK_BRAKE_ENABLE
      + (UInt8)PROG_CFG_CTL_DOORLOCK_SPEED_ENABLE) ;
}

static UInt8 prog_secure_lock_option_count (void)
{
  return (UInt8)(2 + (UInt8)PROG_CFG_SECURE_LOCK_SMART_ENABLE) ;
}

static void prog_build_feature_table_default (void)
{
  UInt8 feature_num = 0 ;

  #define APPEND_FEATURE(enable, feat, option_count, default_value) \
    do { \
      if ((enable) == 1) { \
        feature_table_default[feature_num].feature = (feat) ; \
        feature_table_default[feature_num].option_number = (option_count) ; \
        feature_table_default[feature_num].option_current = (default_value) ; \
        feature_num++ ; \
      } \
    } while (0)

  APPEND_FEATURE(PROG_CFG_FEAT_RF_OUTPUT_ENABLE, FEAT_RF_OUTPUT, 3, PROG_CFG_FEAT_RF_OUTPUT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_UNLOCK_DRIVER_ENABLE, FEAT_UNLOCK_DRIVER, 2, PROG_CFG_FEAT_UNLOCK_DRIVER_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_RELOCK_ENABLE, FEAT_RELOCK, 2, PROG_CFG_FEAT_RELOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUTO_LOCK_ENABLE, FEAT_AUTO_LOCK, 2, PROG_CFG_FEAT_AUTO_LOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUTO_UNLOCK_ENABLE, FEAT_AUTO_UNLOCK, 2, PROG_CFG_FEAT_AUTO_UNLOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_TRUNK_ENABLE, FEAT_TRUNK, prog_aux_option_count(), DEFAULT_TRUNK_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUX1_ENABLE, FEAT_AUX1, prog_aux_option_count(), DEFAULT_AUX1_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUX2_ENABLE, FEAT_AUX2, prog_aux_option_count(), DEFAULT_AUX2_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUX3_ENABLE, FEAT_AUX3, prog_aux_option_count(), DEFAULT_AUX3_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_AUX4_ENABLE, FEAT_AUX4, prog_aux_option_count(), DEFAULT_AUX4_OPTION) ;
  APPEND_FEATURE(PROG_CFG_FEAT_HORN_CONFIRMATION_ENABLE, FEAT_HORN_CONFIRMATION, 5, PROG_CFG_FEAT_HORN_CONFIRMATION_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_COMFORT_CLOSURE_ENABLE, FEAT_COMFORT_CLOSURE, 3, PROG_CFG_FEAT_COMFORT_CLOSURE_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_OEM_SECURITY_ENABLE, FEAT_OEM_SECURITY, 2, PROG_CFG_FEAT_OEM_SECURITY_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_KEY_IGN_ARM_DEFEAT_ENABLE, FEAT_KEY_IGN_ARM_DEFEAT, 2, PROG_CFG_FEAT_KEY_IGN_ARM_DEFEAT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_LIGHT_CONTROL_ENABLE, FEAT_LIGHT_CONTROL, 2, PROG_CFG_FEAT_LIGHT_CONTROL_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_EXT_LIGHT_SUPERVISION_ENABLE, FEAT_EXT_LIGHT_SUPERVISION, 2, PROG_CFG_FEAT_EXT_LIGHT_SUPERVISION_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_HEATED_SEATS_ENABLE, FEAT_HEATED_SEATS, 2, PROG_CFG_FEAT_HEATED_SEATS_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_DEFROSTER_ENABLE, FEAT_DEFROSTER, 2, PROG_CFG_FEAT_DEFROSTER_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_SEATS_TEMP_CONTROL_ENABLE, FEAT_SEATS_TEMP_CONTROL, 4, PROG_CFG_FEAT_SEATS_TEMP_CONTROL_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_SEATS_TEMP_LEVEL_ENABLE, FEAT_SEATS_TEMP_LEVEL, prog_heated_seats_level_count(), PROG_CFG_FEAT_SEATS_TEMP_LEVEL_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CTL_DOORLOCK_ENABLE, FEAT_CTL_DOORLOCK, prog_ctl_doorlock_option_count(), PROG_CFG_FEAT_CTL_DOORLOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_SECURE_LOCK_ENABLE, FEAT_SECURE_LOCK, prog_secure_lock_option_count(), PROG_CFG_FEAT_SECURE_LOCK_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CUSTOM1_ENABLE, FEAT_CUSTOM1, prog_custom1_option_count(), PROG_CFG_FEAT_CUSTOM1_DEFAULT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CUSTOM2_ENABLE, FEAT_CUSTOM2, prog_custom2_option_count(), PROG_CFG_FEAT_CUSTOM2_DEFAULT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CUSTOM3_ENABLE, FEAT_CUSTOM3, prog_custom3_option_count(), PROG_CFG_FEAT_CUSTOM3_DEFAULT_VALUE) ;
  APPEND_FEATURE(PROG_CFG_FEAT_CUSTOM4_ENABLE, FEAT_CUSTOM4, prog_custom4_option_count(), PROG_CFG_FEAT_CUSTOM4_DEFAULT_VALUE) ;

  #undef APPEND_FEATURE

  FEATURE_NUM = feature_num ;
}

static UInt16 prog_feat_size (void)
{
  return (UInt16)(FEATURE_NUM * sizeof(Feature_Entry)) ;
}




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

  if (FEATURE_NUM == 0)
  {
    return FALSE ;
  }

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
      if(size == prog_feat_size())
        {
          if (nvfs_rd(nvfs_feat_fw , &feature_table) == NVFS_OK)
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

  if(nvfs_wr_create(nvfs_feat_fw, &feature_table_default , prog_feat_size()) == NVFS_OK)
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
      if((nvfs_rd(nvfs_feat_error , &prog_err) == NVFS_OK) && (size == sizeof(cur_err)))
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
  return(prog_init_table() & prog_init_error()) ;
}





//--------------------------------------------------------------------------//


static Boolean prog_feat_table_read (void)
{
  switch (nvfs_rd(nvfs_feat_fw , &feature_table))
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
  core_contract_db3_prog_config_init_defaults() ;
  prog_build_feature_table_default() ;

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


static void prog_sync_runtime_contract_option(Feature_Name feature_name, UInt8 option_nb)
{
  switch (feature_name)
  {
    case FEAT_COMFORT_CLOSURE:
      set_prog_feat_comfort_closure_value(option_nb);
      break;
    case FEAT_CUSTOM1:
      set_prog_feat_custom1_default_value(option_nb);
      break;
    case FEAT_CUSTOM2:
      set_prog_feat_custom2_value(option_nb);
      break;
    case FEAT_CUSTOM3:
      set_prog_feat_custom3_value(option_nb);
      break;
    case FEAT_CUSTOM4:
      set_prog_feat_custom4_value(option_nb);
      break;
    default:
      break;
  }
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

                    prog_sync_runtime_contract_option(feature_name, option_nb);

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
	UInt8 dei_feature;
	UInt8 dei_value;
	UInt8 prog_feature;
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
  { DEI_FEAT_CONTROL_DOOR_LOCK_OPTION, DEI_CTRL_DOOR_LOCK_IGNITION, FEAT_CTL_DOORLOCK, CTL_DOORLOCK_IGNITION },
  { DEI_FEAT_CONTROL_DOOR_LOCK_OPTION, DEI_CTRL_DOOR_LOCK_RPM,      FEAT_CTL_DOORLOCK, CTL_DOORLOCK_SPEED },
	
// do not map FEAT_COMFORT_CLOSURE
// do not map FEAT_CUSTOM1 .. CUSTOM4

	{ 0, 0, 0, 0 }
};

static Boolean prog_has_any_aux_mapping (void)
{
  return (Boolean)(
      (PROG_CFG_OPTION_TRUNK_ENABLE != 0U) ||
      (PROG_CFG_OPTION_RIGHT_SLIDE_ENABLE != 0U) ||
      (PROG_CFG_OPTION_LEFT_SLIDE_ENABLE != 0U) ||
      (PROG_CFG_OPTION_TAIL_GLASS_ENABLE != 0U) ||
      (PROG_CFG_OPTION_WINDOWS_CLOSURE_ENABLE != 0U) ||
      (PROG_CFG_OPTION_HEAD_LIGHTS_ENABLE != 0U) ||
      (PROG_CFG_OPTION_SUNROOF_CLOSURE_ENABLE != 0U) ||
      (PROG_CFG_OPTION_AUDIO_ENABLE != 0U) ||
      (PROG_CFG_OPTION_TIMED_ENABLE != 0U) ||
      (PROG_CFG_OPTION_PULSED_ENABLE != 0U) ||
      (PROG_CFG_OPTION_GARAGE_DOOR_ENABLE != 0U)) ;
}

static Boolean prog_map_aux_icon_to_option (UInt8 dei_icon, UInt8* prog_option)
{
  if ((prog_option == NULL) || (prog_has_any_aux_mapping() == FALSE))
  {
    return FALSE ;
  }

  switch (dei_icon)
  {
    case DEI_AUX_ICON_TRUNK:
      if (PROG_CFG_OPTION_TRUNK_ENABLE != 0U)
      {
        *prog_option = TRUNK_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_RIGHT_DOOR:
      if (PROG_CFG_OPTION_RIGHT_SLIDE_ENABLE != 0U)
      {
        *prog_option = RIGHT_SLIDING_DOOR_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_LEFT_DOOR:
      if (PROG_CFG_OPTION_LEFT_SLIDE_ENABLE != 0U)
      {
        *prog_option = LEFT_SLIDING_DOOR_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_HATCH:
      if (PROG_CFG_OPTION_TAIL_GLASS_ENABLE != 0U)
      {
        *prog_option = TAIL_GLASS_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_WINDOW:
      if (PROG_CFG_OPTION_WINDOWS_CLOSURE_ENABLE != 0U)
      {
        *prog_option = WINDOWS_CLOSURE_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_LIGHTS:
      if (PROG_CFG_OPTION_HEAD_LIGHTS_ENABLE != 0U)
      {
        *prog_option = HEAD_LIGHTS_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_SUNROOF:
      if (PROG_CFG_OPTION_SUNROOF_CLOSURE_ENABLE != 0U)
      {
        *prog_option = SUNROOF_CLOSURE_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_AUDIO:
      if (PROG_CFG_OPTION_AUDIO_ENABLE != 0U)
      {
        *prog_option = AUDIO_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_TIMED:
      if (PROG_CFG_OPTION_TIMED_ENABLE != 0U)
      {
        *prog_option = TIMED_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_PULSED:
      if (PROG_CFG_OPTION_PULSED_ENABLE != 0U)
      {
        *prog_option = PULSED_OPTION ;
        return TRUE ;
      }
      break ;

    case DEI_AUX_ICON_GARAGE_DOOR:
      if (PROG_CFG_OPTION_GARAGE_DOOR_ENABLE != 0U)
      {
        *prog_option = GARAGE_DOOR_OPTION ;
        return TRUE ;
      }
      break ;

    default:
      break ;
  }

  return FALSE ;
}

static const UInt8 aux_dei[]=
{
//  DEI_FEAT_CH2_ICON, -- do not remap
  DEI_FEAT_AUX_1_ICON,
  DEI_FEAT_AUX_2_ICON,
  DEI_FEAT_AUX_3_ICON,
//  DEI_FEAT_AUX_4_ICON
};

static const UInt8 aux_prog[]=
{
//  FEAT_TRUNK,  -- do not remap
  FEAT_AUX1,
  FEAT_AUX2,
  FEAT_AUX3,
 // FEAT_AUX4
};



void my_prog_set(unsigned feat, unsigned value)
{
	prog_set_option(value,feat); // backwards
}


// overwrite based on current dei feature settings
static void apply_dei_mapping(void)
{
	const struct mapping *map;
  UInt8 i;
  UInt8 val;
  UInt8 mapped_option;

	// again overwrite using values from feature dei
	for (map = mapping_table; map->dei_value; map++)
	{
		if (dei_feature_get(map->dei_feature) == map->dei_value)
		{
			my_prog_set(map->prog_feature, map->prog_value);
		}
	}
  if (prog_has_any_aux_mapping() == TRUE)
  {
    for (i = 0; i < sizeof(aux_dei); i++)
    {
      val = dei_feature_get(aux_dei[i]);
      if (prog_map_aux_icon_to_option(val, &mapped_option) == TRUE)
      {
        my_prog_set(aux_prog[i], mapped_option);
      }
    }
  }

  if (PROG_CFG_FEAT_SECURE_LOCK_ENABLE == 1)
  {
      my_prog_set(FEAT_SECURE_LOCK, 1); // 933 will always disable in digital firmware
  }

  if (PROG_CFG_FEAT_RF_OUTPUT_ENABLE == 1)
  {
      my_prog_set(FEAT_RF_OUTPUT, RF_TD2); // 933 will always disable in digital firmware
  }

}



void prog_remap(void)
{
	apply_dei_mapping();
}




