/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_out_aux4.c 19409 2013-02-21 14:31:27Z martin.bouchard $
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
static void out_aux4_validity_on	  ( void *payload  );
static void out_aux4_validity_off	  ( void  );
static void out_aux4_latch_on   	  ( void *payload  );
static void out_aux4_latch_delayed	  ( Boolean timeout_expired );
static void out_aux4_latch_reset	  ( void  );
static void out_aux4_timed_on		  ( void  );
static void out_aux4_timed_reset( void  );
static void out_aux4_timed_off		  ( void  );
static void out_aux4_timed_off_delayed( Boolean is_expired );
static void out_aux4_ign_reset	( void  );
static void out_aux4_timed_reset( void  );
static void out_aux4_off_delayed( Boolean is_expired );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//---------------------------------------------------------------------------//

void dei_out_aux4_init( void )
{
	switch( dei_feature_get( DEI_FEAT_AUX_4_OUTPUT_TYPE ) )
	{
		case DEI_AUX_OUTPUT_VALIDITY:
		{
			static const Cmd_Array1 cmd_array[] =
						{
								{INPUT_AUX4 | INPUT_ON , (Exec_Func)out_aux4_validity_on  },
					{INPUT_AUX4 		   , (Exec_Func)out_aux4_validity_off },				
						};

						CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
		}
		break;

		case DEI_AUX_OUTPUT_LATCH:
		case DEI_AUX_OUTPUT_LATCH_RESET_IGN:
		{
			static const Cmd_Array1 cmd_array[] =
						{
					{INPUT_AUX4 | INPUT_ON , (Exec_Func)out_aux4_latch_on    },			
					{INPUT_AUX4  		  , (Exec_Func)out_aux4_latch_reset  },			
						};

						CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
		}
		break;
						
		case DEI_AUX_OUTPUT_TIMED:
		{
			static const Cmd_Array1 cmd_array[] =
						{
					{INPUT_AUX4 | INPUT_ON , (Exec_Func)out_aux4_timed_on  },
					{INPUT_AUX4 		   , (Exec_Func)out_aux4_timed_off },				
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

void out_aux4_validity_on( void *payload  )
{
	if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_WITH_ARM    )
	{
		DEI_OUT_AUX4( LOCK_PULSE_LENGTH , 80 , 1 , 1 ) ; 
	}
	else
	if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_WITH_DISARM )
	{
		DEI_OUT_AUX4( LOCK_PULSE_LENGTH , 80 , 1 , 1 ) ; 
	}
	else
	if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_WITH_RS    )
	{
		DEI_OUT_AUX4( 1 , 80 , 1 , 1 ) ; 
	}
	else
	if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_FROM_RF    )	
	{
		if(get_cmd_source_d2d())
		{
			set_cmd_source_d2d(FALSE);
			DEI_OUT_AUX4( 1 , 80 , 1 , 1 ) ;
		}
		else
		  DEI_OUT_AUX4( 1 , 100*30 , 1 , 1 ) ; 
	}							
}

//---------------------------------------------------------------------------//

void out_aux4_validity_off( void  )
{
	DEI_OUT_AUX4_RESET(TRUE); 	
	set_ss_ch6(FALSE);
}

//---------------------------------------------------------------------------//

void out_aux4_latch_on( void *payload  )
{
	PER_EXEC_ATOMIC
	(
		out_q_remove( out_q_main , (Out_Func)out_aux4_latch_delayed , FALSE );

		if( DEI_OUT_PAYLOAD( payload ) & CMD_AUX_WITH_ARM )
		{
			out_q_insert( out_q_main , (Out_Func)out_aux4_latch_delayed , LOCK_PULSE_LENGTH , 1 , 1 , 1 ) ; 
		}
		else
		{
			out_aux4_latch_delayed( TRUE );
		}
	)
}	

//-----------------------------------------------------------------------------//

void out_aux4_latch_delayed( Boolean timeout_expired )
{
	if( timeout_expired )
	{
		set_ss_ch6( TRUE ); //ATOMIC( ss.ch6_status = TRUE );	
					
		out_aux4_control( TRUE );
	}
}

//-----------------------------------------------------------------------------//

void out_aux4_latch_reset(  void  )
{
	PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)out_aux4_latch_delayed , FALSE ) );
		
	set_ss_ch6( FALSE ); //ATOMIC( ss.ch6_status = FALSE );	
		
	out_aux4_control( FALSE );	
}

//-----------------------------------------------------------------------------//

void out_aux4_timed_on( void  )
{
	PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)out_aux4_timed_off_delayed , FALSE ) );
	
	DEI_OUT_AUX4_RESET( FALSE ); 	

	DEI_OUT_AUX4( 2 , FEATURE( DEI_FEAT_AUX_4_TIMED_OUTPUT_SECONDS) * 100 , 1 , 1 ) ; 
}		

//-----------------------------------------------------------------------------//
static void out_aux4_timed_reset( void  )
{
	PER_EXEC_ATOMIC( out_q_remove( out_q_main , (Out_Func)out_aux4_off_delayed , FALSE ) );
	
	set_ss_ch6(FALSE);
	
	out_aux4_control(FALSE);
}

//-----------------------------------------------------------------------------//
static void out_aux4_off_delayed( Boolean is_expired )
{
	if(is_expired)
	{
		set_ss_ch6(TRUE);
		
		out_aux4_control(TRUE);
	}
}

//-----------------------------------------------------------------------------//

void out_aux4_timed_off( void  )
{
	PER_EXEC_ATOMIC
	(
		out_q_remove( out_q_main , (Out_Func)out_aux4_timed_off_delayed , FALSE );

		out_q_insert( out_q_main , (Out_Func)out_aux4_timed_off_delayed , 20 , 1 , 1 , 1 ) ; 
	)	
}

//-----------------------------------------------------------------------------//

void out_aux4_timed_off_delayed( Boolean is_expired )
{
	if( is_expired )
	{
		DEI_OUT_AUX4_RESET( TRUE );
           set_ss_ch6(FALSE); 	
	}
}

//-----------------------------------------------------------------------------//


void out_aux4_control( Boolean state )
{
	prg_out_aux4( state ) ; 								

	set_ss_ch6( state ); //ATOMIC( ss.ch6_status = state ); 
}

//-----------------------------------------------------------------------------//
static void out_aux4_ign_reset	( void  )
{
	DEI_OUT_AUX4_RESET(TRUE);
  set_ss_ch6(FALSE);
}

//-----------------------------------------------------------------------------//		
	
#undef LOCK_PULSE_LENGTH
