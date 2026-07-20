/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_disarm_override.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#include "dei_out_private.h"
#ifdef PLATFORM_933
  #include "sense_rs.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define  DEI_SS_OVERRIDE_PASSWORD		dei_feature_get( DEI_FEAT_VALET_SWITCH_PULSE_COUNT )

typedef struct s_Dei_Ss_Do
{
    Vrtc    vrtc_ign          ;
    Vrtc    vrtc_push         ;
    Boolean is_enable         ;
    UInt8   override_password ;
    Boolean push_pressed      ;
}Dei_Ss_Do ;
    
#define NGMM_SS_VRTC_OVERRIDE_CHECK  (VRTC_1SEC*1)

#ifdef PLATFORM_CM800
#define	DEI_NOTIFY_DISARM_PLD		(get_ss_door_trigger()  ? NOTIFY_PLD_TRIG_DOOR  : 0)|\
									(get_ss_hood_trigger()  ? NOTIFY_PLD_TRIG_HOOD  : 0)|\
									(get_ss_trunk_trigger() ? NOTIFY_PLD_TRIG_TRUNK : 0)|\
									(get_ss_ign_trigger()   ? NOTIFY_PLD_TRIG_IGN   : 0)|\
									(get_ss_sensor_trigger(1)? get_ss_sensor_trigger_src_history()    : 0)|\
									(get_ss_sensor_npc(1)    ? NOTIFY_PLD_TRIG_S1_NPC: 0)

#else

#define	DEI_NOTIFY_DISARM_PLD		(get_ss_door_trigger()  ? NOTIFY_PLD_TRIG_DOOR  : 0)|\
									(get_ss_hood_trigger()  ? NOTIFY_PLD_TRIG_HOOD  : 0)|\
									(get_ss_trunk_trigger() ? NOTIFY_PLD_TRIG_TRUNK : 0)|\
									(get_ss_ign_trigger()   ? NOTIFY_PLD_TRIG_IGN   : 0)|\
									 (get_ss_sensor_trigger(1)? DEI_LED_SHOCK_SENSOR_TRIGGERED: 0 )|\
									 (get_ss_sensor_trigger(2)? DEI_LED_TILT_SENSOR_TRIGGERED: 0 )|\
									 (get_ss_sensor_trigger(0)? DEI_LED_UNKNOWN_SENSOR_TRIGGERED: 0)|\
									(get_ss_sensor_npc(1)    ? NOTIFY_PLD_TRIG_S1_NPC: 0)|\
									(get_ss_sensor_npc(2)    ? NOTIFY_PLD_TRIG_S2_NPC: 0)|\
									(get_ss_sensor_npc(0)    ? NOTIFY_PLD_TRIG_S3_NPC: 0)
#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_ss_disarm_override_ignition_on          ( void );
static void dei_ss_disarm_override_ignition_off         ( void );
static void dei_ss_disarm_override_push_on              ( void );
static void dei_ss_disarm_override_push_off             ( void );
#ifdef DEV_LPC_176x   //eq. 933 platform
static void dei_ss_disarm_override_brake_on              ( void );
static void dei_ss_disarm_override_brake_off             ( void );
#endif
static void dei_ss_disarm_override_timeout_push_expired ( void );
static void dei_ss_disarm_override_timeout_ign_expired  ( void );
static void ngmm_ss_disarm_override(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Ss_Do dei_ss_do;
Vrtc  vrtc_ngmm_override_push_check;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_ss_disarm_override_init( void )
{	
	static const Cmd_Array1 cmd_array[] = 
    {
#ifdef DEV_LPC_236x
        { INPUT_IGN_STATUS_OFF    , (Exec_Func)dei_ss_disarm_override_ignition_off },
        { INPUT_IGN_STATUS_ON	  , (Exec_Func)dei_ss_disarm_override_ignition_on  },
        { INPUT_PUSH_VALET_RELEASE, (Exec_Func)dei_ss_disarm_override_push_off     },
    	{ INPUT_PUSH_VALET_PRESS  , (Exec_Func)dei_ss_disarm_override_push_on      },
#else
    	{ INPUT_EMERGENCY_OVERRIDE  , (Exec_Func)ngmm_ss_disarm_override      },
        { INPUT_IGN_STATUS_ON	  , (Exec_Func)dei_ss_disarm_override_ignition_on  },
        { INPUT_IGN_STATUS_OFF    , (Exec_Func)dei_ss_disarm_override_ignition_off },
        { INPUT_BRAKE_STATUS_RELEASE, (Exec_Func)dei_ss_disarm_override_brake_off     },
    	{ INPUT_BRAKE_STATUS_PRESS  , (Exec_Func)dei_ss_disarm_override_brake_on      },
#endif
    };

    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
            
    dei_ss_do.is_enable    = FALSE ;
    dei_ss_do.push_pressed = FALSE ; 

    dei_ss_do.vrtc_ign  = vrtc_f_create( (Exec_Func)dei_ss_disarm_override_timeout_ign_expired  ); 
	dei_ss_do.vrtc_push = vrtc_f_create( (Exec_Func)dei_ss_disarm_override_timeout_push_expired );	
		//vrtc_ngmm_override_push_check = vrtc_f_create((Exec_Func)ngmm_ss_disarm_override_push_check_timeout);
}


//--------------------------------------------------------------------------//

void dei_ss_disarm_override_ignition_off( void )
{
	dei_ss_do.override_password = 0 ; 

	vrtc_stop( dei_ss_do.vrtc_ign ); 

	dei_ss_do.is_enable = FALSE  ;
}

//--------------------------------------------------------------------------//

void dei_ss_disarm_override_ignition_on( void )
{
	dei_ss_do.override_password = 0 ; 
    
   vrtc_stop( dei_ss_do.vrtc_ign );

	vrtc_update( dei_ss_do.vrtc_ign  , DEI_SS_VRTC_TIMEOUT_OVERRIDE_NEW_PASSWORD );//only 10 seconds to input new password

	vrtc_start( dei_ss_do.vrtc_ign );
}

//--------------------------------------------------------------------------//

void dei_ss_disarm_override_push_on( void )
{
	ATOMIC( dei_ss_do.push_pressed = TRUE );
}

//--------------------------------------------------------------------------//

void dei_ss_disarm_override_push_off( void )
{
	ATOMIC( dei_ss_do.push_pressed = FALSE );
	
	if( dei_ss_do.is_enable )
	{
		dei_ss_do.override_password ++ ;

		vrtc_stop  ( dei_ss_do.vrtc_push ) ;		
		vrtc_update( dei_ss_do.vrtc_push , DEI_SS_VRTC_TIMEOUT_OVERRIDE_NEW_DIGIT ) ;
		vrtc_start( dei_ss_do.vrtc_push );		
	}
}

//--------------------------------------------------------------------------//

void dei_ss_disarm_override_brake_on( void )
{
  if(get_brake_status())
  	ATOMIC( dei_ss_do.push_pressed = TRUE );
	
	if(get_ign_status() && !dei_ss_do.is_enable)
		dei_ss_do.is_enable = TRUE ;
}

//--------------------------------------------------------------------------//

void dei_ss_disarm_override_brake_off( void )
{
  if(!get_brake_status())
  {
    ATOMIC( dei_ss_do.push_pressed = FALSE );
    
    if( dei_ss_do.is_enable )
    {
      dei_ss_do.override_password ++ ;

      vrtc_stop  ( dei_ss_do.vrtc_push ) ;		
      vrtc_update( dei_ss_do.vrtc_push , DEI_SS_VRTC_TIMEOUT_OVERRIDE_NEW_DIGIT ) ;
      vrtc_start( dei_ss_do.vrtc_push );		
    }
  }
}

//--------------------------------------------------------------------------//

void dei_ss_disarm_override_timeout_ign_expired( void )
{
	dei_ss_do.is_enable = FALSE ;
}

//--------------------------------------------------------------------------//
#if 0 // obsolete version
void dei_ss_disarm_override_timeout_push_expired( void )
{
  if( (DEI_SS_OVERRIDE_PASSWORD == dei_ss_do.override_password) && (get_ss_arm()||get_ss_lock()) && !get_ss_valet() && !dei_ss_do.push_pressed )
	{
		
		set_ss_door_trigger  ( FALSE );
		set_ss_hood_trigger  ( FALSE );
		set_ss_trunk_trigger ( FALSE );
		set_ss_ign_trigger   ( FALSE );
		set_ss_sensor_trigger( FALSE );
		set_ss_sensor_npc    ( FALSE );

		ATOMIC( dei_ss_2nd_disarm  = FALSE ) ;
		
#ifdef PLATFORM_CM800
		sensor_reset( ss_sensor );
#else
		sensor_reset( ss_sensor[0] );
		sensor_reset( ss_sensor[1] );
		sensor_reset( ss_sensor[2] );
#endif
		
		//dei_out_disarm_passive_starter_kill(); //MJB Removed due to refactored starter kill
		
		cmd_q_rs_insert( INPUT_OEM_UNLOCK , 100 , (void *)DEI_SS_SILENT );

		TRACE_DEI_SS("\n\r[DEI_SS_DISARM_OVERRIDE_VALID]\n\r");
	}
    else
    {
		TRACE_DEI_SS("\n\r[DEI_SS_DISARM_OVERRIDE_INVALID]\n\r");
    }
	
	ATOMIC( dei_ss_do.is_enable = FALSE );   		
		
}
#endif

//--------------------------------------------------------------------------//

void ngmm_ss_disarm_override( void )
{
  if(  get_ss_arm() && !get_ss_valet() )
	{
		
		set_ss_door_trigger  ( FALSE );
		set_ss_hood_trigger  ( FALSE );
		set_ss_trunk_trigger ( FALSE );
		set_ss_ign_trigger   ( FALSE );
		set_ss_sensor_trigger( FALSE );
		set_ss_sensor_npc    ( FALSE );
		set_ss_last_trigger	 ( FALSE );

		ATOMIC( dei_ss_2nd_disarm  = FALSE ) ;
		
#ifdef PLATFORM_CM800
		sensor_reset( ss_sensor );
#else
		sensor_reset( ss_sensor[0] );
		sensor_reset( ss_sensor[1] );
		sensor_reset( ss_sensor[2] );
		if(ext_sensor_trigger_configured)
		  sensor_reset(ss_sensor_ext);		
#endif
		
		cmd_q_rs_insert( INPUT_OEM_UNLOCK , 100 , (void *)DEI_SS_SILENT );
		
		cmd_q_rs_insert( INPUT_PRG_MODE_EXIT, 100 );
		
		notify_send( NOTIFY_UNLOCK  , DEI_NOTIFY_DISARM_PLD );
		
		TRACE_DEI_SS("\n\r[DEI_SS_DISARM_OVERRIDE_VALID]\n\r");
		
	}
	else
	{
#ifdef PLATFORM_CM800		
		if( (ss_system_type & SYS_TYPE_RS) && get_ss_lock() )
#else			
		if(get_ss_lock()||dei_get_sk_state())
#endif			
		{
			cmd_q_rs_insert( INPUT_OEM_UNLOCK , 100 , (void *)DEI_SS_SILENT );
			cmd_q_rs_insert( INPUT_PRG_MODE_EXIT, 100 );
		}
		else
			TRACE_DEI_SS("\n\r[DEI_SS_DISARM_OVERRIDE_INVALID]\n\r");
	}
		
	ATOMIC( dei_ss_do.is_enable = FALSE );   		
}

//--------------------------------------------------------------------------//

// new version
void dei_ss_disarm_override_timeout_push_expired( void )
{
	
  if( (DEI_SS_OVERRIDE_PASSWORD == dei_ss_do.override_password) && !dei_ss_do.push_pressed )
		ngmm_ss_disarm_override();
	
}

