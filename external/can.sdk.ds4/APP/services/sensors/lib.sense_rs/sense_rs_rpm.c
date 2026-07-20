/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_rpm.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
#include "dei_feature.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Per_Exec_Wake sense_rs_rpm_wire( void );
Per_Exec_Wake sense_rs_speed_wire( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
#ifdef PLATFORM_CM800
void sense_rs_rpm_init( void )
{
    #ifndef ANALOGIC_TACHO_DISABLED
        tacho_init();
    #else
    #if ANALOGIC_TACHO_DISABLED == 1
        #warning "Analogical Tacho disabled"
    #else
        tacho_init();
    #endif
    #endif

    
               	
	per_exec_register( (Per_Exec_Func)sense_rs_rpm_wire ) ; 	    
}

#else
void sense_rs_rpm_init( void )
{
	
	
	if( dei_feature_get( DEI_FEAT_TACHOMETER_INPUT ) == 2 )
	{
		speed_init();
		per_exec_register( (Per_Exec_Func)sense_rs_speed_wire );
	}
  else
	{
		tacho_init();
		if(get_rpm_source() == SOURCE_NONE )
		  set_rpm_source (SOURCE_WIRE );

		per_exec_register( (Per_Exec_Func)sense_rs_rpm_wire ) ; 	    
	}
}
#endif
//--------------------------------------------------------------------------//

Per_Exec_Wake sense_rs_rpm_wire( void )
{
	static UInt16 cur_rpm   = 0 ;
    static UInt16 wire_rpm  = 0 ;
	UInt16 tacho, rpm;
	
	tacho =tacho_get();

    if( wire_rpm != tacho )
    {    
        wire_rpm = tacho;
        set_rpm_wire( tacho );
    }
		
		rpm = get_rpm();
#ifdef PLATFORM_933
		
 	#define REF_RPM    (dei_feature_get( DEI_FEAT_RPM_REF_DIV10 ) * 10)

	
	if( (dei_feature_get( DEI_FEAT_TACH_VAL_LEARNED ) == 2)  && (dei_feature_get( DEI_FEAT_CONTROL_DOOR_LOCK_OPTION ) == 3) && (rpm > (REF_RPM + (0.3*REF_RPM))) && (!get_rpm_control_lock_status()))
	{
      cmd_q_insert(INPUT_RPM_CONTROL_LOCK, COMMAND_INSERT_TIMEOUT );      
			set_rpm_control_lock_status( TRUE );
	}
	
	#undef REF_RPM
#endif
		
	#define TIMEOUT_5SEC	( (tmr > T1TC) ? ((tmr-T1TC)>5*1000*1000*72) : ((T1TC-tmr)>5*1000*1000*72) ) 
	
	TRACE_ZONE
	( 
		static UInt32 tmr     = 0 ;		

		if( TIMEOUT_5SEC && (cur_rpm != rpm) )
		{
			tmr = T1TC ; 
				
			TRACE_DEI_SENSE_RS("\n\r[RPM: %d]\n\r" , rpm );			            									
		}
	) 	
	
	#undef TIMEOUT_5SEC	
                                    
    if( cur_rpm && !rpm )
    {
        cmd_q_rs_insert( INPUT_RPM_CLR , TIMEOUT_Q_INSERT , 0 );

        wake_leave_flagged(WAKE_SENSE_RS);
    }
        

    if( !cur_rpm && rpm )
    {
        cmd_q_rs_insert( INPUT_RPM_SET , TIMEOUT_Q_INSERT , 0 );

        wake_enter_flagged(WAKE_SENSE_RS);
    }        
                
    cur_rpm = rpm ;
	
	return PER_EXEC_SLEEP ;		
}

//--------------------------------------------------------------------------//

#ifdef PLATFORM_933
Per_Exec_Wake sense_rs_speed_wire( void )
{
	static UInt16 cur_speed   = 0 ;
    static UInt16 wire_speed  = 0 ;

    if( wire_speed != speed_get() )
    {    
        set_speed_wire( speed_get() );

        wire_speed = speed_get();
    }
		
		
	#define TIMEOUT_5SEC	( (speed_tmr > T1TC) ? ((speed_tmr-T1TC)>5*1000*1000*72) : ((T1TC-speed_tmr)>5*1000*1000*72) ) 
	
	TRACE_ZONE
	( 
		static UInt32 speed_tmr     = 0 ;		

		if( TIMEOUT_5SEC && (cur_speed != get_speed()) )
		{
			speed_tmr = T1TC ; 
				
			TRACE_DEI_SENSE_RS("\n\r[VSS: %d]\n\r" , get_speed() );			            									
		}
	) 	
	
	#undef TIMEOUT_5SEC	
                                    
    if( cur_speed && !get_speed() )
    {
        cmd_q_rs_insert( INPUT_SPEED_CLR , TIMEOUT_Q_INSERT , 0 );

        wake_leave_flagged(WAKE_SENSE_RS);
    }
        

    if( !cur_speed && get_speed() )
    {
        cmd_q_rs_insert( INPUT_SPEED_SET , TIMEOUT_Q_INSERT , 0 );

        wake_enter_flagged(WAKE_SENSE_RS);
    }        
                
    cur_speed = get_speed() ;
	
	return PER_EXEC_SLEEP ;		
}
#endif
//--------------------------------------------------------------------------//

