/*==========================================================================*/
// $Id: dei_sc_tx_tsk.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
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

void dei_sc_tx_tsk( void )
{
    Dei_Sc_Tx_Frame *frame ; 
    
    for( ; ; )
    {
        frame = (Dei_Sc_Tx_Frame *) q_remove( dei_sc_tx_q_pending , 0 ) ;   
		
        wake_enter_flagged(WAKE_SC_TX_TSK) ;  		       
        {			
			switch( frame->mode )
            {
                case DEI_SC_NORMAL_TX :
                {
					if(dei_sc_ngrf_mode != DEI_SC_NGRF_MODE_NORMAL)
					{
						dei_rf_mode( DEI_RF_MODE_NORMAL_TX , frame->payload , dei_sc_current_sn ) ; 

                        VIRTUAL_WATCH( dei_rf_tx( 0 ) );	

                        dei_sc_ngrf_mode = DEI_SC_NGRF_MODE_NORMAL;
					}
					else if(dei_sc_mode == DEI_SC_MODE_NORMAL)
                    {
                        if(!DEI_SC_PAGGING())
                        {
                            // do not send feedback in case of 1 way remote
                            if(dei_sc_rf_type)
                            {
								dei_rf_mode( DEI_RF_MODE_NORMAL_TX , frame->payload , dei_sc_current_sn ) ; 

                                VIRTUAL_WATCH( dei_rf_tx( 0 ) );	
							}
    					}
    					else
    					{
    						dei_rf_mode( DEI_RF_MODE_PAGGING_TX , frame->payload , dei_sc.car_sn ) ;                     

                            VIRTUAL_WATCH_WITH_CUSTOM_TIMEOUT( dei_rf_tx( 0 ) , 30);	
    					}
                    }
                }
                break;

                case DEI_SC_PAGGING_TX :
                {
                    if( dei_sc_mode == DEI_SC_MODE_NORMAL )
                    {
                        dei_rf_mode( DEI_RF_MODE_PAGGING_TX , frame->payload , dei_sc.car_sn ) ;                     

                        VIRTUAL_WATCH_WITH_CUSTOM_TIMEOUT( dei_rf_tx( 0 ) , 30);	
                    }
                }
                break;
                
                case DEI_SC_PROGRAMMING_TX :
                {
                    if( dei_sc_mode == DEI_SC_MODE_PROGRAMMING )
                    {
                        dei_rf_mode( DEI_RF_MODE_PROGRAMMING_TX , dei_sc_current_sn&0x0000FFFF , dei_sc.car_sn ) ;                                                                                                                        

                        VIRTUAL_WATCH( dei_rf_tx( 0 ));	
                    }
                }
                break ;
            }
                        
            q_return( (Q_Msg*)frame );              
        }
        wake_leave_flagged(WAKE_SC_TX_TSK) ;
    };
}

//--------------------------------------------------------------------------//

