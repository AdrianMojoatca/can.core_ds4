/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_gpio.h 33351 2015-08-25 11:20:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __CONFIG_GPIO_H__
#define __CONFIG_GPIO_H__

#include "device.h"

#define CONFIG_GPIO_VERSION 2  //will generate an error if version does not match

//             <<< Use Configuration Wizard in Context Menu >>>             //

/*==========================================================================*/
/*                                N O T E S                                 */
/*==========================================================================*/

//--------------------------------------------------------------------------//
// DIGITAL OUTPUTS
//--------------------------------------------------------------------------//

//#define XYZ_SET           NEG_OUT1_SET
//#define XYZ_CLR           NEG_OUT1_CLR 

//#define XYZ_SET           NEG_OUT2_SET
//#define XYZ_CLR           NEG_OUT2_CLR

//#define XYZ_SET           NEG_OUT3_SET
//#define XYZ_CLR           NEG_OUT3_CLR

//#define XYZ_SET           NEG_OUT4_SET
//#define XYZ_CLR           NEG_OUT4_CLR
        
//#define XYZ_SET           NEG_OUT5_SET
//#define XYZ_CLR           NEG_OUT5_CLR

//#define XYZ_SET           POS_OUT1_SET
//#define XYZ_CLR           POS_OUT1_CLR 

//#define XYZ_SET           POS_OUT2_SET
//#define XYZ_CLR           POS_OUT2_CLR 

//#define XYZ_SET	        RLY1_CTL_SET
//#define XYZ_CLR	        RLY1_CTL_CLR

//#define XYZ_SET           RLY2_CTL_SET
//#define XYZ_CLR           RLY2_CTL_CLR

//#define XYZ_SET           DAC_RLY_CTL_SET       
//#define XYZ_CLR           DAC_RLY_CTL_CLR

//--------------------------------------------------------------------------//
// RSR OUTPUTS
//--------------------------------------------------------------------------//

//#define RSR_ACC_SET      
//#define RSR_ACC_CLR

//#define RSR_IGN_SET      
//#define RSR_IGN_CLR  

//#define RSR_START_SET    
//#define RSR_START_CLR    

//#define RSR_LOCK_SET
//#define RSR_LOCK_CLR

//#define RSR_UNLOCK_SET
//#define RSR_UNLOCK_CLR

//#define RSR_TRUNK_SET
//#define RSR_TRUNK_CLR  

//#define RSR_PARKING_SET  
//#define RSR_PARKING_CLR  

//--------------------------------------------------------------------------//
// CUSTOM INPUTS
//--------------------------------------------------------------------------//

//#define XYZ_IN            CUSTOM_NEG_IN1
//#define XYZ_CMD           INPUT_CUSTOM_NEG1 

//#define XYZ_IN            CUSTOM_NEG_IN2 
//#define XYZ_CMD           INPUT_CUSTOM_NEG2 

//#define XYZ_IN            CUSTOM_NEG_IN3
//#define XYZ_CMD           INPUT_CUSTOM_NEG3

//#define XYZ_IN            CUSTOM_NEG_IN4 
//#define XYZ_CMD           INPUT_CUSTOM_NEG4 

//#define XYZ_IN            CUSTOM_NEG_IN5
//#define XYZ_CMD           INPUT_CUSTOM_NEG5 

//#define XYZ_IN            CUSTOM_NEG_IN6 
//#define XYZ_CMD           INPUT_CUSTOM_NEG6 

//#define XYZ_IN            CUSTOM_NEG_IN7
//#define XYZ_CMD           INPUT_CUSTOM_NEG7 

//#define XYZ_IN            CUSTOM_POS_IN1 
//#define XYZ_CMD           INPUT_CUSTOM_POS1 

//#define XYZ_IN            CUSTOM_POS_IN2 
//#define XYZ_CMD           INPUT_CUSTOM_POS2 

//#define XYZ_IN            CUSTOM_POS_IN3 
//#define XYZ_CMD           INPUT_CUSTOM_POS3

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
#define LOCK_IN_CFG   		0x01
#define UNLOCK_IN_CFG   	0x01
#define TRUNK_IN_CFG   		0x01
#define AUX1_IN_CFG   		0x01
#define AUX2_IN_CFG   		0x01
#define AUX3_IN_CFG   		0x01
#define GWR_IN_CFG   		0x01
#define POS_IN1_CFG   		0x01					
#define IGN_IN_CFG   		0x01
#define START_IN_CFG   		0x01

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
#define NEG_OUT1_CFG   		0x00
#define NEG_OUT2_CFG   		0x00					
#define NEG_OUT3_CFG   		0x00
#define NEG_OUT4_CFG   		0x00
#define NEG_OUT5_CFG   		0x00
#define POS_OUT1_CFG   	    0x00
#define POS_OUT2_CFG   		0x00
#define RLY1_CTL_CFG   		0x00
#define RLY2_CTL_CFG   		0x00
#define ISO1_CFG   		    0x00
#define ISO2_CFG   		    0x00
#define DAC_RLY_CTL_CFG     0x02
// </h>

#include "gpio.h"

#endif

