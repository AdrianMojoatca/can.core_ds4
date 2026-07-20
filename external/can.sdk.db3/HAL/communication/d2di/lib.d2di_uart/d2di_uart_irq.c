/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
#include "d2di_uart_private.h"
#include "d2di_private.h"
#include "d2di.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void d2di_rls_service (void) ;
static void d2di_rda_service (void) ;
static void d2di_thre_service(void) ;
static void d2di_cti_service(void)  ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static volatile UInt8 d2di_rda_rx;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void d2di_uart_irq_service(void)
{ 
    UInt8  iir_value    ;

    iir_value   = U0IIR ;
    iir_value >>= 1     ; // skip pending bit in IIR
    iir_value  &= 0x07  ; // check bit 1~3, interrupt identification
 
    switch (iir_value)
    {
        case IIR_RLS  : d2di_rls_service  () ; break ;  // Receive Line Status
        case IIR_RDA  : d2di_rda_service  () ; break ;	// Receive Data Available
        case IIR_CTI  : d2di_cti_service  () ; break ;  // Character timeout indicator
        case IIR_THRE : d2di_thre_service () ; break ;  // THRE, transmit holding register empty
    }
}

//--------------------------------------------------------------------------//
static void d2di_cti_service  (void)    /* Character Time-out indicator */
{
    UInt8  lsr_value = U0LSR   ; // clear error
    d2di_uart_err_add(D2DI_UART_ERR_TIMEOUT);
}

//--------------------------------------------------------------------------//
/* Check the rx line status register for possible errors. */
static void d2di_rls_service(void) 
{
    UInt8 lsr_value = U0LSR;

    // possible error, clear it and return
    if (lsr_value & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI))
    {
        static UInt8  log_once;
        UInt8 dummy  = U0RBR; // dummy read on RX to clear error
    
        // overrun error
        if(lsr_value & LSR_OE)
        {
            d2di_uart_err_add(D2DI_UART_ERR_OVERRUN) ;
        }

        // parity error
        if(lsr_value & LSR_PE)
        {
            d2di_uart_err_add(D2DI_UART_ERR_PARITY) ;
        }

        // framing error
        if((lsr_value & LSR_FE) && !(log_once & (1u << D2DI_UART_ERR_FRAMING)))
        {
            log_once |= 1u << D2DI_UART_ERR_FRAMING ;
            d2di_uart_err_add(D2DI_UART_ERR_FRAMING) ;
        }
        
        // rx fifo error
        if((lsr_value & LSR_RXFE) && !(log_once & (1u << D2DI_UART_ERR_RXFE)))
        {
            log_once |= 1u << D2DI_UART_ERR_RXFE ;
            d2di_uart_err_add(D2DI_UART_ERR_RXFE) ;
        }
        
        // break interrupt error
        if((lsr_value & LSR_BI) && !(log_once & (1u << D2DI_UART_ERR_BREAK)))
        {
            log_once |= 1u << D2DI_UART_ERR_BREAK ;
            d2di_uart_err_add(D2DI_UART_ERR_BREAK) ;
        }
    }
    
    else if (lsr_value & LSR_RDR) // normal read
    {
        d2di_rda_service();
    }
}

//--------------------------------------------------------------------------//
/* Rx data available. Insert byte into rx queue. If this is first packet after
 * waking up, receive it @4MGHz without notifying task. */
static void d2di_rda_service(void)
{
    d2di_rda_rx = U0RBR;
    
    if(d2di_uart_rx_q_insert(d2di_rda_rx) == D2DI_UART_Q_FULL)
    {
        // do something here if rx q is full. for now we simply log the error
    }
    
    if(d2di_wake_ready_bytes < (D2DI_MIN_PACKET_SIZE - 2))
    {
        d2di_wake_ready_bytes++;
        d2di_uart_vars.d2di_wake_ready_got_byte = TRUE;
    }
    
    else if(d2di_rda_rx == D2DI_SOF)
    {
        os_irq_set_i(d2di_uart_rx_irq);
    }
}

//--------------------------------------------------------------------------//
/* Transmitter holding register empty. Remove byte from tx queue and send it. */
static void d2di_thre_service(void)
{
    UInt8 lsr_value, tx_val;
    lsr_value = U0LSR;
       
    // if THRE register is empty and tx queue is not empty
    if ((lsr_value & LSR_THRE) && (d2di_uart_tx_q_remove(&tx_val) != D2DI_UART_Q_EMPTY))
    {
        U0THR = (UInt8) tx_val;
    }
}
