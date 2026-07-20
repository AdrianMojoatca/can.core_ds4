/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_init.c 44458 2016-11-24 18:10:27Z martin.bouchard $
/*==========================================================================*/


/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "hobp_private.h"
#include "cmd.h"
#include "timeout_f.h"
#include "hobp_bus.h"
#include <string.h>


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define RX_Q_SIZE         10

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void init_rx (void) ;
static void init_tx (void) ;
static void init_cmd(void) ;
                              
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

Boolean hobp_init (void)           
{
  memset(&hobp_vars , NULL , sizeof(Hobp_Vars))  ;

  if(hobp_init_nvfs() && hobp_bus_init())
    {
      init_rx    () ;
      init_tx    () ;
      init_cmd   () ;
			
#ifdef HONDA_T6_PTS
			hobp_vars.nvfs_entry.is_pts = TRUE;
			#warning "This is PTS bypass"
#else
			hobp_vars.nvfs_entry.is_pts = FALSE;
			#warning "This is Key bypass"
#endif
      #if DBG_HOBP == 1
        
      //  hobp_dbg_init () ;
        
        hobp_dbg_print("\nHobp Init Done" , NULL , NULL) ;
      #endif
      
      return TRUE ;
      
    }
  // error ?
  return FALSE ;
}

//--------------------------------------------------------------------------//

static void init_rx(void)
{
  hobp_vars.rx_q_pending = hobp_bus_reg(RX_Q_SIZE) ;

  hobp_vars.tsk_rx    = os_tsk_create (hobp_rx_task , TASK_HOBP_RX_PRI , TASK_HOBP_RX_STK , 0 , TASK_HOBP_RX_NAME) ;

  os_tsk_start (hobp_vars.tsk_rx , 0) ;
}
//--------------------------------------------------------------------------//

static void init_tx(void)
{
  hobp_vars.tx_timeout = timeout_f_create(HOBP_TX_TIMEOUT , hobp_tx_timeout)  ;

  if(hobp_vars.nvfs_entry.is_pts)//hobp_get_flag(HOBP_PROG_FLAG_PTS)if(hobp_vars.nvfs_entry.is_pts)
    {
      hobp_msg[HOBP_MSG_START_AUTH].data[0]  = 0x93 ;
//      hobp_msg[HOBP_MSG_IMO_STATUS1].data[0] = 0x59 ;
      hobp_msg[HOBP_MSG_IMO_STATUS2].data[0] = 0x59 ;
    }
}
//--------------------------------------------------------------------------//
static void init_cmd(void)
{
  static const Cmd_Array1 cmd_array[] = 
    {
      {INPUT_GWR                , hobp_gwr_off }, 
      {INPUT_GWR      |INPUT_ON , hobp_gwr_on  }, 
      {INPUT_IGNITION           , hobp_ign_off }, 
      {INPUT_IGNITION |INPUT_ON , hobp_ign_on  }, 
    } ;

  CMD_LUT_INSERT1_RAM(cmd_array,EXEC_MODE_NORMAL);
}
