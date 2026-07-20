/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rxt.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __RXT_H__
#define __RXT_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "notify.h" 

#ifdef CM800_DEI_BRAND 
#include "dei_rs.h"
#else
	#ifdef CM800_AUTOSTART_BRAND
      #include "as_rs_runtime.h"
    #endif
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef void (Rxt_Func ) (void * arg) ;

//--------------------------------------------------------------------------//  

typedef enum		   //order is critical
{
	RXT_EXTEND_TIME ,
	RXT_COUNT  		,
	RXT_DOOR   		,
	RXT_DTC			,
	RXT_IGNITION    ,
	RXT_RESERVED1   ,  // reserved for future use
	RXT_RESERVED2   ,
	RXT_RESERVED3   ,

} Rxt_Rdy_Bit ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

#define rxt_init(x,y)    ;
#define rxt_init_reset() ;

#define rxt_oem_start()  ;
	
#define	rxt_set_ready(x) ;
#define	rxt_clr_ready(x) ;

#define	rxt_shutdown()   ;
#define rxt_started()    ;

#define	rxt_get_runtime() 0
#define rxt_reset_count() ;

#ifdef CM800_DEI_BRAND
#define rxt_set_runtime(x) dei_rs_set_oem_rt(x);   //will overwrite the run time base on the value read from the car.
#else
	#ifdef  CM800_AUTOSTART_BRAND
    #define rxt_set_runtime(x) as_rs_set_oem_rt(x);
	#endif
#endif
                                                                                                                        
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

