/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.iter_init.c 33258 2015-08-19 12:46:35Z martin.bouchard $
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

static void _iter_init_ (NVFS_Iterator * const iter) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_iter_init (NVFS_Iterator * const iter , NVFS_Char * const entry_name , UInt16 max_name_size)
{
  int status = NVFS_IRQ_CALL_FAILED ;

  iter->name_buff      = entry_name    ;
  iter->name_buff_size = max_name_size ;

  if (nvfs_atomic_request ())
    {
      if ((status = setjmp (nvfs_vars.throw_vec)) == 0)
        {
          _iter_init_             (iter) ;

          status = nvfs_iter_next (iter) ;
        }

      nvfs_atomic_release () ;
    }

  return (NVFS_Status) status ;
}

//--------------------------------------------------------------------------//

static void _iter_init_ (NVFS_Iterator * const iter)
{
  NVFS_Header nvfs_header ;

  nvfs_rd_header (& nvfs_header) ;

  iter->entry_addr_next = nvfs_header.mem.list.root ;
}

