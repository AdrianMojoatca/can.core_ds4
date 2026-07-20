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
#include "core_contract_db3_can_config.h"

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
static Boolean can_v_usr_dev0_speed_to_timing(UInt32 speed, UInt32 *brp, UInt32 *tseg1, UInt32 *tseg2, UInt32 *sjw) ;
static Boolean can_v_usr_dev1_speed_to_timing(UInt32 speed, UInt32 *brp, UInt32 *tseg1, UInt32 *tseg2, UInt32 *sjw) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_v_usr_init (void)
{
  Boolean can_dev0_enabled = ((CAN_CFG_CAN_DEV_0_RX_BUFFER_SIZE > 0u) || (CAN_CFG_CAN_DEV_0_TX_BUFFER_SIZE > 0u));
  Boolean can_dev1_enabled = ((CAN_CFG_CAN_DEV_1_RX_BUFFER_SIZE > 0u) || (CAN_CFG_CAN_DEV_1_TX_BUFFER_SIZE > 0u));
  Can_Tx_Ctrl * can_tx_ctrl0 = 0;
  Can_Tx_Ctrl * can_tx_ctrl1 = 0;

  if (!can_dev0_enabled && !can_dev1_enabled)
    {
      return;
    }

  can_rx_q_free    = q_create () ;
  can_v_usr_tx_res = os_res_create () ;

  if (can_dev0_enabled)
    {
      can_tx_ctrl0 = ((Can_Tx_Ctrl *) os_mem_request (sizeof * can_tx_ctrl0)) ;
      init_dev0 (can_tx_ctrl0) ;
      os_tsk_start (tsk_can_tx [CAN_DEV_0], can_tx_ctrl0) ;
    }

  if (can_dev1_enabled)
    {
      can_tx_ctrl1 = ((Can_Tx_Ctrl *) os_mem_request (sizeof * can_tx_ctrl1)) ;
      init_dev1 (can_tx_ctrl1) ;
      os_tsk_start (tsk_can_tx [CAN_DEV_1], can_tx_ctrl1) ;
    }

}

/*==========================================================================*/
void init_dev0 (Can_Tx_Ctrl * can_tx_ctrl)
{
  UInt32 can_dev_0_brp;
  UInt32 can_dev_0_tseg1;
  UInt32 can_dev_0_tseg2;
  UInt32 can_dev_0_sjw;

  if (!can_v_usr_dev0_speed_to_timing(CAN_CFG_CAN_DEV_0_SPEED, &can_dev_0_brp, &can_dev_0_tseg1, &can_dev_0_tseg2, &can_dev_0_sjw))
    {
      return;
    }

  pin_map [CAN_DEV_0] = (CAN_CFG_CAN_DEV_0_FT_SW != 0u) ? CAN_DEV_0_P0_0_1 : CAN_DEV_0_P0_21_22;

  can_dev [CAN_DEV_0] = can_dev_create (CAN_DEV_0, can_dev_0_brp, can_dev_0_tseg1, can_dev_0_tseg2, can_dev_0_sjw, FALSE, pin_map [CAN_DEV_0]) ;

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
  UInt32 can_dev_1_brp;
  UInt32 can_dev_1_tseg1;
  UInt32 can_dev_1_tseg2;
  UInt32 can_dev_1_sjw;

  if (!can_v_usr_dev1_speed_to_timing(CAN_CFG_CAN_DEV_1_SPEED, &can_dev_1_brp, &can_dev_1_tseg1, &can_dev_1_tseg2, &can_dev_1_sjw))
    {
      return;
    }

  pin_map [CAN_DEV_1] = CAN_DEV_1_P0_4_5 ; 

  can_dev [CAN_DEV_1] = can_dev_create (CAN_DEV_1, can_dev_1_brp, can_dev_1_tseg1, can_dev_1_tseg2, can_dev_1_sjw, FALSE, CAN_DEV_1_P0_4_5) ;
  
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

/*==========================================================================*/
static Boolean can_v_usr_dev0_speed_to_timing(UInt32 speed, UInt32 *brp, UInt32 *tseg1, UInt32 *tseg2, UInt32 *sjw)
{
  if ((brp == 0) || (tseg1 == 0) || (tseg2 == 0) || (sjw == 0))
    {
      return FALSE;
    }

  switch (speed)
    {
      case 0u: *brp = 119u + 1u; *sjw = 1u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      case 1u: *brp = 79u + 1u;  *sjw = 1u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      case 2u: *brp = 47u + 1u;  *sjw = 1u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      case 3u: *brp = 39u + 1u;  *sjw = 1u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      case 4u: *brp = 31u + 1u;  *sjw = 1u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      case 5u: *brp = 15u + 1u;  *sjw = 1u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      case 6u: *brp = 7u + 1u;   *sjw = 2u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      default: return FALSE;
    }
}

/*==========================================================================*/
static Boolean can_v_usr_dev1_speed_to_timing(UInt32 speed, UInt32 *brp, UInt32 *tseg1, UInt32 *tseg2, UInt32 *sjw)
{
  if ((brp == 0) || (tseg1 == 0) || (tseg2 == 0) || (sjw == 0))
    {
      return FALSE;
    }

  switch (speed)
    {
      case 0u: *brp = 31u + 1u; *sjw = 1u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      case 1u: *brp = 15u + 1u; *sjw = 1u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      case 2u: *brp = 7u + 1u;  *sjw = 2u + 1u; *tseg1 = 10u + 1u; *tseg2 = 5u + 1u; return TRUE;
      default: return FALSE;
    }
}

