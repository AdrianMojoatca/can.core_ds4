/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_bus_off.c 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_usr_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void            bus_off           ( Can_Dev_Id dev_id );
static Per_Exec_Wake   bus_off_check_dev0( void );
static Per_Exec_Wake   bus_off_check_dev1( void );
static void            bus_off_check     ( Can_Dev_Id dev_id );
static void            irq_service_can   (Can_Dev_Id dev_id, Can_Irq can_irq);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void can_usr_bus_off_init( Can_Dev_Id dev_id )
{
    can_set_irq_func(irq_service_can);

    if( dev_id == CAN_DEV_0  )
    {
        PER_EXEC_ATOMIC( per_exec_register( (Per_Exec_Func)bus_off_check_dev0 ) ) ; 
    }
    else
    {
        PER_EXEC_ATOMIC( per_exec_register( (Per_Exec_Func)bus_off_check_dev1 ) ) ; 
    }
}

//--------------------------------------------------------------------------//

void irq_service_can(Can_Dev_Id dev_id, Can_Irq can_irq)
{
    switch (can_irq)
	{
		case CAN_IRQ_BUS_OFF:
		{
			bus_off(dev_id);
		}
		break ;
		 
		case CAN_IRQ_OVERRUN:
		{
			//can_usr_err_add( (dev_id == CAN_DEV_0) ? CAN_USR_ERR_DEV0_OVERRUN : CAN_USR_ERR_DEV1_OVERRUN );
		}
		break ;

		case CAN_IRQ_ERRATA:
		{
			//can_usr_err_add( (dev_id == CAN_DEV_0) ? CAN_USR_ERR_DEV1_ERRATA : CAN_USR_ERR_DEV1_ERRATA );
		}
		break ;
	}
}

//--------------------------------------------------------------------------//

void bus_off( Can_Dev_Id dev_id )
{    
    can_usr[dev_id].bus_off_ind = TRUE ; 
}

//--------------------------------------------------------------------------//

Per_Exec_Wake bus_off_check_dev0( void )
{
    bus_off_check( CAN_DEV_0 );
    
    return PER_EXEC_SLEEP ;  
}

//--------------------------------------------------------------------------//

Per_Exec_Wake bus_off_check_dev1( void )
{
    bus_off_check( CAN_DEV_1 ); 

    return PER_EXEC_SLEEP ; 
}

//--------------------------------------------------------------------------//

void bus_off_check( Can_Dev_Id dev_id )
{
    UInt32 err_cnt ; 

    static Boolean trace_error[ CAN_DEV_COUNT ] = { TRUE , TRUE } ;
	
	Can_IO_Base io_base = dev_id==CAN_DEV_0?CAN_BASE_0:CAN_BASE_1; 
	
	if( can_usr[ dev_id ].bus_off_ind )
    {
        if( trace_error[ dev_id ] )
        {
            trace_error[ dev_id ] = FALSE ; 

            TRACE_CAN_USR("\n\r[ CAN_USR_%s_BUS_OFF_ENTER]\n\r" , (dev_id == CAN_DEV_0) ? "DEV_0" : "DEV_1");

            can_usr_err_add( (dev_id == CAN_DEV_0) ? CAN_USR_ERR_DEV0_BUS_OFF : CAN_USR_ERR_DEV1_BUS_OFF );
        }
        
        (io_base)[CANMOD]&= (~(0x01)) ;
    		
    	err_cnt = ((io_base)[CANGSR]>>24)&((UInt32)(0xFF)) ; 
    		
    	if ( err_cnt == 0 )
    	{
    		//2/Clear BS and ES from CANSR ;
    		(io_base)[CANSR]&=(~((UInt32)(1)<<4)) ; 
    		(io_base)[CANSR]&=(~((UInt32)(1)<<6)) ;		

            can_usr[ dev_id ].bus_off_ind =  FALSE ;
            
            trace_error[ dev_id ] = TRUE ; 
            
            TRACE_CAN_USR("\n\r[ CAN_USR_%s_BUS_OFF_EXIT]\n\r" , (dev_id == CAN_DEV_0) ? "DEV_0" : "DEV_1"); 
    	}
    }
}	

//--------------------------------------------------------------------------//

