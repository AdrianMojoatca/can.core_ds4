/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_bus_private.h 34922 2015-11-02 21:11:00Z martin.bouchard $
/*==========================================================================*/

#ifndef __DST_BUS_PRIVATE_H__
#define __DST_BUS_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
#include "dst_bus.h"
#include "dst.h"
#include "q.h"
#include "tmr.h"
#include "nvfs_usr.h"
#include "timeout_f.h"
#include "dbg.h"
#include "core_contract_db3_dst_gpio.h"

#if DBG_DST_BUS == 1
  #warning "DBG_DST_BUS Is Enable"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define TASK_DST_BUS_NAME       "dst bus"
#define TASK_DST_BUS_PRI        60000
#define TASK_DST_BUS_STK        0x0200



//--------------------------------------------------------------------------//

#define DST_BUS_ERR_BUFF_SIZE   5
//--------------------------------------------------------------------------//

#ifdef DST_BUS_J1850
  #warning "Dst Bus Reader on J1850 Bus"
  
  #define  DST_BUS_READER_RX            J1850_RX 
  #define  DST_BUS_READER_RX_RISING     core_contract_db3_dst_gpio_int_stat_f (CORE_DB3_DST_GPIO_J1850_RX)
  #define  DST_BUS_READER_RX_FALLING    core_contract_db3_dst_gpio_int_stat_r (CORE_DB3_DST_GPIO_J1850_RX)
  #define  DST_BUS_READER_RX_INT_DIS    core_contract_db3_dst_gpio_irq_dis    (CORE_DB3_DST_GPIO_J1850_RX)
  #define  DST_BUS_READER_RX_INT_ENA    core_contract_db3_dst_gpio_irq_ena    (CORE_DB3_DST_GPIO_J1850_RX)
  #define  DST_BUS_READER_RX_INT_CLR    core_contract_db3_dst_gpio_int_clr    (CORE_DB3_DST_GPIO_J1850_RX)

#else
  #define  DST_BUS_READER_RX            ISO1_RX 
  #define  DST_BUS_READER_RX_RISING     core_contract_db3_dst_gpio_int_stat_f (CORE_DB3_DST_GPIO_ISO1_RX)
  #define  DST_BUS_READER_RX_FALLING    core_contract_db3_dst_gpio_int_stat_r (CORE_DB3_DST_GPIO_ISO1_RX)
  #define  DST_BUS_READER_RX_INT_DIS    core_contract_db3_dst_gpio_irq_dis    (CORE_DB3_DST_GPIO_ISO1_RX)
  #define  DST_BUS_READER_RX_INT_ENA    core_contract_db3_dst_gpio_irq_ena    (CORE_DB3_DST_GPIO_ISO1_RX)
  #define  DST_BUS_READER_RX_INT_CLR    core_contract_db3_dst_gpio_int_clr    (CORE_DB3_DST_GPIO_ISO1_RX)
#endif

#define DST_BUS_BURST_OK            !!DST_BUS_READER_RX 

#define  DST_BUS_TAG_IO_RX         ISO2_RX
#define  DST_BUS_TAG_RX_RISING     core_contract_db3_dst_gpio_int_stat_r (CORE_DB3_DST_GPIO_ISO2_RX)
#define  DST_BUS_TAG_RX_FALLING    core_contract_db3_dst_gpio_int_stat_f (CORE_DB3_DST_GPIO_ISO2_RX)
#define  DST_BUS_TAG_RX_INT_DIS    core_contract_db3_dst_gpio_irq_dis    (CORE_DB3_DST_GPIO_ISO2_RX)
#define  DST_BUS_TAG_RX_INT_ENA    core_contract_db3_dst_gpio_irq_ena    (CORE_DB3_DST_GPIO_ISO2_RX)
#define  DST_BUS_TAG_RX_INT_CLR    core_contract_db3_dst_gpio_int_clr    (CORE_DB3_DST_GPIO_ISO2_RX)

#define  DST_BUS_INT_DIS          DST_BUS_READER_RX_INT_DIS ;  DST_BUS_TAG_RX_INT_DIS
#define  DST_BUS_INT_ENA          DST_BUS_READER_RX_INT_ENA ;  DST_BUS_TAG_RX_INT_ENA

#define DST_BUS_TX_SET            ISO2_TX_SET
#define DST_BUS_TX_CLR            ISO2_TX_CLR


//--------------------------------------------------------------------------//

#define CPU_CLOCK_MULT               72

//--------------------------------------------------------------------------//

#define DST_BUS_READER_BURST_TIMEOUT (6000  * CPU_CLOCK_MULT)
#define DST_BUS_READER_BURST_MIN     (45000 * CPU_CLOCK_MULT)
#define DST_BUS_READER_BURST_MAX     (75000 * CPU_CLOCK_MULT)


//--------------------------------------------------------------------------//

#define DST_BUS_ENCRYPTION_TIME_MIN  (5000  * CPU_CLOCK_MULT)
#define DST_BUS_ENCRYPTION_TIME_MAX  (50000 * CPU_CLOCK_MULT)
//--------------------------------------------------------------------------//

//#define DST_BUS_TAG_TX_BYTE_TIMEOUT (1000 * CPU_CLOCK_MULT) //1 ms

#define DST_BUS_TAG_RX_DATA_TIMEOUT (1000 * CPU_CLOCK_MULT) //1 ms//(4000 * CPU_CLOCK_MULT) //4 ms
#define DST_BUS_TAG_TX_DATA_TIMEOUT (3100 * CPU_CLOCK_MULT) //- DST_BUS_READER_DATA_MAX

//--------------------------------------------------------------------------//

#define DST_BUS_BLANK_VALUE NULL

#define DST_BUS_TAG_SYNC      0x50
#define DST_BUS_TAG_ID        0x7E
#define DST_BUS_TAG_DATA_SIZE 10

//--------------------------------------------------------------------------//

#define DST_BUS_CMD_DIS         INPUT_GWR         | INPUT_ON
#define DST_BUS_CMD_ENA         INPUT_GWR 


#define DST_BUS_CMD_RS_ON       INPUT_RS_IGNITION | INPUT_ON
#define DST_BUS_CMD_RS_OFF      INPUT_RS_IGNITION 

//--------------------------------------------------------------------------//

typedef enum
{
  DST_BUS_FLAG_READER       = (1U << 0) ,
  DST_BUS_FLAG_TAG_RX_BYTE  = (1U << 1) ,
  DST_BUS_FLAG_TAG_RX_DONE  = (1U << 2) ,


}Dst_Bus_Flags ;

//--------------------------------------------------------------------------//

typedef enum
{
   NO_DST_BUS_ERR               ,

   DST_BUS_ERR_READER_NO_Q_FREE ,
 
   DST_BUS_ERR_PWD              ,
 
   DST_BUS_ERR_RX_OVERRUN       ,
   DST_BUS_ERR_RX_FRAMING       ,

   DST_BUS_ERR_RX_SIZE          ,

   DST_BUS_ERR_RESP_DATA_NOT_READY ,

   DST_BUS_ERR_COUNT 

}Dst_Bus_Error_Code ;

//--------------------------------------------------------------------------//

typedef struct s_Dst_Bus_Error
{
    UInt16             count                        ;
    
    Dst_Bus_Error_Code code [DST_BUS_ERR_BUFF_SIZE] ;
      
} Dst_Bus_Error ;

//--------------------------------------------------------------------------//

typedef struct s_Dst_Bus_Msg
{
  UInt8  size      ;
  UInt8  data [30] ;   

}Dst_Bus_Msg ;
//--------------------------------------------------------------------------//

typedef void (* Task_Callback) (Dst_Bus_Msg);

//--------------------------------------------------------------------------//

typedef struct s_Dst_Bus_Q_Msg
{ 
  Q_Msg         q_base    ;
    
  Dst_Bus_Msg   msg       ;

  Task_Callback callback  ;

} Dst_Bus_Q_Msg ;
//--------------------------------------------------------------------------//


typedef struct s_Dst_Bus_Nvfs_Entry
{
  Boolean  is_prog           ;
  
  Boolean  reader_data_level ;

}Dst_Bus_Nvfs_Entry;

//--------------------------------------------------------------------------//

typedef struct s_Dst_Bus_Vars
{
  volatile Boolean       do_err_save    ;
                                    
  volatile Dst_Bus_Flags flag           ;
  volatile Boolean       flg_gwr        ; // flag for ground when running 1= activate, 0 = desactivate
  volatile Boolean       flg_ign        ;
           SInt16        fiq_rx_data    ;

  Dst_Bus_Msg       reader_msg          ;
  Dst_Bus_Msg       tag_msg             ;
  Dst_Bus_Msg       tx_msg              ;

  Dst_Trans_Resp    trans_resp          ;

  Q                 q_free              ;
  Q                 q_pending           ; 

  Tmr               tmr_tag_tx           ;
  Tmr               tmr_tag_rx           ;
  Tmr               tmr_tag_rx_timeout   ;

  Tmr               tmr_reader_rx_timeout;

  Tsk               tsk                  ;       

  Dst_Bus_Error     err                  ; // error logging buffer
 
  Dst_Bus_Nvfs_Entry  nvfs_entry         ;

  UInt32            tag_baudrate        ;

}Dst_Bus_Vars ;


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void dst_bus_fiq_service            (void);
void dst_bus_user_service           (void);

void dst_bus_task                   (void);
void dst_bus_tag_rx_done            (void);
void dst_bus_reset                  (void);

void dst_bus_ena                    (void * arg) ;
void dst_bus_dis                    (void * arg) ;
void dst_bus_ign_on                 (void * arg) ;
void dst_bus_ign_off                (void * arg) ;

void dst_bus_set_flag               (Dst_Bus_Flags      flag) ;
void dst_bus_clr_flag               (Dst_Bus_Flags      flag) ;

void dst_bus_reader_process         (Dst_Bus_Msg        msg)  ;
void dst_bus_trans_process          (Dst_Bus_Msg        msg)  ;

void dst_bus_err_add                (Dst_Bus_Error_Code code) ;

void dst_bus_bcc                    (UInt16 * crc , UInt8 *data, UInt8 size) ;

void dst_bus_dbg_print              (StrConst * text ,  const UInt8 data[] , UInt8 size);
Boolean dst_bus_init_nvfs           (void)                    ;
Boolean dst_bus_err_save            (void)                    ;

void    dst_bus_build_tx_msg        (Dst_Trans_Resp trans_resp) ;

void dst_bus_tx_irq_sync            (void)  ;
void dst_bus_tx_irq_service         (void)  ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern const NVFS_Entry_Name nvfs_dst_bus_data   ;
extern const NVFS_Entry_Name nvfs_dst_bus_error  ;

extern       Dst_Bus_Vars    dst_bus_vars        ;

#endif

