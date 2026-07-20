/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_lut_sw.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __CAN_LUT_SW_H__
#define __CAN_LUT_SW_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "can.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum 
{
  // public :

  CAN_LUT_SW_OK              ,
  CAN_LUT_SW_FULL            ,
  CAN_LUT_SW_DUPLICATE       ,
  CAN_LUT_SW_CAN_ID_INVALID  ,
  CAN_LUT_SW_GROUP_ERR       ,
  CAN_LUT_SW_GROUP_OVERLAP   ,
  CAN_LUT_SW_ERR_INTERNAL

} Can_Lut_Sw_Status ;

//--------------------------------------------------------------------------//

typedef void (* Can_Lut_Sw_Index_Func)(void *) ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Lut_Sw_Entry
{
  // public :

  Can_Lut_Definition    def        ;

  Can_Lut_Sw_Index_Func index_func ;

} Can_Lut_Sw_Entry ;

//--------------------------------------------------------------------------//

typedef struct s_Can_Lut
{
  // private :

  UInt16 max_entries ;       // Maximum number of entries possible
  UInt16 num_entries ;       // Actual  number present at any given time

  Can_Lut_Sw_Entry * table ; // Storage space for entries

} Can_Lut_Sw ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

EXTERN_C_ENTER

void                     can_lut_sw_create            (Can_Lut_Sw * const can_lut_sw , Can_Lut_Sw_Entry * const table_mem , UInt16 const table_size) ;

Can_Lut_Sw_Status        can_lut_sw_insert_id_std     (Can_Lut_Sw * const can_lut_sw , Can_Dev_Id const dev_id , UInt16 const id     ,                       const Can_Lut_Sw_Index_Func index_func) ; // Can_Lut_Sw_Status : positive value is table index
Can_Lut_Sw_Status        can_lut_sw_insert_id_std_grp (Can_Lut_Sw * const can_lut_sw , Can_Dev_Id const dev_id , UInt16 const id_min , UInt16 const id_max , const Can_Lut_Sw_Index_Func index_func) ; // Can_Lut_Sw_Status : positive value is table index 

Can_Lut_Sw_Status        can_lut_sw_insert_id_ext     (Can_Lut_Sw * const can_lut_sw , Can_Dev_Id const dev_id , UInt32 const id     ,                       const Can_Lut_Sw_Index_Func index_func) ; // Can_Lut_Sw_Status : positive value is table index
Can_Lut_Sw_Status        can_lut_sw_insert_id_ext_grp (Can_Lut_Sw * const can_lut_sw , Can_Dev_Id const dev_id , UInt32 const id_min , UInt32 const id_max , const Can_Lut_Sw_Index_Func index_func) ; // Can_Lut_Sw_Status : positive value is table index 

Can_Lut_Sw_Status        can_lut_sw_insert_entry      (Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry * const entry)                                                                         ; // Can_Lut_Sw_Status : positive value is table index  
                                                                                                                                                                              
const Can_Lut_Sw_Entry * can_lut_sw_index             (Can_Lut_Sw * const can_lut_sw , const UInt16 index) ;

void                     can_lut_sw_reset             (Can_Lut_Sw * const can_lut_sw) ;

void                     can_lut_sw_iterator_init     (const Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry ** const iterator) ;
void                     can_lut_sw_iterator_next     (const Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry ** const iterator) ; 

EXTERN_C_LEAVE

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

