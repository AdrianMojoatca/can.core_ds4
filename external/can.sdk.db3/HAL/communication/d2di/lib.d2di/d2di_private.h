/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

#ifndef __D2DI_PRIVATE_H__
#define __D2DI_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "dbg.h"
#include "timeout.h"
#include "d2di.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/   

#define TASK_D2DI_NAME             "d2di"
#define TASK_D2DI_PRI              8000
#define TASK_D2DI_STK              0x100
#define D2DI_TSK_TIMEOUT           1000 // 1s task timeout to check uart error

#define D2DI_MAX_PACKET_SIZE       31
#define D2DI_MIN_PACKET_SIZE       7
#define D2DI_DEFAULT_CHECKSUM      0x00

#define D2DI_SRC_ADDRESS_POS       1
#define D2DI_DEST_ADDRESS_POS      2
#define D2DI_CODE_POS              3
#define D2DI_DATA_COUNT_POS        4
#define D2DI_FIRST_DATA_BYTE       5

#define D2DI_SOF                   0x0C
#define D2DI_EOF                   0x0D
#define D2DI_NO_DATA               0x00

#define D2DI_ADDRESS_RS            0x03
#define D2DI_ADDRESS_ALARM         0x04
#define D2DI_ADDRESS_TB            0x06
#define D2DI_ADDRESS_DL            0x07
#define D2DI_ADDRESS_BROADCAST     0xFF

#define D2DI_MAX_DATA_CNT          24
// max timeout between byte reception
#define D2DI_INTERFRAME_TIMEOUT_MS 100

// timeout to insert cmd
#define D2DI_CMD_TIMEOUT           1000

#define D2DI_ERR_BUFF_SIZE          10
//--------------------------------------------------------------------------//

typedef enum
{
    D2DI_NO_VER,
    D2DI_VER_1
} D2Di_Nvfs_Version ;

#define D2DI_VERSION               D2DI_VER_1
//--------------------------------------------------------------------------//
typedef union s_D2Di_Brake_Support
{   
    struct
    {
        unsigned brake     : 1 ;
        unsigned handbrake : 1 ;
        unsigned reserved2 : 6 ;
    } bit ;

    UInt8 byte;

} D2Di_Brake_Support ;
//--------------------------------------------------------------------------//

typedef union s_D2Di_Alarm_Trigger_Support
{   
    struct
    {     
      unsigned front_driver : 1 ;
      unsigned front_pass   : 1 ;
      unsigned rear_driver  : 1 ;
      unsigned rear_pass    : 1 ;
      unsigned trunk        : 1 ;
      unsigned trunk_glass  : 1 ;
      unsigned hood         : 1 ;
      unsigned key_in       : 1 ;
    } bit ;
    
    UInt8 byte;

} D2Di_Alarm_Trigger_Support ;

//--------------------------------------------------------------------------//
typedef struct s_D2Di_Status_Support
{
    D2Di_Alarm_Trigger_Support alarm_trigger;
    D2Di_Brake_Support brake;
    
    unsigned tach              : 1 ;
    unsigned oem_alarm_trigger : 1 ;
    
}D2Di_Status_Support ;
//--------------------------------------------------------------------------//
typedef enum e_D2Di_State
{
    D2DI_STATE_SOF = 0,
    D2DI_STATE_SRC,
    D2DI_STATE_DST,
    D2DI_STATE_CODE,
    D2DI_STATE_CNT,
    D2DI_STATE_DATA,
    D2DI_STATE_CHKSUM,
    D2DI_STATE_EOF
    
} D2Di_State;
//--------------------------------------------------------------------------//
typedef struct s_D2Di_Rs_Info
{
    UInt8 protocol_ver;
    UInt8 manufacturer_id;
    UInt8 d2di_lib_version;
    
} D2Di_Rs_Info;
//--------------------------------------------------------------------------//
typedef enum e_D2Di_Remote_Id
{
    D2DI_DRIVER1 = 1,
    D2DI_DRIVER2,
    D2DI_DRIVER3,
    D2DI_DRIVER4
    
} D2Di_Remote_Id;
//--------------------------------------------------------------------------//
typedef struct s_D2Di_Err
{
    UInt16 count;
    D2Di_Error_Code code[D2DI_ERR_BUFF_SIZE] ;
} D2Di_Error ;
//--------------------------------------------------------------------------//
typedef enum e_D2D_Type
{
    D2D_NULL  ,
    D2D_TYPE1 ,
    D2D_TYPE2 ,
    D2D_TYPE_VALET,   //depreciated, leave here for now
    D2D_TYPE_I,
    
} D2D_Type ; //same enum as in d2d_switch...
//--------------------------------------------------------------------------//

typedef struct s_D2Di_Ena_Bit
{    
    UInt8 d2di_mask[256/8] ;
    UInt8 d2di_ena [256/8] ;
    
} D2Di_Ena_Bit ;

//--------------------------------------------------------------------------//

// private vars struct
typedef struct s_D2Di_Vars 
{
    // scheduler stuff
    unsigned ignition    : 1;
    unsigned rs_ignition : 1;   
    Boolean timeout_ended;
    Boolean block_sleep_restart;    
    D2Di_Status_Support status_support;
    
    // nvfs entries
    D2Di_Rs_Info rs_info;
    D2Di_Error err;
    D2Di_Nvfs_Version version;
    D2D_Type type;
    
    // task stuff
    Tsk tsk_d2di;
    
    // error logging
    Boolean d2di_do_err_save ;
    Timeout d2di_interframe_timeout;
     
} D2Di_Vars ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern D2Di_Vars   d2di_vars ;
extern const D2Di_Ena_Bit d2di_ena_default;
extern D2Di_Ena_Bit d2di_ena_table;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void d2di_insert_checksum        (UInt8 *d2di_packet);
UInt8 d2di_calculate_checksum    (UInt8 *d2di_packet);
void d2di_process_frame          (UInt8 *d2di_frame);
void d2di_sleep_timeout_stop     (void * arg);
void d2di_tx                     (UInt8 code);
void d2di_status_init            (void);
Boolean d2di_mask_is_enabled     (UInt8 d2d_code);
void d2di_task                   (void * tsk_arg);
void d2di_request_rs_info        (void);
void d2di_tx_code                (UInt8 code) ;
Boolean d2di_err_save            (void);
Boolean d2di_get_mask            (UInt8 code);
Boolean d2di_get_ena             (UInt8 code);

#endif
