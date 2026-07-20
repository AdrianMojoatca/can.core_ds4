/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr.c 32204 2015-06-10 17:44:09Z simon.demers $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr.h"
#include "rsr_arm.c                   "
#include "rsr_disarm.c                "
#include "rsr_err_add.c               "
#include "rsr_err_save.c              "
#include "rsr_ext_runtime.c           "
#include "rsr_get_last_state.c        "
#include "rsr_gwr_on.c                "
#include "rsr_ignition.c              "
#include "rsr_hood.c                  "
#include "rsr_init.c                  "
#include "rsr_init_nvfs.c             "
#include "rsr_init_reset.c            "
#include "rsr_input_init.c            "
#include "rsr_lock.c                  "
#include "rsr_oem.c                   "
#include "rsr_out.c                   "
#include "rsr_parking.c               "
#include "rsr_parking_flash.c         "
#include "rsr_req_runtime.c           "
#include "rsr_reset.c                 "
#include "rsr_set_callback.c          "
#include "rsr_set_state.c             "
#include "rsr_start.c                 "
#include "rsr_state_acc_on.c          "
#include "rsr_state_arm.c             "
#include "rsr_state_disarm.c          "
#include "rsr_state_fail.c            "
#include "rsr_state_gwr_on.c          "
#include "rsr_state_ign_on.c          "
#include "rsr_state_run.c             "
#include "rsr_state_start_on.c        "
#include "rsr_state_stop.c            "
#include "rsr_stop.c                  "
#include "rsr_task.c                  "
#include "rsr_timing.c                "
#include "rsr_trunk.c                 "
#include "rsr_unlock.c                "
#include "rsr_valet.c                 "
#include "rsr_vars.c                  "
//--------------------------------------------------------------------------//


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
                                   
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

