/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_led_flash.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_led_private.h"
#include "cmd.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//---------------------------------------------------------------------------//

typedef enum
{
	DEI_LED_CMD_FLASH , 
	DEI_LED_CMD_RESET 
}Dei_Led_Cmd_Type;

//---------------------------------------------------------------------------//

#define DEI_LED_UPDATE_SLEEP(i)		(dei_led_pattern_table[i].cmd_type == DEI_LED_CMD_RESET) ? out_fifo_reset( dei_led_fifo , FALSE ) : \
							        out_fifo_insert( dei_led_fifo , 2*dei_led_pattern_table[i].t_offset , dei_led_pattern_table[i].t_on , 2*dei_led_pattern_table[i].t_off , dei_led_pattern_table[i].pulse , dei_led_pattern_table[i].frames )

#define DEI_LED_UPDATE_WAKE(i)		(dei_led_pattern_table[i].cmd_type == DEI_LED_CMD_RESET) ? out_fifo_reset( dei_led_fifo , FALSE ) : \
							         out_fifo_insert( dei_led_fifo , dei_led_pattern_table[i].t_offset , dei_led_pattern_table[i].t_on , dei_led_pattern_table[i].t_off , dei_led_pattern_table[i].pulse , dei_led_pattern_table[i].frames )

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

#if VRTC_1SEC == 10

static const Dei_Led_Pattern_Data dei_led_pattern_table[] = 
{
	/*DEI_DEI_LED_CMD_RESET         */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },
    
	/*DEI_LED_ARM             		*/{ DEI_LED_CMD_FLASH , 1 , 3        , 7 , 0  , OUT_FIFO_LOOP},
    /*DEI_LED_ARM_NOTIFICATION    	*/{ DEI_LED_CMD_FLASH , 1 , 2        , 2 , 0  , OUT_FIFO_LOOP},
    
	/*DEI_LED_DISARM                */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },     
    
	/*DEI_LED_VALET_ON            	*/{ DEI_LED_CMD_FLASH , 1 , 10*60*60 , 1 , 1  , 1     },
    /*DEI_LED_VALET_OFF             */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },  
    
	/*DEI_LED_TRUNK_BYPASSED        */{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_BYPASSED       	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 3  , OUT_FIFO_LOOP},
    /*DEI_LED_HOOD_BYPASSED       	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 6  , OUT_FIFO_LOOP},
    
    /*DEI_LED_TRUNK_TRIGGERED     	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_SENSOR_TRIGGERED    	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 2  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_TRIGGERED      	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 3  , OUT_FIFO_LOOP},  
    /*DEI_LED_IGN_TRIGGERED       	*/{ DEI_LED_CMD_FLASH , 10 , 3       , 3 , 5  , OUT_FIFO_LOOP},  
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
	/*DEI_LED_REMOTE_DELETE     	*/{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 2  , 60    },
	/*DEI_LED_ZAP			      	*/{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 3  , 60    },
	/*DEI_LED_TACHO_DELETE      	*/{ DEI_LED_CMD_FLASH , 15, 3        , 3 , 4  , 60    },
	/*DEI_LED_RS_TACHO_PRG        	*/{ DEI_LED_CMD_FLASH , 1 , 10*60*60 , 1 , 1  , 1     },

};

#endif

#if VRTC_1SEC == 16

static const Dei_Led_Pattern_Data dei_led_pattern_table[] = 
{
	/*DEI_LED_CMD_RESET            */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },
    
	/*DEI_LED_ARM             		*/{ DEI_LED_CMD_FLASH , 1 , 5        , 11 , 0  , OUT_FIFO_LOOP},
    /*DEI_LED_ARM_NOTIFICATION    	*/{ DEI_LED_CMD_FLASH , 1 , 3        , 3  , 0  , OUT_FIFO_LOOP},
    
	/*DEI_LED_DISARM                */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },     
    
	/*DEI_LED_VALET_ON            	*/{ DEI_LED_CMD_FLASH , 1 , 16*60*60 , 1 , 1  , 1     },
    /*DEI_LED_VALET_OFF             */{ DEI_LED_CMD_RESET , 0 , 0        , 0 , 0  , 0     },  
    
	/*DEI_LED_TRUNK_BYPASSED        */{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_BYPASSED       	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 3  , OUT_FIFO_LOOP},
    /*DEI_LED_HOOD_BYPASSED       	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 6  , OUT_FIFO_LOOP},
    
    /*DEI_LED_TRUNK_TRIGGERED     	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 1  , OUT_FIFO_LOOP},  
	/*DEI_LED_SENSOR_TRIGGERED    	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 2  , OUT_FIFO_LOOP},  
	/*DEI_LED_DOOR_TRIGGERED      	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 3  , OUT_FIFO_LOOP},  
    /*DEI_LED_IGN_TRIGGERED       	*/{ DEI_LED_CMD_FLASH , 16 , 5       , 5 , 5  , OUT_FIFO_LOOP},  
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
	/*DEI_LED_REMOTE_DELETE     	*/{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 2  , 60    },
	/*DEI_LED_ZAP			      	*/{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 3  , 60    },
	/*DEI_LED_TACHO_DELETE      	*/{ DEI_LED_CMD_FLASH , 24 , 5        , 5 , 4  , 60    },
	/*DEI_LED_RS_TACHO_PRG        	*/{ DEI_LED_CMD_FLASH , 1  , 16*60*60 , 1 , 1  , 1     },

};

#endif


/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Boolean dei_led_flash( Dei_Led_Pattern pattern , Boolean led_cnfg)
{
	UInt8 i ;
	
	if( os_res_request( dei_led_res , DEI_LED_RES_TIMEOUT ) )
    {
        vrtc_stop( dei_led_vrtc );
    	{
    		out_fifo_reset( dei_led_fifo , FALSE ) ;
    
    		for( i = 0 ; i < 32 ; i ++ )
    		{			
    			if( pattern&(1<<i) )
    			{				
    				out_fifo_insert( dei_led_fifo , (Out_Func)dei_led_ctrl , dei_led_pattern_table[i].t_offset , dei_led_pattern_table[i].t_on , dei_led_pattern_table[i].t_off , dei_led_pattern_table[i].pulse , dei_led_pattern_table[i].frames );
    			}
    		}
    
    		dei_led_sleep_tmr = 0 ; 
    
    		dei_led_last_pattern = pattern ;
    
    		vrtc_update( dei_led_vrtc  , 1 );
    	}
    	vrtc_start( dei_led_vrtc );
    
        os_res_release( dei_led_res ) ; 
    }

    TRACE_ZONE( dei_led_trace( pattern ) );
	
	return cmd_q_rs_insert( INPUT_LED , 100 , (void*)pattern );
}


//--------------------------------------------------------------------------//

void dei_led_flash_sleep( void )
{
	UInt8 i ;

	for( i = 0 ; i < 32 ; i ++ )
	{
		if( dei_led_last_pattern & (1 << i ) )
		{
			if( !(dei_led_last_pattern & DEI_LED_VALET_ON) )
			{
				if(dei_led_pattern_table[i].cmd_type == DEI_LED_CMD_RESET) 
				{
					out_fifo_reset( dei_led_fifo , FALSE ) ;
				}
				else
				{
				    out_fifo_insert( dei_led_fifo , (Out_Func)dei_led_ctrl , 2*dei_led_pattern_table[i].t_offset , dei_led_pattern_table[i].t_on , 2*dei_led_pattern_table[i].t_off , dei_led_pattern_table[i].pulse , dei_led_pattern_table[i].frames );
				}	
			}
		}
	}
}

//--------------------------------------------------------------------------//


void dei_led_trace( Dei_Led_Pattern pattern )
{			
	if( pattern & DEI_LED_ARM                 ){ TRACE_DEI_LED("L01") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_ARM_NOTIFICATION    ){ TRACE_DEI_LED("L02") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_DISARM              ){ TRACE_DEI_LED("L03") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_VALET_ON            ){ TRACE_DEI_LED("L04") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_VALET_OFF           ){ TRACE_DEI_LED("L05") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_DOOR_BYPASSED       ){ TRACE_DEI_LED("L06") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_HOOD_BYPASSED       ){ TRACE_DEI_LED("L07") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_TRUNK_BYPASSED      ){ TRACE_DEI_LED("L08") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_DOOR_TRIGGERED      ){ TRACE_DEI_LED("L09") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_HOOD_TRIGGERED      ){ TRACE_DEI_LED("L10") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_TRUNK_TRIGGERED     ){ TRACE_DEI_LED("L11") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_IGN_TRIGGERED       ){ TRACE_DEI_LED("L12") ; os_tsk_wait( 10 ); };
	if( pattern & DEI_LED_SENSOR_TRIGGERED    ){ TRACE_DEI_LED("L13") ; os_tsk_wait( 10 ); };
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
}


//--------------------------------------------------------------------------//




