/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog_private_defines.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __PROG_PRIVATE_DEFINES_H__
#define __PROG_PRIVATE_DEFINES_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

// COMPILE TIME CREATION OF DEFINE FOR TRUNK DEFAULT OPTION
#if FEAT_TRUNK_DEFAULT == 0
#define DEFAULT_TRUNK_OPTION	TRUNK_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 1
#define DEFAULT_TRUNK_OPTION	RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 2
#define DEFAULT_TRUNK_OPTION	LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 3
#define DEFAULT_TRUNK_OPTION	TAIL_GLASS_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 4
#define DEFAULT_TRUNK_OPTION	GAS_CAP_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 5
#define DEFAULT_TRUNK_OPTION	HEAD_LIGHTS_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 6
#define DEFAULT_TRUNK_OPTION	SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 7
#define DEFAULT_TRUNK_OPTION	WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_TRUNK_DEFAULT == 8
#define DEFAULT_TRUNK_OPTION	WINDOWS_VENT_OPTION
#endif

// COMPILE TIME CREATION OF DEFINE FOR AUX1 DEFAULT OPTION
#if FEAT_AUX1_DEFAULT == 0
#define DEFAULT_AUX1_OPTION		TRUNK_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 1
#define DEFAULT_AUX1_OPTION		RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 2
#define DEFAULT_AUX1_OPTION		LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 3
#define DEFAULT_AUX1_OPTION		TAIL_GLASS_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 4
#define DEFAULT_AUX1_OPTION		GAS_CAP_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 5
#define DEFAULT_AUX1_OPTION		HEAD_LIGHTS_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 6
#define DEFAULT_AUX1_OPTION		SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 7
#define DEFAULT_AUX1_OPTION		WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_AUX1_DEFAULT == 8
#define DEFAULT_AUX1_OPTION		WINDOWS_VENT_OPTION
#endif

// COMPILE TIME CREATION OF DEFINE FOR AUX2 DEFAULT OPTION
#if FEAT_AUX2_DEFAULT == 0
#define DEFAULT_AUX2_OPTION		TRUNK_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 1
#define DEFAULT_AUX2_OPTION		RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 2
#define DEFAULT_AUX2_OPTION		LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 3
#define DEFAULT_AUX2_OPTION		TAIL_GLASS_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 4
#define DEFAULT_AUX2_OPTION		GAS_CAP_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 5
#define DEFAULT_AUX2_OPTION		HEAD_LIGHTS_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 6
#define DEFAULT_AUX2_OPTION		SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 7
#define DEFAULT_AUX2_OPTION		WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_AUX2_DEFAULT == 8
#define DEFAULT_AUX2_OPTION		WINDOWS_VENT_OPTION
#endif

// COMPILE TIME CREATION OF DEFINE FOR AUX3 DEFAULT OPTION
#if FEAT_AUX3_DEFAULT == 0
#define DEFAULT_AUX3_OPTION		TRUNK_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 1
#define DEFAULT_AUX3_OPTION		RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 2
#define DEFAULT_AUX3_OPTION		LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 3
#define DEFAULT_AUX3_OPTION		TAIL_GLASS_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 4
#define DEFAULT_AUX3_OPTION		GAS_CAP_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 5
#define DEFAULT_AUX3_OPTION		HEAD_LIGHTS_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 6
#define DEFAULT_AUX3_OPTION		SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 7
#define DEFAULT_AUX3_OPTION		WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_AUX3_DEFAULT == 8
#define DEFAULT_AUX3_OPTION		WINDOWS_VENT_OPTION
#endif

// COMPILE TIME CREATION OF DEFINE FOR AUX4 DEFAULT OPTION
#if FEAT_AUX4_DEFAULT == 0
#define DEFAULT_AUX4_OPTION		TRUNK_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 1
#define DEFAULT_AUX4_OPTION		RIGHT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 2
#define DEFAULT_AUX4_OPTION		LEFT_SLIDING_DOOR_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 3
#define DEFAULT_AUX4_OPTION		TAIL_GLASS_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 4
#define DEFAULT_AUX4_OPTION		GAS_CAP_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 5
#define DEFAULT_AUX4_OPTION		HEAD_LIGHTS_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 6
#define DEFAULT_AUX4_OPTION		SUNROOF_CLOSURE_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 7
#define DEFAULT_AUX4_OPTION		WINDOWS_CLOSURE_OPTION
#endif
#if FEAT_AUX4_DEFAULT == 8
#define DEFAULT_AUX4_OPTION		WINDOWS_VENT_OPTION
#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
					
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


