/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_req_init_nvfs.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_req_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/


static Boolean status_req_init_error (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean status_req_init_nvfs (void)
{
	return (status_req_init_error()) ;
}

//--------------------------------------------------------------------------//

static Boolean status_req_init_error (void)
{
  static Status_Req_Err curr_err ;
  
	UInt16 size = 0 ;

	if ((nvfs_rd_size(nvfs_status_req_err , &size) == NVFS_OK))
	{
	  if((size == sizeof(curr_err)) && (nvfs_rd(nvfs_status_req_err , &status_req_err) == NVFS_OK))
	  {
		  #if DBG_STS_R == 1
			dbg_tx_text ("\n nvfs_status_req_err is up to date") ;
		  #endif
		  return (TRUE) ;
	  }
	}

  nvfs_delete(nvfs_status_req_err);

  switch (nvfs_wr_create(nvfs_status_req_err, &curr_err , sizeof(Status_Req_Err)))
    {
      case NVFS_OK :  
        {
		  #if DBG_STS_R == 1
			dbg_tx_text ("\n nvfs_status_req_err re-created") ;
		  #endif
          return status_req_init_error() ;
        }  
    }      
  return FALSE ;  
}

//--------------------------------------------------------------------------//


