/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dbgcli_tx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dbgcli_private.h"
#include "wdog_v.h"

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

void dbgcli_tx (int data)
{
  static UInt8 com_tx_count = 0 ;

  dbgcli_tx_request () ;
  	{

	  if (U3LSR & 0x20)                   // Is THRE true ?
	    {                                 
	      goto write_first  ;
	    }
	
	  if (com_tx_count < 16)
	    {
	      com_tx_count += 1 ;
	
	      goto write_append ;
	    }
	
	  wait_thre :
	    {
	      CRITICAL ;
	
	      C_ENTER ;
	        {
	          U3IER |= 0x02 ;             // Enable THRE IRQ
	        }
	      C_LEAVE ;
	      
		  // Place a virtual watch here in case the THR is never freed.
		  // (tx disabled, UART error, too much data written to FIFO, etc)

		  VIRTUAL_WATCH 
		  (
	        os_irq_wait (dbg_irq_tx,0,0) ;
		  );
	    }
	
	  write_first  : com_tx_count = 1     ;
	
	  write_append : U3THR = (UInt8) data ;
	}
  dbgcli_tx_release () ;
}
//--------------------------------------------------------------------------//

int fputc (int ch, FILE *f)
{

  store_char(ch) ;

  return 0 ;
}


//--------------------------------------------------------------------------//

void dbgcli_tx_request (void)
{
	os_res_request (dbg_tx_res,0) ;
}

//--------------------------------------------------------------------------//

void dbgcli_tx_release (void)
{
	os_res_release (dbg_tx_res) ;
}



/***  some new printf - end implementation ******/

void dbg_tx_text(StrConst * text)
{
	TRACE(text);
}

void dbg_tx_u(UInt32 data, UInt8 digits)
{
	TRACE("%d",data);
}

void dbg_tx(int data)
{
	TRACE("%d",data);
}

void dbg_tx_u16(UInt16 data)
{
	TRACE("%d",data);
}

void dbg_tx_u32(UInt32 data)
{
	TRACE("%d",data);
}

void dbg_tx_b8(UInt8 data)
{
	TRACE("%d",data);
}

void dbg_tx_h4(UInt8  data)
{
	TRACE("%01X",data);
}

void dbg_tx_h8(UInt8  data)
{
	TRACE("%02X",data);
}

void dbg_tx_h16(UInt16 data)
{
	TRACE("%04X",data);
}

void dbg_tx_h32(UInt32 data)
{
	TRACE("%08X",data);
}

void dbg_tx_crlf( void )
{
	TRACE("\n\r");
}

void dbg_tx_space( void )
{
	TRACE(" ");
}


