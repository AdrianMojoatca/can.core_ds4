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

//--------------------------------------------------------------------------//

void fob_ctrl_lock( void )
{		
	if( fob_ctrl_vars.tbp_expired == FALSE )
	{
		TRACE_DEI_FOB("\n\r[FOB_CTRL_TBP_NOT_EXPIRED]\n\r");

		return ;
	}
	
	fob_ctrl_tbp_start();	
	
	TRACE_DEI_FOB("\n\r[FOB_CTRL_LOCK %d]\n\r" , fob_ctrl_vars.lock_count );
	
	ATOMIC( fob_ctrl_vars.lock_count ++ ) ; 	

    if( fob_ctrl_vars.lock_count == 1 )
    {
        TRACE_DEI_FOB("\n\r[FOB_CTRL_START_TIMEOUT]\n\r");
		        
        timeout_update( fob_ctrl_vars.timeout , fob_ctrl_vars.cfg.timeout_sec*FOB_CTRL_1SEC_BASE_TIME );
	    timeout_start ( fob_ctrl_vars.timeout );
    }

	if( fob_ctrl_vars.lock_count == 2 )	//first press of lock after normally arm
	{
		reg_execute( fob_ctrl_vars.reg , (Reg_Id)FOB_CTRL_ENTER , NULL );

		TRACE_DEI_FOB("\n\r[FOB_CTRL_ENTER]\n\r");
		
		ATOMIC( fob_ctrl_vars.in_progress = TRUE );	
	}
	
	if( fob_ctrl_vars.lock_count == fob_ctrl_vars.cfg.locks )
	{			
		if( fob_ctrl_vars.in_progress )
		{
			os_tsk_wait(1000) ;  // TP # 6670
			reg_execute(fob_ctrl_vars.reg , (Reg_Id)FOB_CTRL_RS    , NULL );

			TRACE_DEI_FOB("\n\r[FOB_CTRL_RS]\n\r");

			timeout_update( fob_ctrl_vars.timeout , FOB_CTRL_RESET_TIMEOUT );						
			timeout_start ( fob_ctrl_vars.timeout );

			fob_ctrl_tbp_stop();
		}
	}		
}

//--------------------------------------------------------------------------//
