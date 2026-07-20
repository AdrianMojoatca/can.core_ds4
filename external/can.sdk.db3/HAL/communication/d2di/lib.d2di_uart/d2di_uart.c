/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_uart.h"
#include "d2di_uart_init.c"
#include "d2di_uart_irq.c"
#include "d2di_uart_private.h"
#include "d2di_uart_rw.c"
#include "d2di_uart_wake.c"
#include "d2di_uart_vars.c"

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

Boolean d2di_uart_err_save (void)           
{
    d2di_uart_vars.d2di_uart_do_err_save = FALSE ;
    return (nvfs_wr(nvfs_d2di_bus_error, &d2di_uart_vars.err) == NVFS_OK);
}

//--------------------------------------------------------------------------//
void d2di_uart_err_add (D2Di_Uart_Error_Code code)
{  
    UInt8 ptr = (d2di_uart_vars.err.count % D2DI_UART_ERR_BUFF_SIZE);
    d2di_uart_vars.err.code[ptr] = code ;
    d2di_uart_vars.err.count++ ;
    d2di_uart_vars.d2di_uart_do_err_save = TRUE ;
}

//--------------------------------------------------------------------------//
Boolean d2di_uart_is_err_save_set(void)
{
    if(d2di_uart_vars.d2di_uart_do_err_save)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
