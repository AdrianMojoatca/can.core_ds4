/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: reg_q.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __REG_Q_H__
#define __REG_Q_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "reg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef UInt32       Reg_Q_Exec_Mode ;

//--------------------------------------------------------------------------//

typedef struct s_Reg_Q_Table  *Reg_Q_Table ; 

//--------------------------------------------------------------------------//

typedef struct s_Reg_Q_Flash_Entry1
{
    UInt16              id  ; 
    Reg_Func            func;    
}Reg_Q_Const_Entry1;

//--------------------------------------------------------------------------//

typedef struct s_Reg_Q_Flash_Entry2
{
    UInt16              id  ; 
    Reg_Func            func;
    Reg_Q_Exec_Mode     mode;
}Reg_Q_Const_Entry2;

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Reg_Q_Table reg_q_table_create( UInt8 q_free_size );

void 		reg_q_assign_func ( Reg_Q_Table reg_q_table  , Reg_Id id , Reg_Func func , Reg_Q_Exec_Mode mode );

Boolean 	reg_q_exec_func	  ( Reg_Q_Table reg_q_table  , Reg_Id id , Reg_Arg  arg );
Boolean     reg_q_rem_func    ( Reg_Q_Table reg_q_table  , Reg_Id id , Reg_Func func);

void 		reg_q_set_mode	  ( Reg_Q_Table reg_q_table  , Reg_Q_Exec_Mode mode );
void 		reg_q_clr_mode	  ( Reg_Q_Table reg_q_table  , Reg_Q_Exec_Mode mode );


void reg_q_assing_table1(Reg_Q_Table table,Reg_Q_Const_Entry1* const entry,UInt8 size,Reg_Q_Exec_Mode mode);
void reg_q_assing_table2(Reg_Q_Table table,Reg_Q_Const_Entry2* const entry,UInt8 size);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

