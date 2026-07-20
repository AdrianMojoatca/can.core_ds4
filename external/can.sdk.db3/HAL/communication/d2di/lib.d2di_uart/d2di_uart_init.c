/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_uart_private.h"
#include "d2di.h"
#include "gpio.h"
#include "isr.h"
#include "dbg.h"
#include "fiq.h"
#include "string.h"
#include "core_contract_db3_d2d_gpio.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean d2di_uart_init_ver (void);
static Boolean d2di_uart_init_nvfs(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
const NVFS_Entry_Name nvfs_d2di_uart_ver       = "D2DI_UART_VER";
const NVFS_Entry_Name nvfs_d2di_bus_error      = "D2DI_BUS_ERROR";
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
Boolean d2di_uart_init (void)
{
    if(d2di_uart_init_nvfs())
    {
        CRITICAL ;
        
        memset(&d2di_uart_vars, NULL, sizeof(d2di_uart_vars));
        
        // definition of global queues
        d2di_uart_vars.d2di_uart_rx_q = os_mem_request(sizeof(D2Di_Uart_Q));
        d2di_uart_vars.d2di_uart_tx_q = os_mem_request(sizeof(D2Di_Uart_Q));
        
        d2di_uart_rx_irq = os_irq_create();
        
        C_ENTER ; // configure UART0
        {	
            PCONP   |= (1u << 3) ;                                   // Turn on UART0 clock
            d2di_uart_cfg_baudrate((UInt32)core_contract_db3_d2d_uart_channel(), D2DI_UART_72MHZ); // configure baudrate
            PCLKSEL0 = (PCLKSEL0 & ~(3U << 6)) | (1u << 6) ;         // CPU_CLK / 1 = 72 MHz			
            
            core_contract_db3_d2d_select_main_uart_pins();
            
            isr_set_vector   (ISR_ID_UART_0 , ISR_PRI_14 , ISR_TYPE_IRQ , d2di_uart_irq_service) ;
            
            wake_register      (d2di_wake);
            wake_register_ready(d2di_wake_ready);
            fiq_register(d2di_fiq_service);
            
            core_contract_db3_d2d_rf_enable(TRUE); //enable first to have bus available
            
            U0IER |= (1 << 0);  // enable rx interrupt
            U0IER |= (1 << 1);  // enable tx interrupt
            U0IER |= (1 << 2);  // enable LSR interrupt
        }
    
        C_LEAVE ;
        
        // check if heap was allocated by os
        if(d2di_uart_vars.d2di_uart_rx_q && d2di_uart_vars.d2di_uart_tx_q && d2di_uart_rx_irq)
        {
            #if DBG_D2D == 1
                dbg_tx_text("\n[D2DI_UART] Init done.");
            #endif
            return TRUE;
        }
        
        else
        {
            #if DBG_D2D == 1
                dbg_tx_text("\n[D2DI_UART] Init fail! Not enough heap!");
            #endif
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

//--------------------------------------------------------------------------//
static Boolean d2di_uart_init_bus_error (void)
{
    D2Di_Status status = d2di_nvfs_check(nvfs_d2di_bus_error , &d2di_uart_vars.err , sizeof(D2Di_Uart_Error) , NULL);
    return (status != D2DI_NOT_VALID);
}

//--------------------------------------------------------------------------//
static Boolean d2di_uart_init_ver(void)
{
    D2Di_Status status = d2di_nvfs_check(nvfs_d2di_uart_ver , &d2di_uart_vars.version , sizeof(D2Di_Uart_Nvfs_Version) , D2DI_UART_VERSION);
    
    if(d2di_uart_vars.version != D2DI_UART_VERSION || status == D2DI_RECREATE)
    {
        nvfs_delete(nvfs_d2di_uart_ver);
        nvfs_delete(nvfs_d2di_bus_error);
        
        #if DBG_D2D == 1
            dbg_tx_text("\n[D2DI_UART] Version Mistmatch Nvfs Delete");
        #endif  
    
        d2di_nvfs_check(nvfs_d2di_uart_ver , &d2di_uart_vars.version , sizeof(D2Di_Uart_Nvfs_Version) , D2DI_UART_VERSION);   
    }

    return TRUE ;
}

//--------------------------------------------------------------------------//
static Boolean d2di_uart_init_nvfs(void)
{
    return (d2di_uart_init_ver() && d2di_uart_init_bus_error());
}

//--------------------------------------------------------------------------//
/* Configure uart baudrate based on CPU clock frequency (72MHz if full speed, 4MHz if wakeup speed. */
void d2di_uart_cfg_baudrate(UInt32 chan_num, D2Di_Uart_Clk_Speed clk_speed)
{
    if(clk_speed == D2DI_UART_72MHZ)
    {
        switch(chan_num)
        {
            case 0:
                U0LCR = 0x87 ;  
                U0DLL = 0xD4 ;
                U0DLM = 0x01 ;
                U0FDR = 0x10 ;
                U0LCR &= 0x7F;  
                U0FCR = 0x07 ;  
                break;
                
            case 1:
                U1LCR = 0x87 ;  
                U1DLL = 0xD4 ;
                U1DLM = 0x01 ;
                U1FDR = 0x10 ;
                U1LCR &= 0x7F;  
                U1FCR = 0x07 ;  
                break;
                
            case 2:
                U2LCR = 0x87 ;  
                U2DLL = 0xD4 ;
                U2DLM = 0x01 ;
                U2FDR = 0x10 ;
                U2LCR &= 0x7F;  
                U2FCR  = 0xC7;  
                break;
            
            case 3:
                U3LCR = 0x87 ;  
                U3DLL = 0xD4 ;
                U3DLM = 0x01 ;
                U3FDR = 0x10 ;
                U3LCR &= 0x7F;  
                U3FCR  = 0x07;  
                break;
        }
    }
    
    else if(clk_speed == D2DI_UART_4MHZ)
    {
        switch(chan_num)
        {
            case 0: 
                U0LCR =  0x87 ;  
                U0DLL =  0x11 ;  
                U0DLM =  0x00 ;  
                U0FDR =  0xF8 ;  
                U0LCR &= 0x7F ;  
                U0FCR =  0x07 ;  
                break;           
                                 
            case 1: 
                U1LCR =  0x87 ;  
                U1DLL =  0x11 ;  
                U1DLM =  0x00 ;  
                U1FDR =  0xF8 ;  
                U1LCR &= 0x7F ;  
                U1FCR =  0x07 ;  
                break;           
                                 
            case 2: 
                U2LCR =  0x87 ;  
                U2DLL =  0x11 ;  
                U2DLM =  0x00 ;  
                U2FDR =  0xF8 ;  
                U2LCR &= 0x7F ;  
                U2FCR =  0xC7 ;  
                break;           
                                 
            case 3: 
                U3LCR =  0x87 ;  
                U3DLL =  0x11 ;  
                U3DLM =  0x00 ;  
                U3FDR =  0xF8 ;  
                U3LCR &= 0x7F ;  
                U3FCR =  0x07 ;  
                break;
        }
    }
}


