/*==========================================================================*/
// $Id: rftd2_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _RFTD2_PRIVATE_H_
#define _RFTD2_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "rftd2.h"
#include "os.h"
#include "status.h"
#include "nvfs_usr.h"
#if STATUS_DTC_ENABLE == 1
	#include "obd_can.h"
#else
	#warning "OBD CAN NOT INCLUDED"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef union s_Rftd2_Status
{
  struct
  {
     unsigned lock       	: 1 ;
     unsigned arm        	: 1 ;
     unsigned ignition     	: 1 ;
     unsigned parking_light : 1 ;
     unsigned siren        	: 1 ;
     unsigned valet        	: 1 ;
     unsigned is_running  	: 1 ;
     unsigned panic    		: 1 ;	  
//--------------------------------------------------------------------------//
     unsigned door       	: 1 ;
     unsigned trunk        	: 1 ;
     unsigned hood      	: 1 ;
     unsigned warn_away 	: 1 ;
     unsigned shock_sensor  : 1 ;
     unsigned ready_mode    : 1 ;
     unsigned reserved  	: 2 ;
//--------------------------------------------------------------------------//
	 unsigned dtc_code       : 1 ;
	 unsigned alert          : 1 ;
	 unsigned eng_run_change : 1 ;
	 unsigned panic_change   : 1 ;
	 unsigned rs_arm         : 1 ;
	 unsigned rs_siren       : 1 ;
	 unsigned rs_panic    	 : 1 ;
	 unsigned support_change : 1 ;
  }bit ;

  UInt8 byte[3] ;

}Rftd2_Status ;

//--------------------------------------------------------------------------//

typedef struct s_Rftd2
{
  volatile Rftd2_Status status ;
  volatile Rftd2_Status prev_status ;
}Rftd2 ;

//--------------------------------------------------------------------------//

typedef struct s_Rftd2_Entry
{
  volatile Rftd2_Status support ;
}Rftd2_Entry ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean rftd2_init_nvfs 			    (void);
void 	rftd2_dtc_alert_on  		    (void * arg);
void 	rftd2_dtc_alert_off 		    (void * arg);
void 	rftd2_dtc_alert_update 		    (void * arg);
void    rftd2_dtc_alert_update_ign_on   (void * arg);
void    rftd2_dtc_alert_update_ign_off  (void * arg);
Boolean rftd2_update_support_change     (void);
void 	rftd2_reset 				    (void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Rftd2 	   rftd2 ;
extern Rftd2_Entry rftd2_entry ;

extern const NVFS_Entry_Name nvfs_rftd2_entry ; 

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#endif




