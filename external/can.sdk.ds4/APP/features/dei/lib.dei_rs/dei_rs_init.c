/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_init.c 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
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

void dei_rs_init( void )
{	
    System_Type dei_system_type ;

    if(nvfs_rd_s( nvfs_sys_type , &dei_system_type, sizeof(dei_system_type) ) != NVFS_OK)
			dei_system_type = sys_type_assumed;

    dei_rs_trigger_enable = !!((dei_system_type & SYS_TYPE_SS) == SYS_TYPE_SS );
    
    dei_rs_ecm_init();  
  	
	dei_rs_runtime_init();
  	
	dei_rs_smart_start_init();
  	
	dei_rs_timer_start_init();
	
	dei_rs_evt_init();
	
	dei_rs_tacho_prg_init();	
	
	dei_rs_shutdown_history_init();	
	
	dei_rs_mts_init();
    
	dei_rs_defogger_init();
		
	dei_rs_cmd_init();

	dei_rs_rf_start_stop_init();	

	dei_rs_secure_lock_init();
	
	#ifdef DEV_LPC_236x

    #ifndef ANALOGIC_TEMP_SENSOR_DISABLED
        dei_temp_init();
    #else
    #if ANALOGIC_TEMP_SENSOR_DISABLED == 1
        #warning "Analogical temp sensor disabled"
    #else
        dei_temp_init();
    #endif
    #endif
	#endif
				
	dei_rs_state = NULL ;	
}

//--------------------------------------------------------------------------//

