/*==========================================================================*/
// $Id: dei_sc_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define DEI_SC_OPEN_WINDOW         256
#define DEI_SC_RESYNC_WINDOW       32767
#define DEI_SC_RESYNC_COUNT        2
#define DEI_SC_RESYNC_TIMEOUT      30000
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

void  dei_sc_init    ( void ) 
{
    Tsk tsk  ; 
    UInt8 i  ;
    
    if(dei_sc_init_nvfs())
    {
        TRACE_DEI_SC("\n\r[DEI_SC NVFS OK]\n\r");
    }
	else
	{
		TRACE_DEI_SC("\n\r[DEI NOTIFYOTE NVFS ERROR]\n\r");		
	}

    wake_register            ( (Wake_Func           )dei_sc_save_nvfs );
    wdog_v_register_preamble ( (Wdog_v_Preamble_Func)dei_sc_save_nvfs );

    dei_rf_init() ; 

	dei_sc_timeout_pagging = timeout_create( DEI_SC_TIMEOUT_PAGGING );	
	dei_sc_timeout_lock    = timeout_create( DEI_SC_TIMEOUT_LOCK    );	
	
	timeout_stop( dei_sc_timeout_pagging ) ;        
    
    for( i = 0 ; i < DEI_RF_ID_COUNT ; i ++ )
    {
        dei_sc_sync_count[i] = sync_count_create( DEI_SC_OPEN_WINDOW , DEI_SC_RESYNC_WINDOW , DEI_SC_RESYNC_COUNT , DEI_SC_RESYNC_TIMEOUT ) ; 
        
        sync_count_update( dei_sc_sync_count[i] , dei_sc.remote[i].count ) ; 
        
        dei_rf_set_id( i , dei_sc.remote[i].sn ) ;                  
    }

     
    init_tx_q_pipe :
    {
        Dei_Sc_Tx_Frame *frame ; 
        
        dei_sc_tx_q_free = q_create() ; 

        for( i = 0  ; i < TASK_DEI_RF_TX_Q ; i ++ )
        {
            frame = (Dei_Sc_Tx_Frame *)os_mem_request( sizeof( struct s_Dei_Sc_Tx_Frame ) ) ; 
                                   
            q_assign( dei_sc_tx_q_free , (Q_Msg*)frame ) ;
            
            q_insert( dei_sc_tx_q_free , (Q_Msg*)frame ) ;
       }
       

       dei_sc_tx_q_pending = q_create(); 
    }
           

    dei_sc_info_clr();


	dei_sc_notify_init();

	dei_sc_automatic_page_update_init();

	dei_sc_icon_lock_active_zone_report_init();

    tsk = os_tsk_create (dei_sc_rx_tsk , TASK_DEI_RF_RX_PRI , TASK_DEI_RF_RX_STK , 0 , TASK_DEI_RF_RX_NAME ) ;
    
    os_tsk_start (tsk , 0) ;    

    tsk = os_tsk_create (dei_sc_tx_tsk , TASK_DEI_RF_TX_PRI , TASK_DEI_RF_TX_STK , 0 , TASK_DEI_RF_TX_NAME ) ;
    
    os_tsk_start (tsk , 0) ;
  

    
}

//--------------------------------------------------------------------------//

#undef DEI_SC_OPEN_WINDOW         
#undef DEI_SC_RESYNC_WINDOW       
#undef DEI_SC_RESYNC_COUNT        
#undef DEI_SC_RESYNC_TIMEOUT      
