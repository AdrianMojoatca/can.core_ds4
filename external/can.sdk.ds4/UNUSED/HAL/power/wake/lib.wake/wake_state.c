/*==========================================================================*/
// $Id: wake_state.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "wake_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define WAKE_TEST       0

#if WAKE_TEST == 1
#include "config_gpio.h"
#endif
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
extern void crt0_pll_connect (void) ;
extern void PLL_Lock_Wait    (void) ;
extern void crt0_clock_setup (void) ;
extern void crt0_pll_feed    (void) ;

void static wake_state_trace (void) ;
void static wake_state_test  (void) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

Per_Exec_Wake wake_state (void)
{
    int fiq_critical ;
    int irq_critical ;
    
    Boolean wake_by_gpio = FALSE;    

    #ifdef PLATFORM_CM800
	#if CM800_DEI_BRAND == 1
    if(wake_vars.sleep_freq != CPU_FREQ_0MHZ) return wake_state1();
	#endif
    #endif
    
    wake_state_trace();
    
    if(wake_vars.counter1 != 0) return PER_EXEC_SLEEP;
    if(wake_vars.counter2 != 0) return PER_EXEC_SLEEP;
    
    if( !(--wake_vars.timeout) )
    {
        CRITICAL_TSK;
        
        #if DBG_WAKE == 1
          dbg_tx_text("Sleeping...");  os_tsk_wait(100);
        #endif        

        C_ENTER_TSK;
        fiq_critical = __disable_fiq();
        irq_critical = __disable_irq();
        {            
            gpio_wake_enter_sleep();
            
            wake_service(s_SLEEP) ;
            
            wake_vars.do_wake = FALSE;

            wake_cpu_sleep();            
        }        
        if(!irq_critical)__enable_irq(); // --- enable the IRQ because we need to handle
                                         // --- the RTC events

        while(!(wake_vars.do_wake || (gpio_wake_ext_triggered())))
        {                    
            // --- if I'm interrupted by RTC(without wake nontification) and the GPIO state wasn't changed 
            // --- I'm sleeping again
            
            irq_critical = __disable_irq();
                        
            wake_cpu_sleep();

            wake_state_test();
            
            if(!irq_critical)__enable_irq();            
        }

        wake_by_gpio = gpio_wake_ext_triggered() ;
                        
        irq_critical = __disable_irq();
        {                        
            wake_vars.tc = T2TC;                         

            while( !wake_service_ready() );
            
            wake_cpu_wake ();

            wake_service(s_WAKE);

            wake_vars.timeout = wake_vars.timeout2;

            gpio_wake_exit_sleep();
        }        
        if(!fiq_critical)__enable_fiq();
        if(!irq_critical)__enable_irq();
        C_LEAVE_TSK;
        
        if(wake_vars.do_wake )
        {
          #if DBG_WAKE == 1  
            dbg_tx_text("\n\rWake by RTC");
          #endif
        }

        if(wake_by_gpio)
        {
          #if DBG_WAKE == 1  
            dbg_tx_text("\n\rWake by GPIO");
          #endif
        }

    }
                            
    return PER_EXEC_SLEEP ; 
}

/*--------------------------------------------------------------------------*/

void wake_cpu_sleep( void )
{
    PLLCON  = 0    ;        // Disable PLL
    
    crt0_pll_feed();        // Do a safe PLL FEED sequence

    CCLKCFG = 0   ;         // Reset clock divider to /1
      
    CLKSRCSEL = 0 ;         // Select internal RC oscillator as clock source (4 MHz)

    SCS &= ~(1u << 5) ;     // Disable the main oscillator
    
    PCON = 0x02   ;         // Enter POWER DOWN mode
}

/*--------------------------------------------------------------------------*/

void wake_cpu_wake( void )
{
    __asm
    {
        bl crt0_clock_setup
        bl PLL_Lock_Wait
        bl crt0_pll_connect
    };    
}

/*--------------------------------------------------------------------------*/

void wake_state_trace( void )
{
    #if DBG_WAKE == 1
    {
        static int timeout_trace = 0;
    
        if( ++timeout_trace == 500 ) //--- trace the wake_state on every 5 seconds
        {                   
            dbg_tx_text("\n\r[wake_state");
            dbg_tx_text(" c1=");dbg_tx_h8(wake_vars.counter1);
            dbg_tx_text(" c2=");dbg_tx_h8(wake_vars.counter2);
            dbg_tx_text("  t=");dbg_tx_u(wake_vars.timeout, 5);
            dbg_tx_text("]\n\r");
                        
            timeout_trace = 0;
        }  
    }
    #endif
}      
        
/*--------------------------------------------------------------------------*/

void wake_state_test( void )
{
    #if WAKE_TEST == 1
    
        static Boolean test = FALSE;    
    
        OUT_2(test);
    
        test = !test;
    
    #endif
}
