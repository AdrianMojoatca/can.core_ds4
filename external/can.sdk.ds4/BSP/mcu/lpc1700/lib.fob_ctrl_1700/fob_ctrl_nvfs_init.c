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
#include "dei_feature.h"
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
	if( dei_feature_get( DEI_FEAT_OEM_RS_ACTIVATION ) != 1 )
	{
		TRACE_DEI_FOB("\n\r[OEM REMOTE START ENABLED]\n\r");								

		fob_ctrl_vars.cfg.is_enable 		= TRUE ; 
		fob_ctrl_vars.rs_pattern 		    = dei_feature_get( DEI_FEAT_OEM_RS_ACTIVATION )  ; 
		fob_ctrl_vars.cfg.timeout_sec		= FOB_CTRL_TIMEOUT ;
		fob_ctrl_vars.cfg.timeout_10ms  = FOB_CTRL_TIMEOUT_BETWEEN_PRESSES;
		
	}
	else
	{
		TRACE_DEI_FOB("\n\r[OEM REMOTE START DISABLED]\n\r");		
		fob_ctrl_vars.cfg.is_enable 		= FALSE ; 
	}

	TRACE_DEI_FOB("\n\r[OEM REMOTE START: %s]\n\r",fob_ctrl_vars.cfg.is_enable ? "ENABLE":"DISABLE" );
		
	TRACE_DEI_FOB("\n\r[OEM REMOTE START PATTERN: %d]\n\r",fob_ctrl_vars.cfg.rs_pattern );
	
	TRACE_DEI_FOB("\n\r[OEM_%ds_TIMEOUT]\n\r",fob_ctrl_vars.cfg.timeout_sec );

	TRACE_DEI_FOB("\n\r[OEM_%ds_TBP_TIMEOUT]\n\r",fob_ctrl_vars.cfg.timeout_10ms*10 );
	
	return fob_ctrl_vars.cfg.is_enable ;	
}

//--------------------------------------------------------------------------//
