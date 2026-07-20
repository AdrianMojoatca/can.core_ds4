/** 
 * @file   d2d_switch.h
 * @brief  
 * author  DIRECTED ELECTRONICS
 * @date   7/17/2015
 */

#ifndef __D2D_SWITCH_H__
#define __D2D_SWITCH_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "q.h"
#include "gpio.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
enum
{
    D2D_NULL  ,
    D2D_TYPE1 ,
    D2D_TYPE2 ,
    D2D_TYPE_VALET,   //depreciated, leave here for now
  
    D2D_TYPE_I,
		D2D_TYPE_3P,
};

#define IER_RBR     0x01
#define IER_THRE    0x02
#define IER_RLS     0x04

#define IIR_PEND    0x01
#define IIR_RLS     0x03
#define IIR_RDA     0x02
#define IIR_CTI     0x06
#define IIR_THRE    0x01

#define LSR_RDR     0x01
#define LSR_OE      0x02
#define LSR_PE      0x04
#define LSR_FE      0x08
#define LSR_BI      0x10
#define LSR_THRE    0x20
#define LSR_TEMT    0x40
#define LSR_RXFE    0x80

typedef enum
{
/* D2D Comunication Error */
   D2D_ERR_OVERRUN = 1,
   D2D_ERR_PARITY     ,
   D2D_ERR_FRAMING    ,
   D2D_ERR_BREAK      ,
   D2D_ERR_RXFE       , 
   D2D_ERR_TIMEOUT    ,

   D2D_ERR_COUNT 

}D2D_Error_Code ;

enum
{
    INTERNAL_PORT,     //master
    UART_PORT0   ,     //under dash port
    UART_PORT1   ,     //upper dash port
    UART_PORT3   ,     //debug port.  NGMM BLE
                         //UART2 is used by ISO, reserved for IF firmware
    VALET_PORT
};

static const Gpio_Pl_T uart_pl_tx[] =
{
    0,                  //reserved for internal port
  GPIO_PL_D2D_TX,
  GPIO_PL_RF_D2D_TX,
  GPIO_PL_DBG_PORT_TX,
};

static const Gpio_Pl_T uart_pl_rx[] =
{
    0,                  //reserved for internal port
  GPIO_PL_D2D_RX,
  GPIO_PL_RF_D2D_RX,
  GPIO_PL_DBG_PORT_RX,
};


//definition of protocol ID
#define PROTO_AUTH       0x01  // add this to any protocol id to include 4 auth bytes
#define PROTO_INFO       0x02
#define PROTO_AID        0x04
#define PROTO_DHP        0x06
#define PROTO_TEST       0x08
#define PROTO_IDMAP      0x0A
#define PROTO_MAINP      0x0C
#define PROTO_TIME       0x0E
#define PROTO_D2D        0x10
#define PROTO_SCPP       0x12
#define PROTO_SENS       0x14
#define PROTO_INTL       0x16
#define PROTO_FUNC       0x18
#define PROTO_M2M        0x1A
#define PROTO_CMDQ       0x1E

#define PROTO_APPCAP     0xDA
#define PROTO_DBG        0xDC
//#define PROTO_D2DECHO    0xDE  // TEST ONLY! do not enable this on standard product!
#define PROTO_DIAG       0xE0   // error

#define PROTO_NVFTP      0xF2
#define PROTO_FIO        0xF4
#define PROTO_FAIL       0xFA


// diagnostic codes used by PROTO_DIAG  (reasons for dropping)
#define DIAG_NONE      0  // no other information provided
#define DIAG_NEEDAUTH  1  // protocol needs auth
#define DIAG_ORIGIN    2  // not allowed on this port 
#define DIAG_PARAM     3  // some bad parameter in packet
#define DIAG_MODE      4  // wrong state or mode for this packet
#define TMSTAMP_FMT_U32SEC 1

//bus
//--------------------------------------------------------------------------//
#define CHAR_ERROR        0x00
#define CHAR_ENDMSG       0x01
#define CHAR_SHIFT        0x02
#define CHAR_SHIFT_AMOUNT 0x80 //ONLY IN DATA PACKET
#define CHAR_SLEEP        0x10
#define CHAR_MAINTENANCE  0x40
#define CHAR_OPEN         0xFF

//signals to task
#define TO_MASTER      1
#define TO_OPEN        2

#define BUS_SLEEP        0
#define BUS_MAINT        1
//#define BUS_WAKE       2
#define BUS_MASTER_DELAY 2
#define BUS_MASTER       3
#define BUS_OPEN         4
#define BUS_DATA         5
#define BUS_SHIFT        6
#define BUS_ENDMSG       7
// special states for master only
#define BUS_TXDATA       8  // I am sending
#define BUS_TXSHIFT      9
#define BUS_TXENDMSG     10
#define BUS_MAINT_END    11


//--------------------------------------------------------------------------//
//API between D2D and SCPP
#define D2D_SCPP_TX_LEN   5  //header (1 byte) + cmd (1 byte) + pld (3 bytes)
                             //header will be handled by IVU
#define D2D_SCPP_PLD_LEN  4



//! definition of d2d msg switch
#define SMSG_LG2SIZE  5    //2^5  
#define SMSG_SIZE    (1<<SMSG_LG2SIZE) //longer d2d message: AID+PID(D2D1)+2WAY MSG(4)=6
#define PORTS         (4+1)                 //0: INTERNAL PORT, UART0~3: in NGMM, UART0, 1 used for d2d0/1/2, 
                                        //      UART2 for digital app (ISO), UART3 for BLE
																				// From hardware ver 7, swaps the functionality of UART3 and UART0, UART0 for BLE
#define SMSG_MAX      27                //(24+3) must be greater (PORTS*PORTS)

typedef UInt8 Smsg;

#define D2D2_TXTRIES_LIMIT 3

#define D2D_SWITCH_Q_TIMEOUT 50

typedef void VP2A(void *, unsigned);
typedef void VP1A(void *);
typedef Boolean BP2A(void *, unsigned);

#define DETECT_SIZE  4

struct s_Smsg_Port
{
    VP2A *sendfunc;
    VP1A *txirq;
    VP2A *rxirq;
    VP1A *timeoutfunc;
    BP2A *filterfunc;

    // d2d2 variables
    UInt8 number;   // which port are we according to the switch?
    UInt8 busstate; 
    UInt16 itimer;
    UInt8  wofs_hist;
    UInt8  rx_hist[DETECT_SIZE];  
    UInt8 pre_wake;
} ;

typedef struct s_Smsg_Port SmsgPort;

struct s_Uart_Port
{
    // necessary switch variable must be first  //TODO: replace with s_Smsg_Port
    SmsgPort smsgbase;
    
    UInt8 givechance;

    Smsg txpending; // queue of pending Smsgs to send
    UInt8 *txbuf;
    UInt8 txlen;
    UInt8 txofs;
    UInt8 txtries;

    Smsg rxmsg;
    UInt8 *rxbuf;
    UInt8 rxlen;

    UInt16 crc;
// DEBUG variables...
    UInt8 last_char;
};

//! end of def d2d msg switch


//reserved AID for ports
#define AID_MASTER       ((UInt8)(~1))   //0xFE
#define AID_UART0        0xFD
#define AID_UART1        0xFC
#define AID_UART3        0xFB 


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
UInt8 smsg_getlen(Smsg id);
void  smsg_setlen(Smsg id, UInt8 len);
UInt8 smsg_getport(Smsg id);
UInt8 *smsg_getptr(Smsg id);
void  smsg_free(Smsg id);
Smsg  smsg_alloc(UInt8 srcportnumber);
int   smsg_is_lowmemory(void);

void  smsg_enq(Smsg *q, Smsg id);
Smsg  smsg_deq(Smsg *q);
UInt8 smsg_register(UInt8 port, void *pstruct);

void  smsg_submit(Smsg msg);

//port timer delta, 1ms base
#define PORT_TIMER_SECOND 1000 // number of 'ticks' per second
#define PORT_TIMER_MS(amt) ((amt)*PORT_TIMER_SECOND/1000)

void  port_timer_rewind(void *port,UInt16 amt);

void  d2d_switch_init(void);

void d2d_switch_port_tx(void *port,UInt8 c);
void d2d_switch_port_tx_irq_ctrl(void *port, Boolean ctrl);
void d2d_switch_port_rx_irq_ctrl(void *port, Boolean ctrl);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
//share for all ports
extern const UInt8 ham8[256];
extern SmsgPort *smsg_port[PORTS];

extern Q smsg_notify_free;
extern Q smsg_notify_pending;

extern UInt8 uart_config[3];   //uart0, 1, 3
extern UInt8 hold_bus_off;
extern UInt8 intl_port_is_waiting;
#endif

