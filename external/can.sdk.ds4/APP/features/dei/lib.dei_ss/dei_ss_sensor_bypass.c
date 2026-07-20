/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_sensor_bypass.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#ifdef PLATFORM_933
  #include "sense_rs.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_ss_sensor_bypass_toggle( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_ss_sensor_bypass( void *remote )
{	
    if((ss_system_type & SYS_TYPE_SS) != SYS_TYPE_SS)
      return;
  //if( !ss.armed     || ss.valet       )
  	if( !get_ss_arm() || get_ss_valet() )
	{
		 notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA ); 

		 return ;
	}
	
    dei_ss_sensor_bypass_toggle();  //toggle command, no pld
#if 0
	switch ( ((Cmd_Rf_Pld*)cmd_get_pld( remote ))->bits.pld )
	{
		case CMD_RF_SENSOR_BYPASS_WARN:
        {
             notify_send( NOTIFY_SENSOR_BYPASS_WARN  );
             
			 sensor_enable( ss_sensor ) ; 

			 sensor_warning_disable( ss_sensor ); 

			 dei_ss_sensor_bp = DEI_SS_SENSOR_BYPASS_FULL ;
        }
        break;

        case CMD_RF_SENSOR_BYPASS_SHOCK: 
        case CMD_RF_SENSOR_BYPASS_OPT:
		case CMD_RF_SENSOR_BYPASS_FULL:
		{
            notify_send( NOTIFY_SENSOR_BYPASS_SHOCK );                     

			sensor_enable( ss_sensor );

			sensor_warning_disable( ss_sensor ); 

			sensor_trigger_disable( ss_sensor );

			dei_ss_sensor_bp = DEI_SS_SENSOR_BYPASS_OFF ;
                        
        };
        break;

		case CMD_RF_SENSOR_BYPASS_TOGGLE:
		{
			dei_ss_sensor_bypass_toggle();	
		}
        break;
        
        default :
        {
            notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA ) ; 	
        }
        break;
    }    
#endif
}


//--------------------------------------------------------------------------//

void dei_ss_sensor_bypass_toggle( void )
{
	switch( dei_ss_sensor_bp )
	{
		case DEI_SS_SENSOR_BYPASS_OFF :
		{
			notify_send( NOTIFY_SENSOR_BYPASS_WARN );
				
			dei_prk_flash( DEI_PRK_SENSOR_BYPASS_WARN );

#ifdef PLATFORM_CM800
			sensor_enable( ss_sensor ) ; 

			sensor_warning_disable( ss_sensor ); 
#else
			sensor_enable( ss_sensor[0] ) ; 
			sensor_warning_disable( ss_sensor[0] ); 
			
			sensor_enable( ss_sensor[1] ) ; 
			sensor_warning_disable( ss_sensor[1] ); 
			
			sensor_enable( ss_sensor[2] ) ; 
			sensor_warning_disable( ss_sensor[2] ); 
			if(ext_sensor_trigger_configured)
			{
				sensor_enable( ss_sensor_ext ) ; 
				sensor_warning_disable( ss_sensor_ext ); 
			}
#endif
				
			dei_ss_sensor_bp = DEI_SS_SENSOR_BYPASS_WARN ;

			TRACE_DEI_SS("\n\r[DEI_SS_SENSOR_BYPASS_WARN]\n\r");
		}
		break ;

		case DEI_SS_SENSOR_BYPASS_WARN :
		{
			notify_send( NOTIFY_SENSOR_BYPASS_FULL ); 
					
			dei_prk_flash( DEI_PRK_SENSOR_BYPASS_FULL );
					
			dei_ss_sensor_bp = DEI_SS_SENSOR_BYPASS_FULL ;  


#ifdef PLATFORM_CM800			
			sensor_enable( ss_sensor );

			sensor_warning_disable( ss_sensor ); 

			sensor_trigger_disable( ss_sensor );	
#else
			sensor_enable( ss_sensor[0] );
			sensor_warning_disable( ss_sensor[0] ); 
			sensor_trigger_disable( ss_sensor[0] );	
			
			sensor_enable( ss_sensor[1] );
			sensor_warning_disable( ss_sensor[1] ); 
			sensor_trigger_disable( ss_sensor[1] );	

			sensor_enable( ss_sensor[2] );
			sensor_warning_disable( ss_sensor[2] ); 
			sensor_trigger_disable( ss_sensor[2] );	

			if(ext_sensor_trigger_configured)
			{
				sensor_enable( ss_sensor_ext ) ; 
				sensor_warning_disable( ss_sensor_ext ); 
				sensor_trigger_disable( ss_sensor_ext );
			}

#endif

			TRACE_DEI_SS("\n\r[DEI_SS_SENSOR_BYPASS_FULL]\n\r");
		}
		break ; 

		case DEI_SS_SENSOR_BYPASS_FULL:
		{
#ifdef PLATFORM_CM800
			sensor_enable( ss_sensor );
#else
			sensor_enable( ss_sensor[0] );
			sensor_enable( ss_sensor[1] );
			sensor_enable( ss_sensor[2] );
			if(ext_sensor_trigger_configured)
			{
				sensor_enable( ss_sensor_ext ) ; 
			}
#endif
			
			notify_send( NOTIFY_SENSOR_BYPASS_OFF );
					
			dei_prk_flash( DEI_PRK_SENSOR_BYPASS_OFF ); 	
					
			dei_ss_sensor_bp = DEI_SS_SENSOR_BYPASS_OFF ;
			
			TRACE_DEI_SS("\n\r[DEI_SS_SENSOR_BYPASS_OFF]\n\r");  	
		}
		break ;

		default : break ;
	}
}		

//--------------------------------------------------------------------------//

