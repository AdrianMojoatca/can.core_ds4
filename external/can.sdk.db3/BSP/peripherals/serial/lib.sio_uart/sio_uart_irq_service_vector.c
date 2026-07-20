/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sio_uart_irq_service_vector.c 32991 2015-07-28 15:42:19Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sio_uart_private.h"

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

//--------------------------------------------------------------------------//
//                         D E S C R I P T I O N                            //
//                                                                          //
// Each function below acts as the low-level entry point for its respective //
// UART interrupt.  When the VIC detects a UART IRQ request, it sets the    //
// PC to point to the respective function below.                            //
//                                                                          //
// Upon entry to each funtion, the LR is saved, the OS notified, and then   //
// the address of the SIO_UART object associated is determined and passed   //
// to the central interrupt handler.                                        //
//                                                                          //
// The SIO_UART object address is calculated by first loading the corect    //
// position in the SIO_UART object table and then reading that position     //
// to get the objects address.                                              //
//                                                                          //
// The calling convention for this compiler is to pass the address in R0,   //
// so thats where we load it before calling 'sio_uart_irq_service'.         //
//                                                                          //
// The 'sio_uart_irq_service' function will service the respective UART     //
// IRQ source and return, wherein we notify the OS that we are done.        //
//                                                                          //
//--------------------------------------------------------------------------//

void sio_uart_irq_service_vector_0 (void)
{
  sio_uart_irq_service( sio_uart_table[ 0 ]);
}

//--------------------------------------------------------------------------//

void sio_uart_irq_service_vector_1 (void)
{
	sio_uart_irq_service(sio_uart_table[ 1 ]);

}

//--------------------------------------------------------------------------//

void sio_uart_irq_service_vector_2 (void)
{

  sio_uart_irq_service(sio_uart_table[ 2 ]);

}

//--------------------------------------------------------------------------//

void sio_uart_irq_service_vector_3 (void)
{
	sio_uart_irq_service(sio_uart_table[ 3 ]);

}
