/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_dbg.h 33426 2015-08-28 11:08:28Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_DBG_H__
#define __CONFIG_DBG_H__


#if DBG_JTAG == 1
  #warning "DEBUG is enable Please keep this #WARNING"
#endif

//--------------------------------------------------------------------------//
// <e0>  DEBUG_TEXT Enable
#define DEBUG_TEXT              1          // 

#define USE_TRACE             DEBUG_TEXT  //
//--------------------------------------------------------------------------//
// <e1> PRINT MEM/STATUS DEBUG
// </e>
#if DEBUG_TEXT == 1
    #define DBG_PRINT_MEM       0   //
#endif

//--------------------------------------------------------------------------//
// <h> TEMPLATE DEBUG
// <q1> lib.detect
// <q2> lib.sense
// <q3> lib.function
// </h>
#if DEBUG_TEXT == 1
    #define DBG_DETECT          1   //
    #define DBG_SENSE           1   //
    #define DBG_FUNCTION        1   //
#endif
//--------------------------------------------------------------------------//
// <h> GENERIC DEBUG
// <q1> lib.cmd
// <q2> lib.ctl_dl
// <q3> lib.d2d
// <q4> lib.eips
// <q5> lib.obd_can
// <q6> lib.prog
// <q7> lib.reset
// <q8> lib.rsr_feature
// <q9> lib.rsr
// <q10> lib.rxt
// <q11> lib.secure_lock
// <q12> lib.status_req
// <q13> lib.vin
// <q14> lib.w2w
// <q15> lib.wdog_v
// <q16> lib.wake
// <q17> lib.gpio/gpio_wake
// </h>
#if DEBUG_TEXT == 1
    #define DBG_CMD             0   //
    #define DBG_CTL_DL          0   //
    #define DBG_D2D             0   //
    #define DBG_EIPS            0   //
    #define DBG_OBD_CAN         0   //
    #define DBG_PROG            0   //
    #define DBG_RESET           0   //
    #define DBG_RSR_FEAT        0   //
    #define DBG_RSR             0   //
    #define DBG_RXT             0   //
    #define DBG_SECURE_LOCK     0   //
    #define DBG_STS_R           0   //
    #define DBG_VIN             0   //
    #define DBG_W2W             0   //
    #define DBG_WDOG_V          0   //
    #define DBG_WAKE            0
    #define DBG_GPIO            0
    #define DBG_PTS_CTL         0

#endif
//--------------------------------------------------------------------------//
// </e>


#endif


