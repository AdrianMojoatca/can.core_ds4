/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_out_aux1.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_out_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define LOCK_PULSE_LENGTH		 dei_out_lock_pulse_length_table[FEATURE(DEI_FEAT_DOORLOCK_OUTPUT_DURATION)-1]
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_out_aux1_validity_on	  ( void *payload );
static void dei_out_aux1_validity_off	  ( void *payload );
static void dei_out_aux1_latch_on   	  ( void *payload );
static void dei_out_aux1_latch_delayed	  ( Boolean timeout_expired );
static void dei_out_aux1_latch_reset	  ( void *payload );
static void dei_out_aux1_timed_on		  ( void *payload );
static void dei_out_aux1_timed_off		  ( void *payload );
static void dei_out_aux1_timed_off_delayed( Boolean is_expired );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//---------------------------------------------------------------------------//

void dei_out_aux1_init( void )
{
	switch( dei_feature_get( DEI_FEAT_AUX_1_OUTPUT_TYPE ) )
	{
		case DEI_AUX_OUTPUT_VALIDITY:
		{
			static const Cmd_Array1 cmd_array[] = 
            {
                {INPUT_AUX1 | INPUT_ON , (Exec_Func)dei_out_aux1_validity_on  },
    			{INPUT_AUX1 		   , (Exec_Func)dei_out_aux1_validity_off }
            };

            CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
		}
		break;

		case DEI_AUX_OUTPUT_LATCH:
		case DEI_AUX_OUTPUT_LATCH_RESET_IGN:
		{
			static const Cmd_Array1 cmd_array[] = 
            {            
                {INPUT_AUX1 | INPUT_ON , (Exec_Func)dei_out_aux1_latch_on    },
    			{INPUT_AUX1  		  , (Exec_Func)dei_out_aux1_latch_reset  },
            };

            CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
		}
		break;
				
		case DEI_AUX_OUTPUT_TIMED:
		{
			static const Cmd_Array1 cmd_array[] = 
            {                    
                {INPUT_AUX1 | INPUT_ON , (Exec_Func)dei_out_aux1_timed_on     }, 
    			{INPUT_AUX1 		   , (Exec_Func)dei_out_aux1_timed_off    }, 				
            };

            CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
		}
		break;			
			
		case DEI_AUX_OUTPUT_OFF:
		{
			
		}
		break;			

		default :
		{
			break ;
		}
	}
}		
	

//---------------------------------------------------------------------------//

void dei_out_aux1_validity_on( void *payload )
{
	if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_WITH_ARM    )
	{
		DEI_OUT_AUX1( LOCK_PULSE_LENGTH , 80 , 1 , 1 ) ; 
	}
	else
	if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_WITH_DISARM )
	{
		DEI_OUT_AUX1( LOCK_PULSE_LENGTH , 80 , 1 , 1 ) ; 
	}
	else
	if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_WITH_RS    )
	{
		DEI_OUT_AUX1( 1 , 80 , 1 , 1 ) ; 
	}
	else
	if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_FROM_RF    )	
	{
		DEI_OUT_AUX1( 1 , 100*30 , 1 , 1 ) ; 
	}							
}

//---------------------------------------------------------------------------//

void dei_out_aux1_validity_off( void *pld )
{
	DEI_OUT_AUX1_RESET(TRUE); 	
}

//---------------------------------------------------------------------------//

void dei_out_aux1_latch_on( void *payload )
{
	PER_EXEC_ATOMIC
	(
		out_q_remove( out_q_main , (Out_Func)dei_out_aux1_latch_delayed , FALSE );

		if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_WITH_ARM )
		{
			out_q_insert( out_q_main , (Out_Func)dei_out_aux1_latch_delayed , LOCK_PULSE_LENGTH , 1 , 1 , 1 ) ; 
		}
		else
		{
			dei_out_aux1_latch_delayed( TRUE );
		}
	)
}	

//-----------------------------------------------------------------------------//

void dei_out_aux1_latch_delayed( Boolean timeout_expired )
{
	if( timeout_expired )
	{
		set_ss_ch3(TRUE); //ATOMIC( ss.ch3_status = TRUE );	
		
		dei_out_aux1_control( TRUE );
	}
}

//-----------------------------------------------------------------------------//

void dei_out_aux1_latch_reset(  void *payload )
{
	PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)dei_out_aux1_latch_delayed , FALSE ) );

	set_ss_ch3(FALSE); //ATOMIC( ss.ch3_status = FALSE );
		
	dei_out_aux1_control( FALSE );	
}

//-----------------------------------------------------------------------------//

void dei_out_aux1_timed_on( void *payload )
{
	PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)dei_out_aux1_timed_off_delayed , FALSE ) );
		
	DEI_OUT_AUX1_RESET( FALSE ); 	

	DEI_OUT_AUX1( 2 , FEATURE( DEI_FEAT_AUX_1_TIMED_OUTPUT_SECONDS) * 100 , 1 , 1 ) ; 
}		

//-----------------------------------------------------------------------------//

void dei_out_aux1_timed_off( void *payload )
{
	PER_EXEC_ATOMIC
	(
		out_q_remove( out_q_main , (Out_Func)dei_out_aux1_timed_off_delayed , FALSE );

		out_q_insert( out_q_main , (Out_Func)dei_out_aux1_timed_off_delayed , 20 , 1 , 1 , 1 ) ; 
	)	
}

//-----------------------------------------------------------------------------//

void dei_out_aux1_timed_off_delayed( Boolean is_expired )
{
	if( is_expired )
	{
		DEI_OUT_AUX1_RESET( TRUE ); 	
	}
}

//-----------------------------------------------------------------------------//

void dei_out_aux1_control( Boolean state )
{
	prg_out_aux1( state ) ; 
	
	set_ss_ch3(state);	//ATOMIC( ss.ch3_status = state ); 
}
	
//-----------------------------------------------------------------------------//		

#undef LOCK_PULSE_LENGTH	
