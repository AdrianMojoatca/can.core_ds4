/*==========================================================================*/
// $Id: wake_state.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "wake_private.h"
#include "isr.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define WAKE_TEST       0

#if WAKE_TEST == 1
#include "config_gpio.h"
#endif

//#define D2D1_WAKE_DBG_PIN

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void static wake_state_trace (void) ;
void static wake_state_test  (void) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

UInt8 wake_reason; // robust copy of gpio_wake_reason

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
#if USE_TIMESTAMP == 1
Wake_Func  wake_func_warning;
/*--------------------------------------------------------------------------*/
void wake_warning_register(Wake_Func func)  
{  //only support 1 warning function today
	wake_func_warning = func;
}
#endif

static UInt8 sleep_soon = 0;
void wake_sleep_soon(void)
{
	sleep_soon = 1;
	wake_vars.timeout = WAKE_TIMEOUT2; // 1 second
}

static UInt8 sleep_blink = 0;
Per_Exec_Wake wake_state (void)
{
    int fiq_critical ;
    int irq_critical ;
    

    wake_state_trace();
    
    if(wake_vars.counter1 != 0) return PER_EXEC_SLEEP;  // don't because we have some wake_enter bits set
    if(wake_vars.counter2 != 0) return PER_EXEC_SLEEP;  // don't because we are inside wake_ck_enter
	
    if(is_validation && sleep_soon)
        OUT_LED_BLUE(++sleep_blink & 0x10);
    
    if(sleep_soon && wake_vars.timeout > 100)
        wake_vars.timeout = 100; // no one can put a longer timeout until we sleep
    
    if( !(--wake_vars.timeout) )
    {
        CRITICAL_TSK;
        
        #if DBG_WAKE == 1
          dbg_tx_text("Sleeping... ");  os_tsk_wait(100);
        #endif        
						
      C_ENTER_TSK;
			IRQ_DIS_GLOBAL;
			isr_set_fiq_pri(ISR_ID_TIMER_1);
			isr_set_fiq_pri(ISR_ID_TIMER_2);
			isr_set_fiq_pri(ISR_ID_TIMER_3);
        {            
				gpio_wake_set_shadow_enable(1); // allow shadows to be updated starting now
            gpio_wake_enter_sleep();
            
            wake_service(s_SLEEP) ;  // some people call nvfs_write
            
            wake_vars.do_wake = FALSE;
					
					  wake_going_sleep = TRUE;
		        sleep_soon = 0; // ... since we are going to sleep right now
            wake_cpu_sleep(); 
					
					wake_reason = 0;
        }    
			IRQ_ENA_GLOBAL;	
				
				for(;;)
				{
				  wake_reason = gpio_wake_reason();
					
					if(wake_reason)
						break;
					
					if(wake_vars.do_wake)
						break; // something non-pin set this flag  (vrtc)
					
					#if DBG_JTAG == 0
						SCB->SCR |= 0x04;
						
						//PCON = 0x09;		//TODO need to change for final power down mode
						PCON = 0x01;
					#endif

					#if DBG_JTAG == 1
						SCB->SCR |= 0x00;
						
						PCON = 0x00;
 					#endif
						__wfi();
            
            wake_state_test();
        }
				//IRQ_DIS_GLOBAL;
#ifdef D2D1_WAKE_DBG_PIN
				OUT_1(1);
#endif
				T2TC = 0;    // restart hardware timer
				IRQ_DIS_GLOBAL_ALL;   // _ALL, which fix the variation of sampling of d2d1_wake
				
        {                        
          wake_vars.tc = T2TC;                         
					gpio_wake_set_shadow_enable(0); // prevent further shadow updates so we can see the real reason we woke up!
				
				// wake_reason must be set before wake_service is called...	
				if(wake_reason == 0) // not pin?
        {
					if(wake_vars.do_wake) // wake by vrtc?					
					   wake_reason = 0x80; // wake by some non-pin source.  we assume RTC but it could be firmware special
					else
						wake_reason = 0x3F; // maybe impossible but let's be sure...
        }

					while( !wake_service_ready() );
		          wake_cpu_wake ();
					
  				isr_set_fiq_pri(ISR_ID_EINT_3);

					IRQ_DIS_GLOBAL;
					IRQ_ENA_GLOBAL_ALL;

          wake_going_sleep = FALSE;
					wake_service(s_WAKE);

					IRQ_DIS_GLOBAL_ALL;

          wake_vars.timeout = wake_vars.timeout2;

          gpio_wake_exit_sleep();
        }        
			isr_reset_fiq_pri(ISR_ID_TIMER_1);
			isr_reset_fiq_pri(ISR_ID_TIMER_2);
			isr_reset_fiq_pri(ISR_ID_TIMER_3);
			isr_reset_fiq_pri(ISR_ID_EINT_3);

				
			IRQ_ENA_GLOBAL;
			IRQ_ENA_GLOBAL_ALL;


      C_LEAVE_TSK;
	
    }
#if USE_TIMESTAMP == 1
		else
		{
			if(wake_vars.timeout == 10  && wake_func_warning )   //1 sec before sleep?
				(*wake_func_warning)(0);                             //warn user
		}
#endif

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
	
}

/*--------------------------------------------------------------------------*/

void wake_cpu_wake( void )
{
	crt0_clock_setup();
	PLL_Lock_Wait();
	crt0_pll_connect();
	
	SCB->SCR = 0x00;
	
	PCON = 0x00;

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
					  dbg_tx_text(" frm "); dbg_tx_h32(wake_vars.flags);
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
