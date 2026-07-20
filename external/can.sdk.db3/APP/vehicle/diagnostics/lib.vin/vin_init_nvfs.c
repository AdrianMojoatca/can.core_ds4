/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_init_nvfs.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "vin_private.h"
#include "nvram.h"
#include "nvfs_usr.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean vin_init_data (void) ;
static Boolean vin_init_error (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
Boolean vin_init_nvfs (void)
{
  return vin_init_data() & vin_init_error() ;
}

//--------------------------------------------------------------------------//

static Boolean vin_init_data (void)
{
  static Vin_Info cur_info ;

	UInt16 size = 0 ;

  if ((nvfs_rd_size(nvfs_vin , &size) == NVFS_OK))
	{
	  if((size == sizeof(cur_info.vin.code)) && (nvfs_rd(nvfs_vin , &vin_info.vin.code) == NVFS_OK))
	  {
		  #if DBG_VIN == 1
			dbg_tx_text ("\nVin nvfs is up to date") ;
		  #endif
		  return (TRUE) ;
	  }
	}

  nvfs_delete(nvfs_vin);

  switch (nvfs_wr_create(nvfs_vin, &cur_info.vin.code , sizeof(cur_info.vin.code)))
    {
      case NVFS_OK :  
        {
		  #if DBG_VIN == 1
			dbg_tx_text ("\nVin nvfs re-created") ;
	      #endif
          return vin_init_data() ;
        }  
    }      

  return FALSE ;
}

//--------------------------------------------------------------------------//

static Boolean vin_init_error (void)
{
  static Vin_Error cur_err ;
  
  UInt16 size = 0 ;

	if ((nvfs_rd_size(nvfs_vin_error , &size) == NVFS_OK))
	{
	  if((size == sizeof(cur_err)) && (nvfs_rd(nvfs_vin_error , &vin_err) == NVFS_OK))
	  {
		  #if DBG_VIN == 1
			dbg_tx_text ("\nVin nvfs error is up to date") ;
		  #endif
		  return (TRUE) ;
	  }
	}

  nvfs_delete(nvfs_vin_error);


  switch (nvfs_wr_create(nvfs_vin_error, &cur_err , sizeof(Vin_Error)))
    {
      case NVFS_OK :  
        {
		  #if DBG_VIN == 1
			dbg_tx_text ("\nVin nvfs error re-created") ;
		  #endif
          return vin_init_error() ;
        }  
    }      
  return FALSE ;
}


