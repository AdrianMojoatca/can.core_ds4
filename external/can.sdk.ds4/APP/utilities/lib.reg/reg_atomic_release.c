/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: reg_atomic_release.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
static void  fiq_release( Reg_Table reg );
static void  irq_release( Reg_Table reg );
static void  res_release( Reg_Table reg );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc  Release the atomic section
//!        
//!
//! \assume -none- 
//!
//! \notes  -none-
//!
//! \param  reg       the Reg_Table object
//!
//! \return -none-
//--------------------------------------------------------------------------//

void reg_atomic_release( Reg_Table reg )
{
	Reg_Atomic latomic;
	latomic = reg->atomic_vars.atomic;     //workaround for keil uvison 5
	//if( !!(reg->atomic_vars.atomic & REG_ATOMIC_USE_IRQ) ) 
	if( latomic & REG_ATOMIC_USE_IRQ) 
    {
       	irq_release( reg );
    }
    
    if( !!(reg->atomic_vars.atomic & REG_ATOMIC_USE_RES) ) 
    {
       	res_release( reg );
    }	
}


//--------------------------------------------------------------------------//

void irq_release( Reg_Table reg )
{
  #ifdef DEV_LPC_176x
	IRQ_ENA_GLOBAL;
  #else
	int irqstate;
	UInt8 numgrants;
	irqstate = reg->atomic_vars.irq_state;  //workaround for keil uvison 5
	--reg->atomic_vars.irq_num_grants;
	numgrants=reg->atomic_vars.irq_num_grants;
    //if( (--reg->atomic_vars.irq_num_grants == 0) && !reg->atomic_vars.irq_state )
	if(numgrants == 0 && irqstate == 0)
    {
        __enable_irq () ;
    }
  #endif
}

//--------------------------------------------------------------------------//

void res_release( Reg_Table reg )
{
    if( os_irq_executing )
    {
        os_res_release_i( reg->atomic_vars.res ) ;
    }
    else
    {
        os_res_release ( reg->atomic_vars.res ) ;
    }
}

//--------------------------------------------------------------------------//

