/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_lut_read.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void read_std     (Can_Lut_Definition * can_lut_def , UInt16 offset) ;
static void read_std_grp (Can_Lut_Definition * can_lut_def , UInt16 offset) ;
static void read_ext     (Can_Lut_Definition * can_lut_def , UInt16 offset) ;
static void read_ext_grp (Can_Lut_Definition * can_lut_def , UInt16 offset) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean can_lut_read (UInt16 index , Can_Lut_Definition * can_lut_def)
{
  if (index < can_lut_counters.std    ) { read_std     (can_lut_def , index) ; return TRUE ; } else index -= can_lut_counters.std     ;

  if (index < can_lut_counters.std_grp) { read_std_grp (can_lut_def , index) ; return TRUE ; } else index -= can_lut_counters.std_grp ;

  if (index < can_lut_counters.ext    ) { read_ext     (can_lut_def , index) ; return TRUE ; } else index -= can_lut_counters.ext     ;

  if (index < can_lut_counters.ext_grp) { read_ext_grp (can_lut_def , index) ; return TRUE ; }

  return FALSE ;
}

//--------------------------------------------------------------------------//

static void read_std (Can_Lut_Definition * const can_lut_def , const UInt16 offset)
{
  UInt32 ident_pair = CAN_AFRAM [(CAN_SFF_SA >> 2) + (offset >> 1)] ;

  UInt16 ident_one  = (offset & 1) ? (UInt16) ident_pair : (UInt16) (ident_pair >> 16) ;

  can_lut_def->type             =  CAN_LUT_STD                   ;
  can_lut_def->dev_id           = (Can_Dev_Id) (ident_one >> 13) ;
  can_lut_def->ident.single.val =  ident_one & 0x07FF            ;
}

//--------------------------------------------------------------------------//

static void read_std_grp (Can_Lut_Definition * const can_lut_def , const UInt16 offset)
{
  UInt32 ident_grp = CAN_AFRAM [(CAN_SFF_GRP_SA >> 2) + offset] ;

  can_lut_def->type             =  CAN_LUT_STD_GROUP             ;
  can_lut_def->dev_id           = (Can_Dev_Id) (ident_grp >> 29) ;
  can_lut_def->ident.group.min  = (ident_grp >> 16) & 0x07FF     ;
  can_lut_def->ident.group.max  =  ident_grp        & 0x07FF     ;
}

//--------------------------------------------------------------------------//

static void read_ext (Can_Lut_Definition * const can_lut_def , const UInt16 offset)
{
  UInt32 ident_one = CAN_AFRAM [(CAN_EFF_SA >> 2) + offset] ;

  can_lut_def->type             =  CAN_LUT_EXT                   ;
  can_lut_def->dev_id           = (Can_Dev_Id) (ident_one >> 29) ;
  can_lut_def->ident.single.val =  ident_one & 0x1FFFFFFF        ;
}

//--------------------------------------------------------------------------//

static void read_ext_grp (Can_Lut_Definition * const can_lut_def , const UInt16 offset)
{
  UInt16 index = (CAN_EFF_GRP_SA >> 2) + (offset << 1U) ;

  UInt32 ident_min = CAN_AFRAM [index + 0] ;
  UInt32 ident_max = CAN_AFRAM [index + 1] ;

  can_lut_def->type             =  CAN_LUT_EXT_GROUP             ;
  can_lut_def->dev_id           = (Can_Dev_Id) (ident_min >> 29) ;
  can_lut_def->ident.group.min  = (ident_min & 0x1FFFFFFF)       ;
  can_lut_def->ident.group.max  = (ident_max & 0x1FFFFFFF)       ;
}

