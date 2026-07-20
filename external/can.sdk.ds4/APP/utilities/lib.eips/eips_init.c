/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: eips_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "eips_private.h"
#include "cmd.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void eips_init_timer(void) ;
static void eips_init_cmd  (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean eips_init (void)
{
  if(!eips_init_nvfs())
    {
      return FALSE ; // if error?
    }

#if DBG_EIPS == 1
  eips_vars.option.enabled = TRUE ;
#endif

  if(eips_vars.option.enabled)
    {    
      eips_init_timer() ;
      eips_init_cmd  () ;
      return TRUE ;
    }

  return FALSE ;
}
//--------------------------------------------------------------------------//
static void eips_init_cmd(void)
{
  static const Cmd_Array1 cmd_array[] = 
    {                                                                        
        {INPUT_GWR				| INPUT_ON , eips_gwr_on			}, 	
        {INPUT_GWR				  		   , eips_gwr_off			},  		
        {INPUT_ALL_DOOR_STATUS_OPEN        , eips_door_set   	    }, 	
        {INPUT_ALL_DOOR_STATUS_CLOSE	   , eips_door_clr	   	    }, 
        {INPUT_RPM_SET                     , eips_rpm_set           },
        {INPUT_RPM_CLR                     , eips_rpm_clr           },
        {INPUT_SPEED_SET                   , eips_speed_set         },
        {INPUT_SPEED_CLR                   , eips_speed_clr         },
        {INPUT_SKD_SET                     , eips_skd_set           },
        {INPUT_SKD_CLR                     , eips_skd_clr           },


    } ;

    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);


}
//--------------------------------------------------------------------------//

static void eips_init_timer(void)
{
Start_EIPS_Timer:
  {    
    #ifndef EIPS_TIMEOUT_START
      {
        #define EIPS_TIMEOUT_START EIPS_START_DEFAULT_TIMEOUT
      }
    #endif

    eips_vars.timeout_start = timeout_f_create(EIPS_TIMEOUT_START, eips_shutdown_timeout_start) ;

    #if DBG_EIPS == 1
      {
        dbg_tx_text("\r\nEIPS start Timer(sec) = ") ;
        dbg_tx_u(EIPS_TIMEOUT_START , 4) ;
      }
    #endif
  }

Shutdown_Timer:
  {
    if(eips_vars.option.shutdown == NULL)
      {
        eips_vars.option.shutdown = EIPS_SHUTDOWN_DEFAULT_TIMEOUT;
      }
   
    eips_vars.timeout_shutdown = timeout_f_create(eips_vars.option.shutdown * EIPS_SEC , eips_shutdown_func) ;

    #if DBG_EIPS == 1
      {
        dbg_tx_text("\r\nEIPS Shutdown Timer(sec) = ") ;
        dbg_tx_u(eips_vars.option.shutdown , 4) ;
      }
    #endif
  }

Alert_Timer:
  {
    if(eips_vars.option.alert_reminder == NULL)
      {
        eips_vars.option.alert_reminder = EIPS_ALERT_DEFAULT_TIMEOUT;
      }
    
    eips_vars.timeout_alert = timeout_f_create(eips_vars.option.alert_reminder  * EIPS_SEC, eips_alert_func) ;

    #if DBG_EIPS == 1
      {
        dbg_tx_text("\r\nEIPS Alert    Timer(sec) = ") ; dbg_tx_u(eips_vars.option.alert_reminder , 4) ;
      }
    #endif
  }

Check_Timer:
  {   
    eips_vars.check_timeout = EIPS_ENGINE_START_CHECK_TIMEOUT;
    
    eips_vars.timeout_shutdown_check = timeout_f_create(eips_vars.check_timeout * EIPS_SEC , eips_shutdown_check_func) ;

    #if DBG_EIPS == 1
      {
        dbg_tx_text("\r\nEIPS Check    Timer(sec) = ") ; dbg_tx_u(eips_vars.check_timeout , 4) ;
      }
    #endif
  }


}
