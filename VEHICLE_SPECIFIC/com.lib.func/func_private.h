/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: func_private.h 4229 2012-10-18 18:45:04Z florin.olariu $
/*==========================================================================*/
#ifndef __FUNCFUNC_PRIVATE_H__
#define __FUNCFUNC_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "func.h"
#include "sense.h"
#include "detect.h"

#include "sense_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

enum
{
    FUNC_CAN_WRU_RUN_IGN_ON        , 
    FUNC_CAN_WRU_RUN_IGN_OFF       ,
    FUNC_CAN_WRU_STOP_IGN_ON       ,  
    FUNC_CAN_WRU_STOP_IGN_OFF      , 

    FUNC_CAN_WRD_FRONT_RUN_IGN_ON  , 
    FUNC_CAN_WRD_FRONT_RUN_IGN_OFF ,    
    FUNC_CAN_WRD_REAR_RUN_IGN_ON   , 
    FUNC_CAN_WRD_REAR_RUN_IGN_OFF  ,
    
    
    FUNC_CAN_WRD_STOP_IGN_ON       ,  
    FUNC_CAN_WRD_STOP_IGN_OFF      ,
    
    FUNC_CAN_LIGHT_ON              ,
    FUNC_CAN_LIGHT_OFF             ,

    FUNC_CAN_LOCK_CL_ERROR         ,
    FUNC_CAN_UNLOCK_CL_ERROR       ,
    FUNC_CAN_DISARM                ,
    
    FUNC_CAN_WAKE                  ,

    FUNC_CAN_MSG_COUNT
};

/*--------------------------------------------------------------------------*/

typedef enum
{
    FUNC_ARM                 , 
    FUNC_DISARM              ,
    FUNC_LOCK                ,
    FUNC_UNLOCK              ,
    FUNC_TRUNK_RELEASE       ,
    FUNC_RELOCK_TRUNK_RELEASE,
    FUNC_OEM_LOCK            ,
    FUNC_OEM_UNLOCK          ,
    FUNC_OEM_TRUNK           ,
    //FUNC_WRU                 ,
    //FUNC_WRD_FRONT           ,
    //FUNC_WRD_REAR            ,
    //FUNC_GAS_CAP             ,
    FUNC_LIGHT               ,
    FUNC_LIGHT_DISABLE       ,    
    
    FUNC_PRC_COUNT

}Func_Prc ; 

/*--------------------------------------------------------------------------*/

#define FUNC_CAN_PER    ((func_cfg->can_dev_id == CAN_DEV_0) ? CAN_PER_DEV0 : CAN_PER_DEV1)
#define FUNC_CAN_DEV    (func_cfg->can_dev_id)

/*--------------------------------------------------------------------------*/

#define     FUNC_RS_TAKEOVER_TIMEOUT        90000
#define     FUNC_TIMEOUT_HAZARD_OUT         15    //150 ms
#define     FUNC_GWR_OFF_RELOCK_TIMEOUT		7*100 //7 seconds

/*--------------------------------------------------------------------------*/
#if DBG_FUNCTION == 1
#define     TRACE_FUNC      TRACE
#else
#define     TRACE_FUNC(...)   ;
#endif

#define DATA 			can_msg->data._8_
/*--------------------------------------------------------------------------*/
     
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void                func_cmd_init        ( void );
void                func_prc_init        ( void );

/*--------------------------------------------------------------------------*/

void                func_cmd_lock         ( void );
void                func_cmd_unlock       ( void );
void                func_cmd_trunk        ( void );
void                func_cmd_oem_lock     ( void );
void                func_cmd_oem_unlock   ( void );
void                func_cmd_oem_trunk    ( void );
void                func_cmd_light_on     ( void *arg );
void                func_cmd_light_off    ( void *arg );
void                func_cmd_dome_on      ( void );
void                func_cmd_dome_off     ( void );
void                func_cmd_aux1         ( void );
void                func_cmd_aux2         ( void );
void                func_cmd_aux3         ( void );

/*--------------------------------------------------------------------------*/

void                func_oem_lock_start   ( void );
Boolean             func_oem_lock_exec    ( void );
void                func_oem_lock_stop    ( void );

/*--------------------------------------------------------------------------*/

void                func_oem_unlock_start ( void );
Boolean             func_oem_unlock_exec  ( void );
void                func_oem_unlock_stop  ( void );

/*--------------------------------------------------------------------------*/

void                func_oem_trunk_start  ( void );
Boolean             func_oem_trunk_exec   ( void );
void                func_oem_trunk_stop   ( void );

/*--------------------------------------------------------------------------*/

void                func_arm_start       ( void );
void                func_arm_stop        ( void );
Boolean             func_arm_exec        ( void );

/*--------------------------------------------------------------------------*/

void                func_disarm_start    ( void );
void                func_disarm_stop     ( void );
Boolean             func_disarm_exec     ( void );

/*--------------------------------------------------------------------------*/

void                func_lock_start      ( void );
void                func_lock_stop       ( void );
Boolean             func_lock_exec       ( void );

/*--------------------------------------------------------------------------*/

void                func_unlock_all_start   ( void );
void                func_unlock_all_stop    ( void );
Boolean             func_unlock_all_exec    ( void );

/*--------------------------------------------------------------------------*/

void                func_trunk_release_start( void );
void                func_trunk_release_stop ( void );
Boolean             func_trunk_release_exec ( void );

/*--------------------------------------------------------------------------*/

void                func_gas_start          ( void );
void                func_gas_stop           ( void );
Boolean             func_gas_exec           ( void );

/*--------------------------------------------------------------------------*/

void                func_wru_start       ( void );
void                func_wru_stop        ( void );
Boolean             func_wru_exec        ( void );

/*--------------------------------------------------------------------------*/

void                func_wrd_front_start ( void );
void                func_wrd_front_stop  ( void );
Boolean             func_wrd_front_exec  ( void );

/*--------------------------------------------------------------------------*/

void                func_wrd_rear_start  ( void );
void                func_wrd_rear_stop   ( void );
Boolean             func_wrd_rear_exec   ( void );

/*--------------------------------------------------------------------------*/

void                func_light_start     ( void );
Boolean             func_light_exec      ( void );
void                func_light_stop      ( void );

/*--------------------------------------------------------------------------*/

void                func_hazard_init     ( void );

/*--------------------------------------------------------------------------*/

void 				func_rs_init		 ( void );
void                func_rs_gwr_on       ( void );
void                func_rs_gwr_off      ( Boolean do_shutdown);
void                func_rs_ign_on       ( void );
void                func_rs_ign_off      ( Boolean do_shutdown);
void                func_rs_str_on       ( void );
void                func_rs_str_off      ( void );
void                func_rs_release_keyin( void );

void                func_rs_pts_callback ( Boolean state );

/*--------------------------------------------------------------------------*/

void                func_eips_init       ( void );
void                func_eips_shutdown   ( void );
void                func_eips_fail       ( void *arg );
Timeout_Q_Status    func_eips_alert      ( void *arg );

/*--------------------------------------------------------------------------*/

void                func_can_wake_enter  ( void );
void                func_can_wake_leave  ( void );

/*--------------------------------------------------------------------------*/

void 			 func_cmd_gwr_off   	  ( void );
Timeout_Q_Status func_gwr_off_relock     ( void );

void tester_lock( void  );
void tester_unlock( void  );
void tester_trunk_rel( void  );
/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern const Can_Msg       func_can_table[FUNC_CAN_MSG_COUNT] ; 
extern const Process_Cfg   func_prc_cfg  [FUNC_PRC_COUNT];
extern       Process_Node* func_prc      [FUNC_PRC_COUNT];

extern       Boolean       func_light_done; 

extern       Func_Cfg*     func_cfg;


extern Boolean is_arm_lock;
extern Boolean is_disarm_unlock;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

