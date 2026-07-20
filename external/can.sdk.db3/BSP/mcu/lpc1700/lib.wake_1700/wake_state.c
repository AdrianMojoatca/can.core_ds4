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
#endif


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
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
    wake_state_trace();
    
    if(wake_vars.counter1 != 0) return PER_EXEC_SLEEP;
    if(wake_vars.counter2 != 0) return PER_EXEC_SLEEP;
    
    if( !(--wake_vars.timeout) )
    {
        CRITICAL_TSK;
        
        #if DBG_WAKE == 1
          dbg_tx_text("Sleeping... ");  os_tsk_wait(100);
        #endif        

      C_ENTER_TSK;
			IRQ_DIS_GLOBAL;
			#ifdef PLATFORM_404
			isr_set_fiq_pri(ISR_ID_TIMER_0);
			#endif
			isr_set_fiq_pri(ISR_ID_TIMER_1);
			isr_set_fiq_pri(ISR_ID_TIMER_2);
			isr_set_fiq_pri(ISR_ID_TIMER_3);
        {            
            gpio_wake_enter_sleep();
            
            wake_service(s_SLEEP) ;
            
            wake_vars.do_wake = FALSE;

					  wake_going_sleep = TRUE;
            wake_cpu_sleep(); 

        }    
				
			IRQ_ENA_GLOBAL;	
       while(!(wake_vars.do_wake || (gpio_wake_ext_triggered())))
        {   
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
				IRQ_DIS_GLOBAL;

                        
        {                        
          wake_vars.tc = T2TC;                         

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

				
        if(wake_by_gpio)
        {
					wake_by_gpio = FALSE;
          #if DBG_WAKE == 1  
            dbg_tx_text("\n\rWake by GPIO");
          #endif
        }
				else if(wake_vars.do_wake )
        {
          #if DBG_WAKE == 1  
            dbg_tx_text("\n\rWake by RTC");
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
