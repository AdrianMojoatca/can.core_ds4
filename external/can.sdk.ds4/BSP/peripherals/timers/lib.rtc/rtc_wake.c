/*==========================================================================*/
// $Id: rtc_wake.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "rtc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void rtc_wake_low_speed ( void );
static void rtc_wake_high_speed( void );											
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


//--------------------------------------------------------------------------//

void rtc_wake( Wake_Status status )
{        
	status == s_WAKE ? rtc_wake_high_speed() : rtc_wake_low_speed();	
}

//--------------------------------------------------------------------------//

#if defined CM800_DEI_BRAND &&  defined DEV_LPC_236x

void rtc_wake_low_speed( void )
{	    
    switch( wake_get_current_cpu_freq() )
	{
		case CPU_FREQ_0MHZ:
		{
		
		}
		break;
		
		case CPU_FREQ_1MHZ:
		{						
	    	RTC_PREINT  = 0x0017 ;
	    	RTC_PREFRAC = 0x3500 ; 
			
			/*1s Tick = 0.31250000*/
		}
		break;
    
		case CPU_FREQ_2MHZ:
		{	    
	    	RTC_PREINT  = 0x002F ;
	    	RTC_PREFRAC = 0x6A00 ; 
			
			/*1s Tick = 0.31250000*/			
		}
		break;
    
    	case CPU_FREQ_3MHZ:
		{
	    	RTC_PREINT  = 0x0048 ;
	    	RTC_PREFRAC = 0x1F00 ; 

			/*1s Tick = 0.31250000*/			
		}
		break;

		case CPU_FREQ_4MHZ:
		{
			RTC_PREINT  = 0x0060 ;
	    	RTC_PREFRAC = 0x5400 ; 

			/*1s Tick = 0.31250000*/			
		}
		break;	
    
		case CPU_FREQ_6MHZ:
		{    
	    	RTC_PREINT  = 0x0091 ;
	    	RTC_PREFRAC = 0x3E00 ;
			
			/*1s Tick = 0.31250000*/ 
		}
		break;
    
    	case CPU_FREQ_12MHZ:
		{
	    	RTC_PREINT  = 0x0123 ;
	    	RTC_PREFRAC = 0x7C00 ; 
		}
		break;

		default : break;
	}                   
}

#else

void rtc_wake_low_speed( void )
{	    
#ifdef DEV_LPC_236x
    RTC_PREINT  = 0x0060 ; //--- we running at 4MHZ while sleeping
    RTC_PREFRAC = 0x5400 ;     
#endif
}

#endif


//--------------------------------------------------------------------------//

void rtc_wake_high_speed( void )
{
#ifdef DEV_LPC_236x
	RTC_PREINT  = 0x06DC ;
	RTC_PREFRAC = 0x6800 ; 
#endif
}	

//--------------------------------------------------------------------------//

