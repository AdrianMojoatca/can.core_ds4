/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_timer_start.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Rs_Timer_Start
{
    Vrtc   vrtc_timeout ; 
    UInt8  count        ;
    UInt8  interval     ;
}Dei_Rs_Timer_Start;
    
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void    dei_rs_timer_start_execute      	    (void);
static void    dei_rs_timer_start_enable_by_lock  	(void);
static void    dei_rs_timer_start_disable_by_unlock	(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Rs_Timer_Start   dei_rs_timer_start_vars;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_timer_start_init( void )
{
	dei_rs_timer_start_vars.vrtc_timeout = vrtc_f_create( (Exec_Func)dei_rs_timer_start_execute );

	cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {    
            { INPUT_OEM_LOCK           , (Exec_Func)dei_rs_timer_start_enable_by_lock       },		
            { INPUT_OEM_ARM            , (Exec_Func)dei_rs_timer_start_enable_by_lock       },		
        	{ INPUT_RF_LOCK            , (Exec_Func)dei_rs_timer_start_enable_by_lock    	},     
        	{ INPUT_RF_SILENT_LOCK     , (Exec_Func)dei_rs_timer_start_enable_by_lock  	    },     	
        	{ INPUT_OEM_UNLOCK         , (Exec_Func)dei_rs_timer_start_disable_by_unlock    },	
            { INPUT_OEM_DISARM         , (Exec_Func)dei_rs_timer_start_disable_by_unlock    },	
        	{ INPUT_RF_UNLOCK          , (Exec_Func)dei_rs_timer_start_disable_by_unlock    }, 
            { INPUT_RF_SILENT_UNLOCK   , (Exec_Func)dei_rs_timer_start_disable_by_unlock    },         
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
}

//--------------------------------------------------------------------------//

void dei_rs_timer_start_enable_by_lock( void )
{
	//if( rs.timer_on  )
	if( get_rs_timer()  )
	{
	    dei_rs_timer_start();

		TRACE_DEI_RS("\n\r[DEI_RS TIMER MODE ACTIVATED( COUNT = %d) , (INTERVAL = %d)]\n\r" , dei_rs_timer_start_vars.count , dei_rs_timer_start_vars.interval );		
	}
}

//--------------------------------------------------------------------------//

void dei_rs_timer_start_disable_by_unlock( void )
{
	//if( rs.timer_on )
	if( get_rs_timer() )
	{
		TRACE_DEI_RS("\n\r[DEI_RS TIMER MODE DEACTIVATED(CAR UNLOCKED)]\n\r");
		
		dei_rs_timer_stop();
	}
}

//--------------------------------------------------------------------------//

void dei_rs_timer_start( void )
{
	const UInt8 timer_data[] = { 1 , 2 , 3 , 4 , 6 , 8 , 10 , 12 , 14 , 16 , 18 , 20 , 22 ,24 }; 
	
	#define COUNT			timer_data[dei_feature_get( DEI_FEAT_TIMER_MODE_START   )-1]		
	#define INTERVAL		timer_data[dei_feature_get( DEI_FEAT_TIMER_MODE_INTERVAL)-1]		

	dei_rs_timer_start_vars.count   = COUNT ; 
    dei_rs_timer_start_vars.interval= INTERVAL; 	
	
	vrtc_update( dei_rs_timer_start_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SMART_1H * dei_rs_timer_start_vars.interval); 

	vrtc_start( dei_rs_timer_start_vars.vrtc_timeout ); 

	#undef COUNT
	#undef INTERVAL
}

//--------------------------------------------------------------------------//

void dei_rs_timer_stop( void )
{
	vrtc_stop( dei_rs_timer_start_vars.vrtc_timeout ); 
}

//--------------------------------------------------------------------------//

void dei_rs_timer_start_execute( void )
{
	TRACE_DEI_RS("\n\r[DEI_RS TIMER MODE TIMEOUT]\n\r");
	
	//if( rs.timer_on && !dei_rs_error_start() )
	if( get_rs_timer() && !dei_rs_error_start() )
	{
		//if( ss.locked   && !dei_rs_state )
		if( get_ss_lock() && !dei_rs_state )
		{		
			dei_rs_runtime_set( DEI_RS_TIMER_START ); 
			
			if( brand_id == BRAND_VP_SC )
				notify_send( NOTIFY_RS_START );

			dei_rs_execute(); 

			if( --dei_rs_timer_start_vars.count )
			{				
				//if( rs.timer_on  && ss.locked     )
				if( get_rs_timer() && get_ss_lock() )
				{
					vrtc_update( dei_rs_timer_start_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SMART_1H * dei_rs_timer_start_vars.interval ); 
					
					vrtc_start( dei_rs_timer_start_vars.vrtc_timeout );

					TRACE_DEI_RS("\n\r[DEI_RS TIMER MODE ACTIVATED( COUNT = %d)]\n\r" , dei_rs_timer_start_vars.count );
				}
				else
				{
					TRACE_DEI_RS("\n\r[DEI_RS TIMER MODE DEACTIVATED (CAR UNLOCKED)]\n\r");
				}
			}
			else
			{
				//rs.timer_on = FALSE ; 
				set_rs_timer( FALSE );

				dei_prk_flash( DEI_PRK_TMR_OFF ); 

				notify_send( NOTIFY_TIMER_MODE_OFF );

				TRACE_DEI_RS("\n\r[DEI_RS TIMER MODE DEACTIVATED (COUNT EXPIRED)]\n\r");
			}
		}
		else
		{			
			vrtc_update( dei_rs_timer_start_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SMART_1H * dei_rs_timer_start_vars.interval); 

			vrtc_start( dei_rs_timer_start_vars.vrtc_timeout );	

			TRACE_DEI_RS("\n\r[DEI_RS TIMER MODE ACTIVATED( COUNT = %d)]\n\r" , dei_rs_timer_start_vars.count );
		}
	}
	else
	{
		dei_rs_mts_smart_timer_disable();
			
		TRACE_DEI_RS("\n\r[DEI_RS TIMER MODE DEACTIVATED (INVALID START CONDITION)]\n\r");		
	}	
}

//--------------------------------------------------------------------------//

