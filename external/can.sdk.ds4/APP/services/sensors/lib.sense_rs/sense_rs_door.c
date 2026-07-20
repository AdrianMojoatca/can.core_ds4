/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_door.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
static void sense_rs_door_open_data       ( void );
static void sense_rs_door_close_data      ( void );
void sense_rs_door_cmd_init        ( void );
static void sense_rs_door_cmd_init_timeout( Boolean status );
void sense_rs_door_status_update(UInt32 *door_status, Boolean status, UInt8 from);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean sense_rs_data_old_status = FALSE ;
static UInt32 sense_rs_wire_old_status = 0;
static Boolean sense_rs_wire_old_allsts = FALSE;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
void sense_rs_door(Boolean do_init ,  Boolean status, UInt8  from)
{
	sense_rs_door_status_update(&sense_rs_doors_sts, status, from);
  TRACE_DEI_SENSE_RS("\n\rsese_rs_doors_sts: %04X sense_rs_wire_old_status: %04X\n\r" , sense_rs_doors_sts, sense_rs_wire_old_allsts);	
	if( do_init ) 
    {
			  TRACE_DEI_SENSE_RS("\n\rinit dr: %d sts: %d\n\r" , from, status);
			  set_door_wire_status( sense_rs_doors_sts ? TRUE: FALSE );

        sense_rs_data_old_status = get_door_status();

			  sense_rs_wire_old_status = sense_rs_doors_sts;
			  sense_rs_wire_old_allsts = get_door_status();

        sense_rs_door_cmd_init();
    }
    else
    {
			TRACE_DEI_SENSE_RS("\n\rfrom dr: %d sts: %d\n\r" , from, status);	

			if(sense_rs_doors_sts != sense_rs_wire_old_status)
			{
				set_door_wire_status( sense_rs_doors_sts ? TRUE: FALSE );
				if(get_door_status() != sense_rs_wire_old_allsts)
				{
					cmd_q_rs_insert( get_door_status() ? INPUT_ALL_DOOR_STATUS_OPEN :  INPUT_ALL_DOOR_STATUS_CLOSE , TIMEOUT_Q_INSERT );
					TRACE_DEI_SENSE_RS("\n\r[DOOR_WIRE_%s]\n\r" , get_door_status() ? "OPEN":"CLOSE" );					
					
					sense_rs_wire_old_allsts = get_door_status();
				}
			}
			sense_rs_wire_old_status = sense_rs_doors_sts;
    }
}

/*--------------------------------------------------------------------------*/

void sense_rs_door_open_data( void )
{
	Boolean cur_status = get_door_status() ;
	
	if( !sense_rs_data_old_status && cur_status )
	{
		cmd_q_rs_insert( INPUT_ALL_DOOR_STATUS_OPEN , 100 , 0 ); 
		
		TRACE_DEI_SENSE_RS("\n\r[DOOR_DATA_OPEN]\n\r" );			
	}

	ATOMIC( sense_rs_data_old_status = cur_status );
}

/*--------------------------------------------------------------------------*/

void sense_rs_door_close_data( void )
{
	Boolean cur_status = get_door_status() ;
	
	if( sense_rs_data_old_status && !cur_status )
	{
		cmd_q_rs_insert( INPUT_ALL_DOOR_STATUS_CLOSE , 100 , 0 ); 
		
		TRACE_DEI_SENSE_RS("\n\r[DOOR_DATA_CLOSE]\n\r" );			
	}

	ATOMIC( sense_rs_data_old_status = cur_status );
}

/*--------------------------------------------------------------------------*/

void sense_rs_door_cmd_init( void )
{
    PER_EXEC_ATOMIC
    (
        if( get_door_status() )
        {
            out_q_remove( out_q_main , (Out_Func)sense_rs_door_cmd_init_timeout  , TRUE );        
            out_q_insert( out_q_main , (Out_Func)sense_rs_door_cmd_init_timeout  , SENSE_RS_CMD_INIT_TIMEOUT , 1 , 1 , 1); 
        }
    )
}

/*--------------------------------------------------------------------------*/

void sense_rs_door_cmd_init_timeout( Boolean status )
{
    if( status )
    {
        if( get_door_status() )
        {
            cmd_q_rs_insert( INPUT_ALL_DOOR_STATUS_OPEN , TIMEOUT_Q_INSERT );
    			
    	    TRACE_DEI_SENSE_RS("\n\r[SENSE_RS_DOOR_CMD_INIT]\n\r"  );					
        }
    }
}

/*--------------------------------------------------------------------------*/
void sense_rs_door_status_update(UInt32 *door_status, Boolean status, UInt8  from)
{
		if(status)
			*door_status |= (1<<from);
		else
			*door_status &= ~(1<<from);
}

/*--------------------------------------------------------------------------*/

