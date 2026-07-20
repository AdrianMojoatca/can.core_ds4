/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_feature_check.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "ngmm_feature_private.h"
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

void dei_feature_check_content(void)
{
	Boolean ret = TRUE ;

    UInt8 i ;
        
    for( i = 0 ; i <  FEAT_DEI_SIZE ; i++ )
    {
        if( dei_feature_check_list[i].check == TRUE )
        {
            if( dei_feature_get((Dei_Feat)i) < dei_feature_check_list[i].min_value )
            {
							dei_feature_set((Dei_Feat)i, dei_feature_default_table[i]);
							
//                ret = FALSE  ; 

//                TRACE_ZONE
//                (
//                    
//                    TRACE_DEI_FEAT("\n\r[DEI_FEATURE_MIN_VALUE_ERROR : %d]",i);os_tsk_wait(10);
//                    TRACE_DEI_FEAT("\n\r[expected = %02x]",dei_feature_check_list[i].min_value);os_tsk_wait(10);
//                    TRACE_DEI_FEAT("\n\r[found    = %02x]",dei_feature_get(i)                 );os_tsk_wait(10);
//                )
            }


            if( dei_feature_get((Dei_Feat)i) > dei_feature_check_list[i].max_value )
            {
							dei_feature_set((Dei_Feat)i, dei_feature_default_table[i]);
						
//                ret = FALSE  ; 

//                TRACE_ZONE
//                (
//                    
//                    TRACE_DEI_FEAT("\n\r[DEI_FEATURE_MAX_VALUE_ERROR : %d]" , i);os_tsk_wait(10);
//                    TRACE_DEI_FEAT("\n\r[expected = %02x]",dei_feature_check_list[i].max_value);os_tsk_wait(10);
//                    TRACE_DEI_FEAT("\n\r[found    = %02x]",dei_feature_get(i)                 );os_tsk_wait(10);
//                )
            }
        }
    }


    if( ret == FALSE )
    {
        dei_feature_error();
    }
    
    TRACE_DEI_FEAT("\n\r[DEI_FEATURE_CONTENT_OK]\n\r");    
}

//--------------------------------------------------------------------------//

void dei_feature_check_size(void)
{
    static UInt16 size ;
    
    if( nvfs_rd_size( nvfs_feat_dei , &size ) == NVFS_OK )
    {
        if( size != FEAT_DEI_SIZE )
        {
            TRACE_ZONE
            (
                    
                TRACE_DEI_FEAT("\n\r[DEI_FEATURE_SIZE_ERROR]");os_tsk_wait(10);
                TRACE_DEI_FEAT("\n\r[expected = %d bytes]",FEAT_DEI_SIZE);os_tsk_wait(10);
                TRACE_DEI_FEAT("\n\r[found    = %d bytes]",size        );os_tsk_wait(10);
            )   

            dei_feature_error();
        }                
    }
    
    TRACE_DEI_FEAT("\n\r[DEI_FEATURE_SIZE_OK]");
}

//--------------------------------------------------------------------------//

void dei_feature_error(void)
{
    led_reset();
    
    led_flash( LED_FLASH_NOW   , LED_RED   , 100 ,  100 , 100  , 20 , 65535 );
    led_flash( LED_FLASH_LATER , LED_GREEN , 100 ,  100 , 100  , 20 , 65535 );
    led_flash( LED_FLASH_LATER , LED_BOTH  , 100 ,  100 , 100  , 20 , 65535 );

    

    while(1)
    {        
        if( !IN_PUSH )
        {
            os_tsk_wait(100);

            if( !IN_PUSH )
            {            
                //nvfs_delete( nvfs_feat_dei );
    
                //nvram_usr_commit();        

                TRACE_DEI_FEAT("\n\r[DEI_FEATURE_RESET_TO_DEFAULT]\n\r");

                while(IN_PUSH);
    
                wdog_v_force_reset(WDOG_V_RESET_FIRMWARE,(char*)__func__,__LINE__);
            }
        }
    }    
}

//--------------------------------------------------------------------------//

