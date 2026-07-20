/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: fob_ctrl_lock.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "fob_ctrl_private.h"
#include "dei_feature.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void fob_ctrl_execute( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void fob_ctrl_lock( void )
{
	if( fob_ctrl_vars.tbp_expired == FALSE )
	{
		TRACE_DEI_FOB("\n\r[FOB_CTRL_TBP_NOT_EXPIRED]\n\r");

		return ;
	}
	
	fob_ctrl_tbp_start();	
	
	if( fob_ctrl_vars.cmd_waiting == FOB_CTRL_WAITING_NULL ) // starting the OEM RS pattern
	{
		TRACE_DEI_FOB("\n\r[FOB_CTRL_START_TIMEOUT]\n\r");
					
		timeout_update( fob_ctrl_vars.timeout , fob_ctrl_vars.cfg.timeout_sec*FOB_CTRL_1SEC_BASE_TIME );
		timeout_start ( fob_ctrl_vars.timeout );
		
		if( dei_feature_get (DEI_FEAT_OEM_RS_ACTIVATION) == 2 )
		{
			// 3x lock start
			fob_ctrl_vars.cmd_waiting = FOB_CTRL_WAITING_LOCK;
			fob_ctrl_vars.lock_count = 1;
		}
		
		if(dei_feature_get (DEI_FEAT_OEM_RS_ACTIVATION) == 3)
		{
			// Lock then Trunk
			fob_ctrl_vars.cmd_waiting = FOB_CTRL_WAITING_TRUNK;

			//reg_execute( fob_ctrl_vars.reg , (Reg_Id)FOB_CTRL_ENTER , NULL );

			//TRACE_DEI_FOB("\n\r[FOB_CTRL_ENTER]\n\r");

		}
		
		if(dei_feature_get (DEI_FEAT_OEM_RS_ACTIVATION) == 4)
		{
			// Lock-Unlock-Lock
			fob_ctrl_vars.cmd_waiting = FOB_CTRL_WAITING_UNLOCK;
		}
	}
	else
	{
		fob_ctrl_vars.lock_count++;
		fob_ctrl_vars.in_progress = TRUE;
		if((dei_feature_get (DEI_FEAT_OEM_RS_ACTIVATION) == 4 ) && (fob_ctrl_vars.cmd_waiting == FOB_CTRL_WAITING_LOCK))
		{
			fob_ctrl_vars.cmd_waiting = FOB_CTRL_WAITING_NULL;
			fob_ctrl_execute();
		}
		else if((dei_feature_get (DEI_FEAT_OEM_RS_ACTIVATION) == 2 ) && (fob_ctrl_vars.cmd_waiting == FOB_CTRL_WAITING_LOCK))
		{
			if( fob_ctrl_vars.lock_count != 3)
			{
				fob_ctrl_vars.cmd_waiting = FOB_CTRL_WAITING_LOCK;
				reg_execute( fob_ctrl_vars.reg , (Reg_Id)FOB_CTRL_ENTER , NULL );

				TRACE_DEI_FOB("\n\r[FOB_CTRL_ENTER]\n\r");
			}
			else
			{
				fob_ctrl_vars.cmd_waiting = FOB_CTRL_WAITING_NULL;
				//fob_ctrl_vars.lock_count = 0;
				fob_ctrl_execute();
			}
		}
	}
}

//--------------------------------------------------------------------------//

void fob_ctrl_unlock( void )
{
	if( fob_ctrl_vars.tbp_expired == FALSE )
	{
		TRACE_DEI_FOB("\n\r[FOB_CTRL_TBP_NOT_EXPIRED]\n\r");

		return ;
	}
	
	fob_ctrl_tbp_start();	
	
	if( (dei_feature_get (DEI_FEAT_OEM_RS_ACTIVATION) == 4 ) && (fob_ctrl_vars.cmd_waiting == FOB_CTRL_WAITING_UNLOCK))
	{
		// Lock-Unlock-Lock
		reg_execute( fob_ctrl_vars.reg , (Reg_Id)FOB_CTRL_ENTER , NULL );

		TRACE_DEI_FOB("\n\r[FOB_CTRL_ENTER]\n\r");
		
		fob_ctrl_vars.in_progress = TRUE;
		fob_ctrl_vars.cmd_waiting = FOB_CTRL_WAITING_LOCK;
	}
	else
	{
		fob_ctrl_reset();
	}

}


//--------------------------------------------------------------------------//

void fob_ctrl_trunk( void )
{
	if( fob_ctrl_vars.tbp_expired == FALSE )
	{
		TRACE_DEI_FOB("\n\r[FOB_CTRL_TBP_NOT_EXPIRED]\n\r");

		return ;
	}
	
	fob_ctrl_tbp_start();	
	
	if( (dei_feature_get (DEI_FEAT_OEM_RS_ACTIVATION) == 3 ) && (fob_ctrl_vars.cmd_waiting == FOB_CTRL_WAITING_TRUNK))
	{
		// Lock-Trunk
		fob_ctrl_vars.cmd_waiting = FOB_CTRL_WAITING_NULL;
		fob_ctrl_execute();
	}
	/*
	else
	{
		fob_ctrl_reset();
	}
	*/
}
//--------------------------------------------------------------------------//

void fob_ctrl_execute( void )
{
		os_tsk_wait(1000); // TP # 6670		
		reg_execute(fob_ctrl_vars.reg , (Reg_Id)FOB_CTRL_RS    , NULL );

		TRACE_DEI_FOB("\n\r[FOB_CTRL_RS]\n\r");

		timeout_update( fob_ctrl_vars.timeout , FOB_CTRL_RESET_TIMEOUT );						
		timeout_start ( fob_ctrl_vars.timeout );

		fob_ctrl_tbp_stop();
}

//--------------------------------------------------------------------------//
