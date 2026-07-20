/*==========================================================================*/
// $Id: sensor_proto_private.h 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

#ifndef __SENSOR_PROTO_PRIVATE_H_
#define __SENSOR_PROTO_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "wake.h"
#include "timeout.h"
#include "q.h"
#include "nvfs_usr.h"
#include "nvfs.h"  
#include "trace.h"
#include "cmd.h"
#include "notify.h"
#include "stdarg.h"
#include "vrtc_f.h"
#include "wdog_v.h"
#include "d2d.h"
#include "intl_port.h"
#include "sensor_proto.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef DEBUG_SENSOR_PROTO
#define DEBUG_SENSOR_PROTO			0
#endif

#if DEBUG_SENSOR_PROTO == 1
#define TRACE_SENSOR_PROTO			TRACE
#else
#define TRACE_SENSOR_PROTO(...)		;
#endif

#define TASK_SENSOR_PROTO_TX_NAME  				"sensor_proto_tx"
#define TASK_SENSOR_PROTO_TX_PRI   				10000
#define TASK_SENSOR_PROTO_TX_STK   				0x0130
#define TASK_SENSOR_PROTO_TX_Q     				4


//--------------------------------------------------------------------------//

//--------------------------------------------------------------------------//

typedef UInt8      Sensor_Proto_Payload ;

//--------------------------------------------------------------------------//


typedef struct s_Sensor_Proto_Tx_Frame
{
  Q_Msg               	base      ;

	Sensor_Proto_Payload 	pld[10];
	UInt8 								pld_size;

}Sensor_Proto_Tx_Frame ; 




//--------------------------------------------------------------------------//

//#define	DEI_SC_VRTC_TIMEOUT_FIRST_RF_UPDATE		VRTC_1SEC*30		//30 seconds for first update
#define	DEI_SC_VRTC_TIMEOUT_RF_UPDATE			VRTC_1SEC*60*60		//1h for next updates
#define DEI_SC_VRTC_TIMEOUT_LOCK_ZONE_ACTIVE 	VRTC_1SEC*2			//2 seconds

#define DEI_SC_COUNT_RF_UPDATE					4					//update the remote for maximum 4 times


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void sensor_proto_tx_tsk( void );
void sensor_temp_update( SInt16 temp_val );
void sensor_shock_update( UInt8 shock_val );
void sensor_tilt_update( UInt8 tilt_val );
void sensor_proto_armdisarm(void);
Sensor_Proto_Payload* sensor_proto_payload_build( UInt8 pld_num , UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len );


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Q	sensor_proto_tx_q_free;
extern Q   sensor_proto_tx_q_pending        ;
extern UInt8 pld_size;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void extsensor_init(void);
void diag(UInt8 who, UInt8 what);

//--------------------------------------------------------------------------//

#endif




