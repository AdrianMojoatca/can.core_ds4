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
//#define APP_VERSION "1.00_01" //  06_05_2026 - first test
//#define APP_VERSION "1.00_02" //  07_05_2026 - second test
//#define APP_VERSION "1.00_03" //  07_05_2026 - scos tester_lock_unlock (sa fie ca frd37 DB2 de pe site)
//#define APP_VERSION "1.00_04" //  11_05_2026
//#define APP_VERSION "1.00_05" //  12_05_2026
//#define APP_VERSION "1.00_06" //  13_05_2026
//#define APP_VERSION "1.00_07" //  13_05_2026
//#define APP_VERSION "1.00_08" //  14_05_2026 - w2w_execute.c - modificat gwr cu INPUT_GWR_ON in loc de W2W_NEG7_CMD
//#define APP_VERSION "1.00_09" //  14_05_2026 - w2w_execute.c - modificat gwr cu INPUT_GWR_ON in loc de W2W_NEG7_CMD
//#define APP_VERSION "1.00_10" //  20_05_2026 - cu noul lib de gpio_iface
//#define APP_VERSION "1.00_11" //  20_05_2026 - cu noul lib de gpio_iface completat cu datele disponibile
#define APP_VERSION "1.00_12" //  15_06_2026 - cu noul lib de gpio_iface completat cu datele disponibile si noul wrapper (doar pt. config firmware)

#define FW_INFO     ""
																	
//--------------------------------------------------------------------------//
#if defined PLATFORM_CM800 || defined PLATFORM_933

#define DEI_SS_IGNITION_CHECKING_TIMEOUT  7000

#endif

#endif

