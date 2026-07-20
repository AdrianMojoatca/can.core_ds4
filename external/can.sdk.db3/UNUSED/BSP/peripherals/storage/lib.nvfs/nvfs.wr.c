/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.wr.c 33258 2015-08-19 12:46:35Z martin.bouchard $
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

static void _wr_ (NVFS_Entry_Name const entry_name , const void * const ram_srce) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_wr (NVFS_Entry_Name const entry_name , const void * const ram_srce)
{
  int status = NVFS_IRQ_CALL_FAILED ;

  if (nvfs_atomic_request ())
    {
      if ((status = setjmp (nvfs_vars.throw_vec)) == 0)
        {
          _wr_ (entry_name , ram_srce) ;
        }
    
      nvfs_atomic_release () ;
    }

  return (NVFS_Status) status ;
}

//--------------------------------------------------------------------------//

static void _wr_ (NVFS_Entry_Name const entry_name , const void * const ram_srce)
{
  UInt16     entry_addr ;

  NVFS_Entry nvfs_entry ;

  if (!entry_name || entry_name[0] == 0 || ram_srce == 0)
    {
      nvfs_throw (NVFS_ERROR) ;
    }
  
  if (!nvfs_find_entry (entry_name , & entry_addr , 0))
    {
      nvfs_throw (NVFS_NAME_NOT_FOUND) ;
    }

  nvfs_rd_entry (& nvfs_entry , entry_addr) ;

  if (nvfs_entry.link.attributes & NVFS_ATTRIB_WR_PROTECTED)
    {
      nvfs_throw (NVFS_WR_PROTECTED) ;
    }

  if (!nvram_wr (nvfs_entry.link.data.addr , ram_srce , nvfs_entry.link.data.size))
    {
      nvfs_throw (NVFS_ERROR) ;
    }

  nvfs_wr_entry (& nvfs_entry , entry_addr) ;
}

