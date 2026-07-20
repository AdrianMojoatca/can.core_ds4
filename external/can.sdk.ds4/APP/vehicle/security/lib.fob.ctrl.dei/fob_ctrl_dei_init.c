/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: fob_ctrl_dei_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "fob_ctrl.h"
#include "dei_feature.h"
#include "cmd.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
#ifdef PLATFORM_CM800
static UInt8 fob_ctrl_dei_chirp  ;
#endif
Boolean fob_ctrl_dei_rs;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static void fob_ctrl_dei_chirp_disable( void );
static void fob_ctrl_dei_chirp_restore( void );
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void fob_ctrl_dei_init( void )
{
	fob_ctrl_init();

	if(fob_ctrl_is_enable())
	{ 
		fob_ctrl_callback( FOB_CTRL_ENTER , fob_ctrl_dei_chirp_disable );
		fob_ctrl_callback( FOB_CTRL_LEAVE , fob_ctrl_dei_chirp_restore );
		fob_ctrl_callback( FOB_CTRL_RS    , fob_ctrl_dei_rs_toggle     );	
	}
	
	fob_ctrl_dei_rs = FALSE;
}

//--------------------------------------------------------------------------//

void fob_ctrl_dei_chirp_disable( void )
{
#ifdef PLATFORM_CM800  
	fob_ctrl_dei_chirp = dei_feature_get( DEI_FEAT_CONFIRMATION_CHIRPS );
	dei_feature_set( DEI_FEAT_CONFIRMATION_CHIRPS , DEI_CONFIRMATION_CHIRPS_OFF_WARNING_ON );		
#endif  
}

//--------------------------------------------------------------------------//

void fob_ctrl_dei_chirp_restore( void )
{
#ifdef PLATFORM_CM800  
	dei_feature_set( DEI_FEAT_CONFIRMATION_CHIRPS , fob_ctrl_dei_chirp );		
#endif  
}

//--------------------------------------------------------------------------//

void fob_ctrl_dei_rs_toggle( void )
{
	fob_ctrl_dei_rs = TRUE; // set flag showing this comes from fob (or d2d) meaning bypass activation pulse count
	cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , NULL );
}

//--------------------------------------------------------------------------//

void fob_ctrl_dei_rs_status_set( Boolean status )
{
	fob_ctrl_dei_rs = status;
}

//--------------------------------------------------------------------------//

Boolean fob_ctrl_dei_rs_status_get( void )
{
	return fob_ctrl_dei_rs ;
}
