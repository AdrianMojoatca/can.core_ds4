/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_private.h 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

#ifndef __SENSE_RS_PRIVATE_H__
#define __SENSE_RS_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs.h"
#include "cmd.h"
#include "per_exec.h"
#include "ied.h"
#include "idb.h"
#include "tacho.h"
#include "status_system.h"
#include "out_q.h"
#include "wake.h"
#include "gpio.h"
#include "nvfs_usr.h"


#include "trace.h"
#include "dbg.h"

#ifdef PLATFORM_933
	#include "speed.h"
  #include "timeout_f.h"	
#endif


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

#define SENSE_RS_CMD_INIT_TIMEOUT               150     //1500ms

/*--------------------------------------------------------------------------*/

#define SENSE_RS_PIN_AUTO_MANUAL                !IN_AUTO_MANUAL
#define SENSE_RS_PIN_SENSOR                     IN_SENSOR
#define SENSE_RS_PIN_PUSH_BOARD                 !IN_PUSH
#define SENSE_RS_PIN_PUSH_ANTENNA               IN_PUSH_VALET

/*--------------------------------------------------------------------------*/

#define SENSE_RS_PUSH_BOARD_DEBOUNCE            1      //<--for 10 ms
#define SENSE_RS_PUSH_ANTENNA_DEBOUNCE          10     //<--for 50  ms
#define SENSE_RS_SENSOR_DEBOUNCE_DEBOUNCE       5

/*--------------------------------------------------------------------------*/

#define SENSE_RS_SENSOR_PULSE_TRIGGER_MAX       1000    //<--for 10  seconds
#define SENSE_RS_SENSOR_PULSE_TRIGGER_MIN       80      //<--for 800 ms
#define SENSE_RS_SENSOR_PULSE_WARNING           70      //<--for 700 ms

#define SENSE_EXT_PRESS_TIMEOUT                 1500    // ext_trigger pulse longer than 1.5 sec and keep being ground

/*--------------------------------------------------------------------------*/
#if DEBUG_SENSE_RS == 1
#define TRACE_DEI_SENSE_RS          TRACE
#else
#define TRACE_DEI_SENSE_RS(...)     ;
#endif
/*--------------------------------------------------------------------------*/

#define TIMEOUT_Q_INSERT                        200

/*--------------------------------------------------------------------------*/

typedef Boolean (*Sense_Rs_Input_Read)( void );

/*--------------------------------------------------------------------------*/

typedef void (*Sense_Rs_Input_Change_Executor)(Boolean do_init , Boolean status, UInt8);

/*--------------------------------------------------------------------------*/

typedef struct s_Sense_Rs_Input_Cfg
{
    UInt32  time_0_to_1   ;
    UInt32  time_1_to_0   ;
        
}Sense_Rs_Input_Cfg;

/*--------------------------------------------------------------------------*/

typedef struct s_Sense_Rs_Input
{
    Ied             ied     ;    
    Sense_Rs_Input_Selector  selector;            
}Sense_Rs_Input ;

/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void                   sense_rs_pi_init    ( void );
Boolean                sense_rs_pi_status  ( Sense_Rs_Input_Selector selector );
Sense_Rs_Input_Cfg*    sense_rs_pi_cfg     ( Sense_Rs_Input_Selector selector );
void                   sense_rs_pi_executor( Sense_Rs_Input_Selector selector , Boolean do_init , Boolean status );
Per_Exec_Wake          sense_rs_pi_loop    ( void *dummy , Sense_Rs_Input *input );




void    sense_rs_rpm_init         ( void );
void    sense_rs_push_board_init  ( void );
void    sense_rs_push_antenna_init( void );
void    sense_rs_sensor_init      ( void );
void    sense_rs_gearbox_type     ( void );

#ifdef PLATFORM_CM800
	void    sense_rs_glowplug     ( Boolean do_init , Boolean status );
	void    sense_rs_safety_sw    ( Boolean do_init , Boolean status );
	void    sense_rs_custom_neg1  ( Boolean do_init , Boolean status );
	void    sense_rs_custom_neg2  ( Boolean do_init , Boolean status );
	void    sense_rs_custom_neg3  ( Boolean do_init , Boolean status );
	void    sense_rs_custom_pos2  ( Boolean do_init , Boolean status );
#else
	void sense_rs_headlight_detect( Boolean do_init , Boolean status, UInt8 from);
	void sense_rs_inst_trig( Boolean do_init , Boolean status, UInt8 from);
	void sense_rs_valet_switch( Boolean do_init , Boolean status, UInt8 from);
	void sense_rs_ext_sensor_trigger(Boolean do_init , Boolean status, UInt8 from);
	void sense_rs_tamper( Boolean do_init, Boolean status, UInt8 from);
	void sense_rs_ptim_stop( Boolean do_init, Boolean status, UInt8 from);
	void sense_rs_ext_open( Boolean do_init, Boolean status, UInt8 from);
	void sense_rs_ext_close( Boolean do_init, Boolean status, UInt8 from);
	void sense_rs_ext_trunk( Boolean do_init, Boolean status, UInt8 from);
void sense_rs_not_configured( Boolean do_init , Boolean status, UInt8 from);
	
#endif
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern       Sense_Rs_Input_Config *sense_rs_input_config;
extern const Sense_Rs_Input_Config  sense_rs_input_config_default;
#ifdef PLATFORM_933
extern Timeout sense_ext_trigger_timer;
extern UInt32 sense_rs_doors_sts;
extern UInt32 sense_rs_hoods_sts;
extern UInt32 sense_rs_trunks_sts;
extern UInt32 sense_rs_igns_sts;
extern UInt32 sense_rs_brakes_sts;
extern UInt32 sense_rs_handbrakes_sts;
extern UInt32 sense_rs_headlights_sts;
extern UInt32 sense_rs_extrigs_sts;
extern UInt8 sense_rs_door_defined;
extern UInt32 sense_rs_tamper_sts;
extern UInt32 sense_rs_ptim_stop_sts;
extern UInt32 sense_rs_ext_open_sts;
extern UInt32 sense_rs_ext_close_sts;
extern UInt32 sense_rs_ext_trunk_sts;


#endif
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
#ifdef PLATFORM_933
void sense_ext_trigger_check_timeout(void);
#endif
/*==========================================================================*/

#endif


