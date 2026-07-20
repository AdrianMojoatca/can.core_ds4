/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tach_disable.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "tach_private.h"

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

void tach_disable (void)
{   
    CRITICAL ;

  if (tach_vars.is_enabled  && (!tach_vars.flag_ign && !tach_vars.flag_rs_ign))
    {
      timeout_stop(tach_vars.timeout);

        C_ENTER ;               
        {
            PWM1MR0  = 0x02  ;  // need give right signal value to go in low mode below
					#ifdef DEV_LPC_236x
            PWM1MR2  = 0x01  ;  
            PWM1LER |= 0x05  ;  // Latch the register in shadow register
					#else
						PWM1MR3	=	0x01;
            PWM1LER |= 0x09  ;  // Latch the register in shadow register
					#endif

  					PWM1MR0  = 0x01  ;  // MATCH VALUE is 0
					#ifdef DEV_LPC_236x
            PWM1MR2  = 0x00  ;  // OUTPUT REMAINS LOW
            PWM1LER |= 0x05  ;  // Latch the register in shadow register
					#else
						PWM1MR3	=	0x00;
            PWM1LER |= 0x09  ;  // Latch the register in shadow register
					#endif
            tach_vars.is_enabled = FALSE ;
        }
        C_LEAVE ;

      #if DBG_TACH == 1
        tach_dbg_print("\nTach Disable", NULL, NULL);
      #endif
    }   
}


