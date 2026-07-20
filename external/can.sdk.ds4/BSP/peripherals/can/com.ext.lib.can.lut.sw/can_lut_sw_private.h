/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_lut_sw_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __CAN_LUT_SW_PRIVATE_H__
#define __CAN_LUT_SW_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <setjmp.h>

#include "can_lut_sw.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Exception
{
  Can_Lut_Sw_Status  status    ;

  jmp_buf            throw_vec ;

} Exception ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void can_lut_sw_check_if_valid_entry         (Exception  * const exception  , const Can_Lut_Sw_Entry * const entry) ;

void can_lut_sw_check_if_valid_can_id        (Exception  * const exception  , const UInt32 can_id , const UInt32 can_id_max) ;

void can_lut_sw_check_if_valid_can_id_group  (Exception  * const exception  , const Can_Lut_Group  * const can_lut_group  , const UInt32 can_id_max) ;
void can_lut_sw_check_if_valid_can_id_single (Exception  * const exception  , const Can_Lut_Single * const can_lut_single , const UInt32 can_id_max) ;

void can_lut_sw_check_if_table_conflict      (Exception  * const exception  , const Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry * const entry) ;
void can_lut_sw_check_if_table_full          (Exception  * const exception  , const Can_Lut_Sw * const can_lut_sw)                                     ;

void can_lut_sw_check_if_single_equal        (Exception  * const exception  , const Can_Lut_Single * const single_lhs , const Can_Lut_Single * const single_rhs) ;
void can_lut_sw_check_if_single_in_group     (Exception  * const exception  , const Can_Lut_Single * const single_lhs , const Can_Lut_Group  * const group_rhs ) ;

void can_lut_sw_check_if_group_overlap       (Exception  * const exception  , const Can_Lut_Group  * const group_lhs  , const Can_Lut_Group  * const group_rhs ) ;

void can_lut_sw_table_insert                 (Can_Lut_Sw * const can_lut_sw , const Can_Lut_Sw_Entry  * const entry) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

