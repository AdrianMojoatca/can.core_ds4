/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: brand_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "brand_private.h"
#include "nvfs_usr.h"
#include "trace.h"
#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
	#include "sensor_proto.h"
#endif

#if CM800_DEI_BRAND == 1
    #include "dei_system.h"
    #include "dei_sc.h"
		#ifdef DEV_LPC_236x
			#include "dei_rf1.h"
			#include "dei_ask.h"
		#endif
#endif

#if CM800_AUTOSTART_BRAND == 1
    #include "as_system.h"
    #include "system_type.h"
    #include "asrf.h"
#endif

#if CM800_ASTROSTART_BRAND == 1
    #include "astrostart_system.h"
#endif
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
Boolean brand_init (void)
{
#ifdef PLATFORM_CM800	
    switch (nvfs_rd_s(nvfs_brand , &brand_id, sizeof(brand_id)))
    {
		case    NVFS_OK : 
	  	{
			TRACE_BRAND("\n\r[BRAND NVFS OK]\n\r");		
		  }
		break;
      	
		default         : 
		  {
			  TRACE_BRAND("\n\r[BRAND NVFS ERROR]\n\r");					
			  return (FALSE) ;
		  }
    }   
 
    switch(brand_id)
    {
        
        //  --- DEI BRAND --- 
        case BRAND_XPRESS_START       :
        case BRAND_VP_1BTN            :      
        case BRAND_CLIFFORD_1BTN      :
        {
            #if CM800_DEI_BRAND == 1
            {
							#ifdef DEV_LPC_236x
                dei_rf1_init();
							#endif
                
                dei_system_init(); 
            }
            #endif
        }
        break;
        
        case BRAND_VP_SC              :                    
        case BRAND_CLIFFORD_SC        :            
        case BRAND_AVITAL             :
        case BRAND_AUTOMATE           :
        {
            #if CM800_DEI_BRAND == 1
            {
                dei_sc_init();
                
                dei_system_init(); 
            }
            #endif
        }
        break;
        
        case BRAND_VP_ASK             :
        {
            #if CM800_DEI_BRAND == 1
            {
                dei_ask_init();
                dei_system_init(); 
            }
            #endif
        }
        break ;

        //  --- AUTOSTART BRAND ---         
        case BRAND_AUTOSTART_LED      :
        case BRAND_AUTOSTART_LCD1     :
        case BRAND_AUTOSTART_LCD2     : 
        {
        
        }
        break;    
                                
        case BRAND_AUTOSTART_LED_HDR  :
        {
            #if CM800_AUTOSTART_BRAND == 1
            {   
                asrf_init(ASRF_LED_HDR);
                
                as_system_init(); 
            }
            #endif
        }
        break;
        
        case BRAND_AUTOSTART_LCD1_HDR :    
        {
            #if CM800_AUTOSTART_BRAND == 1
            {     
                asrf_init(ASRF_LCD1_HDR);
                
                as_system_init(); 
            }
            #endif
        }
        break;
        
        case BRAND_AUTOSTART_LCD2_HDR : 
        {
            #if CM800_AUTOSTART_BRAND == 1
            {
                asrf_init(ASRF_LCD2_HDR);                
                
                as_system_init(); 
            }
            #endif
        }
        break;
        
        case BRAND_AUTOSTART_NGRF     :
        {        
            #if CM800_AUTOSTART_BRAND == 1
            {  
                System_Type system_type ;

                nvfs_rd_s( nvfs_sys_type , &system_type, sizeof(system_type) );
                
                if((system_type & SYS_TYPE_SS) == SYS_TYPE_SS) 
                {
                    asrf_init(ASRF_LCD2_HDR);
                }
                else
                {
					         asrf_init(ASRF_LCD1_HDR);
                }
                
                as_system_init(); 
            }
            #endif
        }  
        break ;

        
        //  --- ASTROSTART BRAND ---         
        case BRAND_ASTROSTART_LED1    :  
        case BRAND_ASTROSTART_LED2    :
        case BRAND_ASTROSTART_LCD1    :
        case BRAND_ASTROSTART_LCD2    : 
        {
            #if CM800_ASTROSTART_BRAND == 1
            {
                astrostart_system_init(); 
            }
            #endif
        }   
        break ;

        default:return (FALSE) ;    
    }
#else
//PLATFORM_933 will assume brand_id to VPX if brand_id not existed		
			dei_sc_init();
			sensor_proto_init();
			dei_system_init(); 

#endif

    return TRUE ;
}

//--------------------------------------------------------------------------//



