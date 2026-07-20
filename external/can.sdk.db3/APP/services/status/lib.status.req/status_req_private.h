/*==========================================================================*/
// $Id: status_req_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _STATUS_REQ_PRIVATE_H_
#define _STATUS_REQ_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "d2d.h"
#include "nvfs_usr.h"
#include "status_req.h"
#include "status.h"
#include "timeout_f.h"
#if STATUS_DTC_ENABLE == 1
	#include "obd_can.h"
#else
	#warning "OBD CAN NOT INCLUDED"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define STATUS_REQ_ERR_BUFF_SIZE 5

#define RPM_MIN_INTERVAL 250  //minimum interval between req rpm
#define RPM_RELEASE_BLOCK 500 //minimum rpm to answer without minimum interval 

//--------------------------------------------------------------------------//

typedef enum
{
   STATUS_REQ_D2D_TIMEOUT = 1,

   STATUS_REQ_ERR_COUNT 

}Status_Req_Err_Code;

//--------------------------------------------------------------------------//

typedef struct s_Status_Req_Err
{
  UInt16         		count ;
  Status_Req_Err_Code 	code [STATUS_REQ_ERR_BUFF_SIZE] ;
  
}Status_Req_Err ;

//--------------------------------------------------------------------------//

typedef struct s_Status_Req_Vars
{
  Boolean        		smartstart_is_connect ;
  Boolean               obd_can_check_ena ;
  Timeout               rpm_timeout ;
  
}Status_Req_Vars ;

//--------------------------------------------------------------------------//


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void status_req_input_status 				(void * arg) ;
void status_req_rpm							(void * arg) ;
void status_req_speed						(void * arg) ;
void status_req_eng_temp					(void * arg) ;
void status_req_intake_temp		    		(void * arg) ;
void status_req_dtc							(void * arg) ;
void status_req_clr_dtc						(void * arg) ;
void status_req_fuel_level					(void * arg) ;
void status_req_odo							(void * arg) ;
void status_req_bat_volt					(void * arg) ;
void status_req_tpms						(void * arg) ;
void status_req_vin_1						(void * arg) ;
void status_req_vin_2						(void * arg) ;
void status_req_vin_3						(void * arg) ;
void status_req_vin_4						(void * arg) ;
void status_req_vin_5						(void * arg) ;
void status_req_vin_6						(void * arg) ;
void status_req_dtc_code					(void) ;
void status_req_vin							(void * arg) ;
void status_req_protocol_ver				(void * arg) ;
void status_req_al_query					(void * arg) ;
void status_req_al_alert_clear				(void * arg) ;
void status_req_module_info					(void * arg) ;
void status_req_extended_status_support		(void * arg) ;
void status_req_extended_status_support_sync(void * arg);
void status_req_runtime						(void * arg) ;
void status_req_2way_status					(void * arg) ;
void status_req_extended_status				(void * arg) ;

void status_req_err_add 					(Status_Req_Err_Code code) ;
void status_req_reset 						(void) ;
void status_req_rpm_blocked                 (void * arg) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Status_Req_Err  status_req_err  ;
extern Status_Req_Vars status_req_vars ;

extern const NVFS_Entry_Name nvfs_status_req_err ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


//--------------------------------------------------------------------------//

#endif




