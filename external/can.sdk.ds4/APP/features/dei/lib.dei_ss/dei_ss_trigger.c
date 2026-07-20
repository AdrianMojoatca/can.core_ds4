/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_trigger.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
	#include "sensor_proto.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Ss_Trigger
{
    Vrtc	   vrtc; 
    Boolean    in_progress;
    UInt8      cycle_count;
}Dei_Ss_Trigger;    

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_ss_trigger_active     ( void );
static void dei_ss_trigger_timeout    ( void ); 
static void dei_ss_trigger_mem_update ( Dei_Ss_Trigger_Source trg_zone );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Ss_Trigger   dei_ss_trigger_vars;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_ss_trigger_init( void  )
{	
	dei_ss_trigger_vars.vrtc = vrtc_f_create( (Exec_Func)dei_ss_trigger_timeout );	

    dei_ss_trigger_vars.in_progress = FALSE ; 
}

//--------------------------------------------------------------------------//
UInt8 get_ss_trigger_source(void)
{
	return (UInt8)current_trg_zone;
}
//--------------------------------------------------------------------------//

void dei_ss_trigger_start( Dei_Ss_Trigger_Source trg_zone )
{		
	if( dei_ss_trigger_enable == FALSE ) return ;
    
	current_trg_zone = trg_zone;
	
	set_ss_last_trigger( trg_zone );
	
	set_ss_trigger( TRUE );

	dei_ss_trigger_mem_update( trg_zone );
	
	set_ss_sensor_trigger_all( FALSE );  //init ss_sensor_trigger
	
	switch( trg_zone )
	{
		case DEI_SS_TRIGGER_ZONE_SENSOR  :
		{
			if( !dei_ss_trigger_vars.in_progress )
            {
                dei_ss_trigger_vars.cycle_count = 3 ;
            }			
						
			set_ss_sensor_trigger( TRUE );
		}
		break;
		
		case DEI_SS_TRIGGER_ZONE_HOOD     :
		{
			if( !dei_ss_trigger_vars.in_progress )
            {
                dei_ss_trigger_vars.cycle_count = 3 ; 
            }
						
			set_ss_hood_trigger( TRUE );
		}
		break ;

		case DEI_SS_TRIGGER_ZONE_TRUNK    :
		{
			if( !dei_ss_trigger_vars.in_progress )
            {
                dei_ss_trigger_vars.cycle_count = 3 ; 
            }
						
			set_ss_trunk_trigger( TRUE );
		}
		break ;

		case DEI_SS_TRIGGER_ZONE_IGN :
		{
			if( !dei_ss_trigger_vars.in_progress )
            {
                dei_ss_trigger_vars.cycle_count = 3 ; 
            }
						
			set_ss_ign_trigger( TRUE );

		}
		break ;

		case DEI_SS_TRIGGER_ZONE_DOOR     :
		{
			if( !dei_ss_trigger_vars.in_progress )
            {
                dei_ss_trigger_vars.cycle_count = 3 ; 
            }
						
			set_ss_door_trigger( TRUE );
		}
		break ;
		
#ifdef PLATFORM_933		
		case DEI_SS_TRIGGER_ZONE_SENSOR2 :
		{
			if( !dei_ss_trigger_vars.in_progress )
            {
                dei_ss_trigger_vars.cycle_count = 3 ;
            }			
						
			set_ss_sensor_trigger( TRUE );
		}
		break;

		case DEI_SS_TRIGGER_ZONE_SENSOR3 :
		{
			if( !dei_ss_trigger_vars.in_progress )
            {
                dei_ss_trigger_vars.cycle_count = 3 ;
            }			
						
			set_ss_sensor_trigger( TRUE );
		}
		break;

#endif		
		
		default :
		{
			;
		}
		break;
	};

	dei_ss_trigger_active();	
	
	cmd_q_rs_insert( INPUT_TRIGGER_ON  , 100 );    
}

//--------------------------------------------------------------------------//	

void dei_ss_trigger_stop( void )
{	
	if( get_ss_trigger()  )
    {        
		set_ss_trigger( FALSE );
        
        dei_prk_flash( DEI_PRK_TRIGGER_STOP );
        
        dei_siren_chirp( DEI_SIREN_STOP );
        
        cmd_q_rs_insert( INPUT_TRIGGER_OFF  , 100 );
		
        dei_ss_trigger_vars.in_progress = FALSE ; 
	    
	    vrtc_stop( dei_ss_trigger_vars.vrtc ) ; 
    }
}

//--------------------------------------------------------------------------//	
void dei_ss_trigger_mem_update( Dei_Ss_Trigger_Source trg_zone )
{
#ifndef PLATFORM_933	//DS4 will always update event mem in order		
	UInt8 i ;

	Boolean event_exist = FALSE  ; 

	for( i = 0 ; i < SS_TRIGGER_MEM_SIZE ; i ++ )
	{
		if( get_ss_memory_trigger_event(i) == trg_zone )
		{
			event_exist = TRUE  ; 
		}
	}

	if( !event_exist )
#endif		
	{
		set_ss_memory_trigger_event( trg_zone );
	}		
}

//--------------------------------------------------------------------------//	

void dei_ss_trigger_active( void )
{
	UInt32 trig_src = 0;
	static const UInt32 trigger_source[] = 
	{
		NOTIFY_PLD_TRIG_TRUNK , NOTIFY_PLD_TRIG_S1 , NOTIFY_PLD_TRIG_DOOR  , NOTIFY_PLD_TRIG_IGN , NOTIFY_PLD_TRIG_HOOD, NOTIFY_PLD_TRIG_S2, NOTIFY_PLD_TRIG_S3, NOTIFY_PLD_TRIG_CUSTOM
	};
				 								
	led_update:
	{
		dei_led_flash ( DEI_SS_LED_ALARM_LAST_TRIG , LED_ENABLE );		
	}

	
//    if( !dei_ss_trigger_vars.in_progress )
//    {    
        remote_update:
	    {	  
			trig_src = get_ss_last_trigger() - 1;
		    notify_send( NOTIFY_TRIGGER , trigger_source[ trig_src ] ) ;
	    }
	                    
        dei_ss_trigger_vars.in_progress = TRUE ;
    
        vrtc_stop  ( dei_ss_trigger_vars.vrtc ); 		
    	
        vrtc_update( dei_ss_trigger_vars.vrtc , VRTC_1SEC * dei_feature_get( DEI_FEAT_SIREN_DURATION ) ); 
        
        siren_prk_update:
    	{    	  
			if( !((get_ss_sensor_silent_arm() && 
				     ((get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_SENSOR) || (get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_SENSOR2) || (get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_SENSOR3)))
				|| (get_ss_full_silent_arm() && (get_ss_last_trigger() != DEI_SS_TRIGGER_ZONE_IGN))) )   //ext sensor
    		{    						  
			    if( ((get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_DOOR) || (get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_IGN)) && (dei_ss_trigger_vars.cycle_count == 3) )   
				{				
					dei_siren_chirp( DEI_SIREN_CHIRP_THEN_TRIGGER ) ; 
				}
				else
				{
					dei_siren_chirp( DEI_SIREN_TRIGGER ) ; 	
				}
                                
                dei_prk_flash( DEI_PRK_TRIGGER ) ; //TRIG
				
    		}
    	}
            	
    	vrtc_start( dei_ss_trigger_vars.vrtc );
//    }
}	

//--------------------------------------------------------------------------//	

void dei_ss_trigger_timeout( void )	
{
	
	#define DOOR   (!get_ss_door_bypass() &&  get_door_status() )
    #define HOOD   (!get_ss_hood_bypass() &&  get_hood_status() )
    #define TRUNK  (!get_ss_trunk_bypass()&&  get_trunk_status())
    #define IGN    get_ign_status() 
#ifdef PLATFORM_933
	#define SENSOR (((get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_SENSOR) || (get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_SENSOR2) || (get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_SENSOR3)) && (get_sensor_status()))
#else
	#define SENSOR ((get_ss_last_trigger() == DEI_SS_TRIGGER_ZONE_SENSOR) && get_sensor_status())
#endif
	
	UInt8 stat_bit_mask = 0x04;		// Mask bit to get sensors in alarm state
	UInt16 evt_state;
	Dei_Ss_Trigger_Source trig_zone = DEI_SS_NO_TRIGGER;

	if( !get_ss_trigger() )return  ;
    
    if( !dei_ss_trigger_vars.in_progress  )
    {
        dei_ss_trigger_active();

        return ;
    }    
	
		#ifdef DEV_LPC_176x
		set_sensor_status(FALSE);
		
		// clear all sensor status
		
		// send broadcast message to check is any sensor is still in ALARM
		sensor_proto_tx(PLD_E0, 0xFF, 0, &stat_bit_mask, 1);
		
		set_ss_trig_src(DEI_SS_SENSOR_NO_TRIGGER);
		set_trig_sensor_index(0);

//		os_evt_clr(sensor_proto_evt, SENSOR_PROTO_ALRM_SRC);
//		evt_state = os_evt_wait( sensor_proto_evt, SENSOR_PROTO_ALRM_SRC, SENSOR_PROTO_ALRM_SRC,	1000);
		
		//never_evt_state = evt_state;
		
//		if(evt_state & SENSOR_PROTO_ALRM_SRC)
//			{

			os_tsk_wait(200);

				if((sensor_trigger( ss_sensor[get_trig_sensor_index()] )) && (get_ss_trig_src() != DEI_SS_SENSOR_NO_TRIGGER) )
				{
					//ss.sensor_triggered = TRUE ; 
					set_ss_sensor_trigger( TRUE );
					if( get_trig_sensor_index() == 1)
						trig_zone = DEI_SS_TRIGGER_ZONE_SENSOR ;             
					else if( get_trig_sensor_index() == 2 )
						trig_zone = DEI_SS_TRIGGER_ZONE_SENSOR2 ;             
					else //if( get_trig_sensor_index() == 0 )
					{// all others use zone 'other'
						trig_zone = DEI_SS_TRIGGER_ZONE_SENSOR3 ; 
					}

					set_ss_last_trigger( trig_zone );
				
					set_ss_trigger( TRUE );

					dei_ss_trigger_mem_update( trig_zone );
					
				}		        
//			}

		#endif
    if( (--dei_ss_trigger_vars.cycle_count) && (DOOR || HOOD || TRUNK || IGN || SENSOR ) )							  
	{ 									
		dei_ss_trigger_vars.in_progress = FALSE ; 

        dei_prk_flash  ( DEI_PRK_TRIGGER_STOP );
        dei_siren_chirp( DEI_SIREN_STOP       );
                       
        vrtc_update( dei_ss_trigger_vars.vrtc , DEI_SS_VRTC_TIMEOUT_TRIGGER_PAUSE ) ; //2 seconds pause

    	vrtc_start ( dei_ss_trigger_vars.vrtc ) ; 
	}
	else
	{	    
		dei_ss_trigger_stop();
	}
    
    
    #undef DOOR
    #undef TRUNK
    #undef HOOD
    #undef IGN
	#undef SENSOR	
}
//--------------------------------------------------------------------------//

