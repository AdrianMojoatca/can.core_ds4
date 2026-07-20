/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dbgcli_irq_service_vector.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dbgcli_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void dbg_irq_service (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/// Executes in SUPERVISOR mode ///

void dbg_irq_service (void)
{
  switch (U3IIR & 0x0F)                              // Read UIIR also clears IRQ state
    {
      case 0x04 :                                    // RDA - Receive threshold triggered
      case 0x0C :                                    // CTI - Timeout threshold triggered
        {
          U3IER &= ~0x01 ;                           // Disable RDA & CTI IRQ

          dbg_rda_rx = U3RBR ;                       // Read data to clear IRQ condition

          os_irq_set_i (dbg_irq_rx) ;                // Signal task that data has arrived

          break ;
        }

      case 0x02 :                                    // THRE - Transmitter Transmit Hold Register Empty
        {
          U3IER &= ~0x02 ;                           // Disable THRE IRQ

          os_irq_set_i (dbg_irq_tx) ;                // Signal task that tx fifo is empty

          break ;
        }
    }
}

