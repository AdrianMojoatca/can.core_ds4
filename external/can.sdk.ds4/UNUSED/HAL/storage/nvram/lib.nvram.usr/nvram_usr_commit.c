/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_usr_commit.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvram_usr_private.h"

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

//******************************************************//
// WRITING TO EEPROM (FLASH)
// with disabling of CAN to be sure not sitcking in OVERRUN error.
//******************************************************//

Boolean nvram_usr_commit (void)
{
	Boolean status ;

	ram_dummy_write = 1 ;	// write to ram pass through cache in CPU. if write is done to ram, last cache value 
	ram_dummy_write = 1 ;	// will be written in ram and will be replaced by the new one in cache.
							// to be sure the last value we really wanted to be written is written, we do some
							// dummy write to allow CPU to write value from cache to ram.

	status = nvram_cache_commit () ;	// nvram_cache_commit is long to execute which can create overrun errors on CAN

	return status ;
}

