/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf1_usr_irq.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf1_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_rf1_short_press_cmd_exec  (void);
static void dei_rf1_long_press_cmd_exec   (void);
static void dei_rf1_short_press_cmd_insert(void);
static void dei_rf1_long_press_cmd_insert (void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//---------------------------------------------------------------------------//

void dei_rf1_usr_irq (void)
{    
	if( dei_rf1_vars.tx_complete )
	{
		dei_rf1_vars.tx_complete = FALSE ; 
			
		TRACE_DEI_RF1("[DEI_RF1_TX :%08X]\n\r",dei_rf1_vars.tx_copy_data);
	}

	if( dei_rf1_vars.rx_complete )
	{		
		TRACE_DEI_RF1("[DEI_RF1 DATA :%08X]\n\r",dei_rf1_vars.rx_data);

		if( dei_rf1_vars.rx_prg )
		{
			if( dei_rf1_rx_hhu_add() )
			{
				TRACE_DEI_RF1("[DEI_RF1_HHU_ADD]\n\r");

				dei_rf1_nvfs_update();

				dei_rf1_notify_car_finder();
			}
			else
			{
				TRACE_DEI_RF1("[DEI_RF1_HHU_EXIST]\n\r");
			}
		}
		else
		{
			if( dei_rf1_rx_hhu_check() )
			{
				if( (dei_rf1_vars.rx_data&0x000000FF)==DEI_RF1_RX_SHORT )
				{
					dei_rf1_short_press_cmd_exec();
				}
				
				if( (dei_rf1_vars.rx_data&0x000000FF)==DEI_RF1_RX_LONG )
				{
					dei_rf1_long_press_cmd_exec();
				}
								
				TRACE_DEI_RF1("[DEI_RF1_HHU_FOUND]\n\r");
			}
			else
			{
				TRACE_DEI_RF1("[DEI_RF1_HHU_NOT_FOUND]\n\r");
			}
		}
		
		dei_rf1_vars.rx_complete  = FALSE ; 
		dei_rf1_vars.rx_data = 0; 
		dei_rf1_vars.rx_idx  = 0;

		dei_rf1_ext_enable_rising();
	}
} 


//---------------------------------------------------------------------------//

void dei_rf1_short_press_cmd_exec(void)
{
	Exec_Q_Msg *exec_q_msg = NULL; 
     
    exec_q_msg = exec_q_remove( dei_rf1_vars.exec_q_free );
		
	if( exec_q_msg )
	{
		exec_q_insert( exec_q_msg , (Exec_Func)dei_rf1_short_press_cmd_insert );
	}
}

//---------------------------------------------------------------------------//

void dei_rf1_long_press_cmd_exec(void)
{
	Exec_Q_Msg *exec_q_msg = NULL; 
     
    exec_q_msg = exec_q_remove( dei_rf1_vars.exec_q_free );
		
	if( exec_q_msg )
	{
		exec_q_insert( exec_q_msg , (Exec_Func)dei_rf1_long_press_cmd_insert );
	}
}

//---------------------------------------------------------------------------//

void dei_rf1_short_press_cmd_insert(void)
{
	Cmd_Rf_Pld rf_pld  ; 

	rf_pld.bits.id       = dei_rf1_vars.hhu_id_used ;
	rf_pld.bits.batt_low = FALSE ; 
	rf_pld.bits.pld      = 0 ;


	cmd_q_rs_insert( INPUT_RF_ONE_BUTTON_FUNCTION , DEI_RF1_CMD_TIMEOUT , rf_pld );

	TRACE_DEI_RF1("[DEI_RF1_SHORT_PRESS]\n\r");
}

//---------------------------------------------------------------------------//

void dei_rf1_long_press_cmd_insert(void)
{
	Cmd_Rf_Pld rf_pld  ; 

	rf_pld.bits.id       = dei_rf1_vars.hhu_id_used ;
	rf_pld.bits.batt_low = FALSE ; 
	rf_pld.bits.pld      = 0 ;


	cmd_q_rs_insert( INPUT_RF_STOP , DEI_RF1_CMD_TIMEOUT , rf_pld );

	TRACE_DEI_RF1("[DEI_RF1_LONG_PRESS]\n\r");
}

//---------------------------------------------------------------------------//
