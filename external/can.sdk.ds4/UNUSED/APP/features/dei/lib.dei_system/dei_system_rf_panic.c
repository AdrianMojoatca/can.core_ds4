/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_panic.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Vrtc	  dei_system_panic_vrtc; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//


void dei_system_panic_init( void  )
{	
	dei_system_panic_vrtc = vrtc_f_create( (Exec_Func)dei_system_panic_stop );	    
}

//--------------------------------------------------------------------------//

void dei_system_rf_panic( void *remote )
{	
	if( get_ss_trigger()  )
	{
		goto panic_not_available ;
	}
	
	if( dei_system_type == SYS_TYPE_RS )
    {
        goto panic_available ; //--- panic is always available in RS only systems
    }
    
    if( dei_feature_get( DEI_FEAT_PANIC_MODE )== 1 )//panic mode 1
	{
		goto panic_available ;
	}

	if( dei_feature_get( DEI_FEAT_PANIC_MODE )== 2 )//panic mode 2
	{	  
		if( get_ign_status() )
		{
			goto panic_not_available ; 
		}
		 
		goto panic_available ;
	}

	if( dei_feature_get( DEI_FEAT_PANIC_MODE )== 3 )//panic mode 3
	{
		goto panic_not_available ; 
	}


	panic_available:
	{
		notify_send( NOTIFY_PANIC ) ; 

		dei_system_panic_start();

		return ;
   }


   panic_not_available:
   {
   		notify_send( NOTIFY_RANGE_CHECK ); 
   }
}


//--------------------------------------------------------------------------//

void dei_system_panic_start( void )
{		
	vrtc_stop  ( dei_system_panic_vrtc ); 	

	vrtc_update( dei_system_panic_vrtc , VRTC_1SEC * dei_feature_get( DEI_FEAT_SIREN_DURATION ) ); 

	set_ss_panic( TRUE );
	
	dei_prk_flash( DEI_PRK_TRIGGER ); 

	dei_siren_chirp( DEI_SIREN_TRIGGER );

    cmd_q_rs_insert( INPUT_PANIC | INPUT_ON , 100 );

	vrtc_start( dei_system_panic_vrtc );
}

//--------------------------------------------------------------------------//

void dei_system_panic_stop( void )
{	
	if( get_ss_panic() )
    {                
		set_ss_panic( FALSE );

        vrtc_stop( dei_system_panic_vrtc ) ;         

        dei_prk_flash  ( DEI_PRK_TRIGGER_STOP );        
        dei_siren_chirp( DEI_SIREN_STOP       );
        
        cmd_q_rs_insert( INPUT_PANIC  , 100 );        
    }
}
//--------------------------------------------------------------------------//	



