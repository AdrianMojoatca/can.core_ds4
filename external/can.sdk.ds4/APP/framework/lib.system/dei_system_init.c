/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_init.c 20535 2013-04-19 21:19:34Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
#include "system_ui.h"
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
static void dei_system_convenient_init(void);
//--------------------------------------------------------------------------//

void dei_system_init( void )
{ 	
   if(nvfs_rd_s( nvfs_sys_type , &dei_system_type, sizeof(dei_system_type) ) != NVFS_OK)
		 dei_system_type = sys_type_assumed;
	
    if(dei_system_type & SYS_TYPE_IID)
		{
      dei_system_type &= ~(SYS_TYPE_IID | SYS_TYPE_RS);    // to allow the system initialization properly and disable RS feature.
		}
		
    switch( dei_system_type )
    {
    	case    SYS_TYPE_RS                      :  dei_system_rs_init()           ; break ;      
     	case    SYS_TYPE_SS                      :  dei_system_ss_init    ( )      ; break ;
     	case    SYS_TYPE_RS | SYS_TYPE_SS        :  dei_system_hybrid_init( )      ; break ;
      case    SYS_TYPE_NULL                    :  dei_system_convenient_init()   ; break ;
		  default                                  :                                 ; break ;
    }

    dei_system_2nd_unlock = FALSE;   //TRUE ; wrong init
		vrtc_system_2nd_unlock = vrtc_f_create((Exec_Func)dei_system_2nd_unlock_reset);

    fob_ctrl_dei_init(); //do not change the place . The order its's very important.
	system_ui_init();
}


//--------------------------------------------------------------------------//
static void dei_system_convenient_init(void)
{
		static const Cmd_Array1 cmd_array[] = 
        {
//rs error          
          { INPUT_RF_VALET_TOOGLE              , (Exec_Func)dei_system_rf_error  },
          { INPUT_RF_FULL_SILENT_ARM           , (Exec_Func)dei_system_rf_error  },     
          { INPUT_RF_SENSOR_SILENT_ARM         , (Exec_Func)dei_system_rf_error  }, 
          { INPUT_RF_SENSOR_BYPASS_LEVEL       , (Exec_Func)dei_system_rf_error  },          
          { INPUT_RF_SENSOR1_ADJUST            , (Exec_Func)dei_system_rf_error  },     
          { INPUT_RF_PUT_NEW_SENSOR1_VALUE     , (Exec_Func)dei_system_rf_error  },         
          { INPUT_RF_TRIGGER_HISTORY_REQUEST   , (Exec_Func)dei_system_rf_error  },   
//ss error
          { INPUT_RF_START_STOP_TOOGLE     , (Exec_Func)dei_system_rf_error },   
        	{ INPUT_RF_SMART_START_TOOGLE    , (Exec_Func)dei_system_rf_error },       
        	{ INPUT_RF_TIMER_MODE_TOOGLE     , (Exec_Func)dei_system_rf_error },       
        	{ INPUT_RF_RUNTIME_RESET         , (Exec_Func)dei_system_rf_error },       
        	{ INPUT_RF_REAR_DEFOGGER         , (Exec_Func)dei_system_rf_error },       
        	{ INPUT_RF_TEMP_REPORT_REQ       , (Exec_Func)dei_system_rf_error },       
        	{ INPUT_RF_TEMP_START_ADJUST     , (Exec_Func)dei_system_rf_error },   
        	{ INPUT_RF_PUT_NEW_TEMP          , (Exec_Func)dei_system_rf_error },   
        	{ INPUT_RF_START_TIMES_ADJUST    , (Exec_Func)dei_system_rf_error },   
        	{ INPUT_RF_PUT_NEW_START_TIMES   , (Exec_Func)dei_system_rf_error },  
    		  { INPUT_RF_RUNTIME_CHECK         , (Exec_Func)dei_system_rf_error }
        };

    dei_system_common_init();        
        
    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
}
