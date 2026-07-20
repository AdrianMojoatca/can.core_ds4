/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// 
/*==========================================================================*/

#ifndef __DBGCLI_H__
#define __DBGCLI_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define TRACE_RX()              dbg_rx()
#define TRACE_ZONE( x )    do { x; }while (0);
#define TRACE(args...)      		dbg_print( args )

#define 	printf		dbgprintf
#define DBGTX_FIFO_SIZE			(1<<10)			//1024 byte size fifo

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
extern UInt32 dbgclitx_avail(void);
extern UInt32 dbgclitx_room(void);
Boolean dbg_print( const char *str, ... );
UInt8 dbg_rx (void);
UInt8 get_dbgtx_fifo(void);
void dbgcli_handler(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
#define DBGCLI_BUF_SIZE    128
extern UInt8 	dbg_rx_buf[DBGCLI_BUF_SIZE];
extern UInt8 	dbg_tx_buf[DBGCLI_BUF_SIZE];
extern UInt8  dbg_rx_nw, dbg_rx_nr;	
extern UInt8  dbg_tx_ofs;
extern UInt8  dbg_dest;	
extern UInt8  dbg_tx_max_wait; 
extern UInt8  dbg_uart_avail;

extern UInt8 is_validation;

extern UInt8 *dbgtx_fifo;

extern UInt16  dbgtx_nr,dbgtx_nw;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/



/*==========================================================================*/

#endif

