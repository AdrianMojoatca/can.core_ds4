/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __WDOG_H__
#define __WDOG_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//------------------------------------------------------------------------//
// Bootload Watchdog Reset Code Addr
extern  unsigned Image$$ER_RAM_BOOT_WDOG_RESET_ID$$Base ;
#define WDOG_RESET_CODE_ADDR ((unsigned) (& Image$$ER_RAM_BOOT_WDOG_RESET_ID$$Base))   

//--------------------------------------------------------------------------//

enum e_Wdog_Status
{
  WDOG_RESET_POWERUP = -1 ,  // Reset due to power-up or hardware reset
  WDOG_RESET_TIMEOUT = -2 ,  //  "     "  "  watchdog timeout

  WDOG_RESET_USER    =  0    // Forced-reset user values start from here (0 to 255)
} ;

//--------------------------------------------------------------------------//

typedef enum e_Wdog_Clk_Mode // !!! Hardware Dependant Values !!!
{
  WDOG_CLK_MODE_RC   = 0x00 ,
  WDOG_CLK_MODE_RTC  = 0x02 ,
  WDOG_CLK_MODE_PCLK = 0x01

} Wdog_Clk_Mode ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

EXTERN_C_ENTER

SInt16 wdog_init        (const Wdog_Clk_Mode clk_mode , UInt32 timeout)  ; // Returns e_Wdog_Status

void   wdog_kick        (void) ;

void   wdog_enable      (void) ;

void   wdog_force_reset (const UInt8 reset_code) ;

EXTERN_C_LEAVE

/*==========================================================================*/

#endif

