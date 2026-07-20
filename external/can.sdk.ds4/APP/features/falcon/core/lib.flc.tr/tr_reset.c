/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: out_create.c 1046 2010-04-08 12:51:11Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "tr_private.h"
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

void tr_reset_lock( void )
{
	ATOMIC(tr_lock_status = TRUE);
	
	tr_reset();
}

void tr_reset_unlock(void)
{
	ATOMIC(tr_lock_status = FALSE);
	
	tr_reset();
}

void tr_reset_doors(void)
{
    if( tr_lock_status )
    {
        timeout_q_remove( (Timeout_Q_Func)tr_car_rearm  ) ;
        timeout_q_remove( (Timeout_Q_Func)tr_car_relock ) ;		
        timeout_q_remove( (Timeout_Q_Func)tr_timeout    ) ;
    
        ATOMIC(tr_status     =FALSE);
        ATOMIC(tr_oem        =FALSE); 
        ATOMIC(tr_lock_status=FALSE);

        TRACE("\n\r[TRUNK_RELEASE_TRIGGER_RESET]\n\r");    
    }
}

//--------------------------------------------------------------------------//

void tr_reset( void )
{
    if( tr_status )
    {
        timeout_q_remove( (Timeout_Q_Func)tr_car_rearm  ) ;
        timeout_q_remove( (Timeout_Q_Func)tr_car_relock ) ;		
        timeout_q_remove( (Timeout_Q_Func)tr_timeout    ) ;
    
    	ATOMIC(tr_status=FALSE);
        ATOMIC(tr_oem   =FALSE); 

        TRACE("\n\r[TRUNK_RELEASE_TRIGGER_RESET]\n\r");    
    }
}         

//--------------------------------------------------------------------------//

