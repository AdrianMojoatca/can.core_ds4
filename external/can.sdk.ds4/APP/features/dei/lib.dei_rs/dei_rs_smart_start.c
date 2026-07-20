/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_smart_start.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
	#include "temperature.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Rs_Smart_Start
{
    Vrtc   vrtc_timeout; 
    UInt8  count_start ;
    UInt8  interval	   ;    
}Dei_Rs_Smart_Start;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean  dei_rs_smart_start_voltage_check    (void);
static Boolean  dei_rs_smart_start_temp_check       (void);
static void     dei_rs_smart_start_execute          (void);
static void     dei_rs_smart_start_enable_by_lock   (void);
static void     dei_rs_smart_start_disable_by_unlock(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Rs_Smart_Start   dei_rs_smart_start_vars;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_smart_start_init( void )
{
	dei_rs_smart_start_vars.vrtc_timeout = vrtc_f_create( (Exec_Func)dei_rs_smart_start_execute );

	cmd_register:
    {
        static const Cmd_Array1 cmd_array[]=
        {
            { INPUT_OEM_LOCK                  , (Exec_Func)dei_rs_smart_start_enable_by_lock      },
            { INPUT_OEM_ARM                   , (Exec_Func)dei_rs_smart_start_enable_by_lock      },
        	{ INPUT_RF_LOCK                   , (Exec_Func)dei_rs_smart_start_enable_by_lock      },
        	{ INPUT_RF_SILENT_LOCK            , (Exec_Func)dei_rs_smart_start_enable_by_lock  	  },
        	{ INPUT_OEM_UNLOCK                , (Exec_Func)dei_rs_smart_start_disable_by_unlock   },
            { INPUT_OEM_DISARM                , (Exec_Func)dei_rs_smart_start_disable_by_unlock   },
        	{ INPUT_RF_UNLOCK                 , (Exec_Func)dei_rs_smart_start_disable_by_unlock   },
            { INPUT_RF_SILENT_UNLOCK          , (Exec_Func)dei_rs_smart_start_disable_by_unlock   }
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
}

//--------------------------------------------------------------------------//

void dei_rs_smart_start_enable_by_lock( void )
{
	//if( rs.smart_on )
	if( get_rs_smart() )
	{
		dei_rs_smart_start();

		TRACE_DEI_RS("\n\r[DEI_RS SMART MODE ACTIVATED( COUNT = %d)]\n\r" , dei_rs_smart_start_vars.count_start );		
	}
}

//--------------------------------------------------------------------------//

void dei_rs_smart_start_disable_by_unlock( void )
{
	//if( rs.smart_on )
	if( get_rs_smart() )
	{
		TRACE_DEI_RS("\n\r[DEI_RS SMART MODE DEACTIVATED(CAR UNLOCKED)]\n\r");
		
		dei_rs_smart_stop();
	}
}

//--------------------------------------------------------------------------//

void dei_rs_smart_start( void )
{	
	const UInt8 timer_data[] = { 1 , 2 , 3 , 4 , 6 , 8 , 10 , 12 , 14 , 16 , 18 , 20 , 22 ,24 }; 
	
	#define COUNT			timer_data[dei_feature_get( DEI_FEAT_TIMER_MODE_START   )-1]		
	#define INTERVAL		timer_data[dei_feature_get( DEI_FEAT_TIMER_MODE_INTERVAL)-1]		

	dei_rs_smart_start_vars.count_start    = COUNT    ; 
	dei_rs_smart_start_vars.interval       = INTERVAL ;
	
	vrtc_update( dei_rs_smart_start_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SMART_1H * dei_rs_smart_start_vars.interval ); 

	vrtc_start( dei_rs_smart_start_vars.vrtc_timeout ); 

	#undef COUNT
	#undef INTERVAL
}

//--------------------------------------------------------------------------//

void dei_rs_smart_stop( void )
{
	vrtc_stop( dei_rs_smart_start_vars.vrtc_timeout ); 
}

//--------------------------------------------------------------------------//

void dei_rs_smart_start_execute( void )
{
	TRACE_DEI_RS("\n\r[DEI_RS SMART MODE TIMEOUT]\n\r");
	
  //if( rs.smart_on    && !dei_rs_error_start() )
	if( get_rs_smart() && !dei_rs_error_start() )
	{
		wake_enter_flagged(WAKE_DEI_RS_SMART);		
		{
			os_tsk_wait( 1000 ); //wait 1000 ms for voltage and temperature update
		}
		wake_leave_flagged(WAKE_DEI_RS_SMART);
		
		//if( ss.locked   && !dei_rs_state && ( dei_rs_smart_start_voltage_check() || dei_rs_smart_start_temp_check() ) )
		if( get_ss_lock() && !dei_rs_state && ( dei_rs_smart_start_voltage_check() || dei_rs_smart_start_temp_check() ) )
		{		
			dei_rs_runtime_set( DEI_RS_SMART_START ); 
			
			if( brand_id == BRAND_VP_SC )
				notify_send( NOTIFY_RS_START );
			
			dei_rs_execute(); 

			if( --dei_rs_smart_start_vars.count_start )
			{
			  //if( rs.smart_on    && ss.locked )
				if( get_rs_smart() && get_ss_lock() )
				{
					vrtc_update( dei_rs_smart_start_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SMART_1H * dei_rs_smart_start_vars.interval ); 

					vrtc_start( dei_rs_smart_start_vars.vrtc_timeout );

					TRACE_DEI_RS("\n\r[DEI_RS SMART MODE ACTIVATED( COUNT = %d)]\n\r" , dei_rs_smart_start_vars.count_start );
				}
				else
				{
					TRACE_DEI_RS("\n\r[DEI_RS SMART MODE DEACTIVATED (CAR UNLOCKED)]\n\r");
				}	
			}
			else
			{
				//rs.smart_on = FALSE ; 
				set_rs_smart( FALSE );

				dei_prk_flash( DEI_PRK_SMART_OFF ); 				
				
				notify_send( NOTIFY_SMART_MODE_OFF );

				TRACE_DEI_RS("\n\r[DEI_RS SMART MODE DEACTIVATED (COUNT EXPIRED)]\n\r");
			}
		}
		else
		{
			//if( ss.locked )
			if( get_ss_lock() )
			{
				vrtc_update( dei_rs_smart_start_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SMART_1H * dei_rs_smart_start_vars.interval ); 

				vrtc_start( dei_rs_smart_start_vars.vrtc_timeout );	

				TRACE_DEI_RS("\n\r[DEI_RS SMART MODE ACTIVATED( COUNT = %d)]\n\r" , dei_rs_smart_start_vars.count_start );
			}
			else
			{
				TRACE_DEI_RS("\n\r[DEI_RS SMART MODE DEACTIVATED (CAR UNLOCKED)]\n\r");
			}	
		}
	}
	else
	{
		dei_rs_mts_smart_timer_disable();			
						
		TRACE_DEI_RS("\n\r[DEI_RS SMART MODE DEACTIVATED (INVALID START CONDITION)]\n\r");	
	}	
}

//--------------------------------------------------------------------------//

Boolean dei_rs_smart_start_voltage_check( void )
{
	static const double v_low[] = { 9.0 , 9.5 , 10.0 , 10.5 , 11.0 , 11.5  ,12.0 , 12.5 };

	#define V_LOW  v_low[ dei_feature_get(DEI_FEAT_SMART_START_LOW_BATTERY)-1 ]

	TRACE_DEI_RS("\n\r[DEI_RS SMART MODE - %s BY VOLTAGE]\n\r" , (get_voltage()<V_LOW) ? "TRIGGERED" : "NOT TRIGGERED");
    
	return ( get_voltage() < V_LOW ) ;

	#undef V_LOW 
}

//--------------------------------------------------------------------------//

Boolean dei_rs_smart_start_temp_check( void )
{
	static const UInt8 temp_low_table[]  = { 0 , 40-29 , 40-23 , 40-18 , 40-12 , 40-6  , 40-1  , 40+4  , 40+10 , 40+15 , 40+21 };
	static const UInt8 temp_high_table[] = { 0 , 40+4  , 40+10 , 40+15 , 40+21 , 40+26 , 40+32 , 40+38 , 40+43 , 40+49 , 40+54 };

	UInt8 low , high ;

	if( (dei_feature_get(DEI_FEAT_SMART_START_LOW_TEMP )-1) > sizeof( temp_low_table  ) ) return FALSE ; 
	if( (dei_feature_get(DEI_FEAT_SMART_START_HIGH_TEMP)-1) > sizeof( temp_high_table ) ) return FALSE ; 	
	
#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
	temperature_update();
	os_tsk_wait(360);
#endif
		
  //if( !temperature.bit.in_sensor_present )return FALSE ; 	
	if( !get_temp_sensor_present() )return FALSE ; 	
		
	low  = temp_low_table [ dei_feature_get(DEI_FEAT_SMART_START_LOW_TEMP  ) - 1 ];
	high = temp_high_table[ dei_feature_get(DEI_FEAT_SMART_START_HIGH_TEMP ) - 1 ];	 		
	
	#define LOW_TRIGGER  ((get_temp_sensor_value() < low ) && (dei_feature_get(DEI_FEAT_SMART_START_LOW_TEMP )!= DEI_FEATURE_SMART_OFF) )
	#define HIGH_TRIGGER ((get_temp_sensor_value() > high) && (dei_feature_get(DEI_FEAT_SMART_START_HIGH_TEMP)!= DEI_FEATURE_SMART_OFF) )  

	TRACE_DEI_RS("\n\r[DEI_RS SMART MODE- %s BY TEMPERATURE]\n\r" , LOW_TRIGGER || HIGH_TRIGGER ? "TRIGGERED" : "NOT TRIGGERED");
	
	return LOW_TRIGGER || HIGH_TRIGGER ;

	#undef LOW_TRIGGER
	#undef TEMP_TRIGGER
}

//--------------------------------------------------------------------------//
