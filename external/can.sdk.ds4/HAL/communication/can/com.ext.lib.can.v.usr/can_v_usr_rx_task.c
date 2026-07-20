/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_usr_rx_task.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_usr_private.h"
#include "wake.h"
#include "exec.h"
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

void can_v_usr_rx_task (Can_V_Usr * arg)
{   
    Boolean first_after_wake = TRUE ;

    for (;;)
    {
        Can_Msg can_msg ;

        if(can_do_err_save)
          {
            if(!can_v_usr_err_save ())
              {
                // fail to save what's happen display some error?????
              }

            can_do_err_save = FALSE ;
          }
        
    
        if (first_after_wake)
        {
            can_v_rx (arg->can_v_rx , & can_msg , 0) ;
            wake_enter_flagged(WAKE_CAN_V_USR) ;
            first_after_wake = FALSE ;
        }
        else
        {
            if (! can_v_rx (arg->can_v_rx , & can_msg , 10000))
            {   
                first_after_wake = TRUE ;
                wake_leave_flagged(WAKE_CAN_V_USR) ;
                continue ;
            }
        }
        
        /*can_msg_process:*/
        {
            Can_Rx_Q_Msg * can_rx_q_msg = (Can_Rx_Q_Msg *) q_remove_c (can_rx_q_free) ;

            // We try to place a VIRTUAL_WATCH in every task, in every library, to make sure the task does not block
                if (can_rx_q_msg)
                {   
                    //VIRTUAL_WATCH
                    //(
                        can_rx_q_msg->can_msg = can_msg ;
        
                        exec_q_insert ((Exec_Q_Msg *) can_rx_q_msg, arg->can_lut_sw_table [can_msg.lut_index_rx].index_func );
                    //);
                }
                else
                {
                    can_v_usr_err_add (CAN_V_USR_ERR_RX_Q_NO_FREE, CAN_DEV_0) ; // using dev0 by default as we don't know
                }
        }    
    }
}
