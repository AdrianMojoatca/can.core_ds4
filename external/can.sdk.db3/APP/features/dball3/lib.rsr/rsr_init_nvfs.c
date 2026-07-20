/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_init_nvfs.c 19207 2013-02-12 19:50:47Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean rsr_init_version  (void) ;
static Boolean rsr_init_data     (void) ;
static Boolean rsr_init_error    (void) ;
static Boolean rsr_init_option   (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
Boolean rsr_init_nvfs (void)
{
  return(rsr_init_version () & rsr_init_option() & rsr_init_error()) ;
}

//--------------------------------------------------------------------------//
Boolean rsr_init_version (void)
{
  UInt8 version ;
  
  if(nvfs_rd(nvfs_rsr_ver , &version) == NVFS_OK)
    {
      #if DBG_RSR == 1
        TRACE("\nRsr Read Version %d" , version) ;
      #endif
      
      if(version == RSR_VERSION)
        {
          return TRUE ;
        }
        
    }

  rsr_reset() ;

  version = RSR_VERSION ;
  
  #if DBG_RSR == 1 
    TRACE("\nRsr Write Version: %d" , version) ;
  #endif    

  switch (nvfs_wr_create(nvfs_rsr_ver, &version , sizeof(version)))
    {
      case NVFS_OK :  
        {          
          return rsr_init_version() ; 
        }  
    }      

  return FALSE ;

}

//--------------------------------------------------------------------------//

static Boolean rsr_init_option (void)
{
    return TRUE ;  // nvfs not required now could be used for future expension
}

//--------------------------------------------------------------------------//

static Boolean rsr_init_error (void)
{
  static Rsr_Error cur_err ;
  
  switch (nvfs_rd(nvfs_rsr_error , &rsr_err))
    {
      case NVFS_OK : return (TRUE) ;
    }

  switch (nvfs_wr_create(nvfs_rsr_error, &cur_err , sizeof(Rsr_Error)))
    {
      case NVFS_OK :  
        {         
          return rsr_init_error() ;//  & nvram_usr_commit() ;
        }  
    }      
  return FALSE ;  
}
