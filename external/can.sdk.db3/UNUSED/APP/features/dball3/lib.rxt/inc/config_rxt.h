/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

#include "cmd.h"

/*==========================================================================*/
// $Id: config_rxt.h 84 2012-02-14 19:47:53Z louis-philippe.rispoli $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_RXT_H__
#define __CONFIG_RXT_H__

//--------------------------------------------------------------------------//
#define CONFIG_RXT_VERSION 2  //will generate an error if lib.rsr does not match with config file
//--------------------------------------------------------------------------//

// <h> RXT CONFIG
// <h> RXT Count
//    	<o0> Max Start Count Before RXT Disable	         <1-10:1>
//    	<e> Extend Runtime Count Enable               (Will not increment Start count)
//    	<o2> Max Extend runtime Count	 <1-10:1>
// </e>
// </h>
#define RXT_MAX_RS_NUMBER				2	    // maximum number of consecutive remote start
#define RXT_COUNT_EXTEND_ENA			1		// count extend runtime in the total possible amount of oem rs
#define RXT_MAX_EXTEND_RS_NUMBER		1		// maximum number of extend runtime

//    	<o0> RXT Attempt   	     <1-5:1>
#define RXT_ATTEMPT         			3 	

//    	<o0> RXT Runtime   	     <1-30:1>
#define RXT_RUNTIME         			10 		

//    	<o0> RXT Start Timeout   	 <1000-10000:1000>
#define RXT_START_TIMEOUT				5000

//    	<o0> RXT Stop Timeout   	 <1000-10000:1000>
#define RXT_STOP_TIMEOUT				5000



//<e0>3X Lock Start Activation Timeout
//<o1>3X Lock Start Activation Timeout (seconds)
//    <0=> disable
//    <2=>2 seconds
//    <3=>3 seconds
//    <4=>*4 seconds
//    <5=>5 seconds
//    <6=>6 seconds
//    <7=>7 seconds
//    <8=>8 seconds
//    <9=>9 seconds
//    <9=>10 seconds
//</e>
#define RXT_FEAT_3X_LOCK_START_ENABLED  			0x0
#define RXT_FEAT_3X_LOCK_START_DEF_VALUE 			0x4

// </h>
//--------------------------------------------------------------------------//

/*==========================================================================*/

#endif

