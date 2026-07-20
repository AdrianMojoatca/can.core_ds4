/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: key_init_nvfs.c 16273 2012-07-31 13:01:18Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "key_private.h"
#include "nvram_usr.h"
#include "nvfs_usr.h"
#include <string.h>
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean init_key   (void) ;
static Boolean init_data  (void) ;
static Boolean init_error (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
Boolean key_init_nvfs (void)
{
  return(init_key() & init_error()) ;
}

//--------------------------------------------------------------------------//

static Boolean init_key (void)
{
  UInt16 size = 0 ;

  if(nvfs_rd_size(nvfs_key , &size) == NVFS_OK)
    {
      if(size == KEY_NVFS_SIZE && nvfs_rd (nvfs_key , &key_memory) == NVFS_OK)
        {
          return TRUE ;
        }
    }

  nvfs_delete(nvfs_key)  ;

  memset(&key_memory , 0 , KEY_NVFS_SIZE) ;

  key_memory.sel_addr       = 0x03 ;
  key_memory.usr_data1      = 0x80 ;
  key_memory.uid.item.ma    = 0x01 ; 
  
  key_memory.config.byte[0] = 0x50 ;
  key_memory.config.byte[1] = 0x46 ;

  return TRUE ;
}

//--------------------------------------------------------------------------//

static Boolean init_error (void)
{
  static Key_Error curr_err ;
  
  switch (nvfs_rd (nvfs_key_error , &key_error))
    {
      case NVFS_OK : return (TRUE) ;
    }

  switch (nvfs_wr_create (nvfs_key_error, &curr_err , sizeof(Key_Error)))
    {
      case NVFS_OK :  
        {         
          return init_error() ;
        }  
    }      
  return FALSE ;
}

