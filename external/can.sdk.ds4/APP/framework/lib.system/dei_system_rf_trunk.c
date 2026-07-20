/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_trunk.c 17934 2012-10-29 19:23:36Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
#include "dei_ss_private.h"
#include "sense_rs.h"
#include "prg_out.h"
#include "timestamp.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define POP_TRUNK_STATE_DONE 					0		// Idle state
#define POP_TRUNK_STATE_POPPED 				1		// Pop trunk command received
#define POP_TRUNK_STATE_WAIT_CLOSE 		2		// Waiting for trunk to close

#define POP_TRUNK_STATE_WAIT10MIN			3		// Wait 10 minutes in OFF mode
#define POP_TRUNK_STATE_WAIT_CLOSE_2	4		// Wait for trunk to close in OFF mode

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void dei_ss_zone_bypass_stop( void );
//void    dei_ss_arm_actions         ( Dei_Ss_Silent silent , void *remote );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

Boolean dei_system_prev_lock_status = FALSE;			// State of provious lock
//Boolean trunk_state = FALSE;											// Trunk state
int pop_trunk_state = POP_TRUNK_STATE_DONE;				// Pop trunk state machine state

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

// trilock
#define TRILOCK_INTERVAL 3
time_t trilock_stamp = 0;
int trilock_cnt = 0;

//-------- PTIM section ---- //


// Power Tailgate Interrupt Module
// the PTIM Relay is normally open, so trunk commands from oem (or us) cannot activate the motor
UInt8 ptim_state = 0;
UInt8 ptim_waited = 0;
#define PTIM_WAIT_MAX 5

void ptim_timer_start(unsigned seconds)
{
	vrtc_stop( dei_system_pop_trunk_vrtc );
	vrtc_update( dei_system_pop_trunk_vrtc , VRTC_1SEC*seconds );
	vrtc_start( dei_system_pop_trunk_vrtc );
}

void trilock_lock(void)
{
	if(trilock_cnt < 0)
		trilock_cnt = 0;
	
	if(++trilock_cnt == 1)
	{
		trilock_stamp = time(0);
		ptim_timer_start(TRILOCK_INTERVAL+1);
	}
	if(trilock_cnt == 3)
		cmd_q_rs_insert( INPUT_SLIDE_CLOSE , 100 );
		
}

void trilock_unlock(void)
{
	if(trilock_cnt > 0)
		trilock_cnt = 0;
	
	if(--trilock_cnt == -1)
	{
		trilock_stamp = time(0);
		ptim_timer_start(TRILOCK_INTERVAL+1);
	}
	else if(trilock_cnt == -3)
		cmd_q_rs_insert( INPUT_SLIDE_OPEN , 100 );
}

void trilock_timeout(void)
{
	// check trilock
	if(trilock_stamp && time(NULL) - trilock_stamp >= TRILOCK_INTERVAL)
	{// trilock timeout
    trilock_cnt = 0;
		trilock_stamp = 0;
	}
}




static UInt8 ptim_stopped = 0;  // FIXME: this needs to be connected to some input

// user wants the vehicle to perform a Trunk command
void ptim_begin(void)
{
	switch(ptim_state)
	{
		case 0:
			ptim_waited = 0;
			ptim_state = 1;

		  if(ptim_stopped) // if we need it...
        cmd_q_rs_insert( INPUT_SLIDE_PARTIAL , 100 );		// send the vent command

			ptim_timer_start(2);
		  break;
	}
}


void ptim_timeout(void)
{	
	switch(ptim_state)
	{
		case 1:
			if(ptim_stopped)
			{
				if(++ptim_waited < PTIM_WAIT_MAX)
					ptim_timer_start(1); // wait a little longer
				else
					ptim_state = 0; // cancelled because system is still stopped
			}
			else
			{
				ptim_state = 2;
				prg_out_execute(1+PRG_OUT_PTIM_RELAY,1); // energize relay
				ptim_timer_start(15);
				cmd_q_rs_insert( INPUT_TRUNK | INPUT_ON  , 100, CMD_TRUNK_WITH_DISARM  );  // pop trunk
			}
			break;
		case 2:
			ptim_state = 0;
		  prg_out_execute(1+PRG_OUT_PTIM_RELAY,0); // stop relay
		  break;
		
	}
  		
}

void dei_system_ptim_stop_on(void)
{
	ptim_stopped = 1;
}

void dei_system_ptim_stop_off(void)
{
	ptim_stopped = 0;
}

//--------------------------------------------------------------------------//
// cap touch

void dei_system_ext_open_trigger(void)
{
	if(prg_out_shadow_read(1+PRG_OUT_STS_PROX))
	{
    cmd_q_rs_insert( INPUT_SLIDE_OPEN , 100 );
	}	
}

void dei_system_ext_close_trigger(void)
{
	if(prg_out_shadow_read(1+PRG_OUT_STS_PROX))
	{
    cmd_q_rs_insert( INPUT_SLIDE_CLOSE , 100 );
	}	
}


void dei_system_ext_trunk_trigger(void)
{
	if(get_trunk_status() || prg_out_shadow_read(1+PRG_OUT_STS_PROX))
	{
    cmd_q_rs_insert( INPUT_RF_TRUNK | INPUT_RF_EXT , 100 );
	}	
}
//-----------------------------------------------------------------------//
// proximity
UInt32 prox_stamp = 0;
#define PROX_TIME_AMT (2*60*60*VRTC_1SEC)

static void prox_off(void)
{
	prox_stamp = 0;
	prg_out_execute(1+PRG_OUT_STS_PROX,0);
}

void dei_system_prox_oem_arm(void)
{
	trilock_lock();
	prox_off();
}

void dei_system_prox_rf_lock(void)
{
	prox_off();
}
void dei_system_prox_oem_disarm(void)
{
	trilock_unlock(); // this will play with timer
	prox_stamp = time(NULL);
	prg_out_execute(1+PRG_OUT_STS_PROX,1);		
}
void dei_system_prox_rf_unlock(void)
{
	prox_stamp = time(NULL);
	ptim_timer_start(PROX_TIME_AMT);
	prg_out_execute(1+PRG_OUT_STS_PROX,1);
}

//--------------------------------------------------------------------------//


void dei_system_rf_trunk( void *remote )
{	
	if( dei_rs_mts_ready_for_activation() )
	{
		cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );

		return  ; 
	}
	
  dei_system_rf_range_check();		
}


//--------------------------------------------------------------------------//

void dei_system_rf_trunk_ext( void *remote )
{
	System_Type	systype;
	
  if( prg_out_user_adj(1+PRG_OUT_PTIM_RELAY,0) ) // is there any PTIM_RELAY configured?
	{
		if(ptim_state == 0)
		{
			ptim_begin();
		}
		return;			
	}
	
	
  if( get_ss_panic()  || get_ss_trigger()  ) return ;
	
	// Save previous locked status
	dei_system_prev_lock_status = get_ss_lock();
	// Get system type
	if(nvfs_rd_s( nvfs_sys_type , &systype, sizeof(systype) ) != NVFS_OK)
		systype = sys_type_assumed;
	
	// Disarm with trunk ON =========================================================================================
	
	if( dei_feature_get( DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE )==DEI_UNLOCK_DISARM_WITH_TRUNK_ON ) {
		
		if( get_ss_arm() || ((systype==1)&&(get_ss_lock())) ) { // If armed, then disarm (without notifying HHU)
			cmd_q_rs_insert(INPUT_RF_UNLOCK,100,(void*)CMD_TRUNK_WITH_DISARM );				
        }
		cmd_q_rs_insert( INPUT_TRUNK | INPUT_ON  , 100 , (void*)CMD_TRUNK_WITH_DISARM ) ; // Pop Trunk
		notify_send( NOTIFY_TRUNK_RELEASE_OPEN );
    }
	
	// Disarm with trunk OFF ========================================================================================
	
	if ( dei_feature_get( DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE )==DEI_UNLOCK_DISARM_WITH_TRUNK_OFF ) {
		
		if( get_ss_arm() || ((systype==1)&&(get_ss_lock())) ) { // If armed, then disarm (without notifying HHU)
			// Set state to Wait10Min
			pop_trunk_state=POP_TRUNK_STATE_WAIT10MIN;
			// Sensor bypass for 10 minutes
			sensor_bypass( ss_sensor[0] , VRTC_1SEC*60*10 );
			sensor_bypass( ss_sensor[1] , VRTC_1SEC*60*10 ); 
			sensor_bypass( ss_sensor[2] , VRTC_1SEC*60*10 );
			if(ext_sensor_trigger_configured)
				sensor_bypass( ss_sensor_ext , VRTC_1SEC*60*10 );
			// Pop the trunk
			cmd_q_rs_insert( INPUT_TRUNK | INPUT_ON  , 100  ); 
			// Start 10 min. timer
			vrtc_update( dei_system_pop_trunk_vrtc , VRTC_1SEC*60*10 );
			vrtc_start( dei_system_pop_trunk_vrtc );
        }
		else {
			cmd_q_rs_insert( INPUT_TRUNK | INPUT_ON  , 100  ) ; 	// Pop the trunk		
        }
		notify_send( NOTIFY_TRUNK_RELEASE_OPEN );
    }	

	// Disarm with trunk Smart Unlock ===============================================================================
	
	if ( dei_feature_get( DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE )==DEI_UNLOCK_DISARM_WITH_TRUNK_SMART ) {
		
		if( get_ss_arm() || ((systype==1)&&(get_ss_lock())) ) { // If armed then disarm (without notifying HHU)
			// Disarm
            cmd_q_rs_insert(INPUT_RF_UNLOCK,100,(void*)CMD_TRUNK_WITH_DISARM );
			os_tsk_wait(50);
			// Pop the trunk
			cmd_q_rs_insert( INPUT_TRUNK | INPUT_ON  , 100 , (void*)CMD_TRUNK_WITH_DISARM ) ; // Pop the trunk 
			// Start 30s timer
			vrtc_update( dei_system_pop_trunk_vrtc , VRTC_1SEC*30 );
			vrtc_start( dei_system_pop_trunk_vrtc );
			// Change state to POPPED
			pop_trunk_state=POP_TRUNK_STATE_POPPED;
        }
		else { // Not armed, don't bother disarming
			cmd_q_rs_insert( INPUT_TRUNK | INPUT_ON  , 100 , (void*)CMD_TRUNK_WITH_DISARM ) ; // Pop the trunk
        }
		notify_send( NOTIFY_TRUNK_RELEASE_OPEN );
    }
}	

//--------------------------------------------------------------------------//

void dei_system_rf_trunk_release( void *remote )
{  
		if( get_ss_panic()  || get_ss_trigger()  ) return ;
	
		cmd_q_rs_insert( INPUT_TRUNK , 100 , NULL );
}

//--------------------------------------------------------------------------//

void RearmAndRelock() {
	
#if 0
	set_ss_lock(TRUE);
	cmd_q_rs_insert_hhu_num( dei_sc_get_hhu_num() , INPUT_LOCK1 | INPUT_ON , 100 , CMD_LOCK_WITH_ARM ) ; 
	set_ss_arm ( TRUE );
	cmd_q_rs_insert( INPUT_ARM , 100 ) ;
	dei_ss_arm_actions( DEI_SS_NO_SILENT , NULL);
	if( dei_feature_get(DEI_FEAT_PLIGHT_CONFIRMATION) != 2 ) {
		dei_prk_flash( get_rs_gwr() ? DEI_PRK_LOCK_RS : DEI_PRK_LOCK );
		}
#else	
	cmd_q_rs_insert( INPUT_RF_LOCK , 100 , 0 ) ;
#endif
	}

void dei_system_trunk_close(void) {
	
	//trunk_state=FALSE;
	prg_out_execute(1+PRG_OUT_STS_TRUNK,0); // tell anyone who is interested
	
	if(pop_trunk_state==POP_TRUNK_STATE_WAIT_CLOSE) {
	
		// Wait for 1 second
		os_tsk_wait(1000) ;
		RearmAndRelock();
		pop_trunk_state=POP_TRUNK_STATE_DONE;
		}
	else if (pop_trunk_state==POP_TRUNK_STATE_WAIT_CLOSE_2) {
		// Sensor bypass reset to 1 second
		sensor_bypass( ss_sensor[0] , VRTC_1SEC );
		sensor_bypass( ss_sensor[1] , VRTC_1SEC ); 
		sensor_bypass( ss_sensor[2] , VRTC_1SEC );
		if(ext_sensor_trigger_configured)
			sensor_bypass( ss_sensor_ext , VRTC_1SEC );
		// Set state to DONE
		pop_trunk_state=POP_TRUNK_STATE_DONE;
		}
		
	}

void dei_system_trunk_open(void) {
	
	//trunk_state=TRUE;
	prg_out_execute(1+PRG_OUT_STS_TRUNK,1); // tell anyone who is interested
	
	// If trunk state machine in POPPED state
	if((pop_trunk_state==POP_TRUNK_STATE_POPPED)||(pop_trunk_state==POP_TRUNK_STATE_WAIT_CLOSE)) {
		// Stop timer
		vrtc_stop( dei_system_pop_trunk_vrtc );
		// Go to WAIT_CLOSE state
		pop_trunk_state=POP_TRUNK_STATE_WAIT_CLOSE;
		}
	else if(pop_trunk_state==POP_TRUNK_STATE_WAIT10MIN) {
		vrtc_stop( dei_system_pop_trunk_vrtc );
		pop_trunk_state=POP_TRUNK_STATE_WAIT_CLOSE_2;
		}
	}

// Trunk pop timeout	
	
void dei_system_pop_trunk_timeout( void ) {	

	UInt32 now = time(NULL);
	
	if(prox_stamp && now - prox_stamp >= PROX_TIME_AMT)
	{
		prox_off();		
	}
	if(prox_stamp)
	{
		prox_stamp = now;
		ptim_timer_start(PROX_TIME_AMT);
	}
	
	trilock_timeout();
	
	if(prg_out_user_adj(1+PRG_OUT_PTIM_RELAY,0))
	{
		ptim_timeout();
		return;
	}
	
	if(pop_trunk_state==POP_TRUNK_STATE_POPPED) { // 30 second timeout
		RearmAndRelock();
		}
    else if ( pop_trunk_state == POP_TRUNK_STATE_WAIT10MIN ){  // 10 minute by pass  trunk never opened
        set_ss_trunk_bypass( FALSE );   // End trunk bypass
        dei_ss_zone_bypass_start();     // Timeout zone bypass in 5 sec
    }
	pop_trunk_state=POP_TRUNK_STATE_DONE; // Return to done state
}

void dei_system_rf_poptrunk_zone_open( void) {
	
	if((pop_trunk_state==POP_TRUNK_STATE_POPPED)||(pop_trunk_state==POP_TRUNK_STATE_WAIT_CLOSE)) {
		// Stop timer
		vrtc_stop( dei_system_pop_trunk_vrtc );
		// Go to DONE state
		pop_trunk_state=POP_TRUNK_STATE_DONE;
		}
	}

void dei_system_rf_trunk_disarm( void *cmdpld ) {
	
  if(!( *cmd_get_pld( cmdpld ) & CMD_TRUNK_WITH_DISARM ))	{
		vrtc_stop( dei_system_pop_trunk_vrtc );
		pop_trunk_state=POP_TRUNK_STATE_DONE;
		}
	}
	
void dei_system_oem_trunk( void *remote )
{
    System_Type	systype;
	
	  if(prg_out_user_adj(1+PRG_OUT_PTIM_RELAY,0))  // ptim relay defined?
		{
			ptim_begin();
			return;
		}
	
    if( get_ss_panic()  || get_ss_trigger()  ) return ;

	// Get system type
	if(nvfs_rd_s( nvfs_sys_type , &systype, sizeof(systype) ) != NVFS_OK)
		systype = sys_type_assumed;
	
	// Disarm with trunk ON =========================================================================================
	if( dei_feature_get( DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE )==DEI_UNLOCK_DISARM_WITH_TRUNK_ON ) {
		
		if( get_ss_arm() || ((systype==1)&&(get_ss_lock())) ) { // If armed, then disarm 
            cmd_q_rs_insert(INPUT_OEM_DISARM,100,(void*)CMD_TRUNK_WITH_DISARM );
			os_tsk_wait(100);		
        }
		notify_send( NOTIFY_TRUNK_RELEASE_OPEN );
    }
	
	// Disarm with trunk OFF ========================================================================================
	
	if ( dei_feature_get( DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE )==DEI_UNLOCK_DISARM_WITH_TRUNK_OFF ) {
		
		if( get_ss_arm() || ((systype==1)&&(get_ss_lock())) ) { // If armed, then disarm (without notifying HHU)
			// Set state to Wait10Min
			pop_trunk_state=POP_TRUNK_STATE_WAIT10MIN;
			// Sensor bypass for 10 minutes
			sensor_bypass( ss_sensor[0] , VRTC_1SEC*60*10 );
			sensor_bypass( ss_sensor[1] , VRTC_1SEC*60*10 ); 
			sensor_bypass( ss_sensor[2] , VRTC_1SEC*60*10 );
			if(ext_sensor_trigger_configured)
				sensor_bypass( ss_sensor_ext , VRTC_1SEC*60*10 );
			// Start 10 min. timer
			vrtc_update( dei_system_pop_trunk_vrtc , VRTC_1SEC*60*10 );
			vrtc_start( dei_system_pop_trunk_vrtc );
        }
		notify_send( NOTIFY_TRUNK_RELEASE_OPEN );
    }	

	// Disarm with trunk Smart Unlock ===============================================================================
	
	if ( dei_feature_get( DEI_FEAT_UNLOCK_DISARM_WITH_TRUNK_RELEASE )==DEI_UNLOCK_DISARM_WITH_TRUNK_SMART ) {
		
		if( get_ss_arm() || ((systype==1)&&(get_ss_lock())) ) { // If armed then disarm
			// Disarm
            cmd_q_rs_insert(INPUT_OEM_DISARM,100,(void*)CMD_TRUNK_WITH_DISARM );
			os_tsk_wait(100);
			// Start 30s timer
			vrtc_update( dei_system_pop_trunk_vrtc , VRTC_1SEC*30 );
			vrtc_start( dei_system_pop_trunk_vrtc );
			// Change state to POPPED
			pop_trunk_state=POP_TRUNK_STATE_POPPED;
        }
        
        notify_send( NOTIFY_TRUNK_RELEASE_OPEN );
    }
}	
	
	
