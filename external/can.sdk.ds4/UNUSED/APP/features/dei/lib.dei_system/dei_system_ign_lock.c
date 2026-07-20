/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_ign_lock.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define IGN_NO_LOCK      (dei_feature_get( DEI_FEAT_IGN_CONTROLLED_LOCKS ) == 1)
#define IGN_LOCK_UNLOCK	 (dei_feature_get( DEI_FEAT_IGN_CONTROLLED_LOCKS ) == 2)
#define IGN_LOCK	 	 (dei_feature_get( DEI_FEAT_IGN_CONTROLLED_LOCKS ) == 3)
#define IGN_UNLOCK	 	 (dei_feature_get( DEI_FEAT_IGN_CONTROLLED_LOCKS ) == 4)
#define SECOND_UNLOCK_ON (dei_feature_get( DEI_FEAT_2ND_UNLOCK ) != 1)  


typedef struct s_Dei_System_Ign_Lock
{
    Vrtc    vrtc_lock;
    Vrtc    vrtc_unlock_skip;
    Boolean lock_active ; 
    Boolean unlock_skip ;
}Dei_System_Ign_Lock;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_system_ign_lock_timeout_lock_expired  		( void );
static void dei_system_ign_lock_control				        ( void );
static void dei_system_ign_lock_unlock_skip_timeout     	( void );
static void dei_system_ign_lock_unlock_skip_trigger_timeout	( void );
static void dei_system_ign_lock_unlock_skip_activate    	( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_System_Ign_Lock  dei_system_ign_lock_vars;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_system_ign_lock_init( void )
{
	dei_system_ign_lock_vars.lock_active  = FALSE ;
    dei_system_ign_lock_vars.unlock_skip  = FALSE ;  
    
    if( !IGN_NO_LOCK ) //feature is enable
	{
		dei_system_ign_lock_vars.unlock_skip = FALSE ; 
		
		dei_system_ign_lock_vars.vrtc_lock   	  = vrtc_f_create( (Exec_Func)dei_system_ign_lock_timeout_lock_expired  ) ;
		dei_system_ign_lock_vars.vrtc_unlock_skip = vrtc_create  ( (Vrtc_Func)dei_system_ign_lock_unlock_skip_timeout   ) ;
		
		cmd_register:
        {
            static const Cmd_Array1 cmd_array[] = 
            {
                { INPUT_IGN_STATUS_ON , (Exec_Func)dei_system_ign_lock_control          	      },
        		{ INPUT_IGN_STATUS_OFF, (Exec_Func)dei_system_ign_lock_control          	      },
        		{ INPUT_GWR_OFF       , (Exec_Func)dei_system_ign_lock_unlock_skip_trigger_timeout},
        		{ INPUT_GWR_ON        , (Exec_Func)dei_system_ign_lock_unlock_skip_activate   	  }
            };
            
            CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
        }
	}
}

//--------------------------------------------------------------------------//

void dei_system_ign_lock_control( void )
{		  
	if( get_rs_gwr() || get_ss_lock() ) return ;//not available in remote start and when the car is locked	
	  
  	if( get_ign_status() )
	{
		dei_system_ign_lock_vars.lock_active = TRUE ; 

		vrtc_stop( dei_system_ign_lock_vars.vrtc_lock ) ; 
		
		vrtc_update( dei_system_ign_lock_vars.vrtc_lock , DEI_SYSTEM_VRTC_TIMEOUT_IGN_LOCK ) ; 
				
		if( IGN_LOCK_UNLOCK || IGN_LOCK )
		{
			vrtc_start( dei_system_ign_lock_vars.vrtc_lock ); 			
		}

	}
	else
	{
		dei_system_ign_lock_vars.lock_active = FALSE ; 

		vrtc_stop( dei_system_ign_lock_vars.vrtc_lock );
		
		if( IGN_LOCK_UNLOCK || IGN_UNLOCK )
		{
			if( !dei_system_ign_lock_vars.unlock_skip )
			{
				cmd_q_rs_insert( INPUT_UNLOCK_DRIVER1 | INPUT_ON , 100 , (void*)CMD_UNLOCK_IGNITION ) ; 
        if(!dei_system_2nd_unlock && SECOND_UNLOCK_ON)
				{
						cmd_q_rs_insert( INPUT_UNLOCK_ALL1 | INPUT_ON , 100 , (void*)CMD_UNLOCK_IGNITION ) ; 
			  }				
			}
		}
	}
}

//--------------------------------------------------------------------------//

void dei_system_ign_lock_timeout_lock_expired( void )
{
	if( dei_system_ign_lock_vars.lock_active )
	{
		dei_system_ign_lock_vars.lock_active = FALSE ; 

	//  if( !(door.byte&0x0F)  )
		if( !get_door_status() )
		{
			cmd_q_rs_insert( INPUT_LOCK1  | INPUT_ON , 100 , (void*)CMD_LOCK_IGNITION ) ; 
    }

		vrtc_stop( dei_system_ign_lock_vars.vrtc_lock ) ; 
	}
}

//--------------------------------------------------------------------------//

void dei_system_ign_lock_unlock_skip_trigger_timeout( void )
{
	vrtc_stop( dei_system_ign_lock_vars.vrtc_unlock_skip ) ;
		
	vrtc_update( dei_system_ign_lock_vars.vrtc_unlock_skip , 	DEI_SYSTEM_VRTC_TIMEOUT_IGN_LOCK );

	vrtc_start( dei_system_ign_lock_vars.vrtc_unlock_skip );
}

//--------------------------------------------------------------------------//

void dei_system_ign_lock_unlock_skip_timeout( void )
{
	ATOMIC( dei_system_ign_lock_vars.unlock_skip = FALSE );
}

//--------------------------------------------------------------------------//

void dei_system_ign_lock_unlock_skip_activate( void )
{
	ATOMIC( dei_system_ign_lock_vars.unlock_skip = TRUE );
}

//--------------------------------------------------------------------------//

#undef IGN_NO_LOCK      
#undef IGN_LOCK_UNLOCK	 
#undef IGN_LOCK	 	 
#undef IGN_UNLOCK	 	 
#undef SECOND_UNLOCK_ON 
