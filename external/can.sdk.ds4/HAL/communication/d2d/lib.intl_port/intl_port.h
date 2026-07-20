/** 
 * @file   intl_port.h
 * @brief  
 * author  DIRECTED ELECTRONICS
 * @date   7/20/2015
 */

#ifndef __intl_port_h__
#define __intl_port_h__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
#include "q.h"
#include "d2d_switch.h"


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef struct s_Uart_Port Intl_Port;

//d2d configuration related
struct s_D2D_Entry
{
  SInt16 code       ;
  UInt8  nbyte      ;
  SInt16 input_code ;
} ;

typedef struct s_D2D_Entry D2D_Entry ;

typedef void (* D2D_Lut_Func) (void) ;

typedef struct s_D2D_Lut_Entry
{
  UInt8        code ;
  D2D_Lut_Func func ;

struct s_D2D_Lut_Entry * next ;

} D2D_Lut_Entry ;

typedef struct s_D2d_Ena_Bit
{    
  UInt8 d2d_mask[256/8] ;
  UInt8 d2d_ena [256/8] ;
  
}D2d_Ena_Bit ;

#define D2D_ERR_BUFF_SIZE 10
typedef struct s_D2d_Err
{
  UInt16         count                    ;
  D2D_Error_Code code [D2D_ERR_BUFF_SIZE] ;
}D2d_Error ;
//

enum
{
	AID_SCAN     ,
	AID_CLEARALL ,
	AID_POLL     ,
	AID_REVOKE   ,
	AID_SEARCH   ,
	AID_CLAIM    ,
	AID_ASSIGN   ,
};

#define UUID_SIZE 16
#define IS_SLAVE_ID(c) ((UInt8)((c)-0x20) < (0xE0-0x20)) // valid dynamic ID range is 0x20..0xDF only

extern const UInt8 default_aid_content[];
extern UInt8 my_aid_content[1 + UUID_SIZE]; // first byte is current aid.  the rest is UUID

#define my_id my_aid_content[0]   // my id byte
#define my_uuid (my_aid_content+1) // pointer to uuid

// bit masks for ApplicationCapability
#define APPCAP_SS      (1<<0) // alarm system
#define APPCAP_RS      (1<<1) // remote start
#define APPCAP_MTS     (1<<2) // manual transmission
#define APPCAP_TEMP    (1<<3) // cam request temperature
#define APPCAP_SENSADJ (1<<4) // can adjust sensors
#define APPCAP_DEFOG   (1<<5) // can control defroster

#define APPCAP_LEGACY  (1<<15) // pre-DS4 main module attached

//--------------------------------------------------------------------------//
#define BLUE_LED_FLASH_ACK_ON               200
#define BLUE_LED_FLASH_ACK_OFF              200

//--------------------------------------------------------------------------//



//mainp addition
#define MAINP_INST_ALLOWED      (1<<7)
//#define MAINP_POLL_RATE         (1<<6) //0: slow adv, 1: fast adv. no need since BLE card poll rate using sleep/wake state
//  reserved                    (1<<5)
//  reserved                    (1<<4)		/**< the fob present bit is handled by the BLE card itself */

// enum in bits 3..0
#define MAINP_STATE_MASK        0x0F
// values for mainp state:
#define MAINP_DISARMED 0
#define MAINP_PAIRING  1
#define MAINP_ERASEALL 2
#define MAINP_CLRTACH  3
#define MAINP_ZAP      4
// reserved			   5
#define MAINP_ARMED    6
// reserved            7
/* definition for ignition */
#define MAINP_DISARMED_IGN 0x8
#define MAINP_DISARMED_IGN_PAIR    0x9
#define MAINP_DISARMED_IGN_ERASE   0xA
#define MAINP_DISARMED_IGN_CLRTACH 0xB
#define MAINP_DISARMED_IGN_ZAP	   0xC
//#define MAINP_ARMED_IGN    Could not be armed and ignition, it will trigger alarm */

#define MAINP_DRIVING  0xD		// the MAINP_DRIVING could be replaced by IGNITION combination
#define MAINP_SCANNING 0xE
#define MAINP_MAINT    0xF



/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
//master (internal port)
void    intl_port_init(void);

void    pke_module_init(void);

void    proto_sendmsg(UInt8 proto, const UInt8 *p_data, UInt8 plen);
int     proto_sendmsg_wouldblock(void);  // call inside CRTITICAL, returns true if proto_sendmsg would block due to not enough buffers

void    intl_port_scpp_feedback(UInt8 hhu_no, UInt8 *payload, UInt8 plen);
void 		intl_port_sensor_proto_req( UInt8 *payload, UInt8 size);

void    d2d_lut_insert (UInt8 d2d_code , D2D_Lut_Func func);
Boolean d2d_lut_service (UInt8 d2d_code);
Boolean d2d_get_mask(UInt8 code);
Boolean d2d_get_ena(UInt8 code);
void    d2d_convert(D2D_Entry * d2d_entry);
Boolean d2d_cmd_enabled(UInt8 cmd);
SInt16  d2d_rx(TskTimeout   timeout);                                          
void    mainp_status(UInt8 sts);
void    intl_k2g_status_init(void);
UInt8   mainp_last_staus_get(void);    
void    request_ble_info(void);
void d2d2_update_siren_to_ble(UInt8 num, UInt8 delay);
void change_mainp_state(UInt8 newstate);
void rewind_scanning_timer(void);
void d2d2_func_send(UInt8 funcnum);
void d2d2_func_delete_unit(UInt8 unit);

void save_normal_fail_dbg_msg(UInt8 who);
void finish_ble_info_scan(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Intl_Port intl_port;
extern Q intl_port_txmsg_free;
extern Q intl_port_txmsg_pending;
extern UInt8 failing_units;
extern UInt8 have_devices;

extern const D2D_Entry input_table[] ;
extern const UInt8 INPUT_NUM_FUNC;
extern D2d_Ena_Bit     d2d_ena_table ;
extern D2d_Error       d2d_err       ;
extern const    D2d_Ena_Bit     d2d_ena_default ;


extern UInt8 d2d2_rftd_carrier[5];  //used to carry RFTD data
extern UInt8 fw_init_is_done;

extern UInt16 my_appcap;
extern UInt32 ble_was_detected;
#define BLE_WAS_DETECTED  ((UInt32)0x424C4544)


extern void d2d2_func_multicast_send(UInt8 funcid);

extern UInt32 timeserv_get(void); // terrible name for binary time service
extern void body_forbid(void); // disable pairing bodydance sequence because ivu or valet switch is present on system
extern void body_prk_set_level(UInt8 level);

#endif

