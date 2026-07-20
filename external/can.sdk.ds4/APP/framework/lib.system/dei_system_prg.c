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
	
    cmd_register1:
    {
        static const Cmd_Array1 cmd_array[] = 
        {

        	{ INPUT_REMOTE_PAIRING    , (Exec_Func)prg_evt_remote_pairing     },
        	{ INPUT_REMOTE_DELETE, (Exec_Func)prg_evt_remote_delete },
        	{ INPUT_ZAP    , (Exec_Func)prg_evt_zap     },
        	{ INPUT_TACH_DELETE, (Exec_Func)prg_evt_tach_delete },

        	{ INPUT_IGN_STATUS_ON    	  	, (Exec_Func)prg_evt_ign_on           },
        	{ INPUT_IGN_STATUS_OFF	      , (Exec_Func)prg_evt_ign_off          },
        	{ INPUT_ALL_DOOR_STATUS_CLOSE , (Exec_Func)prg_evt_door_close       },
          { INPUT_BLE_PRG_ENTER         , (Exec_Func)prg_menu_execute}
        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL|EXEC_MODE_PROG);
    }
	    
    cmd_register2:
    {
        static const Cmd_Array1 cmd_array[] = 
        {        
          { INPUT_RF_LOCK                , (Exec_Func)prg_evt_lock              },
        	{ INPUT_OEM_LOCK               , (Exec_Func)prg_evt_lock              },
          { INPUT_OEM_ARM                , (Exec_Func)prg_evt_lock              },
        	{ INPUT_RF_PROGRAMMING_DONE    , (Exec_Func)prg_evt_remote_programmed },
//        	{ INPUT_RF_LOCK                , (Exec_Func)dei_system_rf_range_check },
//          { INPUT_RF_SILENT_LOCK         , (Exec_Func)dei_system_rf_range_check },
          { INPUT_RF_ONE_BUTTON_FUNCTION , (Exec_Func)prg_evt_lock              },
					{ INPUT_TAP_HOLD  , (Exec_Func)prg_menu_select_tap_hold 			},
					{	INPUT_TWO_TAP_HOLD  , (Exec_Func)prg_menu_select_two_tap_hold  			},
					{	INPUT_THREE_TAP_HOLD , (Exec_Func)prg_menu_select_three_tap_hold   	}
        };
        
        //CMD_LUT_INSERT1(cmd_array , EXEC_MODE_PROG);
        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
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
		dei_siren_chirp( DEI_SIREN_REMOTE_PROGRAMMING_ENTER ); 
		dei_led_flash  ( DEI_LED_REMOTE_PROGRAMMING , TRUE );
			
		TRACE_SYS("\n\r[ REMOTE PROGRAMING ENTER ]\n\r");
		
		notify_send( NOTIFY_PROGRAMMING_ENTER );		

		os_tsk_wait(500);
		dhp_pairmode();
}

//--------------------------------------------------------------------------//

void dei_system_prg_remote_programming_done ( void )
{
	dei_siren_chirp( DEI_SIREN_REMOTE_LEARNED ); 	

	notify_send(NOTIFY_REMOTE_PROGRAMMED_ACK);

	TRACE_SYS("\n\r[ REMOTE PROGRAMMING DONE ]\n\r");	
	
	dei_prk_flash(DEI_PRK_FLASH1);
}

//--------------------------------------------------------------------------//

void dei_system_prg_remote_delete_enter( void )
{
	//wake_enter();
        
	notify_send( NOTIFY_REMOTE_DELETE_ENTER );		
	
	dei_siren_chirp( DEI_SIREN_REMOTE_DELETE_ENTER ); 
	dei_led_flash  ( DEI_LED_REMOTE_DELETE , TRUE  );

	mainp_status(0x82);  //installer allowed, eraseall mode  
	
	TRACE_SYS("\n\r[ REMOTE DELETE ENTER ]\n\r");
}

//--------------------------------------------------------------------------//

void dei_system_prg_remote_delete_done( void )
{
	notify_send( NOTIFY_REMOTE_DELETE ) ; 

	dei_siren_chirp( DEI_SIREN_REMOTE_PROGRAMMING_DONE ); 	
	os_tsk_wait(500);
	
	dhp_eraseall();
	
	notify_send( NOTIFY_REMOTE_DELETE ) ; 

	TRACE_SYS("REMOTE DELETE DONE\n\r");	
}

//--------------------------------------------------------------------------//

void dei_system_prg_tacho_delete_enter( void )
{
	TRACE_SYS("\n\r[ TACHO DELETE ENTER ]\n\r");

//	notify_send( NOTIFY_PROGRAMMING_LEAVE );		
	notify_send( NOTIFY_REMOTE_TACHO_DELETE );		

  dei_siren_chirp( DEI_SIREN_TACHO_DELETE_ENTER ); 
	dei_led_flash  ( DEI_LED_TACHO_DELETE , TRUE  );
}

//--------------------------------------------------------------------------//
extern const UInt8              dei_feature_default_table[];
void dei_system_prg_tacho_delete_done( void )
{	
  	set_rs_rpm_ref(0);
  	set_rs_vtacho_ref(0);
    
    dei_feature_set( DEI_FEAT_VIRTUAL_TACH_FINE_TUNE , dei_feature_default_table[DEI_FEAT_VIRTUAL_TACH_FINE_TUNE]) ;
	dei_feature_set( DEI_FEAT_VTACHO_REF , 0x03 ); //put in not programmed state

	TRACE_SYS("\n\r[ TACHO DELETE DONE ]\n\r");	

	dei_siren_chirp( DEI_SIREN_REMOTE_PROGRAMMING_DONE ); 	
}

//--------------------------------------------------------------------------//

void dei_system_prg_zap_enter( void )
{
	TRACE_SYS("\n\r[ ZAP ENTER ]\n\r");

//	notify_send( NOTIFY_PROGRAMMING_LEAVE );		
	notify_send( NOTIFY_REMOTE_ZAP_ENTER );		

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
    	
	TRACE_SYS("\n\r[ ZAP DONE ]\n\r");	

	dei_siren_chirp( DEI_SIREN_REMOTE_PROGRAMMING_DONE ); 	
}

//--------------------------------------------------------------------------//

void dei_system_prg_exit( void )
{
	TRACE_SYS("\n\r[ PROGRAMMING EXIT ]\n\r");
	mainp_status(0x00);	
	notify_send( NOTIFY_PROGRAMMING_LEAVE ) ; 

  dei_system_prg_mode = FALSE;

	cmd_q_rs_insert( INPUT_PRG_MODE_EXIT, 100 );

	dei_siren_chirp( DEI_SIREN_PROGRAMMING_EXIT ); 
	dei_led_flash  ( DEI_LED_RESET , TRUE);

	if (get_ble_prg_menu())
	{
		body_prk_set_level(0);
	}
    nvram_usr_commit();
}

//--------------------------------------------------------------------------//
Boolean dei_system_prg_mode_get(void)
{
  return dei_system_prg_mode;
}

void dei_system_prg_mode_set(Boolean val)
{
   dei_system_prg_mode = val;
}  
