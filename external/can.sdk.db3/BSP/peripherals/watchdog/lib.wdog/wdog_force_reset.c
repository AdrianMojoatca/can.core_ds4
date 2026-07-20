/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_force_reset.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "wdog_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void wdog_force_reset (const UInt8 reset_code)
{
#ifdef DEV_LPC_176x
	IRQ_DIS_GLOBAL_ALL;
#else
  IRQ_DIS_GLOBAL ;

  __disable_fiq();
#endif
  /*-------------------------------------------------------------------*\
  | First we must write the reset code twice in order to force the MAM  |
  | to physically write the data to RAM.  If we don't, the value of     |
  | the reset code will not reflect this last write.  Note that the     |
  | declaration of WDOG_RESET_CODE is 'volatile' in order to ensure     |
  | that the compiler generates two consecutive write sequences.        |
  \*-------------------------------------------------------------------*/

  WDOG_RESET_CODE = reset_code ;
  WDOG_RESET_CODE = reset_code ;

  /*-------------------------------------------------------------------*\
  | Second, we force the WDOG to be enabled whether it already has been |
  | or not and generate a valid feed sequence to get it started.        |
  \*-------------------------------------------------------------------*/

  WDMOD |= 0x03 ; // Force WDOG enable

  WDFEED = 0xAA ; // First  part of feed is correct
  WDFEED = 0x55 ; // Second part of feed is correct - Wdog started

  /*-------------------------------------------------------------------*\
  | Third, we generate and illegal feed sequence which will cause an    |
  | immediate CPU reset.  The feed sequence must start with 0xAA and be |
  | followed by a value not equel to 0x55. (user man chap. 25 sec 4.3). |
  \*-------------------------------------------------------------------*/

  WDFEED = 0xAA ; // First  part of feed is correct
  WDFEED = 0x00 ; // Second part of feed is wrong   - Wdog reset triggered

  ///     !!! Bye-Bye !!!     ///
}

