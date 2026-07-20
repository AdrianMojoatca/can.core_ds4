/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __D2D_PRIVATE_H__
#define __D2D_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "irq_user.h"
#include "d2d.h"
#include "wake.h"
#include "tmr.h"
#include "dbg.h"

#if DBG_D2D == 1
  #warning "DBG_D2D Is Enable"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// <h>  Task 'd2d' settings
// <s0> Name
// <o0> Priority           <0-65535>
// <i>  Lowest  priority = 0
// <i>  Highest priority = 65535
// <o1> Stack Size (bytes) <0x0000-0xFFFF:4>
// <i>  Must be a multiple of 4.
// </h>
#define TASK_D2D_NAME  "d2d"
#define TASK_D2D_PRI   8000
#define TASK_D2D_STK   0x0100

//--------------------------------------------------------------------------//

#define D2D_GET_TIMEOUT 5000
#define D2D_Q_TIMEOUT   1000

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define D2D_ERR_BUFF_SIZE 10

#define UART0_BASE ((volatile UInt32 *) 0xE000C000) 
#define UART1_BASE ((volatile UInt32 *) 0xE0010000)
#define UART2_BASE ((volatile UInt32 *) 0xE0078000)
#define UART3_BASE ((volatile UInt32 *) 0xE007C000) 

typedef volatile UInt32 * Uart_IO_Base ;

//--------------------------------------------------------------------------//

struct s_D2D_Entry
{
  SInt16 code       ;
  UInt8  nbyte      ;
  SInt16 input_code ;
} ;

//--------------------------------------------------------------------------//

typedef struct s_D2D_Lut_Entry
{
  UInt8        code ;
  D2D_Lut_Func func ;

  struct s_D2D_Lut_Entry * next ;

} D2D_Lut_Entry ;

//--------------------------------------------------------------------------//

typedef struct s_D2d_Ena_Bit
{    
  UInt8 d2d_mask[256/8] ;
  UInt8 d2d_ena [256/8] ;
  
}D2d_Ena_Bit ;

//--------------------------------------------------------------------------//
typedef struct s_D2D_Func
{
  D2D_Tx_Func       tx       ;
  D2D_Tx_Frame_Func tx_frame ;
  D2D_Get_Func      get_mask ;
  D2D_Get_Func      get_ena  ;

}D2D_Func ;
//--------------------------------------------------------------------------//

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

//--------------------------------------------------------------------------//

typedef struct s_D2d_Err
{

  UInt16         count                    ;

  D2D_Error_Code code [D2D_ERR_BUFF_SIZE] ;
 
  
}D2d_Error ;

//--------------------------------------------------------------------------//

typedef struct s_D2D_First_Byte
{
  UInt8   byte        ;
  Boolean validity    ;
  Boolean d2d_rx_swi  ;

} D2D_First_Byte ; 

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
Boolean d2d_init_nvfs           (void)                    ;

void    d2d_set_table           (D2d_Ena_Bit table)       ;

void    d2d_task                (void * tsk_arg)          ;
int     get_d2d_cmd             (TskTimeout timeout)      ;

void    d2d_irq_service         (void)                    ;
void    d2d_wake                (Wake_Status wake_status) ;
Boolean d2d_wake_ready          (void)                    ;

void    d2d_fiq_service         (void)                    ;
void    d2d_lut_service         (UInt8 d2d_code)          ;

D2D_Lut_Entry * d2d_lut_search  (UInt8 d2d_code)          ;

void    d2d_reply               (UInt8 data)              ;

void    d2d_tx_request          (void)                    ;
void    d2d_tx_release          (void)                    ;

void    d2d_err_add             (D2D_Error_Code code)     ;

Boolean d2d_err_save            (void)                    ;

void    d2d_gpio                (void)                    ;

void    d2d_exception_gwr_enc      (void) ;
void    d2d_exception_read_eeprom  (void) ;
void    d2d_exception_write_eeprom (void) ;

void    d2d_reset                  (void) ;

void    d2d_tx_code            (UInt8 code) ;
void    d2d_tx_frame_data      (UInt8 * data ,UInt8 size) ;

void    d2d_code_dis        (UInt8 code)                        ;
void    d2d_code_ena        (UInt8 code)                        ;

Boolean d2d_get_mask_func   (UInt8 code)                        ;
Boolean d2d_get_ena_func    (UInt8 code)                        ;

UInt8   input_to_d2d        (UInt16 data)                       ;
void    d2d_convert         (D2D_Entry  * d2d_entry)            ;



/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Tsk             tsk_d2d       ;
extern Irq             d2d_irq_tx    ;
extern Irq             d2d_irq_rx    ;
extern Res             d2d_tx_res    ;
extern D2d_Ena_Bit     d2d_ena_table ;
extern D2D_Lut_Entry * d2d_lut_list  ;
extern D2d_Error       d2d_err       ;
extern D2D_Func        d2d_func      ;

extern volatile SInt16          d2d_rda_rx      ;
extern volatile UInt8           d2d_gpio_state ;
extern volatile D2D_First_Byte  d2d_first_byte ;

extern const    D2d_Ena_Bit     d2d_ena_default ;

extern Boolean d2d_do_err_save ;

extern Wake_Status d2d_wake_status ;

extern UInt8 d2d_echo_idx, d2d_echo_cnt;
#define D2D_ECHO_SIZE 8
extern UInt8 d2d_echo_buf[D2D_ECHO_SIZE];

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
#ifdef PLATFORM_404
#define D2DLSR U1LSR
#define D2DIIR U1IIR
#define D2DRBR U1RBR
#define D2DIER U1IER
#else // dball
#define D2DLSR U0LSR
#define D2DIIR U0IIR
#define D2DRBR U0RBR
#define D2DIER U0IER
#endif

/*==========================================================================*/

#endif

