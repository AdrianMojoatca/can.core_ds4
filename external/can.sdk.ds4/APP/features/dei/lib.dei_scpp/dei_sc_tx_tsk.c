/*==========================================================================*/
// $Id: dei_sc_tx_tsk.c 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
#include "d2d.h"
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
    UInt8 tx_hhu_no;
    UInt8 tx_pld[8];

    for( ; ; )
    {
        frame = (Dei_Sc_Tx_Frame *) q_remove( dei_sc_tx_q_pending , 0 ) ;   
        
        wake_enter_flagged(WAKE_SC_TX_TSK);
        {
            tx_hhu_no = 0 ;
					
					  memcpy(tx_pld, &frame->payload, 4); //tx_pld = frame->payload;
            switch( frame->mode )
            {
										case DEI_SC_STANDBY_TX:
											tx_hhu_no |= (1<<3); // set STANDBYMASK
											// fall through
                    case DEI_SC_NORMAL_TX :
                    {
                        if(tx_pld[0] == 0xD4 && (dei_led_pattern & 0xFFFF0000))
												{
													tx_pld[D2D_SCPP_PLD_LEN] = (dei_led_pattern >>16) & 0x000000FF;
													tx_pld[D2D_SCPP_PLD_LEN+1] = (dei_led_pattern >>24) & 0x000000FF;
													intl_port_scpp_feedback(tx_hhu_no , tx_pld, D2D_SCPP_PLD_LEN+2);
												}											
												else
                          intl_port_scpp_feedback(tx_hhu_no , tx_pld, D2D_SCPP_PLD_LEN);
                    }
                    break;
                    case DEI_SC_PAGGING_TX :
                    {
                        tx_hhu_no = 0xFF ;
                        intl_port_scpp_feedback(tx_hhu_no , tx_pld, D2D_SCPP_PLD_LEN);
                    }
                    break;
										
										case DEI_SC_PROGRAMMING_TX:
										if(dei_sc_mode == DEI_SC_MODE_PROGRAMMING)
										{
											//tx_pld = 0x000000D6;
											frame->payload = 0x000000D6;
											memcpy(tx_pld, &frame->payload, 4);
											intl_port_scpp_feedback(tx_hhu_no , tx_pld, D2D_SCPP_PLD_LEN);
										}
										break;
            }
                
            q_return( (Q_Msg*)frame );
        }
        wake_leave_flagged(WAKE_SC_TX_TSK) ;
    };
}
