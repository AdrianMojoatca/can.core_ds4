/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_lock.c 32362 2015-06-16 18:52:36Z jean-francois.dube $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"

void gpio_iface_rsr_lock_on(void);
void gpio_iface_rsr_lock_off(void);

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define RSR_ARM_OFFSET		1100

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rsr_lock (void * arg)
{ 
  #if RSR_LOCK_ENA == 1
  rsr_out_action (RSR_LOCK_OUT, 0, rsr_opt.lock.dur, RSR_OUT_PAUSE, rsr_opt.lock.pulse) ;
  #endif

  #if RSR_ARM_ENA == 1 
    #if RSR_ARM_OUTPUT == 1
    rsr_out_action (RSR_ARM_OUT, ((rsr_opt.lock.dur+RSR_OUT_PAUSE)*rsr_opt.lock.pulse + RSR_ARM_OFFSET), rsr_opt.lock.dur, RSR_OUT_PAUSE, 1) ;
	#endif
  #endif

  #if RSR_DL_PARKING_FLASH == 1
	rsr_parking_flash(RSR_PARK_FCT_LENGHT, 1) ;
  #endif 
}

//--------------------------------------------------------------------------//

void rsr_lock_timeout (void * arg)
{
  #if RSR_LOCK_ENA == 1	
	switch (rsr_out_ctl(RSR_LOCK_OUT))
    {
    case RSR_OUT_STATE_SET : gpio_iface_rsr_lock_on(); break ;
        case RSR_OUT_STATE_CLR : gpio_iface_rsr_lock_off(); break ;
		  default: break ;
	}
  #endif	 
}

