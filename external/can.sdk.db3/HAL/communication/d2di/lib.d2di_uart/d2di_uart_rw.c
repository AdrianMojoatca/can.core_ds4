/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_uart_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef Boolean d2di_lock_t;
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean d2di_lock_acquire(void);
static void d2di_lock_release(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
/* Lock to protect d2di_insert_frame method.
 * 0 = unlocked (available)
 * 1 = locked (not available)
 */
static d2di_lock_t d2di_lock = FALSE;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/* Insert 1 byte in queue. */
D2Di_Uart_Q_Err d2di_uart_rx_q_insert(UInt8 c)
{
    UInt8 next_head = (d2di_uart_vars.d2di_uart_rx_q->head + 1) & (D2DI_UART_Q_MAX_SIZE - 1);
    if(next_head != d2di_uart_vars.d2di_uart_rx_q->tail)
    {
        d2di_uart_vars.d2di_uart_rx_q->data[d2di_uart_vars.d2di_uart_rx_q->head] = c;
        d2di_uart_vars.d2di_uart_rx_q->head = next_head;
        return D2DI_UART_Q_OK;
    }
    else    // no more space in queue
    {
        d2di_uart_err_add(D2DI_UART_ERR_RX_Q_FULL);
        return D2DI_UART_Q_FULL;
    }
}

//--------------------------------------------------------------------------//
/* Insert 1 byte in queue. */
D2Di_Uart_Q_Err d2di_uart_tx_q_insert(UInt8 c)
{
    UInt8 next_head = (d2di_uart_vars.d2di_uart_tx_q->head + 1) & (D2DI_UART_Q_MAX_SIZE - 1);
    if(next_head != d2di_uart_vars.d2di_uart_tx_q->tail)
    {
        d2di_uart_vars.d2di_uart_tx_q->data[d2di_uart_vars.d2di_uart_tx_q->head] = c;
        d2di_uart_vars.d2di_uart_tx_q->head = next_head;
        return D2DI_UART_Q_OK;
    }
    else    // no more space in queue
    {
        d2di_uart_err_add(D2DI_UART_ERR_TX_Q_FULL);
        return D2DI_UART_Q_FULL;
    }
}

//--------------------------------------------------------------------------//

/* Remove 1 byte from rx queue. Byte value is stored in val param. */
D2Di_Uart_Q_Err d2di_uart_rx_q_remove(UInt8 *val)
{
    if(d2di_uart_vars.d2di_uart_rx_q->head != d2di_uart_vars.d2di_uart_rx_q->tail)
    {
        *val = d2di_uart_vars.d2di_uart_rx_q->data[d2di_uart_vars.d2di_uart_rx_q->tail];
        d2di_uart_vars.d2di_uart_rx_q->tail = (d2di_uart_vars.d2di_uart_rx_q->tail + 1) & (D2DI_UART_Q_MAX_SIZE - 1);
        return D2DI_UART_Q_OK;
    }
    else    // queue is empty
    {
        return D2DI_UART_Q_EMPTY;
    }
}

//--------------------------------------------------------------------------//
/* Remove 1 byte from tx queue. Byte value is stored in val param. */
D2Di_Uart_Q_Err d2di_uart_tx_q_remove(UInt8 *val)
{
    if(d2di_uart_vars.d2di_uart_tx_q->head != d2di_uart_vars.d2di_uart_tx_q->tail)
    {
        *val = d2di_uart_vars.d2di_uart_tx_q->data[d2di_uart_vars.d2di_uart_tx_q->tail];
        d2di_uart_vars.d2di_uart_tx_q->tail = (d2di_uart_vars.d2di_uart_tx_q->tail + 1) & (D2DI_UART_Q_MAX_SIZE - 1);
        return D2DI_UART_Q_OK;
    }
    else    // queue is empty
    {
        return D2DI_UART_Q_EMPTY;
    }
}

//--------------------------------------------------------------------------//

/*
 * Insert a d2di frame in the uart tx queue
 * Params:
 *   d2di frame (UInt8 *)  : pointer to array of bytes containing d2di packet
 *   frame_size (UInt8)    : size in bytes of d2di packet
 * The function will wait to acquire the lock before starting to insert (keep bytes in order)
 * The function will retry any byte that may not be inserted because the q is full.
 */
void d2di_uart_tx_q_insert_frame(const UInt8 *d2di_frame, UInt8 frame_size)
{   
    UInt8 i;
    
    // spin until lock acquired
    while(!d2di_lock_acquire());
    
    // if not enough space available don't insert just yet
    while((((D2DI_UART_Q_MAX_SIZE - 1) - (d2di_uart_vars.d2di_uart_tx_q->head - d2di_uart_vars.d2di_uart_tx_q->tail)) < frame_size));
    
    /* We put the first byte in the tx register after inserting the rest of frame bytes in
     * the tx queue. This is done because sending the first byte is actually what triggers
     * the rest of the transmission and we want to have the bytes already queued up after
     * sending.  In the case where the q is not empty, we just append the bytes as transmission
     * was already triggered. */
    if(d2di_uart_tx_q_is_empty()) // q empty, need to trigger transmission
    {
        for(i = 1; i < frame_size; i++)
        {
            d2di_uart_tx_q_insert(d2di_frame[i]);
        }

        // insert first byte manually to trigger transmission
        U0THR = d2di_frame[0];
    }
    
    else // q not empty, transmission already triggered
    {
        for(i = 0; i < frame_size; i++)
        {
            d2di_uart_tx_q_insert(d2di_frame[i]);
        }
    }
    
    d2di_lock_release();
}

//--------------------------------------------------------------------------//
Boolean d2di_uart_rx_q_is_empty(void)
{
    Boolean empty = FALSE;

    ATOMIC
    (
        empty = (d2di_uart_vars.d2di_uart_rx_q->head == d2di_uart_vars.d2di_uart_rx_q->tail);
    )
    
    return empty;    
}

//--------------------------------------------------------------------------//
Boolean d2di_uart_tx_q_is_empty(void)
{
    Boolean empty = FALSE;

    ATOMIC
    (
        empty = (d2di_uart_vars.d2di_uart_tx_q->head == d2di_uart_vars.d2di_uart_tx_q->tail);
    )
    
    return empty;    
}

//--------------------------------------------------------------------------//

// returns TRUE if lock acquired FALSE otherwise
static Boolean d2di_lock_acquire(void)
{
    Boolean status = FALSE;
    ATOMIC
    (
        if(!d2di_lock)
        {
            d2di_lock = TRUE;
            status = TRUE;
        }
    )
    return status;
}

//--------------------------------------------------------------------------//
static void d2di_lock_release(void)
{
    ATOMIC
    (
        d2di_lock = FALSE;
    )
}

