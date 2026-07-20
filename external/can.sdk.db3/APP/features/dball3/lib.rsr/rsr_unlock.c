/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_unlock.c 32362 2015-06-16 18:52:36Z jean-francois.dube $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"

void gpio_iface_rsr_unlock_on(void);
void gpio_iface_rsr_unlock_off(void);

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define UNLOCK_AFTER_DISARM (rsr_opt.unlock.dur + 300)	// 300ms delay between disarm and unlock
#define UNLOCK_WITH_DISARM	0

#if RSR_DISARM_ENA == 1 && RSR_DISARM_OUTPUT == 2
  #define RSR_UNLOCK_OFFSET   UNLOCK_AFTER_DISARM
#else
  #define RSR_UNLOCK_OFFSET   UNLOCK_WITH_DISARM  
#endif


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/ 

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rsr_unlock (void * arg)
{ 
  #if RSR_DISARM_ENA == 1 && RSR_DISARM_OUTPUT != 3
    rsr_out_action (RSR_DISARM_OUT, 0, rsr_opt.unlock.dur, RSR_OUT_PAUSE, 1) ;
  #endif

  #if RSR_UNLOCK_ENA == 1
    rsr_out_action (RSR_UNLOCK_OUT, RSR_UNLOCK_OFFSET, rsr_opt.unlock.dur, RSR_OUT_PAUSE, rsr_opt.unlock.pulse) ;
  #endif 

  #if RSR_DL_PARKING_FLASH == 1
	rsr_parking_flash(RSR_PARK_FCT_LENGHT, 2) ;
  #endif   
}

//--------------------------------------------------------------------------//

void rsr_unlock_timeout (void * arg)
{
  #if RSR_UNLOCK_ENA == 1
	switch (rsr_out_ctl(RSR_UNLOCK_OUT))
    {
    case RSR_OUT_STATE_SET : gpio_iface_rsr_unlock_on(); break ;
        case RSR_OUT_STATE_CLR : gpio_iface_rsr_unlock_off(); break ;
		  default: break ;
	  }
  #endif	 
}

