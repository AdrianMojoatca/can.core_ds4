/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: func_rs.c 2770 2011-05-23 06:43:37Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "func_private.h"

#include "eips.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Timeout_Q_Status func_eips_alert_timeout( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void func_eips_init( void )
{
    if( eips_is_enable() )
    {        
        cmd_register:
        {
            static const Cmd_Array1 cmd_array[] = 
            {        
                { INPUT_ALL_DOOR_STATUS_OPEN    , (Exec_Func)eips_door_set          },
                { INPUT_ALL_DOOR_STATUS_CLOSE   , (Exec_Func)eips_door_clr          },        
                { INPUT_SKD_SET                 , (Exec_Func)eips_skd_set           },
                { INPUT_SKD_CLR                 , (Exec_Func)eips_skd_clr           },
                { INPUT_RPM_SET                 , (Exec_Func)eips_rpm_set           },
                { INPUT_RPM_CLR                 , (Exec_Func)eips_rpm_clr           },        
                { INPUT_SPEED_SET               , (Exec_Func)eips_speed_set         },
                { INPUT_SPEED_CLR               , (Exec_Func)eips_speed_clr         },                         
                { INPUT_EIPS_ALERT         	    , (Exec_Func)func_eips_alert   },
                { INPUT_EIPS_SHUTDOWN      	    , (Exec_Func)func_eips_shutdown},
                { INPUT_EIPS_SHUTDOWN_FAIL 	    , (Exec_Func)func_eips_fail    },
            };

            CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
        };
    }

}
/*--------------------------------------------------------------------------*/

void func_eips_shutdown( void )
{
    tsk_rs_shutdown();
}

/*--------------------------------------------------------------------------*/

Timeout_Q_Status func_eips_alert( void *arg)
{
	timeout_q_remove( (void *)func_eips_alert_timeout   );

	timeout_q_insert( (Timeout_Q_Func)func_eips_alert_timeout , NULL , 3 , 1 , 1 );
	
	ATOMIC( FUNC_EIPS_ALARM_OUT( TRUE ) );
	
	return TIMEOUT_Q_RUNNING ; 	
}	

//--------------------------------------------------------------------------//

void func_eips_fail( void *arg)
{
	timeout_q_remove( (void *)func_eips_alert );

	if( !!arg )
	{
		timeout_q_insert( (Timeout_Q_Func)func_eips_alert_timeout , NULL , 10 , 100 , 30 );				
	}		
}	

//--------------------------------------------------------------------------//

Timeout_Q_Status func_eips_alert_timeout( void )
{
	ATOMIC( FUNC_EIPS_ALARM_OUT( TRUE ) );

	return TIMEOUT_Q_COMPLETE ; 
}

//--------------------------------------------------------------------------//

