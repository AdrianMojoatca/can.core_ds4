/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_led_flash.c 19278 2013-02-18 07:53:08Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_led_private.h"
#include "cmd.h"
#include "notify.h"
#include "status_system.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//---------------------------------------------------------------------------//

typedef enum
{
	DEI_LED_CMD_FLASH , 
	DEI_LED_CMD_RESET 
}Dei_Led_Cmd_Type;

Dei_Led_Pattern current_pattern=DEI_LED_NONE;

//---------------------------------------------------------------------------//

#define DEI_LED_UPDATE_SLEEP(i)		(dei_led_pattern_table[i].cmd_type == DEI_LED_CMD_RESET) ? out_fifo_reset( dei_led_fifo , FALSE ) : \
							        out_fifo_insert( dei_led_fifo , 2*dei_led_pattern_table[i].t_offset , dei_led_pattern_table[i].t_on , 2*dei_led_pattern_table[i].t_off , dei_led_pattern_table[i].pulse , dei_led_pattern_table[i].frames )

#define DEI_LED_UPDATE_WAKE(i)		(dei_led_pattern_table[i].cmd_type == DEI_LED_CMD_RESET) ? out_fifo_reset( dei_led_fifo , FALSE ) : \
							         out_fifo_insert( dei_led_fifo , dei_led_pattern_table[i].t_offset , dei_led_pattern_table[i].t_on , dei_led_pattern_table[i].t_off , dei_led_pattern_table[i].pulse , dei_led_pattern_table[i].frames )

#define AS_LED_UPDATE_SLEEP(i)		(as_led_pattern_table[i].cmd_type == AS_LED_CMD_RESET) ? out_fifo_reset( as_led_fifo , FALSE ) : \
							        out_fifo_insert( as_led_fifo , 2*as_led_pattern_table[i].t_offset , as_led_pattern_table[i].t_on , 2*as_led_pattern_table[i].t_off , as_led_pattern_table[i].pulse , as_led_pattern_table[i].frames )

#define AS_LED_UPDATE_WAKE(i)		(as_led_pattern_table[i].cmd_type == AS_LED_CMD_RESET) ? out_fifo_reset( as_led_fifo , FALSE ) : \
							         out_fifo_insert( as_led_fifo , as_led_pattern_table[i].t_offset , as_led_pattern_table[i].t_on , as_led_pattern_table[i].t_off , as_led_pattern_table[i].pulse , as_led_pattern_table[i].frames )

//---------------------------------------------------------------------------//

typedef struct s_Dei_Led_Pattern_Data
{
	Dei_Led_Cmd_Type cmd_type ;
	UInt32 t_offset  ; 
	UInt32 t_on      ;
	UInt32 t_off     ;
	UInt16 pulse     ;
	UInt16 frames    ;
}Dei_Led_Pattern_Data ;

//---------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

//---------------------------------------------------------------------------//


static void dei_led_trace( Dei_Led_Pattern pattern );

//---------------------------------------------------------------------------//

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


static const UInt8 led_pattern_table[] = 
{
	/*DEI_DEI_LED_CMD_RESET         */{ 0 } 																																	, //{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },
    
	/*DEI_LED_ARM             			*/{ LED_PATTERN( INVER_ALLOW, SPEC_OFFSET, LONG_PULSE, LED_FLASH_FOREVER)},		//{ DEI_LED_CMD_FLASH , 1 , 3        , 7 , 0  , OUT_FIFO_LOOP},
    /*DEI_LED_ARM_NOTIFICATION    */{ LED_PATTERN( INVER_ALLOW, SPEC_OFFSET, SHORT_PULSE, LED_FLASH_FOREVER)},  //{ DEI_LED_CMD_FLASH , 1 , 2        , 2 , 0  , OUT_FIFO_LOOP},
    
	/*DEI_LED_DISARM                */{0}                                                                      ,    //{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },     
    
	/*DEI_LED_VALET_ON            	*/{0xFF}																																		,		//{ DEI_LED_CMD_FLASH , 1 , 10*60*60 , 1 , 1  , 1     },
//    /*DEI_LED_VALET_OFF           */{0}																																				,		//{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },  
    
	/*DEI_LED_TRUNK_BYPASSED        */{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_1_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_BYPASSED       	*/{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_3_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 3  , OUT_FIFO_LOOP},
    /*DEI_LED_HOOD_BYPASSED       */{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_6_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 6  , OUT_FIFO_LOOP},
    
    /*DEI_LED_TRUNK_TRIGGERED     */{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_1_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_SHOCK_SENSOR_TRIGGERED*/{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_2_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 2  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_TRIGGERED      	*/{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_3_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 3  , OUT_FIFO_LOOP},  
    /*DEI_LED_IGN_TRIGGERED       */{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_5_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 5  , OUT_FIFO_LOOP},  
	/*DEI_LED_HOOD_TRIGGERED      	*/{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_6_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 6  , OUT_FIFO_LOOP},  
    
        
	/*DEI_LED_RS_ERROR_RUNTIME    	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_1_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 1  , 60    },  
	/*DEI_LED_RS_ERROR_OVERREV      */{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_2_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 2  , 60    },  
	/*DEI_LED_RS_ERROR_LOW_RPM    	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_3_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 3  , 60    },  
	/*DEI_LED_RS_ERROR_TRANSMITTER	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_4_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 4  , 60    },  
	/*DEI_LED_RS_ERROR_BRAKE      	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_5_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 5  , 60    },  
	/*DEI_LED_RS_ERROR_HOOD       	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_6_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 6  , 60    },  
	/*DEI_LED_RS_ERROR_MTS        	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_7_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 7  , 60    },  
	/*DEI_LED_RS_ERROR_NEUTRAL    	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_8_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 8  , 60    },  
	/*DEI_LED_RS_ERROR_LOW_BATT   	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_9_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 9  , 60    },  
	/*DEI_LED_RS_ERROR_TRIGGER    	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_10_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 10 , 60    },  
	/*DEI_LED_RS_ERROR_GLOWPLUG   	*/{LED_PATTERN( INVER_ALLOW, SHORT_OFFSET, SHORT_PULSE, LED_11_FLASH)},		//{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 11 , 60    },

	/*DEI_LED_REMOTE_PROGRAMMING   	*/{LED_PATTERN( INVER_ALLOW, LONG_OFFSET, SHORT_PULSE, LED_1_FLASH)},		//{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 1  , 60    },
	/*DEI_LED_REMOTE_DELETE     		*/{LED_PATTERN( INVER_ALLOW, LONG_OFFSET, SHORT_PULSE, LED_2_FLASH)},		//{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 2  , 60    },
	/*DEI_LED_ZAP			      				*/{LED_PATTERN( INVER_ALLOW, LONG_OFFSET, SHORT_PULSE, LED_3_FLASH)},		//{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 3  , 60    },
	/*DEI_LED_TACHO_DELETE      		*/{LED_PATTERN( INVER_ALLOW, LONG_OFFSET, SHORT_PULSE, LED_4_FLASH)},		//{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 4  , 60    },
	/*DEI_LED_RS_TACHO_PRG        	*/{0xFF},																																//{ DEI_LED_CMD_FLASH , 1 , 10*60*60 , 1 , 1  , 1     },

	/*DEI_LED_TILT_SENSOR_TRIGGERED */{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_4_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 2  , OUT_FIFO_LOOP},
	/*DEI_LED_UNKNOWN_SENSOR_TRIGGERED*/{LED_PATTERN( INVER_ALLOW, MED_OFFSET, SHORT_PULSE, LED_7_FLASH)},		//{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 2  , OUT_FIFO_LOOP},


};



#if VRTC_1SEC == 10

static const Dei_Led_Pattern_Data dei_led_pattern_table[] = 
{
	/*DEI_DEI_LED_CMD_RESET         */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },
    
	/*DEI_LED_ARM             			*/{ DEI_LED_CMD_FLASH , 1 , 3        , 7 , 0  , OUT_FIFO_LOOP},
    /*DEI_LED_ARM_NOTIFICATION    */{ DEI_LED_CMD_FLASH , 1 , 2        , 2 , 0  , OUT_FIFO_LOOP},
    
	/*DEI_LED_DISARM                */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },     
    
	/*DEI_LED_VALET_ON            	*/{ DEI_LED_CMD_FLASH , 1 , 10*60*60 , 1 , 1  , 1     },
    ///*DEI_LED_VALET_OFF           */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },  
    
	/*DEI_LED_TRUNK_BYPASSED        */{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_BYPASSED       	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 3  , OUT_FIFO_LOOP},
    /*DEI_LED_HOOD_BYPASSED       */{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 6  , OUT_FIFO_LOOP},
    
    /*DEI_LED_TRUNK_TRIGGERED     */{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_SENSOR_TRIGGERED    	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 2  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_TRIGGERED      	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 3  , OUT_FIFO_LOOP},  
    /*DEI_LED_IGN_TRIGGERED       */{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 5  , OUT_FIFO_LOOP},  
	/*DEI_LED_HOOD_TRIGGERED      	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 6  , OUT_FIFO_LOOP},  
    
        
	/*DEI_LED_RS_ERROR_RUNTIME    	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 1  , 60    },  
	/*DEI_LED_RS_ERROR_OVERREV      */{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 2  , 60    },  
	/*DEI_LED_RS_ERROR_LOW_RPM    	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 3  , 60    },  
	/*DEI_LED_RS_ERROR_TRANSMITTER	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 4  , 60    },  
	/*DEI_LED_RS_ERROR_BRAKE      	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 5  , 60    },  
	/*DEI_LED_RS_ERROR_HOOD       	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 6  , 60    },  
	/*DEI_LED_RS_ERROR_MTS        	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 7  , 60    },  
	/*DEI_LED_RS_ERROR_NEUTRAL    	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 8  , 60    },  
	/*DEI_LED_RS_ERROR_LOW_BATT   	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 9  , 60    },  
	/*DEI_LED_RS_ERROR_TRIGGER    	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 10 , 60    },  
	/*DEI_LED_RS_ERROR_GLOWPLUG   	*/{ DEI_LED_CMD_FLASH , 7 , 3        , 3 , 11 , 60    },

	/*DEI_LED_REMOTE_PROGRAMMING   	*/{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 1  , 60    },
	/*DEI_LED_REMOTE_DELETE     		*/{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 2  , 60    },
	/*DEI_LED_ZAP			      				*/{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 3  , 60    },
	/*DEI_LED_TACHO_DELETE      		*/{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 4  , 60    },
	/*DEI_LED_RS_TACHO_PRG        	*/{ DEI_LED_CMD_FLASH , 1 , 10*60*60 , 1 , 1  , 1     },

};

#endif

#if VRTC_1SEC == 16

static const Dei_Led_Pattern_Data dei_led_pattern_table[] = 
{
	/*DEI_LED_CMD_RESET            	*/{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },
    
	/*DEI_LED_ARM             			*/{ DEI_LED_CMD_FLASH , 1 , 5        , 11 , 0  , OUT_FIFO_LOOP},
    /*DEI_LED_ARM_NOTIFICATION    */{ DEI_LED_CMD_FLASH , 1 , 3        , 3  , 0  , OUT_FIFO_LOOP},
    
	/*DEI_LED_DISARM                */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },     
    
	/*DEI_LED_VALET_ON            	*/{ DEI_LED_CMD_FLASH , 1 , 16*60*60 , 1 , 1  , 1     },
    /*DEI_LED_VALET_OFF           */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },  
    
	/*DEI_LED_TRUNK_BYPASSED        */{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_BYPASSED       	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 3  , OUT_FIFO_LOOP},
    /*DEI_LED_HOOD_BYPASSED       */{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 6  , OUT_FIFO_LOOP},
    
    /*DEI_LED_TRUNK_TRIGGERED     */{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_SENSOR_TRIGGERED    	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 2  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_TRIGGERED      	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 3  , OUT_FIFO_LOOP},  
    /*DEI_LED_IGN_TRIGGERED       */{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 5  , OUT_FIFO_LOOP},  
	/*DEI_LED_HOOD_TRIGGERED      	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 6  , OUT_FIFO_LOOP},  
    
        
	/*DEI_LED_RS_ERROR_RUNTIME    	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 1  , 60    },  
	/*DEI_LED_RS_ERROR_OVERREV      */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 2  , 60    },  
	/*DEI_LED_RS_ERROR_LOW_RPM    	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 3  , 60    },  
	/*DEI_LED_RS_ERROR_TRANSMITTER	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 4  , 60    },  
	/*DEI_LED_RS_ERROR_BRAKE      	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 5  , 60    },  
	/*DEI_LED_RS_ERROR_HOOD       	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 6  , 60    },  
	/*DEI_LED_RS_ERROR_MTS        	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 7  , 60    },  
	/*DEI_LED_RS_ERROR_NEUTRAL    	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 8  , 60    },  
	/*DEI_LED_RS_ERROR_LOW_BATT   	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 9  , 60    },  
	/*DEI_LED_RS_ERROR_TRIGGER    	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 10 , 60    },  
	/*DEI_LED_RS_ERROR_GLOWPLUG   	*/{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 11 , 60    },

	/*DEI_LED_REMOTE_PROGRAMMING   	*/{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 1  , 60    },
	/*DEI_LED_REMOTE_DELETE     		*/{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 2  , 60    },
	/*DEI_LED_ZAP			      				*/{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 3  , 60    },
	/*DEI_LED_TACHO_DELETE      		*/{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 4  , 60    },
	/*DEI_LED_RS_TACHO_PRG        	*/{ DEI_LED_CMD_FLASH , 1  , 16*60*60 , 1 , 1  , 1     },

};

#endif


static const Dei_Led_Pattern_Data as_led_pattern_table[] = 
{
	/*AS_LED_CMD_RESET            */{ DEI_LED_CMD_RESET , 0  , 0       , 0 , 0  , 0     },
    
	/*AS_LED_ARM             	  */{ DEI_LED_CMD_FLASH , 1  , 5       , 11 , 0  , OUT_FIFO_LOOP},
    /*AS_LED_ARM_NOTIFICATION     */{ DEI_LED_CMD_FLASH , 1  , 2       , 2  , 0  , OUT_FIFO_LOOP},
    
	/*AS_LED_DISARM               */{ DEI_LED_CMD_RESET , 0  , 0       , 0 , 0  , 0     },     
    
	/*AS_LED_VALET_ON             */{ DEI_LED_CMD_FLASH , 1  , 16*60*60, 1 , 1  , 1    },
    /*AS_LED_VALET_OFF            */{ DEI_LED_CMD_FLASH , 1  , 5       , 5 , 5  , 1    },  
    
	/*AS_LED_TRUNK_BYPASSED       */{ DEI_LED_CMD_FLASH , 28 , 4       , 4 , 4  , OUT_FIFO_LOOP},  
	/*AS_LED_DOOR_BYPASSED        */{ DEI_LED_CMD_FLASH , 28 , 4       , 4 , 3  , OUT_FIFO_LOOP},
    /*AS_LED_HOOD_BYPASSED        */{ DEI_LED_CMD_FLASH , 28 , 4       , 4 , 6  , OUT_FIFO_LOOP},
    
    /*AS_LED_TRUNK_TRIGGERED      */{ DEI_LED_CMD_FLASH , 28 , 5       , 5 , 4  , OUT_FIFO_LOOP},  
	/*AS_LED_SENSOR_TRIGGERED     */{ DEI_LED_CMD_FLASH , 28 , 5       , 5 , 2  , OUT_FIFO_LOOP},  
	/*AS_LED_DOOR_TRIGGERED       */{ DEI_LED_CMD_FLASH , 28 , 5       , 5 , 3  , OUT_FIFO_LOOP},  
    /*AS_LED_IGN_TRIGGERED        */{ DEI_LED_CMD_FLASH , 28 , 5       , 5 , 5  , OUT_FIFO_LOOP},  
	/*AS_LED_HOOD_TRIGGERED       */{ DEI_LED_CMD_FLASH , 28 , 5       , 5 , 6  , OUT_FIFO_LOOP},  
    
        
	/*AS_LED_RS_ERROR_RUNTIME     */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 1  , 60    },  
	/*AS_LED_RS_ERROR_OVERREV     */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 2  , 60    },  
	/*AS_LED_RS_ERROR_LOW_RPM     */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 3  , 60    },  
	/*AS_LED_RS_ERROR_TRANSMITTER */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 4  , 60    },  
	/*AS_LED_RS_ERROR_BRAKE       */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 5  , 60    },  
	/*AS_LED_RS_ERROR_HOOD        */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 6  , 60    },  
	/*AS_LED_RS_ERROR_MTS         */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 7  , 60    },  
	/*AS_LED_RS_ERROR_NEUTRAL     */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 8  , 60    },  
	/*AS_LED_RS_ERROR_LOW_BATT    */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 9  , 60    },  
	/*AS_LED_RS_ERROR_TRIGGER     */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 10 , 60    },  
	/*AS_LED_RS_ERROR_GLOWPLUG    */{ DEI_LED_CMD_FLASH , 11 , 5        , 5 , 11 , 60    },

	/*AS_LED_REMOTE_PROGRAMMING   */{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 1  , 60    },
	/*AS_LED_REMOTE_DELETE     	  */{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 2  , 60    },
	/*AS_LED_ZAP			      		  */{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 3  , 60    },
	/*AS_LED_TACHO_DELETE      	  */{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 4  , 60    },
	/*AS_LED_RS_TACHO_PRG         */{ DEI_LED_CMD_FLASH , 3 , 3        , 3 , 5  , 60    },
};

static UInt32 pattern_pre=0;
static uLed multi_led_patt;
UInt32 rebuild_led_pat=0;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Dei_Led_Pattern get_led_last_pattern() {

	return dei_led_last_pattern ;
	}

void build_multi_led_pattern(UInt32 pat, UInt8 bit)	
{
	UInt8 x;
	
	static const Dei_Led_Pattern pattern_ret[] =   //same as the definition in dei_ss_push_trg_history.c
	{		
		DEI_LED_TRUNK_TRIGGERED     ,
		DEI_LED_SHOCK_SENSOR_TRIGGERED    ,
		DEI_LED_DOOR_TRIGGERED      ,
    	DEI_LED_IGN_TRIGGERED       ,
		DEI_LED_HOOD_TRIGGERED    ,
		DEI_LED_TILT_SENSOR_TRIGGERED		,
		DEI_LED_UNKNOWN_SENSOR_TRIGGERED
	};											 
	
	for(x = 0; x < 4; x++)   //retrieve from newest source first
	{
		if(pat == pattern_ret[get_ss_memory_trigger_event(x) - 1])
		{
			rebuild_led_pat |= (led_pattern_table[bit] << (8*(4-1-x)));
		}
	}
}
	
//--------------------------------------------------------------------------//
Boolean dei_led_flash( Dei_Led_Pattern pattern, Boolean led_cnfg )
{
	UInt8 i ;
	UInt8 j = 0;
	UInt32 pattern_tmp;
	UInt8 bit_cnt;
	
  vrtc_stop( dei_led_vrtc );
	multi_led_patt.cur = 0;
	
	if( pattern == DEI_LED_HHU_CONV_ALERT )
	{
		// already saved last pattern 
		if( ( dei_led_last_pattern == DEI_LED_RESET ) || ( dei_led_last_pattern == DEI_LED_DISARM ))
		{
			// turn LED solid ON for 2 sec
			notify_send(NOTIFY_LED_FLASH, led_pattern_table[4] );		// valet mode on pattern
			vrtc_update( dei_led_temp_patt_vrtc, DEI_LED_TIMEOUT_RESUME_PATT );
			vrtc_start( dei_led_temp_patt_vrtc );
		}
		else
		{
			// turn LED solid OFF for 2 sec
			notify_send(NOTIFY_LED_FLASH, led_pattern_table[0] );		// Reset pattern
			vrtc_update( dei_led_temp_patt_vrtc, DEI_LED_TIMEOUT_RESUME_PATT );
			vrtc_start( dei_led_temp_patt_vrtc );
		}
		
		dei_led_temp_patt =  TRUE;
	}
	else
	{
		bit_cnt = 0;
		rebuild_led_pat = 0;
		for( i = 0 ; i < 32 ; i ++ )
			{			
				if( pattern&(1<<i))
				{
          bit_cnt++;					
					multi_led_patt.s.byte[j++ & 0x03] = led_pattern_table[i];
					build_multi_led_pattern((pattern&(1<<i)), i);
				}

// VALET pattern should be stay in IVU/or D2D2 VALET module until new state, should not be cleared in 1 hour				
//				if(pattern & DEI_LED_VALET_ON)
//				{
//					vrtc_update( dei_led_vrtc, DEI_LED_TIMEOUT_SLEEP );
//					vrtc_start( dei_led_vrtc );
//				}
			}
			
			if(((bit_cnt > 1) && (bit_cnt <= 4)) && rebuild_led_pat)
			{
				if(bit_cnt < 4)
				  multi_led_patt.cur = (rebuild_led_pat>>(8*(4 - bit_cnt)));  //led byte non-zero
				else
					multi_led_patt.cur = rebuild_led_pat;
			}
			
			if( (pattern != dei_led_last_pattern) && !dei_led_temp_patt )
			{
				if( led_cnfg )
					notify_send(NOTIFY_LED_FLASH, multi_led_patt.cur );
				else
					notify_send( NOTIFY_LED_FLASH, 0 );		// reset pattern - for Antenna LED OFF
			}

		dei_led_last_pattern = pattern;
		dei_led_last_patt_confg = led_cnfg;
		TRACE_ZONE( dei_led_trace( pattern ) );
	}
	

return TRUE;
}


//--------------------------------------------------------------------------//

void dei_led_flash_sleep( void )
{
	// VALET pattern should be stay in IVU/or D2D2 VALET module until new state, should not be cleared in 1 hour
}


//--------------------------------------------------------------------------//

void dei_led_resume_pattern_execute( void )
{
	UInt8 i ;
	UInt8 j = 0;
  UInt8 bit_cnt;
	
	dei_led_temp_patt = FALSE;
	vrtc_stop( dei_led_temp_patt_vrtc );
	multi_led_patt.cur = 0;

	// resume last pattern 
		for( i = 0 ; i < 32 ; i ++ )
		{			
			if( dei_led_last_pattern&(1<<i))
			{				
				bit_cnt++;
				multi_led_patt.s.byte[j++ & 0x03] = led_pattern_table[i];
				build_multi_led_pattern((dei_led_last_pattern&(1<<i)), i);
			}
		}

		if(bit_cnt > 1 && bit_cnt <= 4)
		{
				if(bit_cnt < 4)
				  multi_led_patt.cur = (rebuild_led_pat>>(8*(4 - bit_cnt)));  //led byte non-zero
				else
					multi_led_patt.cur = rebuild_led_pat;
		}
		
		if( dei_led_last_patt_confg )
			notify_send(NOTIFY_LED_FLASH, multi_led_patt.cur );
		else
			notify_send( NOTIFY_LED_FLASH, 0 );		// reset pattern - for Antenna LED OFF
}
//--------------------------------------------------------------------------//

void dei_led_resume(void)
{
	  vrtc_stop( dei_led_temp_patt_vrtc );
		vrtc_update( dei_led_temp_patt_vrtc, DEI_LED_TIMEOUT_RESUME_PATT );
		vrtc_start( dei_led_temp_patt_vrtc );
}


//--------------------------------------------------------------------------//
void dei_led_trace( Dei_Led_Pattern pattern )
{
	if( pattern & DEI_LED_ARM                 ){ TRACE_DEI_LED("L01") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_ARM_NOTIFICATION    ){ TRACE_DEI_LED("L02") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_DISARM              ){ TRACE_DEI_LED("L03") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_VALET_ON            ){ TRACE_DEI_LED("L04") ; os_tsk_wait( 10 ); };
	//if( pattern & DEI_LED_VALET_OFF           ){ TRACE_DEI_LED("L05") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_DOOR_BYPASSED       ){ TRACE_DEI_LED("L06") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_HOOD_BYPASSED       ){ TRACE_DEI_LED("L07") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_TRUNK_BYPASSED      ){ TRACE_DEI_LED("L08") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_DOOR_TRIGGERED      ){ TRACE_DEI_LED("L09") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_HOOD_TRIGGERED      ){ TRACE_DEI_LED("L10") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_TRUNK_TRIGGERED     ){ TRACE_DEI_LED("L11") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_IGN_TRIGGERED       ){ TRACE_DEI_LED("L12") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SHOCK_SENSOR_TRIGGERED    ){ TRACE_DEI_LED("L13") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_RUNTIME    ){ TRACE_DEI_LED("L14") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_OVERREV    ){ TRACE_DEI_LED("L15") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_LOW_RPM    ){ TRACE_DEI_LED("L16") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_TRANSMITTER){ TRACE_DEI_LED("L17") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_BRAKE      ){ TRACE_DEI_LED("L18") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_HOOD       ){ TRACE_DEI_LED("L19") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_MTS        ){ TRACE_DEI_LED("L20") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_NEUTRAL    ){ TRACE_DEI_LED("L21") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_LOW_BATT   ){ TRACE_DEI_LED("L22") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_TRIGGER    ){ TRACE_DEI_LED("L23") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_ERROR_GLOWPLUG   ){ TRACE_DEI_LED("L24") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_RS_TACHO_PRG        ){ TRACE_DEI_LED("L25") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_REMOTE_PROGRAMMING  ){ TRACE_DEI_LED("L26") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_REMOTE_DELETE       ){ TRACE_DEI_LED("L27") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_TACHO_DELETE        ){ TRACE_DEI_LED("L28") ; os_tsk_wait( 10 ); };

	if( pattern & DEI_LED_ZAP                   ){ TRACE_DEI_LED("L_ZAP") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_HHU_CONV_ALERT        ){ TRACE_DEI_LED("L_HHU_CONV_ALERT") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR1_ADJUST        ){ TRACE_DEI_LED("L_SENS1_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR2_ADJUST        ){ TRACE_DEI_LED("L_SENS2_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR3_ADJUST        ){ TRACE_DEI_LED("L_SENS3_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR4_ADJUST        ){ TRACE_DEI_LED("L_SENS4_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR5_ADJUST        ){ TRACE_DEI_LED("L_SENS5_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR6_ADJUST        ){ TRACE_DEI_LED("L_SENS6_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR7_ADJUST        ){ TRACE_DEI_LED("L_SENS7_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR8_ADJUST        ){ TRACE_DEI_LED("L_SENS8_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR9_ADJUST        ){ TRACE_DEI_LED("L_SENS9_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR10_ADJUST        ){ TRACE_DEI_LED("L_SENS10_ADJ") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR11_ADJUST        ){ TRACE_DEI_LED("L_SENS11_ADJ") ; os_tsk_wait( 10 ); };
}


//--------------------------------------------------------------------------//




