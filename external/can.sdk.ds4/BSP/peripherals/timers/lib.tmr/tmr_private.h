/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: tmr_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __TMR_PRIVATE_H__
#define __TMR_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "tmr.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define AVAILABLE_TIMER_COUNT		3	// number of hardware timer available for match register use.
#define AVAILABLE_MATCH_REG_COUNT	(AVAILABLE_TIMER_COUNT * 4)

#define TxIR	(*(volatile unsigned long *)(tmr->timer_base_addr + 0x00))
#define TxTC	(*(volatile unsigned long *)(tmr->timer_base_addr + 0x08))
#define TxMCR	(*(volatile unsigned long *)(tmr->timer_base_addr + 0x14))
#define TxMR	(*(volatile unsigned long *)(tmr->timer_base_addr + 0x18 + (tmr->tmr_index * 4)))

struct s_Tmr
{
	UInt32 timer_base_addr ;
	UInt8  tmr_index ;	
} ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern UInt32 timer_base_addr [] ;
extern UInt8  tmr_counter        ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

