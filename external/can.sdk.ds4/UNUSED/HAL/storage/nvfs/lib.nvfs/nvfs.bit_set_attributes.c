/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.bit_set_attributes.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

NVFS_Status nvfs_bit_set_attributes (NVFS_Entry_Name const entry_name , NVFS_Attributes attributes_flags)
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
          NVFS_Attributes attributes ;
    
          if ((nvfs_status = nvfs_get_attributes (entry_name , & attributes)) == NVFS_OK)
            {
               nvfs_status = nvfs_set_attributes (entry_name ,   attributes | attributes_flags) ;
            }
        }
      nvfs_atomic_release () ;
    }

  NVFS_EXCEPTION_REPORT (nvfs_status) ;

  return nvfs_status ;
}

