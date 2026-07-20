/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_usr_init_opt.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_usr_private.h"
#include "wdog_v.h"
#include "gpio.h"

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

static Boolean rx_free_q_ok (void);
static Boolean tx_free_q_ok (void);


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_v_usr_init_opt (Can_V_Init_Options * options, Boolean single_wire)
{
  static Can_Tx_Ctrl * can_tx_ctrl ;

  #define OP(z) options->can_bit_def->z

  if(options->can_dev_id == CAN_DEV_0)
    {
      can_tx_ctrl                  = ((Can_Tx_Ctrl *) os_mem_request (sizeof * can_tx_ctrl)) ;

      pin_map[options->can_dev_id] = (single_wire? CAN_DEV_0_P0_21_22 : CAN_DEV_0_P0_0_1) ;
			
			#ifdef DEV_LPC_176x
			if( single_wire)
			{
				
				gpio_pl_set(GPIO_PL_MODE0);
				gpio_pl_set(GPIO_PL_MODE1);
				gpio_pl_direction(GPIO_PL_J1850_TX_NU);
				gpio_pl_direction(GPIO_PL_CAN_SW_TX);
			}
			#endif
    }
  else if (options->can_dev_id == CAN_DEV_1)
    {
      can_tx_ctrl                  = ((Can_Tx_Ctrl *) os_mem_request (sizeof * can_tx_ctrl)) ; 
      
      pin_map[options->can_dev_id] = CAN_DEV_1_P0_4_5 ;
    }
  else 
    {
      return ;
    }

  can_dev [options->can_dev_id] = can_dev_create (options->can_dev_id, OP(brg), OP (tseg_1) , OP (tseg_2) , OP (sjw) , FALSE , pin_map[options->can_dev_id]) ;

  /*init_rx:*/
  {     
      static Boolean do_first = TRUE ;
			static UInt8 rx_q_size = 0;

      if(do_first)
        {
          do_first         = FALSE ;
          can_rx_q_free    =      q_create () ;
		  //can_v_usr_tx_res = os_res_create () ;
					add_rx_q_free_item (options->rx_size) ;
					rx_q_size				= options->rx_size;
        }
				else if(options->rx_size > rx_q_size)
				{
					add_rx_q_free_item (options->rx_size - rx_q_size) ;
					rx_q_size				= options->rx_size;
				}
				

  }

  /*init_tx:*/
  {
      static Boolean do_first[CAN_DEV_COUNT] = {TRUE, TRUE} ;
			static UInt8 tx_q_size[CAN_DEV_COUNT] = {0, 0};

			if(do_first[options->can_dev_id])
			{
				do_first[options->can_dev_id] = FALSE;
				can_v_usr_tx_res [options->can_dev_id] = os_res_create () ;
				can_tx_q_free    [options->can_dev_id] = q_create () ;
				can_tx_q_pending [options->can_dev_id] = q_create () ;
			
				add_tx_q_free_item (options->can_dev_id, options->tx_size) ;
				tx_q_size[options->can_dev_id] = options->tx_size;
			}
			else if(options->tx_size > tx_q_size[options->can_dev_id])
			{
				add_tx_q_free_item (options->can_dev_id, options->tx_size - tx_q_size[options->can_dev_id]) ;
				tx_q_size[options->can_dev_id] = options->tx_size;
			}
			
      can_tx_ctrl->can_tx_q_pending    = &can_tx_q_pending [options->can_dev_id] ;
      
      can_dev_tx [options->can_dev_id] = can_tx_create (can_dev [options->can_dev_id] , CAN_TX_0) ; // more priority to direct sender       
      can_tx_ctrl->can_tx              = can_tx_create (can_dev [options->can_dev_id] , CAN_TX_1) ; // less priority to delayed sender      

      tsk_can_tx [options->can_dev_id] = os_tsk_create (can_v_usr_tx_task , TASK_CAN_USR_TX1_PRI , TASK_CAN_USR_TX1_STK , 0 , TASK_CAN_USR_TX1_NAME) ;
  }

  wdog_v_register_queue_check(rx_free_q_ok, "CANRX");
  wdog_v_register_queue_check(tx_free_q_ok, "CANTX");

  os_tsk_start (tsk_can_tx [options->can_dev_id], can_tx_ctrl) ;


  #undef OP
}

//--------------------------------------------------------------------------// 

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


// This will be called by wdog_v to check if a free q still has available free messages;
// A free q should never be empty - that can only happen if q_return() is not used properly.
Boolean rx_free_q_ok (void)
{	
	return !q_empty(can_rx_q_free);
}

Boolean tx_free_q_ok (void)
{	
	int dev;

	for (dev=0; dev<CAN_DEV_COUNT; dev++)
	{
		if (can_tx_q_free[dev])
		{
			if (q_empty(can_tx_q_free[dev])) return FALSE;
		}
	}

	return TRUE;
}



