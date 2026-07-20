/*==========================================================================*/
// $Id: dei_sc_payload_build.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sensor_proto_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

typedef struct Sensor_Proto_PldE0
{                           
	UInt8 dest_aid			;
	UInt8 chanl			;
	UInt8 command		;	
	UInt8 pld			;
	
}Sensor_Proto_PldE0;

//--------------------------------------------------------------------------//

typedef struct Sensor_Proto_PldE1
{                           
	UInt8 dest_aid			;
	UInt8 chanl			;
	UInt8 command		;	
	UInt8 pld[4]	;
	
}Sensor_Proto_PldE1;

//--------------------------------------------------------------------------//

typedef struct Sensor_Proto_PldE2
{                           
	UInt8 dest_aid			;
	UInt8 chanl			;
	UInt8 command		;	
	
}Sensor_Proto_PldE2;

//--------------------------------------------------------------------------//

typedef struct Sensor_Proto_PldE3
{                           
	UInt8 dest_aid			;
	UInt8 chanl					;
	UInt8 command				;	
	UInt8 config_pld[6]	;
	
}Sensor_Proto_PldE3;

//--------------------------------------------------------------------------//

typedef struct s_Sensor_Proto_PldE4_Shock
{
	UInt8 dest_aid		;
	UInt8 chanl		;
	UInt8 command	;
	UInt8 alert_bit	;
	UInt8 id[5] ;
}Sensor_Proto_PldE4_Shock;

//--------------------------------------------------------------------------//

typedef struct s_Sensor_Proto_PldE4_Tilt
{
	UInt8 dest_aid		;
	UInt8 chanl		;
	UInt8 command	;
	UInt8 alert_bit	;
	UInt8 id[4] ;
}Sensor_Proto_PldE4_Tilt;

//--------------------------------------------------------------------------//
typedef struct s_Sensor_Proto_PldE4_Temp
{
	UInt8 dest_aid	;
	UInt8 chanl		;
	UInt8 command	;
	UInt8 alert_bit	;
	UInt8 id[4] ;
}Sensor_Proto_PldE4_Temp;

//--------------------------------------------------------------------------//
typedef struct s_Sensor_Proto_PldE9
{
	UInt8 dest_aid	;
	UInt8 chanl		;
	UInt8 command	;
	UInt8 alarm_level[4]	;
}Sensor_Proto_PldE9;

//--------------------------------------------------------------------------//
typedef struct s_Sensor_Proto_PldC1
{
	UInt8 dest_aid	;
	UInt8 chanl		;
	UInt8 command	;
}Sensor_Proto_PldC1;

//--------------------------------------------------------------------------//

typedef union  s_Sensor_Proto_Pld
{
	Sensor_Proto_PldE0 				pldE0;
	Sensor_Proto_PldE1 				pldE1;
	Sensor_Proto_PldE2 				pldE2;
	Sensor_Proto_PldE3				pldE3;
	Sensor_Proto_PldE4_Shock 	pldE4_shock;
	Sensor_Proto_PldE4_Tilt 	pldE4_tilt;
	Sensor_Proto_PldE4_Temp 	pldE4_temp;
	Sensor_Proto_PldE9				pldE9;
	Sensor_Proto_PldC1				pldC1;

	Sensor_Proto_Payload payload[10];
}Sensor_Proto_Pld;

//--------------------------------------------------------------------------//

typedef Sensor_Proto_Payload ( *Sensor_Payload_Build ) ( UInt8, UInt8, UInt8*, UInt8 ) ;

//--------------------------------------------------------------------------//
    
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
UInt8  sensor_proto_build_pldE0( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len );
UInt8  sensor_proto_build_pldE1( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len );
UInt8  sensor_proto_build_pldE2( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len );
UInt8  sensor_proto_build_pldE3( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len );
UInt8  sensor_proto_build_pldE4( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len );
UInt8  sensor_proto_build_pldE8( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len );
UInt8  sensor_proto_build_pldE9( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len );
UInt8  sensor_proto_build_pldC1( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

Sensor_Proto_Pld sensor_pld;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Sensor_Proto_Payload* sensor_proto_payload_build( UInt8 pld_num , UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len )
{
    const Sensor_Payload_Build payload_build[] = 
    {
		(Sensor_Payload_Build)sensor_proto_build_pldE0 , 
		(Sensor_Payload_Build)sensor_proto_build_pldE1 , 
		(Sensor_Payload_Build)sensor_proto_build_pldE2 , 
		(Sensor_Payload_Build)sensor_proto_build_pldE3 , 
		(Sensor_Payload_Build)sensor_proto_build_pldE4 ,
		(Sensor_Payload_Build)sensor_proto_build_pldE8 , 
		(Sensor_Payload_Build)sensor_proto_build_pldE9 ,
		(Sensor_Payload_Build)sensor_proto_build_pldC1 


    };

		pld_size = (payload_build[ pld_num ])( aid, chan, pld, pld_len );
    return (sensor_pld.payload) ; 
}

//--------------------------------------------------------------------------//

UInt8 sensor_proto_build_pldE0( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len )
{
	UInt8 i, pld_size = 0;

    sensor_pld.pldE0.dest_aid = aid ; 
    sensor_pld.pldE0.chanl = chan ; 
    sensor_pld.pldE0.command = SENSOR_PROTO_READ_STAT_REQ ; 

	pld_size = 3;
	if(pld)
	{
		for( i=0; i<pld_len; i++)
		{
			sensor_pld.pldE1.pld[i] = *(pld+i);
		}
		pld_size += pld_len;
	}

 	return pld_size ; 

}

//--------------------------------------------------------------------------//

UInt8 sensor_proto_build_pldE1( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len )
{
		UInt8 i,pld_size = 0;

    sensor_pld.pldE1.dest_aid = aid ; 
    sensor_pld.pldE1.chanl = chan ; 
    sensor_pld.pldE1.command = SENSOR_PROTO_WRITE_REP_BIT ; 
		pld_size = 3;
	
		if(pld)
		{
			for( i=0; i<pld_len; i++)
			{
				sensor_pld.pldE1.pld[i] = *(pld+i);
			}
			pld_size += pld_len;
		}


 	return pld_size ; 

}

//--------------------------------------------------------------------------//

UInt8 sensor_proto_build_pldE2( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len )
{
		UInt8 pld_size = 0;

    sensor_pld.pldE2.dest_aid = aid ; 
    sensor_pld.pldE2.chanl = chan ; 
    sensor_pld.pldE2.command = SENSOR_PROTO_READ_CONFIG_REQ ; 
	pld_size = 3;

 	return pld_size ; 

}

//--------------------------------------------------------------------------//

UInt8 sensor_proto_build_pldE3( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len )
{
		UInt8 i,pld_size = 0;

    sensor_pld.pldE3.dest_aid = aid ; 
    sensor_pld.pldE3.chanl = chan ; 
    sensor_pld.pldE3.command = SENSOR_PROTO_WRITE_CONFIG_REQ ; 
		pld_size = 3;
	
		if(pld)
		{
			for( i=0; i<pld_len; i++)
			{
				sensor_pld.pldE3.config_pld[i] = *(pld+i);
			}
			pld_size += pld_len;
		}

 	return pld_size ; 
}

//--------------------------------------------------------------------------//


UInt8 sensor_proto_build_pldE4( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len )
{
	UInt8 i,pld_size = 3;

		sensor_pld.pldE4_shock.dest_aid = aid ; 
		sensor_pld.pldE4_shock.chanl = chan ; 
		sensor_pld.pldE4_shock.command = SENSOR_PROTO_SENSOR_TYPE_REQ ; 
		sensor_pld.pldE4_shock.alert_bit = 0 ; 

	if(pld)
	{
		for( i=0; i<pld_len; i++)
		{
			sensor_pld.pldE4_shock.id[i] = *(pld+i);
		}
		pld_size += pld_len;
	}
  return pld_size ; 

}

//--------------------------------------------------------------------------//


UInt8 sensor_proto_build_pldE8( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len )
{
	UInt8 i,pld_size = 3;

		sensor_pld.pldE4_shock.dest_aid = aid ; 
		sensor_pld.pldE4_shock.chanl = chan ; 
		sensor_pld.pldE4_shock.command = SENSOR_PROTO_READ_ALARM_LEVEL ; 

  return pld_size ; 

}

//--------------------------------------------------------------------------//

UInt8 sensor_proto_build_pldE9( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len )
{
	UInt8 i,pld_size = 4;

		sensor_pld.pldE9.dest_aid = aid ; 
		sensor_pld.pldE9.chanl = chan ; 
		sensor_pld.pldE9.command = SENSOR_PROTO_WRITE_ALARM_LEVEL ; 

	if(pld)
	{
		for( i=0; i<pld_len; i++)
		{
			sensor_pld.pldE9.alarm_level[i] = *(pld+i);
		}
		pld_size += pld_len;
	}
  return pld_size ; 

}

//--------------------------------------------------------------------------//

UInt8 sensor_proto_build_pldC1( UInt8 aid, UInt8 chan, UInt8* pld, UInt8 pld_len )
{
	UInt8 i,pld_size = 3;

		sensor_pld.pldC1.dest_aid = aid ; 
		sensor_pld.pldC1.chanl = chan ; 
		sensor_pld.pldC1.command = SENSOR_PROTO_WRITE_DEFAULT ; 

  return pld_size ; 

}

//--------------------------------------------------------------------------//
