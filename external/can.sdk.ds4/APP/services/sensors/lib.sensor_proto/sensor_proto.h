/*==========================================================================*/
// $Id: dei_sc.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef _SENSOR_PROTO_H_
#define _SENSOR_PROTO_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define		SENSOR_PROTO_READ_STAT_RESP						0xF0
#define		SENSOR_PROTO_WRITE_REP_BIT_RESP				0xF1
#define		SENSOR_PROTO_READ_CONFIG_RESP					0xF2
#define 	SENSOR_PROTO_WRITE_CONFIG_RESP				0xF3		
#define 	FUNC_PROTO_FUNC_TYPE_RESP					0xF4		
#define 	SENSOR_PROTO_SENSOR_TYPE_RESP					0xF4		
#define		SENSOR_PROTO_ALARM_LEVEL_RESP					0xF8
#define   SENSOR_PROTO_DIAGNOSTIC_RESP          0xFF

#define 	FUNC_PROTO_WRITE_STAT				0xE0
#define 	SENSOR_PROTO_READ_STAT_REQ				0xE0

#define		SENSOR_PROTO_WRITE_REP_BIT				0xE1
#define		SENSOR_PROTO_READ_CONFIG_REQ			0xE2
#define 	SENSOR_PROTO_WRITE_CONFIG_REQ			0xE3
#define 	SENSOR_PROTO_SENSOR_TYPE_REQ			0xE4
#define   SENSOR_PROTO_READ_ALARM_LEVEL			0xE8
#define   SENSOR_PROTO_WRITE_ALARM_LEVEL		0xE9
#define		SENSOR_PROTO_WRITE_DEFAULT				0xC1

#define 	FUNC_PROTO_MULTICAST_REMAP				0xEA
#define 	FUNC_PROTO_WRITE_CONFIG_REQ			  0xE3


// report/status bit mask definitions
#define DEFECT_MASK (1u << 7)
#define EXCEPTION_MASK (1u << 6)
#define TEST_MASK (1u << 3)
#define ALARM_MASK  (1u << 2)
#define WARN_MASK		(1u << 1)
#define NORMAL_MASK	(1u << 0)


#define PLD_E0		0
#define PLD_E1		1
#define PLD_E2		2
#define PLD_E3		3
#define PLD_E4		4
#define PLD_E8		5
#define PLD_E9		6
#define PLD_C1		7

//FIXME BUG: this is only true for NGSS!!
#define SHOCK		1
#define TILT		2
#define TEMP		3



typedef enum
{
	SENSOR_PROTO_ALRM_SRC				= 1u << 0 ,

	SENSOR_PROTO_CONFIG_RECV		= 1u << 1 ,

	SENSOR_PROTO_WRITE_CONFIG        = 1u << 2 ,

	//PRG_EVT_IGN_ON              = 1u << 3 ,

	//PRG_EVT_IGN_OFF             = 1u << 4 ,

	//PRG_EVT_DOOR_CLOSED         = 1u << 5 ,

	//PRG_EVT_LOCK                = 1u << 6 ,

	//PRG_EVT_REMOTE_PROGRAMMED   = 1u << 7 

}Sensor_Proto_Evt ; 


#define ALARM_SRC_RCV		1

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

void sensor_proto_cmd(UInt8 aid_sender, UInt8 cmd, UInt8 chanl, UInt8 len, UInt8 *pld);
void sensor_proto_device_lost(UInt8 devnum);
void sensor_proto_device_lost_force_notify(UInt8 devnum);
void  sensor_proto_tx( UInt8 pld_num, UInt8 dest_aid, UInt8 sens_chanl ,UInt8* pld, UInt8 pld_len);
void  sensor_proto_init    ( void ) ;
void sensor_proto_via_ble_enter(void);
void sensor_proto_via_ble_exit(void);
UInt8 sensor_proto_zonecheck(void); // returns which zone are in trouble
Boolean sensor_missing_safety(void);
Boolean sensor_missing_security(void);
void sensor_proto_delete_unit(UInt8 unit);
void sensor_proto_reminder(UInt8 unit);
void sens_process(UInt8 *pkt, UInt8 len);
void sensors_status_invalidate(void);

void send_identify_request(void);
UInt8 sensor_proto_get_factory_bits(void);
void sensor_proto_set_factory_bits(UInt8 bits);
void sensadj_sort(void);
EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern UInt8 dest_aid_req;
extern Evt sensor_proto_evt;

extern UInt8 ble_siren_temp_avail;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/



#endif



