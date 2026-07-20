/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: out_private.h 1046 2010-04-08 12:51:11Z florin.olariu $
/*==========================================================================*/

#ifndef __DL_PRIVATE_H__
#define __DL_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "cmd.h"
#include "timeout_q.h"
#include "trace.h"
#include "dl.h"
#include "status_system.h"
#include "timeout_q.h"
#include <string.h>
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define DL_TR_TIMEOUT               100*60     //60  sec 
#define DL_TR_RELOCK_TIMEOUT        100*10     //10  sec
#define DL_TR_REARM_TIMEOUT         100*2      //2   sec
#define DL_PULSE                    80         //800 ms
#define DL_OFFSET                   30         //300 ms  
#define DL_RELOCK_TIMEOUT           500        //5   sec
#define DL_UNLOCK_ALL_TIMEOUT       600        //6   sec
#define DL_CMD_INSERT_TIMEOUT       500        //500 ms
#define DL_DISARM_FAILES_TIMEOUT    100*2
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
typedef struct s_Dl
{
#if LIB_DL_DISABLE_ANALOG == 0
    Dl_Out out;
#endif
	
    Dl_Evt evt;
    
#if LIB_DL_DISABLE_ANALOG == 0
    Boolean unlock_out;
    Boolean lock_out;
    Boolean trunk_out;
#endif
	
    Boolean do_unlock_all;
    Boolean relock_window;
    Boolean do_unlock_after_gwr;
    
    Boolean lock_status;
    Boolean oem_disable;
    
    
    Boolean unlock_dd_enable;
    Boolean unlock_after_disarm_enable;
        
    Boolean tr_oem;
    Boolean tr_lock_status;
    Boolean tr_status;
}Dl;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Dl dl; 
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

//--------------------------------------------------------------------------//

void             dl_lock                     (void);
void             dl_do_lock                  (void);

#if LIB_DL_DISABLE_ANALOG == 0
void             dl_do_lock_out_reset        (void);
Timeout_Q_Status dl_lock_ctrl_timeout        (void);
#endif

void             dl_do_lock_oem              (void);

void             dl_unlock                   (void);
void             dl_unlock_all               (void);

#if LIB_DL_DISABLE_ANALOG == 0
void             dl_do_unlock_out_reset      (void);
#endif

void             dl_do_unlock                (void);
void             dl_do_unlock_after_dis      (void);

#if LIB_DL_DISABLE_ANALOG == 0
Timeout_Q_Status dl_unlock_ctrl_timeout      (void);
#endif

Timeout_Q_Status dl_unlock_after_disarm_fails(void);
Timeout_Q_Status dl_unlock_all_timeout       (void);
void             dl_do_unlock_oem            (void);

void             dl_do_relock             (void);
void             dl_do_relock_reset       (void);
Timeout_Q_Status dl_relock_timeout        (void);

void             dl_tr_trigger_oem        (void);
void             dl_tr_trigger_rs         (void);
void             dl_tr_timeout            (void);
void             dl_tr_car_rearm          (void);
void             dl_tr_car_relock         (void);
void             dl_tr_reset_lock         (void);
void             dl_tr_reset_unlock       (void);
void             dl_tr_reset_doors        (void);
void             dl_tr_reset              (void);

#if LIB_DL_DISABLE_ANALOG == 0
void             dl_do_trunk              (Boolean later);
void             dl_do_trunk_out_reset    (void);
#endif

Timeout_Q_Status dl_trunk_ctrl_timeout    (void);
Timeout_Q_Status dl_oem_disable_expired   (void);
//void             dl_ctrl_out              (DL_OUT out);



//--------------------------------------------------------------------------//

#endif

