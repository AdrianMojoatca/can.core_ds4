/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rxt.h 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __RXT_H__
#define __RXT_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef void (Rxt_Func ) (void * arg) ;

//--------------------------------------------------------------------------//  

typedef enum		   //order is critical, must update Rxt_Rdy when updating this enum
{
	RXT_EXTEND_TIME ,
	RXT_COUNT  		,
	RXT_DOOR   		,
	RXT_DTC			,
	RXT_IGNITION    ,
	RXT_ENABLE      , 
	RXT_HOOD        ,	 // reserved for future use
	RXT_RESERVED1   ,

} Rxt_Rdy_Bit ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean rxt_init 					(Rxt_Func * start_func, Rxt_Func * stop_func) ;	 // start/stop functions sent on the bus
void 	rxt_init_reset 				(void) ;

void 	rxt_oem_start     			(void)   ;	 	// used to synchronize aftermarket when oem fob start is sensed
	
void 	rxt_set_ready 				(Rxt_Rdy_Bit bit) ;	  // set ready to start bit
void 	rxt_clr_ready 				(Rxt_Rdy_Bit bit) ;	  // clear ready to start bit

void 	rxt_shutdown 				(void) ;	  		 // notify engine stopped or takeover done 
void 	rxt_started 				(void) ;			 // notify engine started 

Boolean rxt_set_runtime 			(UInt8 runtime) ;	 // set vehicle runtime (value in minutes)
UInt8 	rxt_get_runtime 			(void) ;
void 	rxt_reset_count 			(void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

