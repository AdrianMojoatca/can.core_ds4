/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rs_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
#include "system_type.h"
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

void dei_system_rs_init( void )
{		
  System_Type sys_type;
  
  nvfs_rd( nvfs_sys_type , &sys_type );
  
	dei_system_common_init();
	
	dei_rs_init();
	
    dei_ss_init( );      
    		
	//rs_error:
	{
		static const Cmd_Array1 cmd_array[] = 
        {
            { INPUT_RF_VALET_TOOGLE              , (Exec_Func)dei_system_rf_error  },
        	{ INPUT_RF_FULL_SILENT_ARM           , (Exec_Func)dei_system_rf_error  },     
        	{ INPUT_RF_SENSOR_SILENT_ARM         , (Exec_Func)dei_system_rf_error  }, 
        	{ INPUT_RF_SENSOR_BYPASS_LEVEL       , (Exec_Func)dei_system_rf_error  },          
        	{ INPUT_RF_SENSOR1_ADJUST            , (Exec_Func)dei_system_rf_error  },     
        	{ INPUT_RF_PUT_NEW_SENSOR1_VALUE     , (Exec_Func)dei_system_rf_error  },         
        	{ INPUT_RF_TRIGGER_HISTORY_REQUEST   , (Exec_Func)dei_system_rf_error  },   
        };
//
      if((sys_type & (SYS_TYPE_RS | SYS_TYPE_RS)) == SYS_TYPE_RS )  //RS only
        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
	}  
    
    
}

//--------------------------------------------------------------------------//

