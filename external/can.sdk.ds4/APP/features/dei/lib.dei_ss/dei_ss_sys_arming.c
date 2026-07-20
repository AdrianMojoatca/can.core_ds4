/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_sys_arming.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#include "dei_sc.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

void prg_out_ground_when_arm           ( Boolean ctrl );

 #define PASSIVE_ARM_DISABLE				 (dei_feature_get( DEI_FEAT_SYSTEM_ARMING_MODE )==1)
 #define PASSIVE_ARM_WITHOUT_LOCK			 (dei_feature_get( DEI_FEAT_SYSTEM_ARMING_MODE )==2)
 #define PASSIVE_ARM_WITH_LOCK				 (dei_feature_get( DEI_FEAT_SYSTEM_ARMING_MODE )==3)
 
 #ifdef PLATFORM_CM800
	 #define AUTOARMING_WITHOUT_LOCK			 (dei_feature_get( DEI_FEAT_SYSTEM_ARMING_MODE )==4)
	 #define AUTOARMING_WITH_LOCK				 (dei_feature_get( DEI_FEAT_SYSTEM_ARMING_MODE )==5) 
	 #define ONE_TIME_BYPASS_ENABLE              (dei_feature_get( DEI_FEAT_ONE_TIME_BYPASS )==1)

 typedef struct s_Dei_Ss_Sys_Arming
 {
    Boolean autoarming_enable   ; 
    Boolean otb_enable          ;
    Boolean req_lock ;

    Vrtc vrtc_sys_arming; 
    Vrtc vrtc_otb ;

    Exec_Func event ;     

}Dei_Ss_Sys_Arming;

#else
	 #define AUTOARMING_DISABLE						(dei_feature_get( DEI_FEAT_AUTO_REARM ) == 1)
	 #define AUTOARMING_WITHOUT_LOCK			(dei_feature_get( DEI_FEAT_AUTO_REARM )== 2)
	 #define AUTOARMING_WITH_LOCK					(dei_feature_get( DEI_FEAT_AUTO_REARM )== 3) 

 typedef struct s_Dei_Ss_Sys_Arming
 {
    Boolean autoarming_enable   ; 
	  Boolean passive_arming_enable ;
    Boolean otb_enable          ;
    Boolean req_lock ;

    Vrtc vrtc_sys_arming; 
    Vrtc vrtc_sys_arming_led_flash; 
    Vrtc vrtc_otb ;

    Exec_Func event ;     

}Dei_Ss_Sys_Arming;

	Dei_Led_Pattern pre_ign_on_led_pattern;

#endif

#define ARMTIMERTYPE_NONE				0
#define ARMTIMERTYPE_PASSIVEARM	1
#define ARMTIMERTYPE_AUTOREARM	2

int arm_timer_type=ARMTIMERTYPE_NONE;

                      
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void dei_ss_sys_arming_zone_open    ( void );
static void dei_ss_sys_arming_ign_on       ( void );
static void dei_ss_sys_arming_ign_off      ( void );
static void dei_ss_sys_arming_zone_ign_off ( void );
static void dei_ss_sys_arming_zone_close   ( void );
static void dei_ss_sys_arming_lock         ( void );
static void dei_ss_sys_arming_unlock       ( void );
static void dei_ss_sys_arming_led          ( void );
static void dei_ss_sys_arming_chirp        ( void );
static void dei_ss_sys_arming_timeout      ( void );

static void dei_ss_autoarming_zone_open    ( void );
static void dei_ss_autoarming_zone_close	 ( void );
static void dei_ss_autoarming_lock         ( void );
static void dei_ss_autoarming_unlock       ( void );

static void dei_ss_otb_ignition_on         ( void );
static void dei_ss_otb_ignition_off        ( void );
static void dei_ss_otb_timeout             ( void );

static void dei_ss_sys_auto_arm_lock       ( void );

#ifdef PLATFORM_933
static void ngmm_ss_sys_arming_disable_open( void );
static void ngmm_ss_sys_arming_enable_close( void );
static void ngmm_ign_autoarm_check         ( void );
static void dei_ss_sys_arming_brake_press  ( void );
#endif
void dei_ss_sys_arming_led_flash_timeout( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Dei_Ss_Sys_Arming        dei_ss_sys_arming_vars;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
#ifdef PLATFORM_933
Dei_Led_Pattern dei_ss_led_disarm(void)
{
Dei_Led_Pattern pat;
	
	switch(get_ss_last_trigger())
	{
		case DEI_SS_TRIGGER_ZONE_DOOR:
      pat = DEI_LED_DOOR_TRIGGERED;
      break;
		
    case DEI_SS_TRIGGER_ZONE_HOOD:
      pat = DEI_LED_HOOD_TRIGGERED;
      break;
		
    case DEI_SS_TRIGGER_ZONE_TRUNK:
			pat = DEI_LED_TRUNK_TRIGGERED;
      break;
		
    case DEI_SS_TRIGGER_ZONE_IGN:
      pat = DEI_LED_IGN_TRIGGERED;
      break;
		
    case DEI_SS_TRIGGER_ZONE_SENSOR:
			switch(get_trig_sensor_index())
      {
        case 1:
					pat = DEI_LED_SHOCK_SENSOR_TRIGGERED;
				  break;
				case 2:
					pat = DEI_LED_TILT_SENSOR_TRIGGERED;
				  break;
        default:
          pat = DEI_LED_UNKNOWN_SENSOR_TRIGGERED;
				  break;
			}
			break;
			
		default:
		  pat = DEI_LED_NONE;
		  break;
	}
  return (Dei_Led_Pattern)(pat | DEI_LED_DISARM);
}
#endif


void dei_ss_sys_arming_init( void )
{		
    dei_ss_sys_arming_vars.autoarming_enable		= FALSE ; 
#ifdef PLATFORM_933	
	  dei_ss_sys_arming_vars.passive_arming_enable= FALSE ;
#endif
    dei_ss_sys_arming_vars.otb_enable       		= FALSE ;
    dei_ss_sys_arming_vars.req_lock         		= FALSE ;
    dei_ss_sys_arming_vars.event            		= NULL  ;    

    dei_ss_sys_arming_vars.vrtc_sys_arming = vrtc_f_create( (Exec_Func)dei_ss_sys_arming_timeout ) ;
    dei_ss_sys_arming_vars.vrtc_sys_arming_led_flash = vrtc_f_create( (Exec_Func)dei_ss_sys_arming_led_flash_timeout ) ;

	//TRACE("DEI_FEAT_SYSTEM_ARMING_MODE=%d\r\n" ,dei_feature_get( DEI_FEAT_SYSTEM_ARMING_MODE ));
	
	cmd_lut_insert( INPUT_IGN_STATUS_ON 	, (Exec_Func)dei_ss_sys_arming_ign_on, EXEC_MODE_NORMAL);
	cmd_lut_insert( INPUT_IGN_STATUS_OFF 	, (Exec_Func)dei_ss_sys_arming_ign_off, EXEC_MODE_NORMAL);
	
	if( PASSIVE_ARM_WITHOUT_LOCK || PASSIVE_ARM_WITH_LOCK )
	{
		//PASSIVE ARMING INIT
        //cmd_register1:
        {
            static const Cmd_Array1 cmd_array[] = 
            {
            	{ INPUT_ALL_DOOR_STATUS_CLOSE   , (Exec_Func)dei_ss_sys_arming_zone_close  },
            	{ INPUT_ALL_DOOR_STATUS_OPEN 	, (Exec_Func)dei_ss_sys_arming_zone_open   },
//            	{ INPUT_IGN_STATUS_ON 			    , (Exec_Func)dei_ss_sys_arming_ign_on      },
            	{ INPUT_RF_LOCK                 , (Exec_Func)dei_ss_sys_arming_lock        },
            	{ INPUT_RF_UNLOCK               , (Exec_Func)dei_ss_sys_arming_unlock      },
                { INPUT_RF_SILENT_LOCK        	, (Exec_Func)dei_ss_sys_arming_lock        },
                { INPUT_RF_SILENT_UNLOCK      	, (Exec_Func)dei_ss_sys_arming_unlock      },
            	{ INPUT_OEM_LOCK                , (Exec_Func)dei_ss_sys_arming_lock        },
                { INPUT_OEM_ARM                 , (Exec_Func)dei_ss_sys_arming_lock        },
#ifdef PLATFORM_CM800
                { INPUT_TRUNK_STATUS_CLOSE      , (Exec_Func)dei_ss_sys_arming_zone_close  },
                { INPUT_TRUNK_STATUS_OPEN 		, (Exec_Func)dei_ss_sys_arming_zone_open   },
                { INPUT_HOOD_STATUS_CLOSE       , (Exec_Func)dei_ss_sys_arming_zone_close  },
                { INPUT_HOOD_STATUS_OPEN 		, (Exec_Func)dei_ss_sys_arming_zone_open   },
            	{ INPUT_RF_UNLOCK               , (Exec_Func)dei_ss_sys_arming_unlock      },
            	{ INPUT_OEM_UNLOCK              , (Exec_Func)dei_ss_sys_arming_unlock      },
            	{ INPUT_OEM_DISARM              , (Exec_Func)dei_ss_sys_arming_unlock      },
#else
                { INPUT_TRUNK_STATUS_CLOSE      , (Exec_Func)ngmm_ss_sys_arming_enable_close  },
                { INPUT_TRUNK_STATUS_OPEN 		, (Exec_Func)ngmm_ss_sys_arming_disable_open   },
                { INPUT_HOOD_STATUS_CLOSE       , (Exec_Func)ngmm_ss_sys_arming_enable_close  },
                { INPUT_HOOD_STATUS_OPEN 		, (Exec_Func)ngmm_ss_sys_arming_disable_open   },
                { INPUT_BRAKE_STATUS_PRESS      , (Exec_Func)dei_ss_sys_arming_brake_press },
#endif
            };

            CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
        }
#ifdef PLATFORM_933				
  			dei_ss_sys_arming_vars.passive_arming_enable = FALSE ;       
#endif
	};

	if( AUTOARMING_WITHOUT_LOCK || AUTOARMING_WITH_LOCK )
	{
		//AUTOARMING INIT

    //    cmd_register2:
        {
    		static const Cmd_Array1 cmd_array[] = {            
            { INPUT_TRUNK_STATUS_OPEN 		  	, (Exec_Func)dei_ss_autoarming_zone_open  },
        		{ INPUT_HOOD_STATUS_OPEN 		  		, (Exec_Func)dei_ss_autoarming_zone_open  },
        		{ INPUT_ALL_DOOR_STATUS_OPEN 	  	, (Exec_Func)dei_ss_autoarming_zone_open  },
						
            { INPUT_TRUNK_STATUS_CLOSE 		  	, (Exec_Func)dei_ss_autoarming_zone_close  },
        		{ INPUT_HOOD_STATUS_CLOSE 		  	, (Exec_Func)dei_ss_autoarming_zone_close  },
        		{ INPUT_ALL_DOOR_STATUS_CLOSE 	  , (Exec_Func)dei_ss_autoarming_zone_close  },
						
        		{ INPUT_RF_LOCK                   , (Exec_Func)dei_ss_autoarming_lock       },
            { INPUT_RF_SILENT_LOCK            , (Exec_Func)dei_ss_autoarming_lock       },
            { INPUT_RF_UNLOCK                 , (Exec_Func)dei_ss_autoarming_unlock     },
            { INPUT_RF_SILENT_UNLOCK          , (Exec_Func)dei_ss_autoarming_unlock     },
          	{ INPUT_OEM_LOCK                  , (Exec_Func)dei_ss_autoarming_lock       },
           	{ INPUT_OEM_UNLOCK                , (Exec_Func)dei_ss_autoarming_unlock     },
            { INPUT_OEM_ARM                   , (Exec_Func)dei_ss_autoarming_lock       },
          	{ INPUT_OEM_DISARM                , (Exec_Func)dei_ss_autoarming_unlock     }
            };

            CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
        }
				
		dei_ss_sys_arming_vars.autoarming_enable = FALSE ;       
	};

#ifdef PLATFORM_CM800
	if( ONE_TIME_BYPASS_ENABLE )
	{
		//ONE TIME BYPASS INIT//
		//!!!the events should be registered last( before autoarming and passive arming events )//

		dei_ss_sys_arming_vars.vrtc_otb = vrtc_f_create( (Exec_Func)dei_ss_otb_timeout ) ;

	  // 	cmd_register3:
        { 
            static const Cmd_Array1 cmd_array[] =
            {
                { INPUT_IGN_STATUS_OFF     , (Exec_Func)dei_ss_otb_ignition_off },
            	{ INPUT_IGN_STATUS_ON	   , (Exec_Func)dei_ss_otb_ignition_on  },
        		{ INPUT_RF_LOCK            , (Exec_Func)dei_ss_otb_timeout      },
                { INPUT_RF_SILENT_LOCK     , (Exec_Func)dei_ss_otb_timeout      },
            	{ INPUT_RF_UNLOCK          , (Exec_Func)dei_ss_otb_timeout      },
                { INPUT_RF_SILENT_UNLOCK   , (Exec_Func)dei_ss_otb_timeout      },
            	{ INPUT_OEM_LOCK           , (Exec_Func)dei_ss_otb_timeout      },
            	{ INPUT_OEM_UNLOCK         , (Exec_Func)dei_ss_otb_timeout      },
                { INPUT_OEM_ARM            , (Exec_Func)dei_ss_otb_timeout      },
            	{ INPUT_OEM_DISARM         , (Exec_Func)dei_ss_otb_timeout      }
            };

            CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
        }

		dei_ss_sys_arming_vars.otb_enable = FALSE ;
		
	}
#endif
	        
}


//--------------------------------------------------------------------------//
//PASSIVE ARMING EVENTS//
//--------------------------------------------------------------------------//
#ifdef PLATFORM_933
void ngmm_ss_sys_arming_disable_open( void )
{
	
	if(dei_ss_sys_arming_vars.passive_arming_enable)
	{
		// stop passive arming timer
		dei_led_flash( DEI_LED_RESET ,TRUE );
		vrtc_stop( dei_ss_sys_arming_vars.vrtc_sys_arming );
	}
	
}

void ngmm_ss_sys_arming_disable(void) {
	
	dei_led_flash( DEI_LED_RESET ,TRUE );
	vrtc_stop( dei_ss_sys_arming_vars.vrtc_sys_arming );
}

//--------------------------------------------------------------------------//

void dei_ss_sys_arming_brake_press( void )
{
	if(dei_ss_sys_arming_vars.passive_arming_enable)
	{
		// stop passive arming timer
        dei_ss_sys_arming_vars.passive_arming_enable = FALSE;
		dei_led_flash( DEI_LED_RESET ,TRUE );
		vrtc_stop( dei_ss_sys_arming_vars.vrtc_sys_arming );
	}
	
}

void dei_ss_sys_arming_led_flash_timeout( void )
{
	if( !get_ss_arm() )
	{
		dei_led_flash( DEI_LED_ARM_NOTIFICATION , LED_ENABLE);
	}
}


//--------------------------------------------------------------------------//

void ngmm_ss_sys_arming_enable_close( void )
{
	if(dei_ss_sys_arming_vars.passive_arming_enable)
	{
		// enable timer after closing the zone
		dei_ss_sys_arming_zone_close();
	}
}

#endif
	
//--------------------------------------------------------------------------//
	
void dei_ss_sys_arming_zone_open( void )
{    
    vrtc_stop( dei_ss_sys_arming_vars.vrtc_sys_arming );
	
	ATOMIC
    ( 
        dei_ss_sys_arming_vars.event    = NULL  ;
#ifdef PLATFORM_933	
  			dei_ss_sys_arming_vars.autoarming_enable = FALSE ;       
#endif
        dei_ss_sys_arming_vars.req_lock = FALSE ; 
    ) 

	if( get_ss_arm() || get_ss_valet() ) return ;
	
	//restore LED status only if ignition is off

	if(!get_ign_status())	{
		if(!get_ss_valet()) {
			if(get_ss_last_trigger()==DEI_SS_NO_TRIGGER) {
				dei_led_flash( DEI_LED_RESET , TRUE ); 
				}
			}
		else {
			dei_led_flash( DEI_LED_VALET_ON , LED_ENABLE );
			}
		}
	else {
		dei_led_flash( DEI_LED_RESET ,TRUE );
		}	
}

void dei_ss_sys_arming_save_led_pattern(Dei_Led_Pattern pattern) {
	
	pre_ign_on_led_pattern=pattern;
}

//--------------------------------------------------------------------------//

void dei_ss_sys_arming_ign_on( void ) {
	
	if( !get_rs_gwr() ) {
		vrtc_stop( dei_ss_sys_arming_vars.vrtc_sys_arming );
   	ATOMIC ( 
			dei_ss_sys_arming_vars.event    = NULL  ;
            dei_ss_sys_arming_vars.req_lock = FALSE ; 
#ifdef PLATFORM_933			
			dei_ss_sys_arming_vars.passive_arming_enable = FALSE;
#endif
            ) 
    	
		pre_ign_on_led_pattern=get_led_last_pattern();

		if(!get_ss_valet()) {
			dei_led_flash( DEI_LED_RESET , TRUE );
			}
		}	
}

void dei_ss_sys_arming_ign_off( void ) 
{    
		if(get_ss_arm() && !get_rs_gwr()) 
		{
			if(system_type_get()==SYS_TYPE_RS) { // RS only
				dei_led_flash(pre_ign_on_led_pattern,TRUE);
			}				
		}
}

//--------------------------------------------------------------------------//

void dei_ss_sys_arming_zone_ign_off( void) {
	
	if( !get_ss_arm() && !get_ss_one_time_bypass() && !get_ss_valet() )	{
#ifdef PLATFORM_933		
			//dei_ss_sys_arming_vars.autoarming_enable = TRUE ;       
			dei_ss_sys_arming_vars.passive_arming_enable = TRUE ;       
#endif		
			dei_ss_sys_arming_unlock();
			}
    ATOMIC( dei_ss_sys_arming_vars.req_lock = FALSE );             
}


void dei_ss_sys_arming_zone_close( void ) {
	
	arm_timer_type=ARMTIMERTYPE_PASSIVEARM;
	
    if( !get_ss_arm() && !get_ss_one_time_bypass() && !get_ss_valet() && (!get_ign_status() || get_rs_gwr()))	{

        #ifdef PLATFORM_933		
		dei_ss_sys_arming_vars.autoarming_enable = TRUE ;       
		dei_ss_sys_arming_vars.passive_arming_enable = TRUE ; 
        #endif	
        
		dei_ss_sys_arming_unlock();
			
		if(PASSIVE_ARM_WITH_LOCK || PASSIVE_ARM_WITHOUT_LOCK ) {
			vrtc_update( dei_ss_sys_arming_vars.vrtc_sys_arming , DEI_SS_VRTC_TIMEOUT_SYS_ARMING_CHIRP );
			vrtc_start ( dei_ss_sys_arming_vars.vrtc_sys_arming );
			}
		//else vrtc_stop(dei_ss_sys_arming_vars.vrtc_sys_arming );
    }
    
    ATOMIC( dei_ss_sys_arming_vars.req_lock = FALSE );             
}

//--------------------------------------------------------------------------//

void dei_ss_sys_arming_unlock( void )
{
	if( (!get_door_status ()||get_ss_door_bypass () ) && 
        (!get_trunk_status()||get_ss_trunk_bypass() ) && 
        (!get_hood_status ()||get_ss_hood_bypass () ) && 
         !get_ss_valet() && (!get_ign_status() || get_rs_gwr()) )
	{
		if( !get_ss_door_trigger() && !get_ss_hood_trigger() && !get_ss_trunk_trigger() && !get_ss_sensor_trigger(1) && !get_ss_sensor_trigger(2) && !get_ss_sensor_trigger(0) )
        {
            if ( dei_ss_sys_arming_vars.passive_arming_enable )
            {
                vrtc_update( dei_ss_sys_arming_vars.vrtc_sys_arming_led_flash , 1 );
                vrtc_start ( dei_ss_sys_arming_vars.vrtc_sys_arming_led_flash );
            }
        }
	
		ATOMIC( dei_ss_sys_arming_vars.event = (Exec_Func)dei_ss_sys_arming_chirp );
         
        // This is a Passive Arming function. AUTOARMING should not be started or stopped here
//        if(AUTOARMING_WITHOUT_LOCK || AUTOARMING_WITH_LOCK ) { 
//            vrtc_update( dei_ss_sys_arming_vars.vrtc_sys_arming , DEI_SS_VRTC_TIMEOUT_SYS_ARMING_CHIRP );
//            vrtc_start ( dei_ss_sys_arming_vars.vrtc_sys_arming );
//            }
//        else vrtc_stop(dei_ss_sys_arming_vars.vrtc_sys_arming );
	}
	pre_ign_on_led_pattern=get_led_last_pattern();
}

//--------------------------------------------------------------------------//

void dei_ss_sys_arming_lock( void )
{
	vrtc_stop( dei_ss_sys_arming_vars.vrtc_sys_arming ) ; 
    dei_ss_sys_arming_vars.passive_arming_enable = FALSE;
	ATOMIC
    ( 
        dei_ss_sys_arming_vars.event    = NULL ; 
        dei_ss_sys_arming_vars.req_lock = TRUE ;
    )
	
	pre_ign_on_led_pattern=get_led_last_pattern();
}

//--------------------------------------------------------------------------//


void dei_ss_sys_arming_chirp( void )
{
 	if( !get_ss_valet() )
	{
		#define SILENT_MODE	((dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) == 3) || (dei_feature_get(DEI_FEAT_CONFIRMATION_CHIRPS) == 4))
				
	    if( !SILENT_MODE )
		{
			dei_siren_chirp( DEI_SIREN_ARM_NOTIFICATION );
		}
        		
	    #undef SILENT_MODE 	
	
        
		ATOMIC( dei_ss_sys_arming_vars.event = (Exec_Func)dei_ss_sys_auto_arm_lock );

		vrtc_update( dei_ss_sys_arming_vars.vrtc_sys_arming , DEI_SS_VRTC_TIMEOUT_SYS_ARMING_LOCK );

		vrtc_start( dei_ss_sys_arming_vars.vrtc_sys_arming );
	}
}

//--------------------------------------------------------------------------//

void dei_ss_sys_arming_timeout( void )
{
	if( dei_ss_sys_arming_vars.event )
	{
		(dei_ss_sys_arming_vars.event)(NULL); 
	}
	prg_out_ground_when_arm( get_ss_arm() );

}

//--------------------------------------------------------------------------//
//AUTOARMING EVENTS//
//--------------------------------------------------------------------------//

void dei_ss_autoarming_zone_open( void )
{
    vrtc_stop( dei_ss_sys_arming_vars.vrtc_sys_arming ) ; 

	ATOMIC( dei_ss_sys_arming_vars.event = (Exec_Func)NULL );

    ATOMIC(dei_ss_sys_arming_vars.autoarming_enable = FALSE) ; 

 	if( !get_ss_arm() )
	{
	 	dei_led_flash( DEI_LED_DISARM , LED_ENABLE ) ; 
	}
	

}	

void dei_ss_autoarming_zone_close(void) {
	
	ATOMIC(dei_ss_sys_arming_vars.autoarming_enable = TRUE) ; 
	}

//--------------------------------------------------------------------------//

void dei_ss_autoarming_unlock( void )
{  
	arm_timer_type=ARMTIMERTYPE_AUTOREARM;

	if( get_ign_status() && !get_rs_gwr() )
    {
        dei_led_flash( DEI_LED_DISARM , LED_ENABLE ) ;
        
        ATOMIC(dei_ss_sys_arming_vars.autoarming_enable = FALSE); 
    }
    else
    {        
      if( dei_ss_sys_arming_vars.autoarming_enable && !get_ss_valet() )
    	{
    		if( !get_ss_door_trigger() && !get_ss_hood_trigger() && !get_ss_trunk_trigger() && !get_ss_ign_trigger() && !get_ss_sensor_trigger(1) && !get_ss_sensor_trigger(2) && !get_ss_sensor_trigger(0) )
            {            
							vrtc_update( dei_ss_sys_arming_vars.vrtc_sys_arming_led_flash , 1 );
							vrtc_start ( dei_ss_sys_arming_vars.vrtc_sys_arming_led_flash );
            }
    	
    		ATOMIC( dei_ss_sys_arming_vars.event = (Exec_Func)dei_ss_sys_arming_chirp );
    
    		vrtc_update( dei_ss_sys_arming_vars.vrtc_sys_arming , DEI_SS_VRTC_TIMEOUT_SYS_ARMING_CHIRP );
    
    		vrtc_start ( dei_ss_sys_arming_vars.vrtc_sys_arming );
    	}
    }
}

//--------------------------------------------------------------------------//	

void dei_ss_autoarming_lock( void )
{
 	if( (!get_door_status ()||get_ss_door_bypass () ) && 
        (!get_trunk_status()||get_ss_trunk_bypass() ) && 
        (!get_hood_status ()||get_ss_hood_bypass () ) && 
        (!get_ign_status () || get_rs_gwr()) )
    {    
        dei_ss_sys_arming_vars.autoarming_enable = TRUE ; 
    }

	vrtc_stop( dei_ss_sys_arming_vars.vrtc_sys_arming ) ; 
	prg_out_ground_when_arm( get_ss_arm() ); //MJB

	ATOMIC( dei_ss_sys_arming_vars.event = (Exec_Func)NULL );
}



//--------------------------------------------------------------------------//	
//ONE TIME BYPASS EVENTS//
//--------------------------------------------------------------------------//	

void dei_ss_otb_ignition_on( void )
{
	if( !get_ss_arm() && !get_rs_gwr() )
	{
		vrtc_stop( dei_ss_sys_arming_vars.vrtc_otb ) ; 
	
		vrtc_update( dei_ss_sys_arming_vars.vrtc_otb , DEI_SS_VRTC_TIMEOUT_ONE_TIME_BYPASS );

		dei_ss_sys_arming_vars.otb_enable = TRUE ; 

		vrtc_start( dei_ss_sys_arming_vars.vrtc_otb ) ;
	}
	else
	{
		dei_ss_sys_arming_vars.otb_enable = FALSE ;
	}
}

//--------------------------------------------------------------------------//	

void dei_ss_otb_ignition_off( void )
{
	if( dei_ss_sys_arming_vars.otb_enable )
	{
		vrtc_stop( dei_ss_sys_arming_vars.vrtc_otb ) ; 

		set_ss_one_time_bypass( TRUE );
		
		dei_siren_chirp( DEI_SIREN_ARM )  ;  
	}
	else
	{
		if( !get_ss_arm() )
		{
			set_ss_one_time_bypass( FALSE );
		}
	}

	dei_ss_sys_arming_vars.otb_enable = FALSE ;

}

//--------------------------------------------------------------------------//	

void dei_ss_otb_timeout( void )
{
	dei_ss_sys_arming_vars.otb_enable = FALSE ;
} 		

//--------------------------------------------------------------------------//	
//--------------------------------------------------------------------------//	
//AUTO ARM LOCK FUNCTION//
//--------------------------------------------------------------------------//	
//--------------------------------------------------------------------------//	

#ifdef PLATFORM_933
void dei_ss_sys_auto_arm_lock( void ) {
	Boolean dolock;
	
	if( get_ss_valet() ) return ;
	
	if( (PASSIVE_ARM_WITHOUT_LOCK && dei_ss_sys_arming_vars.req_lock && dei_ss_sys_arming_vars.passive_arming_enable) ||\
			(PASSIVE_ARM_WITH_LOCK && dei_ss_sys_arming_vars.passive_arming_enable ) ||\
     	(AUTOARMING_WITH_LOCK && dei_ss_sys_arming_vars.autoarming_enable) ||\
     	(AUTOARMING_WITHOUT_LOCK && dei_ss_sys_arming_vars.autoarming_enable)
			)
	{		
		
		if(arm_timer_type==ARMTIMERTYPE_PASSIVEARM) {
			dolock = (PASSIVE_ARM_WITH_LOCK && dei_ss_sys_arming_vars.passive_arming_enable);
		}
		else if(arm_timer_type==ARMTIMERTYPE_AUTOREARM) {
			dolock = (AUTOARMING_WITH_LOCK && dei_ss_sys_arming_vars.autoarming_enable);
		}
		else dolock=0;
		
		if(PASSIVE_ARM_WITHOUT_LOCK && AUTOARMING_WITHOUT_LOCK)
		{
			dei_ss_sys_arming_vars.req_lock=FALSE;
		}
		dei_ss_sys_arming_vars.passive_arming_enable = FALSE;
		dei_ss_sys_arming_vars.autoarming_enable = FALSE;
		if(dolock /*dei_ss_sys_arming_vars.req_lock*/) {
			cmd_q_rs_insert( INPUT_RF_SILENT_LOCK , 100 , 0 );
			}
		else {
			cmd_q_rs_insert( INPUT_OEM_LOCK , 100 , (void*)DEI_SS_PASSIVE_NO_LOCK );
			}
		}
	else {
		cmd_q_rs_insert( INPUT_OEM_LOCK , 100 , (void*)DEI_SS_PASSIVE_NO_LOCK );

		dei_ss_sys_arming_vars.passive_arming_enable = FALSE;
		}

	if( dei_feature_get(DEI_FEAT_PLIGHT_CONFIRMATION) != 2 ) {
		dei_prk_flash( get_rs_gwr() ? DEI_PRK_LOCK_RS : DEI_PRK_LOCK );
		}		
	if(get_headlight_wire_status()== TRUE) { // If headlights are ON, notify user
		// send headlight alert now
		dei_sc_set_alert_event( 0x02 );
		notify_send( NOTIFY_CONVENIENCE_ALERT  ) ; 			
		}
	}


#else

void dei_ss_sys_auto_arm_lock( void )
{
	if( get_ss_valet() ) return ;
	
	if( (PASSIVE_ARM_WITHOUT_LOCK && dei_ss_sys_arming_vars.req_lock )||PASSIVE_ARM_WITH_LOCK  || AUTOARMING_WITH_LOCK )
	{							 		
		cmd_q_rs_insert( INPUT_RF_SILENT_LOCK , 100 , 0 );
	}
	else
	{
			cmd_q_rs_insert( INPUT_OEM_LOCK , 100 , (void*)DEI_SS_SILENT );
	
			if( dei_feature_get(DEI_FEAT_PARKING_LIGHT_OUTPUT) != 3 ) //parking light off
			{
					dei_prk_flash( get_rs_gwr() ? DEI_PRK_LOCK_RS : DEI_PRK_LOCK );
			}
			else
			{
					dei_prk_flash(DEI_PRK_LOCK);
			}
	}		
}
#endif

//--------------------------------------------------------------------------//		

#undef PASSIVE_ARM_DISABLE
#undef PASSIVE_ARM_WITHOUT_LOCK
#undef PASSIVE_ARM_WITH_LOCK
#undef AUTOARMING_WITHOUT_LOCK
#undef AUTOARMING_WITH_LOCK
#undef ONE_TIME_BYPASS_ENABLE
