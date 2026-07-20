/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk_flash.c 1106 2010-04-27 13:30:39Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_can_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void oem_enable		( void );
static void oem_sense_skip	( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Timeout	timeout_oem ; 
extern Boolean inside_trunk_release;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_oem_disable_init( void )
{
	
	
	timeout_oem = timeout_f_create( 0 , (Exec_Func)oem_enable ); 
	
	cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {
            {INPUT_LOCK1          | INPUT_ON , (Exec_Func)oem_sense_skip },
        	{INPUT_UNLOCK_DRIVER1 | INPUT_ON , (Exec_Func)oem_sense_skip },		
        	{INPUT_UNLOCK_ALL1    | INPUT_ON , (Exec_Func)oem_sense_skip },		
        	{INPUT_LOCK2          | INPUT_ON , (Exec_Func)oem_sense_skip },
        	{INPUT_UNLOCK_DRIVER2 | INPUT_ON , (Exec_Func)oem_sense_skip },		
        	{INPUT_UNLOCK_ALL2    | INPUT_ON , (Exec_Func)oem_sense_skip },		
        	{INPUT_LOCK3          | INPUT_ON , (Exec_Func)oem_sense_skip },
        	{INPUT_UNLOCK_DRIVER3 | INPUT_ON , (Exec_Func)oem_sense_skip },		
        	{INPUT_UNLOCK_ALL3    | INPUT_ON , (Exec_Func)oem_sense_skip },		
        	{INPUT_TRUNK          | INPUT_ON , (Exec_Func)oem_sense_skip }		
        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
    }
}

//--------------------------------------------------------------------------//

void sense_rs_oem_disable( UInt32 ten_msec_ticks )
{
	
	ATOMIC( sense_rs_can_oem_active = FALSE ) ;

	TRACE_SENSE_RS_CAN("\n\r[SENSE_OEM_DISABLE]\n\r");
	
	timeout_update( timeout_oem , ten_msec_ticks*10 );

	timeout_start ( timeout_oem );
}       

//--------------------------------------------------------------------------//

void oem_enable( void )
{
	
	TRACE_SENSE_RS_CAN("\n\r[SENSE_OEM_ENABLE123]\n\r");
	
	ATOMIC( sense_rs_can_oem_active = TRUE ) ;     
}

//--------------------------------------------------------------------------//

void oem_sense_skip( void )
{
	sense_rs_oem_disable( 200 );
}

//--------------------------------------------------------------------------//

