/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: req_execute_quick.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "reg_private.h"
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
//! \desc  Use this method to execute all the Reg_Func registered in Reg_Table 

//! \assume Reg_Table is a valid pointer

//! \notes  Public method . The atomic sections is not requested in this function.
//!         Use this method when the Reg_Table contain FIQ/IRQ service vectors

//! \param  reg       the Reg_Table object

//! \return \b Boolean \b TRUE : if at least one executed function is returning TRUE
//!                    \b FALSE: if all the executed functions returning FALSE
//!                              
//--------------------------------------------------------------------------//
Boolean reg_execute_quick( Reg_Table reg )
{
    Reg_Obj_With_Data curr = NULL ;
    
    Boolean ret = FALSE ; 

    curr = (!!reg && !!reg->next) ? reg->next->next_reg_obj : NULL;
            
    while( curr )
    {
        if( curr->func )
        {
            ret |= (curr->func)(0,curr->arg);
        }
                
        curr = curr->next ;
    }
    
    return !!curr && ret;                   
}

//--------------------------------------------------------------------------//

