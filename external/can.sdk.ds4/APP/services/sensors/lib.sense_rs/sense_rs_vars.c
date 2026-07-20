/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_vars.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

Sense_Rs_Input_Config *sense_rs_input_config;

#ifdef PLATFORM_CM800

const Sense_Rs_Input_Config sense_rs_input_config_default = 
{
    {
        SENSE_RS_IN_FUNCTION_CUSTOM_NEG1,            // SENSE_RS_IN_NEG1   Blue/Black    CN3 PIN14 custom neg1 default function
        SENSE_RS_IN_FUNCTION_CUSTOM_NEG2,            // SENSE_RS_IN_NEG2   Lt. Blue/Red  CN3 PIN1  without default function
        SENSE_RS_IN_FUNCTION_CUSTOM_NEG3,            // SENSE_RS_IN_NEG3   Blue/Red      CN3 PIN12 without default function

        SENSE_RS_IN_FUNCTION_HANDBRAKE  ,            // SENSE_RS_IN_NEG4   White/Black   CN3 PIN2  handbrake sense default function
        SENSE_RS_IN_FUNCTION_IN_HOOD    ,            // SENSE_RS_IN_NEG5   Gray          CN3 PIN3  hood sense default function
        SENSE_RS_IN_FUNCTION_SAFETY_SW  ,            // SENSE_RS_IN_NEG6   Black/White   CN3 PIN4  safety switch sense default function
        SENSE_RS_IN_FUNCTION_IN_TRUNK   ,            // SENSE_RS_IN_NEG7   Lt.Blue       CN3 PIN13 trunk sensedefault function
        
        SENSE_RS_IN_FUNCTION_GLOWPLUG   ,            // SENSE_RS_IN_POS1   Gray/Black    CN3 PIN5  glowplug sense default function
        SENSE_RS_IN_FUNCTION_BRAKE      ,            // SENSE_RS_IN_POS2   Lt.Brown      CN3 PIN16 brake sense default function 
        SENSE_RS_IN_FUNCTION_IGNITION   ,            // SENSE_RS_IN_POS3   Pink          CN9 PIN7  ignition sense default function
        
        SENSE_RS_IN_FUNCTION_DOOR       ,            // SENSE_RS_IN_NP     Green/Gray    CN3 PIN10(negative)
                                                     //                                  -->sharing same IO pin . Door sense default function
                                                     //                    Purple/Grey   CN3 PIN11(positive)
    },

    SENSE_RS_START_INPUT_MAPPED_TO_START,  
	
    SENSE_RS_PUSH_ANTENNA_NOT_DEFINED 	
};
    

#else

const Sense_Rs_Input_Config sense_rs_input_config_default = 
{
    {
			SENSE_RS_IN_FUNCTION_START_TRIGGER,				// SENSE_RS_IN_NEG1
			SENSE_RS_IN_FUNCTION_HEADLIGHT_DETECT,		// SENSE_RS_IN_NEG3
			SENSE_RS_IN_FUNCTION_HANDBRAKE,						// SENSE_RS_IN_NEG4
			SENSE_RS_IN_FUNCTION_IN_HOOD,							// SENSE_RS_IN_NEG5
			SENSE_RS_IN_FUNCTION_IN_TRUNK,						// SENSE_RS_IN_NEG7
			SENSE_RS_IN_FUNCTION_INST_TRIGGER,				// SENSE_RS_IN_POS1
			SENSE_RS_IN_FUNCTION_BRAKE,								// SENSE_RS_IN_POS2
			SENSE_RS_IN_FUNCTION_IGNITION,						// SENSE_RS_IN_POS3
			SENSE_RS_IN_FUNCTION_DOOR,								// SENSE_RS_IN_NP
			
    },

    SENSE_RS_START_INPUT_MAPPED_TO_START,  
	
    SENSE_RS_PUSH_ANTENNA_NOT_DEFINED 	
};

Timeout sense_ext_trigger_timer;
Boolean sense_rs_trig_cmd_arrive=FALSE;

UInt8 sense_rs_door_defined=0;  // for debug
UInt32 sense_rs_doors_sts=0;
UInt32 sense_rs_hoods_sts=0;
UInt32 sense_rs_trunks_sts=0;
UInt32 sense_rs_igns_sts=0;
UInt32 sense_rs_brakes_sts=0;
UInt32 sense_rs_handbrakes_sts=0;
UInt32 sense_rs_headlights_sts=0;
UInt32 sense_rs_extrigs_sts=0;
UInt32 sense_rs_tamper_sts=0;
UInt32 sense_rs_ptim_stop_sts=0;
UInt32 sense_rs_ext_open_sts=0;
UInt32 sense_rs_ext_close_sts=0;
UInt32 sense_rs_ext_trunk_sts=0;

#endif
/*==========================================================================*/
