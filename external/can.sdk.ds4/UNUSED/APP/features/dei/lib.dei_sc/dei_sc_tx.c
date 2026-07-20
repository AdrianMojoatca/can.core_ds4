/*==========================================================================*/
// $Id: dei_sc_tx.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Sc_Ack_Table 
{
    UInt8 cmd  ; 
 
    DEI_SC_PAYLOAD pld ; 

}Dei_Sc_Ack_Table ;         


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


static const Dei_Sc_Ack_Table dei_sc_ack_table[] = 
{
    /*DEI_SC_ACK_LOCK	                        */	 { 0xA0	,	DEI_SC_PLD001     },
    /*DEI_SC_ACK_LOCKED_VALET_ON	            */	 { 0xA1	,	DEI_SC_PLD001     },
    /*DEI_SC_ACK_SILENT_ARMED	                */	 { 0xA2	,	DEI_SC_PLD001     },
    /*DEI_SC_ACK_ARM_WITH_ACTIVE_ZONE_REPORT	*/	 { 0xA3	,	DEI_SC_PLD004     },
    /*DEI_SC_ACK_DISARM_UNLOCK	                */	 { 0xA4	,	DEI_SC_PLD002     },
    /*DEI_SC_ACK_UNLOCKED_VALET_ON	            */	 { 0xA5	,	DEI_SC_PLD002     },
    /*DEI_SC_ACK_SILENT_DISARMED	            */	 { 0xA6	,	DEI_SC_PLD002     },
    /*DEI_SC_ACK_TRUNK_RELEASE	                */	 { 0xA7	,	DEI_SC_PLD011     },
    /*DEI_SC_ACK_PANIC_ON	                    */	 { 0xA8	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_NOT_USED_A9	                */	 { 0xA9	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_SENSOR_BYPASS_CONFIRM	        */	 { 0xAA	,	DEI_SC_PLD006     },
    /*DEI_SC_ACK_FULL_BYPASS_CONFIRM	        */	 { 0xAB	,	DEI_SC_PLD004     },
    /*DEI_SC_ACK_SENSOR_BYPASS_OFF	            */	 { 0xAC	,	DEI_SC_PLD004     },
    /*DEI_SC_ACK_UPDATE_PAGE	                */	 { 0xAD	,	DEI_SC_PLD003     },
    /*DEI_SC_ACK_AUX1_ON	                    */	 { 0xAE	,	DEI_SC_PLD011     },
    /*DEI_SC_ACK_AUX1_OFF	                    */	 { 0xAF	,	DEI_SC_PLD011     },
    /*DEI_SC_ACK_AUX2_ON	                    */	 { 0xB0	,	DEI_SC_PLD011     },
    /*DEI_SC_ACK_AUX2_OFF	                    */	 { 0xB1	,	DEI_SC_PLD011     },
    /*DEI_SC_ACK_AUX3_ON	                    */	 { 0xB2	,	DEI_SC_PLD011     },
    /*DEI_SC_ACK_AUX3_OFF	                    */	 { 0xB3	,	DEI_SC_PLD011     },
    /*DEI_SC_ACK_AUX4_ON	                    */	 { 0xB4	,	DEI_SC_PLD011     },
    /*DEI_SC_ACK_AUX4_OFF	                    */	 { 0xB5	,	DEI_SC_PLD011     },
    /*DEI_SC_ACK_VALET_ON	                    */	 { 0xB6	,	DEI_SC_PLD003     },
    /*DEI_SC_ACK_VALET_OFF	                    */	 { 0xB7	,	DEI_SC_PLD003     },
    /*DEI_SC_ACK_CAR_FINDER_ON	                */	 { 0xB8	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_ADJUST_SENSOR_ENTER	        */	 { 0xB9	,	DEI_SC_PLD008     },
    /*DEI_SC_ACK_ADJUST_TEMP_ENTER	            */	 { 0xB9	,	DEI_SC_PLD009     },
    /*DEI_SC_ACK_ADJUST_START_COUNT_ENTER	    */	 { 0xB9	,	DEI_SC_PLD010     },
    /*DEI_SC_ACK_SENSOR_LEVEL_INCREASE	        */	 { 0xBA	,	DEI_SC_PLD008     },
    /*DEI_SC_ACK_SENSOR_LEVEL_DECREASE	        */	 { 0xBB	,	DEI_SC_PLD008     },
    /*DEI_SC_ACK_SENSOR_LEVEL_RESET	            */	 { 0xBC	,	DEI_SC_PLD008     },
    /*DEI_SC_ACK_TEMP_LEVEL_RESET	            */	 { 0xBC	,	DEI_SC_PLD009     },
    /*DEI_SC_ACK_START_COUNT_LEVEL_RESET	    */	 { 0xBC	,	DEI_SC_PLD010     },
    /*DEI_SC_ACK_ALARM_WARNING_EVENT	        */	 { 0xBD	,	DEI_SC_PLD005     },
    /*DEI_SC_ACK_ALARM_TRIGGER_EVENT	        */	 { 0xBE	,	DEI_SC_PLD005     },
    /*DEI_SC_ACK_PROGRAMMING_MODE_EXIT	        */	 { 0xBF	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_NOTIFYOTE_START_ON	            */	 { 0xC0	,	DEI_SC_PLD003     },
    /*DEI_SC_ACK_NOTIFYOTE_START_OFF	         */	 { 0xC1	,	DEI_SC_PLD003     },
    /*DEI_SC_ACK_ERROR_MSG	                    */	 { 0xC2	,	DEI_SC_PLD012     },
    /*DEI_SC_ACK_TIMER_MODE_ON	                */	 { 0xC3	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_TIMER_MODE_OFF	                */	 { 0xC4	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_SMART_START_ON	                */	 { 0xC5	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_SMART_START_OFF	            */	 { 0xC6	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_RUNTIME_CONFIRM 	            */	 { 0xC7	,	DEI_SC_PLD001     },
    /*DEI_SC_ACK_REAR_DEFOGER_ON	            */	 { 0xC8	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_REAR_DEFOGER_OFF               */	 { 0xC9	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_NOT_USED_CA	                */	 { 0xCA	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_TRIGGER_ZONE_REPORT	        */	 { 0xCB	,	DEI_SC_PLD005     },
    /*DEI_SC_ACK_SENSOR_SILENT_ARM	            */	 { 0xCC	,	DEI_SC_PLD001     },
    /*DEI_SC_ACK_VECHICLE_TEMP_REPORT	        */	 { 0xCD	,	DEI_SC_PLD013     },
    /*DEI_SC_ACK_FULL_SILENT_ARM	            */	 { 0xCE	,	DEI_SC_PLD001     },
    /*DEI_SC_ACK_RUNTIME_LOW_ALERT	            */	 { 0xCF	,	DEI_SC_PLD001     },
    /*DEI_SC_ACK_RANGE_CHECK	                */	 { 0xD0	,	DEI_SC_PLD003     },
    /*DEI_SC_ACK_IGNITION_OFF	                */	 { 0xD1	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_GARAGE_DOOR_OPEN	            */	 { 0xD2	,	DEI_SC_PLDNA      },
    /*DEI_SC_ACK_GARAGE_DOOR_CLOSE	            */	 { 0xD3	,	DEI_SC_PLDNA      },        
    /*DEI_SC_ACK_MODE_CONFIGURATION	            */	 { 0xD4	,	DEI_SC_PLD015     }        
};

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Boolean  dei_sc_tx( DEI_SC_ACK ack , Dei_Sc_Tx_Mode tx_mode , TskTimeout timeout)
{
    Dei_Sc_Tx_Frame *frame  ;

	remote_free_frame:
	{
		frame = (Dei_Sc_Tx_Frame *)q_remove( dei_sc_tx_q_free , timeout ) ; 
		
    	if( !frame ) return FALSE ;    	
	}

    frame->payload   = dei_sc_payload_build( dei_sc_ack_table[ ack ].pld , dei_sc_ack_table[ ack ].cmd  ) ;     
    frame->mode      = tx_mode ; 

    q_insert( dei_sc_tx_q_pending , (Q_Msg *) frame ) ; 

  return TRUE ; 
}

//--------------------------------------------------------------------------//

