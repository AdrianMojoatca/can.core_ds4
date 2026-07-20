/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk_tsk.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_prk_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef enum
{

	DEI_PRK_ERROR  , 

	DEI_PRK_NORMAL 

}Dei_Prk_Mode ;

//--------------------------------------------------------------------------//

typedef struct s_Dei_Prk_Table
{
    Dei_Prk_Mode    mode       ; 
    
    TskTimeout      timeout    ;
    
    UInt8           count      ;

    Boolean         delay_start;

}Dei_Prk_Table ;

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean 	dei_prk_error_pattern 	( TskTimeout timeout , UInt8 count , Boolean delay_start );
static Boolean 	dei_prk_normal_pattern	( TskTimeout timeout , UInt8 count , Boolean delay_start );
static void 	dei_prk_pattern_trace   ( Dei_Prk_Pattern pattern ); 

static void     dei_prk_remove_rs_pattern              ( void );
static Boolean  dei_prk_check_for_stop_trigger_pattern ( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static const Dei_Prk_Table dei_prk_pattern_table[] = 
{
    /*DEI_PRK_LOCK             		 */ { DEI_PRK_NORMAL , 500  , 1   , TRUE} ,
    /*DEI_PRK_LOCK_RS                */ { DEI_PRK_NORMAL , 500  , 0   , TRUE} ,
    /*DEI_PRK_UNLOCK_NO_EVENT  		 */ { DEI_PRK_NORMAL , 500  , 2   , TRUE} , 
    /*DEI_PRK_UNLOCK_NO_EVENT_RS	 */ { DEI_PRK_NORMAL , 500  , 1   , TRUE} , 
	/*DEI_PRK_UNLOCK_LOW_BATT 		 */ { DEI_PRK_NORMAL , 500  , 3   , TRUE} , 
    /*DEI_PRK_UNLOCK_TRIGGER_ON		 */ { DEI_PRK_NORMAL , 500  , 4   , TRUE} , 
    /*DEI_PRK_UNLOCK_NPC_ON    		 */ { DEI_PRK_NORMAL , 500  , 5   , TRUE} , 
    /*DEI_PRK_TRIGGER          		 */ { DEI_PRK_NORMAL , 500  , 255 , TRUE} , 
    /*DEI_PRK_WARNING          		 */ { DEI_PRK_NORMAL , 500  , 4   , TRUE} , 
    /*DEI_PRK_MTS_ON           		 */ { DEI_PRK_ERROR  , 250  , 5   , FALSE}, 
    /*DEI_PRK_MTS_OFF          		 */ { DEI_PRK_ERROR  , 500  , 5   , TRUE} , 
    /*DEI_PRK_SMART_ON         		 */ { DEI_PRK_NORMAL , 250  , 5   , TRUE} , 
    /*DEI_PRK_SMART_OFF        		 */ { DEI_PRK_NORMAL , 500  , 5   , TRUE} , 
    /*DEI_PRK_TMR_ON           		 */ { DEI_PRK_NORMAL , 250  , 4   , TRUE} , 
    /*DEI_PRK_TMR_OFF          		 */ { DEI_PRK_NORMAL , 500  , 4   , TRUE} , 
                                                  
    /*DEI_PRK_RS_CONFIRM       		 */ { DEI_PRK_ERROR  , 500  , 1   , FALSE}, 
                                                  
    /*DEI_PRK_RS_CONSTANT      		 */ { DEI_PRK_NORMAL , 0    , 1   , TRUE} , 
    /*DEI_PRK_RS_PULSED        		 */ { DEI_PRK_NORMAL , 2000 , 255 , TRUE} ,     
    /*DEI_PRK_RS_DONE           	 */ { DEI_PRK_NORMAL , 1    , 0   , TRUE} , 
                                                  
    /*DEI_PRK_RS_ERROR_START_BRAKE   */ { DEI_PRK_ERROR  , 250 , 5    , TRUE} , 
    /*DEI_PRK_RS_ERROR_START_HOOD    */ { DEI_PRK_ERROR  , 250 , 6    , TRUE} , 
    /*DEI_PRK_RS_ERROR_START_MTS     */ { DEI_PRK_ERROR  , 250 , 7    , TRUE} , 
    /*DEI_PRK_RS_ERROR_START_PARKING */ { DEI_PRK_ERROR  , 250 , 8    , TRUE} ,
    /*DEI_PRK_RS_ERROR_LOW_TACH      */ { DEI_PRK_ERROR  , 250 , 9    , TRUE} ,

	/*DEI_PRK_FULL_SILENT_ARM        */ { DEI_PRK_NORMAL , 250 , 4    , TRUE} ,
    /*DEI_PRK_TRIGGER_STOP           */ { DEI_PRK_NORMAL , 1   , 0    , TRUE} ,
    /*DEI_PRK_CAR_FINDER       		 */ { DEI_PRK_NORMAL , 500 , 5    , TRUE} ,

	/*DEI_PRK_SENSOR_BYPASS_OFF      */	{ DEI_PRK_NORMAL , 500  , 1   , TRUE} ,
	/*DEI_PRK_SENSOR_BYPASS_WARN     */ { DEI_PRK_NORMAL , 500  , 2   , TRUE} ,
	/*DEI_PRK_SENSOR_BYPASS_FULL     */ { DEI_PRK_NORMAL , 500  , 3   , TRUE}  
};

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_prk_tsk( void )
{
    Dei_Prk_Pattern curr_patt , last_patt = DEI_PRK_NO_PATTERN; 
	
	Boolean         done ; 

    for( ; ; )
    {
        curr_patt = os_pip_remove( dei_prk_pip , 0 ) ; 

		TRACE_ZONE( dei_prk_pattern_trace( curr_patt ); );

        wake_enter_flagged(WAKE_PRK_TSK);
		{
			if( (curr_patt == DEI_PRK_RS_PULSED) || (curr_patt == DEI_PRK_RS_CONSTANT ) )
	        {
	           last_patt = curr_patt ; 
	        }
	
	        if( curr_patt == DEI_PRK_RS_STOP )
	        {
	            last_patt = DEI_PRK_NO_PATTERN ;
                
                dei_prk_remove_rs_pattern();								
	        }

            if( curr_patt == DEI_PRK_TRIGGER )
            {
                if( dei_prk_check_for_stop_trigger_pattern() )
                {
                    last_patt = DEI_PRK_NO_PATTERN   ; 
                    curr_patt = DEI_PRK_TRIGGER_STOP ;

                    TRACE_ZONE( dei_prk_pattern_trace( curr_patt ); );
                }                
            }
	
	        if( dei_prk_pattern_table[curr_patt].mode == DEI_PRK_NORMAL )
			{
				done = dei_prk_normal_pattern( dei_prk_pattern_table[curr_patt].timeout , dei_prk_pattern_table[curr_patt].count , dei_prk_pattern_table[curr_patt].delay_start ) ; 
			}
			else
			{	   
				done = dei_prk_error_pattern( dei_prk_pattern_table[curr_patt].timeout , dei_prk_pattern_table[curr_patt].count , dei_prk_pattern_table[curr_patt].delay_start  ) ; 			
			}
			
			if( done && (last_patt != DEI_PRK_NO_PATTERN) )
	        {
	            os_pip_insert_c( dei_prk_pip, last_patt ) ; 
	        }                
		}
		wake_leave_flagged(WAKE_PRK_TSK);
    };
}
         
        
         
//--------------------------------------------------------------------------//

Boolean dei_prk_error_pattern ( TskTimeout timeout , UInt8 count , Boolean delay_start )
{
    UInt8 i;
    
    dei_prk_output( FALSE ) ; 
        
    if( dei_prk_offset() ) return FALSE ;
	
	if( delay_start )
    {
        os_tsk_wait( 2000 );     
    }

    for( i = 0  ; i < count ; i ++ )
    {
        dei_prk_output( TRUE ) ; 

        os_tsk_wait( timeout );

        dei_prk_output( FALSE ) ;

        os_tsk_wait( timeout );
    };

    return TRUE ; 
}

//--------------------------------------------------------------------------//

Boolean dei_prk_normal_pattern( TskTimeout timeout , UInt8 count, Boolean delay_start )
{
    UInt8 i;
           
    dei_prk_output( FALSE ) ; 
        
    if( dei_prk_offset() ) return FALSE ;   
    
    if( count == 0 )
    {
        if( os_irq_wait( dei_prk_irq , FALSE , timeout ) ) return FALSE ;  

        return TRUE ;
    }      

    for( i = 0  ; i < count ; i ++ )
    {        
        dei_prk_output( TRUE ) ; 

        if( os_irq_wait( dei_prk_irq , FALSE , timeout ) ) return FALSE ;  
        
        dei_prk_output( FALSE ) ;

        if( os_irq_wait( dei_prk_irq , FALSE , (timeout == 2000) ? 1000 : timeout ) ) return FALSE ;        
    };

    return TRUE ; 
}

//--------------------------------------------------------------------------//

void dei_prk_remove_rs_pattern( void )
{
    SInt16 pattern , pattern_buffer[DEI_PRK_PIP_SIZE] ;
    UInt8 i , index = 0 ; 

    ATOMIC
    (
        do
        {
            pattern = os_pip_remove_c( dei_prk_pip ); 
    
            if( (pattern >=0) && (pattern != DEI_PRK_RS_CONSTANT) && (pattern != DEI_PRK_RS_PULSED) )
            {
                pattern_buffer[index ++] = pattern ; 
            }
        
        }while( pattern >= 0 );
    
        for(i = 0 ; i < index ; i ++ )
        {
            os_pip_insert_c( dei_prk_pip , pattern_buffer[i] );
        }
    )
}

//--------------------------------------------------------------------------//

Boolean  dei_prk_check_for_stop_trigger_pattern( void )
{
    SInt16 pattern , pattern_buffer[DEI_PRK_PIP_SIZE] ;
    UInt8 i , index = 0 ; 
    Boolean ret = FALSE ; 

    ATOMIC
    (
        do
        {
            pattern = os_pip_remove_c( dei_prk_pip ); 
    
            if( (pattern >=0) && (pattern != DEI_PRK_TRIGGER_STOP) && (pattern != DEI_PRK_TRIGGER))
            {
                pattern_buffer[index ++] = pattern ; 

                ret = TRUE ; 
            }
        
        }while( pattern >= 0 );
    
        for(i = 0 ; i < index ; i ++ )
        {
            os_pip_insert_c( dei_prk_pip , pattern_buffer[i] );
        }
    )

    return ret ; 
}


//--------------------------------------------------------------------------//


void dei_prk_pattern_trace( Dei_Prk_Pattern pattern )
{
	if( (pattern == DEI_PRK_LOCK) 		    	)TRACE_DEI_PRK("K01");
	
	if( (pattern == DEI_PRK_UNLOCK_NO_EVENT)    )TRACE_DEI_PRK("K02");
	
	if( (pattern == DEI_PRK_UNLOCK_LOW_BATT)    )TRACE_DEI_PRK("K03");
	
	if( (pattern == DEI_PRK_UNLOCK_TRIGGER_ON)  )TRACE_DEI_PRK("K04");
	
	if( (pattern == DEI_PRK_UNLOCK_NPC_ON)      )TRACE_DEI_PRK("K05");
	
	if( (pattern == DEI_PRK_TRIGGER)            )TRACE_DEI_PRK("K06");
	
	if( (pattern == DEI_PRK_WARNING)            )TRACE_DEI_PRK("K07");
	
	if( (pattern == DEI_PRK_MTS_ON)             )TRACE_DEI_PRK("K08");
	
	if( (pattern == DEI_PRK_MTS_OFF)            )TRACE_DEI_PRK("K09");
	
	if( (pattern == DEI_PRK_SMART_ON)           )TRACE_DEI_PRK("K10");
	
	if( (pattern == DEI_PRK_SMART_OFF)          )TRACE_DEI_PRK("K11");
	
	if( (pattern == DEI_PRK_TMR_ON)             )TRACE_DEI_PRK("K12");
	
	if( (pattern == DEI_PRK_TMR_OFF)            )TRACE_DEI_PRK("K13");
	
	if( (pattern == DEI_PRK_RS_CONSTANT)        )TRACE_DEI_PRK("K14");
	
	if( (pattern == DEI_PRK_RS_PULSED)          )TRACE_DEI_PRK("K15");

	if( (pattern == DEI_PRK_RS_CONFIRM)         )TRACE_DEI_PRK("K16");
	
	if( (pattern == DEI_PRK_RS_STOP)            )TRACE_DEI_PRK("K17");
	
	if( (pattern == DEI_PRK_ERROR_START_BRAKE)  )TRACE_DEI_PRK("K18");
	
	if( (pattern == DEI_PRK_ERROR_START_HOOD)   )TRACE_DEI_PRK("K19");
	
	if( (pattern == DEI_PRK_ERROR_START_MTS)    )TRACE_DEI_PRK("K20");
	
	if( (pattern == DEI_PRK_ERROR_START_PARKING))TRACE_DEI_PRK("K21");
	
	if( (pattern == DEI_PRK_ERROR_LOW_TACH     ))TRACE_DEI_PRK("K22");

	if( (pattern == DEI_PRK_FULL_SILENT_ARM     ))TRACE_DEI_PRK("K23");
    
    if( (pattern == DEI_PRK_TRIGGER_STOP        ))TRACE_DEI_PRK("K24");

    if( (pattern == DEI_PRK_CAR_FINDER          ))TRACE_DEI_PRK("K25");
	
	os_tsk_wait(10);    
}

//--------------------------------------------------------------------------//

