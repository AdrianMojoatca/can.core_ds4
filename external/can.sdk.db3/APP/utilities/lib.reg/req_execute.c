/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: req_execute.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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
//!        and linked with id key .        

//! \assume Reg_Table is a valid pointer

//! \notes  Public method . 

//! \param  reg       the Reg_Table object
//! \param  id        the key for what we want to execute the functions

//! \return \b Boolean \b TRUE : if at least one executed function is returning TRUE
//!                    \b FALSE: if all the executed functions returning FALSE
//!                              or the ID key is not found                                                         
//--------------------------------------------------------------------------//


Boolean reg_execute( Reg_Table reg  , Reg_Id id , Reg_Arg arg )
{
    Reg_Table_Id table_id = NULL ;
    
    Boolean      ret = FALSE ;

    if( reg_atomic_request( reg ) )
    {                
        table_id = reg_check_id( reg , id );

        if( table_id != NULL )
        {            
            Reg_Obj_With_Data curr = table_id->next_reg_obj ;
            
            while( curr )
            {
                if( curr->func )
                {
                    ret |= (curr->func)(arg,curr->arg);
                }
                
                curr = curr->next ;
            }
        }
                           
        reg_atomic_release( reg );
    }
    
    return !!table_id && ret;                   
}

//--------------------------------------------------------------------------//

