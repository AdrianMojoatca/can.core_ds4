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

// maintain these individually.  perhaps one day the rising mask may differ from the falling mask
static UInt32 io0_int_stat_r, io0_int_stat_f, io2_int_stat_r, io2_int_stat_f; // cleared only when we go to sleep.

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
		

		IO0_INT_EN_R = (GPIO_WAKE_EXT0_MASK) ;// --- Enable the external interrupt for PORT 0 and PORT2     
		

		IO0_INT_EN_F = (GPIO_WAKE_EXT0_MASK) ;//     The interrupt will not trigger the service vector because the EINT3 is disconnected from VIC  


		IO2_INT_EN_R = (GPIO_WAKE_EXT2_MASK) ;     


		IO2_INT_EN_F = (GPIO_WAKE_EXT2_MASK) ;  		
		
    io0_int_stat_r = io0_int_stat_f = io2_int_stat_r = io2_int_stat_f = 0; // ONLY NOW do we clear all shadows
	
		
//		IO0_INT_CLR  = (GPIO_WAKE_EXT0_MASK); // --- Clear any pending interrupts
//		IO2_INT_CLR  = (GPIO_WAKE_EXT2_MASK);       
		
    IO0_INT_CLR  = 0xFFFFFFFF;
    IO2_INT_CLR  = 0xFFFFFFFF;



    #if DBG_GPIO == 1
        dbg_tx_text("\r\nGpio Sleep") ;
    #endif                               
}

/*--------------------------------------------------------------------------*/

static UInt8 int_stat_shadow_enable;
void gpio_wake_set_shadow_enable(UInt8 enable)
{
	int_stat_shadow_enable = enable;
}	

void try_update_shadow(void)
{
	  if(int_stat_shadow_enable)
		{
		 // accumulate all ints that become pending as they arrive
			{		
				io0_int_stat_r |= IO0_INT_STAT_R;
				io0_int_stat_f |= IO0_INT_STAT_F;
			}
			{
				io2_int_stat_r |= IO2_INT_STAT_R;
				io2_int_stat_f |= IO2_INT_STAT_F;
			}
	  }	
}

void gpio_wake_func_eint3_handler( void )
{
	
	  try_update_shadow(); // update int status shadows if allowed
	
		// this clears every interrupt now, but we have everything that happened since going to sleep
		IO0_INT_CLR  = 0xFFFFFFFF;
		IO2_INT_CLR  = 0xFFFFFFFF;
	
    if(gpio_wake_reason())
    {				        
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

    IO0_INT_CLR  = 0xFFFFFFFF;
    IO2_INT_CLR  = 0xFFFFFFFF;

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


static UInt8 lsbit8(UInt32 x)
{
UInt8	i;
  for(i=0;i<8;i++)
  {
    if(x & 1)
      break;
    x>>=1;
  }
  return i;
}


// return 0..31 lsbit set, or 32=no bits set
static UInt8 lsbit32(UInt32 x)
{
  // optimized.. faster than checking 32 bits one by one
  if(x & 0xFFFF)
  {
    if(x & 0xFF)
      return lsbit8(x);

    return 8+lsbit8(x>>8);
  }
  if(x)
  {
    x>>=16;
    if(x & 0xFF)
      return 16+lsbit8(x);

    return 24+lsbit8(x>>8);
  }
  return 32;
}

// returns 0=no reason, or 0x40..0x5F for port 0, or 0x60..0x7F for port 2
UInt8 gpio_wake_reason( void )
{
	UInt32 uart_bit_mask = 0;
	UInt32 io0,io2;
	UInt8 reason;
	

				  // we look at the shadow values only, we do NOT directly read the status registers
					io0 = (io0_int_stat_r | io0_int_stat_f) & GPIO_WAKE_EXT0_MASK;
					io2 = (io2_int_stat_r | io2_int_stat_f) & GPIO_WAKE_EXT2_MASK;
						
					reason = lsbit32(io0); // test port 0 sources
	
          if(reason < 32)
            reason += 0x40;  // 40..5F means port 0 bit number
          else
          {
	          reason = lsbit32(io2);
            if(reason < 32)
              reason += 0x60; // 60..7F means port 2 bit number
            else
						{
							// no other reason???
              reason = 0;  // no interrupt source edge changed
						}
          }

#ifdef PLATFORM_933	// (BUILD_NGMM == 1)
	uart_bit_mask = (1u<<GPIO_PL_PIN_NUM(GPIO_PL_D2D_RX));
	if( io0 & uart_bit_mask)
		wake_uart_no = UART_PORT0;
	
	uart_bit_mask = (1u<<GPIO_PL_PIN_NUM(GPIO_PL_RF_D2D_RX));
	if( io2 & uart_bit_mask)
		wake_uart_no = UART_PORT1;

	uart_bit_mask = (1u<<GPIO_PL_PIN_NUM(GPIO_PL_UART3_WAKE));
	if(  io0 & uart_bit_mask)
		wake_uart_no = UART_PORT3;
#endif
	
    return reason;
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
		
	  IO0_INT_EN_R = (GPIO_WAKE_EXT0_MASK) ;// --- Enable the external interrupt for PORT 0 and PORT2     
    IO0_INT_EN_F = (GPIO_WAKE_EXT0_MASK) ;//     The interrupt will not trigger the service vector because the EINT3 is disconnected from VIC  
    IO2_INT_EN_R = (((GPIO_WAKE_EXT2_MASK)&(~EXT_P2_RF))) ;     
    IO2_INT_EN_F = (((GPIO_WAKE_EXT2_MASK)&(~EXT_P2_RF))) ;  		

    IO0_INT_CLR  = (GPIO_WAKE_EXT0_MASK); // --- Clear any pending interrupts
    IO2_INT_CLR  = (((GPIO_WAKE_EXT2_MASK)&(~EXT_P2_RF)));         

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

   1
        

		gpio_pl_select(GPIO_PL_D2D_RX);
	
    VICIntEnable |= (1ul<<ISR_ID_EINT_3)   ;//--- Connect to  VIC the EINT3                
        
    #if DBG_GPIO == 1
        dbg_tx_text("\r\nGpio Wake") ;
    #endif                 
}

/*--------------------------------------------------------------------------*/
#endif
#endif
