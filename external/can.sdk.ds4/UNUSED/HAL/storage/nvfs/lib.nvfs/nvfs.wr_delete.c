/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.wr_delete.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "nvfs_private.h"

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
                                                   
NVFS_Status nvfs_wr_delete (NVFS_Entry_Name const entry_name , const void * const ram_srce , UInt16 entry_size)
{
  NVFS_Status nvfs_status = NVFS_ERR_UNINITIALIZED ;

  #if NVFS_USE_OS == 1
  
  if(os_irq_executing)
  { 
	NVFS_EXCEPTION_REPORT(NVFS_ERR_INVALID_CALL);
  	
  	return NVFS_ERR_INVALID_CALL;
  }
  
  #endif
  
  if (nvfs_vars.is_initialized)
    {
      nvfs_atomic_request () ;
        {
          switch (nvfs_status = nvfs_delete (entry_name))
            {
              case NVFS_OK             :
              case NVFS_NAME_NOT_FOUND :
                {
                  nvfs_status = nvfs_create (entry_name , entry_size) ;
    
                  if (nvfs_status == NVFS_OK)
                    {
                      nvfs_status = nvfs_wr (entry_name , ram_srce) ;
                    }
                }
            }
        }
      nvfs_atomic_release () ;
    }

  NVFS_EXCEPTION_REPORT (nvfs_status) ;

  return nvfs_status ;
}

