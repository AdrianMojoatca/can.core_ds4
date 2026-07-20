/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_ign.c 28526 2014-11-20 14:15:04Z martin.bouchard $
/*==========================================================================*/


/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_private.h"
#include "wake.h"
#include "hobp_bus.h"

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
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void hobp_ign_on (void * arg)
{
  if(!hobp_vars.ign)
    {
      ATOMIC
        (
          hobp_vars.ign         = TRUE  ;
        )

      wake_enter() ;

      #if DBG_HOBP == 1
        hobp_dbg_print("\nHobp Ign On" , NULL , NULL) ;
      #endif
      
      if(!hobp_log_is_done() || hobp_vars.gwr)
        {         
          hobp_bus_ena(arg) ;

          hobp_msg[HOBP_MSG_START_AUTH].data[0] =  hobp_vars.nvfs_entry.is_pts? 0x93 : 0x63 ;//hobp_get_flag(HOBP_PROG_FLAG_PTS)

          hobp_tx_broadcast(&hobp_msg[HOBP_MSG_START_AUTH] , HOBP_TX_TIMEOUT) ;
					
					serial_process(0);
        }
    }
}
//--------------------------------------------------------------------------//

void hobp_ign_off (void * arg)
{
  if(hobp_vars.ign)
    {
      ATOMIC
        (
          hobp_vars.ign = FALSE ;
        )
     
      //timeout_stop(hobp_vars.tx_timeout) ;

      hobp_bus_dis(arg) ;

      hobp_err_save() ;
      
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHobp Ign Off" , NULL , NULL) ;
      #endif
      





      wake_leave() ;

    }

}


