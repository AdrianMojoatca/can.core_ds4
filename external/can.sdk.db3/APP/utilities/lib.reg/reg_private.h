/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: reg_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __REG_PRIVATE_H__
#define __REG_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "reg.h"
#include "stdarg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef struct s_Reg_Obj_With_Data
{
    struct s_Reg_Obj_With_Data *next; 
        
    Reg_Func func    ; 
        
    Reg_Own_Arg  arg  ;

}*Reg_Obj_With_Data ;

//--------------------------------------------------------------------------//

typedef struct s_Reg_Obj_Without_Data
{
    struct s_Reg_Obj_Without_Data *next; 
        
    Reg_Func func    ;             

}*Reg_Obj_Without_Data ;


//--------------------------------------------------------------------------//

typedef struct s_Reg_Atomic_Vars
{
    Reg_Atomic  atomic ; 
    
    UInt8       irq_num_grants;
    UInt8       fiq_num_grants; 
    
    Res         res           ;

    int         fiq_state     ;
    int         irq_state     ;

}Reg_Atomic_Vars ; 
    
//--------------------------------------------------------------------------//

typedef struct s_Reg_Table_Id
{
    struct s_Reg_Table_Id*  next_id ;    
    
    void*    next_reg_obj ;

    Reg_Id   id ;

}*Reg_Table_Id ;

//--------------------------------------------------------------------------//

typedef struct s_Reg_Table
{
  Reg_Table_Id      next ;  
  
  Reg_Atomic_Vars   atomic_vars;

}*Reg_Table ;

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Reg_Table_Id reg_check_id   ( Reg_Table  reg   , Reg_Id id  );
Reg_Table_Id reg_create_id  ( Reg_Table  reg   , Reg_Id id  );
Reg_Table    reg_table_build( Reg_Atomic atomic, UInt8 size );
void         reg_atomic_init( Reg_Table reg , Reg_Atomic atomic );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

