/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs.rd.c 33258 2015-08-19 12:46:35Z martin.bouchard $
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

static void _rd_ (NVFS_Entry_Name const entry_name , void * const ram_dest) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

NVFS_Status nvfs_rd (NVFS_Entry_Name const entry_name , void * const ram_dest)
{
  int status = NVFS_IRQ_CALL_FAILED ;

  if (nvfs_atomic_request ())
    {
      if ((status = setjmp (nvfs_vars.throw_vec)) == 0)
        {
          _rd_ (entry_name , ram_dest) ;
        }

      nvfs_atomic_release () ;
    }

  return (NVFS_Status) status ;
}

//--------------------------------------------------------------------------//

static void _rd_ (NVFS_Entry_Name const entry_name , void * const ram_dest)
{
  UInt16     entry_addr ;

  NVFS_Entry nvfs_entry ;

  if (!entry_name || entry_name[0] == 0 || ram_dest == 0)
    {
      nvfs_throw (NVFS_ERROR) ;
    }
  
  if (!nvfs_find_entry (entry_name , & entry_addr , 0))
    {
      nvfs_throw (NVFS_NAME_NOT_FOUND) ;
    }

  nvfs_rd_entry (& nvfs_entry , entry_addr) ;

  if (!nvram_rd (nvfs_entry.link.data.addr , ram_dest , nvfs_entry.link.data.size))
    {
      nvfs_throw (NVFS_ERROR) ;
    }
}

// replacement API for nvfs_rd() to fix issue NCC-DIRE002-019
NVFS_Status nvfs_rd_s (NVFS_Entry_Name const entry_name , void * const ram_dest, UInt16 ram_size)
{
NVFS_Status sts;
UInt16 entry_size;
	
	sts = nvfs_rd_size(entry_name,&entry_size);
	if(sts == NVFS_OK)
	{
    if(ram_size >= entry_size)
			nvfs_rd(entry_name,ram_dest); // safe to read.
		else
			sts = NVFS_ERROR; // read data too big for supplied ram_size
	}
	return sts;
}


