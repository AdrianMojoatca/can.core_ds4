/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_1btn.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_system_one_btn_function( void );
static void dei_system_one_btn_stop	( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_system_rf_1btn_init( void )
{		
    static const Cmd_Array1 cmd_array[] = 
    {
        { INPUT_RF_STOP 			   , (Exec_Func)dei_system_one_btn_stop    },
	    { INPUT_RF_ONE_BUTTON_FUNCTION , (Exec_Func)dei_system_one_btn_function}
    };

    CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
}

//--------------------------------------------------------------------------//

void dei_system_one_btn_stop( void )
{
  	UInt8 i ;
	UInt8 pulse_count = dei_feature_get( DEI_FEAT_ACTIVATION_PULSE_COUNT );
		
	if( get_rs_gwr() )
	{		
		
        if( get_rs_mts_progress() )
        {
            if( get_rs_mts_lock() == FALSE )
            {
                cmd_q_rs_insert( INPUT_RF_LOCK , 100 , 0 );                
            }
            else
            {        
                for( i = 0 ; i < pulse_count ; i ++ )
        		{
        			cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );
        		}
            }
        }
        else
        {
            for( i = 0 ; i < pulse_count ; i ++ )
        	{
        		cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );
        	}
        }            
	}
	else
	{
		cmd_q_rs_insert( INPUT_RF_CAR_FINDER  , 100 , 0 );
	}
}

//--------------------------------------------------------------------------//

void dei_system_one_btn_function( void )
{
  	if( get_rs_gwr() )
	{		
    	cmd_q_rs_insert( INPUT_RF_UNLOCK , 100 , 0 );
	}
	else
	{
		cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );
	}
}

//--------------------------------------------------------------------------//

