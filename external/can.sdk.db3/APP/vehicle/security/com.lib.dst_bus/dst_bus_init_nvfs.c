/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_bus_init_nvfs.c 34930 2015-11-03 16:27:11Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_bus_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean init_data (void) ;
static Boolean init_error(void) ;

static NVFS_Status create_entry      (NVFS_Entry_Name const entry_name , void * src , UInt16 size) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean dst_bus_init_nvfs (void)
{  
  return (init_data() && init_error()) ;
}

//--------------------------------------------------------------------------//

static Boolean init_data(void)
{  
  if(nvfs_size_is_valid(nvfs_dst_bus_data , sizeof(Dst_Bus_Nvfs_Entry)) == NVFS_OK)
    {
      #if DBG_DST_BUS == 1
        dst_bus_dbg_print ("\nDst Bus Nvfs Data Ok", NULL , NULL) ;
      #endif
      
      return (nvfs_rd(nvfs_dst_bus_data, &dst_bus_vars.nvfs_entry) == NVFS_OK) ;
    }
  else if(create_entry (nvfs_dst_bus_data , &dst_bus_vars.nvfs_entry , sizeof(Dst_Bus_Nvfs_Entry)) == NVFS_OK)
    {
      #if DBG_DST_BUS == 1
        dst_bus_dbg_print ("\nDst Bus Nvfs Data Create", NULL , NULL) ;
      #endif

      return (init_data()) ;
    }

  #if DBG_DST_BUS == 1
    dst_bus_dbg_print ("\nDst Bus Nvfs Data Fail", NULL , NULL) ;
  #endif

  return FALSE ;
  
}

//--------------------------------------------------------------------------//

static Boolean init_error(void)
{  
  if(nvfs_size_is_valid(nvfs_dst_bus_error , sizeof(Dst_Bus_Error)) == NVFS_OK)
    {
      #if DBG_DST_BUS == 1
        dst_bus_dbg_print ("\nDst Bus Nvfs Error Ok", NULL , NULL) ;
      #endif

      return (nvfs_rd(nvfs_dst_bus_error, &dst_bus_vars.err) == NVFS_OK) ;
    }
  else if(create_entry (nvfs_dst_bus_error , &dst_bus_vars.err , sizeof(Dst_Bus_Error)) == NVFS_OK)
    {
      #if DBG_DST_BUS == 1
        dst_bus_dbg_print ("\nDst Bus Nvfs Error Create", NULL , NULL) ;
      #endif
      
      return (init_error()) ;
    }
 
  #if DBG_DST_BUS == 1
    dst_bus_dbg_print ("\nDst Bus Nvfs Error Fail", NULL , NULL) ;
  #endif

  return FALSE ;
}

//--------------------------------------------------------------------------//

static NVFS_Status create_entry (NVFS_Entry_Name const entry_name , void * src , UInt16 size)
{
  nvfs_delete(entry_name) ;
  
  memset(src , DST_BUS_BLANK_VALUE ,  size) ;  

  return (nvfs_wr_create (entry_name, src , size)) ;
}

