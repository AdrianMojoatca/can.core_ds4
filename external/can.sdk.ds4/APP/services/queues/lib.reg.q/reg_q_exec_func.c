/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: reg_q_exec_func.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

void reg_q_exec_table(Reg_Q_Table reg_q_table , Reg_Id id , Reg_Arg arg);

void reg_q_exec_table_type1( Reg_Q_Table reg_q_table , Reg_Q_Ram_Entry1*ram_entry, Reg_Id id , Reg_Arg arg);
void reg_q_exec_table_type2( Reg_Q_Table reg_q_table , Reg_Q_Ram_Entry2*ram_entry, Reg_Id id , Reg_Arg arg);

Boolean reg_q_insert_func(Reg_Q_Table reg_q_table,Reg_Func func,Reg_Id id,Reg_Arg arg );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


//--------------------------------------------------------------------------//

Boolean reg_q_exec_func( Reg_Q_Table reg_q_table,Reg_Id id,Reg_Arg arg )
{
    Reg_Table_Id        table_id = NULL;     
    
    if( reg_atomic_request( (Reg_Table)reg_q_table ) )
	{	
		reg_q_exec_table(reg_q_table,id,arg);
        
        table_id = reg_check_id( (Reg_Table)reg_q_table , id);
        
        if( table_id != NULL )
        {
            Reg_Obj_With_Data curr = table_id->next_reg_obj ;
            
            while( curr )
            {                 
                if( !!(((Reg_Q_Exec_Mode)curr->arg) & reg_q_table->mode) )
			    {
				    reg_q_insert_func(reg_q_table,curr->func,id,arg);				    
			    }

                curr = curr->next ;
	        }
        }

        reg_atomic_release( (Reg_Table)reg_q_table );
	}

    return !!table_id ;
}

//--------------------------------------------------------------------------//

void reg_q_exec_table(Reg_Q_Table reg_q_table , Reg_Id id , Reg_Arg arg)
{
    Reg_Q_Ram_Entry1*curr = reg_q_table->ram_entry;    
    
    while( curr )
    {
        switch( curr->type )
        {
            case REQ_Q_FLASH_ENTRY_TYPE1 : reg_q_exec_table_type1(reg_q_table,(Reg_Q_Ram_Entry1*)curr,id,arg);break;	
            case REQ_Q_FLASH_ENTRY_TYPE2 : reg_q_exec_table_type2(reg_q_table,(Reg_Q_Ram_Entry2*)curr,id,arg);break;	
            
            default : break;
        }

        curr = curr->next;
    }   
        
}		

//--------------------------------------------------------------------------//

void reg_q_exec_table_type1( Reg_Q_Table reg_q_table , Reg_Q_Ram_Entry1 *ram_entry, Reg_Id id , Reg_Arg arg)
{
    UInt8 i = 0;
    
    Reg_Q_Const_Entry1 const *flash_entry_table = (Reg_Q_Const_Entry1*)ram_entry->flash_entry;

    #define FLASH_ENTRY(i)  (&(flash_entry_table[i]))
        
    for(i=0;i<ram_entry->size;i++)
    {                
        if((FLASH_ENTRY(i)->id==id) && (!!((ram_entry->mode) & reg_q_table->mode)) )        
        {
            reg_q_insert_func(reg_q_table,FLASH_ENTRY(i)->func,id,arg);
        }
    }

    #undef FLASH_ENTRY
}           
    
//--------------------------------------------------------------------------//

void reg_q_exec_table_type2( Reg_Q_Table reg_q_table , Reg_Q_Ram_Entry2 *ram_entry, Reg_Id id , Reg_Arg arg)
{
    UInt8 i = 0;
    
    Reg_Q_Const_Entry2* flash_entry_table = (Reg_Q_Const_Entry2*)ram_entry->flash_entry;

    #define FLASH_ENTRY(i)  (&(flash_entry_table[i]))

    for(i=0;i<ram_entry->size;i++)
    {
        if((FLASH_ENTRY(i)->id==id) && (!!((FLASH_ENTRY(i)->mode) & reg_q_table->mode)))
        {
            reg_q_insert_func(reg_q_table,FLASH_ENTRY(i)->func,id,arg);    
        }
    }

    #undef  FLASH_ENTRY
}           
    
//--------------------------------------------------------------------------//

Boolean reg_q_insert_func(Reg_Q_Table reg_q_table,Reg_Func func,Reg_Id id,Reg_Arg arg )
{
    Exec_Q_Msg *exec_q_msg = (Exec_Q_Msg *) q_remove( reg_q_table->q_free , 100 ) ; 
	
    while( !exec_q_msg )
    {
        if(exec_q_free_add( reg_q_table->q_free  , sizeof (Exec_Q_Msg)))
        {
            exec_q_msg = (Exec_Q_Msg *) q_remove( reg_q_table->q_free , 100 ) ;         
        }else{
            return FALSE;
        }
    }
    
    exec_q_insert((Exec_Q_Msg *) exec_q_msg ,(Exec_Func)func,arg,id) ;

    return TRUE;
}

//--------------------------------------------------------------------------//




