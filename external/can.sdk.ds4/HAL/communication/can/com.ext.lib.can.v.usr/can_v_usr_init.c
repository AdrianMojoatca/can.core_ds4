/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_usr_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_usr_private.h"
#include "fiq.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void init_dev0 (Can_Tx_Ctrl * can_tx_ctrl) ;
void init_dev1 (Can_Tx_Ctrl * can_tx_ctrl) ;

void add_rx_q_free_item (UInt8 count) ;
void add_tx_q_free_item (Can_Dev_Id can_dev_id , UInt8 count) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_v_usr_init (void)
{
  #if CAN_DEV_0_ENABLE == 0 && CAN_DEV_1_ENABLE == 0
    #warning !!! DELETE CAN_V_USR_INIT FROM TASK_MAIN IF NO CAN IS REQUIRED !!!
  #endif

#if CAN_DEV_0_ENABLE == 1
  Can_Tx_Ctrl * can_tx_ctrl0 = ((Can_Tx_Ctrl *) os_mem_request (sizeof * can_tx_ctrl0)) ;
#endif

#if CAN_DEV_1_ENABLE == 1
  Can_Tx_Ctrl * can_tx_ctrl1 = ((Can_Tx_Ctrl *) os_mem_request (sizeof * can_tx_ctrl1)) ;
#endif

#if CAN_DEV_0_ENABLE == 1 || CAN_DEV_1_ENABLE == 1
  can_rx_q_free    = q_create () ;
  can_v_usr_tx_res = os_res_create () ;
#endif

#if CAN_DEV_0_ENABLE == 1
  init_dev0 (can_tx_ctrl0) ;
#endif

#if CAN_DEV_1_ENABLE == 1
  init_dev1 (can_tx_ctrl1) ;
#endif 
 
#if CAN_DEV_0_ENABLE == 1
  os_tsk_start (tsk_can_tx [CAN_DEV_0], can_tx_ctrl0) ;
#endif 

#if CAN_DEV_1_ENABLE == 1
  os_tsk_start (tsk_can_tx [CAN_DEV_1], can_tx_ctrl1) ;
#endif 

}

/*==========================================================================*/
void init_dev0 (Can_Tx_Ctrl * can_tx_ctrl)
{
  #if CAN_DEV_0_FT_SW == 1

    pin_map [CAN_DEV_0] = CAN_DEV_0_P0_0_1 ;  

    can_dev [CAN_DEV_0] = can_dev_create (CAN_DEV_0, CAN_DEV_0_BRP, CAN_DEV_0_TSEG1, CAN_DEV_0_TSEG2, CAN_DEV_0_SJW, FALSE, CAN_DEV_0_P0_0_1) ;
       
  #else  

    pin_map [CAN_DEV_0] = CAN_DEV_0_P0_21_22 ;
   
    can_dev [CAN_DEV_0] = can_dev_create (CAN_DEV_0, CAN_DEV_0_BRP, CAN_DEV_0_TSEG1, CAN_DEV_0_TSEG2, CAN_DEV_0_SJW, FALSE, CAN_DEV_0_P0_21_22) ;       
  #endif

  init_rx:
  {
      add_rx_q_free_item (CAN_DEV_0_RX_Q_FREE_COUNT) ;      
  }

  init_tx:
  {
      can_tx_q_free    [CAN_DEV_0] = q_create () ;
      can_tx_q_pending [CAN_DEV_0] = q_create () ;
    
      add_tx_q_free_item (CAN_DEV_0, CAN_DEV_0_TX_Q_FREE_COUNT) ;
    
      can_tx_ctrl->can_tx_q_pending = &can_tx_q_pending [CAN_DEV_0] ;
      
      can_dev_tx [CAN_DEV_0] = can_tx_create (can_dev [CAN_DEV_0] , CAN_TX_0) ; // more priority to direct sender       
      can_tx_ctrl->can_tx    = can_tx_create (can_dev [CAN_DEV_0] , CAN_TX_1) ; // less priority to delayed sender      
    
      tsk_can_tx [CAN_DEV_0] = os_tsk_create (can_v_usr_tx_task , TASK_CAN_USR_TX0_PRI , TASK_CAN_USR_TX0_STK , 0 , TASK_CAN_USR_TX0_NAME) ;
  }
}

/*==========================================================================*/
void init_dev1 (Can_Tx_Ctrl * can_tx_ctrl)
{ 

  pin_map [CAN_DEV_1] = CAN_DEV_1_P0_4_5 ; 

  can_dev [CAN_DEV_1] = can_dev_create (CAN_DEV_1, CAN_DEV_1_BRP, CAN_DEV_1_TSEG1, CAN_DEV_1_TSEG2, CAN_DEV_1_SJW, FALSE, CAN_DEV_1_P0_4_5) ;
  
  init_rx:
  {     
      add_rx_q_free_item (CAN_DEV_1_RX_Q_FREE_COUNT) ;  
  }

  init_tx:
  {
      can_tx_q_free    [CAN_DEV_1] = q_create () ;
      can_tx_q_pending [CAN_DEV_1] = q_create () ;
    
      add_tx_q_free_item (CAN_DEV_1, CAN_DEV_1_TX_Q_FREE_COUNT) ;
    
      can_tx_ctrl->can_tx_q_pending = &can_tx_q_pending [CAN_DEV_1] ;
      
      can_dev_tx [CAN_DEV_1] = can_tx_create (can_dev [CAN_DEV_1] , CAN_TX_0) ; // more priority to direct sender       
      can_tx_ctrl->can_tx    = can_tx_create (can_dev [CAN_DEV_1] , CAN_TX_1) ; // less priority to delayed sender      

      tsk_can_tx [CAN_DEV_1] = os_tsk_create (can_v_usr_tx_task , TASK_CAN_USR_TX1_PRI , TASK_CAN_USR_TX1_STK , 0 , TASK_CAN_USR_TX1_NAME) ;
  }
}

/*==========================================================================*/
void add_rx_q_free_item (UInt8 count)
{
  UInt8 n ;

  for (n = 0 ; n < count ; n++)
    {
      Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Can_Rx_Q_Msg)) ;

      q_assign (can_rx_q_free , q_msg) ;
      q_insert (can_rx_q_free , q_msg) ;
    }
}

/*==========================================================================*/
void add_tx_q_free_item (Can_Dev_Id can_dev_id , UInt8 count)
{
  UInt8 n ;

  for (n = 0 ; n < count ; n++)
    {
      Q_Msg * q_msg = (Q_Msg *) os_mem_request (sizeof (Can_Tx_Q_Msg)) ;

      q_assign (can_tx_q_free [can_dev_id] , q_msg) ;
      q_insert (can_tx_q_free [can_dev_id] , q_msg) ;
    }   
}

