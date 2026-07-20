/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_rx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "d2d_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void d2d_rx_timeout_service (void * io_base) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

int d2d_rx (TskTimeout timeout)
{
	SInt16 c;
	
  CRITICAL ;
  
  if(tsk_d2d)
  {
      Uart_IO_Base io_base = UART0_BASE ;   //BUG! should be whichever uarr this is
			

			if(d2d_echo_idx == d2d_echo_cnt && d2d_rda_rx == (-1))
			{
					if ((D2DLSR & 0x01))
					{
						  return ((UInt8) D2DRBR) ;
					}
					
					{
						os_timeout_arm_set (d2d_rx_timeout_service , (void *) io_base ) ;
						{
								C_ENTER ;
									{
										D2DIER |= 0x01 ;                         // Enable RDA interrupt

										os_irq_wait (d2d_irq_rx,FALSE,timeout) ; // Wait until data arrives in UART
										
									}
								C_LEAVE ;
						}
						os_timeout_arm_clr () ;
					}
				}
					
				if(d2d_echo_idx < d2d_echo_cnt) // this could happen during the os_irq_wait
				{
					return 0x100 + d2d_echo_buf[d2d_echo_idx++];  // return next byte which was already echoed
				}
				if(d2d_rda_rx != (-1))
				{
					c = d2d_rda_rx;
					d2d_rda_rx = (-1);  // this is the last byte received
          return(c)    ;
        }

       return (-1) ;
    
  }
  else
  {
      d2d_rda_rx = (-1) ;
      
      #if DBG_D2D == 1
        TRACE("\n[D2D NO RX]");
      #endif      
      
      return  d2d_rda_rx;
  }    
}
//--------------------------------------------------------------------------//
static void d2d_rx_timeout_service (void * io_base)
{
  CRITICAL ;

  C_ENTER  ;
    {
      d2d_rda_rx = (-1) ;
	}
  C_LEAVE  ;
}
