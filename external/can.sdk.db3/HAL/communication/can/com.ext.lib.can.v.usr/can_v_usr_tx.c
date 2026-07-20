/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_usr_tx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_usr_private.h"

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

Boolean can_v_usr_tx (Can_Dev_Id can_dev_id , Can_Msg * can_msg , UInt32 delay)
{   
    if (delay & ~SW_WAKE_BIT)
    {
        Can_Tx_Q_Msg * can_tx_q_msg = (Can_Tx_Q_Msg *) q_remove_c (can_tx_q_free [can_dev_id]) ;
    
        if (can_tx_q_msg)
        {
            can_tx_q_msg->can_msg = *can_msg ;
            can_tx_q_msg->delay   = delay    ;
    
            q_insert (can_tx_q_pending [can_dev_id], (Q_Msg *) can_tx_q_msg) ;
        }
        else
        {   
		//do not want to report error of Tx since the error is fixed during programmation...fix test
           // can_v_usr_err_add (CAN_V_USR_ERR_TX_Q_NO_FREE, can_dev_id) ;

          //  return FALSE ;        
        }
    }
    else
    {
        if (pin_map [CAN_DEV_0] == CAN_DEV_0_P0_21_22)
          {
            can_v_usr_sw_mode (can_v_usr_check_sw_wake_bit (delay)) ;
          }
               
    // The 'tx_res' resource is only requested and released here, nowhere else in the library.        
        os_res_request (can_v_usr_tx_res[can_dev_id], 0) ;
        
        if (!can_v_usr_tx_one_shot (can_dev_tx [can_dev_id], can_msg))
        {
			//do not want to report error of Tx since the error is fixed during programmation...fix test
            //can_v_usr_err_add (CAN_V_USR_ERR_TX_TIMEOUT, can_dev_id) ;
        }
        
        os_res_release (can_v_usr_tx_res[can_dev_id]) ;

        if (pin_map [CAN_DEV_0] == CAN_DEV_0_P0_21_22)
            can_v_usr_sw_mode (SWCAN_NORMAL_MODE) ;
    }
  return TRUE ;
}

//--------------------------------------------------------------------------//

Boolean can_v_usr_tx_one_shot (Can_Tx can_dev_tx, Can_Msg * can_msg)
{
    UInt8 i ;

    SInt8 return_status = 0 ;
    
    // A virtual watch here should not be needed here. 
 
    // However, we could still place the virtual watch here to protect against future programming mistakes.

    //os_res_request (can_v_usr_tx_res, 0) ;
  
    {
        for (i = 0 ; i < 50 ; i ++)
        {
            switch (can_tx_one_shot (can_dev_tx, can_msg, CANV_TX_TIMEOUT)) 
            {
                case CAN_TX_OK               :         return_status = 1  ; break ;
                case CAN_TX_BUSY             : i = 0 ; return_status = 0  ; break ;
                case CAN_TX_BUS_ERROR        :         return_status = -1 ; break ;
                case CAN_TX_LOST_ARBITRATION :         return_status = 0  ; break ;
                case CAN_TX_TIMEOUT          :         return_status = -1 ; break ; 
            }
            if (return_status != 0)
                break ;
        }
    }
    //os_res_release (can_v_usr_tx_res) ;

    if (return_status == 1)
        return TRUE ;
    else
        return FALSE ;
}

//--------------------------------------------------------------------------//

SWCAN_Mode can_v_usr_check_sw_wake_bit (UInt32 value)
{
    if (value & SW_WAKE_BIT)
    {
        return SWCAN_WAKE_UP_MODE ;
    }
    else
    {
        return SWCAN_NORMAL_MODE ;
    }
}

