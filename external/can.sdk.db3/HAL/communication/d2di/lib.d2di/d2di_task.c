/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_private.h"
#include "d2di_uart.h"
#include "wake.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/* If UART rx q contains data decode the frame and if it is valid process it.*/
void d2di_task (void * tsk_arg)
{
    UInt8 *d2di_frame;
    d2di_request_rs_info();
    
    for(;;)
    {
        // wait for UART signal that 0x0C is received
        if(os_irq_wait(d2di_uart_rx_irq, FALSE, D2DI_TSK_TIMEOUT))
        {
            if(!d2di_uart_rx_q_is_empty())
            {
                wake_enter();
                if(d2di_decode_frame(&d2di_frame))
                {
                    d2di_process_frame(d2di_frame);
                }
                wake_leave();
            }
            
            if(d2di_vars.d2di_do_err_save)
            {
                #if DBG_D2D == 1
                    dbg_tx_text("\n[D2DI_TASK] Logged error!");
                #endif
                
                wake_enter() ;
                d2di_err_save() ;
                wake_leave() ;
            }
        }
        
        // periodically check if any errors on bus
        else
        {
            if(d2di_uart_is_err_save_set())
            {
                #if DBG_D2D == 1
                    dbg_tx_text("\n[D2DI_UART] Logged bus error!");
                #endif
                
                wake_enter();
                d2di_uart_err_save();
                wake_leave();
            }
        }
    }
}
