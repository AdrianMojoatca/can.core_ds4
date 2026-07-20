/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_feature_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_feature_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if  DEI_FEATURE_VERSION  != 24
#error !!Update config_dei_feature.h with the latest version from LIB.DEI_FEATURE\TEMPLATE folder         
#endif

#if NEG_OUT1_CFG == 2
  #if FEAT_DEI_SYS31_ENABLE != 0x00 || FEAT_DEI_SYS31 != 0x1D
    #error "config error OUTPUT1 used in data"
  #endif
#endif

#if NEG_OUT2_CFG == 2
  #if FEAT_DEI_SYS32_ENABLE != 0x00 || FEAT_DEI_SYS32 != 0x1D
    #error "config error OUTPUT2 used in data"
  #endif
#endif

#if NEG_OUT3_CFG == 2
  #if FEAT_DEI_SYS33_ENABLE != 0x00 || FEAT_DEI_SYS33 != 0x1D
    #error "config error OUTPUT3 used in data"
  #endif
#endif

#if NEG_OUT4_CFG == 2
  #if FEAT_DEI_SYS34_ENABLE != 0x00 || FEAT_DEI_SYS34 != 0x1D
    #error "config error OUTPUT4 used in data"
  #endif
#endif

#if NEG_OUT5_CFG == 2
  #if FEAT_DEI_SYS35_ENABLE != 0x00 || FEAT_DEI_SYS35 != 0x1D
    #error "config error OUTPUT5 used in data"
  #endif
#endif

#if POS_OUT1_CFG == 2
  #if FEAT_DEI_SYS36_ENABLE != 0x00 || FEAT_DEI_SYS36 != 0x1D
    #error "config error OUTPUT6 used in data"
  #endif
#endif

#if POS_OUT2_CFG == 2
  #if FEAT_DEI_SYS37_ENABLE != 0x00 || FEAT_DEI_SYS37 != 0x1D
    #error "config error OUTPUT7 used in data"
  #endif
#endif

#if RLY1_CTL_CFG == 2
  #if FEAT_DEI_SYS45_ENABLE != 0x00 || FEAT_DEI_SYS45 != 0x1D
    #error "config error RELAY1 used in data"
  #endif
#endif

#if RLY2_CTL_CFG == 2
  #if FEAT_DEI_SYS46_ENABLE != 0x00 || FEAT_DEI_SYS46 != 0x1D
    #error "config error RELAY2 used in data"
  #endif
#endif

#if AUX3_IN_CFG == 2
  #if FEAT_DEI_SYS49_ENABLE != 0x00 || (FEAT_DEI_SYS49 != 0x0A && FEAT_DEI_SYS49 != 0x0B && FEAT_DEI_SYS49 != 0x0C)
    #error "config error AUX3_IN used in data"
  #endif
#endif

#if POS_IN1_CFG == 2
  #if FEAT_DEI_SYS55_ENABLE != 0x00 || (FEAT_DEI_SYS55 != 0x0D && FEAT_DEI_SYS55 != 0x0E)
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

    wake_register            ( (Wake_Func           )dei_feature_save_sleep );
    wdog_v_register_preamble ( (Wdog_v_Preamble_Func)dei_feature_save );
    
    version_ok = dei_feature_version_init();

    if( version_ok == TRUE )
    {
        dei_feature_check_size();
    }
    
    switch (nvfs_rd(nvfs_feat_dei , &dei_feature_list))
    {
        case NVFS_OK :  
        {
            if( version_ok )
            {               
#if (DBG_JTAG == 1 && BUILD_NGMM != 1)
                switch (nvfs_wr_create(nvfs_feat_dei, &dei_feature_default_table , FEAT_DEI_SIZE))
                {
                    case NVFS_OK :  
                    {
                        nvfs_rd(nvfs_feat_dei , &dei_feature_list);

                        feature_set_table(&dei_feature_list) ;

                        TRACE_DEI_FEAT("\n\r[DEI_FEATURE_LOAD_DEFAULT]\n\r");

                        return nvram_usr_commit()  ;  
                    }  
                }      
#else
                feature_set_table(&dei_feature_list) ;

                TRACE_DEI_FEAT("\n\r[DEI_FEATURE_LOAD_FROM_NVFS]\n\r");             

                dei_feature_check_content();
#endif
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
                nvfs_rd(nvfs_feat_dei , &dei_feature_list);
                
                feature_set_table(&dei_feature_list) ;
                        
                TRACE_DEI_FEAT("\n\r[DEI_FEATURE_LOAD_DEFAULT]\n\r");
              
                return nvram_usr_commit()  ;  
            }  
        }      
    
        return FALSE ;
    }

    return TRUE ;
}

//--------------------------------------------------------------------------//

Boolean dei_feature_version_init( void )
{   
    Boolean ret = FALSE ; 
    
    static UInt8 version ; 
    
    switch( nvfs_rd( nvfs_feat_dei_ver , &version ) )
    {
        case NVFS_OK :
        {
            if( version == DEI_FEATURE_VERSION )
            {
                TRACE_DEI_FEAT("\n\r[DEI_FEATURE_VERSION_OK]\n\r");

                ret = TRUE ; 
            }
            else
            {
                nvfs_delete( nvfs_feat_dei_ver ); 
                
                if( nvfs_wr_create( nvfs_feat_dei_ver , &version , sizeof( version ) ) == NVFS_OK )
                {
                    version = DEI_FEATURE_VERSION ; 

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
                version = DEI_FEATURE_VERSION ; 

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

