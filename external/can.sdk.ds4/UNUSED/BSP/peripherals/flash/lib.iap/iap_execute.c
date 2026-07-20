/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: iap_execute.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "iap_private.h"

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

/*--------------------------------------------------------------------------*\
| This function will perform a call into the on-chip boot-loader to execute  |
| the IAP command previously setup in the 'commands' buffer.  When done,     |
| the status of the command will be present in the 'results' buffer.         |
|                                                                            |
| The hard-coded target address below is specified in the LPC2300 datasheet  |
| and is gauranteed not to change by NXP/PHILIPS.  Since it is only used     |
| here, it does not require a macro to define it.                            |
|                                                                            |
| Note that the target address is an odd one.  This will cause the ARM CPU   |
| to switch to THUMB mode and begin execution of code at 0x7FFFFFFF0.  This  |
| is a standard way to switch to THUMB mode on the ARM7TDMI processors.      |
|                                                                            |
| Upon returning, the CPU mode (ARM or THUMB) will be restored by the IAP    |
| routine.                                                                   |
|                                                                            |
| See the LPC2300 data sheet for details on the IAP application and API.     |
\*--------------------------------------------------------------------------*/

void iap_execute (UInt32 commands [] , UInt32 results [])
{
  ATOMIC_ALL
    (
       ((void (*)(void * , void *)) 0x7FFFFFF1) (commands , results) ;
    )
}

