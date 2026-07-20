/*==========================================================================*/
// $Id: wake_state1.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "wake_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define WAKE_TEST1       0

#if WAKE_TEST1 == 1
#include "config_gpio.h"
#endif
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void  wake_state_trace1 (void);
static void  wake_state_test1  (void);
static void  wake_cpu_sleep1   (void); 
static void  wake_cpu_wake1    (void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

Per_Exec_Wake wake_state1 (void)
{
    int fiq_critical;
    int irq_critical;
    
    CRITICAL_TSK;
    
    wake_state_trace1();
    
    if(wake_vars.counter1 != 0) return PER_EXEC_SLEEP;
    if(wake_vars.counter2 != 0) return PER_EXEC_SLEEP;    
    if(--wake_vars.timeout    ) return PER_EXEC_SLEEP;
                   
    #if DBG_WAKE == 1
        dbg_tx_text("Sleeping..."); os_tsk_wait(100);
    #endif        

    C_ENTER_TSK;
    {        
        fiq_critical = __disable_fiq();
        irq_critical = __disable_irq();
        {            
            gpio_wake_enter_sleep1() ; // prepare the GPIOtowo enter in sleep mode (all the pins defined , except the RF pin)
            
            wake_vars.current_freq = wake_vars.sleep_freq; //update the current freq
            
            wake_service(s_SLEEP)    ; // execute the GO TO SLEEP services
            
            wake_vars.do_wake = FALSE; // clear the do wake flag

            wake_cpu_sleep1();         // put the CPU at low speed   
        }        
        if(!fiq_critical)__enable_fiq();
        if(!irq_critical)__enable_irq(); 
                
        while(!(wake_vars.do_wake || (gpio_wake_ext_triggered1())))
        {                    
            if(wake_vars.sleep_freq==CPU_FREQ_0MHZ)
            {
                PCON = 0x02 ;//--->> GO TO POWER DOWN MODE
            }
            else
            {
                PCON = 0x01 ;//--->> GO TO IDLE MODE
            }
            
            wake_state_test1();
        }
                                            
        irq_critical = __disable_irq();
        fiq_critical = __disable_fiq();
        {
            //we exiting from sleep mode , put the CPU at 4 MHZ (LIb.D2D expecting this value for freq).
            
            CLKSRCSEL = 0 ;    // switch RC 4MHz oscillator
            CCLKCFG   = 0 ;    // CCLK = PLL/1
                    
            wake_vars.tc = T2TC;    

    	    wake_vars.current_freq = CPU_FREQ_4MHZ;
                        
            while( !wake_service_ready() );
            
            wake_cpu_wake1();

            wake_vars.current_freq = CPU_FREQ_72MHZ; //now we running at 72MHZ
            
            wake_service(s_WAKE);

            wake_vars.timeout = wake_vars.timeout2;

            gpio_wake_exit_sleep1();
        }        
        if(!fiq_critical)__enable_fiq();
        if(!irq_critical)__enable_irq();
    }
    C_LEAVE_TSK;
        
    if(wake_vars.do_wake )
    {
        #if DBG_WAKE == 1  
            dbg_tx_text("\n\rWake by RTC or by RF");
        #endif
    }
    else
    {
        #if DBG_WAKE == 1  
            dbg_tx_text("\n\rWake by GPIO");
        #endif        
    }
                            
    return PER_EXEC_SLEEP ; 
}

/*--------------------------------------------------------------------------*/

void wake_cpu_sleep1( void )
{
    PLLCON  = 0    ;        // Disable PLL
    
    crt0_pll_feed();        // Do a safe PLL FEED sequence
    
    switch( wake_vars.sleep_freq )
	{
        //10.4 mA
		case CPU_FREQ_0MHZ: 
		{
			wake_vars.current_freq = CPU_FREQ_4MHZ;
            
            CCLKCFG = 0   ;         // Reset clock divider to /1
            
            CLKSRCSEL = 0 ; 		// switch RC 4MHz oscillator
						
			SCS &= ~(1u << 5);	    // Disable main oscillator									
		}
		break;
		
        //12.2mA (no antenna) , 19.3mA (with noisy antenna)
		case CPU_FREQ_1MHZ :
		{
            wake_vars.current_freq = CPU_FREQ_1MHZ;
            
			CCLKCFG = 11 ;      // CCLK = PLL/12			
		}
		break;
        
        //12.4mA (no antenna) , 19.5mA (with noisy antenna)
		case CPU_FREQ_2MHZ :
		{
            wake_vars.current_freq = CPU_FREQ_2MHZ;
            
			CCLKCFG = 5 ;	    // CCLK = PLL/6	 			 			
		}
		break;

        //12.6mA (no antenna) , 19.7mA (with noisy antenna)
		case CPU_FREQ_3MHZ :
		{
            wake_vars.current_freq = CPU_FREQ_3MHZ;
            
			CCLKCFG = 3 ;      // CCLK = PLL/4			
		}
		break;

        //13.3mA (no antenna) , 19.9mA (with noisy antenna)
		case CPU_FREQ_4MHZ :   // Using RC osc. because CCLKCFG cannot take even values
		{            
            wake_vars.current_freq = CPU_FREQ_4MHZ;
            
			CLKSRCSEL = 0 ;    // switch RC 4MHz oscillator
			CCLKCFG   = 0 ;    // CCLK = PLL/1									 			
		}
		break;
        
        //13.1mA (no antenna) , 20.3mA (with noisy antenna)
		case CPU_FREQ_6MHZ :
		{
            wake_vars.current_freq = CPU_FREQ_6MHZ;
            
			CCLKCFG = 1 ;      // CCLK = PLL/2			
		}
		break;
		
        //14.3mA (no antenna) , 21.6mA (with noisy antenna)
		case CPU_FREQ_12MHZ:
		{
            wake_vars.current_freq = CPU_FREQ_12MHZ;
            
			CCLKCFG = 0 ;      // CCLK = PLL/1			
		}
		break;
		
		default : break ;
	}			
}

/*--------------------------------------------------------------------------*/

void wake_cpu_wake1( void )
{
    __asm
    {
        bl crt0_clock_setup
        bl PLL_Lock_Wait
        bl crt0_pll_connect
    };    
}

/*--------------------------------------------------------------------------*/

void wake_state_trace1( void )
{
    #if DBG_WAKE == 1
    {
        static int timeout_trace = 0;
    
        if( ++timeout_trace == 500 ) //--- trace the wake_state on every 5 seconds
        {                   
            dbg_tx_text("\n\r[wake_state1");
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

void wake_state_test1( void )
{
    #if WAKE_TEST1 == 1
    
        static Boolean test = FALSE;    
    
        OUT_2(test);
    
        test = !test;
    
    #endif
}

/*--------------------------------------------------------------------------*/


    