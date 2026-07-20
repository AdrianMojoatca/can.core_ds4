/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: fob_ctrl_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

void fob_ctrl_init( void )
{
	if( fob_ctrl_nvfs_init() )
	{	
		fob_ctrl_vars.reg   = reg_table_create( REG_ATOMIC_USE_RES );	

		fob_ctrl_vars.in_progress = FALSE ; 
	
		fob_ctrl_vars.lock_count  = 0   ;
		fob_ctrl_vars.tbp_expired = TRUE;
		 
		fob_ctrl_vars.timeout     = timeout_f_create( fob_ctrl_vars.cfg.timeout_sec*FOB_CTRL_1SEC_BASE_TIME , (Exec_Func)fob_ctrl_reset      );
		fob_ctrl_vars.timeout_tbp = timeout_f_create( fob_ctrl_vars.cfg.timeout_10ms*10                     , (Exec_Func)fob_ctrl_tbp_expired);
		
		cmd_register:
        {
            static const Cmd_Array1 cmd_array[] = 
            {
                { INPUT_OEM_LOCK  , (Exec_Func)fob_ctrl_lock  },
        		{ INPUT_RF_LOCK   , (Exec_Func)fob_ctrl_reset },
        		{ INPUT_RF_UNLOCK , (Exec_Func)fob_ctrl_reset },
        		{ INPUT_OEM_UNLOCK, (Exec_Func)fob_ctrl_reset },
                { INPUT_OEM_DISARM, (Exec_Func)fob_ctrl_reset }
            };

            CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
        }
					
	}
	   
}

//--------------------------------------------------------------------------//

