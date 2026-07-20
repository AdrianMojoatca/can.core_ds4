/*==========================================================================*/
// $Id: dei_sc_payload_build.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
#include "status_system.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld001
{
	unsigned command             :8 ; 	
	unsigned runtime_programmed  :6 ;
	unsigned remote_start        :1 ;
	unsigned timer_mode          :1 ;	
	unsigned runtime_remaining   :6 ;  		
	unsigned smart_mode          :1 ;
	unsigned mts                 :1 ;
	unsigned dummy1              :2	;
	unsigned security						 :5 ;
	unsigned dummy2							 :1 ;
	

}Dei_Sc_Pld001 ; 

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld002
{
	unsigned command             :8 ; 
	unsigned runtime_programmed  :6 ;
	unsigned remote_start        :1 ;
	unsigned timer_mode          :1 ;
	unsigned runtime_remaining   :6 ;  		
	unsigned smart_mode          :1 ;
	unsigned mts                 :1 ;
	unsigned trigger             :2 ;
	unsigned security						 :5 ;
	unsigned dummy               :1 ;
}Dei_Sc_Pld002 ; 

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld003
{
	unsigned command             :8 ; 
	unsigned runtime_programmed  :6 ;
	unsigned remote_start        :1 ;
	unsigned timer_mode          :1 ;	
	unsigned runtime_remaining   :6 ;  	
	unsigned smart_mode          :1 ;
	unsigned mts                 :1 ;
	unsigned security            :5 ;
	unsigned dummy               :3 ;
}Dei_Sc_Pld003 ; 

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld004
{
	unsigned command            :8;
	
	unsigned trunk_status       :1; 
	unsigned sensor1_status     :1;
	unsigned door_status        :1;
	unsigned sensor2_status     :1;
	unsigned ignition_status    :1;
	unsigned hood_status        :1;	
	unsigned sensor3_status     :1;
	unsigned r7_byte2           :1;

	unsigned sensor1_npc_status :1;	
	unsigned sensor2_npc_status :1;
	unsigned sensor3_npc_status :1;	
	unsigned sensor1_icon_type  :3;
	unsigned r6_byte3           :1;
	unsigned r7_byte3           :1;
			
	unsigned sensor2_icon_type  :3;	
	unsigned sensor3_icon_type  :3;
	unsigned r6_byte4           :1;
	unsigned r7_byte4			:1;
	
}Dei_Sc_Pld004;
	
//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld005
{
	unsigned command                   :8;
	
    unsigned trunk_trigger_status      :1;
	unsigned sensor1_trigger_status    :1;
	unsigned door_trigger_status       :1;
	unsigned sensor2_trigger_status    :1;	
	unsigned ignition_trigger_status   :1;
	unsigned hood_trigger_status       :1;	
	unsigned sensor3_trigger_status    :1;
	unsigned custom_trigger_status     :1;
	
	unsigned sensor1_npc_status        :1;	
	unsigned sensor2_npc_status        :1;	
	unsigned sensor3_npc_status        :1;	
	unsigned sensor1_icon_type         :3;
	unsigned r6_byte3                  :1;
	unsigned r7_byte3                  :1;
	
	
	unsigned sensor2_icon_type         :3;		
	unsigned sensor3_icon_type         :3;
	unsigned r6_byte4                  :1;
	unsigned r7_byte4                  :1;	
}Dei_Sc_Pld005;

//--------------------------------------------------------------------------//
	
typedef struct 	s_Dei_Sc_Pld006
{
    unsigned command                        :8;	
    unsigned trunk_bypass_status            :1;
	unsigned sensor1_trigger_bypass_status  :1;
	unsigned door_bypass_status             :1;
	unsigned sensor2_trigger_bypass_status  :1;	
	unsigned ignition_bypass_status         :1;
	unsigned hood_bypass_status             :1;	
	unsigned sensor3_trigger_bypass_status  :1;	
	unsigned r7_byte2                       :1;
	
	unsigned sensor1_warning_bypass_status  :1;	
	unsigned sensor2_warning_bypass_status  :1;	
	unsigned sensor3_warning_bypass_status  :1;
	unsigned sensor1_icon_type              :3;
	unsigned r6_byte3                       :1;
	unsigned r7_byte3                       :1;
	
	unsigned sensor2_icon_type              :3;		
	unsigned sensor3_icon_type              :3;
	unsigned r6_byte4                       :1;
	unsigned r7_byte4                       :1;
	

}Dei_Sc_Pld006;

//--------------------------------------------------------------------------//

typedef struct 	s_Dei_Sc_Pld007
{
	unsigned command             :8;
	unsigned sensor_bypass_level :4;
}Dei_Sc_Pld007;

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld008
{
	unsigned command       :8 ; 
	unsigned sensor1_level :8 ;
}Dei_Sc_Pld008;

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld009
{
	unsigned command   :8; 
	unsigned smart_start_temp_high :4;
	unsigned smart_start_temp_low  :4;
}Dei_Sc_Pld009;

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld010
{
	unsigned command   :8; 
	unsigned timer_mode_starts_count    :4;
	unsigned timer_mode_starts_period   :4;
}Dei_Sc_Pld010;

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld011
{                           
	unsigned command          :8;	
	unsigned auxch2_icon      :4;		
	unsigned auxch3_icon      :4;	
	unsigned auxch4_icon      :4;		
	unsigned auxch5_icon      :4;
    
    unsigned auxch2_status    :1;
	unsigned auxch3_status    :1;
	unsigned auxch4_status    :1;
	unsigned auxch5_status    :1;
	unsigned auxch6_status    :1;
	
}Dei_Sc_Pld011;

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld012
{
	unsigned command    :8;
	unsigned error_code :8;
	unsigned dummy      :16;
}Dei_Sc_Pld012;

//--------------------------------------------------------------------------//

typedef struct s_Dei_Sc_Pld013
{
	unsigned command      :8;
	unsigned vechicle_temp:8;
	unsigned dummmy       :16;
}Dei_Sc_Pld013;

//--------------------------------------------------------------------------//
typedef struct s_Dei_Sc_Pld015
{
	unsigned command      :8;
	unsigned status_mode  :8;
	unsigned led          :8;
	unsigned reserved     :8;
}Dei_Sc_Pld015;

//--------------------------------------------------------------------------//
typedef struct s_Dei_Sc_Pld016
{
	unsigned command      :8;
	unsigned alert_event  :8;
	unsigned reserved     :16;
}Dei_Sc_Pld016;

//--------------------------------------------------------------------------//

typedef union  s_Dei_Sc_Pld
{
	Dei_Sc_Pld001 pld001;
	Dei_Sc_Pld002 pld002;
	Dei_Sc_Pld003 pld003;
	Dei_Sc_Pld004 pld004;
	Dei_Sc_Pld005 pld005;
	Dei_Sc_Pld006 pld006;
	Dei_Sc_Pld007 pld007;
	Dei_Sc_Pld008 pld008;
	Dei_Sc_Pld009 pld009;
	Dei_Sc_Pld010 pld010;
	Dei_Sc_Pld011 pld011;
	Dei_Sc_Pld012 pld012;
	Dei_Sc_Pld013 pld013;
	Dei_Sc_Pld015 pld015;
	Dei_Sc_Pld016 pld016;

	Dei_Sc_Payload payload;
}Dei_Sc_Pld;

//--------------------------------------------------------------------------//

typedef Dei_Sc_Payload ( *Payload_Build ) ( UInt8 ) ;

//--------------------------------------------------------------------------//
    
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Dei_Sc_Payload dei_sc_build_pld001( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld002( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld003( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld004( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld005( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld006( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld007( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld008( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld009( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld010( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld011( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld012( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld013( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld015( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pld016( UInt8 cmd );
static Dei_Sc_Payload dei_sc_build_pldna ( UInt8 cmd );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/



/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_payload_build( DEI_SC_PAYLOAD pld , UInt8 cmd )
{
    const Payload_Build payload_build[] = 
    {
        (Payload_Build)dei_sc_build_pld001 , (Payload_Build)dei_sc_build_pld002 , (Payload_Build)dei_sc_build_pld003 , 
        (Payload_Build)dei_sc_build_pld004 , (Payload_Build)dei_sc_build_pld005 , (Payload_Build)dei_sc_build_pld006 , 
        (Payload_Build)dei_sc_build_pld007 , (Payload_Build)dei_sc_build_pld008 , (Payload_Build)dei_sc_build_pld009 , 
        (Payload_Build)dei_sc_build_pld010 , (Payload_Build)dei_sc_build_pld011 , (Payload_Build)dei_sc_build_pld012 , 
        (Payload_Build)dei_sc_build_pld013 , (Payload_Build)dei_sc_build_pld015 , (Payload_Build)dei_sc_build_pld016 ,
		(Payload_Build)dei_sc_build_pldna
    };

    return (payload_build[ pld ])( cmd ) ; 
}

//--------------------------------------------------------------------------//
volatile UInt8 never_index;
UInt8 dei_sc_security_status( void )
{
	static UInt8 stat_encode[32]={
	0x11, 0x00, 0x03, 0x02,
	0x15, 0x04, 0x0D, 0x0C,
	0x17, 0x06, 0x0F, 0x0E,
	0x19, 0x08, 0x0B, 0x0A,
	0x01, 0x10, 0x13, 0x12,
	0x05, 0x14, 0x1D, 0x1C,
	0x07, 0x16, 0x1F, 0x1E,
	0x09, 0x18, 0x1B, 0x1A
	};
	
	UInt8 table_index, byte1 , byte2;
	
	table_index = 0; // disarm
	if(get_ss_arm())
	{
		if(get_ss_sensor_silent_arm())
			table_index = 3; // stealth 1 is odd
		else if(get_ss_full_silent_arm())
			table_index = 2; // stealth 2 is even
		else
			table_index = 1; // plain vanilla arm
	}
	
	table_index |=  ((get_ss_valet() << 2) | ( get_rs_valet() << 3) | (!get_ss_lock() << 4));
//	byte1 = get_ss_arm();
//	byte2 = (get_ss_sensor_silent_arm()|get_ss_sensor_silent_arm());
	
//	table_index = byte1||
	never_index = table_index;
	return (stat_encode[table_index]);
}


Dei_Sc_Payload dei_sc_build_pld001( UInt8 cmd )
{
    Dei_Sc_Pld pld ;
		UInt8 security;
	pld.payload = 0 ; 

    pld.pld001.command = cmd ; 

    pld.pld001.runtime_programmed = dei_sc_info_get( DEI_SC_INFO_RUNTIME_PROGRAMMED  ) ; 
    pld.pld001.runtime_remaining  = dei_sc_info_get( DEI_SC_INFO_RUNTIME_NOTIFYAINING) ; 
    pld.pld001.remote_start       = dei_sc_info_get( DEI_SC_INFO_REMOTE_START        ) ; 
    pld.pld001.smart_mode         = dei_sc_info_get( DEI_SC_INFO_SMART_MODE          ) ; 
    pld.pld001.timer_mode         = dei_sc_info_get( DEI_SC_INFO_TIMER_MODE          ) ; 
    pld.pld001.mts                = dei_sc_info_get( DEI_SC_INFO_MTS                 ) ;
	
		security = dei_sc_security_status();
    pld.pld001.security           = security /*dei_sc_info_get( DEI_SC_INFO_SECURITY            )*/ ;
//    pld.pld001.status             = 1 ;

    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld002( UInt8 cmd )
{
    Dei_Sc_Pld pld ; 
		UInt8 security;
	pld.payload = 0 ; 

    pld.pld002.command = cmd ; 

    pld.pld002.runtime_programmed = dei_sc_info_get( DEI_SC_INFO_RUNTIME_PROGRAMMED  ) ;
    pld.pld002.runtime_remaining  = dei_sc_info_get( DEI_SC_INFO_RUNTIME_NOTIFYAINING) ;  
    pld.pld002.remote_start       = dei_sc_info_get( DEI_SC_INFO_REMOTE_START        ) ; 
    pld.pld002.smart_mode         = dei_sc_info_get( DEI_SC_INFO_SMART_MODE          ) ; 
    pld.pld002.timer_mode         = dei_sc_info_get( DEI_SC_INFO_TIMER_MODE          ) ; 
    pld.pld002.mts                = dei_sc_info_get( DEI_SC_INFO_MTS                 ) ;
    pld.pld002.trigger            = dei_sc_info_get( DEI_SC_INFO_TRIGGER             ) ;
		security = dei_sc_security_status();
    pld.pld002.security           = security /*dei_sc_info_get( DEI_SC_INFO_SECURITY            )*/ ;
//    pld.pld001.status             = 1 ;

    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld003( UInt8 cmd )
{
    Dei_Sc_Pld pld ; 
		UInt8 security;
	pld.payload = 0 ; 

    pld.pld003.command = cmd ; 

    pld.pld003.runtime_programmed = dei_sc_info_get( DEI_SC_INFO_RUNTIME_PROGRAMMED  ) ; 
    pld.pld003.runtime_remaining  = dei_sc_info_get( DEI_SC_INFO_RUNTIME_NOTIFYAINING) ; 
    pld.pld003.remote_start       = dei_sc_info_get( DEI_SC_INFO_REMOTE_START        ) ; 
    pld.pld003.smart_mode         = dei_sc_info_get( DEI_SC_INFO_SMART_MODE          ) ; 
    pld.pld003.timer_mode         = dei_sc_info_get( DEI_SC_INFO_TIMER_MODE          ) ; 
    pld.pld003.mts                = dei_sc_info_get( DEI_SC_INFO_MTS                 ) ;
		security = dei_sc_security_status();
    pld.pld003.security           = security /*dei_sc_info_get( DEI_SC_INFO_SECURITY            )*/ ;
//    pld.pld001.status             = 1 ;

    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld004( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 

	pld.payload = 0 ; 
    
    pld.pld004.command            = cmd ; 
	pld.pld004.door_status        = dei_sc_info_get( DEI_SC_INFO_DOOR_STATUS    ) ; 
    pld.pld004.hood_status        = dei_sc_info_get( DEI_SC_INFO_HOOD_STATUS    ) ;
	pld.pld004.trunk_status       = dei_sc_info_get( DEI_SC_INFO_TRUNK_STATUS   ) ;
	pld.pld004.ignition_status    = dei_sc_info_get( DEI_SC_INFO_IGNITION_STATUS) ;

    pld.pld004.sensor1_status     = dei_sc_info_get( DEI_SC_INFO_SENSOR1_STATUS ) ;	
	pld.pld004.sensor2_status     = dei_sc_info_get( DEI_SC_INFO_SENSOR2_STATUS ) ;		
	pld.pld004.sensor3_status     = dei_sc_info_get( DEI_SC_INFO_SENSOR3_STATUS ) ;	
	
    pld.pld004.sensor1_npc_status = dei_sc_info_get( DEI_SC_INFO_SENSOR1_NPC    ) ;	
    pld.pld004.sensor2_npc_status = dei_sc_info_get( DEI_SC_INFO_SENSOR2_NPC    ) ;	
	pld.pld004.sensor3_npc_status = dei_sc_info_get( DEI_SC_INFO_SENSOR3_NPC    ) ;	

    pld.pld004.sensor1_icon_type  = dei_sc_info_get( DEI_SC_INFO_SENSOR1_ICON   ) ;		
	pld.pld004.sensor2_icon_type  = dei_sc_info_get( DEI_SC_INFO_SENSOR2_ICON   ) ;		
	pld.pld004.sensor3_icon_type  = dei_sc_info_get( DEI_SC_INFO_SENSOR3_ICON   ) ;	

    return pld.payload ; 
}	

//--------------------------------------------------------------------------//


Dei_Sc_Payload dei_sc_build_pld005( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	
	pld.payload = 0 ;     

    pld.pld005.command            = cmd ; 
	
    pld.pld005.door_trigger_status        = dei_sc_info_get( DEI_SC_INFO_DOOR_TRIGGERED     ) ; 
    pld.pld005.hood_trigger_status        = dei_sc_info_get( DEI_SC_INFO_HOOD_TRIGGERED     ) ;
	pld.pld005.trunk_trigger_status       = dei_sc_info_get( DEI_SC_INFO_TRUNK_TRIGGERED    ) ;
	pld.pld005.ignition_trigger_status    = dei_sc_info_get( DEI_SC_INFO_IGNITION_TRIGGERED ) ;

    pld.pld005.sensor1_trigger_status     = dei_sc_info_get( DEI_SC_INFO_SENSOR1_TRIGGERED  ) ;	
	pld.pld005.sensor2_trigger_status     = dei_sc_info_get( DEI_SC_INFO_SENSOR2_TRIGGERED  ) ;		
	pld.pld005.sensor3_trigger_status     = dei_sc_info_get( DEI_SC_INFO_SENSOR3_TRIGGERED  ) ;	
	
    pld.pld005.sensor1_npc_status         = dei_sc_info_get( DEI_SC_INFO_SENSOR1_NPC      ) ;	
    pld.pld005.sensor2_npc_status         = dei_sc_info_get( DEI_SC_INFO_SENSOR2_NPC      ) ;	
	pld.pld005.sensor3_npc_status         = dei_sc_info_get( DEI_SC_INFO_SENSOR3_NPC      ) ;	

    pld.pld005.sensor1_icon_type          = dei_sc_info_get( DEI_SC_INFO_SENSOR1_ICON     ) ;		
	pld.pld005.sensor2_icon_type          = dei_sc_info_get( DEI_SC_INFO_SENSOR2_ICON     ) ;		
	pld.pld005.sensor3_icon_type          = dei_sc_info_get( DEI_SC_INFO_SENSOR3_ICON     ) ;	
	
	if( get_ss_custom_trigger_zone() )
	{
		set_ss_custom_trigger_zone( FALSE );
		pld.pld005.custom_trigger_status          =  1 ;	
	}

    return pld.payload ; 
}	

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld006( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
    
    pld.payload = 0 ; 

	pld.pld006.command            = cmd ; 
	pld.pld006.door_bypass_status            = dei_sc_info_get( DEI_SC_INFO_DOOR_TRIGGER_BYPASS         ) ; 
    pld.pld006.hood_bypass_status            = dei_sc_info_get( DEI_SC_INFO_HOOD_TRIGGER_BYPASS         ) ;
	pld.pld006.trunk_bypass_status           = dei_sc_info_get( DEI_SC_INFO_TRUNK_TRIGGER_BYPASS        ) ;
	pld.pld006.ignition_bypass_status        = dei_sc_info_get( DEI_SC_INFO_IGNITION_TRIGGER_BYPASS     ) ;

    pld.pld006.sensor1_trigger_bypass_status = dei_sc_info_get( DEI_SC_INFO_SENSOR1_TRIGGER_BYPASS ) ;	
	pld.pld006.sensor2_trigger_bypass_status = dei_sc_info_get( DEI_SC_INFO_SENSOR2_TRIGGER_BYPASS ) ;		
	pld.pld006.sensor3_trigger_bypass_status = dei_sc_info_get( DEI_SC_INFO_SENSOR3_TRIGGER_BYPASS ) ;	
	
    pld.pld006.sensor1_warning_bypass_status = dei_sc_info_get( DEI_SC_INFO_SENSOR1_WARNING_BYPASS ) ;	
    pld.pld006.sensor2_warning_bypass_status = dei_sc_info_get( DEI_SC_INFO_SENSOR2_WARNING_BYPASS ) ;	
	pld.pld006.sensor3_warning_bypass_status = dei_sc_info_get( DEI_SC_INFO_SENSOR3_WARNING_BYPASS ) ;	

    pld.pld006.sensor1_icon_type             = dei_sc_info_get( DEI_SC_INFO_SENSOR1_ICON        ) ;		
	pld.pld006.sensor2_icon_type             = dei_sc_info_get( DEI_SC_INFO_SENSOR2_ICON        ) ;		
	pld.pld006.sensor3_icon_type             = dei_sc_info_get( DEI_SC_INFO_SENSOR3_ICON        ) ;	

    return pld.payload ; 
}	

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld007( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	pld.payload = 0 ; 
    
    pld.pld007.command            = cmd ; 
	pld.pld007.sensor_bypass_level = dei_sc_info_get( DEI_SC_INFO_SENSOR_BYPASS_LEVEL ) ; 
    
    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld008( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	pld.payload = 0 ; 
    
    pld.pld008.command      = cmd ; 
	pld.pld008.sensor1_level = dei_sc_info_get( DEI_SC_INFO_SENSOR1_LEVEL) ; 
    
    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld009( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	pld.payload = 0 ; 
    
    pld.pld009.command               = cmd ; 
	pld.pld009.smart_start_temp_high = dei_sc_info_get( DEI_SC_INFO_SMART_START_HIGH_TEMP) ; 
    pld.pld009.smart_start_temp_low  = dei_sc_info_get( DEI_SC_INFO_SMART_START_LOW_TEMP ) ; 
    
    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld010( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	pld.payload = 0 ; 
    
    pld.pld010.command                    = cmd ; 
	pld.pld010.timer_mode_starts_count    = dei_sc_info_get( DEI_SC_INFO_TIMER_MODE_STARTS_COUNT  ) ; 
    pld.pld010.timer_mode_starts_period   = dei_sc_info_get( DEI_SC_INFO_TIMER_MODE_STARTS_PERIOD ) ; 
    
    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld011( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	pld.payload = 0 ; 
    
    pld.pld011.command           = cmd ; 
	pld.pld011.auxch2_status    = dei_sc_info_get( DEI_SC_INFO_AUXCH2_STATUS ); 
	pld.pld011.auxch3_status    = dei_sc_info_get( DEI_SC_INFO_AUXCH3_STATUS ); 
	pld.pld011.auxch4_status    = dei_sc_info_get( DEI_SC_INFO_AUXCH4_STATUS ); 
	pld.pld011.auxch5_status    = dei_sc_info_get( DEI_SC_INFO_AUXCH5_STATUS ); 
    pld.pld011.auxch6_status    = dei_sc_info_get( DEI_SC_INFO_AUXCH6_STATUS ); 
	
    pld.pld011.auxch2_icon      = dei_sc_info_get( DEI_SC_INFO_AUXCH2_ICON   ); 
	pld.pld011.auxch3_icon      = dei_sc_info_get( DEI_SC_INFO_AUXCH3_ICON   ); 
	pld.pld011.auxch4_icon      = dei_sc_info_get( DEI_SC_INFO_AUXCH4_ICON   ); 
	pld.pld011.auxch5_icon      = dei_sc_info_get( DEI_SC_INFO_AUXCH5_ICON   ); 
	    
    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld012( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	pld.payload = 0 ; 
    
    pld.pld012.command          = cmd ; 
	pld.pld012.error_code       = dei_sc_info_get( DEI_SC_INFO_ERROR ); 
	    
    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld013( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	pld.payload = 0 ; 
    
    pld.pld013.command         = cmd ; 
	pld.pld013.vechicle_temp   = dei_sc_info_get( DEI_SC_INFO_VECHICLE_TEMP ); 
	    
    return pld.payload ; 
}

//--------------------------------------------------------------------------//
Dei_Sc_Payload dei_sc_build_pld015( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	  pld.payload = 0 ; 
    
    pld.pld015.command         = cmd ; 
	  pld.pld015.reserved        = 0; 
	
	switch(dei_sc_ngrf_mode )
	{
		case DEI_SC_NGRF_MODE_PROGRAMMING_ENTER:
		{
	    pld.pld015.status_mode     = 1;
        pld.pld015.led             = dei_led_pattern & 0x00FF; 
		}
			break;
		case DEI_SC_NGRF_MODE_ERASE:
		{
	    pld.pld015.status_mode     = 2;
        pld.pld015.led             = dei_led_pattern & 0x00FF; 
		}
			break;
		case DEI_SC_NGRF_MODE_ZAP:
		{
	    pld.pld015.status_mode     = 3;
        pld.pld015.led             = dei_led_pattern & 0x00FF; 
		}
			break;
		case DEI_SC_NGRF_MODE_TACHO_DELETE:
		{
	    pld.pld015.status_mode     = 4;
        pld.pld015.led             = dei_led_pattern & 0x00FF; 
		}
			break;
		case DEI_SC_NGRF_MODE_NORMAL:
		{
	    pld.pld015.status_mode     = 0;
      pld.pld015.led             = dei_led_pattern & 0x00FF;		//dei_led_pattern; 
			
			pld.pld015.reserved = (dei_led_pattern>>8) & 0x00FF;
		}
			break;
		case DEI_SC_NGRF_MODE_PROGRAMMING_LEAVE:
		{
	    pld.pld015.status_mode     = 0;
      pld.pld015.led             = 0; 		// Reset LED flashig after programming mode end
  	 dei_sc_ngrf_mode = DEI_SC_NGRF_MODE_NORMAL;
		}
			break;
		default :
			break;
	}
    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pld016( UInt8 cmd )
{
  Dei_Sc_Pld pld  ; 
	pld.payload = 0 ; 
    
  pld.pld016.command         = cmd ; 
	pld.pld016.alert_event     = dei_sc_alert_event; 
	    
    return pld.payload ; 
}

//--------------------------------------------------------------------------//

Dei_Sc_Payload dei_sc_build_pldna( UInt8 cmd )
{
    Dei_Sc_Pld pld  ; 
	pld.payload = 0 ; 
    
    pld.pld013.command         = cmd ; 
	pld.pld013.vechicle_temp   = dei_sc_info_get( DEI_SC_INFO_VECHICLE_TEMP ); 
	    
    return pld.payload ; 
}

//--------------------------------------------------------------------------//
