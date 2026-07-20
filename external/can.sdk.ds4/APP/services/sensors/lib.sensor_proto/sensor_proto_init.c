/*==========================================================================*/
// $Id: dei_sc_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sensor_proto_private.h"
#include "system_type.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void  sensor_proto_init    ( void ) 
{
    Tsk tsk  ; 
    UInt8 i  ;
    System_Type sys_type;
   
    if(nvfs_rd_s( nvfs_sys_type , &sys_type, sizeof(sys_type) ) != NVFS_OK)
			sys_type = sys_type_assumed;
    
		cmd_register :
    {
        static const Cmd_Array1 cmd_array[] = 
        {                
          {INPUT_ARM        , (Exec_Func)sensor_proto_armdisarm		 },	 
					{INPUT_DISARM     , (Exec_Func)sensor_proto_armdisarm    }, 
			
          {INPUT_OEM_LOCK   , (Exec_Func)sensor_proto_armdisarm		 },	 
          {INPUT_OEM_UNLOCK , (Exec_Func)sensor_proto_armdisarm		 },	 

       };

       if((sys_type & SYS_TYPE_SS) == SYS_TYPE_SS )
         CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
		
		extsensor_init();

		sensor_proto_evt = os_evt_create();

    init_tx_q_pipe :
    {
        Sensor_Proto_Tx_Frame *frame ; 
        
        sensor_proto_tx_q_free = q_create() ; 

        for( i = 0  ; i < TASK_SENSOR_PROTO_TX_Q ; i ++ )
        {
            frame = (Sensor_Proto_Tx_Frame *)os_mem_request( sizeof( struct s_Sensor_Proto_Tx_Frame ) ) ; 
                                   
            q_assign( sensor_proto_tx_q_free , (Q_Msg*)frame ) ;
            
            q_insert( sensor_proto_tx_q_free , (Q_Msg*)frame ) ;
       }
       

       sensor_proto_tx_q_pending = q_create(); 
    }

    tsk = os_tsk_create (sensor_proto_tx_tsk , TASK_SENSOR_PROTO_TX_PRI , TASK_SENSOR_PROTO_TX_STK , 0 , TASK_SENSOR_PROTO_TX_NAME ) ;
    
    os_tsk_start (tsk , 0) ;
}

//--------------------------------------------------------------------------//
