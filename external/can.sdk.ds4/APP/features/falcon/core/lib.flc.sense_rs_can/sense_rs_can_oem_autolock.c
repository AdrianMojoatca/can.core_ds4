/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk_flash.c 1106 2010-04-27 13:30:39Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_can_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void autolock_door_open( void );
static void autolock_lock     ( void );
static void autolock_unlock   ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean enable_autolock ;
static Boolean lock_status     ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_oem_autolock_init( void )
{ 
    enable_autolock = FALSE ;
    
    lock_status     = FALSE ;  
    
    cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {    
            {INPUT_ALL_DOOR_STATUS_OPEN            , (Exec_Func)autolock_door_open },    
            {INPUT_LOCK1		        |INPUT_ON  , (Exec_Func)autolock_lock 	   },
            {INPUT_LOCK2		        |INPUT_ON  , (Exec_Func)autolock_lock 	   },
            {INPUT_LOCK3		        |INPUT_ON  , (Exec_Func)autolock_lock 	   },
        	{INPUT_OEM_LOCK 			           , (Exec_Func)autolock_lock      },
        	{INPUT_UNLOCK_DRIVER1       | INPUT_ON , (Exec_Func)autolock_unlock    },
        	{INPUT_UNLOCK_DRIVER2       | INPUT_ON , (Exec_Func)autolock_unlock    },
        	{INPUT_UNLOCK_DRIVER3       | INPUT_ON , (Exec_Func)autolock_unlock    },
        	{INPUT_OEM_UNLOCK 	                   , (Exec_Func)autolock_unlock    },
        	{INPUT_UNLOCK_ALL1	        | INPUT_ON , (Exec_Func)autolock_unlock    },
        	{INPUT_UNLOCK_ALL2	        | INPUT_ON , (Exec_Func)autolock_unlock    },
        	{INPUT_UNLOCK_ALL3	        | INPUT_ON , (Exec_Func)autolock_unlock    }
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
}

//--------------------------------------------------------------------------//

Boolean sense_rs_oem_autolock_can( Boolean instant_level )
{    
    if( get_door_status() || get_ign_status() ) ATOMIC( enable_autolock = FALSE ) ; 
    
    return sense_rs_oem_lock_can(enable_autolock && instant_level);
}

//--------------------------------------------------------------------------//

void autolock_door_open( void )
{
    if( enable_autolock )
    {        
        ATOMIC( enable_autolock  = FALSE ) ;
    }
}

//--------------------------------------------------------------------------//

void autolock_lock( void )
{
	if( get_ign_status() && !get_rs_gwr() ) return  ; 

	ATOMIC( lock_status = TRUE ; enable_autolock = FALSE ) ;     
}

//--------------------------------------------------------------------------//

void autolock_unlock( void )
{
	if( lock_status ) ATOMIC( enable_autolock = TRUE ) ; 

    ATOMIC( lock_status = FALSE ) ; 
}

//--------------------------------------------------------------------------//

