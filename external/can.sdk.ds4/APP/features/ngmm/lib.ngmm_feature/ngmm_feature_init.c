/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_feature_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "ngmm_feature_private.h"
#include "prog.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if  NGMM_FEATURE_VERSION  != 23
#error !!Update config_ngmm_feature.h with the latest version from LIB.NGMM_FEATURE\TEMPLATE folder         
#endif

#if NEG_OUT1_CFG == 2
  #if FEAT_DEI_SYS40_ENABLE != 0x00 || FEAT_DEI_SYS40 != DEI_OUT_PRG_FUNCTION_RESERVED
    #error "config error OUTPUT1 used in data"
  #endif
#endif

#if NEG_OUT2_CFG == 2
  #if FEAT_DEI_SYS41_ENABLE != 0x00 || FEAT_DEI_SYS41 != DEI_OUT_PRG_FUNCTION_RESERVED
    #error "config error OUTPUT2 used in data"
  #endif
#endif

#if NEG_OUT3_CFG == 2
  #if FEAT_DEI_SYS42_ENABLE != 0x00 || FEAT_DEI_SYS42 != DEI_OUT_PRG_FUNCTION_RESERVED
    #error "config error OUTPUT3 used in data"
  #endif
#endif

#if NEG_OUT4_CFG == 2
  #if FEAT_DEI_SYS43_ENABLE != 0x00 || FEAT_DEI_SYS43 != DEI_OUT_PRG_FUNCTION_RESERVED
    #error "config error OUTPUT4 used in data"
  #endif
#endif

#if NEG_OUT5_CFG == 2
  #if FEAT_DEI_SYS44_ENABLE != 0x00 || FEAT_DEI_SYS44 != DEI_OUT_PRG_FUNCTION_RESERVED
    #error "config error OUTPUT5 used in data"
  #endif
#endif

#if POS_OUT1_CFG == 2
  #if FEAT_DEI_SYS45_ENABLE != 0x00 || FEAT_DEI_SYS45 != DEI_OUT_PRG_FUNCTION_RESERVED
    #error "config error OUTPUT6 used in data"
  #endif
#endif

#if POS_OUT2_CFG == 2
  #if FEAT_DEI_SYS46_ENABLE != 0x00 || FEAT_DEI_SYS46 != DEI_OUT_PRG_FUNCTION_RESERVED
    #error "config error OUTPUT7 used in data"
  #endif
#endif

#if RLY1_CTL_CFG == 2
  #if FEAT_DEI_SYS54_ENABLE != 0x00 || FEAT_DEI_SYS54 != DEI_OUT_PRG_FUNCTION_RESERVED
    #error "config error RELAY1 used in data"
  #endif
#endif

#if RLY2_CTL_CFG == 2
  #if FEAT_DEI_SYS55_ENABLE != 0x00 || FEAT_DEI_SYS55 != DEI_OUT_PRG_FUNCTION_RESERVED
    #error "config error RELAY2 used in data"
  #endif
#endif

#if AUX3_IN_CFG == 2
  #if FEAT_DEI_SYS58_ENABLE != 0x00 || (FEAT_DEI_SYS58 != 0x0A && FEAT_DEI_SYS58 != 0x0B && FEAT_DEI_SYS58 != DEI_IN_PRG_FUNCTION_RESERVED)
    #error "config error AUX3_IN used in data"
  #endif
#endif

#if POS_IN1_CFG == 2
  #if FEAT_DEI_SYS62_ENABLE != 0x00 || (FEAT_DEI_SYS62 != 0x0B && FEAT_DEI_SYS62 != DEI_IN_PRG_FUNCTION_RESERVED)
    #error "config error POS_IN1 used in data"
  #endif
#endif




/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean dei_feature_version_init( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Boolean dei_feature_init( void )
{       
    Boolean version_ok ; 
	  Boolean result = TRUE;

    wake_register            ( (Wake_Func           )dei_feature_save_sleep );
    wdog_v_register_preamble ( (Wdog_v_Preamble_Func)dei_feature_save );
    
    version_ok = dei_feature_version_init();

    if( version_ok == TRUE )
    {
        dei_feature_check_size();
    }
    
    switch (nvfs_rd_s(nvfs_feat_dei , dei_feature_list, sizeof(dei_feature_list))) // array
    {
        case NVFS_OK :  
        {
					if( version_ok )
					{               
							feature_set_table(&dei_feature_list) ;

							TRACE_DEI_FEAT("\n\r[DEI_FEATURE_LOAD_FROM_NVFS]\n\r");             

							dei_feature_check_content();
					}
					else
					{
							nvfs_delete( nvfs_feat_dei );                                               
					}                   
        }
        break ;
        
        default :
        {
            version_ok = FALSE ;
        }
        break ;  
    }   
    
    if(! version_ok )
    {   
        switch (nvfs_wr_create(nvfs_feat_dei, &dei_feature_default_table , FEAT_DEI_SIZE))
        {
            case NVFS_OK :  
            {
                nvfs_rd_s(nvfs_feat_dei , dei_feature_list, sizeof(dei_feature_list)); // array
                
                feature_set_table(&dei_feature_list) ;
                        
                TRACE_DEI_FEAT("\n\r[DEI_FEATURE_LOAD_DEFAULT]\n\r");
              
                result = nvram_usr_commit()  ;  
            }
						break;
						default:
							result = FALSE;
						
        }      
    }
		
		prog_remap();  // overwrite config_prog variables based on dei_feature settings

    return result ;
}

//--------------------------------------------------------------------------//

Boolean dei_feature_version_init( void )
{   
    Boolean ret = FALSE ; 
    
    static UInt8 version ; 
    
    switch( nvfs_rd_s( nvfs_feat_dei_ver , &version, sizeof(version) ) )
    {
        case NVFS_OK :
        {
            if( version == NGMM_FEATURE_VERSION )
            {
                TRACE_DEI_FEAT("\n\r[DEI_FEATURE_VERSION_OK]\n\r");

                ret = TRUE ; 
            }
            else
            {
                nvfs_delete( nvfs_feat_dei_ver ); 
                
                if( nvfs_wr_create( nvfs_feat_dei_ver , &version , sizeof( version ) ) == NVFS_OK )
                {
                    version = NGMM_FEATURE_VERSION ; 

                    nvfs_wr( nvfs_feat_dei_ver , &version ) ; 
                    
                    TRACE_DEI_FEAT("\n\r[DEI_FEATURE_VERSION_ERROR]\n\r");
                }
                else
                {
                    TRACE_DEI_FEAT("\n\r[DEI_FEATURE_VERSION_ERROR_CREATE]\n\r");       
                }
            }
        }
        break ; 


        default :
        {
            if( nvfs_wr_create( nvfs_feat_dei_ver , &version , sizeof( version ) ) == NVFS_OK )
            {
                version = NGMM_FEATURE_VERSION ; 

                nvfs_wr( nvfs_feat_dei_ver , &version ) ; 
                
                TRACE_DEI_FEAT("\n\r[DEI_FEATURE_VERSION_NOT_FOUND]\n\r");
            }
            else
            {
                TRACE_DEI_FEAT("\n\r[DEI_FEATURE_VERSION_ERROR_CREATE]\n\r");       
            }
        }
        break ; 
    }
    
    return ret  ;
}   

//--------------------------------------------------------------------------//      

