/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_firmware.h 35235 2015-11-12 22:44:01Z huiyi.wang $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_FIRMWARE_H__
#define __CONFIG_FIRMWARE_H__

//#include "vers.out"
 
#define CORE_CONFIG_FW_VERSION_1_00

/*==========================================================================*/
/*                                N O T E S                                 */
/*==========================================================================*/
/* // Include any configuration specific to ALL Platform                    */
/*==========================================================================*/
//--------------------------------------------------------------------------//
// <h>  Firmware information
// <s0.8> Name
// <s1.4> Version
// <s2.255> Additional Information
// </h>

#define FW_NAME     "FRD37EU"

//#define APP_VERSION "1.00_00" //  06_10_2025 - primam versiune
//#define APP_VERSION "1.00_04" //  08_05_2026 - func_rs_call_back() in loc (sau dupa) tsk_rs_pts_ctrl()
#define APP_VERSION "1.00_05" //  03_08_2026 - Claude

#define FW_INFO     ""

//--------------------------------------------------------------------------//
#if defined PLATFORM_CM800 || defined PLATFORM_933

#define DEI_SS_IGNITION_CHECKING_TIMEOUT  7000

#endif

#endif

