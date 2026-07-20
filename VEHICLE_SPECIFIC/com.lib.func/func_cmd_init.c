/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: niss_bp_init.c 1282 2010-07-13 19:33:32Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "func_private.h"
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

/*--------------------------------------------------------------------------*/

void func_cmd_init( void )
{        
    func_light_done = FALSE ;     
    
    cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {    
            { INPUT_LOCK1          | INPUT_ON , (Exec_Func)func_cmd_lock        },
            { INPUT_UNLOCK_ALL1    | INPUT_ON , (Exec_Func)func_cmd_unlock      },
            { INPUT_UNLOCK_DRIVER1 | INPUT_ON , (Exec_Func)func_cmd_unlock      },            
            { INPUT_LOCK2          | INPUT_ON , (Exec_Func)func_cmd_lock        },
            { INPUT_UNLOCK_ALL2    | INPUT_ON , (Exec_Func)func_cmd_unlock      },
            { INPUT_UNLOCK_DRIVER2 | INPUT_ON , (Exec_Func)func_cmd_unlock      },            
            { INPUT_LOCK3          | INPUT_ON , (Exec_Func)func_cmd_lock        },
            { INPUT_UNLOCK_ALL3    | INPUT_ON , (Exec_Func)func_cmd_unlock      },
            { INPUT_UNLOCK_DRIVER3 | INPUT_ON , (Exec_Func)func_cmd_unlock      },            
            { INPUT_TRUNK          | INPUT_ON , (Exec_Func)func_cmd_trunk       },                                        
            { INPUT_AUX1 | INPUT_ON           , (Exec_Func)func_cmd_aux1        },
            { INPUT_AUX2 | INPUT_ON           , (Exec_Func)func_cmd_aux2        },
            { INPUT_AUX3 | INPUT_ON           , (Exec_Func)func_cmd_aux3        },        
            { INPUT_OEM_LOCK                  , (Exec_Func)func_cmd_oem_lock    },
            { INPUT_OEM_UNLOCK                , (Exec_Func)func_cmd_oem_unlock  },
            { INPUT_OEM_TRUNK                 , (Exec_Func)func_cmd_oem_trunk   },                
        	{ INPUT_GWR_OFF                   , (Exec_Func)func_cmd_gwr_off     },             
            { LF_PARKING_LIGHT_ON             , (Exec_Func)func_cmd_light_on    },
            { LF_PARKING_LIGHT_OFF            , (Exec_Func)func_cmd_light_off   },                           
        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
    }
}
                   
/*--------------------------------------------------------------------------*/

