/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_handbrake.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
static void sense_rs_handbrake_cmd_init        ( void );
static void sense_rs_handbrake_cmd_init_timeout( Boolean status );
void sense_rs_handbrake_status_update(UInt32 *handbrake_status, Boolean status, UInt8 from);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static UInt32 sense_rs_handbrake_wire_old_sts=0;
static Boolean sense_rs_handbrake_wire_old_allsts=FALSE;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void sense_rs_handbrake( Boolean do_init  , Boolean status, UInt8 from)
{	
	sense_rs_handbrake_status_update(&sense_rs_handbrakes_sts, status, from);	
	if( do_init )
    {
			  set_handbrake_wire_status( sense_rs_handbrakes_sts ? TRUE : FALSE ) ;
			  sense_rs_handbrake_wire_old_sts = sense_rs_handbrakes_sts;
			  sense_rs_handbrake_wire_old_allsts = get_handbrake_status();			
        
        sense_rs_handbrake_cmd_init();	    
    }
    else
    {    
			if(sense_rs_handbrakes_sts != sense_rs_handbrake_wire_old_sts)
			{
				set_handbrake_wire_status(sense_rs_handbrakes_sts ? TRUE : FALSE);
				if(get_handbrake_status() != sense_rs_handbrake_wire_old_allsts)
				{
					cmd_q_rs_insert( get_handbrake_status() ? INPUT_HANDBRAKE_STATUS_ON :  INPUT_HANDBRAKE_STATUS_OFF , TIMEOUT_Q_INSERT );
					sense_rs_handbrake_wire_old_allsts = get_handbrake_status();
				}
			}
			sense_rs_handbrake_wire_old_sts = sense_rs_handbrakes_sts;			
    }
}

/*--------------------------------------------------------------------------*/

void sense_rs_handbrake_cmd_init( void )
{
    PER_EXEC_ATOMIC
    (
        if( get_handbrake_status() )
        {
            out_q_remove( out_q_main , (Out_Func)sense_rs_handbrake_cmd_init_timeout  , TRUE );        
            out_q_insert( out_q_main , (Out_Func)sense_rs_handbrake_cmd_init_timeout  , SENSE_RS_CMD_INIT_TIMEOUT , 1 , 1 , 1); 
        }
    )
}

/*--------------------------------------------------------------------------*/

void sense_rs_handbrake_cmd_init_timeout( Boolean status )
{
    if( status )
    {
        if( get_handbrake_status() )
        {
            cmd_q_rs_insert( INPUT_HANDBRAKE_STATUS_ON , TIMEOUT_Q_INSERT );
    			
    	    TRACE_DEI_SENSE_RS("\n\r[SENSE_RS_HANDBRAKE_CMD_INIT]\n\r" );
        }
    }
}

/*--------------------------------------------------------------------------*/
void sense_rs_handbrake_status_update(UInt32 *handbrake_status, Boolean status, UInt8 from)
{
		if(status)
			*handbrake_status |= (1<<from);
		else
			*handbrake_status &= ~(1<<from);
}

/*--------------------------------------------------------------------------*/
