/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_rsr_feature.h 19058 2013-01-30 18:38:43Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_RSR_FEATURE_H__
#define __CONFIG_RSR_FEATURE_H__

// Update when the nvfs entry changes and become backward-incomptatible (must be decimal between 1 and 255)
#define RS_FEATURE_VERSION 5

//--------------------------------------------------------------------------//
// <h> Remote Start Features
//--------------------------------------------------------------------------//

//<e0>Remote Start Runtime
//<o1>Remote Start Runtime (minutes) <1-60:1>
//</e>
#define RS_FEAT_REMOTE_START_RUNTIME_ENABLED	0x01
#define RS_FEAT_REMOTE_START_RUNTIME_DEF_VALUE	0x0F

//<e0>Diesel Start Delay
//<o1>Diesel Start Delay Duration (seconds)
//    <0=>  *Disabled
//    <1=>  1  sec
//    <2=>  2  sec
//    <3=>  3  sec
//    <4=>  4  sec
//    <5=>  5  sec
//    <6=>  6  sec
//    <7=>  7  sec
//    <8=>  8  sec
//    <9=>  9  sec
//    <10=> 10 sec
//    <11=> 11 sec
//    <12=> 12 sec
//    <13=> 13 sec
//    <14=> 14 sec
//    <15=> 15 sec
//    <16=> 16 sec
//    <17=> 17 sec
//    <18=> 18 sec
//    <19=> 19 sec
//    <20=> 20 sec
//    <21=> 21 sec
//    <22=> 22 sec
//    <23=> 23 sec
//    <24=> 24 sec
//    <25=> 25 sec
//    <26=> 26 sec
//    <27=> 27 sec
//    <28=> 28 sec
//    <29=> 29 sec
//    <30=> 30 sec
//    <31=> 31 sec
//    <32=> 32 sec
//    <33=> 33 sec
//    <34=> 34 sec
//    <35=> 35 sec
//    <36=> 36 sec
//    <37=> 37 sec
//    <38=> 38 sec
//    <39=> 39 sec
//    <40=> 40 sec
//    <41=> 41 sec
//    <42=> 42 sec
//    <43=> 43 sec
//    <44=> 44 sec
//    <45=> 45 sec
//    <46=> 46 sec
//    <47=> 47 sec
//    <48=> 48 sec
//    <49=> 49 sec
//    <50=> 50 sec
//    <51=> 51 sec
//    <52=> 52 sec
//    <53=> 53 sec
//    <54=> 54 sec
//    <55=> 55 sec
//    <56=> 56 sec
//    <57=> 57 sec
//    <58=> 58 sec
//    <59=> 59 sec
//    <60=> 60 sec
//    <61=> 61 sec
//    <62=> 62 sec
//    <63=> 63 sec
//    <64=> 64 sec
//    <65=> 65 sec
//    <66=> 66 sec
//    <67=> 67 sec
//    <68=> 68 sec
//    <69=> 69 sec
//    <70=> 70 sec
//    <71=> 71 sec
//    <72=> 72 sec
//    <73=> 73 sec
//    <74=> 74 sec
//    <75=> 75 sec
//    <76=> 76 sec
//    <77=> 77 sec
//    <78=> 78 sec
//    <79=> 79 sec
//    <80=> 80 sec
//    <81=> 81 sec
//    <82=> 82 sec
//    <83=> 83 sec
//    <84=> 84 sec
//    <85=> 85 sec
//    <86=> 86 sec
//    <87=> 87 sec
//    <88=> 88 sec
//    <89=> 89 sec
//    <90=> 90 sec
//</e>
#define RS_FEAT_DIESEL_START_DELAY_ENABLED		0x01
#define RS_FEAT_DIESEL_START_DELAY_DEF_VALUE	0x03

//<e0>Start Attempts
//<o1>Start Attempts
//    <1=>1 attempt
//    <2=>2 attempts
//    <3=>*3 attempts
//</e>
#define RS_FEAT_START_ATTEMPTS_ENABLED			0x01
#define RS_FEAT_START_ATTEMPTS_DEF_VALUE		0x03

//<e0>3X Lock Start Activation Timeout
//<o1>3X Lock Start Activation Timeout (seconds)
//    <0=> disable
//    <2=>2 seconds
//    <3=>3 seconds
//    <4=>*4 seconds
//    <5=>5 seconds
//    <6=>6 seconds
//    <7=>7 seconds
//    <8=>8 seconds
//    <9=>9 seconds
//    <10=>10 seconds
//</e>
#define RS_FEAT_3X_LOCK_START_ENABLED			0x01
#define RS_FEAT_3X_LOCK_START_DEF_VALUE			0x04

//<e0>Engine Checking Mode
//<o1> Engine Checking Mode
//         <0=> *Data Tachometer 
//         <1=> Checking Off Crank 0.6 sec       
//         <2=> Checking Off Crank 0.8 sec       
//         <3=> Checking Off Crank 1.0 sec       
//         <4=> Checking Off Crank 1.2 sec       
//         <5=> Checking Off Crank 1.4 sec
//         <6=> Checking Off Crank 1.6 sec
//         <7=> Checking Off Crank 1.8 sec
//         <8=> Checking Off Crank 2.0 sec
//        <18=> Checking Off Crank 4.0 sec 
//</e>
#define RS_FEAT_ENGINE_CHECK_ENABLED			0x01
#define RS_FEAT_ENGINE_CHECK_DEF_VALUE			0x00

// </h>


#endif

