/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_trunk_release.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#include "dei_sc.h"
#ifdef PLATFORM_933
  #include "sense_rs.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Ss_Trunk_Release
{
    Vrtc vrtc ;
}Dei_Ss_Trunk_Release;

#ifdef PLATFORM_933
	Boolean dei_ss_trunk_input_active;
	Boolean dei_ss_prev_lock_status;
  Boolean dei_ss_trunk_release_cmd;
#endif
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_ss_trunk_release_start  ( void );
static void dei_ss_trunk_release_stop   ( void );
static void dei_ss_trunk_release_timeout( void );

static void dei_ss_trunk_release_aux_sensor_bypass( void );

#ifdef PLATFORM_933
	static void ngmm_ss_trunk_release_start  ( void );
	static void ngmm_ss_trunk_release_timeout( void );
	static void ngmm_ss_trunk_release_input_activated_timeout( void );
	static void ngmm_ss_trunk_sense_input_activated( void );
#endif

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Ss_Trunk_Release dei_ss_trunk_release_vars;
Vrtc dei_ss_trunk_release_input_activated;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_ss_trunk_release_init( void )
{
	dei_ss_trunk_release_vars.vrtc = vrtc_f_create( (Exec_Func)dei_ss_trunk_release_timeout ) ;
#ifdef PLATFORM_933	
	dei_ss_trunk_release_input_activated = vrtc_f_create( (Exec_Func) ngmm_ss_trunk_release_input_activated_timeout );
#endif	
	cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {
            { INPUT_OEM_LOCK                  , (Exec_Func)dei_ss_trunk_release_stop      },
            { INPUT_OEM_ARM                   , (Exec_Func)dei_ss_trunk_release_stop      },
        	{ INPUT_RF_LOCK                   , (Exec_Func)dei_ss_trunk_release_stop      }, 
        	{ INPUT_OEM_UNLOCK                , (Exec_Func)dei_ss_trunk_release_stop      },
            { INPUT_OEM_DISARM                , (Exec_Func)dei_ss_trunk_release_stop      },
        	{ INPUT_RF_UNLOCK                 , (Exec_Func)dei_ss_trunk_release_stop      },    				    
        	{ INPUT_OEM_TRUNK                 , (Exec_Func)dei_ss_trunk_release_start     },   
          { INPUT_RF_AUX3                   , (Exec_Func)dei_ss_trunk_release_aux_sensor_bypass	      },
        	{ INPUT_RF_AUX4                   , (Exec_Func)dei_ss_trunk_release_aux_sensor_bypass 	  },
        	{ INPUT_RF_AUX5                   , (Exec_Func)dei_ss_trunk_release_aux_sensor_bypass 	  },
        	{ INPUT_RF_AUX6                   , (Exec_Func)dei_ss_trunk_release_aux_sensor_bypass 	  },
        	{ INPUT_OEM_AUX1                  , (Exec_Func)dei_ss_trunk_release_aux_sensor_bypass 	  },
        	{ INPUT_OEM_AUX2                  , (Exec_Func)dei_ss_trunk_release_aux_sensor_bypass 	  },
#ifdef PLATFORM_933					
          { INPUT_RF_TRUNK  | INPUT_RF_EXT  , (Exec_Func)ngmm_ss_trunk_release_start     },   
					{ INPUT_TRUNK_STATUS_OPEN 				,	(Exec_Func)ngmm_ss_trunk_sense_input_activated					},
        	{ INPUT_TRUNK_STATUS_CLOSE        , (Exec_Func)ngmm_ss_trunk_release_timeout   },
#else
          { INPUT_RF_TRUNK  | INPUT_RF_EXT  , (Exec_Func)dei_ss_trunk_release_start     },   
        	{ INPUT_TRUNK_STATUS_CLOSE        , (Exec_Func)dei_ss_trunk_release_timeout   },
#endif					
        };

        CMD_LUT_INSERT1(cmd_array ,EXEC_MODE_NORMAL);
    }
}

//--------------------------------------------------------------------------//

void dei_ss_trunk_release_start( void )
{
	if( get_ss_arm() && !get_ss_valet() )
	{
		set_ss_trunk_release( TRUE );
		
				// bypass al sensors for 10 min timeout 
		sensor_bypass( ss_sensor[0] , DEI_SS_VRTC_TIMEOUT_TRUNK_RELEASE ); //10 minutes
		sensor_bypass( ss_sensor[1] , DEI_SS_VRTC_TIMEOUT_TRUNK_RELEASE ); //10 minutes
		sensor_bypass( ss_sensor[2] , DEI_SS_VRTC_TIMEOUT_TRUNK_RELEASE ); //10 minutes

		vrtc_stop  ( dei_ss_trunk_release_input_activated ) ; 

		vrtc_update( dei_ss_trunk_release_input_activated , DEI_SS_VRTC_TIMEOUT_ZONE_BYPASS );

		vrtc_start ( dei_ss_trunk_release_input_activated ) ;
	}
}

//--------------------------------------------------------------------------//

void dei_ss_trunk_release_stop( void )
{
	//if( ss.trunk_release_active )
	if( get_ss_trunk_release() )
	{
		//ss.trunk_release_active = FALSE; 
		set_ss_trunk_release( FALSE );

		vrtc_stop( dei_ss_trunk_release_vars.vrtc );
	}
}

//--------------------------------------------------------------------------//

void dei_ss_trunk_release_timeout( void )
{ 
	if( get_ss_arm() && get_ss_trunk_release()  && !get_ss_valet() )
	{
#ifdef PLATFORM_CM800
		sensor_bypass( ss_sensor , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
#else
		// bypass al sensors for 5 sec timeout 
		sensor_bypass( ss_sensor[0] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
		sensor_bypass( ss_sensor[1] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
		sensor_bypass( ss_sensor[2] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
		if(ext_sensor_trigger_configured)
		  sensor_bypass( ss_sensor_ext , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
#endif
		
		vrtc_stop( dei_ss_trunk_release_vars.vrtc ) ; 

		set_ss_arm( TRUE );
		
 		cmd_q_rs_insert( INPUT_ARM   , 100 ); 
#ifdef PLATFORM_933
		cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , CMD_LOCK_WITH_ARM ) ; 
#else
		cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)CMD_LOCK_WITH_ARM);
#endif
		if( dei_feature_get(DEI_FEAT_PARKING_LIGHT_OUTPUT) != 3 ) //parking light off
		{
		dei_prk_flash( get_rs_gwr() ? DEI_PRK_LOCK_RS : DEI_PRK_LOCK );
		}
		else
		{
				dei_prk_flash( DEI_PRK_LOCK );    
		}
	}
		
	set_ss_trunk_release( FALSE );
            	
}


//--------------------------------------------------------------------------//		 

void dei_ss_trunk_release_aux_sensor_bypass( void )
{
    if( get_ss_arm() )
    {
#ifdef PLATFORM_CM800
			    sensor_bypass( ss_sensor , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_AUX ); //15 seconds
#else
			// Bypass all sensors for 15 sec 
					sensor_bypass( ss_sensor[0] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_AUX ); //15 seconds
					sensor_bypass( ss_sensor[1] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_AUX ); //15 seconds
					sensor_bypass( ss_sensor[2] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_AUX ); //15 seconds
			    if(ext_sensor_trigger_configured)
						sensor_bypass( ss_sensor_ext , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_AUX ); //15 seconds
#endif
    }
}    

//--------------------------------------------------------------------------//


#ifdef PLATFORM_933
void ngmm_ss_trunk_release_start( void )
{
	dei_ss_trunk_release_cmd = TRUE;
	if(  !get_ss_valet() )
	{
		if(get_ss_lock() && (dei_feature_get( DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE )==DEI_UNLOCK_DISARM_WITH_TRUNK_SMART))
			dei_ss_prev_lock_status = TRUE;

		//vrtc_stop  ( dei_ss_trunk_release_input_activated ) ; 

		//vrtc_update( dei_ss_trunk_release_input_activated , DEI_SS_VRTC_TIMEOUT_ZONE_BYPASS );

		//vrtc_start ( dei_ss_trunk_release_input_activated ) ;
		
	}
}


//--------------------------------------------------------------------------//

void ngmm_ss_trunk_release_timeout( void )
{
	if( dei_ss_trunk_release_cmd  && !get_ss_valet() )
	{
#ifdef PLATFORM_CM800
		sensor_bypass( ss_sensor , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
#else
		// bypass al sensors for 5 sec timeout 
		sensor_bypass( ss_sensor[0] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
		sensor_bypass( ss_sensor[1] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
		sensor_bypass( ss_sensor[2] , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
		if(ext_sensor_trigger_configured)
			sensor_bypass( ss_sensor_ext , DEI_SS_VRTC_TIMEOUT_SENSOR_BYPASS_TRUNK_RELEASE ); //5 seconds
#endif
		
		vrtc_stop( dei_ss_trunk_release_vars.vrtc ) ; 

	}
	set_ss_trunk_release( FALSE );
}



//--------------------------------------------------------------------------//		 

void ngmm_ss_trunk_release_input_activated_timeout( void )
{
	if( dei_ss_trunk_input_active )
	{
		// start 10 min timer
		vrtc_stop  ( dei_ss_trunk_release_vars.vrtc ) ; 

		vrtc_update( dei_ss_trunk_release_vars.vrtc , DEI_SS_VRTC_TIMEOUT_TRUNK_RELEASE );

		vrtc_start ( dei_ss_trunk_release_vars.vrtc ) ;
		
		dei_ss_trunk_input_active = FALSE;

	}
	else
	{
		if(dei_ss_prev_lock_status && dei_feature_get( DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE )==DEI_UNLOCK_DISARM_WITH_TRUNK_SMART)
		{
			dei_ss_prev_lock_status = FALSE;
			set_ss_arm ( TRUE );
			cmd_q_rs_insert( INPUT_ARM            , 100 ) ; 						
#ifdef PLATFORM_933
			cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , CMD_UNLOCK_WITH_TRUNK_CLOSE ) ; 
#else
			cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)CMD_UNLOCK_WITH_TRUNK_CLOSE );
#endif
		}
		ngmm_ss_trunk_release_timeout();
	}
}


//--------------------------------------------------------------------------//		 

void ngmm_ss_trunk_sense_input_activated( void )
{
	dei_ss_trunk_input_active = TRUE ;
}

#endif
