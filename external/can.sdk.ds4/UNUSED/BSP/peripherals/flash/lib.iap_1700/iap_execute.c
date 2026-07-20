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
| The hard-coded target address below is specified in the LPC1700 datasheet  |
| and is gauranteed not to change by NXP/PHILIPS.  Since it is only used     |
| here, it does not require a macro to define it.                            |
|                                                                            |
| See the LPC1700 data sheet for details on the IAP application and API.     |
\*--------------------------------------------------------------------------*/

void iap_execute (UInt32 i_args [] , UInt32 o_args [])
{
  ATOMIC_ALL (((void (*)(UInt32 * , UInt32 *)) 0x1FFF1FF1) (i_args , o_args)) ;
}


/**
 *  \brief check if the sector in the address range is blank or not
 *  
 *  \param [in] sector_start start address
 *  \param [in] sector_end end address
 *  \return iap_status_code 
 *  
 *  \details More details
 */
Iap_Status iap_sector_blankcheck (UInt32 sector_start , UInt32 sector_end, UInt32 *first_nblk_loc, UInt32 *first_nblk_val)
{
  UInt32 arg_i [5] , arg_o [5] ;

  arg_i [0] = CMD_BLANK_CHECK  ;
  arg_i [1] = sector_start       ;
  arg_i [2] = sector_end         ;
                           
  iap_execute (arg_i , arg_o) ;
	if((Iap_Status) arg_o [0] == IAP_SECTOR_NOT_BLANK)
	{
		*first_nblk_loc = arg_o[1];
		*first_nblk_val = arg_o[2];
	}

  return (Iap_Status) arg_o [0] ;
}
