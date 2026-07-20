/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_tx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "d2d_private.h"
#include "bitfield.h"

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
void    d2d_tx_frame_set_func (D2D_Tx_Frame_Func func)
{
  ATOMIC(d2d_func.tx_frame =  func) ;
  
  #if DBG_D2D == 1
    TRACE("\n[D2D Set Tx Frame Func]");
  #endif  
}

//--------------------------------------------------------------------------//
void d2d_tx (UInt8 code)
{
  if(d2d_func.tx)  
    {
      d2d_func.tx(code) ;
    }
  else
    {
      #if DBG_D2D == 1
        TRACE("\nD2D No TX") ;
      #endif     
    }     
}
//--------------------------------------------------------------------------//
void d2d_tx_code(UInt8 code)
{
  #define MASK bitfield_rd(d2d_ena_table.d2d_mask , code , code)
  #define ENA  bitfield_rd(d2d_ena_table.d2d_ena  , code , code)
  
  if(MASK & ENA) 
    {
      d2d_tx_request () ;
        {
          d2d_reply (code) ;
        }
      d2d_tx_release () ;
      
      #if DBG_D2D == 1
        TRACE("\nD2D Tx Code: %X" , code) ;
      #endif       
    }
  else
    {
      #if DBG_D2D == 1
        TRACE("\nD2D Tx Code is skip: %X" , code) ;
      #endif     
    }     
    
  #undef MASK
  #undef ENA
}
//--------------------------------------------------------------------------//
void    d2d_tx_set_func (D2D_Tx_Func func)
{
  ATOMIC(d2d_func.tx =  func) ;

  #if DBG_D2D == 1
    TRACE("\n[D2D Set Tx Func]");
  #endif  
}
//--------------------------------------------------------------------------//
void d2d_tx_frame (UInt8 * data ,UInt8 size)
{
  if(d2d_func.tx_frame)  
    {
      d2d_func.tx_frame(data,size) ;
    }
  else
    {
      #if DBG_D2D == 1
        TRACE("\nD2D No TX Frame") ;
      #endif     
    }    
}
//--------------------------------------------------------------------------//
void d2d_tx_frame_data (UInt8 * data ,UInt8 size)
{
  UInt8 code = * data ;
    
  #define MASK bitfield_rd(d2d_ena_table.d2d_mask , code , code)
  #define ENA  bitfield_rd(d2d_ena_table.d2d_ena  , code , code)
  
  if(MASK & ENA) 
    {
      d2d_tx_request () ;
        {
          while(size-- != 0)
            {
              d2d_reply (*data++) ;
            }
    	  }
      d2d_tx_release () ;
      
      #if DBG_D2D == 1
        TRACE("\nD2D Tx Frame Code: %X" , code) ;
      #endif       
      
    }
  else
    {
      #if DBG_D2D == 1
        TRACE("\nD2D Tx Frame Code is skip: %X" , code) ;
      #endif     
    }    

  #undef MASK
  #undef ENA
}
//--------------------------------------------------------------------------//

void d2d_tx_request (void)
{
	os_res_request (d2d_tx_res,0) ;
}

//--------------------------------------------------------------------------//

void d2d_tx_release (void)
{
	os_res_release (d2d_tx_res) ;
}



