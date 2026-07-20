/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ask_private.h 31490 2015-05-13 15:36:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __DEI_ASK_PRIVATE_H__
#define __DEI_ASK_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ask.h"
#include "trace.h"
#include "tmr_irq.h"
#include "irq_user.h"
#include "wake.h"
#include "fiq.h"
#include "isr.h"
#include "config_gpio.h"
#include "800_gpio.h"
#include "keeloq.h"
#include "sync_count.h"
#include "cmd.h"

#include "nvfs_usr.h"
#include "nvram_usr.h"
#include "nvfs.h"  
#include "notify.h"
#include "wdog_v.h"

#include <string.h>
#include "dbg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if DEBUG_DEI_ASK == 1
  #define TRACE_DEI_ASK			TRACE
  #warning "TRACE_DEI_ASK Is Enable"
#else
  #define TRACE_DEI_ASK(...)		;
#endif

//---------------------------------------------------------------------------//

typedef void (*Dei_Ask_State )( Boolean is_rising_edge );

//---------------------------------------------------------------------------//

typedef enum
{
    DEI_ASK_BUTTON_LOCK         = 0x02 , 
    DEI_ASK_BUTTON_UNLOCK       = 0x04 ,
    DEI_ASK_BUTTON_START_STOP   = 0x01 ,
    DEI_ASK_BUTTON_TRUNK        = 0x08 ,
    DEI_ASK_BUTTON_AUX1         = 0x0A ,
    DEI_ASK_BUTTON_AUX2         = 0x0C ,
	DEI_ASK_BUTTON_CAR_FINDER   = 0x09 ,
    
    DEI_ASK_BUTTON_UNKNOWN      = 0xFF
}Dei_Ask_Button;

//---------------------------------------------------------------------------//

typedef enum
{
    DEI_ASK_FIRST_PRESS , 
    DEI_ASK_EXTENDED_PRESS , 
    DEI_ASK_RELEASE

}Dei_Ask_Press_Type;

//---------------------------------------------------------------------------//

typedef enum
{ 
    DEI_ASK_BIT_HEADER   ,
    DEI_ASK_BIT_HIGH     ,
    DEI_ASK_BIT_LOW      ,
    DEI_ASK_BIT_ERROR    ,
    
    DEI_ASK_BIT_NOT_DECODED

}Dei_Ask_Bit;

//---------------------------------------------------------------------------//

typedef enum
{
    DEI_ASK_BIT_CHECK_LOW_TIME ,
    DEI_ASK_BIT_CHECK_HIGH_TIME,
    
    DEI_ASK_BIT_CHECK_BOTH

}Dei_Ask_Time_Bit_Check_What;

//---------------------------------------------------------------------------//

typedef struct s_Dei_Ask_Bit_Time_Checker
{
    UInt32                        t_low_min ; 
    UInt32                        t_low_max ;
    
    UInt32                        t_high_min;
    UInt32                        t_high_max;
    
    Dei_Ask_Bit                   bit       ;     
    Dei_Ask_Time_Bit_Check_What   check_what;

}Dei_Ask_Bit_Time_Checker;

//---------------------------------------------------------------------------//

typedef union s_Fixed_Portion
{
    struct
    {   unsigned sn     :28;
        unsigned button :4;
        unsigned v_low  :1;
        unsigned repeat :1;
        unsigned dummy  :30;                                
    }bits;
    
    UInt64 data;
    
}Fixed_Portion;

typedef union s_Encrypted_Portion
{
    struct
    {
        unsigned sc     :16;
        unsigned disc   :10;
        unsigned ovr    :2;
        unsigned button :4;
        
        unsigned dummy  :32;
        
    }bits;
    
    UInt64 data;
    
}Encrypted_Portion;

//---------------------------------------------------------------------------//

#define DEI_ASK_MAX_REMOTES         4

typedef struct s_Dei_Ask_Remote_Entry
{
    UInt16  sync_count   ;
    UInt32  serial_number;
    UInt16  disc         ;
    Boolean is_1button   ;

}Dei_Ask_Remote_Entry;


typedef struct s_Dei_Ask_Remote_Database
{
    Dei_Ask_Remote_Entry remote[DEI_ASK_MAX_REMOTES];
    
    UInt16 index;
    
}Dei_Ask_Remote_Database;

//---------------------------------------------------------------------------//

typedef struct s_Dei_Ask
{    
	Dei_Ask_Bit_Time_Checker  bit_checker_header;
    Dei_Ask_Bit_Time_Checker  bit_checker_low   ;
    Dei_Ask_Bit_Time_Checker  bit_checker_high  ;
    
    
    UInt32  current_timestamp;
    UInt32  last_timestamp   ;
    
    UInt32  t_low ;
    UInt32  t_high;
    
    UInt8   bit_index;
    
    Encrypted_Portion  data_enc ;
    Fixed_Portion      data_fix ;
            
    UInt64  last_data_enc;
    UInt64  last_data_fix;
    
    int     irq_status;
    int     fiq_status;
    
    Irq     rx_irq;
    
    Boolean rx_complete;
    
    Dei_Ask_State state;
    
    Boolean programming_on;
    
    UInt8    last_button ; 
    UInt8    frame_count ;
    Boolean  last_is_1button;
    
    Dei_Ask_Remote_Database  database;
    
    Sync_Count_Obj           sync_count_obj[DEI_ASK_MAX_REMOTES];
    
}Dei_Ask;

//---------------------------------------------------------------------------//

#define DEI_ASK_ENC_PORTION_DATA_SIZE           32
#define DEI_ASK_FIX_PORTION_DATA_SIZE           34

#define DEI_ASK_CMD_Q_INSERT_TIMEOUT            500


#define TASK_DEI_ASK_NAME  "dei_ask"
#define TASK_DEI_ASK_PRI   10000
#define TASK_DEI_ASK_STK   0x0130

//---------------------------------------------------------------------------//
//CPU CLOCK FREQ.
//---------------------------------------------------------------------------//

#define DEI_ASK_TIME_BASE_72MHZ		72
#define DEI_ASK_TIME_BASE_12MHZ		12
#define DEI_ASK_TIME_BASE_6MHZ		6
#define DEI_ASK_TIME_BASE_4MHZ		4
#define DEI_ASK_TIME_BASE_3MHZ		3
#define DEI_ASK_TIME_BASE_2MHZ		2
#define DEI_ASK_TIME_BASE_1MHZ		1

//---------------------------------------------------------------------------//
//BIT TIME VALUES FOR 72MHZ
//---------------------------------------------------------------------------//
//More info at : http://www.microchip.com/wwwproducts/Devices.aspx?product=HCS300
//             : http://ww1.microchip.com/downloads/en/DeviceDoc/21137G.pdf

#define DEI_ASK_TE_MIN_72MHZ               260*DEI_ASK_TIME_BASE_72MHZ
#define DEI_ASK_TE_MAX_72MHZ               660*DEI_ASK_TIME_BASE_72MHZ

//HEADER BIT
#define DEI_ASK_HEADER_BIT_LOW_MIN_72MHZ   10*DEI_ASK_TE_MIN_72MHZ
#define DEI_ASK_HEADER_BIT_LOW_MAX_72MHZ   10*DEI_ASK_TE_MAX_72MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MIN_72MHZ  1*DEI_ASK_TE_MIN_72MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MAX_72MHZ  1*DEI_ASK_TE_MAX_72MHZ

//HIGH BIT
#define DEI_ASK_HIGH_BIT_LOW_MIN_72MHZ     2*DEI_ASK_TE_MIN_72MHZ
#define DEI_ASK_HIGH_BIT_LOW_MAX_72MHZ     2*DEI_ASK_TE_MAX_72MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MIN_72MHZ    1*DEI_ASK_TE_MIN_72MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MAX_72MHZ    1*DEI_ASK_TE_MAX_72MHZ

//LOW BIT
#define DEI_ASK_LOW_BIT_LOW_MIN_72MHZ      1*DEI_ASK_TE_MIN_72MHZ
#define DEI_ASK_LOW_BIT_LOW_MAX_72MHZ      1*DEI_ASK_TE_MAX_72MHZ
#define DEI_ASK_LOW_BIT_HIGH_MIN_72MHZ     2*DEI_ASK_TE_MIN_72MHZ
#define DEI_ASK_LOW_BIT_HIGH_MAX_72MHZ     2*DEI_ASK_TE_MAX_72MHZ

//---------------------------------------------------------------------------//
//BIT TIME VALUES FOR 12MHZ
//---------------------------------------------------------------------------//

#define DEI_ASK_TE_MIN_12MHZ               260*DEI_ASK_TIME_BASE_12MHZ
#define DEI_ASK_TE_MAX_12MHZ               660*DEI_ASK_TIME_BASE_12MHZ

//HEADER BIT
#define DEI_ASK_HEADER_BIT_LOW_MIN_12MHZ   10*DEI_ASK_TE_MIN_12MHZ
#define DEI_ASK_HEADER_BIT_LOW_MAX_12MHZ   10*DEI_ASK_TE_MAX_12MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MIN_12MHZ  1*DEI_ASK_TE_MIN_12MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MAX_12MHZ  1*DEI_ASK_TE_MAX_12MHZ

//HIGH BIT
#define DEI_ASK_HIGH_BIT_LOW_MIN_12MHZ     2*DEI_ASK_TE_MIN_12MHZ
#define DEI_ASK_HIGH_BIT_LOW_MAX_12MHZ     2*DEI_ASK_TE_MAX_12MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MIN_12MHZ    1*DEI_ASK_TE_MIN_12MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MAX_12MHZ    1*DEI_ASK_TE_MAX_12MHZ

//LOW BIT
#define DEI_ASK_LOW_BIT_LOW_MIN_12MHZ      1*DEI_ASK_TE_MIN_12MHZ
#define DEI_ASK_LOW_BIT_LOW_MAX_12MHZ      1*DEI_ASK_TE_MAX_12MHZ
#define DEI_ASK_LOW_BIT_HIGH_MIN_12MHZ     2*DEI_ASK_TE_MIN_12MHZ
#define DEI_ASK_LOW_BIT_HIGH_MAX_12MHZ     2*DEI_ASK_TE_MAX_12MHZ

//---------------------------------------------------------------------------//
//BIT TIME VALUES FOR 6MHZ
//---------------------------------------------------------------------------//

#define DEI_ASK_TE_MIN_6MHZ               260*DEI_ASK_TIME_BASE_6MHZ
#define DEI_ASK_TE_MAX_6MHZ               660*DEI_ASK_TIME_BASE_6MHZ

//HEADER BIT
#define DEI_ASK_HEADER_BIT_LOW_MIN_6MHZ   10*DEI_ASK_TE_MIN_6MHZ
#define DEI_ASK_HEADER_BIT_LOW_MAX_6MHZ   10*DEI_ASK_TE_MAX_6MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MIN_6MHZ  1*DEI_ASK_TE_MIN_6MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MAX_6MHZ  1*DEI_ASK_TE_MAX_6MHZ

//HIGH BIT
#define DEI_ASK_HIGH_BIT_LOW_MIN_6MHZ     2*DEI_ASK_TE_MIN_6MHZ
#define DEI_ASK_HIGH_BIT_LOW_MAX_6MHZ     2*DEI_ASK_TE_MAX_6MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MIN_6MHZ    1*DEI_ASK_TE_MIN_6MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MAX_6MHZ    1*DEI_ASK_TE_MAX_6MHZ

//LOW BIT
#define DEI_ASK_LOW_BIT_LOW_MIN_6MHZ      1*DEI_ASK_TE_MIN_6MHZ
#define DEI_ASK_LOW_BIT_LOW_MAX_6MHZ      1*DEI_ASK_TE_MAX_6MHZ
#define DEI_ASK_LOW_BIT_HIGH_MIN_6MHZ     2*DEI_ASK_TE_MIN_6MHZ
#define DEI_ASK_LOW_BIT_HIGH_MAX_6MHZ     2*DEI_ASK_TE_MAX_6MHZ

//---------------------------------------------------------------------------//
//BIT TIME VALUES FOR 4MHZ
//---------------------------------------------------------------------------//

#define DEI_ASK_TE_MIN_4MHZ               260*DEI_ASK_TIME_BASE_4MHZ
#define DEI_ASK_TE_MAX_4MHZ               660*DEI_ASK_TIME_BASE_4MHZ

//HEADER BIT
#define DEI_ASK_HEADER_BIT_LOW_MIN_4MHZ   10*DEI_ASK_TE_MIN_4MHZ
#define DEI_ASK_HEADER_BIT_LOW_MAX_4MHZ   10*DEI_ASK_TE_MAX_4MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MIN_4MHZ  1*DEI_ASK_TE_MIN_4MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MAX_4MHZ  1*DEI_ASK_TE_MAX_4MHZ

//HIGH BIT
#define DEI_ASK_HIGH_BIT_LOW_MIN_4MHZ     2*DEI_ASK_TE_MIN_4MHZ
#define DEI_ASK_HIGH_BIT_LOW_MAX_4MHZ     2*DEI_ASK_TE_MAX_4MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MIN_4MHZ    1*DEI_ASK_TE_MIN_4MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MAX_4MHZ    1*DEI_ASK_TE_MAX_4MHZ

//LOW BIT
#define DEI_ASK_LOW_BIT_LOW_MIN_4MHZ      1*DEI_ASK_TE_MIN_4MHZ
#define DEI_ASK_LOW_BIT_LOW_MAX_4MHZ      1*DEI_ASK_TE_MAX_4MHZ
#define DEI_ASK_LOW_BIT_HIGH_MIN_4MHZ     2*DEI_ASK_TE_MIN_4MHZ
#define DEI_ASK_LOW_BIT_HIGH_MAX_4MHZ     2*DEI_ASK_TE_MAX_4MHZ

//---------------------------------------------------------------------------//
//BIT TIME VALUES FOR 3MHZ
//---------------------------------------------------------------------------//

#define DEI_ASK_TE_MIN_3MHZ               260*DEI_ASK_TIME_BASE_3MHZ
#define DEI_ASK_TE_MAX_3MHZ               660*DEI_ASK_TIME_BASE_3MHZ

//HEADER BIT
#define DEI_ASK_HEADER_BIT_LOW_MIN_3MHZ   10*DEI_ASK_TE_MIN_3MHZ
#define DEI_ASK_HEADER_BIT_LOW_MAX_3MHZ   10*DEI_ASK_TE_MAX_3MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MIN_3MHZ  1*DEI_ASK_TE_MIN_3MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MAX_3MHZ  1*DEI_ASK_TE_MAX_3MHZ

//HIGH BIT
#define DEI_ASK_HIGH_BIT_LOW_MIN_3MHZ     2*DEI_ASK_TE_MIN_3MHZ
#define DEI_ASK_HIGH_BIT_LOW_MAX_3MHZ     2*DEI_ASK_TE_MAX_3MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MIN_3MHZ    1*DEI_ASK_TE_MIN_3MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MAX_3MHZ    1*DEI_ASK_TE_MAX_3MHZ

//LOW BIT
#define DEI_ASK_LOW_BIT_LOW_MIN_3MHZ      1*DEI_ASK_TE_MIN_3MHZ
#define DEI_ASK_LOW_BIT_LOW_MAX_3MHZ      1*DEI_ASK_TE_MAX_3MHZ
#define DEI_ASK_LOW_BIT_HIGH_MIN_3MHZ     2*DEI_ASK_TE_MIN_3MHZ
#define DEI_ASK_LOW_BIT_HIGH_MAX_3MHZ     2*DEI_ASK_TE_MAX_3MHZ

//---------------------------------------------------------------------------//
//BIT TIME VALUES FOR 2MHZ
//---------------------------------------------------------------------------//

#define DEI_ASK_TE_MIN_2MHZ               220*DEI_ASK_TIME_BASE_2MHZ
#define DEI_ASK_TE_MAX_2MHZ               660*DEI_ASK_TIME_BASE_2MHZ

//HEADER BIT
#define DEI_ASK_HEADER_BIT_LOW_MIN_2MHZ   10*DEI_ASK_TE_MIN_2MHZ
#define DEI_ASK_HEADER_BIT_LOW_MAX_2MHZ   10*DEI_ASK_TE_MAX_2MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MIN_2MHZ  1*DEI_ASK_TE_MIN_2MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MAX_2MHZ  1*DEI_ASK_TE_MAX_2MHZ

//HIGH BIT
#define DEI_ASK_HIGH_BIT_LOW_MIN_2MHZ     2*DEI_ASK_TE_MIN_2MHZ
#define DEI_ASK_HIGH_BIT_LOW_MAX_2MHZ     2*DEI_ASK_TE_MAX_2MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MIN_2MHZ    1*DEI_ASK_TE_MIN_2MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MAX_2MHZ    1*DEI_ASK_TE_MAX_2MHZ

//LOW BIT
#define DEI_ASK_LOW_BIT_LOW_MIN_2MHZ      1*DEI_ASK_TE_MIN_2MHZ
#define DEI_ASK_LOW_BIT_LOW_MAX_2MHZ      1*DEI_ASK_TE_MAX_2MHZ
#define DEI_ASK_LOW_BIT_HIGH_MIN_2MHZ     2*DEI_ASK_TE_MIN_2MHZ
#define DEI_ASK_LOW_BIT_HIGH_MAX_2MHZ     2*DEI_ASK_TE_MAX_2MHZ

//---------------------------------------------------------------------------//
//BIT TIME VALUES FOR 1MHZ
//---------------------------------------------------------------------------//

#define DEI_ASK_TE_MIN_1MHZ               220*DEI_ASK_TIME_BASE_1MHZ
#define DEI_ASK_TE_MAX_1MHZ               660*DEI_ASK_TIME_BASE_1MHZ

//HEADER BIT
#define DEI_ASK_HEADER_BIT_LOW_MIN_1MHZ   10*DEI_ASK_TE_MIN_1MHZ
#define DEI_ASK_HEADER_BIT_LOW_MAX_1MHZ   10*DEI_ASK_TE_MAX_1MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MIN_1MHZ  1*DEI_ASK_TE_MIN_1MHZ
#define DEI_ASK_HEADER_BIT_HIGH_MAX_1MHZ  1*DEI_ASK_TE_MAX_1MHZ

//HIGH BIT
#define DEI_ASK_HIGH_BIT_LOW_MIN_1MHZ     2*DEI_ASK_TE_MIN_1MHZ
#define DEI_ASK_HIGH_BIT_LOW_MAX_1MHZ     2*DEI_ASK_TE_MAX_1MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MIN_1MHZ    1*DEI_ASK_TE_MIN_1MHZ
#define DEI_ASK_HIGH_BIT_HIGH_MAX_1MHZ    1*DEI_ASK_TE_MAX_1MHZ

//LOW BIT
#define DEI_ASK_LOW_BIT_LOW_MIN_1MHZ      1*DEI_ASK_TE_MIN_1MHZ
#define DEI_ASK_LOW_BIT_LOW_MAX_1MHZ      1*DEI_ASK_TE_MAX_1MHZ
#define DEI_ASK_LOW_BIT_HIGH_MIN_1MHZ     2*DEI_ASK_TE_MIN_1MHZ
#define DEI_ASK_LOW_BIT_HIGH_MAX_1MHZ     2*DEI_ASK_TE_MAX_1MHZ

//---------------------------------------------------------------------------//
                                               
#define	DEI_ASK_EXT_ENA()	       	   ATOMIC_FIQ(ATOMIC(PINSEL4 |=   0x00400000 ; EXTINT = 0x02))
#define DEI_ASK_EXT_DIS()	       	   ATOMIC_FIQ(ATOMIC(PINSEL4 &= (~0x00400000); EXTINT = 0x02))

//clear external interrupt
#define DEI_ASK_EXT_CLR()              EXTINT = 0x02

//test if external interrupt EINT1 was triggered
#define DEI_ASK_EXT_ACTIVE()          (EXTINT&0x02)

//enable falling edge for external interrupt
#define DEI_ASK_EXT_ENAF()            (EXTPOLAR &= (~0x02))

//enable rising edge for external interrupt
#define DEI_ASK_EXT_ENAR()            (EXTPOLAR |= ( 0x02))

//check if the external interrupt is set to rising-edge
#define DEI_ASK_EXT_IS_RISING_EDGE()  (EXTPOLAR&0x02) 

//--------------------------------------------------------------------------//


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Dei_Ask  dei_ask ;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

void        dei_ask_ext_init        (void);
Boolean     dei_ask_nvfs_init       (void);
void        dei_ask_notify_init     (void);

void        dei_ask_handle_frame_programming_mode (void);
void        dei_ask_handle_frame_normal_mode      (void);
UInt8       dei_ask_check_frame_programming_mode  (void);
UInt8       dei_ask_check_frame_normal_mode       (void);


void        dei_ask_handle_button_event(Dei_Ask_Button button,Dei_Ask_Press_Type press_type,Boolean is_1button);

void        dei_ask_trace_frame     (void);

void        dei_ask_database_init   (void);
void        dei_ask_database_delete (void);
void        dei_ask_database_save   (void);

void        dei_ask_prg_enter       (void);
void        dei_ask_prg_leave       (void);


void        dei_ask_bit_time_set    (void);

void        dei_ask_handler_fiq     (void);
void        dei_ask_handler_irq     (void);
void        dei_ask_handler_irq_user(void);
void        dei_ask_handler_wake    (Wake_Status wake_status);

void        dei_ask_fiq_mode_enter  (void);
void        dei_ask_fiq_mode_leave  (void);

void        dei_ask_atomic_enter    (void);
void        dei_ask_atomic_leave    (void);

void        dei_ask_state_header                   (Boolean is_rising_edge);
void        dei_ask_state_data_enc_portion         (Boolean is_rising_edge);
void        dei_ask_state_data_fix_portion         (Boolean is_rising_edge);
void        dei_ask_state_data_fix_portion_last_bit(Boolean is_rising_edge);

void        dei_ask_task   (void);
Boolean     dei_ask_receive(TskTimeout timeout);

#endif

