/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_ss_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_system_ss_init( void )
{		
	dei_system_common_init();

    dei_ss_init();      
		
   // ss_error:
	{
    	static const Cmd_Array1 cmd_array[] = 
        {        
            { INPUT_RF_START_STOP_TOOGLE     , (Exec_Func)dei_system_rf_error },   
        	{ INPUT_RF_SMART_START_TOOGLE    , (Exec_Func)dei_system_rf_error },       
        	{ INPUT_RF_TIMER_MODE_TOOGLE     , (Exec_Func)dei_system_rf_error },       
        	{ INPUT_RF_RUNTIME_RESET         , (Exec_Func)dei_system_rf_error },       
        	{ INPUT_RF_REAR_DEFOGGER         , (Exec_Func)dei_system_rf_error },       
//        	{ INPUT_RF_TEMP_REPORT_REQ       , (Exec_Func)dei_system_rf_error },       
        	{ INPUT_RF_TEMP_START_ADJUST     , (Exec_Func)dei_system_rf_error },   
        	{ INPUT_RF_PUT_NEW_TEMP          , (Exec_Func)dei_system_rf_error },   
        	{ INPUT_RF_START_TIMES_ADJUST    , (Exec_Func)dei_system_rf_error },   
        	{ INPUT_RF_PUT_NEW_START_TIMES   , (Exec_Func)dei_system_rf_error },  
    		{ INPUT_RF_RUNTIME_CHECK         , (Exec_Func)dei_system_rf_error }
        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
	}        
}

//--------------------------------------------------------------------------//



