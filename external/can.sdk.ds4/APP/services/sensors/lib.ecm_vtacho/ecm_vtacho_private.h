/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ecm_vtacho_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/
#ifndef __ECM_VTACHO_PRIVATE_H__
#define __ECM_VTACHO_PRIVATE_H__
/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "ecm_vtacho.h"
#include "cmd.h"
#include "status_system.h"
#include "voltage.h"
#include "trace.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

#if DEBUG_ECM_TACHO == 1
#define TRACE_DEI_ECM			TRACE
#else
#define TRACE_DEI_ECM(...)		;
#endif
/*--------------------------------------------------------------------------*/

#define ECM_VTACHO_FINE_TUNE_DISABLE		10

/*--------------------------------------------------------------------------*/

typedef Ecm_Vtacho_Evt (*Ecm_Vtacho_State)( void );

/*--------------------------------------------------------------------------*/

typedef struct s_Ecm_Vtacho_Vars
{
	double 	v_ref         ;
	double 	v_min         ;
    double  v_ign_on      ;
	
	UInt16	time          ;
	UInt8   retry_count   ;

	UInt8   fine_tune     ;
	UInt16  cranking_time ;
    Boolean running       ;

	Ecm_Vtacho_State state;
	Ecm_Vtacho_Ref   ref  ;

	Res              res  ;
	
}Ecm_Vtacho_Vars;

/*--------------------------------------------------------------------------*/

#define ECM_VTACHO_VMIN_COUNT				            3
#define ECM_VTACHO_VOLTAGE_CRANKING_DIFF	            0.3
#define ECM_VTACHO_VOLTAGE_ENGINE_STARTED_BY_USER_DIFF  0.8

#define ECM_VTACHO_CRANKING_TIME_MIN		50	 
#define ECM_VTACHO_CRANKING_TIME_STEP		20   
#define ECM_VTACHO_CRANKING_TIME_ADJUST		2

#define ECT_VTACHO_TIMEOUT_READ_VREF		10		
#define ECM_VATCHO_TIMEOUT_CHECK_RUNNING	1000	
#define ECM_VTACHO_VOLTAGE_RUNNING_DIFF		0.4   //was 0.1		

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

Ecm_Vtacho_Evt ecm_vtacho_gwr     ( void );
Ecm_Vtacho_Evt ecm_vtacho_ign     ( void );
Ecm_Vtacho_Evt ecm_vtacho_start   ( void );
Ecm_Vtacho_Evt ecm_vtacho_delay   ( void );
Ecm_Vtacho_Evt ecm_vtacho_check   ( void );
void 		   ecm_vtacho_rs_gwr  ( void );
void 		   ecm_vtacho_rs_ign  ( void );
void 		   ecm_vtacho_rs_start( void );
void           ecm_vtacho_vign_on ( void );

void ecm_vtacho_trace  ( double volt_val );

/*--------------------------------------------------------------------------*/

                
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Ecm_Vtacho_Vars ecm_vtacho_vars ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

