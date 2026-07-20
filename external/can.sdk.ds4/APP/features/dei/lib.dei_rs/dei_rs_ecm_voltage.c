/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_ecm_voltage.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Rs_Ecm_Voltage
{
    double  voltage_ref        ; 
    UInt16  engine_check_period;
    Boolean engine_running     ; 
    Boolean first_check        ;
}Dei_Rs_Ecm_Voltage;
    

#define VBAT_CHECK_THRESHOLD  (0.5)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void    dei_rs_ecm_voltage_init          ( void );
static Boolean dei_rs_ecm_voltage_engine_running( void );
static void    dei_rs_ecm_voltage_trace         ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
Dei_Rs_Ecm_Voltage      dei_rs_ecm_voltage_vars;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_ecm_voltage( void )
{
	while( dei_rs_state != NULL )
	{			
		rs_check_state:
		{			
			os_tsk_wait( 10 ); 
			
			if( (dei_rs_state == dei_rs_gwr       )||\
                (dei_rs_state == dei_rs_acc       )||\
                (dei_rs_state == dei_rs_ign       )||\
                (dei_rs_state == dei_rs_wait_start)
                )
			{			  
				if( dei_rs_state == dei_rs_acc )
                {                                        
                    dei_rs_ecm_voltage_vars.engine_running = get_ign_status();
                    
                    set_rs_start_successful( FALSE );

#if DEBUG_DEI_RS	== 1
      //TRACE_DEI_RS("\n\r[DEI_RS_ENGINE_CHECK : %s ]\n\r" , dei_rs_ecm_voltage_vars.engine_running ? "RUNNING" : "NOT RUNNING"); 
			//too much, will make d2d2 debug out overload
#endif									
                }
                                
                if( dei_rs_state == dei_rs_ign )
                {
                    dei_rs_ecm_voltage_init();
                }
                                
                if( dei_rs_ecm_voltage_vars.engine_running )
				{					
					dei_rs_ecm_voltage_vars.first_check = TRUE;      //ignition on, started already
					dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING ); 
				}								
			}	
					
			if( dei_rs_state == dei_rs_start )
			{
				static const TskTimeout cranking_time[] = 
				{
					600 , 800 , 1000 , 1200 , 1400 , 1600 , 1800 , 2000 , 4000 
				}; 
								  
				os_tsk_wait( cranking_time[ dei_feature_get( DEI_FEAT_CRANKING_TIME )-1 ] + get_rs_retry_count() * 50 );
	
				dei_rs_evt_signal( DEI_RS_EVT_ENGINE_RUNNING );								
			}
	
			if( dei_rs_state == dei_rs_running )
			{	
				if( !dei_rs_ecm_voltage_engine_running() )	
				{
					dei_rs_evt_signal( DEI_RS_EVT_LOW_BATT );
#if DEBUG_DEI_RS == 1
          TRACE_DEI_RS("\n\r[DEI_RS_VOLTAGE_ERROR]\n\r");				
#endif					
				}
                else
                {                                                            
                    if( (get_rs_start_successful() == FALSE)  && dei_rs_ecm_voltage_vars.first_check )
                    {
                        set_rs_start_successful( TRUE );
                        dei_rs_ecm_voltage_vars.first_check = FALSE  ;
                    
                        notify_send( NOTIFY_RS_SUCCESSFULLY_STARTED );
												if( brand_id != BRAND_VP_SC )
												  notify_send( NOTIFY_RS_START );
			
			            if( dei_feature_get(DEI_FEAT_PARKING_LIGHT_OUTPUT) != 3 ) 
                        {
                            dei_prk_flash( (dei_feature_get( DEI_FEAT_PARKING_LIGHT_OUTPUT)==1) ? DEI_PRK_RS_CONSTANT : DEI_PRK_RS_PULSED );
                        }

                        dei_rs_auto_defogger();
                    }
                }					                          
			}
		}
	}

	os_tsk_stop();
}
				
//--------------------------------------------------------------------------//

void dei_rs_ecm_voltage_init( void )
{	
	dei_rs_ecm_voltage_vars.voltage_ref  = get_voltage() ; 
				
	dei_rs_ecm_voltage_vars.engine_check_period = 0 ; 	

#if DEBUG_DEI_RS == 1
    TRACE_DEI_RS("\n\r[DEI_RS_VOLTAGE_REF_READ]\r"); 
    dei_rs_ecm_voltage_trace();
#endif	

    dei_rs_ecm_voltage_vars.first_check = FALSE  ;
}

//--------------------------------------------------------------------------//

Boolean dei_rs_ecm_voltage_engine_running( void )
{
	Boolean ret = TRUE ;
    
    if( ++dei_rs_ecm_voltage_vars.engine_check_period == DEI_RS_VOLTAGE_CHECK_TIMEOUT )	
	{			    
    	dei_rs_ecm_voltage_vars.engine_check_period = 0 ;
    	
        dei_rs_ecm_voltage_vars.first_check = TRUE ;
    	    
        if( dei_rs_ecm_voltage_vars.engine_running == TRUE )
    	{	  
#if DEBUG_DEI_RS == 1			
    		TRACE_DEI_RS("\n\r[DEI_RS_VOLTAGE_READ1]\r");
        dei_rs_ecm_voltage_trace();
#endif            
            if( get_voltage() < (dei_rs_ecm_voltage_vars.voltage_ref - VBAT_CHECK_THRESHOLD) )
    		{			
    			set_rs_retry_count( 2 );
                
    			ret = FALSE ; 
    		}
            
    	}
        else
        {
#if DEBUG_DEI_RS == 1				
            TRACE_DEI_RS("\n\r[DEI_RS_VOLTAGE_READ2]\r");
            dei_rs_ecm_voltage_trace();
#endif    
					  ret = (get_voltage() > (dei_rs_ecm_voltage_vars.voltage_ref + 0.3) );
            if(ret == FALSE)
                dei_rs_ecm_voltage_vars.first_check = FALSE  ;
        }
    }
	  
	return ret ;
}

//--------------------------------------------------------------------------//
void dei_rs_ecm_voltage_trace( void )
{
	#define FRACTION_MASK  0xFF
	UInt16 volt_tmp;
	volt_tmp = (get_voltage()*256.0)/1;   //convert to integer
#ifdef PLATFORM_933	
	if(dbgclitx_room() < 200)
    return;
#endif
  
	TRACE("\n\r VOLTAGE[V]: ");
	TRACE("%d.", (volt_tmp>>8));
	volt_tmp = (volt_tmp & FRACTION_MASK) * 10;
	TRACE("%c", (volt_tmp>>8)+'0');
	volt_tmp = (volt_tmp & FRACTION_MASK) * 10;
	TRACE("%c\n\r", (volt_tmp>>8)+'0');
}

//--------------------------------------------------------------------------//

