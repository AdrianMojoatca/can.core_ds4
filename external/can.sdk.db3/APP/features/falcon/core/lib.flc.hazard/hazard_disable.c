/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hazard_disable.c 18031 2012-11-05 10:53:56Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "hazard_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Timeout_Q_Status enable_back( void ) ; 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void hazard_disable_lock( void )
{
    TRACE_HAZARD("\n\r HAZARD_LOCK \n\r");
	
	if( get_rs_gwr() )
    {
        if( hazard_cfg & HAZARD_SKIP_RS_LOCK )
        {
            hazard_temporary_disable( HAZARD_SKIP_RS_LOCK_TIMEOUT );
        }
    }
    else
    {
        if( !!(hazard_cfg & HAZARD_SKIP_LOCK) )
        {
            hazard_temporary_disable( HAZARD_SKIP_LOCK_TIMEOUT );
        }
    }
}

//--------------------------------------------------------------------------//

void hazard_disable_unlock( void )
{
    TRACE_HAZARD("\n\r HAZARD_UNLOCK \n\r");
	
	if( get_rs_gwr() )
    {
        if( hazard_cfg & HAZARD_SKIP_RS_UNLOCK )
        {
            hazard_temporary_disable( HAZARD_SKIP_RS_UNLOCK_TIMEOUT );
        }
    }
    else
    {
        if( !!(hazard_cfg & HAZARD_SKIP_UNLOCK) )
        {
            hazard_temporary_disable( HAZARD_SKIP_UNLOCK_TIMEOUT );
        }
    }
}

//--------------------------------------------------------------------------//

void hazard_temporary_disable( UInt32 timeout )
{
    hazard_enable = FALSE ;

    timeout_q_remove( (void *)enable_back ) ; 
    
    timeout_q_insert( (Timeout_Q_Func)enable_back , NULL , timeout , 1 , 1 ) ;
	
	TRACE_HAZARD("\n\r HAZARD_DISABLE \n\r" ); 
}

//--------------------------------------------------------------------------//

Timeout_Q_Status enable_back( void )
{
    hazard_enable = TRUE ;

	TRACE("\n\r HAZARD_ENABLE \n\r" );

    return TIMEOUT_Q_COMPLETE ;
}

//--------------------------------------------------------------------------//

