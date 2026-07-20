/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_irq_service_vector.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

       void d2d_irq_service    (void) ;
static void rls_service        (void) ;
static void rda_service        (void) ;
static void cti_service        (void) ;
static void thre_service       (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void d2d_insert_d2d_code(UInt8 code)
{
  if(tsk_d2d)
    {
      d2d_rda_rx = code;
      os_irq_set (d2d_irq_rx) ;                /* Signal task that data has arrived*/    
    }
  else
    {
      #if DBG_D2D == 1
        TRACE("\n[D2D NO Insert]");
      #endif      
    }  
    

}
//--------------------------------------------------------------------------//
/// Executes in SUPERVISOR mode ///

void d2d_irq_service (void)
{
  UInt8  iir_value    ;

  iir_value   = D2DIIR ;    
  iir_value >>= 1     ;			                 /* skip pending bit in IIR                 */
  iir_value  &= 0x07  ;			                 /* check bit 1~3, interrupt identification */
 
  switch (iir_value)
    {
      case IIR_RLS  : rls_service  () ; break ;  /* Receive Line Status                     */
      case IIR_RDA  : rda_service  () ; break ;	 /* Receive Data Available                  */
      case IIR_CTI  : cti_service  () ; break ;  /* Character timeout indicator             */
      case IIR_THRE : thre_service () ; break ;  /* THRE, transmit holding register empty   */
    }
}

//--------------------------------------------------------------------------//
static void rls_service  (void) 
{
  UInt8  lsr_value = D2DLSR   ; 

  if ( lsr_value & LSR_RDR )	/* Receive Data Ready */			
    {
   	  /* If no error on RLS, normal ready, save into the data buffer. */
   	  /* Note: read RBR will clear the interrupt */
   	  d2d_rda_rx = D2DRBR;
	  os_irq_set_i (d2d_irq_rx) ;                /* Signal task that data has arrived*/
    }
  else if ( lsr_value & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
    {
   	  /* There are errors or break interrupt */
   	  /* Read LSR will clear the interrupt */
   	  UInt8  dummy  = D2DRBR      ;		/* Dummy read on RX to clear */
						                /*interrupt, then bail out */
//	  error_write(ERR_D2D_FRAMING,TRUE,0) ;
    }
}
//--------------------------------------------------------------------------//
static void rda_service  (void)                 /* Receive Data Available */
{
  UInt8  lsr_value = D2DLSR   ;

  if ( lsr_value & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
	{     
      /* There are errors or break interrupt */
      /* Read LSR will clear the interrupt */
      static UInt8  log_once     ;
      UInt8  dummy  = D2DRBR      ;             /* Dummy read on RX to clear */
						                       /*interrupt, then bail out   */

	  if(lsr_value & LSR_OE)				   /* Overrun error             */
	    {
          d2d_err_add(D2D_ERR_OVERRUN) ;
	    }
	  if(lsr_value & LSR_PE)				   /* Parity  error             */
	    {
	      d2d_err_add(D2D_ERR_PARITY) ;
	    }
	  if(lsr_value & LSR_FE )				   /* Framing error             */
	    {
	      if(!(log_once & (1u << D2D_ERR_FRAMING)))
            {
              log_once |= 1u << D2D_ERR_FRAMING ;
              d2d_err_add(D2D_ERR_FRAMING) ;
            }
	    }
	  if(lsr_value & LSR_RXFE)				  /* Rx Fifo error              */
	    {
	       if(!(log_once & (1u << D2D_ERR_RXFE)))
            {
              log_once |= 1u << D2D_ERR_RXFE ;
              d2d_err_add(D2D_ERR_RXFE) ;
            }
	    }
	  if(lsr_value & LSR_BI)				  /* Break interupt error       */
	    {
	       if(!(log_once & (1u << D2D_ERR_BREAK)))
            {
               log_once |= 1u << D2D_ERR_BREAK ;
              d2d_err_add(D2D_ERR_BREAK) ;
            }
	    }
	}
  else
  {
    d2d_rda_rx = D2DRBR;
    os_irq_set_i (d2d_irq_rx) ;                /* Signal task that data has arrived*/
  }
}
//--------------------------------------------------------------------------//
static void cti_service  (void)    /* Character Time-out indicator */
{
  UInt8  lsr_value = D2DLSR   ;
  /*UART0Status |= 0x100; */		/* Bit 9 as the CTI error */
}
//--------------------------------------------------------------------------//
static void thre_service (void)    /* THRE interrupt */
{
  UInt8  lsr_value = D2DLSR   ;     /* Check status in the LSR to see if */
								   /* valid data in U0THR or not */ 
  if ( lsr_value & LSR_THRE )
    {
   	  os_irq_set_i (d2d_irq_tx) ;  /* Signal task that tx fifo is empty*/
    }
  else
    {
   	  ;                            /* tx fifo not empty  */
    }
}

//--------------------------------------------------------------------------//


