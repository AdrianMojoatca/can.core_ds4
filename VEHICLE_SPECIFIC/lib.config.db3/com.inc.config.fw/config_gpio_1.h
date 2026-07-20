/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_gpio.h 35235 2015-11-12 22:44:01Z huiyi.wang $
/*==========================================================================*/

#ifndef __CONFIG_GPIO_H__
#define __CONFIG_GPIO_H__

#include "device.h"
#include "dac.h"


#define CONFIG_GPIO_VERSION 2  //will generate an error if version does not match

//             <<< Use Configuration Wizard in Context Menu >>>             //

/*==========================================================================*/
/*                                N O T E S                                 */
/*==========================================================================*/

/*==========================================================================*/

/*==========================================================================*/
#if !defined PLATFORM_CM800 && !defined PLATFORM_933
#define TIMEOUT_Q_SIZE      (20)
#include "db3_gpio.h"
#endif

#define SPEED_PROT_NORMAL(__out__,_state_)\
        {(tsk_rs_invalid_state()==FALSE)?__out__( _state_) :__out__(FALSE); }

#define SPEED_PROT_INVERTED(__out__,_state_)\
        {(tsk_rs_invalid_state()==FALSE)?__out__(!_state_) :__out__(TRUE ); } // to be used for inverted outputs (as PHAL2)



/********************   ADI M - 17_10_2025 - DB3 gpio **************/
#define FUNC_HAZARD_OUT(x)			OUT_2(x) 
#define FUNC_BRAKE_OUT(x)			OUT_RELAY1(x)
#define FUNC_PTS_OUT(x)             OUT_5(x)
#define FUNC_PTS_OUT1(x)			OUT_1_PLUS(x)
/********************** END DB3 gpio *******************************/


//--------------------------------------------------------------------------//
// OUTPUTS
//--------------------------------------------------------------------------//

/*
#if defined PLATFORM_CM800 || defined PLATFORM_933
	
    #define VW02_FUNC_EIPS_ALARM_OUT(x)     ;

	#define VW02_FUNC_HAZARD_OUT(x)		    OUT_1(x)
	#define VW02_FUNC_BRAKE_OUT(x)		    OUT_2_PLUS(x)
	#define VW02_FUNC_PTS_OUT(x)		    SPEED_PROT_NORMAL( OUT_4, x )
	
	#define VW02_CLUTCH_PWM_CUT(x)      	OUT_RELAY3(x)
	#define VW02_CLUTCH_PWM_OUT(x)      	OUT_PHAL2_TX(!x)
	#define VW02_CLUTCH_SENSOR(x)       	OUT_RELAY1(x)
	
	//#define VW02_FUNC_PASSAT_KEYIN_OUT(x)	OUT_RELAY1(x) 

	#define VW02_OUT_LOCK(x)    		    ;
	#define VW02_OUT_UNLOCK(x)  		    ;
	#define VW02_OUT_TRUNK(x)   		    ;

	#define VW02_OUT_RF_LOOP(x)             OUT_RELAY2(x)

	#define VW02_OUT_SENSE_DOOR_TRUNK(x)   	;
#else
	//STANDARD & RSR modes
	#define VW02_FUNC_EIPS_ALARM_OUT(x)     ;
	#define VW02_FUNC_HAZARD_OUT(x)		    OUT_1(x)
	#define VW02_FUNC_BRAKE_OUT(x)		    OUT_1_PLUS(x)
	#define VW02_FUNC_PTS_OUT(x)		    SPEED_PROT_NORMAL( OUT_2, x )
	#define VW02_FUNC_IGN_OUT(x)            ;

//STANDARD mode
	#define VW02_CLUTCH_PWM_CUT(x)      	OUT_RELAY1(x)
	#define VW02_CLUTCH_PWM_OUT(x)      	OUT_PHAL2_TX(!x) 
	#define VW02_CLUTCH_SENSOR(x)       	OUT_RELAY2(x)//;OUT_5(x)  // ADI - 22.02.2017

//RSR mode only
	#define VW02_FUNC_PASSAT_KEYIN_OUT(x)	OUT_PHAL2_TX(x)//RSR_OUT_PHAL2_TX(!x) 
	#define VW02_OUT_LOCK(x)    		    OUT_3(x)//RSR_OUT_3(x)
	#define VW02_OUT_UNLOCK(x)  		    OUT_4(x)//RSR_OUT_4(x)

	#define VW02_OUT_TRUNK(x)   		    OUT_5(x)//RSR_OUT_5(x)
	#define VW02_OUT_RF_LOOP(x)             OUT_PHAL1_TX(x)//RSR_OUT_PHAL1_TX(x)
	#define VW02_OUT_SENSE_DOOR_TRUNK(x)   	;

#endif
*/
//--------------------------------------------------------------------------//
    #define FUNC_RAP(x);       //OUT_4(x)

	#define BRAKE_SENSE_SET;               //POS_OUT1_SET
	#define BRAKE_SENSE_CLR;               //POS_OUT1_CLR
	
	#define IGNITION_SENSE_SET            POS_OUT2_SET   
	#define IGNITION_SENSE_CLR            POS_OUT2_CLR	 
	
	#define HBRAKE_SENSE_SET              NEG_OUT1_SET	  
	#define HBRAKE_SENSE_CLR              NEG_OUT1_CLR	   
	
	#define DOOR_SENSE_SET                NEG_OUT3_SET		
	#define DOOR_SENSE_CLR                NEG_OUT3_CLR		 
	
	#define TRUNK_SENSE_SET               ;//NEG_OUT4_SET		  
	#define TRUNK_SENSE_CLR               ;//NEG_OUT4_CLR		   
	
	#define HOOD_SENSE_SET                ///NEG_OUT5_SET			
	#define HOOD_SENSE_CLR                ;//NEG_OUT5_CLR
	
	#define KEY_WRAP_SET                  //RLY2_CTL_SET
	#define KEY_WRAP_CLR                  //RLY2_CTL_CLR

	//#define HAZARD_OUT                  //  GPIO_PL_NEG_OUT1 

	#define PARKING_SET      		      //RLY1_CTL_SET
	#define PARKING_CLR                   //RLY1_CTL_CLR 	



//--------------------------------------------------------------------------//
// RSR OUTPUTS          
//--------------------------------------------------------------------------//

#define RSR_UNLOCK_SET                NEG_OUT3_SET
#define RSR_UNLOCK_CLR                NEG_OUT3_CLR

#define RSR_LOCK_SET                  NEG_OUT5_SET
#define RSR_LOCK_CLR                  NEG_OUT5_CLR

#define RSR_TRUNK_SET                 RSR_UNLOCK_SET; //NEG_OUT4_SET
#define RSR_TRUNK_CLR                 RSR_UNLOCK_CLR; //NEG_OUT4_CLR

#define RSR_START_SET;      		   //POS_OUT1_SET
#define RSR_START_CLR;                 //POS_OUT1_CLR 


//--------------------------------------------------------------------------//
// <h>  Programmable Inputs/Outputs by Platform
//--------------------------------------------------------------------------//
//     <h>  Inputs Linking

//    <o0> LOCK_IN
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)		
//    <o1> UNLOCK_IN
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)		
//    <o2> TRUNK_IN
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)		
//    <o3> AUX1_IN
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)		
//    <o4> AUX2_IN
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)	
//    <o5> AUX3_IN
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)	         
//    <o6> GWR_IN
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)				
//    <o7> POS_IN1
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)		
//    <o8> IGN_IN  
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)			
//    <o9> START_IN  
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only for W2W CMD)
//         <2=> Linked for application        (used by all platform, config CUSTOM INPUTS)			
// </h> 
#define GWR_IN_CFG          0x01
#define POS_IN1_CFG         0x01                    
#define IGN_IN_CFG          0x01
#define START_IN_CFG        0x01
#define LOCK_IN_CFG         0x01
#define UNLOCK_IN_CFG       0x01
#define TRUNK_IN_CFG        0x01
#define AUX1_IN_CFG         0x01
#define AUX2_IN_CFG         0x01
#define AUX3_IN_CFG         0x01

//     <h>  Outputs
           
//    <o0> NEG_OUT1
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)	
//    <o1> NEG_OUT2
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)		
//    <o2> NEG_OUT3 
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)		
//    <o3> NEG_OUT4 
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)			
//    <o4> NEG_OUT5
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)		
//    <o5> POS_OUT1
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)		
//    <o6> POS_OUT2
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)		
//    <o7> RLY1_CTL
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)			
//    <o8> RLY2_CTL
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)		
//    <o9> ISO1
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)	
//    <o10> ISO2
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)
//    <o11> DAC_RLY_CTL
//         <0=> Not Configured
//         <1=> Linked for W2W                  (used by dball only)
//         <2=> Linked for application        (used by all platform at app level)	

// </h> 
#define NEG_OUT1_CFG        0x01
#define NEG_OUT2_CFG        0x01                    
#define NEG_OUT3_CFG        0x02
#define NEG_OUT4_CFG        0x01
#define NEG_OUT5_CFG        0x01
#define POS_OUT1_CFG        0x00
#define POS_OUT2_CFG        0x01
#define RLY1_CTL_CFG        0x01
#define RLY2_CTL_CFG        0x01
#define ISO1_CFG            0x02
#define ISO2_CFG            0x02
#define DAC_RLY_CTL_CFG     0x02
// </h>

#include "gpio.h"

#endif

