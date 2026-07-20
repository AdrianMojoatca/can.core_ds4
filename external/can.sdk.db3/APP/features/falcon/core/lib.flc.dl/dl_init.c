/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: out_create.c 1046 2010-04-08 12:51:11Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dl_private.h"
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

//--------------------------------------------------------------------------//

void dl_init(void)
{
   cmd_register:
   {
       static const Cmd_Array1 cmd_array[] =
       {            
            {INPUT_LOCK1         | INPUT_ON  , (Exec_Func)dl_lock               },
            {INPUT_LOCK2         | INPUT_ON  , (Exec_Func)dl_lock               },
            {INPUT_LOCK3         | INPUT_ON  , (Exec_Func)dl_lock               },
            
            {INPUT_UNLOCK_DRIVER1| INPUT_ON  , (Exec_Func)dl_unlock             },
    	    {INPUT_UNLOCK_DRIVER2| INPUT_ON  , (Exec_Func)dl_unlock             },
            {INPUT_UNLOCK_DRIVER3| INPUT_ON  , (Exec_Func)dl_unlock             },
            
			{INPUT_UNLOCK_ALL1   | INPUT_ON  , (Exec_Func)dl_unlock_all         },                                    
			{INPUT_UNLOCK_ALL2   | INPUT_ON  , (Exec_Func)dl_unlock_all         },                                 
			{INPUT_UNLOCK_ALL3   | INPUT_ON  , (Exec_Func)dl_unlock_all         },
            {INPUT_IGN_STATUS_OFF            , (Exec_Func)dl_do_unlock_after_dis},
            {INPUT_GWR_OFF                   , (Exec_Func)dl_do_relock          },
            
            {INPUT_OEM_TRUNK                 , (Exec_Func)dl_tr_trigger_oem     },
            {INPUT_TRUNK         | INPUT_ON  , (Exec_Func)dl_tr_trigger_rs      },
    	    {INPUT_OEM_LOCK                  , (Exec_Func)dl_tr_reset_lock      },
    	    {INPUT_OEM_UNLOCK                , (Exec_Func)dl_tr_reset_unlock    },            
            {INPUT_LOCK1         | INPUT_ON  , (Exec_Func)dl_tr_reset_lock      },
            {INPUT_UNLOCK_DRIVER1| INPUT_ON  , (Exec_Func)dl_tr_reset_unlock    },
    	    {INPUT_UNLOCK_ALL1   | INPUT_ON  , (Exec_Func)dl_tr_reset_unlock    },            
            {INPUT_LOCK2         | INPUT_ON  , (Exec_Func)dl_tr_reset_lock      },
            {INPUT_UNLOCK_DRIVER2| INPUT_ON  , (Exec_Func)dl_tr_reset_unlock    },
    	    {INPUT_UNLOCK_ALL2   | INPUT_ON  , (Exec_Func)dl_tr_reset_unlock    },            
            {INPUT_LOCK3         | INPUT_ON  , (Exec_Func)dl_tr_reset_lock      },
            {INPUT_UNLOCK_DRIVER3| INPUT_ON  , (Exec_Func)dl_tr_reset_unlock    },
    	    {INPUT_UNLOCK_ALL3   | INPUT_ON  , (Exec_Func)dl_tr_reset_unlock    },            
            {INPUT_ALL_DOOR_STATUS_OPEN      , (Exec_Func)dl_tr_reset_doors     },              
            {INPUT_TRUNK_STATUS_CLOSE        , (Exec_Func)dl_tr_timeout         },                
            {INPUT_TRIGGER_OFF               , (Exec_Func)dl_tr_reset           },
            {INPUT_PANIC                     , (Exec_Func)dl_tr_reset           }            
       };

       CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
   }
   
   memset(&dl,0,sizeof(struct s_Dl));
}

//--------------------------------------------------------------------------//

void dl_cfg(DL_CFG cfg , void *param)
{
    switch(cfg)
    {
        case DL_OEM_DISARM_WITH_TRANSPONDER:dl.unlock_after_disarm_enable =       !!(param);break;
        case DL_DRIVER_DOOR_PRIORITY       :dl.unlock_dd_enable           =       !!(param);break;
        case DL_EVENT_CALLBACK             :dl.evt                        = (Dl_Evt)(param);break;
#if LIB_DL_DISABLE_ANALOG == 0
        case DL_OUT_CALLBACK               :dl.out                        = (Dl_Out)(param);break;
#endif
        
        default:
            break;
    }
}

//--------------------------------------------------------------------------//            

#if LIB_DL_DISABLE_ANALOG == 0
void dl_ctrl_out(DL_OUT out)
{
	timeout_q_remove((void *)dl_oem_disable_expired);
    
    ATOMIC(dl.oem_disable = TRUE);
    
    timeout_q_insert((Timeout_Q_Func)dl_oem_disable_expired,NULL,300, 1 , 1);
                                                                                    
	dl.out(out);
}
#endif

//--------------------------------------------------------------------------//

Timeout_Q_Status dl_oem_disable_expired( void )
{
    dl.oem_disable = FALSE ;
    
    TRACE("DL_OEM_DISABLE_EXPIRED]\n\r");
    
    return TIMEOUT_Q_COMPLETE;
}

//--------------------------------------------------------------------------//
