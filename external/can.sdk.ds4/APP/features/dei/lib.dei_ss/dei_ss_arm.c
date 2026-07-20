/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_arm.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
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

void dei_ss_arm( void *remote )
{		  
	if( get_ss_panic()  )
	{
		dei_siren_chirp( DEI_SIREN_STOP );
		return ;	   //the panic state is handle by system  ; don't change the state
	}

	dei_ss_trigger_stop();
	    
	if( get_ign_status() && !get_rs_gwr()) 	return ; 
		
	dei_ss_arm_actions( DEI_SS_NO_SILENT , remote);
}

//--------------------------------------------------------------------------//

void dei_ss_arm_oem( void *pld )
{
	if( get_ss_panic()  )
	{
		dei_siren_chirp( DEI_SIREN_STOP );
		return ;	   //the panic state is handle by system  ; don't change the state
	}

	dei_ss_trigger_stop();
	
	if( get_ign_status() && !get_rs_gwr()) return ;  	
	
	if(get_ss_arm()==FALSE)
	{
	    dei_ss_arm_actions( DEI_SS_NO_SILENT , NULL);
	} // skip the arm actions if the system is armed
}

//--------------------------------------------------------------------------//

#ifdef PLATFORM_CM800
void dei_ss_lock_oem( void *pld)
{
	if( get_ss_panic()  )  return ;	   //the panic state is handle by system  ; don't change the state

	dei_ss_trigger_stop();
	
	if( get_ign_status() && !get_rs_gwr()) return ;  	

	dei_ss_arm_actions( *cmd_get_pld( pld ) , NULL);
}

#else

void dei_ss_lock_oem( void *pld )
{
	Dei_Ss_Silent pld_val = DEI_SS_NO_SILENT;
	if( get_ss_panic()  )  
	{
		dei_siren_chirp( DEI_SIREN_STOP );
		return ;	   //the panic state is handle by system  ; don't change the state
	}

	dei_ss_trigger_stop();
	
	if( get_ign_status() && !get_rs_gwr()) return ;  	

	pld_val = (Dei_Ss_Silent)*cmd_get_pld( pld );
	
	if( pld_val == DEI_SS_PASSIVE_NO_LOCK )
		pld_val = DEI_SS_SILENT;
	dei_ss_arm_actions( pld_val , NULL);
}

#endif
//--------------------------------------------------------------------------//

