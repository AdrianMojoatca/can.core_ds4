/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: reg_atomic_request.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "reg_private.h"
#include "isr.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void    fiq_request( Reg_Table reg );
static void    irq_request( Reg_Table reg );
static Boolean res_request( Reg_Table reg );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc  Request the access to atomic section
//!        

//! \assume -none- 

//! \notes  Blocking call if the  REG_ATOMIC_USE_RES is used

//! \param  reg       the Reg_Table object


//! \return \b Boolean \b TRUE  if the atmomic section was successfully requested
//!                    \b FALSE if we failed to gain access( not used yet , added for further use ) 
//--------------------------------------------------------------------------//


Boolean reg_atomic_request( Reg_Table reg )
{
    Boolean ret = TRUE  ; 
    
	if( !!(reg->atomic_vars.atomic & REG_ATOMIC_USE_IRQ) ) 
    {
       	irq_request( reg );
    }
    
    if( !!(reg->atomic_vars.atomic & REG_ATOMIC_USE_RES) ) 
    {
       	ret = res_request( reg );
    }
	    
    return ret ; 
}


//--------------------------------------------------------------------------//

void irq_request( Reg_Table reg )
{
  #ifdef DEV_LPC_176x
	IRQ_DIS_GLOBAL;
  #else
  int irq_state = __disable_irq () ;

    if (reg->atomic_vars.irq_num_grants++ == 0)
    {
        reg->atomic_vars.irq_state = irq_state ;
    }
  #endif
}

//--------------------------------------------------------------------------//

Boolean res_request( Reg_Table reg )
{
    if( os_irq_executing )
    {
        return os_res_request_i( reg->atomic_vars.res ) ;
    }
        
    return os_res_request( reg->atomic_vars.res , 0 );
}

//--------------------------------------------------------------------------//

