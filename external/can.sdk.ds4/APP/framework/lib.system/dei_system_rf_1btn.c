/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_1btn.c 19287 2013-02-18 12:08:05Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define OB_TIMEOUT 1700

#define OB_IDLE 0  // no command in progress
#define OB_TAP  1  // saw 1st tap.  timer running

static UInt8 ob_state = OB_IDLE;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_system_one_btn_function( void );
static void dei_system_one_btn_stop	( void );
static void ngmm_one_btn_function(void);
static void ngmm_one_btn_ext_function(void);
static void ngmm_one_btn_timeout(void *);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
Timeout ngmm_one_btn_timer;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_system_rf_1btn_init( void )
{		
    static const Cmd_Array1 cmd_array[] = 
    {
        { INPUT_RF_STOP 			   , (Exec_Func)dei_system_one_btn_stop    },
	    { INPUT_RF_ONE_BUTTON_FUNCTION , (Exec_Func)dei_system_one_btn_function}
    };

		ngmm_one_btn_timer = timeout_f_create(OB_TIMEOUT , ngmm_one_btn_timeout) ;
    CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
}

//--------------------------------------------------------------------------//

void ngmm_one_btn_function(void)  // tap
{
	if(ob_state == OB_IDLE)
	{// 1st tap. don't do anything yet...
		ob_state = OB_TAP;
		timeout_start(ngmm_one_btn_timer);
		dei_sc_pagging_rewind(OB_TIMEOUT+100);
	
		notify_send( NOTIFY_STANDBY );  // keep hhu waiting
	}
	else
	{// 2nd tap means unlock right away
		ob_state = OB_IDLE;
		timeout_stop(ngmm_one_btn_timer);
		cmd_q_rs_insert( INPUT_RF_UNLOCK , 100 , 0 );
	}
}

void ngmm_one_btn_ext_function(void) // hold
{
	if(ob_state == OB_IDLE)
	{
	  cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );		
	}
	else // was OB_TAP...
	{
		ob_state = OB_IDLE;
		timeout_stop(ngmm_one_btn_timer);
	  cmd_q_rs_insert( INPUT_RF_CAR_FINDER , 100 , 0 ); // bonus command		
	}
}

void ngmm_one_btn_timeout(void *arg)
{
	arg = arg; // not used
	
	if(ob_state != OB_IDLE) // so far, OB_TAP is the only other state...
	{
		ob_state = OB_IDLE;
	  cmd_q_rs_insert( INPUT_RF_LOCK , 100 , 0 );
	}
	// timer not running anymore
}


void dei_system_one_btn_stop( void )
{
  	UInt8 i ;
	UInt8 pulse_count = dei_feature_get( DEI_FEAT_ACTIVATION_PULSE_COUNT );
		
	if(onebutton_type)
	{
		ngmm_one_btn_ext_function();
	}
	else
	{
		if( get_rs_gwr() )
		{		
			
					if( get_rs_mts_progress() )
					{
							if( get_rs_mts_lock() == FALSE )
							{
									cmd_q_rs_insert( INPUT_RF_LOCK , 100 , 0 );                
							}
							else
							{        
									for( i = 0 ; i < pulse_count ; i ++ )
							{
								cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );
							}
							}
					}
					else
					{
							for( i = 0 ; i < pulse_count ; i ++ )
						{
							cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );
						}
					}            
		}
		else
		{
			cmd_q_rs_insert( INPUT_RF_CAR_FINDER  , 100 , 0 );
		}
  }
}

//--------------------------------------------------------------------------//

void dei_system_one_btn_function( void )
{
	if(dei_system_prg_mode_get())   // no start/stop action during programming mode
		return;
		
	if(onebutton_type)
	{
		ngmm_one_btn_function();
	}
	else
	{
  	if( get_rs_gwr() )
	  {		
    	cmd_q_rs_insert( INPUT_RF_UNLOCK , 100 , 0 );
	  }
	  else
	  {
		  cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );
	  }
  }
}

//--------------------------------------------------------------------------//

