/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __SENSE_RS_H__
#define __SENSE_RS_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
#ifdef PLATFORM_CM800
typedef enum // --enumerate all the functions available for an input
{
    SENSE_RS_IN_FUNCTION_HANDBRAKE    =1,
    SENSE_RS_IN_FUNCTION_SAFETY_SW    =2,
    SENSE_RS_IN_FUNCTION_IN_HOOD      =3,
    SENSE_RS_IN_FUNCTION_IN_TRUNK     =4,
    SENSE_RS_IN_FUNCTION_DOOR         =5,
    SENSE_RS_IN_FUNCTION_BRAKE        =6,
    SENSE_RS_IN_FUNCTION_GLOWPLUG     =7,
    SENSE_RS_IN_FUNCTION_IGNITION     =8,
    SENSE_RS_IN_FUNCTION_START_TRIGGER=9,

    SENSE_RS_IN_FUNCTION_CUSTOM_NEG1  =10,
    SENSE_RS_IN_FUNCTION_CUSTOM_NEG2  =11,
    SENSE_RS_IN_FUNCTION_CUSTOM_NEG3  =12,
    SENSE_RS_IN_FUNCTION_CUSTOM_POS1  =13,
    SENSE_RS_IN_FUNCTION_CUSTOM_POS2  =14,

    SENSE_RS_IN_FUNCTION_COUNT        =15 

}Sense_Rs_In_Functions;

#else

typedef enum // --enumerate all the functions available for an input
{
    SENSE_RS_IN_FUNCTION_NOT_CONFIGURED		=0,
	
    SENSE_RS_IN_FUNCTION_START_TRIGGER		=1,
    SENSE_RS_IN_FUNCTION_BRAKE        		=2,
    SENSE_RS_IN_FUNCTION_DOOR         		=3,
    SENSE_RS_IN_FUNCTION_IN_HOOD      		=4,
	  SENSE_RS_IN_FUNCTION_INST_TRIGGER			=5,
    SENSE_RS_IN_FUNCTION_HANDBRAKE    		=6,
    SENSE_RS_IN_FUNCTION_HEADLIGHT_DETECT =7,
    SENSE_RS_IN_FUNCTION_IN_TRUNK     		=8,
    SENSE_RS_IN_FUNCTION_VALET_SW     		=9,
    SENSE_RS_IN_FUNCTION_IGNITION     		=10,
    SENSE_RS_IN_FUNCTION_CUSTOM_POS1  		=11,
    SENSE_RS_IN_FUNCTION_EXT_SENSOR_TRIGGER	=12,
    SENSE_RS_IN_FUNCTION_TAMPER             =13,
	  SENSE_RS_IN_FUNCTION_PTIM_STOP        = 14,
	  SENSE_RS_IN_FUNCTION_EXT_OPEN         = 15,
	  SENSE_RS_IN_FUNCTION_EXT_CLOSE        = 16,
		SENSE_RS_IN_FUNCTION_EXT_TRUNK        = 17,

    SENSE_RS_IN_FUNCTION_COUNT        =18
}Sense_Rs_In_Functions;

#endif
/*--------------------------------------------------------------------------*/

typedef enum  //---enumerate all the available inputs from CM800 product
{
    SENSE_RS_IN_NEG1 ,  //Blue/Black    CN3 PIN14 custom neg1 default function
#ifdef PLATFORM_CM800	
    SENSE_RS_IN_NEG2 ,  //Lt. Blue/Red  CN3 PIN1  without default function
#endif
    SENSE_RS_IN_NEG3 ,  //Blue/Red      CN3 PIN12 without default function

    SENSE_RS_IN_NEG4 ,  //White/Black   CN3 PIN2  handbrake sense default function
    SENSE_RS_IN_NEG5 ,  //Gray          CN3 PIN3  hood sense default function
#ifdef PLATFORM_CM800
    SENSE_RS_IN_NEG6 ,  //Black/White   CN3 PIN4  safety switch sense default function
#endif
    SENSE_RS_IN_NEG7 ,  //Lt.Blue       CN3 PIN13 trunk sensedefault function
    
    SENSE_RS_IN_POS1 ,  //Gray/Black    CN3 PIN5  glowplug sense default function
    SENSE_RS_IN_POS2 ,  //Lt.Brown      CN3 PIN16 brake sense default function 
    SENSE_RS_IN_POS3 ,  //Pink          CN9 PIN7  ignition sense default function
    
    SENSE_RS_IN_NP   ,  //Green/Gray    CN3 PIN10(negative)
                        //                                  -->sharing same IO pin . Door sense default function
                        //Purple/Grey   CN3 PIN11(positive)
    
    SENSE_RS_IN_COUNT

}Sense_Rs_Input_Selector ;   

/*--------------------------------------------------------------------------*/

typedef enum
{
    SENSE_RS_START_INPUT_MAPPED_TO_START , 
    SENSE_RS_START_INPUT_MAPPED_TO_TURBO

}Sense_Rs_Start_Input_Config;

typedef enum
{
    SENSE_RS_PUSH_ANTENNA_NORMAL     , 
    SENSE_RS_PUSH_ANTENNA_REVERSED   ,
	SENSE_RS_PUSH_ANTENNA_NOT_DEFINED,

}Sense_Rs_Push_Antenna;


/*--------------------------------------------------------------------------*/

typedef struct s_Sense_Rs_Input_Config
{
    UInt8 inputs[SENSE_RS_IN_COUNT];

    Sense_Rs_Start_Input_Config start_input_config;
	
	Sense_Rs_Push_Antenna push_antenna_type;
    
}Sense_Rs_Input_Config;

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Boolean ext_sensor_trigger_wire_status;
extern Boolean ext_sensor_trigger_configured;
extern Boolean sense_rs_trig_cmd_arrive;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

void                   sense_rs_init      (Sense_Rs_Input_Config *config);
Sense_Rs_Input_Config* sense_rs_get_config(void);

void    sense_rs_door					( Boolean do_init , Boolean status, UInt8 from);
void    sense_rs_hood         ( Boolean do_init , Boolean status, UInt8 from);
void    sense_rs_trunk        ( Boolean do_init , Boolean status, UInt8 from);
void    sense_rs_handbrake    ( Boolean do_init , Boolean status, UInt8 from);
void    sense_rs_brake        ( Boolean do_init , Boolean status, UInt8 from);
void    sense_rs_ign          ( Boolean do_init , Boolean status, UInt8 from);
void    sense_rs_start_trigger( Boolean do_init , Boolean status, UInt8 from);
void    sense_rs_custom_pos1  ( Boolean do_init , Boolean status, UInt8 from);

#endif


