/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_init.c 32362 2015-06-16 18:52:36Z jean-francois.dube $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"
#include "cmd.h"
#include "rf.h"
#include "prog.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define RF_IS_ENA !!(prog_retrieve_option (FEAT_RF_OUTPUT) != NO_RF)

#define RSR_START_DELAY_TIMEOUT 1000
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void rsr_init_opt     (void) ;        
                      
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static const Cmd_Array1 cmd_array[] = 
{    
    {INPUT_VALET_TOGGLE                ,rsr_valet_toogle  },

    {INPUT_GWR              |INPUT_ON  , rsr_gwr_on       },
    {INPUT_IGNITION         | INPUT_ON , rsr_ignition_on  },
    {INPUT_IGNITION                    , rsr_ignition_off },
    {INPUT_START_ONLY       |INPUT_ON  , rsr_start        },
    {INPUT_START            |INPUT_ON  , rsr_start_toggle },
    {INPUT_STOP             |INPUT_ON  , rsr_stop         },
    {INPUT_EXTEND_RUNTIME              , rsr_ext_runtime  },
    {INPUT_REQ_RUNTIME                 , rsr_req_runtime  },

    {INPUT_HOOD_STATUS_OPEN            , rsr_hood_detect  },
    {INPUT_HOOD_STATUS_CLOSE           , rsr_hood_detect  },

    {INPUT_UNLOCK_DRIVER1    | INPUT_ON , rsr_unlock      },   
    {INPUT_UNLOCK_DRIVER2    | INPUT_ON , rsr_unlock      },       
    {INPUT_UNLOCK_DRIVER3    | INPUT_ON , rsr_unlock      },
    {INPUT_UNLOCK_ALL1       | INPUT_ON , rsr_unlock      },   
    {INPUT_UNLOCK_ALL2       | INPUT_ON , rsr_unlock      },       
    {INPUT_UNLOCK_ALL3       | INPUT_ON , rsr_unlock      },  
  
    {INPUT_LOCK1             | INPUT_ON , rsr_lock        },    
    {INPUT_LOCK2             | INPUT_ON , rsr_lock        },
    {INPUT_LOCK3             | INPUT_ON , rsr_lock        },

    {INPUT_TRUNK             | INPUT_ON , rsr_trunk       },

    {INPUT_SECURE_DISARM                , rsr_disarm      },    
    {INPUT_SECURE_ARM                   , rsr_arm         },
};
static const Cmd_Array1 cmd_oem_array[] = 
{
   { INPUT_OEM_LOCK   , rsr_oem_lock  } ,
   { INPUT_OEM_UNLOCK , rsr_oem_unlock} ,
   { INPUT_OEM_TRUNK  , rsr_oem_trunk } ,
   { INPUT_OEM_AUX1   , rsr_oem_aux1  } ,
   { INPUT_OEM_AUX2   , rsr_oem_aux2  } 
};
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void rsr_init (void * arg)           
{
  if(!rsr_init_nvfs())
    {
       // error ?
    }
    rsr_feature_init () ;

    rsr_init_opt () ;  // replace nvfs init_option

    rsr_out_init () ;

    rsr_vars.prk_pattern = RSR_PRK_STOP ;

    rsr_vars.flg_start = os_flg_create (FALSE) ;
    rsr_vars.flg_stop  = os_flg_create (FALSE) ;
        
    rsr_vars.pte       = os_pte_create (RSR_PTE_TIMEOUT) ;

    rsr_vars.fail_timeout       = timeout_f_create(RSR_FAIL_TIMEOUT        , rsr_fail_timeout          ) ;
    rsr_vars.start_delay_timeout= timeout_f_create(RSR_START_DELAY_TIMEOUT , rsr_start_delay_timeout   ) ;

    rf_icon_clr  (ICON_SUCCESS , FALSE) ; // init rf support 
    
    CMD_LUT_INSERT1( cmd_array , EXEC_MODE_NORMAL );

    rsr_input_init();
    
    if (rsr_feature_enabled(RS_FEAT_3X_LOCK_START) && rsr_feature_get(RS_FEAT_3X_LOCK_START))
      {
        rsr_vars.lock_start_timeout = timeout_create ( rsr_feature_get(RS_FEAT_3X_LOCK_START) * 1000 );
        
        CMD_LUT_INSERT1(cmd_oem_array,EXEC_MODE_NORMAL);       
      }

    rsr_vars.tsk = os_tsk_create (rsr_task , TASK_RSR_PRI , TASK_RSR_STK , 0 , TASK_RSR_NAME) ;
    
    os_tsk_start (rsr_vars.tsk , 0) ;
}

//--------------------------------------------------------------------------//

static void rsr_init_opt (void)   
{
  rsr_opt.dtc_check     = RSR_DTC_CHECK ; 
  rsr_opt.trans_check   = RSR_TRANS_CHECK   ;

  rsr_opt.runtime = DEFAULT_RSR_RUNTIME ;
  if (rsr_feature_enabled(RS_FEAT_REMOTE_START_RUNTIME))
    {
      rsr_opt.runtime = rsr_feature_get(RS_FEAT_REMOTE_START_RUNTIME);
    }
  rsr_opt.crank_time    = DEFAULT_RSR_CRANK_TIME    ; 
  if(TACHLESS)
    {
      rsr_opt.crank_time = 400 + (rsr_feature_get(RS_FEAT_ENGINE_CHECK) * 200) ;
    }
  rsr_opt.retry_timeout = DEFAULT_RSR_RETRY_TIMEOUT ; 
  rsr_opt.retry_count   = DEFAULT_RSR_RETRY_COUNTER ; 
  
  rsr_opt.rpm_run       = DEFAULT_RSR_RPM_RUN_VALUE   ; 
  rsr_opt.rpm_crank     = DEFAULT_RSR_RPM_CRANK_VALUE ;
  rsr_opt.rpm_high      = DEFAULT_RSR_RPM_HIGH_VALUE  ; 

  rsr_opt.unlock.pulse  = RSR_UNLOCK_PULSE    ;
  rsr_opt.unlock.dur    = RSR_UNLOCK_DURATION ;    
  rsr_opt.lock.pulse    = RSR_LOCK_PULSE      ;
  rsr_opt.lock.dur      = RSR_LOCK_DURATION   ;  
}


