/*==========================================================================*/
// $Id: gpio_wake_func.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "gpio_wake_private.h"
#include "gpio.h"
#include "wake.h"

#ifdef PLATFORM_933		// (BUILD_NGMM ==1)
	#include "d2d_switch.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static UInt32 __COPY_IO0_INT_EN_R;
static UInt32 __COPY_IO0_INT_EN_F;
static UInt32 __COPY_IO2_INT_EN_R;
static UInt32 __COPY_IO2_INT_EN_F;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
static void gpio_wake_func_eint3_handler( void );

/*--------------------------------------------------------------------------*/

void gpio_wake_enter_sleep(void)
{
	#ifdef DEV_LPC_176x
		fiq_register((Fiq_Func)gpio_wake_func_eint3_handler);
	#else
		VICIntEnClr |= (1ul<<ISR_ID_EINT_3)    ;//--- Disconnect the EINT3 from VIC controller
	#endif
     
	gpio_pl_func_cfg_gpio(GPIO_PL_D2D_RX);
                                           // --- Seems that the external interrupt is triggered no matter is the pin function
	                                         // --- TODO: to use D2D2, if remove this switching, needs to check the UART RX wakes module.
    __COPY_IO0_INT_EN_R = IO0_INT_EN_R;    // --- save the status of external interrupts
    __COPY_IO0_INT_EN_F = IO0_INT_EN_F;
    __COPY_IO2_INT_EN_R = IO2_INT_EN_R;
    __COPY_IO2_INT_EN_F = IO2_INT_EN_F;
		

		IO0_INT_EN_R |= (GPIO_WAKE_EXT0_MASK) ;// --- Enable the external interrupt for PORT 0 and PORT2     
		

		IO0_INT_EN_F |= (GPIO_WAKE_EXT0_MASK) ;//     The interrupt will not trigger the service vector because the EINT3 is disconnected from VIC  


		IO2_INT_EN_R |= (GPIO_WAKE_EXT2_MASK) ;     


		IO2_INT_EN_F |= (GPIO_WAKE_EXT2_MASK) ;  		
		
		IO0_INT_CLR  |= (GPIO_WAKE_EXT0_MASK); // --- Clear any pending interrupts



		IO2_INT_CLR  |= (GPIO_WAKE_EXT2_MASK);         

    #if DBG_GPIO == 1
        dbg_tx_text("\r\nGpio Sleep") ;
    #endif                               
}

/*--------------------------------------------------------------------------*/

void gpio_wake_func_eint3_handler( void )
{
    if(gpio_wake_ext_triggered())
    {
			#ifdef DEV_LPC_176x
 			  wake_by_gpio = TRUE;
			#endif
        IO0_INT_CLR  |= 0xFFFFFFFF;
        IO2_INT_CLR  |= 0xFFFFFFFF;
			
        
        wake_notify_wake();
    }
}

/*--------------------------------------------------------------------------*/

void gpio_wake_exit_sleep( void )
{
#ifdef DEV_LPC_176x
    fiq_unregister((Fiq_Func)gpio_wake_func_eint3_handler);
#endif
		IO0_INT_EN_R  = __COPY_IO0_INT_EN_R; // --- restore the status of external interrupts
    IO0_INT_EN_F  = __COPY_IO0_INT_EN_F;
    IO2_INT_EN_R  = __COPY_IO2_INT_EN_R;
    IO2_INT_EN_F  = __COPY_IO2_INT_EN_F;

    IO0_INT_CLR  |= 0xFFFFFFFF;
    IO2_INT_CLR  |= 0xFFFFFFFF;

		gpio_pl_select(GPIO_PL_D2D_RX);
	
#ifdef DEV_LPC_236x
    VICIntEnable |= (1ul<<ISR_ID_EINT_3)   ;//--- Connect to  VIC the EINT3                
#else
	  NVIC_EnableIRQ(EINT3_IRQn);		// used CMSIS function to enable interrupt ISR_ID_EINT_3
#endif

        
    #if DBG_GPIO == 1
        dbg_tx_text("\r\nGpio Wake") ;
    #endif                 
}

/*--------------------------------------------------------------------------*/

Boolean gpio_wake_ext_triggered( void )
{
#ifdef PLATFORM_933	
	UInt32 uart_bit_mask = 0;
#endif
  UInt32 io0_int_stat_r, io0_int_stat_f, io2_int_stat_r, io2_int_stat_f;
	
  io0_int_stat_r = IO0_INT_STAT_R;
  io0_int_stat_f = IO0_INT_STAT_F;
  io2_int_stat_r = IO2_INT_STAT_R;
  io2_int_stat_f = IO2_INT_STAT_F;

#ifdef PLATFORM_933
	
    uart_bit_mask |= (1u<<GPIO_PL_PIN_NUM(GPIO_PL_D2D_RX));
	if( ( io0_int_stat_r & uart_bit_mask ) || (io0_int_stat_f & uart_bit_mask))
		wake_uart_no = UART_PORT0;
	
	uart_bit_mask |= (1u<<GPIO_PL_PIN_NUM(GPIO_PL_RF_D2D_RX));
	if( ( io2_int_stat_r & uart_bit_mask ) || (io2_int_stat_f & uart_bit_mask))
		wake_uart_no = UART_PORT1;

	uart_bit_mask |= (1u<<GPIO_PL_PIN_NUM(GPIO_PL_UART3_WAKE));
	if( ( io0_int_stat_r & uart_bit_mask ) || (io0_int_stat_f & uart_bit_mask))
		wake_uart_no = UART_PORT3;
#endif
	
    return  !!(io0_int_stat_r&(GPIO_WAKE_EXT0_MASK)) ||
            !!(io0_int_stat_f&(GPIO_WAKE_EXT0_MASK)) ||
            !!(io2_int_stat_r&(GPIO_WAKE_EXT2_MASK)) ||
            !!(io2_int_stat_f&(GPIO_WAKE_EXT2_MASK))  ;
}

/*--------------------------------------------------------------------------*/

#ifdef	PLATFORM_CM800		//defined PLATFORM_CM800 &  defined DEV_LPC_236x
#if CM800_DEI_BRAND == 1

void gpio_wake_enter_sleep1(void)
{
	if(wake_get_current_cpu_freq() == CPU_FREQ_0MHZ)
    {            
        VICIntEnClr |= (1ul<<ISR_ID_EINT_3)    ;//--- Disconnect the EINT3 from VIC controller
    }
    else
    {
        //if we running in IDLE mode the EINT3 needs to be connected to VIC controller otherwise we cannot wake the CPU..
        
        fiq_register((Fiq_Func)gpio_wake_func_eint3_handler);
    }
                
    	gpio_pl_func_cfg_gpio(GPIO_PL_D2D_RX);// --- I think we can remove it...It's switching the D2D RX to GPIO...
                                           // --- Seems that the external interrupt is triggered no matter is the pin function
    __COPY_IO0_INT_EN_R = IO0_INT_EN_R;    // --- save the status of external interrupts
    __COPY_IO0_INT_EN_F = IO0_INT_EN_F;
    __COPY_IO2_INT_EN_R = IO2_INT_EN_R;
    __COPY_IO2_INT_EN_F = IO2_INT_EN_F;
		
	  IO0_INT_EN_R |= (GPIO_WAKE_EXT0_MASK) ;// --- Enable the external interrupt for PORT 0 and PORT2     
    IO0_INT_EN_F |= (GPIO_WAKE_EXT0_MASK) ;//     The interrupt will not trigger the service vector because the EINT3 is disconnected from VIC  
    IO2_INT_EN_R |= (((GPIO_WAKE_EXT2_MASK)&(~EXT_P2_RF))) ;     
    IO2_INT_EN_F |= (((GPIO_WAKE_EXT2_MASK)&(~EXT_P2_RF))) ;  		

    IO0_INT_CLR  |= (GPIO_WAKE_EXT0_MASK); // --- Clear any pending interrupts
    IO2_INT_CLR  |= (((GPIO_WAKE_EXT2_MASK)&(~EXT_P2_RF)));         

    #if DBG_GPIO == 1
        dbg_tx_text("\r\nGpio Sleep") ;
    #endif                               
}


/*--------------------------------------------------------------------------*/

Boolean gpio_wake_ext_triggered1( void )
{
    return  !!(IO0_INT_STAT_R&(GPIO_WAKE_EXT0_MASK)) ||
            !!(IO0_INT_STAT_F&(GPIO_WAKE_EXT0_MASK)) ||
            !!(IO2_INT_STAT_R&((GPIO_WAKE_EXT2_MASK)&(~EXT_P2_RF)) ) ||
            !!(IO2_INT_STAT_F&((GPIO_WAKE_EXT2_MASK)&(~EXT_P2_RF)) );
}

/*--------------------------------------------------------------------------*/

void gpio_wake_exit_sleep1( void )
{
    fiq_unregister((Fiq_Func)gpio_wake_func_eint3_handler);
    
    IO0_INT_EN_R  = __COPY_IO0_INT_EN_R; // --- restore the status of external interrupts
    IO0_INT_EN_F  = __COPY_IO0_INT_EN_F;
    IO2_INT_EN_R  = __COPY_IO2_INT_EN_R;
    IO2_INT_EN_F  = __COPY_IO2_INT_EN_F;

    IO0_INT_CLR  |= 0xFFFFFFFF;
    IO2_INT_CLR  |= 0xFFFFFFFF;
        

		gpio_pl_select(GPIO_PL_D2D_RX);
	
    VICIntEnable |= (1ul<<ISR_ID_EINT_3)   ;//--- Connect to  VIC the EINT3                
        
    #if DBG_GPIO == 1
        dbg_tx_text("\r\nGpio Wake") ;
    #endif                 
}

/*--------------------------------------------------------------------------*/
#endif
#endif
