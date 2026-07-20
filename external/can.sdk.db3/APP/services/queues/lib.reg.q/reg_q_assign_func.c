/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: reg_q_assign_func.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "reg_q_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void reg_q_assign_table_mode_in_flash(Reg_Q_Table table,void * const entry,UInt8 size,Reg_Q_Flash_Entry_Type type );
void reg_q_assign_table_mode_in_ram  (Reg_Q_Table table,void * const entry,UInt8 size,Reg_Q_Flash_Entry_Type type , Reg_Q_Exec_Mode mode );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


//--------------------------------------------------------------------------//

void reg_q_assign_func( Reg_Q_Table reg_q_table  , Reg_Id id , Reg_Func func , Reg_Q_Exec_Mode mode )
{
    reg_func_arg( (Reg_Table)reg_q_table , id , func , (Reg_Own_Arg) mode );           
}

//--------------------------------------------------------------------------//

void reg_q_assing_table1(Reg_Q_Table table , Reg_Q_Const_Entry1* const entry , UInt8 size,Reg_Q_Exec_Mode mode)
{    
    reg_q_assign_table_mode_in_ram(table,entry,size,REQ_Q_FLASH_ENTRY_TYPE1,mode);
}

//--------------------------------------------------------------------------//

void reg_q_assing_table2(Reg_Q_Table table , Reg_Q_Const_Entry2* const entry , UInt8 size)
{        
    reg_q_assign_table_mode_in_flash(table,entry,size,REQ_Q_FLASH_ENTRY_TYPE2);
}

//--------------------------------------------------------------------------//

void reg_q_assign_table_mode_in_ram( Reg_Q_Table table , void * const entry , UInt8 size , Reg_Q_Flash_Entry_Type type , Reg_Q_Exec_Mode mode )
{
    if( reg_atomic_request((Reg_Table)table) )
    {
        Reg_Q_Ram_Entry1 *ram_entry = (Reg_Q_Ram_Entry1*)os_mem_request(sizeof(struct s_Reg_Q_Ram_Entry1));

        if( ram_entry )
        {
            ram_entry->flash_entry = entry;
            ram_entry->type        = type ;
            ram_entry->size        = size ;
            ram_entry->mode        = mode ;   


            ram_entry->next  = table->ram_entry;
            table->ram_entry = ram_entry;
        }

        reg_atomic_release((Reg_Table)table);
    }
}

//--------------------------------------------------------------------------//

void reg_q_assign_table_mode_in_flash(Reg_Q_Table table , void * const entry , UInt8 size , Reg_Q_Flash_Entry_Type type )
{
    if( reg_atomic_request((Reg_Table)table) )
    {
        Reg_Q_Ram_Entry2 *ram_entry = (Reg_Q_Ram_Entry2*)os_mem_request(sizeof(struct s_Reg_Q_Ram_Entry2));

        if( ram_entry )
        {
            ram_entry->flash_entry = entry;
            ram_entry->type        = type ;
            ram_entry->size        = size ;            

            ram_entry->next  = table->ram_entry;
            table->ram_entry = ram_entry;
        }

        reg_atomic_release((Reg_Table)table);
    }
}

//--------------------------------------------------------------------------//



