/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_obd_can.h 33351 2015-08-25 11:20:35Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

// <h> OBD_CAN REQ


#ifndef __CONFIG_OBD_CAN_H__
#define __CONFIG_OBD_CAN_H__

#define CONFIG_OBD_CAN_VERSION 2

// <e0.00>  OBD_CAN_VIN                   
// </e>        
#define OBD_CAN_VIN_MASK 1 

// <e0.00>  OBD_CAN_ENG_COOLANT                        
// </e>   
#define OBD_CAN_ENG_COOLANT_MASK 1 

// <e0.00>  OBD_CAN_RPM                               
// </e>        
#define OBD_CAN_RPM_MASK 1 

// <e0.00>  OBD_CAN_SPEED                               
// </e>     
#define OBD_CAN_SPEED_MASK 1 

// <e0.00>  OBD_CAN_ENG_RUN                            
// </e>       
#define OBD_CAN_ENG_RUN_MASK 1 

// <e0.00>  OBD_CAN_FUEL_LEVEL                          
// </e>     
#define OBD_CAN_FUEL_LEVEL_MASK 1 

// <e0.00>  OBD_CAN_AIR_AMB                            
// </e>     
#define OBD_CAN_AIR_AMB_MASK 1 

// <e0.00>  OBD_CAN_INTAKE_TEMP                        
// </e>    
#define OBD_CAN_INTAKE_TEMP_MASK 1 

// <e0.00>  OBD_CAN_BAT_VOLT                  
// </e>          
#define OBD_CAN_BAT_VOLT_MASK 1 

// <e0.00>  OBD_CAN_DTC_CODE                        
// </e>    
#define OBD_CAN_DTC_CODE_MASK 1 

// <e0.00>  OBD_CAN_DTC_CLR              
// </e>        
#define OBD_CAN_DTC_CLR_MASK 1 

// </h>

// <h>  OBD_CAN_REQ_RETRIES
// <o0> Retries           <0-255>
// <i>  Lowest  retries = 0
// <i>  Highest retries = 255
// <i>	1 Retry = 2 Tries overall
// </h>
#define OBD_CAN_REQ_RETRIES		5

// <h>  OBD_CAN_BAUDRATE
// <o0> Baudrate           <0-65535>
// <i>  Lowest  baudrate = 250
// <i>  Highest baudrate = 500
// <i>	Baudrate is in kbps
// </h>
#define OBD_CAN_BAUDRATE 		500

// <h>  OBD_CAN_IGN_DELAY
// <o0> Delay           <0-255>
// <i>  Lowest  delay = 0
// <i>  Highest delay = 255
// <i>	Delay is in seconds
// </h>
#define OBD_CAN_IGN_DELAY 		1

#endif

