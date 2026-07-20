/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rxt_timeout.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rxt_private.h"
#include "rf.h"
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

void rxt_timeout (void * arg)
{   
    switch (rxt.state)
    {
        case RXT_STATE_STARTING :
        {    
            if(rxt.attempt >= RXT_ATTEMPT)
            {
                rxt.state = RXT_STATE_STOP ;
                
                ATOMIC (rxt.attempt = 0) ;
                led_usr_op_err(RXT_DISPLAY_ERR_START_FAIL ,1) ;
    
                rxt_error (ERR_RXT_START_FAIL) ;

                rxt.runtime = 0 ;
                rxt_rf_update (TRUE) ;
    
                #if DBG_RXT == 1
                  dbg_tx_text (string) ;
                  dbg_tx_text ("error start failed") ;
                #endif
            }
            else
            {
                if(!rxt.oem_start)
                {
                    rxt_do_start () ; //retry
                    timeout_start (rxt.timeout) ;
                    #if DBG_RXT == 1
                      dbg_tx_text (string) ;
                      dbg_tx_text ("start failed, retry...") ;
                    #endif
                }
                else
                {
                    rxt.oem_start = FALSE ;
                    #if DBG_RXT == 1
                      dbg_tx_text (string) ;
                      dbg_tx_text ("oem start failed") ;
                    #endif
                }
            }   
        }
        break ;
        
        case RXT_STATE_STARTED :
        {           
            if(rxt.stopping)
            {
                if(rxt.attempt >= RXT_ATTEMPT)
                {
                    ATOMIC 
                    (
                        rxt.stopping = FALSE ;
                        rxt.attempt = 0 ;
                    )
                    led_usr_op_err(RXT_DISPLAY_ERR_STOP_FAIL ,1) ;
    
                    rxt_error (ERR_RXT_STOP_FAIL) ;
                
                    #if DBG_RXT == 1
                      dbg_tx_text (string) ;
                      dbg_tx_text ("error stop failed") ;
                    #endif  
                }  
                else
                {
                    rxt_do_stop () ; //retry
                    timeout_start (rxt.timeout) ;
                    #if DBG_RXT == 1
                      dbg_tx_text (string) ;
                      dbg_tx_text ("stop failed, retry...") ;
                    #endif
                }   
            }
            else if(--rxt.runtime < RXT_OVERRUN)
            {           
                led_usr_op_err(RXT_DISPLAY_ERR_STOP_FAIL ,1) ;
    
                rxt_error (ERR_RXT_OVERRUN) ;
            
                #if DBG_RXT == 1
                  dbg_tx_text (string) ;
                  dbg_tx_text ("overrun detected") ;
                #endif  
            }
            else
            {
                if(rxt.runtime == RXT_EXPIRE_IN_3MIN || rxt.runtime == RXT_EXPIRE_IN_1MIN)
                {
                    rxt_rf_update (TRUE) ;
                }
                timeout_update (rxt.timeout, RXT_1MIN_TIMEOUT) ;    //decrement time every minute
                timeout_start (rxt.timeout) ;
            }
            rf_sound_set((Rf_Sound_Def)rxt.runtime,FALSE);           
        }
        break ;

        default:
        break ;
    }           
}

//--------------------------------------------------------------------------//

