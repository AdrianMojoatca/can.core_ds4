/*==========================================================================*/
// $Id: dei_out_ground_when_arm.c 19278 2013-02-18 07:53:08Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_out_private.h"
#include "notify.h"
#include "dei_system.h"
#include "bus_status.h" // for cssos timer stuff
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define DEI_STARTER_KILL_TIMEOUT		VRTC_1SEC*45

#define SK_STATE_ON			1
#define SK_STATE_OFF		3
#define SK_STATE_UNSOFF 6

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_out_gwa_delayed_activation( Boolean timeout_expired );
static void out_startkiller_activation( void );

void dei_out_sk_valet_on(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
Boolean dei_out_anti_grind_activated;
Boolean dei_out_passive_starter_kill_activated;
Boolean dei_out_active_starter_kill_activated;

// StarterKill/AntiGrind state variables

Boolean dei_out_sk = FALSE;	// Starter kill
Boolean dei_out_ag = FALSE; // Anti-Grind
Boolean dei_out_gwr = FALSE; // gwr

struct cssos_timer *dei_out_skag_timer;


int dei_out_sk_state = SK_STATE_ON;

Boolean dei_out_is_last_ign_from_rs = FALSE;
Boolean dei_out_sk_immediate = FALSE;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


//--------------------------------------------------------------------------//

Boolean dei_get_sk_state() {
	
	return dei_out_sk;
	}

// Update SK/AG logic

void dei_out_update_ag_sk(void) {
	Boolean result;
	
	result = dei_out_gwr ? dei_out_ag : (dei_out_sk && get_ign_status());
	
	prg_out_anti_grind(result);
	//if(dei_out_sk) TRACE("SK=TRUE ");
	//else TRACE("SK_OUT=FALSE ");
	//if(get_ign_status()) TRACE("IGN=TRUE ");
	//else TRACE("IGN=FALSE ");
	//if(dei_out_ag) TRACE("AG=TRUE ");
	//else TRACE("AG=FALSE ");
	//if(result) TRACE("OUT=TRUE\r\n");
	//else TRACE("OUT=FALSE\r\n");
	}

// Set StarterKill state

void dei_out_set_sk(Boolean state) {
	
	dei_out_sk=state;
	dei_out_update_ag_sk();
	}

// Set AntiGrind state	
	
void dei_out_set_ag(Boolean state) {
	
	dei_out_ag=state;
	dei_out_update_ag_sk();
	}

void dei_out_delay_ag(Boolean state)
{
	dei_out_ag=state;  // set the variable now...
	cssos_timer_rewind(dei_out_skag_timer,RT_SECOND);  // ..but update the output pin only later
}

// Set SK State

void dei_out_set_state(int state) {

	dei_out_sk_state=state;
	switch(dei_out_sk_state) {
		case SK_STATE_ON: 
			dei_out_set_sk(TRUE); 
			vrtc_stop( dei_out_starter_kill_vrtc );
			//TRACE("SK_STATE_ON\r\n"); 
			break;
		case SK_STATE_OFF: 
			dei_out_set_sk(FALSE); 
			vrtc_stop( dei_out_starter_kill_vrtc );
			//TRACE("SK_STATE_OFF\r\n"); 
			break;
		case SK_STATE_UNSOFF: 
			dei_out_set_sk(FALSE); 
			vrtc_update( dei_out_starter_kill_vrtc , dei_out_sk_immediate?VRTC_1SEC:DEI_STARTER_KILL_TIMEOUT );
			vrtc_start( dei_out_starter_kill_vrtc );
			//TRACE("SK_STATE_UNSOFF\r\n"); 
			break;
		}
	}	

// Starter Kill Init

void dei_out_sk_init() {
	
	dei_out_skag_timer = cssos_timer_create(dei_out_update_ag_sk);
	
	dei_out_starter_kill_vrtc = vrtc_f_create( (Exec_Func) dei_out_sk_timeout );
	notify_add(NOTIFY_VALET_ON,(Notify_Func)dei_out_sk_valet_on);
	notify_add(NOTIFY_COMBO_VALET_ON,(Notify_Func)dei_out_sk_valet_on);

	if(dei_feature_get( DEI_FEAT_STARTER_KILL ) == DEI_STARTER_KILL_BAC) {
		dei_out_set_state(SK_STATE_ON);
		}
	else if(dei_feature_get( DEI_FEAT_STARTER_KILL ) == DEI_STARTER_KILL_PASSIVE) {
		dei_out_set_state(SK_STATE_ON);
		}
	else if(dei_feature_get( DEI_FEAT_STARTER_KILL ) == DEI_STARTER_KILL_ACTIVE) {
		if(get_ss_arm()) {
			dei_out_set_state(SK_STATE_ON);
			//TRACE("ARMED ON POWER UP\r\n");
			}
		else {
			dei_out_set_state(SK_STATE_OFF);
			//TRACE("DISARMED ON POWER UP\r\n");
			}
		}	
	else if(dei_feature_get( DEI_FEAT_STARTER_KILL ) == DEI_STARTER_KILL_OFF) {
		dei_out_set_state(SK_STATE_OFF);
		}	
	}

// Starter Kill Unlock Handler

void dei_out_sk_unlock(void *payload) {

	dei_out_sk_immediate = FALSE;

	if(dei_system_prg_mode_get()==TRUE) return; // Do nothing if in prog mode
	
  if(( *cmd_get_pld( payload) & DEI_SS_SILENT_LOGIC_RESUME )) return; // Reject unlock after bootup
	if(dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_BAC) return; // Stay in SK_ON state if in BAC mode
	
	if(dei_out_sk_state==SK_STATE_ON) {
		if(dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_PASSIVE) { // Unlock & MODE_PSV
			dei_out_set_state(SK_STATE_UNSOFF);
			}
		else if((dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_ACTIVE) || get_ss_valet()) { 
			dei_out_set_state(SK_STATE_OFF);
			}
		}
	}

// Starter Kill Lock Handler
	
void dei_out_sk_lock() {
	
	if(dei_system_prg_mode_get()==TRUE) return; // Do nothing if in prog mode
	
	if(dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_OFF) return; // If in MODE_OFF, do not change state
	
	
	if((dei_out_sk_state==SK_STATE_OFF) && !get_ss_valet() && ((!get_ign_status()) || dei_out_is_last_ign_from_rs || get_rs_gwr())) {

		dei_out_set_state(SK_STATE_ON);
		}
	else if((dei_out_sk_state==SK_STATE_UNSOFF)&&(dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_PASSIVE)&&(!get_ign_status())) {
		dei_out_set_state(SK_STATE_ON);
		}
		dei_out_sk_immediate = TRUE;

	}

// Starter Kill Ignition ON Handler

void dei_out_sk_ign_on() {
	
	dei_out_is_last_ign_from_rs = get_rs_gwr();

	if(dei_system_prg_mode_get()==TRUE) return; // Do nothing if in prog mode
	
	dei_out_update_ag_sk();
	}

// Starter Kill Ignition OFF Handler
	
void dei_out_sk_ign_off() {
	
	if(dei_system_prg_mode_get()==TRUE) return; // Do nothing if in prog mode
	
	dei_out_update_ag_sk();
	if((dei_out_sk_state==SK_STATE_OFF)&&(dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_PASSIVE)&& (!get_ss_valet()) && (!dei_out_is_last_ign_from_rs)) {
		dei_out_set_state(SK_STATE_UNSOFF);
		}
  }
	
// Starter Kill Timer Timeout Handler
	
void dei_out_sk_timeout() {
	
	vrtc_stop( dei_out_starter_kill_vrtc );
	if(dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_BAC) dei_out_set_state(SK_STATE_ON);
	else if((dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_ACTIVE) || (dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_OFF)) {
		dei_out_set_state(SK_STATE_OFF);
		}
	else if(dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_PASSIVE) {
		if((!get_rs_gwr() && get_ign_status()) || get_ss_valet()) {
			dei_out_set_state(SK_STATE_OFF);
			}
		else {
			dei_out_set_state(SK_STATE_ON);
			dei_out_sk_immediate = TRUE;
			}
		}
	}

// BAC Cmd Handler	
	
void dei_out_sk_bac() {

	if(dei_system_prg_mode_get()==TRUE) return; // Do nothing if in prog mode
	
	if((dei_out_sk_state==SK_STATE_ON)&&(dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_BAC)) {
		dei_out_set_state(SK_STATE_UNSOFF);
		}
	}	

void dei_out_sk_gwr_off() {
	dei_out_gwr = FALSE;
		dei_out_update_ag_sk();
	}

void dei_out_sk_gwr_on() {
	
	if(dei_system_prg_mode_get()==TRUE) return; // Do nothing if in prog mode
	
	dei_out_gwr = TRUE;

	
//	if(((dei_out_sk_state==SK_STATE_ON) || (dei_out_sk_state==SK_STATE_OFF)) && (dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_PASSIVE) && (!get_ss_valet())) {
//		dei_out_set_state(SK_STATE_UNSOFF);
//		}
	
	dei_out_update_ag_sk();
}

// Valet ON	
	
void dei_out_sk_valet_on(void) {
	
	if(dei_system_prg_mode_get()==TRUE) return; // Do nothing if in prog mode
	
	if(dei_out_sk_state==SK_STATE_ON) {
		if((dei_feature_get(DEI_FEAT_STARTER_KILL)==DEI_STARTER_KILL_ACTIVE) || get_ss_valet()) { 
			dei_out_set_state(SK_STATE_OFF);
			}
		}
	dei_out_sk_immediate = FALSE;

	}
	

void dei_out_ground_when_arm( void *payload )
{
	PER_EXEC_ATOMIC
	(
		out_q_remove( out_q_main , (Out_Func)dei_out_gwa_delayed_activation , FALSE ) ; 

		out_q_insert( out_q_main , (Out_Func)dei_out_gwa_delayed_activation , 5 , 1 , 1 , 1 );
	);
}

//--------------------------------------------------------------------------//

void dei_out_gwa_delayed_activation( Boolean timeout_expired )
{
	if( timeout_expired )
		prg_out_ground_when_arm( get_ss_arm() ); 
		
}

//--------------------------------------------------------------------------//
// if valet on, make sure to deactivate if !ss_arm
void dei_out_ground_when_arm_deactivate( void *payload )
{
	if(!get_ss_arm())
		prg_out_ground_when_arm( FALSE ); 
}


