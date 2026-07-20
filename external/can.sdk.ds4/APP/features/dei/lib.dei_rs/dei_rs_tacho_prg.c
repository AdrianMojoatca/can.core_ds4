/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_tacho_prg.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#include "display.h"
#ifdef PLATFORM_933
	#include "system_ui.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Rs_Tacho_Prg
{
    Boolean is_enabled; 
    Boolean timeout_expired;
    Vrtc    vrtc;    
}Dei_Rs_Tacho_Prg;

#define RPM_SAMPLE_TIME   6  // 60MS IS CLOSE TO 1/16TH SECV
#define RPM_HISTORY_AMT   (2*16)


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_rs_tacho_prg_start_push_valet    ( void );
static void dei_rs_tacho_prg_start_push_onboard  ( void );
static void dei_rs_tacho_prg_done                ( void );
static void dei_rs_tacho_prg_timeout_expired     ( void );
static void dei_rs_tacho_prg_ign_on              ( void );
static void ngmm_rs_tacho_prg_start_push_valet	 ( void ); 
//static void dei_rs_rpm_history									 ( void );
Per_Exec_Wake dei_rs_rpm_history									 ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Rs_Tacho_Prg    dei_rs_tacho_prg_vars;
static UInt16 rpm_history[RPM_HISTORY_AMT];
static UInt8 rpm_index = 0;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_tacho_prg_init( void )
{
	static const Cmd_Array1 cmd_array[] = 
    {
#ifdef DEV_LPC_236x
        { INPUT_EXT_PUSH_VALET_PRESS  , (Exec_Func)dei_rs_tacho_prg_start_push_valet  },
    	{ INPUT_EXT_PUSH_VALET_RELEASE, (Exec_Func)dei_rs_tacho_prg_done              },
#else
    	{ INPUT_TACH_LEARN    	  , (Exec_Func)ngmm_rs_tacho_prg_start_push_valet  },
    	{ INPUT_TACH_LEARN_DONE    	  , (Exec_Func)dei_rs_tacho_prg_done  },
#endif
    	{ INPUT_IGN_STATUS_ON    	  , (Exec_Func)dei_rs_tacho_prg_ign_on            },			
        { INPUT_EXT_PUSH_PRESS        , (Exec_Func)dei_rs_tacho_prg_start_push_onboard},
    	{ INPUT_EXT_PUSH_RELEASE      , (Exec_Func)dei_rs_tacho_prg_done              },        
			
    };

    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
	
#ifdef PLATFORM_933		
		per_exec_register( (Per_Exec_Func)dei_rs_rpm_history ) ; 	    
#endif
	
		dei_rs_tacho_prg_vars.vrtc = vrtc_f_create( (Exec_Func)dei_rs_tacho_prg_timeout_expired);
}

//--------------------------------------------------------------------------//
	
void dei_rs_tacho_prg_start_push_valet( void )
{  
	if( !get_ss_lock() && !dei_rs_state && get_rpm() && !dei_rs_tacho_prg_vars.timeout_expired )
	{
		dei_rs_tacho_prg_vars.is_enabled = TRUE ; 

		dei_led_flash( DEI_LED_RS_TACHO_PRG , TRUE) ; 

		set_rs_rpm_ref( get_rpm() );
	}
}


//--------------------------------------------------------------------------//

void dei_rs_tacho_prg_start_push_onboard( void )
{  
	if( !get_ss_lock() && !dei_rs_state && get_rpm() && !dei_rs_tacho_prg_vars.timeout_expired )
	{
		dei_rs_tacho_prg_vars.is_enabled = TRUE ; 

		display_once(GREEN_LED,3000);//on-board GREEN LED on for 3 seconds

		set_rs_rpm_ref( get_rpm() );
	}
}


//--------------------------------------------------------------------------//


void dei_rs_tacho_prg_done( void )
{
	UInt16 rpm;
	#ifdef PLATFORM_933
	UInt8 past;
	past = timestamp_payload >> (8-4);		// how many 1/16th of a sec ago
	
	if(past > (RPM_HISTORY_AMT -1))
	{
		past = RPM_HISTORY_AMT -1;
		
	}
	
	rpm = rpm_history[(rpm_index - past) & (RPM_HISTORY_AMT -1)];
	
	#else
	
	rpm = get_rpm();

	#endif
	
	
	if( dei_rs_tacho_prg_vars.is_enabled /*&& get_ign_status() && rpm */   )
	{
		dei_led_flash( DEI_LED_RESET , TRUE) ; 

		dei_prk_flash( DEI_PRK_RS_CONFIRM ) ; 
		
		dei_feature_set( DEI_FEAT_ENGINE_CHECKING_MODE , DEI_ECM_TACHOMETER );
		dei_feature_set( DEI_FEAT_RPM_REF_DIV10        , (rpm / 10)         );	// Default Tach reference divided by 10 (make it one-byte nvfs variable)
#ifdef PLATFORM_933
		dei_feature_set( DEI_FEAT_TACH_VAL_LEARNED     , DEI_TACH_LEARNED   );	// save the status that tach value learned
#endif

		nvram_usr_commit();

		cmd_q_rs_insert( INPUT_PRG_MODE_EXIT, 100 );

    vrtc_stop( dei_rs_tacho_prg_vars.vrtc );
        
    ATOMIC(dei_rs_tacho_prg_vars.is_enabled      = FALSE);                
	  ATOMIC(dei_rs_tacho_prg_vars.timeout_expired = FALSE);
	}	
}
				
//--------------------------------------------------------------------------//

void dei_rs_tacho_prg_timeout_expired( void )
{
	ATOMIC( dei_rs_tacho_prg_vars.timeout_expired = TRUE ); 	
}

//--------------------------------------------------------------------------//

void dei_rs_tacho_prg_ign_on( void )
{
	vrtc_stop( dei_rs_tacho_prg_vars.vrtc );

	ATOMIC( dei_rs_tacho_prg_vars.timeout_expired = FALSE );

	vrtc_update( dei_rs_tacho_prg_vars.vrtc , DEI_RS_VRTC_TIMEOUT_TACHO_PRG );//20

	vrtc_start( dei_rs_tacho_prg_vars.vrtc );	
}
	
//--------------------------------------------------------------------------//
void ngmm_rs_tacho_prg_start_push_valet( void )
{  
	if( !get_ss_lock() && !dei_rs_state && get_rpm() && !dei_rs_tacho_prg_vars.timeout_expired)
	{
		dei_rs_tacho_prg_vars.is_enabled = TRUE ; 

		dei_led_flash( DEI_LED_RS_TACHO_PRG , TRUE) ; 

		set_rs_rpm_ref( get_rpm() );
	}
}

//--------------------------------------------------------------------------//

Per_Exec_Wake dei_rs_rpm_history(void)
{
	static UInt8 tmr =0;
	
	if( ++tmr >= RPM_SAMPLE_TIME)
	{
		tmr =0;
		rpm_history[rpm_index & (RPM_HISTORY_AMT -1)] = get_rpm();
		rpm_index++;
	}
  
  return PER_EXEC_SLEEP;
}
