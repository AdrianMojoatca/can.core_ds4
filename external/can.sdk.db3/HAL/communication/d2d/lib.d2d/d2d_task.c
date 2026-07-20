/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_task.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2d_private.h"
#include "cmd.h"
#include "bitfield.h"
#include "wdog_v.h"
#include "dbg.h"

#ifndef PLATFORM_CM800
#include "w2w.h"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void d2d_process (D2D_Entry * d2d_entry) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void d2d_task (void * tsk_arg)
{
  #if DBG_D2D == 1
    TRACE("\n[D2D_TASK_START]");
  #endif
  
  for (;;)
    {
      D2D_Entry d2d_entry ;

      d2d_entry.code = get_d2d_cmd (1000) ;  // note: this is a 16-bit assignmwnt

      if(d2d_entry.code != -1)
        {
          wake_enter_flagged(WAKE_D2D_TSK) ;
          
          #if DBG_D2D == 1
            dbg_tx_text("\n\r[D2D_WAKE_ENTER]");
          #endif

            {
              do
                {
                  d2d_process (&d2d_entry) ;
    
                  d2d_entry.code = get_d2d_cmd (D2D_GET_TIMEOUT) ;
                }
              while (d2d_entry.code != -1) ;
            }
          
          #if DBG_D2D == 1
            dbg_tx_text("\n\r[D2D_WAKE_LEAVE]");
          #endif

          wake_leave_flagged(WAKE_D2D_TSK) ;
        }
      else if(d2d_do_err_save)
        {
          wake_enter_flagged(WAKE_D2D_TSK) ;
   
          d2d_err_save() ;

          d2d_do_err_save = FALSE ;
          

          wake_leave_flagged(WAKE_D2D_TSK) ;
        }

    }
}

//--------------------------------------------------------------------------//

static void d2d_process (D2D_Entry * d2d_entry)
{
  #define MASK bitfield_rd(d2d_ena_table.d2d_mask , d2d_entry->code , d2d_entry->code)
  #define ENA  bitfield_rd(d2d_ena_table.d2d_ena  , d2d_entry->code , d2d_entry->code)
  
  d2d_tx_request () ;
    {
			if(d2d_entry->code & 0x100)  // we already echoed this byte?
				d2d_entry->code &= 0xFF; // then don't do it, just clear that flag
			else
        d2d_reply (d2d_entry->code) ; // echo it now.
    }
  d2d_tx_release () ;  

  if(MASK & ENA)  
    {
      d2d_convert (d2d_entry) ;
	
      if(d2d_entry->input_code != INPUT_INVALID)
        {
          	
          #ifndef PLATFORM_CM800
            cmd_q_insert ((Cmd_List) d2d_entry->input_code, D2D_Q_TIMEOUT) ;
            
            
            w2w_dis(d2d_entry->code) ;
          #else
            cmd_q_rs_insert((Cmd_List) d2d_entry->input_code, D2D_Q_TIMEOUT);
          #endif

          #if DBG_D2D == 1
            TRACE("\n[D2D_Rx Code] %X" , d2d_entry->code);
          #endif        
        }
	  else
	    {
	      d2d_lut_service (d2d_entry->code) ;
          
          #if DBG_D2D == 1
            TRACE("\n[D2D_Rx Code] %X" , d2d_entry->code);
          #endif           
		}
    }
  else
    {
      #if DBG_D2D == 1
        TRACE("\n[D2D_Rx Code Skip] %X" , d2d_entry->code);
      #endif     
    }    
  #undef MASK
  #undef ENA
}

//--------------------------------------------------------------------------//

int get_d2d_cmd (TskTimeout timeout)
{
  /*--------------------------------------------------------------------*\
  | Currently, every command is one byte.  Future commands may required  |
  | multi-byte parsing, so are encapsulated here.                        |
  \*--------------------------------------------------------------------*/

  return d2d_rx (timeout) ;
}

