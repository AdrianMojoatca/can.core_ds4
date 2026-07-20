/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: led_usr_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "led_usr_private.h"
#include "cmd.h"
#include "timeout_f.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static const Cmd_Array1 cmd_array[] = 
{
  {INPUT_GWR               | INPUT_ON , led_usr_gwr_on      },
  {INPUT_GWR                          , led_usr_gwr_off     },
  {INPUT_LOCK1             | INPUT_ON , led_usr_lock        },
  {INPUT_LOCK2             | INPUT_ON , led_usr_lock        },
  {INPUT_LOCK3             | INPUT_ON , led_usr_lock        },
  {INPUT_UNLOCK_DRIVER1    | INPUT_ON , led_usr_unlock      },     
  {INPUT_UNLOCK_DRIVER2    | INPUT_ON , led_usr_unlock      },             
  {INPUT_UNLOCK_DRIVER3    | INPUT_ON , led_usr_unlock      },     
  {INPUT_UNLOCK_ALL1       | INPUT_ON , led_usr_unlock      },     
  {INPUT_UNLOCK_ALL2       | INPUT_ON , led_usr_unlock      },     
  {INPUT_UNLOCK_ALL3       | INPUT_ON , led_usr_unlock      },

  {INPUT_TRUNK             | INPUT_ON , led_usr_trunk       },

  {INPUT_AUX1              | INPUT_ON , led_usr_aux1        },
  {INPUT_AUX2              | INPUT_ON , led_usr_aux2        },
  {INPUT_AUX3              | INPUT_ON , led_usr_aux3        },

  {INPUT_IGNITION          | INPUT_ON , led_usr_rs_ign_on   },
  {INPUT_IGNITION                     , led_usr_rs_ign_off  },
  {INPUT_RS_IGNITION       | INPUT_ON , led_usr_rs_ign_on   },
  {INPUT_RS_IGNITION                  , led_usr_rs_ign_off  },
  {INPUT_STARTER           | INPUT_ON , led_usr_rs_start_on },
  {INPUT_STARTER                      , led_usr_rs_start_off},
  {INPUT_RS_STARTER        | INPUT_ON , led_usr_rs_start_on },
  {INPUT_RS_STARTER                   , led_usr_rs_start_off},
  {INPUT_KEY2GO_LOG_DONE              , led_usr_key2go_led_done},      

} ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void led_usr_init (void)
{
  if(!led_usr_vars.is_init)
    {
      led_usr_vars.timeout = timeout_f_create(NULL , led_usr_timeout) ;

      CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);  
    }

  led_usr_vars.is_init = TRUE ;
  
}
