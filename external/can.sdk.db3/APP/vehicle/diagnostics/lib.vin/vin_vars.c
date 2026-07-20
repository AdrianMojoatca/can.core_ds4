/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_vars.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "vin_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef USE_LT_VIN_TABLE
  #define USE_LT_VIN_TABLE
#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

Vin_Info  vin_info  ;
Vin_Error vin_err   ;

const NVFS_Entry_Name nvfs_vin_error  = "VIN_ERROR" ;

const Vin_Table_Entry vin_table [] =
  {
    /*-------------------------------------------------------------------\
    |                    LISTE FOR TOYOTA / LEXUS                        |
    \-------------------------------------------------------------------*/

#if !defined USE_LT_VIN_TABLE  || defined VIN_TL_TYPE1_ENA 
    {"1:12345J,2:T,8:1K,10:789AB"               , VIN_TL_TYPE1}       , // TUNDRA & CAMRY               : 07-10
    {"1:12345J,2:T,8:E,10:9AB"                  , VIN_TL_TYPE1}       , // COROLLA & Matrix             : 09-10    
    {"1:12345J2:T,8:G,10:789A"                  , VIN_TL_TYPE1}       , // ES350                        : 07-10
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_TL_TYPE2_ENA 
    {"1:12345J,2:T,8:UV,10:AB"                  , VIN_TL_TYPE2}       , // PRIUS & 4RUNNER              : 10
    {"1:12345J,2:T,8:R,10:B"                    , VIN_TL_TYPE2}       , // 4RUNNER                      : 11
    {"1:12345J,2:T,8:A,10:9"                    , VIN_TL_TYPE2}       , // VENZA                        : 09
    {"1:12345J,2:T,8:B,10:AB"                   , VIN_TL_TYPE2}       , // VENZA                        : 10
#endif     
  
#if !defined USE_LT_VIN_TABLE  || defined VIN_TL_TYPE3_ENA 
    {"2:T,8:J,10:89ABCDE"                       , VIN_TL_TYPE3}       , // LAND CRUISER                 : 08-XX
    {"2:T,8:F,10:789ABCDE"                      , VIN_TL_TYPE3}       , // LS460 & LS600h               : 07-XX
    {"2:T,8:W,10:89"                            , VIN_TL_TYPE3}       , // LX570                        : 08-09
    {"2:T,7:A,8:X,10:ABCDE"                     , VIN_TL_TYPE3}       , // LX570                        : 10-XX
#endif     
  
    /*-------------------------------------------------------------------\
    |                    LISTE FOR SCION                                 |
    \-------------------------------------------------------------------*/

#if !defined USE_LT_VIN_TABLE  || defined VIN_TL_TYPE1_NO_BP_ENA 
    {"1:12345J2:T,8:4E"                         , VIN_TL_TYPE1_NO_BP} , // SCION xB & xD                : 08-10
#endif    

    /*-------------------------------------------------------------------\
    |          LISTE FOR BUICK, CHEVROLET, GMC, PONTIAC, CADILAC         |
    \-------------------------------------------------------------------*/

#if !defined USE_LT_VIN_TABLE  || defined VIN_GM_TYPE9_ENA
    {""                                        , VIN_GM_TYPE9}        ,
#endif

     // !!! reste Vibe 09-10 !!! 
#if !defined USE_LT_VIN_TABLE  || defined VIN_GM_TYPE10_ENA 
    {"1:2,2:C,3:N,5:L,6:BCDEFGNP,10:A"          , VIN_GM_TYPE10}      , // CHEVROLET EQUINOX           : 10
    {"1:2,2:C,3:T,5:L,6:BCDEFGHJ,10:A"          , VIN_GM_TYPE10}      , // GMC TERRAIN                 : 10                                                                                                          
    {"1:2,2:G,3:1,4:F,5:ABCEFGJKST,10:A"        , VIN_GM_TYPE10}      , // CHEVROLET CAMARO            : 10
    {"1:2,2:G,3:4,4:G,5:ABCD,10:A"              , VIN_GM_TYPE10}      , // BUICK LACROSSE              : 10
    {"2:G,3:T,6:1TX,10:EFGHIJ"                  , VIN_GM_TYPE10A}     , // SIERRA no Onstar            : 14+
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_GM_TYPE6_ENA                                                                         
    {"1:13,2:G,3:Y,5:CE,6:6,10:789A"            , VIN_GM_TYPE6}       , //(C)CADILAC ESCALADE,(E)SRX  : 7-10
    {"1:13,2:G,3:Y,5:K ,6:467,10:789A"          , VIN_GM_TYPE6}       , // CADILAC ESCALADE           : 7-10
    {"1:1,2:G,3:6,4:D,5:MNPR,10:89A"            , VIN_GM_TYPE6}       , // CADILAC CTS                 : 8-10
    {"1:1,2:G,3:6,4:K,5:D,10:6789A"             , VIN_GM_TYPE6}       , // CADILAC DTS                 : 6-10
    {"1:12,2:G,3:4,4:H,5:DPER,10:6789A"         , VIN_GM_TYPE6}       , // LUCERNE                 : 6-10
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_GM_TYPE5_I_ENA
    {"1:1,2:G,3:C,4:CDH,5:ST,10:456789"                 , VIN_GM_TYPE5_I}      , // COLORADO        : 2004-2009
    {"1:1,2:G,3:DT,4:CDEGHJ,5:56ST,10:456789"           , VIN_GM_TYPE5_I}      , // CANYON          : 2004-2009
    {"1:1,2:G,3:8,4:A,10:34567"                         , VIN_GM_TYPE5_I}      , // ION             : 2003-2007
    {"1:1,2:G,3:G,4:CD,5:ST,10:5678"                    , VIN_GM_TYPE5_I}      , // Isuzu           : 2005-2008
    {"1:5,2:G,3:NT,4:DEM,5:N,10:6789A"                  , VIN_GM_TYPE5_I}      , // H3              : 2006-2010


#endif
    /*-------------------------------------------------------------------\
    |                    LISTE FOR HONDA / ACURA                         |
    \-------------------------------------------------------------------*/

#if !defined USE_LT_VIN_TABLE  || defined VIN_HO_TYPE5_A_ENA   
    {"1:15J,2:HK,3:BGM,4:C,5:PS,10:89ABC"       , VIN_HO_TYPE5_A}     , // ACCORD          : 08-12
    {"1:J,2:H,3:4,4:C,5:UW,10:9ABCDE"           , VIN_HO_TYPE5_A}     , // TSX, WAGON      : 09-XX
    {"1:5,2:J,3:6,4:T,5:F,6:12,10:ABC"          , VIN_HO_TYPE5_A}     , // CROSSTOUR       : 10-12
    {"1:1,2:9,3:U,4:Y,5:B,6:1,8:2,10:A"         , VIN_HO_TYPE5_A}     , // ZDX KEY         : 10-XX
    {"1:1,2:9,3:U,4:U,5:A,6:89,8:2,10:A"        , VIN_HO_TYPE5_A}     , // TL KEY          : 09-XX
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HO_TYPE5_B_ENA
    {"1:5,2:F,3:N,4:R,5:L,6:5,10:BCDEFG"        , VIN_HO_TYPE5_B}     , // ODYSSEY         : 11-XX
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HO_TYPE5_C_ENA
    {"1:5,2:F,3:N,4:Y,5:F,10:9ABCDE"            , VIN_HO_TYPE5_C}     , // PILOT           : 09-XX
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HO_TYPE5_D_ENA
    {"1:12J,2:9H,3:GMX,4:F,5:BGM,10:CDEFGH"     , VIN_HO_TYPE5_D}     , // CIVIC, HYBRID   : 12-XX
    {"1:NS,2:LH,3:AH,4:F,5:BGK,10:CDEFGH"       , VIN_HO_TYPE5_D}     , // CIVIC EUROPE    : 12-XX
    {"4:R,5:M,10:CDE"                           , VIN_HO_TYPE5_D}     , // CRV             : 12-14
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HO_TYPE5_E_ENA
    {"1:1,2:9,3:U,4:Y,5:B,6:1,8:46,10:ABC"      , VIN_HO_TYPE5_E}     , // ZDX PTS         : 10-12  
    {"1:1,2:9,3:U,4:U,5:A,6:89,8:5,10:ABC"      , VIN_HO_TYPE5_E}     , // TL PTS          : 09-12
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HO_TYPE5_F_ENA
    {"1:12,2:H,3:GK,4:C,5:RT,10:DEFGH"          , VIN_HO_TYPE5_F}     , // ACCORD          : 13-XX
    {"1:J,2:H,3:M,4:C,5:RT,10:DEFGH"            , VIN_HO_TYPE5_F}     , // ACCORD          : 13-XX
    {"1:1,2:9,3:X,4:C,5:RT,10:DEFGH"            , VIN_HO_TYPE5_F}     , // ACCORD          : 13-XX

    {"1:5,2:J,3:6,4:T,5:F,10:DEFGH"             , VIN_HO_TYPE5_F}     , // CROSSTOUR       : 13-XX
    {"1:J,2:H,3:L,4:T,5:F,10:DEFGH"             , VIN_HO_TYPE5_F}     , // CROSSTOUR       : 13-XX
    {"1:2,2:H,3:K,4:T,5:F,10:DEFGH"             , VIN_HO_TYPE5_F}     , // CROSSTOUR       : 13-XX
    {"1:3,2:C,3:Z,4:T,5:F,10:DEFGH"             , VIN_HO_TYPE5_F}     , // CROSSTOUR       : 13-XX
    {"1:5,2:F,3:NP,4:T,5:F,10:DEFGH"            , VIN_HO_TYPE5_F}     , // CROSSTOUR       : 13-XX
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HO_TYPE5_G_ENA
    {"1:12J,2:9H,3:K,4:R,5:M,6:4,10:FGHIJ"      , VIN_HO_TYPE5_G}     , // CRX   : 15-XX
#endif

    /*-------------------------------------------------------------------\
    |                    LISTE FOR CHRYSLER/DODGE/JEEP                    |
    \-------------------------------------------------------------------*/

#if !defined USE_LT_VIN_TABLE  || defined VIN_CH_TYPE1_ENA
    {"1:12,2:BCD,3:48,5:JPT,10:123"             , VIN_CH_TYPE1}       , // Caravan, Town N Country, Voyager, 01-03
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_CH_TYPE2_ENA
    {"1:12,2:BC,3:3,5:C,10:XY1"                 , VIN_CH_TYPE2}       , // LHS 99-01
    {"1:12,2:BC,3:3,5:DEFH,10:XY1234"           , VIN_CH_TYPE2}       , // 300M, Concorde, Intrepid, 99-04
    {"1:4,2:BC,3:3,5:G,10:1"                    , VIN_CH_TYPE2}       , // Sebring, Stratus, 01
    {"1:1,2:BC,3:3,5:JL,10:123456"              , VIN_CH_TYPE2}       , // Sebring, Stratus, 02-06
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_CH_TYPE3_A_ENA
    {"1:3,2:C,3:48A,5:FYZ,10:1"                 , VIN_CH_TYPE3_A}     , // PT Cruiser, 01
    {"1:13,2:C,3:48,5:EYZ,10:23"                , VIN_CH_TYPE3_A}     , // PT Cruiser, 02-03
    {"1:13,2:C,3:348,5:24Y,10:45"               , VIN_CH_TYPE3_A}     , // PT Cruiser, 04-05
    {"1:1,2:BCP,3:3,5:5SV,10:Y12345"            , VIN_CH_TYPE3_A}     , // Neon, SX2.0, SRT4, 00-05
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_CH_TYPE3_B_ENA
    {"1:13456,2:BD,3:2367,5:AMNRSU,10:2345"      , VIN_CH_TYPE3_B}     , // RAM, 02-05  //first digit 6 is wrong but keeping in case
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_CH_TYPE3_C_ENA
    {"1:1,2:BD,3:7,5:GL,10:1234"                , VIN_CH_TYPE3_C}     , // Dakota, 01-04
    {"1:1,2:BD,3:48,5:RS,10:123"                , VIN_CH_TYPE3_C}     , // Durango, 01-03
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_CH_TYPE4_ENA
    {"1:1,2:J,3:48,5:KL,10:234"                 , VIN_CH_TYPE4}       , // Liberty, 02-04
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_CH_TYPE_JDL_ENA
    {"1:1,2:J,3:4,5:2W,10:XY"                   , VIN_CH_TYPE_JDL}    , // Grand Cherokee 99-00
    {"1:1,2:J,3:48,5:WX,10:1234"                , VIN_CH_TYPE_JDL}    , // Grand Cherokee 01-04
    {"1:1,2:J,3:4,5:4A,10:123456"               , VIN_CH_TYPE_JDL}    , // Wrangler - TJ  01-06
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_CH_TYPE8_ENA
    {"2:D,5:5GH"                                , VIN_CH_TYPE8A}      , // Journey 09-10
    {"2:AC,3:38,5:Y,10:678"                     , VIN_CH_TYPE8A}      , // PT cruiser 6-8
    {"1:1,2:C,3:4,5:J,10:DEFGH"                 , VIN_CH_TYPE8B}      , // Wrangler 13-XX    


#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_CH_TYPE9_ENA
    {"1:3,   2:CD, 3:4,   4:P, 5:5DGH  ,10:BCDEFG" , VIN_CH_TYPE9A}   , // Journey 11-XX
    {"1:2,   2:B,  3:3,   4:C, 5:LM    ,10:BCDEFG" , VIN_CH_TYPE9A}   , // Charger 11-XX 
    {"1:2,   2:C,  3:3,   4:C, 5:ACDK  ,10:BCDEFG" , VIN_CH_TYPE9A}   , // 300, Charger 11-XX
    {"1:123, 2:C,  3:467,      5:R,    ,10:DEFGHI" , VIN_CH_TYPE9B}   , // Ram  13-XX
    {"1:1,   2:C,  3:3,   4:C, 5:D     ,10:DEFGHI" , VIN_CH_TYPE9C}   , // Dart 13-XX
#endif                                  
                                                                                       
    /*-------------------------------------------------------------------\
    |                    LISTE FOR MAZDA                                 |               
    \-------------------------------------------------------------------*/               

#if !defined USE_LT_VIN_TABLE  || defined VIN_MAZDA_TYPE1_ENA                                                                                         
    {"1:J,2:M,3:1,7:HS,10:AB"                   , VIN_MAZDA_TYPE1}      , // Mazda3                     : 10
    {"1:1,2:Y,3:V,6:8,10:9ABCDEFG"              , VIN_MAZDA_TYPE1}      , // Mazda6                     : 09+
    
#endif 

#if !defined USE_LT_VIN_TABLE  || defined VIN_MAZDA_TYPE2_ENA    
    {"1:J,2:M,3:1,4:B,5:K,10:78"                , VIN_MAZDA_TYPE2}      , // Mazda3                     : 07-08
#endif

    /*-------------------------------------------------------------------\
    |                    LISTE FOR FORD/LINCOLN                                 |
    \-------------------------------------------------------------------*/

//    {"1:1,2:F,3:16ABCDMT,10:89AB"               , VIN_FORD       }      , //    Ford Motor Company  USA                   Ford     MPV - Limousine      
////    {"1:1,2:F,3:6,10:89AB"                      , VIN_FORD       }      , //    Detroit Chassis LLC USA                   Ford     Basic (Stripped) Chassis     
////    {"1:1,2:F,3:A,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  USA                   Ford     Passenger Car    
////    {"1:1,2:F,3:B,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  USA                   Ford     Bus      
////    {"1:1,2:F,3:C,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  USA                   Ford     Basic (Stripped) Chassis     
////    {"1:1,2:F,3:D,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  USA                   Ford     Incomplete Vehicle   
////    {"1:1,2:F,3:M,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  USA                   Ford     MPV*     
////    {"1:1,2:F,3:T,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  USA                   Ford     Truck (Completed Vehicle)    
//    {"1:1,2:L,3:N,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  USA                   Lincoln  Passenger Car    
//    {"1:1,2:M,3:EH,10:89AB"                     , VIN_FORD       }      , //    Ford Motor Company  USA                   Mercury  Passenger Car    
////    {"1:1,2:M,3:H,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  USA                   Mercury  Incomplete Vehicle   
//    {"1:1,2:Z,3:V,10:89AB"                      , VIN_FORD       }      , //    Automotive Alliance International (USA)   Ford     Passenger Car    
//    {"1:2,2:F,3:ADMT,10:89AB"                   , VIN_FORD       }      , //    Ford Motor Company  Canada                Ford     Passenger Car    
////    {"1:2,2:F,3:D,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  Canada                Ford     Incomplete Vehicle   
////    {"1:2,2:F,3:M,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  Canada                Ford     MPV*     
////    {"1:2,2:F,3:T,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  Canada                Ford     Truck (Completed Vehicle)    
//    {"1:2,2:L,3:1JMN,10:89AB"                   , VIN_FORD       }      , //    Ford Motor Company  Canada                Lincoln  Incomplete Vehicle- Limousine    
////    {"1:2,2:L,3:J,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  Canada                Lincoln  Incomplete Vehicle- Hearse   
////    {"1:2,2:L,3:M,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  Canada                Lincoln  MPV*     
////    {"1:2,2:L,3:N,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  Canada                Lincoln  Passenger Car    
//    {"1:2,2:M,3:EHR,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  Canada                Mercury  Passenger Car    
////    {"1:2,2:M,3:H,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  Canada                Mercury  Incomplete Vehicle   
////    {"1:2,2:M,3:R,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company  Canada                Mercury  MPV*     
//    {"1:3,2:F,3:ADNRT,10:89AB"                  , VIN_FORD       }      , //    Ford Motor Company, Mexico                Ford     Passenger Car    
////    {"1:3,2:F,3:D,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company, Mexico                Ford     Incomplete Vehicle   
////    {"1:3,2:F,3:N,10:89AB"                      , VIN_FORD       }      , //    Blue Diamond Truck S. De R.L. De C.V.     Ford     Truck (Completed Vehicle)    
////    {"1:3,2:F,3:R,10:89AB"                      , VIN_FORD       }      , //    Blue Diamond Truck S. De R.L. De C.V.     Ford     Incomplete Vehicle   
////    {"1:3,2:F,3:T,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company, Mexico                Ford     Truck (Completed Vehicle)    
//    {"1:3,2:L,3:N,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company, Mexico                Lincoln  Passenger Car    
//    {"1:3,2:M,3:E,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company, Mexico                Mercury  Passenger Car    
//    {"1:4,2:F,3:24,10:89AB"                     , VIN_FORD       }      , //    Ford Motor Company, USA                   Mazda    MPV*   
////    {"1:4,2:F,3:4,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company, USA                   Mazda    Truck (Completed Vehicle)      
//    {"1:4,2:M,3:2,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company, USA                   Mercury  MPV*     
//    {"1:5,2:L,3:1,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company, USA                   Lincoln  MPV* - Limousine     
////    {"1:5,2:L,3:D,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company, USA                   Lincoln  Incomplete Vehicle- Limousine    
//    {"1:5,2:L,3:MT,10:89AB"                     , VIN_FORD       }      , //    Ford Motor Company, USA                   Lincoln  MPV*     
////    {"1:5,2:L,3:T,10:89AB"                      , VIN_FORD       }      , //    Ford Motor Company, USA                   Lincoln  Truck (Completed Vehicle)    

#if !defined USE_LT_VIN_TABLE  || defined VIN_FORD_ENA
    {"1:123,2:F,3:M,5:U,6:0,7:234,10:89AB"      , VIN_FORD}             , // Escape XLS, XLT, LIMITED                   : 8
    {"1:123,2:F,3:A,5:P,6:2,7:DEFHJK,10:AB"     , VIN_FORD}             , // Taurus FWD(SE,SEL,LTD), AWD(SEL,LTD,SHO)   : 10
    {"1:123,2:L,3:N,5:L,6:9,7:DEF,10:9AB"       , VIN_FORD}             , // MKS, base, deluxe & AWD    : 09-10
    {"1:123,2:L,3:M,5:J,6:5,7:AF,10:AB"         , VIN_FORD}             , // MKT, AWD, FWD              : 10
    {"1:123,2:F,3:A,5:P,6:0,7:GHJCKDL,10:AB"    , VIN_FORD}             , // Fusion  S, SE, SEL FWD, SEL AWD, Sport FWD, Sport AWD, Hybrid : 11
    {"1:123,2:F,3:A,5:P,6:3,7:CDEFGH,10:AB"     , VIN_FORD}             , // Focus  SE, SES 2-Dr. Coupe, S, SE, SES, SEL. 4-Dr. Sedan      :10-11
    {"1:123,2:F,3:M,5:U,6:5,7:3,10:89AB"        , VIN_FORD}             , // explorer   

#endif


#if !defined USE_LT_VIN_TABLE  || defined VIN_FORD_TYPE1_ENA
    {"1:123,2:F,3:A,5:P,6:2,7:DEFHJKLM,10:ABC"          , VIN_FORD_TYPE1}       , // Taurus FWD(SE,SEL,LTD), AWD(SEL,LTD,SHO)   : 10-12
    {"1:123,2:L,3:N,5:L,6:9,7:DEF,10:9ABC"              , VIN_FORD_TYPE1}       , // MKS 4-Dr. Sedan FWD/AWD                    : 10-12
    {"1:123,2:L,3:N,5:J,6:5,7:FALMN,10:ABCDEFGHI"       , VIN_FORD_TYPE1}       , // Mkt                                        : 10-13

#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_FORD_TYPE2_ENA
    {"1:123,2:F,3:M,5:K,6:34,7:AGJK,10:BCDEFGHI"        , VIN_FORD_TYPE2}       , // Edge                                       : 11+
    {"1:123,2:F,3:M,5:K,6:78,7:ABDF,10:BCDEFGHI"        , VIN_FORD_TYPE2}       , // explorer                                   : 11+
    {"1:123,2:F,3:A,5:P,6:2,7:DEFHJKLM,10:DEFGHI"       , VIN_FORD_TYPE1}       , // Taurus FWD(SE,SEL,LTD), AWD(SEL,LTD,SHO)   : 13+
    {"1:123,2:L,3:N,5:L,6:9,7:DEF,10:DEFGHI"            , VIN_FORD_TYPE1}       , // MKS 4-Dr. Sedan FWD/AWD                    : 13+
    {"1:123,2:L,3:N,5:J,6:68,7:J,10:BCDEFGHI"           , VIN_FORD_TYPE1}       , // Mkx                                        : 11+
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_FORD_FIESTA_ENA
    {"1:123,2:F,3:A,5:P,6:4,7:C,10:B"               , VIN_FORD_FIESTA}       , // fiesta   
#endif

                                                                        
    /*-------------------------------------------------------------------\
    |                    LISTE FOR HYUNDAI / KIA                         |
    \-------------------------------------------------------------------*/
 
#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE1_ENA 
   {"1:K,2:M,3:H,4:G"                          , VIN_HY_TYPE1}       , // Genesis Sedan PTS 2 to 5 door 
#endif

/* VIN_HY_TYPE3 must be before VIN_HY_TYPE2 becaus of the post 2013 Genesis coupe involving years */

#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE3_ENA 
   {"1:5,2:N,3:P,4:E"                           , VIN_HY_TYPE3}       , // Sonata PTS
   {"1:K,2:M,3:H,4:E"                           , VIN_HY_TYPE3}       , // Sonata Hybrid PTS
   {"1:K,2:M,3:H,4:H,10:DEFGH"                  , VIN_HY_TYPE3A}      , // Genesis Coupe PTS 2013 - 2016
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE2_ENA 
   {"1:K,2:M,3:H,4:H,10:9ABC"                   , VIN_HY_TYPE2}       , // Genesis Coupe PTS 2009 - 2012 
   {"1:5,2:X,3:Y,4:K,10:BCD"                    , VIN_HY_TYPE2}       , // Sorento PTS 2011-2013
   {"1:K,2:N,3:A,4:G,10:A"                      , VIN_HY_TYPE2}       , // Magentis PTS
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE4_ENA
   {"1:K,2:N,3:D,4:JK,10:9"                     , VIN_HY_TYPE4}       , // Borrego PTS
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE5_ENA
   {"1:K,2:N,3:AD,4:PD"                         , VIN_HY_TYPE5A}       , // Sportage PTS
   {"1:U,2:5,3:Y,4:P"                           , VIN_HY_TYPE5A}       , // Sportage PTS Eastern Europe
   {"1:5K,2:NM,3:PH,4:D,6:46"                   , VIN_HY_TYPE5A}       , // Elantra sedan/coupe PTS 2011+
   {"1:K,2:N,3:A,4:F,6:46,10:GH"                , VIN_HY_TYPE5A}      ,  // Forte Koup PTS 2017+
   {"1:K,2:M,3:H,4:D,6:5"                       , VIN_HY_TYPE5B}       , // Elantra GT PTS 2012+
   {"1:K,2:M,3:H,4:T"                           , VIN_HY_TYPE5B}       , // Veloster PTS 2012+
   {"1:K,2:N,3:A,4:H,10:DEFGHIJ"                , VIN_HY_TYPE5C}       , // KIA Rondo 2013+
   {"1:5,2:X,3:Y,4:K,10:EFGHIJ"                 , VIN_HY_TYPE5C}       , // Sorento PTS 2014+
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE6_ENA
   {"1:K,2:M,3:8,4:N"                           , VIN_HY_TYPE6}       , // Veracruz knob
#endif
     
#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE7_ENA
   {"1:K,2:N,3:AD,4:G,10:B"                     , VIN_HY_TYPE7A}       , // Optima sedan PTS 2011, no take over   
   {"1:K,2:N,3:AD,4:G,10:CDEF"                , VIN_HY_TYPE7}       , // Optima sedan PTS ajouter 
   {"1:5,2:X,3:X,4:G,10:CDEF"                 , VIN_HY_TYPE7}       , // Optima sedan PTS 2012 
#endif         

#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE8_ENA
   {"1:K,2:M,3:H,4:G,5:H"                       , VIN_HY_TYPE8}       , // EQUUS PTS  must be before Genesis sedan
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE9_ENA
   {"1:K,2:N,3:D,4:J,10:BCDEFGH"                , VIN_HY_TYPE9}       , // Soul PTS 2011+  
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_HY_TYPE10_ENA
   {"1:K,2:M,3:H,4:S,10:DEFGH"                  , VIN_HY_TYPE10}      , // Santa fe PTS 2013+  
   {"1:5,2:X,3:Y,4:Z,10:DEFGH"                  , VIN_HY_TYPE10}      , // Santa fe PTS 2013+  
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_KIA_TYPE1_ENA 
   {"1:K,2:N,3:A,4:F,6:5,10:EFGH"               , VIN_KIA_TYPE1}      , // Forte5 2014+  
#endif

#if !defined USE_LT_VIN_TABLE  || defined VIN_KIA_TYPE2_ENA 
   {"1:K,2:N,3:A,4:F,6:46,10:EFGH"              , VIN_KIA_TYPE2}      , // Forte 2014+  
#endif

              
    /*-------------------------------------------------------------------\
    |                    LISTE FOR MIT                               |
    \-------------------------------------------------------------------*/

#if !defined USE_LT_VIN_TABLE  || defined VIN_MIT_TYPE_PTS_ENA
    {"1:4J,2:A,3:34,5:JPR,6:4"                  , VIN_MIT_TYPE_PTS}   , // RVR/OUTLANDER SPORT PTS
#endif
   
    /*-------------------------------------------------------------------\
    |                    LISTE FOR SUZUKI                               |
    \-------------------------------------------------------------------*/

#if !defined USE_LT_VIN_TABLE  || defined VIN_SUZ_TYPE1_ENA
    {"1:J,2:S,3:2,10:AB"                        , VIN_SUZ_TYPE1}      , // Kisashi    : 11
#endif

#if defined USE_LT_VIN_TABLE                                 
    {""                                        , VIN_NO_TYPE}        , // prevent error:  #1514: an empty initializer is invalid for an array with unspecified bound
#endif
    } ;

const UInt8 VIN_TABLE_SIZE  = sizeof_array(vin_table) ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

