/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __VIN_H__
#define __VIN_H__

/*==========================================================================*/
/*                       VIN_DEF Syntax & Grammer                           */
/*==========================================================================*/
/*                                                                          */
/* vin_def       :: ws* vin_def_list ws* EOL                                */
/*                                                                          */
/* vin_def_list  :: vin_part                                                */
/*                  vin_part  ws* , ws* vin_def_list                        */
/*                                                                          */
/* vin_part      :: vin_index ws* : ws* vin_sym_list                        */
/*                                                                          */
/* vin_index     :: [0-9]+                                                  */
/*                                                                          */
/* vin_sym_list  :: vin_sym_range                                           */
/*                  vin_sym_range vin_sym_list                              */
/*                                                                          */
/* vin_sym_range :: vin_sym                                                 */
/*                  vin_sym - vin_sym                                       */
/*                                                                          */
/* vin_sym       :: [0-9A-Z]                                                */
/*                                                                          */
/* ws            :: SPACE or TAB                                            */
/*                                                                          */
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define VIN_SIZE 17

//--------------------------------------------------------------------------//

typedef UInt8 Vin [VIN_SIZE] ;

//--------------------------------------------------------------------------//
typedef enum
{
  VIN_NO_TYPE ,
//--------------------------------------------------------------------------//
/*TOYOTA/LEXUS*/
  VIN_TL_TYPE1       ,
  VIN_TL_TYPE1_NO_BP , // example scion
  VIN_TL_TYPE2       ,
  VIN_TL_TYPE2_NO_BP , //  type...
  VIN_TL_TYPE3       , 
//--------------------------------------------------------------------------//
/*    HONDA   */
  VIN_HO_TYPE4       , // Honda 4
  VIN_HO_TYPE5_A     , // Honda 5 A
  VIN_HO_TYPE5_B     , // Honda 5 B
  VIN_HO_TYPE5_C     , // Honda 5 C
  VIN_HO_TYPE5_D     , // Honda 5 D
  VIN_HO_TYPE5_E     , // Honda 5 PTS                   
  VIN_HO_TYPE5_F     , // Honda 5 ACCORD      
  VIN_HO_TYPE5_G     , // Honda 5 CRV             
//--------------------------------------------------------------------------//                   
/*  CHRYSLER/JEEP   */  
  VIN_CH_TYPE1       ,
  VIN_CH_TYPE2       ,
  VIN_CH_TYPE3_A     ,
  VIN_CH_TYPE3_B     ,
  VIN_CH_TYPE3_C     ,
  VIN_CH_TYPE4       ,
  VIN_CH_TYPE6       ,
  VIN_CH_TYPE7       ,
  VIN_CH_TYPE8       ,
  VIN_CH_TYPE8A      ,
  VIN_CH_TYPE8B      ,
  VIN_CH_TYPE9A      ,
  VIN_CH_TYPE9B      ,
  VIN_CH_TYPE9C      ,
  VIN_CH_TYPE_JDL    ,
//--------------------------------------------------------------------------//
/*  FORD/LINCOLN/MAZDA   */
  VIN_FORD           ,// all other
  VIN_FORD_TYPE1     ,// Taurus  ,Mkt   , Mks PTS
  VIN_FORD_TYPE2     ,// Explorer, Edge , MKX PTS
  VIN_FORD_FIESTA    ,
  VIN_MAZDA_TYPE1    , // Mazda 2009+
  VIN_MAZDA_TYPE2    , // Old Mazda 
//--------------------------------------------------------------------------//                   
/*    GM      */     
  VIN_GM_TYPE1       , // GM1 
  VIN_GM_TYPE2       , // GM2
  VIN_GM_TYPE3       , // GM3
  VIN_GM_TYPE4       , // GM4
  VIN_GM_TYPE5       , // GM5
  VIN_GM_TYPE5_I     ,
  VIN_GM_TYPE6       , // GM6
  VIN_GM_TYPE7       , // GM7
  VIN_GM_TYPE8       , // GM8
  VIN_GM_TYPE9       , // GM9 
  VIN_GM_TYPE10      , // GM10
  VIN_GM_TYPE10A     ,
//--------------------------------------------------------------------------//
/*   HYUNDAI/KIA    */
  VIN_HY_TYPE1       , // Genesis Sedan
  VIN_HY_TYPE2       , // Genesis Coupe, Magentis
  VIN_HY_TYPE3       , // Sonata
  VIN_HY_TYPE3A       , // Genesis Coupe 2013 +
  VIN_HY_TYPE4       , // Borrego
  VIN_HY_TYPE5A       , // Sportage , Elantra 2011+ PTS
  VIN_HY_TYPE5B       , // Veloster 2012+ PTS
  VIN_HY_TYPE5C       , // Kia Rondo 2013+ PTS
  VIN_HY_TYPE6       , // Veracruze knob 11
  VIN_HY_TYPE7A       , // Optima PTS 2011
  VIN_HY_TYPE7       , // Optima PTS 2012+
  VIN_HY_TYPE8       , // EQUUS PTS 2011
  VIN_HY_TYPE9       , // Soul PTS
  VIN_HY_TYPE10       , // Santa Fe PTS
  VIN_KIA_TYPE1       , // 2014 Forte5
  VIN_KIA_TYPE2       , // 2014 Forte

//--------------------------------------------------------------------------//
/*  MITSUBISHI  */
  VIN_MIT_TYPE_PTS   , // MIT PTS
//--------------------------------------------------------------------------//
 /*  SUZUKI  */
  VIN_SUZ_TYPE1      , // Kisashi
//--------------------------------------------------------------------------//
  VIN_TYPE_COUNT

} Vin_Car_Type ;

//--------------------------------------------------------------------------//

typedef enum
{
  VIN_MATCH      =  1 ,  // Match status is a positive code
  VIN_NOMATCH    =  0 ,

  VIN_ERR_INDEX  = -1 ,  // Parse error  is a negative code
  VIN_ERR_RANGE  = -2 ,
  VIN_ERR_SYNTAX = -3 ,

} Vin_Status ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void         vin_init_reset   (void)          ;
void         vin_init         (void)          ;
Boolean      vin_save         (const Vin vin) ;
Boolean      vin_validate     (const Vin vin) ;
Vin_Car_Type vin_lookup       (void)          ;
Boolean      vin_get          (Vin vin)       ;
Boolean      vin_is_done      (void)          ;
                                
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

