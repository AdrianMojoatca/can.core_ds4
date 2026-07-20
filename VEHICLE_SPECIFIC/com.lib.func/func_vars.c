/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: func_vars.c 4229 2012-10-18 18:45:04Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "func_private.h"
#include "func_cfg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

const Can_Msg func_can_table[] = 
{
	{0x181      , 0 , 0 , 2 , 0 , 0x22 , 0x22                                          } ,	// FUNC__CAN_WRU_RUN_IGN_ON
    {0x391      , 0 , 0 , 3 , 0 , 0x55 , 0x00 , 0x00                                   } ,	// FUNC__CAN_WRU_RUN_IGN_OFF
	{0x181      , 0 , 0 , 2 , 0 , 0x00 , 0x00                                          } ,	// FUNC__CAN_WRU_STOP_IGN_ON
    {0x391      , 0 , 0 , 3 , 0 , 0x00 , 0x00 , 0x00                                   } ,	// FUNC__CAN_WRU_STOP_IGN_OFF
	
    {0x181      , 0 , 0 , 2 , 0 , 0x44 , 0x00                                          } ,	// FUNC__CAN_WRD_FRONT_RUN_IGN_ON
    {0x391      , 0 , 0 , 3 , 0 , 0x0A , 0x00 , 0x00                                   } ,	// FUNC__CAN_WRD_FRONT_RUN_IGN_OFF
	
    {0x181      , 0 , 0 , 2 , 0 , 0x00 , 0x44                                          } ,	// FUNC__CAN_WRD_REAR_RUN_IGN_ON
    {0x391      , 0 , 0 , 3 , 0 , 0xA0 , 0x00 , 0x00                                   } ,	// FUNC__CAN_WRD_REAR_RUN_IGN_OFF
	    
    {0x181      , 0 , 0 , 2 , 0 , 0x00 , 0x00                                          } ,	// FUNC__CAN_WRD_STOP_IGN_ON
    {0x391      , 0 , 0 , 3 , 0 , 0x00 , 0x00 , 0x00                                   } ,	// FUNC__CAN_WRD_STOP_IGN_OFF
	
    {0x395      , 0 , 0 , 1 , 0 , 0x81                                                 } ,	// FUNC__CAN_LIGHT_ON 
    {0x395      , 0 , 0 , 1 , 0 , 0x00                                                 } ,	// FUNC__CAN_LIGHT_OFF
    
    {0x291      , 0 , 0 , 3 , 0 , 0x00 , 0x55 , 0x00                                   } ,	// FUNC__CAN_LOCK_CL_ERROR
    {0x291      , 0 , 0 , 3 , 0 , 0x00 , 0xAA , 0x00                                   } ,	// FUNC__CAN_UNLOCK_CL_ERROR

    {0x271      , 0 , 0 , 2 , 0 , 0x07 , 0x00                                          } ,	// FUNC__CAN_DISARM
	        
    {0x000      , 0 , 0 , 8 , 0 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00} 	// FUNC__CAN_WAKE
};

/*--------------------------------------------------------------------------*/

const Process_Cfg   func_prc_cfg[ FUNC_PRC_COUNT ] =
{
        
    { "ARM"   , ((UInt32)1<<FUNC_ARM)             , 
    
         FUNC_ARM_BLOCKED_BY                      , 
         FUNC_ARM_SUSPENDED_BY                    , 
        
        (Process_Exec)func_arm_start              , 
        (Process_Exec)func_arm_stop               ,
        (Process_Exec)func_arm_exec               ,
        
        FUNC_ARM_TIMEOUT
    },

    
    { "DISARM"   , ((UInt32)1<<FUNC_DISARM)       , 
                                                  
         FUNC_DISARM_BLOCKED_BY                   , 
         FUNC_DISARM_SUSPENDED_BY                 , 
        
        (Process_Exec)func_disarm_start           , 
        (Process_Exec)func_disarm_stop            ,
        (Process_Exec)func_disarm_exec            ,
        
        FUNC_DISARM_TIMEOUT
    },

    
    { "LOCK"   , ((UInt32)1<<FUNC_LOCK)           , 
    
         FUNC_LOCK_BLOCKED_BY                     , 
         FUNC_LOCK_SUSPENDED_BY                   , 
        
        (Process_Exec)func_lock_start             , 
        (Process_Exec)func_lock_stop              ,
        (Process_Exec)func_lock_exec              ,
        
        FUNC_LOCK_TIMEOUT
    },


    { "UNLOCK"   , ((UInt32)1<<FUNC_UNLOCK)       , 
                                                        
         FUNC_UNLOCK_BLOCKED_BY                   , 
         FUNC_UNLOCK_SUSPENDED_BY                 , 
        
        (Process_Exec)func_unlock_all_start       , 
        (Process_Exec)func_unlock_all_stop        ,
        (Process_Exec)func_unlock_all_exec        ,
        
        FUNC_UNLOCK_TIMEOUT
    },


    { "TRUNK_RELEASE"   , ((UInt32)1<<FUNC_TRUNK_RELEASE), 
                                                        
         FUNC_TRUNK_RELEASE_BLOCKED_BY              , 
         FUNC_TRUNK_RELEASE_SUSPENDED_BY            , 
        
        (Process_Exec)func_trunk_release_start      , 
        (Process_Exec)func_trunk_release_stop       ,
        (Process_Exec)func_trunk_release_exec       ,
        
        FUNC_TRUNK_RELEASE_TIMEOUT
    },


    { "TRUNK_RELEASE_RELOCK"   , ((UInt32)1<<FUNC_RELOCK_TRUNK_RELEASE), 
                                                        
         FUNC_RELOCK_TR_BLOCKED_BY                 , 
         FUNC_RELOCK_TR_SUSPENDED_BY               , 
        
        (Process_Exec)func_lock_start              , 
        (Process_Exec)func_lock_stop               ,
        (Process_Exec)func_lock_exec               ,
        
        FUNC_RELOCK_TR_TIMEOUT
    },


    { "OEM_LOCK"   , ((UInt32)1<<FUNC_OEM_LOCK)    , 
                                                        
         FUNC_OEM_LOCK_BLOCKED_BY                  , 
         FUNC_OEM_LOCK_SUSPENDED_BY                , 
        
        (Process_Exec)func_oem_lock_start          , 
        (Process_Exec)func_oem_lock_stop           ,
        (Process_Exec)func_oem_lock_exec           ,
        
        FUNC_OEM_LOCK_TIMEOUT
    },


    { "OEM_UNLOCK"   , ((UInt32)1<<FUNC_OEM_UNLOCK), 
                                                        
         FUNC_OEM_UNLOCK_BLOCKED_BY                , 
         FUNC_OEM_UNLOCK_SUSPENDED_BY              , 
        
        (Process_Exec)func_oem_unlock_start        , 
        (Process_Exec)func_oem_unlock_stop         ,
        (Process_Exec)func_oem_unlock_exec         ,
        
        FUNC_OEM_UNLOCK_TIMEOUT
    },


    { "OEM_TRUNK"   , ((UInt32)1<<FUNC_OEM_TRUNK)  , 
                                                        
         FUNC_OEM_TRUNK_BLOCKED_BY                 , 
         FUNC_OEM_TRUNK_SUSPENDED_BY               , 
        
        (Process_Exec)func_oem_trunk_start         , 
        (Process_Exec)func_oem_trunk_stop          ,
        (Process_Exec)func_oem_trunk_exec          ,
        
        FUNC_OEM_TRUNK_TIMEOUT
    },


    /*{ "WRU"   , ((UInt32)1<<FUNC_WRU)              , 
                                                        
         FUNC_WRU_BLOCKED_BY                       , 
         FUNC_WRU_SUSPENDED_BY                     , 
        
        (Process_Exec)func_wru_start               , 
        (Process_Exec)func_wru_stop                ,
        (Process_Exec)func_wru_exec                ,
        
        FUNC_WRU_TIMEOUT
    },


    { "WRD_FRONT"   , ((UInt32)1<<FUNC_WRD_FRONT)   , 
                                                        
         FUNC_WRD_FRONT_BLOCKED_BY                  , 
         FUNC_WRD_FRONT_SUSPENDED_BY                , 
        
        (Process_Exec)func_wrd_front_start          , 
        (Process_Exec)func_wrd_front_stop           ,
        (Process_Exec)func_wrd_front_exec           ,
        
        FUNC_WRD_FRONT_TIMEOUT
    },


    { "WRD_REAR"   , ((UInt32)1<<FUNC_WRD_REAR)     , 
                                                        
         FUNC_WRD_REAR_BLOCKED_BY                   , 
         FUNC_WRD_REAR_SUSPENDED_BY                 , 
        
        (Process_Exec)func_wrd_rear_start           , 
        (Process_Exec)func_wrd_rear_stop            ,
        (Process_Exec)func_wrd_rear_exec            ,
        
        FUNC_WRD_REAR_TIMEOUT
    },


    { "GAS_CAP"   , ((UInt32)1<<FUNC_GAS_CAP)       , 
                                                        
         FUNC_GAS_CAP_BLOCKED_BY                    , 
         FUNC_GAS_CAP_SUSPENDED_BY                  , 
        
        (Process_Exec)func_gas_start                , 
        (Process_Exec)func_gas_stop                 ,
        (Process_Exec)func_gas_exec                 ,
        
        FUNC_GAS_CAP_TIMEOUT
    },
	*/
    
    { "LIGHT"   , ((UInt32)1<<FUNC_LIGHT)           , 
                                                        
         FUNC_LIGHT_BLOCKED_BY                      , 
         FUNC_LIGHT_SUSPENDED_BY                    , 
        
        (Process_Exec)func_light_start              , 
        (Process_Exec)func_light_stop               ,
        (Process_Exec)func_light_exec               ,
        
        FUNC_LIGHT_TIMEOUT
    },


    { "LIGHT_DISABLE"   , ((UInt32)1<<FUNC_LIGHT_DISABLE), 
                                                        
         FUNC_LIGHT_DISABLE_BLOCKED_BY                   , 
         FUNC_LIGHT_DISABLE_SUSPENDED_BY                 , 
        
        (Process_Exec)NULL                               , 
        (Process_Exec)NULL                               ,
        (Process_Exec)NULL                               ,
        
        FUNC_LIGHT_DISABLE_TIMEOUT
    }, 
};

/*--------------------------------------------------------------------------*/

Boolean          func_light_done; 

/*--------------------------------------------------------------------------*/

Func_Cfg*        func_cfg ;

/*--------------------------------------------------------------------------*/

Process_Node*    func_prc[FUNC_PRC_COUNT];

/*--------------------------------------------------------------------------*/

// fake counter for lock/unlock ar/disarm
Boolean is_arm_lock;
Boolean is_disarm_unlock;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

