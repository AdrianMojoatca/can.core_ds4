/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.get_attributes.c 33258 2015-08-19 12:46:35Z martin.bouchard $
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

static void _get_attributes_ (NVFS_Entry_Name const entry_name , NVFS_Attributes * const attributes) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_get_attributes (NVFS_Entry_Name const entry_name , NVFS_Attributes * const attributes)
{
  int status = NVFS_IRQ_CALL_FAILED ;

  if (nvfs_atomic_request ())
    {
      if ((status = setjmp (nvfs_vars.throw_vec)) == 0)
        {
          _get_attributes_ (entry_name , attributes) ;
        }

      nvfs_atomic_release () ;
    }

  return (NVFS_Status) status ;
}

//--------------------------------------------------------------------------//

static void _get_attributes_ (NVFS_Entry_Name const entry_name , NVFS_Attributes * const attributes)
{
  UInt16     entry_addr ;

  NVFS_Entry nvfs_entry ;

  if (!entry_name || entry_name[0] == 0)
    {
      nvfs_throw (NVFS_ERROR) ;
    }
  
  if (!nvfs_find_entry (entry_name , & entry_addr , 0))
    {
      nvfs_throw (NVFS_NAME_NOT_FOUND) ;
    }

  nvfs_rd_entry (& nvfs_entry , entry_addr) ;

  *attributes = nvfs_entry.link.attributes  ;
}

