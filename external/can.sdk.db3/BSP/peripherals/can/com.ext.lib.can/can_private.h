/*<#=====================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __CAN_PRIVATE_H__
#define __CAN_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can.h"
#include "irq_user.h"
#include "fiq.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define HW_LUT_TABLE_SIZE 512
#define HW_LUT_TABLE_OFFSET_MAX (4*HW_LUT_TABLE_SIZE)

//--------------------------------------------------------------------------//

typedef enum
{
  CAN_IRQ_TX_READY            = (1U << 0) ,
  CAN_IRQ_TX_BUS_ERROR        = (1U << 1) ,
  CAN_IRQ_TX_LOST_ARBITRATION = (1U << 2)

} Can_Irq_Tx_Id ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Rx
{
  Irq irq_rx_ready ;

  struct s_Can_Dev * can_dev ;

} * Can_Rx ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Tx
{
  Evt evt_tx ;

  Can_Tx_Id          can_tx_id ;
  struct s_Can_Dev * can_dev   ;

} * Can_Tx ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Dev
{
  Can_IO_Base 			io_base ;

  Can_Dev_Id  			can_dev_id ;

  Can_Rx      			rx [1]               ;
  Can_Tx      			tx [CAN_TX_ID_COUNT] ;

  Boolean     			irq_arbitration_cache ;

  Can_Irq_Service_Func 	irq_service_rx ;

} * Can_Dev ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Tx_Bit
{
  UInt32 cansr_tbs  ;
  UInt32 cancmr_stb ;
  UInt32 canier_tie ;

} Can_Tx_Bit ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Lut_Counters
{
  UInt16 std     ;
  UInt16 std_grp ;

  UInt16 ext     ;
  UInt16 ext_grp ;

} Can_Lut_Counters ;

//--------------------------------------------------------------------------//

typedef enum
{
  CAN_IRQ_ABD_RX_OK        = (1U << 0) ,
  CAN_IRQ_ABD_RX_BUS_ERROR = (1U << 1) 

} Can_Irq_Abd_Evt ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Abd
{
  Can_IO_Base io_base ;
  Evt         evt ;

} Can_Abd ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order Critical !!!
{
  CAN_RM  ,  // Receive Buffer Status.
  CAN_LOM ,  // Listen Only Mode

} Can_Mod ;
//--------------------------------------------------------------------------//

typedef enum e_Can_Cmr
{
  CAN_TR    ,	   //  Transmission Request.
  CAN_AT    ,	   //  Abort Transmission.
  CAN_RRB   ,	   //  Release Receive Buffer.
  CAN_CDO   ,	   //  Clear Data Overrun.
  CAN_SRR   ,	   //  Self Reception Request.
  CAN_STB1  ,	   //  Select Tx Buffer 1.
  CAN_STB2  ,	   //  Select Tx Buffer 2.
  CAN_STB3  ,	   //  Select Tx Buffer 3.

} Can_Cmr ;

//--------------------------------------------------------------------------//

typedef enum e_Can_Ier
{
  CAN_RIE   ,	   //	Receiver Interrupt Enable.
  CAN_TIE1  ,	   //	Transmit Interrupt Enable for Buffer1.
  CAN_EIE   ,	   //	Error Warning Interrupt Enable.
  CAN_DOIE  ,	   //	Data Overrun Interrupt Enable.
  CAN_WUIE  ,	   //	Wake-Up Interrupt Enable.
  CAN_EPIE  ,	   //	Error Passive Interrupt Enable.
  CAN_ALIE  ,	   //	Arbitration Lost Interrupt Enable.
  CAN_BEIE  ,	   //	Bus Error Interrupt Enable.
  CAN_IDIE	,	   //	ID Ready Interrupt Enable.
  CAN_TIE2	,	   //	Transmit Interrupt Enable for Buffer2.
  CAN_TIE3	,	   //	Transmit Interrupt Enable for Buffer3.

} Can_Ier ;

//--------------------------------------------------------------------------//

typedef enum // !!! Order Critical !!!
{
  CAN_RBS ,  // Receive Buffer Status.
  CAN_DOS ,  // Data Overun Status
  CAN_TBS ,  // Transmitter Buffer Status
  CAN_TCS ,  // Transmitter Complete Status
  CAN_RS  ,  // Receive Status
  CAN_TS  ,  // Transmit Status
  CAN_ES  ,  // Error Status
  CAN_BS  ,  // Bus Status

} Can_Gsr ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void can_lut_wr_std_x           (Can_Dev_Id can_dev_id , UInt16 table_index_32) ;
void can_lut_wr_std             (Can_Dev_Id can_dev_id , UInt16 table_index_32 , UInt16 id_std , Boolean _0_or_1_) ;

void can_lut_wr_ext             (Can_Dev_Id can_dev_id , UInt16 table_index_32 , UInt32 id_ext) ;

void can_lut_wr_16              (UInt16 table_index_16 , UInt16 data) ;

void can_irq_service            (void) ;
void can_irq_service_vector     (void) ;
void can_irq_service_tx         (Can_Tx can_tx , UInt32 bus_status) ;
void can_irq_service_rx         (Can_Dev can_dev , UInt32 dev_icr);
void can_irq_service_bus_error  (Can_Dev can_dev) ;
void can_irq_service_overrun 	(Can_Dev can_dev) ;
void can_irq_service_bus_off 	(Can_Dev can_dev) ;

void can_abd_test               (Can_Abd_Status * can_abd_status , TskTimeout timeout) ;
void can_abd_irq_service        (void) ;
void can_abd_irq_service_vector (void) ;

void can_listen_ena             (Can_IO_Base io_base) ;
void can_listen_dis             (Can_IO_Base io_base) ;
void can_pclk_div               (Pclk_Div pclk_div) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Can_Irq_Func     can_irq_func ;

extern Can_Dev          can_dev_table [CAN_DEV_COUNT] ;

extern Can_Lut_Counters can_lut_counters ;

extern const Can_Tx_Bit can_tx_bit_table [CAN_TX_ID_COUNT] ;

extern Can_Abd          can_abd ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

