/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: key_trace.c 16047 2012-07-19 19:41:50Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "key_private.h"

#include "dbg.h" 
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define READER 2
#define TAG    1
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void dbg_print_key(void) ;
                                
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void key_trace (void)           
{ 
  UInt8 frames = sizeof_array(TRACE);

  UInt8 t0 , i , j , length_bytes , length_bits , response_length;

  UInt8 frame_byte, frame[8];
  
  UInt64 frame_data = 0 ;

  int fnr;

  memcpy(key_memory , KEY , sizeof(KEY)) ;  

  dbg_tx_text   ("\r\nKey Content Before\r\n") ;
  dbg_print_key ()                 ;
    
  for( fnr = 0 ; fnr < frames ; fnr++)
    {
      static Ht2_Trans_Resp resp ;

      length_bits = TRACE[fnr].nbit;

      length_bytes = (length_bits + 7) / 8 ;
      
      frame_data = TRACE[fnr].data ;      
 
      for(i = 0 ; i < length_bytes ; i++)
        {
            frame_byte = (frame_data >> (length_bytes-1-i)*8) & 0xFF;

            frame[i] = frame_byte;
        }  
 
          if(TRACE[fnr].src == READER) // reader sends
            {
              UInt8 i ;

              dbg_tx_text("\r\nReader   : ") ;
              dbg_tx_u(length_bits ,2) ;
              dbg_tx_space() ;

              for(i = 0 ; i < length_bytes ; i++)
                {
                  dbg_tx_h8(frame[i]) ;
                }
              
              resp = ht2_command(&frame[0] , length_bits) ;
              
              if(resp.nbit > 0)
                {
                  dbg_tx_text("\r\nResponse : ") ;
                  dbg_tx_u(resp.nbit ,2) ;
                  dbg_tx_space() ;

                  for(i = 0 ; i < ((resp.nbit + 7) / 8) ; i++)
                    {  
                      dbg_tx_h8(resp.data[i]) ;
                    }
                }
            }
          else if(TRACE[fnr].src == TAG)// tag responds 
            {             
              UInt8 i ;

              if(resp.nbit > 0)
                {
                  dbg_tx_text ("\r\nCheck    : ") ;
                  dbg_tx_u    (length_bits, 2)       ;
            
                  dbg_tx_space() ;
    
                  for(i = 0 ; i < length_bytes ; i++)
                    {
                      dbg_tx_h8(frame[i]) ;
                    }
                  dbg_tx_crlf();
                }

              ht2_trans(&frame[0] , length_bits) ;
            }
    }

  dbg_tx_text   ("\r\nKey_Content After\r\n") ;
  dbg_print_key ()                ;



}
//--------------------------------------------------------------------------//

static void dbg_print_key(void)
{
  UInt8 mem_i ;

  for(mem_i = 0 ; mem_i < sizeof_array(KEY) ; mem_i++)
    {
      ht2_mem_dbg_print(&key_memory[mem_i]) ;
    }   

}






