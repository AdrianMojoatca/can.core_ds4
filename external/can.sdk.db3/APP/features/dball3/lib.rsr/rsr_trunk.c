/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_trunk.c 24739 2014-03-10 12:45:07Z louis-philippe.rispoli $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"

void gpio_iface_rsr_trunk_on(void);
void gpio_iface_rsr_trunk_off(void);

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

void rsr_trunk (void * arg)
{
  #if RSR_DISARM_ENA == 1 && RSR_DISARM_OUTPUT != 3
    rsr_out_action (RSR_DISARM_OUT, 0, rsr_opt.unlock.dur, RSR_OUT_PAUSE, 1) ;
  #endif

  #if RSR_TRUNK_ENA == 1
  rsr_out_action (RSR_TRUNK_OUT, (rsr_opt.unlock.dur+RSR_OUT_PAUSE), TRUNK_DURATION, RSR_OUT_PAUSE, 1) ;
  #endif

  #if RSR_DL_PARKING_FLASH == 1
	rsr_parking_flash(RSR_PARK_FCT_LENGHT, 3) ;
  #endif    
}

//--------------------------------------------------------------------------//

void rsr_trunk_timeout (void * arg)
{
  #if RSR_TRUNK_ENA == 1	
	switch (rsr_out_ctl(RSR_TRUNK_OUT))
    {
  case RSR_OUT_STATE_SET : gpio_iface_rsr_trunk_on(); break ;
    case RSR_OUT_STATE_CLR : gpio_iface_rsr_trunk_off(); break ;
		default: break ;
	}
  #endif	 
}
