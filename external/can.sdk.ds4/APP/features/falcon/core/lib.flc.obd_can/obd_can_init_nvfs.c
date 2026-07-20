/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_init_nvfs.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "obd_can_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean init_ver    (void) ;
static Boolean init_config (void) ;
static Boolean init_err    (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean obd_can_init_nvfs (void)
{
  return (init_ver() && init_config() && init_err()) ;
}

//--------------------------------------------------------------------------//

static Boolean init_ver (void)
{
  UInt8 version;
	
  if ((nvfs_size_is_valid(nvfs_obd_can_ver, sizeof(version)) == NVFS_OK) && (nvfs_rd(nvfs_obd_can_ver , &version) == NVFS_OK) && (version == OBD_CAN_CUR_VER))
	{
	  #if DBG_OBD_CAN == 1
	  	dbg_tx_text ("\nObd Can init_ver success ") ;
        dbg_tx_h8(version) ;
	  #endif
	  
      return TRUE;
	}
	//firmware version not identical -> delete entries
  nvfs_delete(nvfs_obd_can_ver)   ;
  nvfs_delete(nvfs_obd_can_config)      ;
  nvfs_delete(nvfs_obd_can_err) ;

	//recreate entries with updated values
	
  version  = OBD_CAN_CUR_VER ;

  nvfs_wr_create(nvfs_obd_can_ver, &version , sizeof(version));

  #if DBG_OBD_CAN == 1
	dbg_tx_text ("\nRecreate Obd Can Version ") ;
    dbg_tx_h8(version) ;
  #endif
	
    
    
  return init_ver();


//  UInt16 size = 0 ;
//  UInt16 cur_ver = OBD_CAN_CUR_VER;
//
//  if(nvfs_rd_size(nvfs_obd_can_ver , &size) == NVFS_OK)
//    {
//      if(size != sizeof(cur_ver))
//        {
//          nvfs_delete(nvfs_obd_can_ver)  ;
//        }
//      else
//        {
//          if (nvfs_rd(nvfs_obd_can_ver , &cur_ver) == NVFS_OK)
//		  {
//		  	return cur_ver == OBD_CAN_CUR_VER;
//		  }
//		  return FALSE;
//        }
//    }
//
//  cur_ver = OBD_CAN_CUR_VER;
//  switch (nvfs_wr_create(nvfs_obd_can_ver, &cur_ver , sizeof(cur_ver)))
//    {
//      case NVFS_OK :  
//        {
//           return init_ver() ;
//        }  
//    }      
//
//  return FALSE ;
}

//--------------------------------------------------------------------------//

static Boolean init_config (void)
{
  if ((nvfs_size_is_valid(nvfs_obd_can_config, sizeof(obd_can_config_default)) == NVFS_OK) && (nvfs_rd (nvfs_obd_can_config , &obd_can_cfg)== NVFS_OK))
    {
      #if DBG_OBD_CAN == 1
		dbg_tx_text ("\nObd Can Config success") ;
	  #endif
	  
      return TRUE ; 
    }   

  nvfs_delete(nvfs_obd_can_config) ;
  
  if (nvfs_wr_create(nvfs_obd_can_config, &obd_can_config_default , sizeof(obd_can_config_default)) == NVFS_OK)
    {
	  #if DBG_OBD_CAN == 1
	    dbg_tx_text ("\nObd Can Config re-create") ;
	  #endif
	
      return init_config() ;
  }

  return FALSE ;
//  UInt16 size = 0 ;
//
//  if (!same_version)
//  {
//    nvfs_delete(nvfs_obd_can_config)  ;
//  }
//  else
//  {
//	  if(nvfs_rd_size(nvfs_obd_can_config , &size) == NVFS_OK)
//	    {
//	      if(size != sizeof(obd_can_config_default))
//	        {
//	          nvfs_delete(nvfs_obd_can_config)  ;
//	        }
//	      else
//	        {
//	          return (nvfs_rd(nvfs_obd_can_config , &obd_can_cfg) == NVFS_OK) ;
//	        }
//	    }
//  }
//
//  switch (nvfs_wr_create(nvfs_obd_can_config, &obd_can_config_default , sizeof(obd_can_config_default)))
//    {
//      case NVFS_OK :  
//        {
//           return init_config(same_version) ;
//        }  
//    }      
//
//  return FALSE ;
}

//--------------------------------------------------------------------------//

static Boolean init_err (void)
{
  if ((nvfs_size_is_valid(nvfs_obd_can_err, sizeof(obd_can_err)) == NVFS_OK) && (nvfs_rd (nvfs_obd_can_err , &obd_can_err)== NVFS_OK))
    {
      #if DBG_OBD_CAN == 1
		dbg_tx_text ("\nObd Can Err success") ;
	  #endif
      
      
      return (TRUE) ;
    }

  nvfs_delete(nvfs_obd_can_err) ;
  
  memset(&obd_can_err, NULL , sizeof(obd_can_err)) ;
  
  if((nvfs_wr_create (nvfs_obd_can_err, &obd_can_err , sizeof(obd_can_err))) == NVFS_OK)
    {
	  #if DBG_OBD_CAN == 1
	    dbg_tx_text ("\nObd Can Err re-create") ;
	  #endif
      
      return init_err() ;  
    }      

  return FALSE ;
    
//  UInt16 size = 0 ;
//
//  if (!same_version)
//  {
//    nvfs_delete(nvfs_obd_can_err)  ;
//  }
//  else
//  {
//	  if(nvfs_rd_size(nvfs_obd_can_err , &size) == NVFS_OK)
//	    {
//	      if(size != sizeof(obd_can_err))
//	        {
//	          nvfs_delete(nvfs_obd_can_err)  ;
//	        }
//	      else
//	        {
//	          return (nvfs_rd(nvfs_obd_can_err , &obd_can_err) == NVFS_OK) ;
//	        }
//	    }
//  }
//
//  obd_can_err.count = 0;
//  switch (nvfs_wr_create(nvfs_obd_can_err, &obd_can_err , sizeof(obd_can_err)))
//    {
//      case NVFS_OK :  
//        {
//           return init_err(same_version) ;
//        }  
//    }      
//
//  return FALSE ;
}

