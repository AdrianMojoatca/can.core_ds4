/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk_tsk.c 19279 2013-02-18 07:59:42Z florin.olariu $
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
                                                  
	  /*AS_PRK_RS_OFF       		 	   */ { DEI_PRK_ERROR  , 450  , 2   , TRUE},       //previous status of PLIGHT is ON, delay 2s to start
		/*AS_PRK_RS_MTS_DISABLE    	   */ { DEI_PRK_NORMAL , 500   , 3   , TRUE}, 
		/*AS_PRK_RS_ADJUST    	   		 */ { DEI_PRK_NORMAL , 550   , 1   , TRUE},
    /*AS_PRK_RUNTIME_RESET				 */ { DEI_PRK_NORMAL , 450   , 3   , TRUE},

    /*DEI_PRK_RS_CONSTANT      		 */ { DEI_PRK_NORMAL , 0    , 1   , TRUE} , 
    /*DEI_PRK_RS_PULSED        		 */ { DEI_PRK_NORMAL , 2000 , 255 , TRUE} ,     
    /*DEI_PRK_RS_DONE           	 */ { DEI_PRK_NORMAL , 1    , 0   , TRUE} , 
                                                  
    /*DEI_PRK_RS_ERROR_START_BRAKE   */ { DEI_PRK_ERROR  , 250 , 5    , TRUE} , 
    /*DEI_PRK_RS_ERROR_START_HOOD    */ { DEI_PRK_ERROR  , 250 , 6    , TRUE} , 
    /*DEI_PRK_ERROR_START_TRUNK		   */ { DEI_PRK_ERROR  , 250 , 11   , TRUE} , 
    /*DEI_PRK_RS_ERROR_START_MTS     */ { DEI_PRK_ERROR  , 250 , 7    , TRUE} , 
    /*DEI_PRK_RS_ERROR_START_PARKING */ { DEI_PRK_ERROR  , 250 , 8    , TRUE} ,
    /*DEI_PRK_RS_ERROR_LOW_TACH      */ { DEI_PRK_ERROR  , 250 , 9    , TRUE} ,

		/*DEI_PRK_RS_VALET_ON            */ { DEI_PRK_NORMAL , 250 , 9   , TRUE}  ,
		/*DEI_PRK_RS_VALET_OFF           */ { DEI_PRK_NORMAL , 500 , 9   , TRUE}  ,

				/*AS_PRK_RS_ERROR_TACH_BEFORE_ST */ { DEI_PRK_ERROR  , 250 , 6    , TRUE} ,	//detected tach before start	

	/*DEI_PRK_FULL_SILENT_ARM        */ { DEI_PRK_NORMAL , 250 , 4    , TRUE} ,
    /*DEI_PRK_TRIGGER_STOP           */ { DEI_PRK_NORMAL , 1   , 0    , TRUE} ,
    /*DEI_PRK_CAR_FINDER       		 */ { DEI_PRK_NORMAL , 500 , 10   , TRUE} ,

	/*DEI_PRK_SENSOR_BYPASS_OFF      */	{ DEI_PRK_NORMAL , 500  , 1   , TRUE} ,
	/*DEI_PRK_SENSOR_BYPASS_WARN     */ { DEI_PRK_NORMAL , 500  , 2   , TRUE} ,
	/*DEI_PRK_SENSOR_BYPASS_FULL     */ { DEI_PRK_NORMAL , 500  , 3   , TRUE},  

    /*AS_PRK_PRG_OPTION1             */ { DEI_PRK_ERROR  , 500  , 1   , FALSE},
    /*AS_PRK_PRG_OPTION2             */ { DEI_PRK_ERROR  , 500  , 2   , FALSE},
    /*AS_PRK_PRG_OPTION3             */ { DEI_PRK_ERROR  , 500  , 3   , FALSE},
    /*AS_PRK_PRG_OPTION4             */ { DEI_PRK_ERROR  , 500  , 4   , FALSE},
    /*AS_PRK_PRG_RESET               */ { DEI_PRK_ERROR  , 250  , 8   , TRUE},
    /*AS_PRK_PRG_VTACH_SAVED         */ { DEI_PRK_ERROR  , 500  , 5   , FALSE},
    /*AS_PRK_VALET_ON                */ { DEI_PRK_ERROR  , 250  , 3   , FALSE},
    /*AS_PRK_VALET_OFF               */ { DEI_PRK_ERROR  , 250  , 2   , TRUE},
    /*AS_PRK_AUTOMATIC_GEARBOX       */ { DEI_PRK_ERROR  , 500  , 2   , FALSE},
    /*AS_PRK_MANUAL_GEARBOX          */ { DEI_PRK_ERROR  , 500  , 4   , FALSE},
    /*AS_PRK_REMOTE_PROGRAMMED       */ { DEI_PRK_ERROR  , 250  , 5   , TRUE},
    /*AS_PRK_TACH_PROGRAMMED         */ { DEI_PRK_ERROR  , 250  , 5   , TRUE},
    /*DEI_PRK_SENSOR_SILENT_ARM      */ { DEI_PRK_NORMAL , 250  , 3   , TRUE},
		/*DEI_PRK_OFF										 */ { DEI_PRK_NORMAL , 0  , 0  , FALSE}
};


static const char *dei_prk_trace_table[] =
{
/*DEI_PRK_LOCK                  */ { "PRK_LK"              } ,
/*DEI_PRK_LOCK_RS               */ { "PRK_LK_RS"           } ,
/*DEI_PRK_UNLOCK_NO_EVENT       */ { "PRK_ULK"             } ,
/*DEI_PRK_UNLOCK_NO_EVENT_RS    */ { "PRK_ULK_RS"          } ,
/*DEI_PRK_UNLOCK_LOW_BATT       */ { "PRK_ULK_LBAT"        } ,
/*DEI_PRK_UNLOCK_TRIGGER_ON     */ { "PRK_ULK_TRG"         } ,
/*DEI_PRK_UNLOCK_NPC_ON         */ { "PRK_ULK_NPC"         } ,
/*DEI_PRK_TRIGGER               */ { "PRK_TRG"             } ,
/*DEI_PRK_WARNING               */ { "PRK_WARN"            } ,
/*DEI_PRK_MTS_ON                */ { "PRK_MTS_ON"          } ,
/*DEI_PRK_MTS_OFF               */ { "PRK_MTS_OFF"         } ,
/*DEI_PRK_SMART_ON              */ { "PRK_SM_ON"           } ,
/*DEI_PRK_SMART_OFF             */ { "PRK_SM_OFF"          } ,
/*DEI_PRK_TMR_ON                */ { "PRK_TST_ON"          } ,
/*DEI_PRK_TMR_OFF               */ { "PRK_TST_OFF"         } ,
/*DEI_PRK_RS_CONFIRM            */ { "PRK_RS_CFM"          } ,
/*AS_PRK_RS_OFF                 */ { "PRK_RS_OFF"          } ,
/*AS_PRK_RS_MTS_DISABLE         */ { "PRK_MTS_DIS"         } ,
/*AS_PRK_RS_ADJUST              */ { "PRK_RS_ADJ"          } ,
/*AS_PRK_RUNTIME_RESET          */ { "PRK_RT_RST"          } ,
/*DEI_PRK_RS_CONSTANT           */ { "PRK_RS_CONST"        } ,
/*DEI_PRK_RS_PULSED             */ { "PRK_RS_PULS"         } ,
/*DEI_PRK_RS_DONE               */ { "PRK_RS_DONE"         } ,
/*DEI_PRK_RS_ERROR_START_BRAKE  */ { "PRK_RS_ERR_BRK"      } ,
/*DEI_PRK_RS_ERROR_START_HOOD   */ { "PRK_RS_ERR_HOOD"     } ,
/*DEI_PRK_RS_ERROR_START_MTS    */ { "PRK_RS_ERR_MTS"      } ,
/*DEI_PRK_RS_ERROR_START_PARKING*/ { "PRK_RS_ERR_PRK"      } ,
/*DEI_PRK_RS_ERROR_LOW_TACH     */ { "PRK_RS_ERR_LTACH"    } ,
/*DEI_PRK_RS_VALET_ON           */ { "PRK_RS_VLT_ON"       } ,
/*DEI_PRK_RS_VALET_OFF          */ { "PRK_RS_VLT_OFF"      } ,
/*AS_PRK_RS_ERROR_TACH_BEFORE_ST*/ { "PRK_RS_ERR_TACH_PRE" } ,
/*DEI_PRK_FULL_SILENT_ARM       */ { "PRK_FULL_SIL_ARM"    } ,
/*DEI_PRK_TRIGGER_STOP          */ { "PRK_TRG_STOP"        } ,
/*DEI_PRK_CAR_FINDER            */ { "PRK_CAR_FIND"        } ,
/*DEI_PRK_SENSOR_BYPASS_OFF     */ { "PRK_SENS_BP_OFF"     } ,
/*DEI_PRK_SENSOR_BYPASS_WARN    */ { "PRK_SENS_BP_WARN"    } ,
/*DEI_PRK_SENSOR_BYPASS_FULL    */ { "PRK_SENS_BP_FULL"    } ,
/*AS_PRK_PRG_OPTION1            */ { "PRK_PRG_OPT1"        } ,
/*AS_PRK_PRG_OPTION2            */ { "PRK_PRG_OPT2"        } ,
/*AS_PRK_PRG_OPTION3            */ { "PRK_PRG_OPT3"        } ,
/*AS_PRK_PRG_OPTION4            */ { "PRK_PRG_OPT4"        } ,
/*AS_PRK_PRG_RESET              */ { "PRK_AS_PRG_RST"      } ,
/*AS_PRK_PRG_VTACH_SAVED        */ { "PRK_AS_VTACH_SV"     } ,
/*AS_PRK_VALET_ON               */ { "PRK_AS_VLT_ON"       } ,
/*AS_PRK_VALET_OFF              */ { "PRK_AS_VLT_OFF"      } ,
/*AS_PRK_AUTOMATIC_GEARBOX      */ { "PRK_AS_AUTO_GBOX"    } ,
/*AS_PRK_MANUAL_GEARBOX         */ { "PRK_AS_MANUAL_GBOX"  } ,
/*AS_PRK_REMOTE_PROGRAMMED      */ { "PRK_AS_HHU_PRG"      } ,
/*AS_PRK_TACH_PROGRAMMED        */ { "PRK_AS_TACH_PRG"     } ,
/*DEI_PRK_SENSOR_SILENT_ARM     */ { "PRK_SENS_SIL_ARM"    } ,
/*DEI_PRK_OFF										*/ { "PRK_OFF"						 }
};

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_prk_tsk( void ) {
	
	Dei_Prk_Pattern curr_patt , last_patt = DEI_PRK_NO_PATTERN; 
	Boolean         done ; 

  for( ; ; ) {
		
			curr_patt = (Dei_Prk_Pattern) os_pip_remove( dei_prk_pip , 0 ) ; 


      wake_enter_flagged(WAKE_PRK_TSK);
			{
				//TRACE_ZONE( dei_prk_pattern_trace_deq( curr_patt ); );
				if( (curr_patt == DEI_PRK_RS_PULSED) || (curr_patt == DEI_PRK_RS_CONSTANT ) ) {
					last_patt = curr_patt ; 
					}
		
				if( curr_patt == DEI_PRK_RS_STOP ) {
					last_patt = DEI_PRK_NO_PATTERN ;
					dei_prk_remove_rs_pattern();								
					}

				if( curr_patt == DEI_PRK_TRIGGER ) {
					if( dei_prk_check_for_stop_trigger_pattern() ) {
						last_patt = DEI_PRK_NO_PATTERN   ; 
						curr_patt = DEI_PRK_TRIGGER_STOP ;
						//TRACE_ZONE( dei_prk_pattern_trace( curr_patt ); );
						}                
					}
		
				if( dei_prk_pattern_table[curr_patt].mode == DEI_PRK_NORMAL )	{
					done = dei_prk_normal_pattern( dei_prk_pattern_table[curr_patt].timeout , dei_prk_pattern_table[curr_patt].count , dei_prk_pattern_table[curr_patt].delay_start ) ; 
					}
				else {	   
					done = dei_prk_error_pattern( dei_prk_pattern_table[curr_patt].timeout , dei_prk_pattern_table[curr_patt].count , dei_prk_pattern_table[curr_patt].delay_start  ) ; 			
					}
				
				if( done && (last_patt != DEI_PRK_NO_PATTERN) ) {
					os_pip_insert_c( dei_prk_pip, last_patt ) ; 
					}                
				}
		wake_leave_flagged(WAKE_PRK_TSK);
    }
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

Boolean  dei_prk_check_for_stop_trigger_pattern( void ) {
  SInt16 pattern , pattern_buffer[DEI_PRK_PIP_SIZE] ;
  UInt8 i , index = 0 ; 
  Boolean ret = FALSE ; 

  ATOMIC(
	
		while((pattern=os_pip_remove_c( dei_prk_pip ))>=0) {
			if(pattern != DEI_PRK_TRIGGER_STOP) 
				pattern_buffer[index ++] = pattern ; 
			else 
				ret = TRUE ; 
			}
		for(i = 0 ; i < index ; i ++ ) {
			os_pip_insert_c( dei_prk_pip , pattern_buffer[i] );
			}
		)
	return ret;
	}
	
/*	
	
	
		do {
			pattern = os_pip_remove_c( dei_prk_pip ); 
			
			if((pattern >=0) && (pattern != DEI_PRK_TRIGGER_STOP) && (pattern != DEI_PRK_TRIGGER)) {
				pattern_buffer[index ++] = pattern ; 
				ret = TRUE ; 
				}
			}
		while( pattern >= 0 );
			
		for(i = 0 ; i < index ; i ++ ) {
			os_pip_insert_c( dei_prk_pip , pattern_buffer[i] );
			}
		)

  return ret ; 
	}

*/
//--------------------------------------------------------------------------//


void dei_prk_pattern_trace( Dei_Prk_Pattern pattern)
{
	if(pattern < DEI_PRK_NO_PATTERN)
	{
    TRACE_DEI_PRK(dei_prk_trace_table[(UInt8)pattern]);
	}
	else
	{
		TRACE_DEI_PRK("PRK_UNKNOWN");
	}

  TRACE_DEI_PRK("\n");
	os_tsk_wait(10);    
}

void dei_prk_pattern_trace_deq( Dei_Prk_Pattern pattern)
{
	TRACE("DEQ: ");
	if(pattern < DEI_PRK_NO_PATTERN)
	{
    TRACE_DEI_PRK(dei_prk_trace_table[(UInt8)pattern]);
	}
	else
	{
		TRACE_DEI_PRK("PRK_UNKNOWN");
	}

  TRACE_DEI_PRK("\n");
	os_tsk_wait(10);    
}

void dei_prk_pattern_trace_enq( Dei_Prk_Pattern pattern)
{
	TRACE("ENQ: ");
	if(pattern < DEI_PRK_NO_PATTERN)
	{
    TRACE_DEI_PRK(dei_prk_trace_table[(UInt8)pattern]);
	}
	else
	{
		TRACE_DEI_PRK("PRK_UNKNOWN");
	}

  TRACE_DEI_PRK("\n");
	os_tsk_wait(10);    
}


//--------------------------------------------------------------------------//

