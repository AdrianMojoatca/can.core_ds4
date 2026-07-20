/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_prg.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
#ifdef DEV_LPC_176x
	#include "aid_dhp.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_system_prg_remote_programming_enter ( void );
static void dei_system_prg_remote_programming_done  ( void );
static void dei_system_prg_remote_delete_enter	    ( void );
static void dei_system_prg_remote_delete_done		( void );
static void dei_system_prg_tacho_delete_enter		( void );
static void dei_system_prg_tacho_delete_done	    ( void );
static void dei_system_prg_zap_enter                ( void );
static void dei_system_prg_zap_done                 ( void );
static void dei_system_prg_exit					    ( void );


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_system_prg_init( void )
{
	prg_init(); 
	
    //cmd_register1:
    {
        static const Cmd_Array1 cmd_array[] = 
        {
        	{ INPUT_EXT_PUSH_VALET_PRESS  , (Exec_Func)prg_evt_push_ext         },
        	{ INPUT_PUSH_VALET_RELEASE    , (Exec_Func)prg_evt_push_release     },
					{ INPUT_PUSH_VALET_RELEASE_   , (Exec_Func)prg_evt_push_release     },
        	{ INPUT_EXT_PUSH_VALET_RELEASE, (Exec_Func)prg_evt_push_ext_release },
        	{ INPUT_IGN_STATUS_ON    	  , (Exec_Func)prg_evt_ign_on           },
        	{ INPUT_IGN_STATUS_OFF	      , (Exec_Func)prg_evt_ign_off          },
        	{ INPUT_ALL_DOOR_STATUS_CLOSE , (Exec_Func)prg_evt_door_close       }
        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL|EXEC_MODE_PROG);
    }
	    
    //cmd_register2:
    {
        static const Cmd_Array1 cmd_array[] = 
        {        
            { INPUT_RF_LOCK                , (Exec_Func)prg_evt_lock              },
        	{ INPUT_OEM_LOCK               , (Exec_Func)prg_evt_lock              },
            { INPUT_OEM_ARM                , (Exec_Func)prg_evt_lock              },
        	{ INPUT_RF_PROGRAMMING_DONE    , (Exec_Func)prg_evt_remote_programmed },
        	{ INPUT_RF_LOCK                , (Exec_Func)dei_system_rf_range_check },
            { INPUT_RF_SILENT_LOCK         , (Exec_Func)dei_system_rf_range_check },
            { INPUT_RF_ONE_BUTTON_FUNCTION , (Exec_Func)prg_evt_lock              }
        };
        
        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_PROG);
    }
	
	prg_func_register( (Reg_Func)dei_system_prg_remote_programming_enter , PRG_FUNC_REMOTE_PROGRAMMING_ENTER ) ; 
	prg_func_register( (Reg_Func)dei_system_prg_remote_programming_done  , PRG_FUNC_REMOTE_PROGRAMMING_DONE  ) ; 
	prg_func_register( (Reg_Func)dei_system_prg_remote_delete_enter      , PRG_FUNC_REMOTE_DELETE_ENTER      ) ; 
	prg_func_register( (Reg_Func)dei_system_prg_remote_delete_done       , PRG_FUNC_REMOTE_DELETE_DONE       ) ; 
	prg_func_register( (Reg_Func)dei_system_prg_tacho_delete_enter       , PRG_FUNC_TACHO_DELETE_ENTER       ) ; 
	prg_func_register( (Reg_Func)dei_system_prg_tacho_delete_done        , PRG_FUNC_TACHO_DELETE_DONE        ) ; 
	prg_func_register( (Reg_Func)dei_system_prg_zap_enter 			      , PRG_FUNC_ZAP_ENTER     			  ) ; 
	prg_func_register( (Reg_Func)dei_system_prg_zap_done        		  , PRG_FUNC_ZAP_DONE        		  ) ; 	
	prg_func_register( (Reg_Func)dei_system_prg_exit                     , PRG_FUNC_EXIT                     ) ;
	
}	

//--------------------------------------------------------------------------//

void dei_system_prg_remote_programming_enter( void )
{	
	if( dei_feature_get(DEI_FEAT_TRANSMITTER_PROGRAMMING ) == 1 )
	{ 
		dei_siren_chirp( DEI_SIREN_REMOTE_PROGRAMMING_ENTER ); 
		dei_led_flash  ( DEI_LED_REMOTE_PROGRAMMING , TRUE);
			
		TRACE_DEI_SYS("\n\r[ REMOTE PROGRAMING ENTER ]\n\r");
		
		cmd_lut_clr_mode( EXEC_MODE_NORMAL );
		cmd_lut_set_mode( EXEC_MODE_PROG   );

		notify_send( NOTIFY_PROGRAMMING_ENTER );		
		#ifdef DEV_LPC_176x
			os_tsk_wait(500);
			dhp_pairmode();
		#endif
	}
	else
	{
		TRACE_DEI_SYS("\n\r[ REMOTE PROGRAMING LOCKED ]\n\r");
				
		dei_system_prg_exit();
	}

}

//--------------------------------------------------------------------------//

void dei_system_prg_remote_programming_done ( void )
{
	dei_siren_chirp( DEI_SIREN_REMOTE_PROGRAMMING_DONE ); 	
#ifdef DEV_LPC_176x	
	notify_send(NOTIFY_REMOTE_PROGRAMMED_ACK);
#endif
	TRACE_DEI_SYS("\n\r[ REMOTE PROGRAMMING DONE ]\n\r");	
}

//--------------------------------------------------------------------------//

void dei_system_prg_remote_delete_enter( void )
{
	notify_send( NOTIFY_REMOTE_DELETE_ENTER );		
	
	if( dei_feature_get(DEI_FEAT_TRANSMITTER_PROGRAMMING) == 1 )
	{
		dei_siren_chirp( DEI_SIREN_REMOTE_DELETE_ENTER ); 
		dei_led_flash  ( DEI_LED_REMOTE_DELETE , TRUE  );
			
		TRACE_DEI_SYS("\n\r[ REMOTE DELETE ENTER ]\n\r");

		cmd_lut_clr_mode( EXEC_MODE_NORMAL );
		cmd_lut_set_mode( EXEC_MODE_PROG   );		
	}
	else
	{
		TRACE_DEI_SYS("\n\r[ REMOTE DELETE LOCKED ]\n\r");
				
		dei_system_prg_exit();
	}

}

//--------------------------------------------------------------------------//

void dei_system_prg_remote_delete_done( void )
{
	notify_send( NOTIFY_REMOTE_DELETE ) ; 
	
	#ifdef DEV_LPC_176x
		dhp_eraseall();
	#endif
	
	dei_siren_chirp( DEI_SIREN_REMOTE_PROGRAMMING_DONE ); 	

	TRACE_DEI_SYS("REMOTE DELETE DONE\n\r");	
}

//--------------------------------------------------------------------------//

void dei_system_prg_tacho_delete_enter( void )
{
	TRACE_DEI_SYS("\n\r[ TACHO DELETE ENTER ]\n\r");

	notify_send( NOTIFY_PROGRAMMING_LEAVE );		

	cmd_lut_clr_mode( EXEC_MODE_NORMAL );
	cmd_lut_set_mode( EXEC_MODE_PROG   );

	dei_siren_chirp( DEI_SIREN_TACHO_DELETE_ENTER ); 
	dei_led_flash  ( DEI_LED_TACHO_DELETE , TRUE  );
}

//--------------------------------------------------------------------------//

void dei_system_prg_tacho_delete_done( void )
{	
  	set_rs_rpm_ref(0);
  	set_rs_vtacho_ref(0);
    
    dei_feature_set( DEI_FEAT_VIRTUAL_TACH_FINE_TUNE , 0xC0 ) ; 
	dei_feature_set( DEI_FEAT_VTACHO_REF , 0x03 ); //put in not programmed state

	TRACE_DEI_SYS("\n\r[ TACHO DELETE DONE ]\n\r");	

	dei_siren_chirp( DEI_SIREN_REMOTE_PROGRAMMING_DONE ); 	
}

//--------------------------------------------------------------------------//

void dei_system_prg_zap_enter( void )
{
	TRACE_DEI_SYS("\n\r[ ZAP ENTER ]\n\r");

	notify_send( NOTIFY_PROGRAMMING_LEAVE );		

	cmd_lut_clr_mode( EXEC_MODE_NORMAL );
	cmd_lut_set_mode( EXEC_MODE_PROG   );

	dei_siren_chirp( DEI_SIREN_ZAP_ENTER ); 
	dei_led_flash  ( DEI_LED_ZAP , TRUE  );
}

//--------------------------------------------------------------------------//

void dei_system_prg_zap_done( void )
{
	UInt8 vtacho_ref = dei_feature_get( DEI_FEAT_VTACHO_REF ) ; 
	
	dei_feature_reset();
	dei_feature_init();
	    
	dei_feature_set( DEI_FEAT_VTACHO_REF , vtacho_ref ); //put in not programmed state
    	
	TRACE_DEI_SYS("\n\r[ ZAP DONE ]\n\r");	

	dei_siren_chirp( DEI_SIREN_REMOTE_PROGRAMMING_DONE ); 	
}

//--------------------------------------------------------------------------//

void dei_system_prg_exit( void )
{
	TRACE_DEI_SYS("\n\r[ PROGRAMMING EXIT ]\n\r");
		
	notify_send( NOTIFY_PROGRAMMING_LEAVE ) ; 

	cmd_lut_clr_mode( EXEC_MODE_PROG   );
	cmd_lut_set_mode( EXEC_MODE_NORMAL );

	dei_siren_chirp( DEI_SIREN_PROGRAMMING_EXIT ); 
	dei_led_flash  ( DEI_LED_RESET , TRUE);

    nvram_usr_commit();
}


//--------------------------------------------------------------------------//
