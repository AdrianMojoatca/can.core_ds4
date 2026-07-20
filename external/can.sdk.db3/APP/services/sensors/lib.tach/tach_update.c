/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tach_update.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "tach_private.h"
#include "status.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean tach_update (UInt16 value)
{
  if(value != rpm.word)
    {
      timeout_stop(tach_vars.timeout);
    
      rpm.word = value;  
    
      tach_update_timeout(NULL);
    
      #if DBG_TACH == 1
        tach_dbg_print("\nTach Update Value=  ", &rpm.byte[1], 1);
        tach_dbg_print(""                      , &rpm.byte[0], 1);
	  #endif
    }
 
  return tach_vars.is_enabled ;  
}

//--------------------------------------------------------------------------//
void tach_update_timeout ( void * arg)
{	
	if (tach_vars.is_enabled)
	{
      if (rpm.word > 250)  // below 250, the value of the PWM would not be in the range of the Responder LE
    		{
    			UInt32 value = 540000000 / 8000 ;	   // prescaler of 1000
    			
          if (rpm.word <= 8000)
    			{
             value = 540000000 / rpm.word ;
    			}
    
    			PWM1MR0  = value     ;	// 60 ms period @ 1000 rpm
					#ifdef DEV_LPC_236x
						PWM1MR2  = value / 2 ;	// 50% duty cycle
						PWM1LER |= 0x05      ;	// Latch the register in shadow register
					#else
						PWM1MR3  = value / 2 ;
						PWM1LER |= 0x09      ;	// Latch the register in shadow register
					#endif
    		}
    		else
    		{
    			PWM1MR0  = 0x01  ;	// PWM RATE is lower than MATCH VALUE	
					#ifdef DEV_LPC_236x
						PWM1MR2  = 0x02  ;	// OUTPUT REMAINS HIGH
						PWM1LER |= 0x05  ;	// Latch the register in shadow register
					#else
						PWM1MR3 = 0x02;
						PWM1LER |= 0x09  ;	// Latch the register in shadow register
					#endif
    		}

     timeout_start(tach_vars.timeout);

     #if DBG_TACH == 1
       tach_dbg_print("\nTach Timeout Value= ", &rpm.byte[1], 1);
       tach_dbg_print(""                      , &rpm.byte[0], 1);
     #endif
   }
}
