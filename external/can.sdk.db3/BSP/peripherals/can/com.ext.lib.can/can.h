/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __CAN_H__
#define __CAN_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#ifdef DEV_LPC_176x
#define CAN_AFRAM  ((volatile UInt32 *) 0x40038000)


#define CAN_BASE_0 ((volatile UInt32 *) 0x40044000) 
#define CAN_BASE_1 ((volatile UInt32 *) 0x40048000) 

#else
#define CAN_AFRAM  ((volatile UInt32 *) 0xE0038000)

#define CAN_BASE_0 ((volatile UInt32 *) 0xE0044000) 
#define CAN_BASE_1 ((volatile UInt32 *) 0xE0048000) 
#endif

//--------------------------------------------------------------------------//
                                                                  
typedef volatile UInt32 * Can_IO_Base ;

//--------------------------------------------------------------------------//

typedef void (* Can_Irq_Service_Func)(Can_IO_Base) ;

//--------------------------------------------------------------------------//
#ifdef DEV_LPC_176x
typedef enum  /// !!! Hardware Dependent Order !!! ///
{
  CAN_DEV_1 ,
  CAN_DEV_0 ,

  CAN_DEV_COUNT
   
} Can_Dev_Id ;

#else
typedef enum  /// !!! Hardware Dependent Order !!! ///
{
  CAN_DEV_0 ,
  CAN_DEV_1 ,

  CAN_DEV_COUNT
   
} Can_Dev_Id ;

#endif
//--------------------------------------------------------------------------//

typedef enum  /// !!! Hardware Dependent Order !!! ///
{
  CAN_TX_0 ,
  CAN_TX_1 ,
  CAN_TX_2 ,

  CAN_TX_ID_COUNT
   
} Can_Tx_Id ;

//--------------------------------------------------------------------------//

typedef enum /// !!! Hardware Dependent Order !!! ///
{
  CAN_TX_OK               ,
  CAN_TX_BUSY             ,
  CAN_TX_BUS_ERROR        ,
  CAN_TX_LOST_ARBITRATION ,
  CAN_TX_TIMEOUT          ,

} Can_Tx_Status ;

//--------------------------------------------------------------------------//

typedef enum
{
	CAN_IRQ_BUS_ERROR  ,
	CAN_IRQ_BUS_OFF    ,
	CAN_IRQ_OVERRUN    ,
	CAN_IRQ_ERRATA     ,
		
	CAN_IRQ_COUNT 

} Can_Irq ;

//--------------------------------------------------------------------------//

typedef enum
{
	CAN_SLEEP_MODE   ,
	CAN_WAKE_MODE    ,

} Can_Sleep_Mode ;

//--------------------------------------------------------------------------//
#ifdef DEV_LPC_176x

typedef enum  /// !!! Hardware Dependent Order !!! ///  
{
  CAN_DEV_0_P0_0_1 = 3   , // CAN-0 RD P0.0  and TD P0.1  
  CAN_DEV_0_P0_21_22  = 2, // CAN-0 RD P0.21 and TD P0.22

  CAN_DEV_1_P0_4_5  = 0 , // CAN-1 RD P0.4  and TD P0.5
  CAN_DEV_1_P2_7_8  = 1   // CAN-1 RD P2.7  and TD P2.8

} Can_Pin_Map ;

#else
typedef enum  /// !!! Hardware Dependent Order !!! ///  
{
  CAN_DEV_0_P0_0_1 = 0   , // CAN-0 RD P0.0  and TD P0.1  
  CAN_DEV_0_P0_21_22  = 1, // CAN-0 RD P0.21 and TD P0.22

  CAN_DEV_1_P0_4_5  = 2 , // CAN-1 RD P0.4  and TD P0.5
  CAN_DEV_1_P2_7_8  = 3   // CAN-1 RD P2.7  and TD P2.8

} Can_Pin_Map ;

#endif
//--------------------------------------------------------------------------//
// These are offsets for the CAN-0 and CAN-1 peripheral regsiters that are  //
// identical.  All other common registers are defined via 'device.h'        //
//--------------------------------------------------------------------------//

typedef enum // Hardware Specific - See LPC2300 User Manual ///
{
  CANMOD  = 0x00 >> 2 ,
  CANCMR  = 0x04 >> 2 ,
  CANGSR  = 0x08 >> 2 ,
  CANICR  = 0x0C >> 2 ,
  CANIER  = 0x10 >> 2 ,
  CANBTR  = 0x14 >> 2 ,
  CANEWL  = 0x18 >> 2 ,
  CANSR   = 0x1C >> 2 ,
  CANRFS  = 0x20 >> 2 ,
  CANRID  = 0x24 >> 2 , 
  CANRDA  = 0x28 >> 2 ,
  CANRDB  = 0x2C >> 2 ,
  CANTFI1 = 0x30 >> 2 ,
  CANTID1 = 0x34 >> 2 ,
  CANTDA1 = 0x38 >> 2 ,
  CANTDB1 = 0x3C >> 2 ,
  CANTFI2 = 0x40 >> 2 ,
  CANTID2 = 0x44 >> 2 ,
  CANTDA2 = 0x48 >> 2 ,
  CANTDB2 = 0x4C >> 2 ,
  CANTFI3 = 0x50 >> 2 ,
  CANTID3 = 0x54 >> 2 ,
  CANTDA3 = 0x58 >> 2 ,
  CANTDB3 = 0x5C >> 2 

} Can_Reg_Offs ;

//--------------------------------------------------------------------------//

typedef enum 
{
  CCLK_DIV1 = 0x15,
  CCLK_DIV2 = 0x2A,
  CCLK_DIV4 = 0x00,
  CCLK_DIV6 = 0x3F
   
} Pclk_Div ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Msg
{
  unsigned id           : 29 ;
                        
  unsigned is_ext       :  1 ;
  unsigned is_rtr       :  1 ;

  unsigned length       :  4 ;

  unsigned lut_index_rx : 10 ;

  union
    {
      UInt8  _8_  [8] ; 
      UInt16 _16_ [4] ; // <--*   *----------------------------------------------------------*
      UInt32 _32_ [2] ; //    *-->| <MSB>,,,<LSB>  Stored as DATA[0] = LSB ,,, DATA[7] = MSB |
      UInt64 _64_ [1] ; // <--*   *----------------------------------------------------------*

    } data ;

} Can_Msg ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Dev * Can_Dev ;

typedef struct s_Can_Tx  * Can_Tx  ;
typedef struct s_Can_Rx  * Can_Rx  ;

typedef void (* Can_Irq_Func)(Can_Dev_Id, Can_Irq) ;

//--------------------------------------------------------------------------//

typedef enum  // !!! Order is Critical !!!
{
  CAN_LUT_STD       ,
  CAN_LUT_STD_GROUP ,
  CAN_LUT_EXT       ,
  CAN_LUT_EXT_GROUP 

} Can_Lut_Entry_Type ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Lut_Range
{
  UInt32 min ;
  UInt32 max ;

} Can_Lut_Group ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Lut_Single
{
  UInt32 val ;

} Can_Lut_Single ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Lut_Definition
{
  union
    {
      Can_Lut_Group  group  ;
      Can_Lut_Single single ;

    } ident ;

  Can_Dev_Id         dev_id ;
  Can_Lut_Entry_Type type   ;

} Can_Lut_Definition ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Bit_Def
{	
  Pclk_Div pclk_div ;
  UInt8 brg ;
  UInt8 tseg_1 ;
  UInt8 tseg_2 ;
  UInt8 sjw ;
  	
} Can_Bit_Def ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Abd_Def
{
  Can_Bit_Def bit_def	  ;
  Can_Dev_Id  can_dev_id  ;
  Can_Pin_Map can_pin_map ;

} Can_Abd_Def ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Abd_Status
{
  Boolean speed_match ;
  UInt8   nb_good_msg ;
 
} Can_Abd_Status ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
                                  
EXTERN_C_ENTER                    

Can_Dev           can_dev_create            (Can_Dev_Id can_dev_id , UInt8 brg , UInt8 tseg_1 , UInt8 tseg_2 , UInt8 sjw , Boolean sample_3_times , Can_Pin_Map pin_map) ;

void              can_dev_ena               (Can_Dev_Id can_dev_id) ;
void              can_dev_dis               (Can_Dev_Id can_dev_id) ;

void              can_dev_set_irq_rx        (Can_Dev can_dev , Can_Irq_Service_Func irq_service_rx) ;

Can_Rx            can_rx_create             (Can_Dev can_dev)                       ;
Can_Tx            can_tx_create             (Can_Dev can_dev , Can_Tx_Id can_tx_id) ;

Boolean           can_rx                    (Can_Rx  can_rx , Can_Msg * can_msg , TskTimeout timeout) ;
Boolean           can_tx                    (Can_Tx  can_tx , Can_Msg * can_msg , TskTimeout timeout) ;

Can_Tx_Status     can_tx_one_shot           (Can_Tx  can_tx , Can_Msg * can_msg, TskTimeout  timeout) ;

void              can_rx_flush              (Can_Rx  can_rx) ;

void              can_lut_reset             (void) ;
                                  
void              can_lut_accept_ena        (void) ;
void              can_lut_accept_dis        (void) ;
                                  
void              can_lut_bypass_ena        (void) ;
void              can_lut_bypass_dis        (void) ;
        
Boolean           can_lut_insert_id_std     (Can_Dev_Id can_dev_id  , UInt16 id_std                        ) ;
Boolean           can_lut_insert_id_std_grp (Can_Dev_Id can_dev_id  , UInt16 id_std_min , UInt16 id_std_max) ;
        
Boolean           can_lut_insert_id_ext     (Can_Dev_Id can_dev_id  , UInt32 id_ext                        ) ;
Boolean           can_lut_insert_id_ext_grp (Can_Dev_Id can_dev_id  , UInt32 id_ext_min , UInt32 id_ext_max) ;

Boolean           can_lut_read              (UInt16 index , Can_Lut_Definition * can_lut_def) ;

void              can_msg_rd                (Can_IO_Base can_io_base , Can_Msg * can_msg)                       ;
void              can_msg_wr                (Can_IO_Base can_io_base , Can_Msg * can_msg , Can_Tx_Id can_tx_id) ;

void              can_set_bitrate           (Can_IO_Base can_io_base , UInt8 brg , UInt8 tseg_1 , UInt8 tseg_2 , UInt8 sjw , Boolean sample_3_times) ;
Can_Irq_Func      can_set_irq_func          (Can_Irq_Func irq_func) ;

void              can_pin_map               (Can_Pin_Map pin_map) ;

Can_Abd_Status    can_abd_lookup            (Can_Abd_Def * const can_abd_def , TskTimeout timeout) ;

void 			  can_ctl_reset 			(Can_Dev can_dev) ;

EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

