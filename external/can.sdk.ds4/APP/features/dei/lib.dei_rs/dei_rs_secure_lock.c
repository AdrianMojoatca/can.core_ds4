/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_secure_lock.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#include "dei_ss.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define SECURE_LOCK_ASSERT()		!!((dei_feature_get(DEI_FEAT_SECURE_LOCK)>=1)&&(dei_feature_get(DEI_FEAT_SECURE_LOCK)<=3))
#define DEI_RS_VRTC_TIMEOUT_DEFER_LOCK  (VRTC_1SEC*2)    //vehicle (Subaru3, Niss08) needs at least 800ms, 2sec to cover more vehicles

#ifdef PLATFORM_CM800
	#define IGN_LOCKING_ON   ((dei_feature_get( DEI_FEAT_IGN_CONTROLLED_LOCKS ) == DEI_IGN_LOCK_UNLOCK)||\
					          (dei_feature_get( DEI_FEAT_IGN_CONTROLLED_LOCKS ) == DEI_IGN_LOCK_ONLY))
#else
	#define RS_IGN_CONTROLLED	 (dei_feature_get( DEI_FEAT_CONTROL_DOOR_LOCK_OPTION ) == 2)	
	#define RS_IGN_LOCK_UNLOCK	 (dei_feature_get( DEI_FEAT_CONTROL_DOOR_LOCK_FUNC ) == 1)
	#define RS_IGN_LOCK	 	 (dei_feature_get( DEI_FEAT_CONTROL_DOOR_LOCK_FUNC ) == 2)
	
	#define IGN_LOCKING_ON   ((RS_IGN_CONTROLLED && RS_IGN_LOCK_UNLOCK)||\
					          (RS_IGN_CONTROLLED && RS_IGN_LOCK))
#endif


typedef void (*Dei_Rs_Secure_Lock_Func)( void );

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void _dei_rs_secure_lock_off_gwr_on   	   ( void );
static void _dei_rs_secure_lock_off_start_off	   ( void );
static void _dei_rs_secure_lock_off_gwr_off  	   ( void );
static void _dei_rs_secure_lock_off_check		   ( void );

static void _dei_rs_secure_lock_on_gwr_on    	   ( void );
static void _dei_rs_secure_lock_on_start_off 	   ( void );
static void _dei_rs_secure_lock_on_gwr_off   	   ( void );
static void _dei_rs_secure_lock_on_check		   ( void );

static void _dei_rs_smart_secure_lock_on_gwr_on    ( void );
static void _dei_rs_smart_secure_lock_on_start_off ( void );
static void _dei_rs_smart_secure_lock_on_gwr_off   ( void );
static void _dei_rs_smart_secure_lock_on_check	   ( void );

static void dei_rs_secure_defer_execute_lock	   ( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Vrtc    		dei_rs_vrtc_secure_lock  ; 
//static Boolean    dei_ss_lock_stored;
static Vrtc       dei_rs_vrtc_defer_execute_lock;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_secure_lock_init( void )
{
	static const Dei_Rs_Secure_Lock_Func vrtc_check_func[]=
	{
		_dei_rs_secure_lock_off_check      , 
		_dei_rs_secure_lock_on_check       ,
		_dei_rs_smart_secure_lock_on_check 
	};

	if( SECURE_LOCK_ASSERT() )
	{
		dei_rs_vrtc_secure_lock = vrtc_f_create( (Exec_Func)vrtc_check_func[dei_feature_get(DEI_FEAT_SECURE_LOCK)-1]);
		dei_rs_vrtc_defer_execute_lock = vrtc_f_create((Exec_Func)dei_rs_secure_defer_execute_lock);
	}
	dei_ss_lock_stored = get_ss_lock() || get_ss_arm();
}

//--------------------------------------------------------------------------//

void dei_rs_secure_lock_gwr_on( void )
{
	static const Dei_Rs_Secure_Lock_Func gwr_on[] = 
	{
		_dei_rs_secure_lock_off_gwr_on , 
		_dei_rs_secure_lock_on_gwr_on  , 			
		_dei_rs_smart_secure_lock_on_gwr_on 
	};
	
	dei_ss_lock_stored = get_ss_lock() || get_ss_arm();

	if( SECURE_LOCK_ASSERT() )
	{
		(gwr_on[dei_feature_get(DEI_FEAT_SECURE_LOCK)-1])();
	}
}

//--------------------------------------------------------------------------//

void dei_rs_secure_lock_start_off( void )
{
	static const Dei_Rs_Secure_Lock_Func start_off[] = 
	{
		_dei_rs_secure_lock_off_start_off , 
		_dei_rs_secure_lock_on_start_off  , 			
		_dei_rs_smart_secure_lock_on_start_off 
	};

	if( SECURE_LOCK_ASSERT() )
	{
		(start_off[dei_feature_get(DEI_FEAT_SECURE_LOCK)-1]) ();
	}
}

//--------------------------------------------------------------------------//

void dei_rs_secure_lock_gwr_off( void )
{
	static const Dei_Rs_Secure_Lock_Func gwr_off[] = 
	{
		_dei_rs_secure_lock_off_gwr_off , 
		_dei_rs_secure_lock_on_gwr_off  , 			
		_dei_rs_smart_secure_lock_on_gwr_off 
	};

	if( SECURE_LOCK_ASSERT() )
	{
		(gwr_off[dei_feature_get(DEI_FEAT_SECURE_LOCK)-1]) ();
	}
}

//--------------------------------------------------------------------------//
//SECURE LOCK OFF
//--------------------------------------------------------------------------//

void _dei_rs_secure_lock_off_gwr_on( void )
{
	if( !get_ign_status() )
	{
		cmd_q_rs_insert( INPUT_DISARM , 100 , 0 );			
		
		cmd_q_rs_insert(INPUT_DOOR_SKIP_SET, 100, 0);
		TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_OFF: door bypass temporarily...]\n\r");			
	}	
}

//--------------------------------------------------------------------------//	

void _dei_rs_secure_lock_off_start_off( void )
{
	vrtc_stop  ( dei_rs_vrtc_secure_lock );
			
	vrtc_update( dei_rs_vrtc_secure_lock ,  DEI_RS_VRTC_TIMEOUT_SECURE_LOCK_OFF );
			
	vrtc_start ( dei_rs_vrtc_secure_lock );	
}

//--------------------------------------------------------------------------//	

void _dei_rs_secure_lock_off_gwr_off( void )
{
	vrtc_stop( dei_rs_vrtc_secure_lock );

	vrtc_update( dei_rs_vrtc_secure_lock , DEI_RS_VRTC_TIMEOUT_SECURE_LOCK_OFF );

	vrtc_start( dei_rs_vrtc_secure_lock );	
}

//--------------------------------------------------------------------------//	

void _dei_rs_secure_lock_off_check( void )
{
	TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_OFF_CHECK]\n\r");	

	if( !get_rs_gwr() )
	{
		if( get_ign_status() )
		{
			if( IGN_LOCKING_ON && !get_door_status() && !get_ss_lock()  &&\
					((get_rs_error_shutdown() == DEI_RS_EVT_TRANSMITTER_SHUTDOWN)||\
					 (get_rs_error_shutdown() == DEI_RS_EVT_BRAKE)) )
			{
#ifdef PLATFORM_933				
			cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , CMD_LOCK_IGNITION ) ; 
#else
				cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)CMD_LOCK_IGNITION );
#endif	
				TRACE_DEI_RS("\n\r[DEI_RS_SECURE_OFF_LOCK_BY_IGN]\n\r");
			}
		}
		else
		{
			cmd_q_rs_insert( INPUT_ARM  , 100 , (void*)CMD_LOCK_IGNITION );			
	
			TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_OFF_ARM]\n\r");
		}
	}
	else
	{   //RS successfully
			cmd_q_rs_insert( INPUT_ARM  , 100 , (void*)CMD_LOCK_IGNITION );			
			TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_OFF_ARM AFTER RS ON]\n\r");
		
		dei_ss_silent_arm_updating(get_ss_arm());  //update the LED flashing pattern
	}
}
//--------------------------------------------------------------------------//
//SECURE LOCK ON
//--------------------------------------------------------------------------//
void _dei_rs_secure_lock_on_gwr_on( void )
{
	if( !get_ign_status() )
	{
		cmd_q_rs_insert( INPUT_DISARM                    , 100 , 0                      ); //disarm OEM system, DS4 is still armed
#ifdef PLATFORM_933		
		cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_DRIVER1_ON , 100 , CMD_UNLOCK_WITH_DISARM ) ; 	
#else		
		cmd_q_rs_insert( INPUT_UNLOCK_DRIVER1 | INPUT_ON , 100 , CMD_UNLOCK_WITH_DISARM );			
#endif
		cmd_q_rs_insert(INPUT_DOOR_SKIP_SET, 100, 0);
		TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_ON: door bypass temporarily...]\n\r");	
		
		TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_ON: UNLOCK while GWR ON]\n\r");	
	}
}

//--------------------------------------------------------------------------//	

void _dei_rs_secure_lock_on_start_off( void )
{
#ifdef PLATFORM_933
	vrtc_stop  ( dei_rs_vrtc_defer_execute_lock );
	vrtc_update( dei_rs_vrtc_defer_execute_lock ,  DEI_RS_VRTC_TIMEOUT_DEFER_LOCK );
	vrtc_start ( dei_rs_vrtc_defer_execute_lock );	
#else
	cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)CMD_LOCK_IGNITION );
	TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_ON: LOCK while STARTER OFF]\n\r");
#endif

}

//--------------------------------------------------------------------------//	

void _dei_rs_secure_lock_on_gwr_off( void )
{
	vrtc_stop( dei_rs_vrtc_secure_lock );

	vrtc_update( dei_rs_vrtc_secure_lock , DEI_RS_VRTC_TIMEOUT_SECURE_LOCK_ON );

	vrtc_start( dei_rs_vrtc_secure_lock );	
}

//--------------------------------------------------------------------------//	

void _dei_rs_secure_lock_on_check( void )
{
	TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_ON_CHECK]\n\r");	

	if( !get_rs_gwr() )
	{
		if( get_ign_status() )
		{
			if( IGN_LOCKING_ON && !get_door_status() && !get_ss_lock()  &&\
					((get_rs_error_shutdown() == DEI_RS_EVT_TRANSMITTER_SHUTDOWN)||\
					 (get_rs_error_shutdown() == DEI_RS_EVT_BRAKE)) )
			{
#ifdef PLATFORM_933
				cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , CMD_LOCK_IGNITION ) ; 
#else				
				cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)CMD_LOCK_IGNITION );
#endif	
				TRACE_DEI_RS("\n\r[DEI_RS_SECURE_ON_LOCK_BY_IGN]\n\r");
			}
		}
		else
		{
			cmd_q_rs_insert( INPUT_ARM              , 100 , 0 );
#ifdef PLATFORM_933
      cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , CMD_LOCK_WITH_ARM ) ; 
			set_ss_lock(TRUE);

			//RS off, resume the system status
			set_ss_arm(TRUE);
			dei_ss_silent_arm_updating(get_ss_arm());
#else		
			cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)CMD_LOCK_WITH_ARM );
#endif	
			TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_ON_ARM_LOCK]\n\r");
		}
	}
}
//--------------------------------------------------------------------------//
//SMART SECURE LOCK ON
//--------------------------------------------------------------------------//

void _dei_rs_smart_secure_lock_on_gwr_on( void )
{
	if( !get_ign_status() && dei_ss_lock_stored )
	{
		cmd_q_rs_insert( INPUT_DISARM                    , 100 , 0                      );
#ifdef PLATFORM_933
		cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_DRIVER1_ON , 100 , CMD_UNLOCK_WITH_DISARM ) ; 	
#else
		cmd_q_rs_insert( INPUT_UNLOCK_DRIVER1 | INPUT_ON , 100 , CMD_UNLOCK_WITH_DISARM );	
#endif		
		cmd_q_rs_insert(INPUT_DOOR_SKIP_SET, 100, 0);
		TRACE_DEI_RS("\n\r[DEI_RS_SMART_SECURE_LOCK_ON: door bypass temporarily...]\n\r");	
		
		TRACE_DEI_RS("\n\r[DEI_RS_SMART_SECURE_LOCK_ON: UNLOCK while GWR ON]\n\r");	
	}
}

//--------------------------------------------------------------------------//	

void _dei_rs_smart_secure_lock_on_start_off( void )
{
	//if( ss.locked )
	if( dei_ss_lock_stored )
	{
#ifdef PLATFORM_933
	  vrtc_stop  ( dei_rs_vrtc_defer_execute_lock );
	  vrtc_update( dei_rs_vrtc_defer_execute_lock ,  DEI_RS_VRTC_TIMEOUT_DEFER_LOCK );
	  vrtc_start ( dei_rs_vrtc_defer_execute_lock );	
#else		
		cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)CMD_LOCK_IGNITION );
		TRACE_DEI_RS("\n\r[DEI_RS_SMART_SECURE_LOCK_ON: LOCK while STARTER OFF]\n\r");
#endif
	}
}

//--------------------------------------------------------------------------//	

void _dei_rs_smart_secure_lock_on_gwr_off( void )
{
	vrtc_stop( dei_rs_vrtc_secure_lock );

	vrtc_update( dei_rs_vrtc_secure_lock , DEI_RS_VRTC_TIMEOUT_SMART_SECURE_LOCK_ON );

	vrtc_start( dei_rs_vrtc_secure_lock );	
}

//--------------------------------------------------------------------------//	

void _dei_rs_smart_secure_lock_on_check( void )
{
	TRACE_DEI_RS("\n\r[DEI_RS_SMART_SECURE_LOCK_ON_CHECK]\n\r");	

	if( !get_rs_gwr() )
	{
		if( get_ign_status() )
		{
			if( IGN_LOCKING_ON && !get_door_status() && !get_ss_lock()  &&\
					((get_rs_error_shutdown() == DEI_RS_EVT_TRANSMITTER_SHUTDOWN)||\
					 (get_rs_error_shutdown() == DEI_RS_EVT_BRAKE)) )
			{
#ifdef PLATFORM_933
				cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , CMD_LOCK_IGNITION ) ; 
#else				
				cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)CMD_LOCK_IGNITION );
#endif	
				TRACE_DEI_RS("\n\r[DEI_RS_SMART_SECURE_ON_LOCK_BY_IGN]\n\r");
			}
		}
		else
		{
#ifdef PLATFORM_933
				if( dei_ss_lock_stored )
				{
          cmd_q_rs_insert( INPUT_ARM, 100 , 0 );

				  cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , CMD_LOCK_WITH_ARM ) ;
					set_ss_lock(TRUE);
					//RS off, resume the system status
					set_ss_arm(TRUE);
			  }

				dei_ss_silent_arm_updating(get_ss_arm());
#else				
			if( get_ss_lock() )
			{
				cmd_q_rs_insert( INPUT_ARM              , 100 , 0 );			
				cmd_q_rs_insert( INPUT_LOCK1 | INPUT_ON , 100 , (void*)CMD_LOCK_WITH_ARM );
			}
#endif	
			TRACE_DEI_RS("\n\r[DEI_RS_SMART_SECURE_LOCK_ON_ARM_LOCK]\n\r");
		}
	}
}

//--------------------------------------------------------------------------//	
void dei_rs_secure_defer_execute_lock	   ( void )
{
	if(dei_feature_get(DEI_FEAT_SECURE_LOCK) == DEI_SECURE_LOCK_ON)
	{
		cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , CMD_LOCK_IGNITION ) ; //inherit from CM800, no rearm on wire
	                                                                                             //DS4 is still armed
 	  set_ss_lock(TRUE);
	  set_ss_arm(TRUE);      //arm DS4
	  dei_ss_silent_arm_updating(get_ss_arm());
		TRACE_DEI_RS("\n\r[DEI_RS_SECURE_LOCK_ON: LOCK while STARTER OFF]\n\r");
	}
	else if(dei_feature_get(DEI_FEAT_SECURE_LOCK) == DEI_SMART_SECURE_LOCK_ON)
	{
		cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1_ON , 100 , CMD_LOCK_IGNITION ) ; 

		set_ss_lock(TRUE);		
		TRACE_DEI_RS("\n\r[DEI_RS_SMART_SECURE_LOCK_ON: LOCK while STARTER OFF]\n\r");
	}
}

