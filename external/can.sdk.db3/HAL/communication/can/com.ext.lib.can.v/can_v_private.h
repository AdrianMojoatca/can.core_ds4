/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

//==========================================================================//
//                                                                          //
//                            SET = { A , B , C }                           //
//                                                                          //
//--------------------------------------------------------------------------//
//                                                                          //
//          ID_Range             Id_Range             Id_Range              //
//      +---------------+    +---------------+    +---------------+         //
// +--->| lhs | A | rhs |<-->| lhs | B | rhs |<-->| lhs | C | rhs |<--+     //
// |    +---------------+    +---------------+    +---------------+   |     //
// |                                                                  |     //
// |                              ID_Set                              |     //
// |                        +-----------------+                       |     //
// +----------------------->| lhs | SET | rhs |<----------------------+     //
//                          +-----------------+                             //
//                                                                          //
//==========================================================================//

#ifndef __CAN_V_PRIVATE_H__
#define __CAN_V_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can.h"
#include "can_lut_sw.h"
#include "can_v.h"
#include "dbg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define NUM_RANGE_TYPES 2
#define NUM_CLASS_TYPES 2

//--------------------------------------------------------------------------//

typedef enum
{
  ITER_L2R , ITER_R2L

} ID_Set_Iterator_Dir ;

//--------------------------------------------------------------------------//

typedef struct s_Can_V_Rx * Can_V_Rx ;

//--------------------------------------------------------------------------//

typedef struct s_Q_Node
{
  struct s_Q_Node * rhs ;
  struct s_Q_Node * lhs ;

} Q_Node ;

//--------------------------------------------------------------------------//

typedef struct s_ID_Set
{
  Q_Node _base_ ; // !!! MUST BE FIRST FIELD !!! \\

} ID_Set ;

//--------------------------------------------------------------------------//

typedef struct s_ID_Range
{
  Q_Node _base_ ; // !!! MUST BE FIRST FIELD !!! \\

  UInt32 min ;
  UInt32 max ;

} ID_Range ;

//--------------------------------------------------------------------------//

typedef struct s_Lut_Map
{
  UInt16   lut_index ;

  Can_V_Rx can_v_rx  ;

  struct s_Lut_Map * next ;

} Lut_Map ;

//--------------------------------------------------------------------------//

typedef union s_Mem_Generic
{
  ID_Set   id_set   ;
  ID_Range id_range ;

  Lut_Map  lut_map  ;

  union s_Mem_Generic * next  ;

} Mem_Generic ;

//--------------------------------------------------------------------------//

typedef struct s_Rx_Q_Msg
{
  MbxMsg _base_ ;  // !!! Must be first !!!

  Can_Msg can_msg ;

} Rx_Q_Msg ;

//--------------------------------------------------------------------------//

typedef struct s_Can_V_Rx
{
  union
    {
      struct
        {
          struct s_Can_V_Rx   * next       ;

          ID_Set              * id_set     ;

          const    Can_Lut_Sw * can_lut_sw ;

          UInt16                rx_q_size  ;

        } init ;

      struct
        {
          Mbx q_free ;
          Mbx q_rcev ;

        } exec ;

    } obj ;

} * Can_V_Rx ;

//--------------------------------------------------------------------------//

typedef struct s_ID_Set_Iterator
{
  Q_Node * root ;
  Q_Node * next ;

  void (* advance)(struct s_ID_Set_Iterator *) ;

} ID_Set_Iterator ;

//--------------------------------------------------------------------------//

typedef enum
{
  COMBO_MODE_0 = 0 ,
  COMBO_MODE_1 = 1

} Combo_Iterator_Mode ;

//--------------------------------------------------------------------------//

typedef struct s_Combo_Iterator
{
  UInt32 bits         ;
  UInt8  num_bits     ;  // 0 <= max_bits <= 32
  SInt8  bits_shifted ;

  Combo_Iterator_Mode mode ;

} Combo_Iterator ;

//--------------------------------------------------------------------------//

typedef enum
{
  HW_LUT_SKIPPED   ,
  HW_LUT_OVERFLOW  ,
  HW_LUT_COMMITTED

} HW_Commit_Status ;

//--------------------------------------------------------------------------//

typedef ID_Range * (* ID_Class_Extract)(const Can_Dev_Id , const Can_Lut_Sw_Entry * const) ;

//--------------------------------------------------------------------------//

typedef HW_Commit_Status (* ID_Set_HW_Commit)(const Can_Dev_Id can_dev_id , ID_Range * const) ;

//--------------------------------------------------------------------------//

typedef union u_Can_V_Vars
{
  struct
    {
      Can_V_Rx   can_v_rx_list  ;
      UInt16     can_v_rx_count ;

      UInt16     can_v_table_size ;

    } init ;

  struct
    {
      Lut_Map ** lut_map_table ;

    } exec ;

} Can_V_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

//-[ Generic Queue ]--------------------------------------------------------//

void             q_init                            (Q_Node * q_root) ;

void             q_unlink                          (Q_Node * q_node) ;

void             q_link_rhs                        (Q_Node * q_root , Q_Node * q_node) ;
void             q_link_lhs                        (Q_Node * q_root , Q_Node * q_node) ;

//-[ Memory Management ]----------------------------------------------------//

ID_Set         * can_v_mem_request_id_set          (void) ;
ID_Range       * can_v_mem_request_id_range        (void) ;

Lut_Map        * can_v_mem_request_lut_map         (void) ;

void             can_v_mem_release                 (void * mem) ;

//-[ ID Range Management ]--------------------------------------------------//

ID_Range       * can_v_id_range_create             (UInt32 min , UInt32 max) ;
void             can_v_id_range_delete             (ID_Range * id_range)     ;
ID_Range       * can_v_id_range_copy               (ID_Range * id_range)     ;

void             can_v_id_range_link_lhs           (ID_Range * id_root , ID_Range * id_range) ;
void             can_v_id_range_link_rhs           (ID_Range * id_root , ID_Range * id_range) ;

void             can_v_id_range_unlink             (ID_Range * id_range)     ;

//-[ ID Set Management ]----------------------------------------------------//

ID_Set         * can_v_id_set_create               (void)            ;
void             can_v_id_set_delete               (ID_Set * id_set) ;
ID_Set         * can_v_id_set_copy                 (ID_Set * id_set) ;

void             can_v_id_set_insert_lhs           (ID_Set * id_set , ID_Range * id_range) ;
void             can_v_id_set_insert_rhs           (ID_Set * id_set , ID_Range * id_range) ;

void             can_v_id_set_append_lhs           (ID_Set * dest , ID_Set * srce) ;
void             can_v_id_set_append_rhs           (ID_Set * dest , ID_Set * srce) ;

ID_Range       * can_v_id_set_peek_lhs             (ID_Set * id_set) ;
ID_Range       * can_v_id_set_peek_rhs             (ID_Set * id_set) ;

void             can_v_id_set_iterator_init        (ID_Set_Iterator * iter , ID_Range ** id_range , ID_Set * id_set , ID_Set_Iterator_Dir iter_dir) ;
void             can_v_id_set_iterator_next        (ID_Set_Iterator * iter , ID_Range ** id_range                                                 ) ;

ID_Set         * can_v_id_set_subtract             (ID_Set * lhs , ID_Set * rhs) ;
ID_Set         * can_v_id_set_intersect            (ID_Set * lhs , ID_Set * rhs) ;

void             can_v_id_set_sort                 (ID_Set * set) ;

//-[ Lut Management ]-------------------------------------------------------//

ID_Range       * can_v_lut_extract_class_std       (const Can_Dev_Id can_dev_id , const Can_Lut_Sw_Entry * const can_lut_sw_entry) ;
ID_Range       * can_v_lut_extract_class_ext       (const Can_Dev_Id can_dev_id , const Can_Lut_Sw_Entry * const can_lut_sw_entry) ;

ID_Set         * can_v_lut_extract_class           (const Can_Dev_Id can_dev_id , const Can_Lut_Sw       * const can_lut_sw , ID_Class_Extract id_class_extract) ;

//-[ Combination Management ]-----------------------------------------------//

void             can_v_combo_iterator_init         (Combo_Iterator * const iter , SInt8 * const bit , const Combo_Iterator_Mode iter_mode , const UInt8 num_bits , const UInt32 bits) ;
void             can_v_combo_iterator_next         (Combo_Iterator * const iter , SInt8 * const bit) ;

//-[ ID Class Management ]--------------------------------------------------//

void             can_v_id_class_create             (const Can_Dev_Id can_dev_id , const ID_Class_Extract id_class_extract) ;
void             can_v_id_class_delete             (void)                                                                  ;
void             can_v_id_class_combine            (ID_Set * id_set_combined [] , const ID_Class_Extract id_class_extract) ;
void             can_v_id_class_wr_hw_lut          (ID_Set * id_set_combined [] , const ID_Set_HW_Commit id_set_hw_commit [] , Boolean * const lut_wr_status) ;

//-[ ID Range HW Management ]-----------------------------------------------//

HW_Commit_Status can_v_id_range_hw_wr_std_single_c (const Can_Dev_Id can_dev_id , ID_Range * const id_range) ;
HW_Commit_Status can_v_id_range_hw_wr_std_group_c  (const Can_Dev_Id can_dev_id , ID_Range * const id_range) ;

HW_Commit_Status can_v_id_range_hw_wr_ext_single_c (const Can_Dev_Id can_dev_id , ID_Range * const id_range) ;
HW_Commit_Status can_v_id_range_hw_wr_ext_group_c  (const Can_Dev_Id can_dev_id , ID_Range * const id_range) ;

//-[ RX LUT Mappping Management ]-------------------------------------------//

Boolean          can_v_rx_lut_map_sw_to_hw         (void) ;
Lut_Map       ** can_v_rx_lut_map_hw_to_sw         (void) ;

//-[ OS Interface Management ]----------------------------------------------//

void             can_v_rx_init_os_interface        (void) ;

//-[ -----------------------------------------------------------------------//

void             can_v_can_irq_init                (Can_Dev can_dev        ) ;
void             can_v_can_irq_service_rx          (Can_IO_Base can_io_base) ;
                                            
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Can_V_Vars can_v_vars ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

