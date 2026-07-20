/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_init_nvfs.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "d2d_private.h"
#include "nvram.h"
#include "nvfs_usr.h"
#include "config_d2d.h"
#include <string.h>
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean d2d_init_table (void) ;
static Boolean d2d_init_error (void) ;
static void    d2d_validate_mask (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean d2d_init_nvfs (void)
{
  d2d_validate_mask () ;    // Validate the D2D mask in NVFS matches the firmware's default one
  
  return(d2d_init_table() & d2d_init_error()) ;
}

//--------------------------------------------------------------------------//

static Boolean d2d_init_table (void)
{

  UInt16 size = 0 ;

  if ((nvfs_rd_size(nvfs_d2d_ena , &size) == NVFS_OK))
    {
      if((size == sizeof(d2d_ena_table)) && (nvfs_rd(nvfs_d2d_ena , &d2d_ena_table) == NVFS_OK))
      {
          #if DBG_D2D == 1
              dbg_tx_text ("\nnvfs_d2d_ena correct read") ;
          #endif
          return (TRUE) ;
      }
    }

  nvfs_delete(nvfs_d2d_ena);

  switch (nvfs_wr_create(nvfs_d2d_ena, &d2d_ena_default , sizeof(d2d_ena_default)))
    {
      case NVFS_OK :  
        {
           #if DBG_D2D == 1
              dbg_tx_text ("\nnvfs_d2d_ena re-created") ;
           #endif
           return d2d_init_table() ;
        }  
    }      

  return FALSE ;
}

//--------------------------------------------------------------------------//

static Boolean d2d_init_error (void)
{
  UInt16 size = 0 ;

  if ((nvfs_rd_size(nvfs_d2d_error , &size) == NVFS_OK))
  {
      if((size == sizeof(d2d_err)) && (nvfs_rd(nvfs_d2d_error , &d2d_err) == NVFS_OK))
      {
          #if DBG_D2D == 1
              dbg_tx_text ("\nnvfs_d2d_error correct read") ;
          #endif
          return (TRUE) ;
      }
  }
  
  nvfs_delete(nvfs_d2d_error);

  memset(&d2d_err , 0 , sizeof (d2d_err)) ; // Make sure the d2d_err RAM variable is cleared
  
  switch (nvfs_wr_create(nvfs_d2d_error, &d2d_err , sizeof(d2d_err)))
    {
      case NVFS_OK :  
        {
          #if DBG_D2D == 1
              dbg_tx_text ("\nnvfs_d2d_error re-created") ;
          #endif
          return d2d_init_error() ;
        }  
    }      
  return FALSE ;
}

//--------------------------------------------------------------------------//

static void d2d_validate_mask (void)
{
  UInt8     index ;
  
  if (nvfs_rd(nvfs_d2d_ena , &d2d_ena_table) == NVFS_OK)
  {
    // Loop through the D2D mask from NVFS and if a discrepancy is found with the firmware's default mask (support bits), reset the D2D NVFS variables
    for (index = 0 ; index < sizeof_array (d2d_ena_table.d2d_mask) ; index++)
    {
      if (d2d_ena_table.d2d_mask[index] ^ d2d_ena_default.d2d_mask[index])
      {
        #if DBG_D2D == 1
          dbg_tx_text ("\n[Found discrepancy in D2D mask for byte ") ;
          dbg_tx_u    (index , 2) ;
          dbg_tx_text (" --> D2D NVFS variables will be reinitialized]\n") ;
        #endif
        
        // Reset the D2D NVFS variables since the mask is different than the default one
        d2d_reset() ;
      
        // Exit
        return ;
      }
    }
  }
  else
  {
    // Reset the (other) D2D NVFS variables since the mask doesn't even exist
    d2d_reset() ;
  }
}

//--------------------------------------------------------------------------//


