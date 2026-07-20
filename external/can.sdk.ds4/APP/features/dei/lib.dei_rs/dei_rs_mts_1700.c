/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_mts.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#include "ngmm_feature.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Rs_Mts
{
    Vrtc    timeout ;
    Vrtc    timeout_door_check;
    Boolean timeout_in_progress ;
    Boolean door_open_status;
	  Boolean handbrake_off;
	  Boolean handbrake_on;
		Boolean progress_break;
		Boolean door_open_close_cycle;
}Dei_Rs_Mts;
    
#define MTS_INIT_BY_REMOTE								(dei_feature_get( DEI_FEAT_MANUAL_TRANSMISSION_INIT ) == 1)
#define MTS_INIT_BY_2_PARK_BRAKE					(dei_feature_get( DEI_FEAT_MANUAL_TRANSMISSION_INIT ) == 2)
#define MTS_INIT_BY_PARK_BRAKE						(dei_feature_get( DEI_FEAT_MANUAL_TRANSMISSION_INIT ) == 3)

#define DEI_RS_MTS_PASSIVE_LOCK_TIMEOUT				VRTC_1SEC*8

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_rs_mts_handbrake_off              ( void );
static void dei_rs_mts_handbrake_on               ( void );
static void dei_rs_mts_brake_on                   ( void );
static void dei_rs_mts_brake_release							( void );
static void dei_rs_mts_enable_event               ( void );
static void dei_rs_mts_timeout                    ( void );
static void dei_rs_mts_hood_status_open           ( void );
static void dei_rs_mts_door_open                  ( void );
static void dei_rs_mts_door_close                 ( void );
static void dei_rs_mts_door_check_tmr             ( void );
static void dei_rs_mts_complete_done							( void );
static void dei_rs_mts_ign_on											( void );


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Rs_Mts  dei_rs_mts_vars;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_mts_init( void )
{  
	set_rs_mts_progress( FALSE );

    dei_rs_mts_vars.timeout_in_progress = FALSE ;
    
  if( (get_gearbox_type() == GEARBOX_MANUAL) )
	{
				dei_rs_mts_vars.timeout = vrtc_f_create( (Exec_Func)dei_rs_mts_timeout ); 
        dei_rs_mts_vars.timeout_door_check = vrtc_f_create( (Exec_Func)dei_rs_mts_door_check_tmr ); 
        dei_rs_mts_vars.door_open_status = FALSE;
        dei_rs_mts_vars.handbrake_off = FALSE;
        dei_rs_mts_vars.handbrake_on = FALSE;
				dei_rs_mts_vars.door_open_close_cycle = FALSE;
		
		    dei_rs_mts_broken_check_timer = timeout_f_create(DEI_RS_MTS_BROKEN_CHK_TIMEOUT, (Exec_Func)dei_rs_mts_broken_check);
		
		
		cmd_register:
        {
            static const Cmd_Array1 cmd_array[] = 
            {
                { INPUT_BRAKE_STATUS_PRESS   , (Exec_Func)dei_rs_mts_brake_on     	                   },
                { INPUT_BRAKE_STATUS_RELEASE   , (Exec_Func)dei_rs_mts_brake_release                   },
        		{ INPUT_HANDBRAKE_STATUS_ON  , (Exec_Func)dei_rs_mts_handbrake_on                     },																								
        		{ INPUT_HANDBRAKE_STATUS_OFF , (Exec_Func)dei_rs_mts_handbrake_off                    },																								
        		{ INPUT_OEM_LOCK             , (Exec_Func)dei_rs_mts_enable_event                     },
                { INPUT_OEM_ARM              , (Exec_Func)dei_rs_mts_enable_event                     },
        		{ INPUT_RF_LOCK              , (Exec_Func)dei_rs_mts_enable_event                     },
        		{ INPUT_RF_SILENT_LOCK       , (Exec_Func)dei_rs_mts_enable_event                     },	 	        
                { INPUT_RF_UNLOCK            , (Exec_Func)dei_rs_mts_door_checking_disable     },
        		{ INPUT_RF_SILENT_UNLOCK     , (Exec_Func)dei_rs_mts_door_checking_disable     },
                { INPUT_OEM_UNLOCK           , (Exec_Func)dei_rs_mts_door_checking_disable     },
                { INPUT_OEM_DISARM           , (Exec_Func)dei_rs_mts_door_checking_disable     },        
//                { INPUT_TRIGGER_ON           , (Exec_Func)dei_rs_mts_door_checking_timeout_stop},	 	                
                { INPUT_ALL_DOOR_STATUS_OPEN,  (Exec_Func)dei_rs_mts_door_open                },
                { INPUT_ALL_DOOR_STATUS_CLOSE, (Exec_Func)dei_rs_mts_door_close               },
								{ INPUT_IGNITION | INPUT_ON, (Exec_Func)dei_rs_mts_ign_on }
            };

            CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
        }
	}
}

//--------------------------------------------------------------------------//

Boolean dei_rs_mts_ready_for_activation( void )
{
	if((dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )!=DEI_ECM_TACHOMETER) || 	( !MTS_INIT_BY_REMOTE )) return FALSE;
	
	
		return get_rs_mts_progress()        &&
				 get_ign_status     ()        &&
				 (get_rpm() > DEI_RS_RPM_MIN) &&
				 !dei_rs_state                &&
						 !get_brake_status()          &&		   
				 get_handbrake_status();
		   
}

//--------------------------------------------------------------------------//

void dei_rs_mts_handbrake_on( void )
{
	if(dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )!=DEI_ECM_TACHOMETER) return;
    
	if( get_ign_status() && (get_rpm() > DEI_RS_RPM_MIN) && !dei_rs_state && get_brake_status()  )
	{
		if( MTS_INIT_BY_PARK_BRAKE  && !dei_rs_mts_vars.progress_break )
		{
			dei_rs_mts_vars.handbrake_on = TRUE; 
			set_rs_mts_progress( TRUE );
		}
	
		if( MTS_INIT_BY_2_PARK_BRAKE  && !dei_rs_mts_vars.progress_break )
		{
			if(dei_rs_mts_vars.handbrake_off)
				set_rs_mts_progress( TRUE );
			else
			{
				vrtc_update( dei_rs_mts_vars.timeout , DEI_RS_VRTC_TIMEOUT_2X_HANDBRAKE ) ;//10 seconds timeout 

				vrtc_start( dei_rs_mts_vars.timeout );

				TRACE_DEI_RS("\n\r[MTS PROCEDURE IN PROGRESS]\n\r");

				dei_rs_mts_vars.handbrake_on = TRUE;
			}
		}
		if( MTS_INIT_BY_REMOTE && !dei_rs_mts_vars.progress_break )
		{
			vrtc_update( dei_rs_mts_vars.timeout , DEI_RS_VRTC_TIMEOUT_MTS ) ;//20 seconds timeout 

			vrtc_start( dei_rs_mts_vars.timeout );

			TRACE_DEI_RS("\n\r[MTS PROCEDURE IN PROGRESS]\n\r");
			set_rs_mts_progress( TRUE );
    }
		
		dei_rs_mts_vars.progress_break = FALSE;
	}
}

//--------------------------------------------------------------------------//


void dei_rs_mts_handbrake_off( void )
{	            	
	if(dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )!=DEI_ECM_TACHOMETER) return;
	
	if( get_rs_mts_progress() )
	{	
		vrtc_stop( dei_rs_mts_vars.timeout ); 

		TRACE_DEI_RS("\n\r[MTS PROCEDURE DISABLED - HANDBRAKE OFF]\n\r");    
		if( dei_rs_state == NULL )
        {
			dei_rs_mts_vars.progress_break = TRUE ;	
            set_rs_mts_progress( FALSE ); 
        }
	}
	//else
	if( MTS_INIT_BY_2_PARK_BRAKE & dei_rs_mts_vars.handbrake_on )
	{
		dei_rs_mts_vars.handbrake_on = FALSE;
		dei_rs_mts_vars.handbrake_off = TRUE;
	}
}

//--------------------------------------------------------------------------//

void dei_rs_mts_brake_on( void )
{	            	
	if(dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )!=DEI_ECM_TACHOMETER) return;
	
	if( get_rs_mts_progress())
	{	
		vrtc_stop( dei_rs_mts_vars.timeout ); 

		TRACE_DEI_RS("\n\r[MTS PROCEDURE DISABLED - BRAKE ON]\n\r");    
    	
		if( get_rs_mts() == TRUE)
        {
            set_rs_mts_progress( FALSE ); 
        }
	}
}


//--------------------------------------------------------------------------//

void dei_rs_mts_brake_release( void )
{
	if( get_ign_status() && (get_rpm() > DEI_RS_RPM_MIN) && !dei_rs_state && !get_brake_status())
	{
		if( ( (MTS_INIT_BY_PARK_BRAKE && dei_rs_mts_vars.handbrake_on) || (MTS_INIT_BY_2_PARK_BRAKE && dei_rs_mts_vars.handbrake_off))\
      			 && get_rs_mts_progress() )
		{
			cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );
			//set_rs_mts_progress( TRUE );
			
			if(dei_system_locked_by_ign_control)
			{
				dei_system_locked_by_ign_control = FALSE;
				os_tsk_wait(500);
        if(dei_feature_get( DEI_FEAT_CONTROL_UNLOCK_ALL_DOOR ) == 2)    //UNLOCK_ALL_DOORS 
					cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_ALL1_ON , 100 , CMD_UNLOCK_IGNITION ) ; 	
				else
					cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_UNLOCK_DRIVER1_ON , 100 , CMD_UNLOCK_IGNITION ) ; 

			}
				
			
			TRACE_DEI_RS("\n\r[MTS PROCEDURE IN PROGRESS]\n\r");
		}
		dei_rs_mts_vars.handbrake_off = FALSE;
		dei_rs_mts_vars.handbrake_on = FALSE;
	}
}
//--------------------------------------------------------------------------//

void dei_rs_mts_timeout( void )
{
	if(dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )!=DEI_ECM_TACHOMETER) return;
	
	vrtc_stop( dei_rs_mts_vars.timeout ); 
	
	if( !dei_rs_state )
	{		
		set_rs_mts_progress( FALSE ); 
		
		dei_rs_mts_vars.handbrake_on = FALSE;

		dei_rs_mts_vars.handbrake_off = FALSE;

		//TRACE_DEI_RS("\n\r[MTS PROCEDURE DISABLED - TIMEOUT EXPIRED]\n\r");  //not true if receives RS off
	}
}

//--------------------------------------------------------------------------//

Boolean  dei_rs_ready_for_mts_enable(void)
{
	if(dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )!=DEI_ECM_TACHOMETER) return FALSE;    
	
    return   (get_rpm() > DEI_RS_RPM_MIN)      &&\
        	  get_rs_mts_progress()            &&\
              get_ign_status     ()            &&\
             (dei_rs_state == dei_rs_running)  &&\
							dei_rs_mts_vars.door_open_close_cycle ;
}

//--------------------------------------------------------------------------//

void dei_rs_mts_enable_event(void)
{  
   if(dei_feature_get( DEI_FEAT_ENGINE_CHECKING_MODE )!=DEI_ECM_TACHOMETER) 
		 return;
    
    if( dei_rs_ready_for_mts_enable() )
	{		        		
        //--- The MTS status will be enable after 10 seconds if the door status is close
        
        set_rs_mts_lock( TRUE );
        notify_send    ( NOTIFY_MTS_ENABLE ); 
				cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_ON, 100 );
				        
        if( !get_rs_turbo()  )
        {            			
					notify_send( NOTIFY_RUNTIME_REMAINING  , 0); 
					notify_send( NOTIFY_RUNTIME_PROGRAMMED , 0);
																										
					os_tsk_wait(500); 
					
					dei_ss_lock_stored = get_ss_lock() || get_ss_arm();  // update status to determine Smart Safelock								
					dei_rs_evt_signal( DEI_RS_EVT_MTS_LOCK_SHUTDOWN );	
																	
					TRACE_DEI_RS("\n\r[MTS ENABLE ENGINE STOPPED]\n\r");        				
        }
        else
        {
					TRACE_DEI_RS("\n\r[MTS ENABLE ENGINE RUNNING]\n\r");        
        }

        dei_rs_mts_door_checking_timeout_start();	
	}	
}

//--------------------------------------------------------------------------//


void dei_rs_mts_door_checking_timeout_start( void )
{    
    PER_EXEC_ATOMIC
    (
        out_q_remove( out_q_main , (Out_Func)dei_rs_mts_door_checking_timeout  , TRUE );
        
        ATOMIC( dei_rs_mts_vars.timeout_in_progress = TRUE );
        
        out_q_insert( out_q_main , (Out_Func)dei_rs_mts_door_checking_timeout  , DEI_RS_MTS_DOOR_CHECKING_TIMEOUT , 1 , 1 , 1); 
    )
}

//--------------------------------------------------------------------------//

void dei_rs_mts_door_checking_timeout( Boolean status )
{
    if( status )
    {
        ATOMIC( dei_rs_mts_vars.timeout_in_progress = FALSE );
        
        if( (get_door_status() == TRUE) || (get_trunk_status()==TRUE) ) //--- The door/trunk is still open , so disable the MTS
        {
					  set_rs_mts( FALSE );
            notify_send  ( NOTIFY_MTS_DISABLE ); 
						cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
            
            notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );                
            
            dei_prk_flash( DEI_PRK_MTS_OFF ); 

            TRACE_DEI_RS("\n\r[DOOR CHECKING : MTS DISABLE]\n\r");
        }
        else
        {
            if( get_ss_trigger() == FALSE )
            {
                set_rs_mts( TRUE );
							
#if 0   //extra code inherited from CM800
								cmd_q_rs_insert( INPUT_ARM              , 100 , (void*)CMD_LOCK_IGNITION );			
								cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1 | INPUT_ON , 100 , CMD_LOCK_WITH_ARM ) ; 
#endif

                TRACE_DEI_RS("\n\r[DOOR CHECKING : MTS ENABLE]\n\r");
            }
            else
            {
                TRACE_DEI_RS("\n\r[DOOR CHECKING : MTS DISABLE TRIGGER]\n\r");                
            }
        }                                                   
    }
}                    

//--------------------------------------------------------------------------//

void dei_rs_mts_door_checking_disable( void )
{
     PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)dei_rs_mts_door_checking_timeout  , TRUE ) );
     
     if( dei_rs_mts_vars.timeout_in_progress == TRUE )
     {
        //--- The timeout is in progress          
        if( dei_rs_state == NULL )
        {
            // --- the car is not remote started anymore
            // --- notify the user about MTS disable
					  set_rs_mts( FALSE );
            notify_send( NOTIFY_MTS_DISABLE    ); 
						cmd_q_rs_insert( INPUT_GND_WHEN_READY_MODE_OFF, 100 );
					
            
            os_tsk_wait( 2500 ); //--- Wait the system to notify the user

            notify_send  ( NOTIFY_ERROR  , NOTIFY_PLD_ERROR_RS_PROGRAMMING );                
            
            dei_prk_flash( DEI_PRK_MTS_OFF ); 

            TRACE_DEI_RS("\n\r[DOOR CHECKING UNLOCK : MTS DISABLE]\n\r");
        }
        else
        {
            // --- the car is still remote started ( idle/turbo mode )
            set_rs_mts_lock( FALSE );                        

            TRACE_DEI_RS("\n\r[DOOR CHECKING UNLOCK : MTS IN PROGRESS]\n\r");
        }       
        
        ATOMIC( dei_rs_mts_vars.timeout_in_progress = FALSE ); 
     }
}

//--------------------------------------------------------------------------//

void dei_rs_mts_door_checking_timeout_stop( void )
{
    PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)dei_rs_mts_door_checking_timeout  , TRUE ) );

    ATOMIC( dei_rs_mts_vars.timeout_in_progress = FALSE );
}    

//--------------------------------------------------------------------------//

Boolean dei_rs_mts_door_checking_timeout_status( void )
{
    return  dei_rs_mts_vars.timeout_in_progress ;
}

//--------------------------------------------------------------------------//
void dei_rs_mts_door_open(void)
{           
    if(!dei_rs_mts_vars.door_open_status)
    {
        dei_rs_mts_vars.door_open_status = TRUE;

        vrtc_stop(dei_rs_mts_vars.timeout_door_check);
    }
}

//--------------------------------------------------------------------------//

void dei_rs_mts_door_close(void)
{           
    if(dei_rs_mts_vars.door_open_status)
    {
        dei_rs_mts_vars.door_open_status = FALSE;
				dei_rs_mts_vars.door_open_close_cycle = TRUE;
        if( get_rs_mts_progress()     &&
					  get_ign_status     ()     && (get_rpm() > DEI_RS_RPM_MIN) &&
					 !get_brake_status()        &&	get_handbrake_status()      &&
					 !get_door_status() )
        {
					if( DEI_RS_MTS_COMPLETION == DEI_MANUAL_TRANS_COMPL_2 
						 || DEI_RS_MTS_COMPLETION == DEI_MANUAL_TRANS_COMPL_3)
					{
						// start 8 sec timer for passive locking
						vrtc_stop( dei_rs_mts_vars.timeout_door_check );
						vrtc_update( dei_rs_mts_vars.timeout_door_check, DEI_RS_MTS_PASSIVE_LOCK_TIMEOUT );
						vrtc_start( dei_rs_mts_vars.timeout_door_check );
						
						TRACE_DEI_RS("\n\r[MTS ENABLE BY DOOR CLOSE W/WO PASSIVE LOCK ]\n\r");
					}
        }
		}
}

//--------------------------------------------------------------------------//

void dei_rs_mts_ign_on(void) {

	dei_rs_mts_vars.door_open_close_cycle=FALSE;		
	}

//--------------------------------------------------------------------------//
void dei_rs_mts_door_check_tmr(void)
{
    if(!get_rs_mts() && !get_door_status() && !get_trunk_status() && !get_hood_status())
    {
			if(DEI_RS_MTS_COMPLETION == DEI_MANUAL_TRANS_COMPL_3)  //ENABLE MTS with Door close and passive locking
			{
          cmd_q_rs_insert(INPUT_RF_SILENT_LOCK , 100 , 0);
				  dei_system_mts_by_passive_locking = TRUE;
					dei_rs_mts_enable_event();
          TRACE_DEI_RS("\n\r[MTS ENABLE BY DOOR OPEN/CLOSE TIMEOUT]\n\r");
			}
			else if(DEI_RS_MTS_COMPLETION == DEI_MANUAL_TRANS_COMPL_2)
			{
					dei_rs_mts_enable_event();
					TRACE_DEI_RS("\n\r[MTS ENABLE BY DOOR CLOSE IN 8 SEC ]\n\r");
			}
		}
		else
		{
			vrtc_stop( dei_rs_mts_vars.timeout_door_check );
			vrtc_update( dei_rs_mts_vars.timeout_door_check, DEI_RS_MTS_PASSIVE_LOCK_TIMEOUT );
			vrtc_start( dei_rs_mts_vars.timeout_door_check );
		}
}
	


//--------------------------------------------------------------------------//


