/*==========================================================================*/
// $Id: dei_sc_rx_tsk.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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
Boolean dei_sc_rf_type;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_sc_rx_tsk( void )
{
    #define BT	((Dei_Sc_Button *)button_event)
	
	Dei_Sc_Button* button_event ;
	Cmd_Rf_Pld     btn_pld      ;

	static UInt8 last_button = 0xFF ; 
    dei_sc_rf_type = 0;
    for( ; ; )
    {
        button_event = (Dei_Sc_Button *)dei_rf_rx( 0 ) ; 

        wake_enter_flagged(WAKE_SC_RX_TSK);
        {        
            if( dei_sc_mode == DEI_SC_MODE_PROGRAMMING )
            {
                if( dei_sc_check_frame_programming( (Dei_Rf_Frame *) button_event ) )
                {                                                            
                    //build the button payload
										
					btn_pld.bits.pld 		= 0 ; 
					btn_pld.bits.id  		= BT->id ;
					btn_pld.bits.batt_low	= 0 ;

					cmd_q_rs_insert( (Cmd)DEI_SC_BUTTON_PROGRAMMING_DONE + INPUT_RF_ENTRY + 1 , 100 , btn_pld.pld );

                    dei_sc_tx( NULL , DEI_SC_PROGRAMMING_TX , 100 );                                                                

                    dei_sc_save_nvfs(s_SLEEP);  //CM800 needs to save remote id in nvfs
                }
                                
                dei_rf_mode( DEI_RF_MODE_PROGRAMMING_RX ) ;                                  
            }
            else
            {
                decode_normal_frame:
								
				if( dei_sc_check_frame_normal( (Dei_Rf_Frame *) button_event ) )
                {                    
                    dei_sc_tx_flush();
                   					
					timeout_update ( dei_sc_timeout_pagging , DEI_SC_TIMEOUT_PAGGING );						
					timeout_start  ( dei_sc_timeout_pagging );
					
					if( (BT->button == DEI_SC_BUTTON_LOCK) || (BT->button == DEI_SC_BUTTON_UNLOCK))
					{
						dei_sc_is_silent = FALSE ; 
					}

					if( (BT->button == DEI_SC_BUTTON_SILENT_LOCK) || (BT->button == DEI_SC_BUTTON_SILENT_UNLOCK))
					{
						dei_sc_is_silent = TRUE ; 
					}
																				
					
					//build the button 
					btn_pld.bits.pld 	 = BT->pl 	   ; 
					btn_pld.bits.id  	 = BT->id 	   ;
					btn_pld.bits.batt_low= BT->batt_low;
					dei_sc_rf_type       = BT->rf_type; // to denote if 1 way or 2 way button
					
					if( last_button != BT->button )
					{
						if( last_button != 0xFF )
						{
							cmd_q_rs_insert( (Cmd)((last_button+INPUT_RF_ENTRY + 1)|INPUT_RF_RELEASE) , 100 , btn_pld.pld );

							TRACE_DEI_SC("\n\r[LIB.DEI_SC BUTTON_RELEASE_NEW_CMD %d]\n\r" , last_button);
						}
					}

					last_button = BT->button ; 
															
					#define BUTTON_CMD	(BT->press_type ? (Cmd)((BT->button + INPUT_RF_ENTRY + 1) | INPUT_RF_EXT) :  (Cmd)(BT->button + INPUT_RF_ENTRY + 1))
					
					cmd_q_rs_insert( BUTTON_CMD , 100 , btn_pld.pld );	
					
					#undef BUTTON_CMD														                								
				}
                
                dei_rf_mode( DEI_RF_MODE_NORMAL_RX ) ; 
				
				
				button_event = (Dei_Sc_Button *)dei_rf_rx( 800 ) ;                

				if( button_event ) goto decode_normal_frame ; 

				if( last_button != 0xFF )
                {
                    cmd_q_rs_insert( (Cmd)((last_button+INPUT_RF_ENTRY + 1)|INPUT_RF_RELEASE) , 100 , btn_pld.pld );
								
				    TRACE_DEI_SC("\n\r[LIB.DEI_SC BUTTON_RELEASE_TIMEOUT %d ]\n\r" , last_button);
                }

				last_button = 0xFF ; 
           }                
        }
        wake_leave_flagged(WAKE_SC_RX_TSK);
    }
	
	#undef BT                             
}

//--------------------------------------------------------------------------//

