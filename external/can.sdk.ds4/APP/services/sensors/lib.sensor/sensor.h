/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sensor.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __SENSOR_H__
#define __SENSOR_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef struct s_Sensor		*Sensor ; 

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Sensor sensor_create( UInt8 activation_count_trg  , UInt32 timeout_trg  , Boolean npc_trg , \
                      UInt8 activation_count_warn , UInt32 timeout_warn , Boolean npc_warn );

Boolean sensor_warning   		        ( Sensor sensor );
Boolean sensor_trigger   		        ( Sensor sensor );

void    sensor_bypass    		        ( Sensor sensor , UInt32 timeout );
void    sensor_enable    		        ( Sensor sensor );
void    sensor_disable   		        ( Sensor sensor );
void 	sensor_warning_disable	        ( Sensor sensor );
void 	sensor_trigger_disable          ( Sensor sensor );
void    sensor_reset     		        ( Sensor sensor );

Boolean sensor_status_npc		        ( Sensor sensor );
Boolean sensor_status_bypass            ( Sensor sensor );
Boolean sensor_status_warning_allowed   ( Sensor sensor );
Boolean sensor_status_trigger_allowed   ( Sensor sensor );

void set_trig_sensor_index( UInt8 index ) ;
UInt8 get_trig_sensor_index( void ) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

