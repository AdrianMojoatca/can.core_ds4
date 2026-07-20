/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_uart_private.h"
#include "wake.h"
#include "gpio.h"
#include "core_contract_db3_d2d_gpio.h"

void gpio_iface_rf_loop_toggle(void);
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
/* --- Set to 1 to use debug pin --- */
#define D2DI_WAKE_DEBUG                    0

#if D2DI_WAKE_DEBUG == 1
    #define D2DI_DBG_PIN_DELAY_US          25 // delay in us to toggle pin (measured)
    #define D2DI_DBG_PIN_TOGGLE()          gpio_iface_rf_loop_toggle()
#else
    #define D2DI_DBG_PIN_DELAY_US          0
    #define D2DI_DBG_PIN_TOGGLE()          ;
#endif

#define D2DI_PIN core_contract_db3_d2d_rx_read(FALSE)

// observed minimum delay to wake (us)
#define CPU_MIN_OBSERVED_WAKE_DELAY_US     160
#define D2DI_BIT_DURATION_US               104

// delay from real wakeup to first stop bit
#define D2DI_STOP_BIT_DELAY_US             ((9 * D2DI_BIT_DURATION_US) - CPU_MIN_OBSERVED_WAKE_DELAY_US) // 9 bits duration - hw wakeup delay

// 4 ticks per us @4MGHz
#define CPU_WAKE_CLOCK_MULT                4

// max delay between 2 bytes
#define D2DI_WAKE_READY_MAX_TIMEOUT_US     1560 // us
#define D2DI_WAKE_READY_MAX_TIMEOUT_TICKS  (CPU_WAKE_CLOCK_MULT * D2DI_WAKE_READY_MAX_TIMEOUT_US)

// delay to wait until turning on UART @ first stop bit (first byte scrap anyways)
#define D2DI_UART_POWERON_DELAY_TICKS      (CPU_WAKE_CLOCK_MULT * D2DI_STOP_BIT_DELAY_US)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
const UInt8 sof_byte = 0x0C;
static volatile Boolean waking_up;
static volatile Boolean do_wake_ready = FALSE;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void d2di_wake (Wake_Status wake_status)
{
    switch(wake_status)
    {
        case s_SLEEP:
            // configure UART0 for 4MHz clock
            d2di_uart_cfg_baudrate((UInt32)core_contract_db3_d2d_uart_channel(), D2DI_UART_4MHZ);
        
            // turn off UART
            PCONP &= ~(1u << 3);
            core_contract_db3_d2d_rx_gpio_mode();
            waking_up = TRUE;
            d2di_uart_vars.d2di_wake_ready_got_byte = FALSE;
            
            break;

        case s_WAKE:
            core_contract_db3_d2d_rx_select(); // switch pin label to rx
            PCONP   |= (1u << 3) ;          // turn on UART clock
            d2di_uart_cfg_baudrate((UInt32)core_contract_db3_d2d_uart_channel(), D2DI_UART_72MHZ);
            waking_up = FALSE;
        
            if(do_wake_ready)
            {
                do_wake_ready = FALSE;
                
                // set to max value so that any byte received from now on notifies task
                d2di_wake_ready_bytes = 0xFF; 
            }
            break;

        default : break ;
    }
}

/*--------------------------------------------------------------------------*/
/* Receive first packet @4MGHz then switch to full speed. */
Boolean d2di_wake_ready(void)
{
    if(do_wake_ready)
    {
        //wake_enter();
        d2di_uart_rx_q_insert(sof_byte);
        T2TC = 0;
        // wait to receive complete first packet (always 7 bytes)
        D2DI_DBG_PIN_TOGGLE();
        while((T2TC < (D2DI_WAKE_READY_MAX_TIMEOUT_TICKS - (2 * D2DI_DBG_PIN_DELAY_US))) && (d2di_wake_ready_bytes < (D2DI_MIN_PACKET_SIZE - 1)))
        {
            d2di_uart_irq_service();
            if(d2di_uart_vars.d2di_wake_ready_got_byte)
            {
                d2di_uart_vars.d2di_wake_ready_got_byte = FALSE;
                T2TC = 0;
            }
        }
        D2DI_DBG_PIN_TOGGLE();
    }
    return TRUE;
}

/*--------------------------------------------------------------------------*/
/* At wake up we need to check the D2D RX pin ASAP to know if the serial port
 * woke us up. If so wait for first stop bit in FIQ (776us) and set a flag to 
 * execute wake_ready routine which will receive first packet @4MGHz. */
void d2di_fiq_service(void)
{
    if(waking_up && !D2DI_PIN)
    {
        waking_up = FALSE;
        core_contract_db3_d2d_rx_select(); // switch pin label to rx

        D2DI_DBG_PIN_TOGGLE();
        T2TC = 0;
        // wait for first stop bit
        while(T2TC < (D2DI_UART_POWERON_DELAY_TICKS - (2 * D2DI_DBG_PIN_DELAY_US)));
        D2DI_DBG_PIN_TOGGLE();
        
        // turn on UART clk
        PCONP   |= (1u << 3) ;
        d2di_wake_ready_bytes = 0;
        do_wake_ready = TRUE;
    }
}
/*--------------------------------------------------------------------------*/
