/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rxt_start.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rxt_private.h"
#include "rf.h"

#include "display.h"
#include "prog.h"
#include "led_usr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define string "\r\n" "rxt "

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rxt_start (void * arg)
{
    if (!rxt_gwr_is_detect && prog_retrieve_option (FEAT_RF_OUTPUT) == NO_RF)
    {
      prog_set_option(RF_TD, FEAT_RF_OUTPUT) ;
    }

    if (prog_retrieve_option (FEAT_RF_OUTPUT) == NO_RF) // if RF disable
    {
        led_usr_op_err(RXT_DISPLAY_ERR_RFTD_NOT_ENA ,1) ;

        #if DBG_RXT == 1
          dbg_tx_text (string) ;
          dbg_tx_text ("not active") ;
        #endif
    }
    else
    {
        if(rxt.state != RXT_STATE_STARTED)
        {       
            rxt.state = RXT_STATE_STARTING ;
        }
                
        if(rxt.ready.byte == 0XFF)
        {
            if(rxt.state == RXT_STATE_STARTING)
            {
                timeout_update (rxt.timeout, RXT_START_TIMEOUT) ;
            }
            else
            {
                timeout_update (rxt.timeout, RXT_1MIN_TIMEOUT) ;
            }
            timeout_start (rxt.timeout) ;
    
            rxt.runtime = rxt.default_runtime ;
            
            rxt_do_start () ;

            display_flash (GREEN_LED, RXT_DISPLAY_FUNC_START, 200, 200) ;
        }
        else
        {
            if(rxt.state == RXT_STATE_STARTING)
            {
                rxt.state = RXT_STATE_STOP ;
            }
            
            led_usr_op_err(RXT_DISPLAY_ERR_OEM_DIS ,1) ;

            #if DBG_RXT == 1
              dbg_tx_text (string) ;
              dbg_tx_text ("disabled") ;
            #endif
        }

        rxt_rf_update ((rxt.ready.byte == 0xFF)?TRUE:FALSE) ;         //update time only when ready
    }
}

//--------------------------------------------------------------------------//

