/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_status_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
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

//--------------------------------------------------------------------------//

Boolean dei_ss_status_init( Dei_Ss_Status *dei_status )
{
	
  	Boolean status  = FALSE ;

	static  UInt8   ss_cur_ver = DEI_SS_VER   ;  	
    static  UInt8   ss_ver     = 0            ;  	
    static  Dei_Ss_Status  cur_status = { 0 , 0 , 0 , 0  , DEI_SS_NO_TRIGGER , DEI_SS_NO_TRIGGER };

  	if( nvfs_rd_s(nvfs_ss_ver , &ss_ver, sizeof(ss_ver)) != NVFS_OK )
    {
    	//The version entry is not present in NVFS
        //Create new entry        
        status |= nvfs_wr_create(nvfs_ss_ver  , &ss_cur_ver , sizeof(ss_cur_ver)) == NVFS_OK ;
      	status |= nvfs_wr_create(nvfs_ss_stat , &cur_status , sizeof(cur_status)) == NVFS_OK ; 
        
        TRACE_DEI_SS("[DEI_SS_VERSION_NOT_PRESENT]\n\r");       
    }
  	else 
    {
    	//The version entry is present in NVFS
      
        if( ss_ver != ss_cur_ver )
        {
        	//The version from NVFS not matching the version from firmware
            //Delete the entry
            status |= nvfs_wr_delete(nvfs_ss_ver  , &ss_cur_ver  , sizeof(ss_cur_ver)) == NVFS_OK ;
          	status |= nvfs_wr_delete(nvfs_ss_stat , &cur_status  , sizeof(cur_status)) == NVFS_OK ; 
            
            //Create new entry
            status |= nvfs_wr_create(nvfs_ss_ver  , &ss_cur_ver , sizeof(ss_cur_ver)) == NVFS_OK ;
      	    status |= nvfs_wr_create(nvfs_ss_stat , &cur_status , sizeof(cur_status)) == NVFS_OK ;
            
            TRACE_DEI_SS("[DEI_SS_VERSION_ERROR]\n\r");                 
        }
    }


    if( nvfs_rd_s(nvfs_ss_stat , dei_status, sizeof(dei_status)) != NVFS_OK )
    {        
        TRACE_DEI_SS("[DEI_SS_STATUS_ERROR]\n\r");                 

        status |= nvfs_wr_create(nvfs_ss_stat , &cur_status , sizeof(cur_status)) == NVFS_OK ;        
    }
    else
    {
                
        TRACE_DEI_SS("[DEI_SS_STATUS_OK]\n\r");                 
    }

  	if(status)
    {
    	status |= nvram_usr_commit() ;
    }

  	status |= nvfs_rd_s(nvfs_ss_stat , dei_status, sizeof(dei_status)) == NVFS_OK ;

  	return status ;
}

//--------------------------------------------------------------------------//


