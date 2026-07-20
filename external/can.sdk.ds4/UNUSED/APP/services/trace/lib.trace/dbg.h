/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dbg.h 28257 2014-10-31 13:59:42Z martin.bouchard $
/*==========================================================================*/
#ifndef __DBG_H__
#define __DBG_H__
/*==========================================================================*/
/*                                N O T E S                                 */
/*==========================================================================*/

/*==========================================================================*/
#include "device.h"
#include "trace.h"

void dbg_tx_text(StrConst * text);
void dbg_tx_u(UInt32 data, UInt8 digits);
void dbg_tx(int data);
void dbg_tx_u16(UInt16 data);
void dbg_tx_u32(UInt32 data);
void dbg_tx_b8(UInt8 data);
void dbg_tx_h4(UInt8  data);
void dbg_tx_h8(UInt8  data);
void dbg_tx_h16(UInt16 data);
void dbg_tx_h32(UInt32 data);
void dbg_tx_crlf( void );
void dbg_tx_space( void );
void dbg_rx( void );
void dbg_print_mem(UInt8 * mem_addr);
void dbg_print_status( void );
void dbg_tx_release( void );
void dbg_tx_request( void );
void dbg_init(void);

/*==========================================================================*/

#endif

