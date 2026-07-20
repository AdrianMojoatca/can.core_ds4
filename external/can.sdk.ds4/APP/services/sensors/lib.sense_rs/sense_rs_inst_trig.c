/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_glowplug.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
static void sense_rs_extrig_cmd_init        ( void );
static void sense_rs_extrig_cmd_init_timeout( Boolean status );
void sense_rs_extrig_status_update(UInt32 *extrig_status, Boolean status, UInt8 from);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static UInt32 sense_rs_extrig_wire_old_sts=0;
static Boolean sense_rs_extrig_wire_old_allsts=FALSE;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/     	

void sense_rs_inst_trig( Boolean do_init , Boolean status, UInt8 from)
{	
	sense_rs_extrig_status_update(&sense_rs_extrigs_sts, status, from);
	if( do_init )
    {
			 set_inst_trig_wire_status( sense_rs_extrigs_sts ? TRUE : FALSE ) ;
			 sense_rs_extrig_wire_old_sts = sense_rs_extrigs_sts;
			 sense_rs_extrig_wire_old_allsts = get_inst_trig_wire_status();
			
			 sense_rs_extrig_cmd_init();
    }
    else
    { 
			if(sense_rs_extrigs_sts != sense_rs_extrig_wire_old_sts)
			{
				set_inst_trig_wire_status(sense_rs_extrigs_sts ? TRUE : FALSE);
				if(get_inst_trig_wire_status() != sense_rs_extrig_wire_old_allsts)
				{
					cmd_q_rs_insert( get_inst_trig_wire_status() ? INPUT_INST_TRIG_STATUS_ON :  INPUT_INST_TRIG_STATUS_OFF , TIMEOUT_Q_INSERT );
					sense_rs_extrig_wire_old_allsts = get_inst_trig_wire_status();
				}
			}
			sense_rs_extrig_wire_old_sts = sense_rs_extrigs_sts;
    }
}

/*--------------------------------------------------------------------------*/     	

void sense_rs_extrig_cmd_init( void )
{
    PER_EXEC_ATOMIC
    (
        if( get_inst_trig_wire_status() )
        {
            out_q_remove( out_q_main , (Out_Func)sense_rs_extrig_cmd_init_timeout  , TRUE );        
            out_q_insert( out_q_main , (Out_Func)sense_rs_extrig_cmd_init_timeout  , SENSE_RS_CMD_INIT_TIMEOUT , 1 , 1 , 1); 
        }
    )
}

/*--------------------------------------------------------------------------*/

void sense_rs_extrig_cmd_init_timeout( Boolean status )
{
    if( status )
    {
         if( get_inst_trig_wire_status() )
         {
            cmd_q_rs_insert( INPUT_INST_TRIG_STATUS_ON , TIMEOUT_Q_INSERT );
    			
    	    TRACE_DEI_SENSE_RS("\n\r[SENSE_RS_EXT_TRIG_CMD_INIT]\n\r" );					
         }
    }
}

/*--------------------------------------------------------------------------*/     	
void sense_rs_extrig_status_update(UInt32 *extrig_status, Boolean status, UInt8 from)
{
		if(status)
			*extrig_status |= (1<<from);
		else
			*extrig_status &= ~(1<<from);
}

/*--------------------------------------------------------------------------*/     	

