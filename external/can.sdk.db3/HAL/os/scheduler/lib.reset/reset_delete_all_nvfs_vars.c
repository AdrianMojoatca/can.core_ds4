/*==========================================================================*/
// $Id: reset_delete_all_nvfs_vars.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "reset_private.h"
#include "dbg.h"

#if	defined PLATFORM_CM800 || defined PLATFORM_933			//def PLATFORM_CM800
#include "nvfs_usr.h"
#include "system_type.h"
#include "brand.h"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define	cNVFS_ENTRY_NAME_MAX_EXPECTED_LENGHT	(50 + 1) // Extra byte for zero end-of-string terminator

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void reset_delete_all_nvfs_vars (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void reset_delete_all_nvfs_vars (void)
{
  NVFS_Iterator iter ;
  NVFS_Status   status ;
  
  NVFS_Status   status_delete ;
  NVFS_Char     name [cNVFS_ENTRY_NAME_MAX_EXPECTED_LENGHT] ;
  
  UInt16		vars_to_skip ;
  UInt16		nb_of_uneraseable_vars = 0;                                                                     
  
  #if	defined PLATFORM_CM800 || defined PLATFORM_933		//def PLATFORM_CM800        
    Brand_Id    brand_id ;
    NVFS_Status brand_nvfs_status;
    System_Type sys_type ;    
    NVFS_Status sys_type_nvfs_status;
  
      
    brand_nvfs_status = nvfs_rd(nvfs_brand    , &brand_id) ;  //save the Brand Id

    #if DBG_RESET == 1
    if(brand_nvfs_status==NVFS_OK)
    {   
        dbg_tx_text ("Brand Id saved\r\n") ;
    }
    else
    {
        dbg_tx_text ("Brand Id saving error\r\n") ;
    }        
    #endif
        
    sys_type_nvfs_status  = nvfs_rd(nvfs_sys_type , &sys_type) ;  //save the system type
  
    #if DBG_RESET == 1
    if(sys_type_nvfs_status==NVFS_OK)
    {   
      dbg_tx_text ("Sys Type saved\r\n") ;
    }
    else
    {
      dbg_tx_text ("Sys Type saving error\r\n") ;
    }
    #endif

  #endif

      
  status = nvfs_iter_init (& iter , name , sizeof_array (name)) ;

  while (status == NVFS_OK && iter.is_valid)
  {
	if (!iter.is_truncated)
    {
	#if DBG_RESET == 1
	  dbg_tx_text ("\t[Deleting '") ;
	  dbg_tx_text (name) ;
	  dbg_tx_text ("'") ;
	#endif
	  
	  status_delete = nvfs_delete (name) ;
	  if ( status_delete == NVFS_OK)
	  {
	  #if DBG_RESET == 1
		dbg_tx_text ("]\r\n") ;
	  #endif
		
		// Since the delete has shifted all the entries, we have to re-initialize the iterator (unfortunately from the root of the list)
		status = nvfs_iter_init (& iter , name , sizeof_array (name)) ;
		
		// Skip the variables that we know we cannot delete
		vars_to_skip = nb_of_uneraseable_vars;
		while (vars_to_skip-- && status == NVFS_OK)
		{
		  status = nvfs_iter_next (& iter) ;
		}
	  }
	  else
	  {
	  #if DBG_RESET == 1
		dbg_tx_text ("  ...Failed") ;
		if (status_delete == NVFS_WR_PROTECTED)
		{
		  dbg_tx_text (" (variable is write-protected)") ;
		}
	    dbg_tx_text ("]\r\n") ;
	  #endif
		
		// Since the delete was not successful, we can resume with the next entry
		status = nvfs_iter_next (& iter) ;
		nb_of_uneraseable_vars++ ;
	  }
    }
  }
  
  #if	defined PLATFORM_CM800 || defined PLATFORM_933		//def PLATFORM_CM800   
  if(brand_nvfs_status == NVFS_OK)
  {
     if(nvfs_wr_create( nvfs_brand , &brand_id  , sizeof(Brand_Id))==NVFS_OK)
     {
        #if DBG_RESET == 1
		    dbg_tx_text ("Brand Id restored\r\n") ;
	    #endif
     }
     else
     {
        #if DBG_RESET == 1
		    dbg_tx_text ("Fail to restore Brand Id\r\n") ;
	    #endif
     }        
  }
  	     
  if(sys_type_nvfs_status == NVFS_OK)
  {
     if(nvfs_wr_create(nvfs_sys_type , &sys_type  , sizeof(System_Type))==NVFS_OK)
     {
        #if DBG_RESET == 1
		    dbg_tx_text ("System Type restored\r\n") ;
	    #endif
     }
     else
     {
        #if DBG_RESET == 1
		    dbg_tx_text ("Fail to restore System Type\r\n") ;
	    #endif
     }        
  }
  #endif
  
  return ;
}
