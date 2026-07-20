/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: fob_ctrl_nvfs_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

Boolean fob_ctrl_nvfs_init( void )
{				
	if( nvfs_rd(nvfs_fob_ctrl,&fob_ctrl_vars.cfg) != NVFS_OK )
	{
		TRACE_DEI_FOB("\n\r[FOB_CTRL_NVFS_RD_ERROR]\n\r");

		fob_ctrl_vars.cfg.is_enable 	= FALSE ; 
		fob_ctrl_vars.cfg.locks     	= 3     ; 
		fob_ctrl_vars.cfg.timeout_sec	= FOB_CTRL_TIMEOUT ;
		fob_ctrl_vars.cfg.timeout_10ms  = FOB_CTRL_TIMEOUT_BETWEEN_PRESSES;
		
		if( nvfs_wr_create( nvfs_fob_ctrl , &fob_ctrl_vars.cfg , sizeof( struct s_Fob_Ctrl_Cfg ) ) != NVFS_OK )
		{
			TRACE_DEI_FOB("\n\r[FOB_CTRL_NVFS_WR_CREATE_ERROR]\n\r");								
		}

		nvram_usr_commit();
	}
	else
	{
		TRACE_DEI_FOB("\n\r[FOB_CTRL_NVFS_RD_OK]\n\r");		
	}

    #if FOB_CTRL_ENABLE == 1
        fob_ctrl_vars.cfg.is_enable = TRUE ; 

        #warning !!FOB CONTROL ACTIVATED!!
    #endif

	if( fob_ctrl_vars.cfg.locks       == 0 )fob_ctrl_vars.cfg.locks        = 3; 
	if( fob_ctrl_vars.cfg.timeout_sec == 0 )fob_ctrl_vars.cfg.timeout_sec  = FOB_CTRL_TIMEOUT;
	if( fob_ctrl_vars.cfg.timeout_10ms== 0 )fob_ctrl_vars.cfg.timeout_10ms = FOB_CTRL_TIMEOUT_BETWEEN_PRESSES;


	TRACE_DEI_FOB("\n\r[FOB_CTRL_%s]\n\r",fob_ctrl_vars.cfg.is_enable ? "ENABLE":"DISABLE" );
		
	TRACE_DEI_FOB("\n\r[FOB_CTRL_%d_LOCKS]\n\r",fob_ctrl_vars.cfg.locks );
	
	TRACE_DEI_FOB("\n\r[FOB_CTRL_%ds_TIMEOUT]\n\r",fob_ctrl_vars.cfg.timeout_sec );

	TRACE_DEI_FOB("\n\r[FOB_CTRL_%ds_TBP_TIMEOUT]\n\r",fob_ctrl_vars.cfg.timeout_10ms*10 );
	
	return fob_ctrl_vars.cfg.is_enable ;	
}

//--------------------------------------------------------------------------//
