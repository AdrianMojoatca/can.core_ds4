/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_push_board.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Per_Exec_Wake sense_rs_push_board_wire( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Ied  sense_rs_ied_push_board ; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_push_board_init( void )
{
    sense_rs_ied_push_board = ied_create( SENSE_RS_PIN_PUSH_BOARD , SENSE_RS_PUSH_BOARD_DEBOUNCE , SENSE_RS_PUSH_BOARD_DEBOUNCE );

	set_push_board_status( SENSE_RS_PIN_PUSH_BOARD );

	per_exec_register( (Per_Exec_Func)sense_rs_push_board_wire ) ; 
}

//--------------------------------------------------------------------------//

Per_Exec_Wake sense_rs_push_board_wire( void )
{
	static Boolean is_pressed  = FALSE ;
	static Boolean is_extended = FALSE ;
	static UInt32   time       = 0 ;

	switch( ied_detect( sense_rs_ied_push_board , SENSE_RS_PIN_PUSH_BOARD ) )
	{
		case +1 :
		{
			cmd_q_rs_insert( INPUT_PUSH_PRESS , 100 ) ; 
			
			TRACE_DEI_SENSE_RS("\n\r[PUSH_BOARD_PRESS]\n\r") ; 

			set_push_board_status( TRUE );
			
			is_pressed  = TRUE ;
			is_extended = FALSE; 
			
			time = 0 ; 
		
			return PER_EXEC_WAKE ;
		}
		
		
		case -1 :
		{
			cmd_q_rs_insert( is_extended ? INPUT_EXT_PUSH_RELEASE : INPUT_PUSH_RELEASE , TIMEOUT_Q_INSERT ) ; 
			
			TRACE_ZONE
			(
				if( is_extended )
				{						
					TRACE_DEI_SENSE_RS("\n\r[PUSH_BOARD_EXT_RELEASE]\n\r") ; 
				}
				else
				{
					TRACE_DEI_SENSE_RS("\n\r[PUSH_BOARD_RELEASE]\n\r" ) ; 
				}
			)

										
			is_extended = FALSE ; 
			
			is_pressed = FALSE ; 

			set_push_board_status( FALSE );

			time = 0 ; 
		
			return PER_EXEC_SLEEP ; 
		}
		

		default :
		{
			if( is_pressed ) 
			{
				if( ++time == 10*17 )//1.7 SECONDS
				{
					cmd_q_rs_insert( INPUT_EXT_PUSH_PRESS, TIMEOUT_Q_INSERT ) ; 

					TRACE_DEI_SENSE_RS("\n\r[PUSH_BOARD_EXT_PRESS]\n\r") ; 

					is_extended = TRUE ;
				}

                return PER_EXEC_WAKE ;
			}
		}
		break;
	}
		 	
	
	return PER_EXEC_SLEEP ;
}

//--------------------------------------------------------------------------//

