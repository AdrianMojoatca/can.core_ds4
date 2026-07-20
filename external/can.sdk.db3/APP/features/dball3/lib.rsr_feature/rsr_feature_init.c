/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_feature_init.c 19684 2013-03-07 07:07:29Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_feature_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean rsr_feature_version_init  ( void );
Boolean rsr_feature_err_init      ( void );
void    rsr_feature_check_size    ( void );
void    rsr_feature_check_content ( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean rsr_feature_init( void )
{		
	Boolean version_ok ; 

    rsr_feature_err_init();

    version_ok = rsr_feature_version_init();

    if ( version_ok )
	{
		rsr_feature_check_size();
	}
	else
	{
		rsr_feature_err_add ( RS_FEAT_ERR_INVALID_VERSION );
	}
  	
	switch (nvfs_rd(nvfs_feat_rs , &rsr_feature_list))
    {
    	case NVFS_OK :  
        {
        	if( version_ok )
			{				
                rsr_feature_check_content();
			}
			else
			{
				nvfs_delete( nvfs_feat_rs );												
			}          	        
		}
		break ;
		
		default :
		{
		 	version_ok = FALSE ;
		}
		break ;  
    }   
  	
	if (!version_ok )
	{	
		switch (nvfs_wr_create(nvfs_feat_rs, &rsr_feature_default_table , sizeof(rsr_feature_default_table) ))
	    {
	    	case NVFS_OK :  
	        {
	        	nvfs_rd(nvfs_feat_rs , &rsr_feature_list);
	          	return TRUE ;  
	        }  
	    }      
		return FALSE ;
	}

	return TRUE ;
}

//--------------------------------------------------------------------------//

Boolean rsr_feature_version_init( void )
{	
	Boolean ret = FALSE ; 
	
	static UInt8 version ; 
	
	switch ( nvfs_rd( nvfs_feat_rs_ver , &version ) )
	{
		case NVFS_OK :
		{
			if( version == RS_FEATURE_VERSION )
			{
				ret = TRUE ; 
			}
			else
			{
				nvfs_delete( nvfs_feat_rs_ver ); 
				
				if( nvfs_wr_create( nvfs_feat_rs_ver , &version , sizeof( version ) ) == NVFS_OK )
				{
					version = RS_FEATURE_VERSION ; 
					nvfs_wr( nvfs_feat_rs_ver , &version ) ; 
				}
			}
		}
		break ; 

		default :
		{
			if( nvfs_wr_create( nvfs_feat_rs_ver , &version , sizeof( version ) ) == NVFS_OK )
			{
				version = RS_FEATURE_VERSION ; 
				nvfs_wr( nvfs_feat_rs_ver , &version ) ; 
			}
		}
		break ; 
	}
	
	return ret  ;
}	

//--------------------------------------------------------------------------//

Boolean rsr_feature_err_init (void)
{
  switch (nvfs_rd(nvfs_feat_rs_err , &rsr_feature_err))
    {
      case NVFS_OK : 
	    {
	  	  return (TRUE) ;
		}
    }

  rsr_feature_err.count = 0;
  
  memset(&rsr_feature_err , NULL, sizeof(rsr_feature_err)) ;
  
  switch (nvfs_wr_create(nvfs_feat_rs_err, &rsr_feature_err , sizeof(rsr_feature_err)))
    {
      case NVFS_OK :  
        {         
          return rsr_feature_err_init() ;
        }  
    }      
  return FALSE ;
}

