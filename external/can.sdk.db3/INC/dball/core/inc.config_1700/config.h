/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config.h 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_H_1700__
#define __CONFIG_H_1700__

//#include "config_dball.h"

/*==========================================================================*/
/*                                N O T E S                                 */
/*==========================================================================*/
/* This file is included by 'device.h' and represents global compile-time   */
/* configuration parameters.  These parameters can be edited through the    */
/* configuration tool in the RealView IDE.                                  */
/*==========================================================================*/

#define FW_VERSION      APP_VERSION

#define DEV_TYPE                    0  // LPC1765 = 0 , DEV_LPC1766 = 1

#define XTAL_OSC_HZ          12000000  // Frequency of external crystal in Hz

#define CPU_CLOCK_HZ         72000000  // CPU Clock Speed in Hz

#if DEV_TYPE != 0 && DEV_TYPE != 1
#error !!! Invalid device type specified
#endif

#if DEV_TYPE == 0
  #define DEV_LPC1765
#endif

#if DEV_TYPE == 1
  #define DEV_LPC1766
#endif

//--------------------------------------------------------------------------//

// <h> OS Configuration
// <o0> OS Stack Size (bytes) <0x0000-0xFFFF:8>
// <i>  Defines the size of the stack used by the OS during
// <i>  application calls. Must be a multiple of 8.
// <o1> OS Region <1-4>
// <e2> Enable OS context hooks feature
// </e>
// <i>  Enables or Disables user context switch hooks.  These are hooks
// <i>  provided by the OS to allow user code to augment context management
// <i>  through user supplied hook functions.  Disable when not required. 
// <i>  Will reduce code/ram usage and context switch time.
// <e3> Enable context switch debug code
// </e>
// <i>  Used for testing implementation of OS context switching.
// <i>  Fills R1 to R12 CPU registers with test values on task start (R1=0x11111111,R2=0x22222222, etc.)
// <i>  Disable when testing is done.  Will reduce code space and task start time.
// <e4> Stop CPU clock when CPU is idle (affects JTAG).
// </e>
// <i>  Checked  : CPU clock stopped when all tasks are blocked (saves power, disables JTAG).
// <i>  Unchecked: CPU executes a NOP loop when all tasks are blocked (increases power, enables JTAG).
// <i>  When debugging with JTAG tool, uncheck.
// </h>

#define OS_STK_SIZE            0x0100 // For OS and IRQs that call OS functions

#define OS_HEAP_SIZE      0x000004000 // For all OS dynamic memory allocation

#define OS_NUM_HEAP_REGIONS         3

#define OS_USE_HOOK_CONTEXT         0 // 0 = Do not use hooks , 1 = Use hooks

#define OS_TASK_START_REG_FILL      0 // On task start, fill registors will debug values

#define OS_STOP_CPU_ON_IDLE         1 // 0 = Do not stop CPU clock, 1 = Stop CPU clock

#define OS_USE_IDLE_STACK           1 // Use a separate stack when in IDLE state (mandatory for Cortex-M3)

#define OS_IRQ_PRIORITY            31 // !!! DO NOT CHANGE !!!


// <h>  NVRAM Library Configuration
// <o0> Base address of FLASH used 
// <i>  Defines the starting point of FLASH memory
// <i>  that will be used to mimic NVRAM.
// <o1> Size of FLASH block.
// <i>  The size in bytes of the FLASH block
// <i>  used to mimic NVRAM.
// </h>

#define NVRAM_FLASH_BASE_ADDR 0x00005000
#define NVRAM_FLASH_SIZE      0x00001000

//--------------------------------------------------------------------------//

// <h>  NVFS Library Configuration
// <o0> Base address of NVRAM used
// <i>  Defines the starting point of logical
// <i>  NVRAM memory used by file system.
// <i>
// <i>  Must not be larger than actual NVRAM memory.
// <o1> Size of file system
// <i>  The size in bytes of the file system.
// <i>
// <i>  Make sure that the base address plus this
// <i>  size does not go outside of the logical
// <i>  NVRAM limits.
// <o2> Used OS     <0=> No    <1=> Yes
// <o3> Used Table  <0=> No    <1=> Yes
// <i>  TABLE driven
// </h>
#define NVFS_BASE               0x0000
#define NVFS_SIZE               0x1000
#define NVFS_USE_OS             0x0001  
#define CRC_DOW_8_USE_TABLE     0x0001 

//--------------------------------------------------------------------------//

#define TIMEOUT_IED_GWR     1
#define TIMEOUT_IED_IGN     1
#define TIMEOUT_IED_START   1
#define TIMEOUT_IED_POS_IN1 1
#define TIMEOUT_IED_LOCK    3
#define TIMEOUT_IED_UNLOCK  3
#define TIMEOUT_IED_TRUNK   3
#define TIMEOUT_IED_AUX1    3
#define TIMEOUT_IED_AUX2    3
#define TIMEOUT_IED_AUX3    3          
#define TIMEOUT_IED_PUSH    1

//--------------------------------------------------------------------------//

#endif

