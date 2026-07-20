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

//--------------------------------------------------------------------------//
#define BLUE_LED_FLASH_ACK_ON               200
#define BLUE_LED_FLASH_ACK_OFF              200

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
//master (internal port)
void    intl_port_init(void);
void    proto_sendmsg(UInt8 proto, const UInt8 *p_data, UInt8 plen);
void    intl_port_scpp_feedback(UInt8 hhu_no, UInt8 *payload, UInt8 plen);
void 		intl_port_sensor_proto_req( UInt8 *payload, UInt8 size);

void    d2d_lut_insert (UInt8 d2d_code , D2D_Lut_Func func);
void    d2d_lut_service (UInt8 d2d_code);
Boolean d2d_get_mask(UInt8 code);
Boolean d2d_get_ena(UInt8 code);
void    d2d_convert(D2D_Entry * d2d_entry);
Boolean d2d_cmd_enabled(UInt8 cmd);
SInt16  d2d_rx(TskTimeout   timeout);                                          
void    mainp_status(UInt8 sts);
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
extern UInt8 ble_card_present;
#endif

