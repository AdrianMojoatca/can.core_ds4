/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_usr_tx_task.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_usr_private.h"
#include "wdog_v.h"

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

void can_v_usr_tx_task (Can_Tx_Ctrl * can_tx_ctrl)
{	
	for (;;)
	{
		Can_Tx_Q_Msg * can_tx_q_msg = (Can_Tx_Q_Msg *) q_remove (*can_tx_ctrl->can_tx_q_pending , 0) ;
			        
        //os_tsk_wait ((can_tx_q_msg->delay)& ~SW_WAKE_BIT) ;
		if((can_tx_q_msg->delay))
          {
            os_tsk_wait ((can_tx_q_msg->delay)& ~SW_WAKE_BIT) ;
          }        
        // We try to place a VIRTUAL_WATCH in every task, in every library, to make sure the task does not block
		
		//VIRTUAL_WATCH
		//(
			if (pin_map[CAN_DEV_0] == CAN_DEV_0_P0_21_22)
				can_v_usr_sw_mode (can_v_usr_check_sw_wake_bit (can_tx_q_msg->delay)) ;

			if (!can_v_usr_tx_one_shot (can_tx_ctrl->can_tx , & can_tx_q_msg->can_msg))
			{
				if(*can_tx_ctrl->can_tx_q_pending == can_tx_q_pending[CAN_DEV_0])
				{
					can_v_usr_err_add (CAN_V_USR_ERR_TX_TIMEOUT, CAN_DEV_0) ;
				}
				else
				{
				 	can_v_usr_err_add (CAN_V_USR_ERR_TX_TIMEOUT, CAN_DEV_1) ;
				}
			}
			if (pin_map[CAN_DEV_0] == CAN_DEV_0_P0_21_22)
				can_v_usr_sw_mode (SWCAN_NORMAL_MODE) ;
					
			q_return ((Q_Msg *) can_tx_q_msg) ;
		//);
	}
}
