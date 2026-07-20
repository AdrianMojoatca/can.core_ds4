/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_private.h 33640 2015-09-15 12:59:32Z simon.demers $
/*==========================================================================*/

#ifndef __RSR_PRIVATE_H__
#define __RSR_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "rsr.h"
#include "core_contract_db3_rsr_config.h"
#include "timeout_f.h"
#include "nvfs.h"
#include "cmd.h"
#include "rsr_feature.h"
#include "dbg.h"
#include "gpio.h"
#include "core_contract_db3_gpio_output.h"

#if DBG_RSR == 1
  #warning "DBG_RSR Is Enable"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if CONFIG_RSR_VERSION != 5  // must be changed when config file is modified
    #error config_rsr does not match
#endif



#define RSR_CMD_INPUT_HOOD_OPEN       (rsr_in_cmd[RSR_HOOD_SEL] | (INPUT_ON ^ RSR_HOOD_POLARITY*INPUT_ON))
#define RSR_CMD_INPUT_HOOD_CLOSE      (rsr_in_cmd[RSR_HOOD_SEL] | (RSR_HOOD_POLARITY*INPUT_ON))
#define RSR_CMD_INPUT_VALET_ON        (rsr_in_cmd[RSR_VALET_SEL]| INPUT_ON)
#define RSR_CMD_INPUT_VALET_OFF       (rsr_in_cmd[RSR_VALET_SEL])

#define TASK_RSR_NAME  "rsr"
#define TASK_RSR_PRI   200     
#define TASK_RSR_STK   260

#define RSR_PTE_TIMEOUT      100

#define RSR_PARK_ERR_TIMEOUT   250
#define RSR_PARK_FCT_LENGHT    500

#define RSR_OUT_PAUSE        300
#define TRUNK_DURATION       3000

#define RSR_FAIL_TIMEOUT     2000

#define RSR_CMD_INSERT_TIMEOUT 500

#define RSR_DISARM_TO_GWR_TIMEOUT    500 // ms
#define RSR_GWR_TO_ACC_TIMEOUT       1800               //1ms base time
#define RSR_ACC_TO_IGN_TIMEOUT       100                //1ms base time
#define RSR_IGN_TO_START_TIMEOUT     2500               //1ms base time 
#define RSR_STOP_TO_REARM_TIMEOUT    3000               //1ms base time 

#define RSR_CUR_TIME                ((rsr_vars.running.sec * 1000) + rsr_vars.running.ms)

#define RSR_ERR_BUFF_SIZE   5

//--------------------------------------------------------------------------//

#define DEFAULT_RSR_RUNTIME                    15  // timebase minute
#define DEFAULT_RSR_RETRY_COUNTER              3
#define DEFAULT_RSR_RETRY_TIMEOUT              2   // timebase second

//--------------------------------------------------------------------------//

enum
{
  RSR_VER_1  = 1,
  RSR_VER_2  = 2,  

  RSR_VER_COUNT

};

#define RSR_VERSION        RSR_VER_2

//--------------------------------------------------------------------------//
typedef enum
{
    RSR_STATE_STOP    ,
    RSR_STATE_DISARM  ,
    RSR_STATE_GWR     ,
    RSR_STATE_ACC     ,
    RSR_STATE_IGN     ,
    RSR_STATE_START   ,
    RSR_STATE_RUN     ,
    RSR_STATE_FAIL    ,
    RSR_STATE_ARM     ,
    
    RSR_STATE_COUNT

} Rsr_State ;

//--------------------------------------------------------------------------//

typedef enum
{
    RSR_NO_ERR           = 0,
    RSR_ERR_RUNTIME      = 1 ,
    RSR_ERR_OVER         = 2,
    RSR_ERR_NO_RPM       = 3,
    RSR_ERR_RKE_STOP     = 4,
    RSR_ERR_BRAKE        = 5,
    RSR_ERR_HOOD         = 6,
    RSR_ERR_MANUAL       = 7,
    RSR_ERR_TRANS        = 8,
    RSR_ERR_LOW_BAT      = 9,
    RSR_ERR_SS_TRIGG     = 10,
    RSR_ERR_W2           = 11,     
    
    RSR_ERR_DOOR_OPEN    = 12,
        
    RSR_ERR_NO_ACC       = 13,
    RSR_ERR_NO_IGN       = 14,
    RSR_ERR_NO_START     = 15,
    RSR_ERR_NO_BRAKE     = 16,

    RSR_ERR_DTC          = 17,


} Rsr_Err_Code ;

//--------------------------------------------------------------------------//

typedef enum
{         
    RSR_PRK_STOP         ,        
    RSR_PRK_CONSTANT     ,    
     
}Rsr_Prk_Pattern;

//--------------------------------------------------------------------------//

typedef struct s_Rsr_Err
{
    UInt16      count                    ;  
    Rsr_Err_Code code [RSR_ERR_BUFF_SIZE] ;
  
} Rsr_Error ;

//--------------------------------------------------------------------------//

typedef void (* volatile Rsr_Callback)  (void)        ;

//--------------------------------------------------------------------------//

typedef struct s_Rsr_Run
{
    UInt16 ms  ;
    UInt8  sec ;
    UInt8  min ;

} Rsr_Run;

//--------------------------------------------------------------------------//

typedef struct s_Rsr_Vars
{
             Boolean            gwr_is_detect ;   
    volatile Boolean            valet_mode ;
    volatile Boolean            ign        ;
	  volatile Boolean            idle_mode  ;
    
    volatile Rsr_State          state      ;
    volatile Rsr_State          last_state ;
    volatile Rsr_Err_Code       err_code   ;
             Rsr_Prk_Pattern    prk_pattern; 
             Rsr_Callback       callback   ;
             Rsr_Run            running    ;

    volatile UInt8              runtime_remaining ;

    Flg        flg_start ;
    Flg        flg_stop  ;  
    Pte        pte       ;
    Tsk        tsk       ;
    Timeout    fail_timeout   ;
    Timeout    lock_start_timeout ;
    Timeout    start_delay_timeout;

} Rsr_Vars ;

//--------------------------------------------------------------------------//
#define SECONDE_TIMEBASE (1000)
#define RETRY_TIMEBASE   SECONDE_TIMEBASE

#define TACHLESS !!(rsr_feature_enabled(RS_FEAT_ENGINE_CHECK) && rsr_feature_get(RS_FEAT_ENGINE_CHECK))

#define RSR_RPM_MUL          100
#define RSR_ENGINE_RUNNING  ((rpm.word >= (rsr_opt.rpm_run * RSR_RPM_MUL)) || (TACHLESS && (rsr_vars.state == RSR_STATE_RUN)))
#define RSR_CRANK_RELEASE   (TACHLESS? FALSE : rpm.word >= (rsr_opt.rpm_crank * RSR_RPM_MUL))
#define RSR_ENGINE_OVER_REV (TACHLESS? FALSE : rpm.word >  (rsr_opt.rpm_high * RSR_RPM_MUL))

//--------------------------------------------------------------------------//
typedef struct s_Rsr_Dl
{
  UInt8     pulse ;   // pulse number
  UInt16    dur   ;   // Duration in ms

}Rsr_Dl ; 

//--------------------------------------------------------------------------//
typedef struct s_Rsr_Option
{
  /* GST */
  Boolean   dtc_check       ; // start if dtc exist
  Boolean   trans_check     ;

  /* Generic */
  UInt8     runtime         ; // timebase minute
  UInt32    crank_time      ; // timebase millisecond
  UInt8     retry_timeout   ; // timebase second
  
  UInt8     retry_count     ; // decimal
  
  UInt8     rpm_run         ; // (rpm_run * 100)    = RPM RUNNING
  UInt8     rpm_crank       ; // (rpm_crank * 100)  = RPM STARTER OFF 
  UInt8     rpm_high        ; // (rpm_high * 100)   = RPM HIGH

  /* DoorLock */
  Rsr_Dl    unlock          ;  
  Rsr_Dl    lock            ; 

}Rsr_Option ; 

//--------------------------------------------------------------------------//

typedef enum
{
    RSR_OUT_STATE_RDY    ,
    RSR_OUT_STATE_OFFSET ,
    RSR_OUT_STATE_SET    ,
    RSR_OUT_STATE_CLR    ,
    RSR_OUT_STATE_DONE   ,

} Rsr_Out_State ;

//--------------------------------------------------------------------------//

typedef enum
{
  #if RSR_DISARM_ENA == 1
    RSR_DISARM_OUT   ,
  #endif
  #if RSR_UNLOCK_ENA == 1
    RSR_UNLOCK_OUT   ,
  #endif
  #if RSR_LOCK_ENA == 1  
    RSR_LOCK_OUT     ,
  #endif
  #if RSR_ARM_ENA == 1
    RSR_ARM_OUT      ,
  #endif
  #if RSR_TRUNK_ENA == 1 
    RSR_TRUNK_OUT    ,
  #endif
 
    RSR_PARKING_OUT  ,

    RSR_OUT_COUNT    ,

} Rsr_Out_Action ;

//--------------------------------------------------------------------------//

typedef struct s_Rsr_Out_Time
{
    UInt16 offset ;
    UInt16 set    ;
    UInt16 clr    ;

} Rsr_Out_Time ;

//--------------------------------------------------------------------------//

typedef struct
{      
    Rsr_Out_State  state   ;
    Rsr_Out_Time   time    ;
    UInt8          pulse   ;
    Timeout        timeout ;

} Rsr_Out ;

//--------------------------------------------------------------------------//

#if DEFAULT_RSR_RPM_RUN_VALUE > DEFAULT_RSR_RPM_CRANK_VALUE   
  #error DEFAULT_RSR_RPM_RUN_VALUE must be smaller than RPM_CRANK_VALUE 
#endif

#if DEFAULT_RSR_RPM_CRANK_VALUE > DEFAULT_RSR_RPM_HIGH_VALUE
  #error DEFAULT_RSR_RPM_CRANK_VALUE must be smaller than RPM_HIGH_VALUE
#endif

#ifndef RSR_HOOD_POLARITY 
  #error "RSR_HOOD_POLARITY has to be defined in config_rsr. See newest template"
#endif

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Rsr_Vars   rsr_vars   ;
extern Rsr_Option rsr_opt    ;
extern Rsr_Error rsr_err     ;

extern const Cmd rsr_in_cmd[] ;

extern const NVFS_Entry_Name nvfs_rsr_error  ;
extern const NVFS_Entry_Name nvfs_rsr_ver    ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void rsr_start              (void * arg) ;
void rsr_start_toggle       (void * arg) ;
void rsr_stop               (void * arg) ;
void rsr_ext_runtime        (void * arg) ;

void rsr_valet_toogle       (void * arg) ;
void rsr_valet_on           (void * arg) ;
void rsr_valet_off          (void * arg) ;
void rsr_hood_open          (void * arg) ;
void rsr_hood_close         (void * arg) ;
void rsr_hood_detect        (void * arg) ;
void rsr_gwr_on             (void * arg) ;
void rsr_ignition_on        (void * arg) ;
void rsr_ignition_off       (void * arg) ;

void rsr_task               (void * tsk_arg) ;
void rsr_timing             (void);
void rsr_timing_reset       (void);

void          rsr_out_init   (void) ;
void          rsr_out_action (Rsr_Out_Action action, UInt16 offset, UInt16 on, UInt16 off, UInt8 pulse);
Rsr_Out_State rsr_out_ctl    (Rsr_Out_Action action) ;

void rsr_disarm              (void * arg) ;
void rsr_unlock              (void * arg) ;
void rsr_lock                (void * arg) ;
void rsr_arm                 (void * arg) ;
void rsr_trunk               (void * arg) ;

void rsr_disarm_timeout      (void * arg) ;
void rsr_unlock_timeout      (void * arg) ;
void rsr_lock_timeout        (void * arg) ;
void rsr_arm_timeout         (void * arg) ;
void rsr_trunk_timeout       (void * arg) ;
void rsr_parking_timeout     (void * arg) ;
void rsr_fail_timeout        (void * arg) ;

void rsr_req_runtime         (void * arg) ;

void rsr_oem_lock            (void * arg) ;
void rsr_oem_unlock          (void * arg) ;
void rsr_oem_trunk           (void * arg) ;
void rsr_oem_aux1            (void * arg) ;
void rsr_oem_aux2            (void * arg) ;

void rsr_notify_start        (Boolean success) ;
void rsr_err_save            (void) ;

void rsr_state_stop          (void) ;
void rsr_state_disarm        (void) ;
void rsr_state_gwr_on        (void) ;
void rsr_state_acc_on        (void) ;
void rsr_state_ign_on       (void) ;
void rsr_state_start_on      (void) ;
void rsr_state_run           (void) ;
void rsr_state_fail          (void) ;
void rsr_state_arm           (void) ;

void rsr_reset               (void) ;

void rsr_parking_pattern     (Rsr_Prk_Pattern pattern) ;
void rsr_parking_on          (void) ;
void rsr_parking_off         (void) ;
void rsr_parking_flash       (UInt16 length, UInt8 count) ;

void rsr_set_state           (Rsr_State    state)    ;
void rsr_set_callback        (Rsr_Callback callback) ;
void rsr_err_add             (Rsr_Err_Code code)     ;
Rsr_State rsr_get_last_state (void) ;

Boolean  rsr_init_nvfs       (void) ;

void rsr_input_init          (void) ;

void rsr_start_delay_timeout (void * arg) ;

#endif

