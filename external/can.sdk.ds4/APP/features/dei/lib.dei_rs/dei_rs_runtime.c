/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_runtime.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#include "vrtc_f.h"
#include "trace.h"
#include "d2d.h"
#include "dei_out_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Rs_Runtime
{
    Boolean runtime_started   ; 
    Vrtc    vrtc_timeout      ;
}Dei_Rs_Runtime;
    

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void dei_rs_req_runtime   ( void * arg) ;

static void dei_rs_runtime_update( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Rs_Runtime   dei_rs_runtime_vars;
static UInt8 retry_runtime_remaining;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


void dei_rs_runtime_pause(void)
{
	 dei_out_set_ag(FALSE);
	
		retry_runtime_remaining = get_rs_runtime_remaining();  // remember how much time was left
}

void dei_rs_runtime_init( void )
{
	dei_rs_runtime_vars.vrtc_timeout = vrtc_f_create( (Exec_Func)dei_rs_runtime_update ); 

	notify_send( NOTIFY_RUNTIME_REMAINING  , 0); 
	notify_send( NOTIFY_RUNTIME_PROGRAMMED , 0); 

	retry_runtime_remaining = 0;
	dei_rs_runtime_vars.runtime_started = FALSE ;
    
    cmd_lut_insert (INPUT_REQ_RUNTIME					, dei_rs_req_runtime	 	, EXEC_MODE_NORMAL) ;

    //init dei_rs_oem
    dei_rs_oem.rt_override = FALSE;
    dei_rs_oem.runtime = 0;
}
//--------------------------------------------------------------------------//
void dei_rs_req_runtime(void * arg)
{
	UInt8 data[2]  = {SET_RUNTIME,};

  	ATOMIC
    (
        data[1] =  (dei_rs_runtime_vars.runtime_started? get_rs_runtime_remaining() : NULL);
    )

  	d2d_tx_frame (&data[0] , sizeof_array(data)) ;    
}
//--------------------------------------------------------------------------//

void dei_rs_runtime_set( Dei_Rs_Start_Source source )
{
	switch( source )
	{
		case DEI_RS_NORMAL_START :
		{
		  //rs.runtime_programmed    = dei_feature_get( DEI_FEAT_REMOTE_START_RUNTIME ) ;
		  //rs.runtime_remaining  = dei_feature_get( DEI_FEAT_REMOTE_START_RUNTIME ) ;			
			if(dei_rs_oem.rt_override)
            {
                set_rs_runtime_programmed( dei_rs_oem.runtime ) ;		  
                set_rs_runtime_remaining ( dei_rs_oem.runtime );			
            }
            else
            {
                set_rs_runtime_programmed( dei_feature_get( DEI_FEAT_REMOTE_START_RUNTIME ) ) ;		  
                set_rs_runtime_remaining ( dei_feature_get( DEI_FEAT_REMOTE_START_RUNTIME ) );			
            }
		}
		break;

		case DEI_RS_TURBO_START :
		{
			//rs.runtime_programmed = dei_feature_get( DEI_FEAT_TURBO_MODE_RUNTIME ) ;
			//rs.runtime_remaining  = dei_feature_get( DEI_FEAT_TURBO_MODE_RUNTIME ) ; 			
			set_rs_runtime_programmed( dei_feature_get( DEI_FEAT_TURBO_MODE_RUNTIME ) ) ;		  
		 	set_rs_runtime_remaining ( dei_feature_get( DEI_FEAT_TURBO_MODE_RUNTIME ) );			
		}
		break;

		case DEI_RS_SMART_START :
		case DEI_RS_TIMER_START :
		{
			//rs.runtime_programmed = dei_feature_get( DEI_FEAT_TIMER_MODE_RUNTIME ) ;
			//rs.runtime_remaining  = dei_feature_get( DEI_FEAT_TIMER_MODE_RUNTIME ) ;			
			set_rs_runtime_programmed( dei_feature_get( DEI_FEAT_TIMER_MODE_RUNTIME ) ) ;		  
		 	set_rs_runtime_remaining ( dei_feature_get( DEI_FEAT_TIMER_MODE_RUNTIME ) );			
		}
		break;

		default :
		{
			;
		}
		break;
	};

	//notify_send( NOTIFY_RUNTIME_REMAINING  , rs.runtime_remaining ); 
	//notify_send( NOTIFY_RUNTIME_PROGRAMMED , rs.runtime_programmed); 
	
	notify_send( NOTIFY_RUNTIME_REMAINING  , get_rs_runtime_remaining() ); 
	notify_send( NOTIFY_RUNTIME_PROGRAMMED , get_rs_runtime_programmed()); 
}

//--------------------------------------------------------------------------//

void dei_rs_runtime_start( void )
{
	dei_rs_runtime_vars.runtime_started = TRUE ; 
	set_rs_runtime_remaining( retry_runtime_remaining ? retry_runtime_remaining : get_rs_runtime_programmed() );   //refresh runtime remaining

	vrtc_stop( dei_rs_runtime_vars.vrtc_timeout );	

	vrtc_update( dei_rs_runtime_vars.vrtc_timeout  , DEI_RS_VRTC_TIMEOUT_RUNTIME_TIME_BASE ) ; //1minute

	vrtc_start( dei_rs_runtime_vars.vrtc_timeout );	
	

	dei_out_delay_ag(TRUE);
}



//--------------------------------------------------------------------------//

void dei_rs_runtime_stop( void )
{
	retry_runtime_remaining = 0;  // forget remaining runtime (next start will load full runtime)
	
	dei_rs_runtime_vars.runtime_started = FALSE ; 
	
	vrtc_stop( dei_rs_runtime_vars.vrtc_timeout );	
	
	// here we always turn off antigrind.  once GWR stops the wire will switch to being a starter kill (or not)
	dei_out_set_ag(FALSE);

}	

//--------------------------------------------------------------------------//

void dei_rs_runtime_reset( void )
{
	//rs.runtime_remaining = rs.runtime_programmed ; 
	set_rs_runtime_remaining( get_rs_runtime_programmed() );
    
	//notify_send( NOTIFY_RUNTIME_REMAINING  , rs.runtime_remaining ); 
	//notify_send( NOTIFY_RUNTIME_PROGRAMMED , rs.runtime_programmed); 

	notify_send( NOTIFY_RUNTIME_REMAINING  , get_rs_runtime_remaining()  ); 
	notify_send( NOTIFY_RUNTIME_PROGRAMMED , dei_rs_oem.rt_override ? dei_rs_oem.runtime :get_rs_runtime_programmed() ); 

	vrtc_update( dei_rs_runtime_vars.vrtc_timeout  , DEI_RS_VRTC_TIMEOUT_RUNTIME_TIME_BASE ) ; //1minute

	vrtc_start( dei_rs_runtime_vars.vrtc_timeout );

}

//--------------------------------------------------------------------------//

void dei_rs_runtime_update( void )
{
	//rs.runtime_remaining -- ; 
	set_rs_runtime_remaining( get_rs_runtime_remaining() - 1 );

  //if( rs.runtime_remaining       && dei_rs_runtime_vars.runtime_started )
	if( get_rs_runtime_remaining() && dei_rs_runtime_vars.runtime_started )
	{				
	  //if( (rs.runtime_remaining       == 3) || (rs.runtime_remaining       == 1 ) )
		if( (get_rs_runtime_remaining() == 3) || (get_rs_runtime_remaining() == 1 ) )
		{
			TRACE_DEI_RS("LOW RUNTIME ALERT\n\r");

		  //notify_send( NOTIFY_RUNTIME_LOW , rs.runtime_remaining ); 
			notify_send( NOTIFY_RUNTIME_LOW , get_rs_runtime_remaining() ); 
		}
#ifdef PLATFORM_933
    notify_send( NOTIFY_RUNTIME_UPDATE , get_rs_runtime_remaining() ) ;   //update the variable DEI_SC_INFO_RUNTIME_NOTIFYAINING
#else
    notify_send( NOTIFY_RUNTIME_CHECK , get_rs_runtime_remaining() ) ; 
#endif

		vrtc_update( dei_rs_runtime_vars.vrtc_timeout  , DEI_RS_VRTC_TIMEOUT_RUNTIME_TIME_BASE ) ; //1minute
		vrtc_start( dei_rs_runtime_vars.vrtc_timeout );
	}
	else
	{
		dei_ss_lock_stored = get_ss_lock() || get_ss_arm();  // update status to determine Smart Safelock
		dei_rs_evt_signal( DEI_RS_EVT_RUNTIME_EXPIRED ); 
	}

	//TRACE_DEI_RS("\n\r[DEI_RS_RUNTIME  PROGRAMMED %d min REMAINING  %d min|]\n\r" , rs.runtime_programmed , rs.runtime_remaining );
	TRACE_DEI_RS("\n\r[DEI_RS_RUNTIME  PROGRAMMED %d min REMAINING  %d min]\n\r" , get_rs_runtime_programmed() , get_rs_runtime_remaining() );
}
		
//--------------------------------------------------------------------------//		
UInt8       dei_rs_get_oem_rt(void)
{
    return(dei_rs_oem.runtime);
}

//--------------------------------------------------------------------------//
void    dei_rs_set_oem_rt(UInt8 val)
{
	UInt8 runtime_default;
	
    dei_rs_oem.rt_override = (val) ? TRUE : FALSE;
    dei_rs_oem.runtime = val;
	
	runtime_default = dei_feature_get( DEI_FEAT_REMOTE_START_RUNTIME );
	if(dei_rs_oem.rt_override && dei_rs_oem.runtime != runtime_default)
	{
        dei_feature_set(DEI_FEAT_REMOTE_START_RUNTIME, dei_rs_oem.runtime);
        TRACE_DEI_RS("\n\r[DEI_RS_OEM_RUNTIME: %d ]\n\r", dei_rs_oem.runtime);

        nvram_usr_commit();
	}
}

//--------------------------------------------------------------------------//

