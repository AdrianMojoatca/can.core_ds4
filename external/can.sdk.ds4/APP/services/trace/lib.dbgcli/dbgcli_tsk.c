/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dbgcli_tsk.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dbgcli_private.h"
#include "wake.h"

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


void dbg_tsk (void)
{
    Dbg_Q_Msg *dbg_q_msg ; 

    Boolean wake = FALSE ; 

  for(;;)
  {
    dbg_q_msg = (Dbg_Q_Msg *)q_remove( dbg_vars.q_pending, 0 );

    if(dbg_dest == 0)
    {    
      if(dbg_uart_avail)
      {
        os_res_request(dbg_str_res, 0);
          wake_ck_enter();
          {
            while(dbgclitx_avail())
                dbgcli_tx(get_dbgtx_fifo()); // write 1 char to uart
          }
          wake_ck_leave(); 
        os_res_release(dbg_str_res);
      }
      else
        dbgtx_nr = dbgtx_nw;  //skip everything in dbgtx_fifo
    }
      
    q_return( (Q_Msg *) dbg_q_msg);
    // we saw an empty fifo, so for sure another semaphore kick will come when next char arrives
  }
}


//--------------------------------------------------------------------------//


UInt32 dbgclitx_avail(void)
{
  SInt32 amt;
	
  amt = (dbgtx_nw - dbgtx_nr) & (DBGTX_FIFO_SIZE-1);
  
  return (UInt32) amt;
}


UInt32 dbgclitx_room(void)
{
  return (DBGTX_FIFO_SIZE-1 - dbgclitx_avail());
}

UInt8 get_dbgtx_fifo(void)
{
  UInt8 b;
    b = dbgtx_fifo[dbgtx_nr & (DBGTX_FIFO_SIZE-1)];
    ++dbgtx_nr;

    return b;
}

//--------------------------------------------------------------------------//
