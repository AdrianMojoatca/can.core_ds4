/*==========================================================================*/
// $Id: prg_out_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "prg_out_private.h"
#include <string.h>
#include "intl_port.h"
#include "vrtc_f.h"
#include "dei_siren.h"
#include "system_type.h"
#include "trace.h"   /* DIAG temp: ACC/IGN/START output-map trace */


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef void ( *Hw_Output)( Boolean );
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void prg_out_dei_init       ( void );
static void prg_out_siren_clear(void);
static void prg_out_slide_clear(void);

	
#if ASTROSTART_BRAND_ENABLE == 1
    static void prg_out_astrostart_init( void );
#else
    #define prg_out_astrostart_init()
#endif

#if AUTOSTART_BRAND_ENABLE == 1
    static void prg_out_autostart_init ( void );
#else
    #define prg_out_autostart_init()
#endif
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static const Hw_Output hw_outputs[] = 
{    
    (Hw_Output)prg_out_hw_o1 , (Hw_Output)prg_out_hw_o2 , (Hw_Output)prg_out_hw_o3 , 
    (Hw_Output)prg_out_hw_o4 , (Hw_Output)prg_out_hw_o5 , (Hw_Output)prg_out_hw_o6 , 
    (Hw_Output)prg_out_hw_o7 , (Hw_Output)prg_out_hw_o8 , (Hw_Output)prg_out_hw_o9 , 
    (Hw_Output)prg_out_hw_o10, (Hw_Output)prg_out_hw_o11, (Hw_Output)prg_out_hw_o12,     
    (Hw_Output)prg_out_hw_o13, (Hw_Output)prg_out_hw_o14, (Hw_Output)prg_out_hw_o15,
    (Hw_Output)prg_out_hw_o16, (Hw_Output)prg_out_hw_o17, (Hw_Output)prg_out_hw_not_configured           
};

UInt8 prg_out_timeout;
static Vrtc siren_vrtc;
static UInt8 iid_relay1_funcid = 0;
static UInt8 sys_type_iid_enabled = 0;

static Vrtc slide_vrtc;

static UInt8 prg_out_user_cnt[1+PRG_OUT_COUNT];

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#ifdef PLATFORM_C800
void prg_out_init( void )
{
    prg_out_table = reg_table_create(REG_ATOMIC_USE_RES) ; 

	switch(brand_id)
    {
      case BRAND_VP_1BTN            :
      case BRAND_VP_SC              :
      case BRAND_CLIFFORD_1BTN      :
      case BRAND_CLIFFORD_SC        :
      case BRAND_AVITAL             :
      case BRAND_AUTOMATE           : 
      case BRAND_VP_ASK             : prg_out_dei_init() ; break ;

      case BRAND_AUTOSTART_LED      :
      case BRAND_AUTOSTART_LCD1     :
      case BRAND_AUTOSTART_LCD2     : 

      case BRAND_AUTOSTART_LED_HDR  :
      case BRAND_AUTOSTART_LCD1_HDR :    
      case BRAND_AUTOSTART_LCD2_HDR : prg_out_autostart_init() ; break ;

      case BRAND_ASTROSTART_LED1    :  
      case BRAND_ASTROSTART_LED2    :
      case BRAND_ASTROSTART_LCD1    :
      case BRAND_ASTROSTART_LCD2    : prg_out_astrostart_init() ; break ;

      default:                                            break ; 
    
    }    
                         
}

#else

void prg_out_init( void )
{
	prg_out_table = reg_table_create(REG_ATOMIC_USE_RES) ; 

	prg_out_dei_init() ;
	
	siren_vrtc = vrtc_f_create( (Exec_Func)prg_out_siren_clear );
	
	slide_vrtc = vrtc_f_create( (Exec_Func)prg_out_slide_clear );
}

#endif

//--------------------------------------------------------------------------//

void prg_out_dei_init( void )
{
	UInt8 i, id ; 
	
	System_Type dei_system_type ;
	
	memset(prg_out_user_cnt, 0, sizeof(prg_out_user_cnt));
	
	if (nvfs_rd_s(nvfs_sys_type, &dei_system_type, sizeof(dei_system_type)) == NVFS_OK)
	{
		if((dei_system_type & SYS_TYPE_IID) == SYS_TYPE_IID)
		  sys_type_iid_enabled = 1;
	}

	for( i = 0 ; i < DEI_PROG_OUT_COUNT ; i ++ )
	{
		id = feature_get(DEI_FEAT_OUTPUT1 + i);
		TRACE("\r\n[PRGMAP] out%d id=%d", (int)(i+1), (int)id);   /* DIAG temp */

		if( id && id <= FEAT_DEI_OUT_OPT_MAX) //feature is enable
		{
			prg_out_user_adj(id,+1); // increment # of users
			
			reg_func( prg_out_table , (Reg_Id)(id) , (Reg_Func)hw_outputs[i] );
			if(i == 14)   // relay1 to follow the iid_engine indication
				iid_relay1_funcid = id;
		}
		else
		{
			reg_func( prg_out_table , (Reg_Id)(id) , (Reg_Func)hw_outputs[17] );		// set to not configured output
		}
	}
}

//--------------------------------------------------------------------------//

#if ASTROSTART_BRAND_ENABLE == 1

void prg_out_astrostart_init( void )
{
	UInt8 i ; 

	for( i = 0 ; i < 15 ; i ++ )
	{
		if( !!(feature_get(ASTRO_FEAT_OUTPUT1 + i)&0x80) ) //feature is enable
		{
			reg_func( prg_out_table , (Reg_Id)(feature_get(ASTRO_FEAT_OUTPUT1 + i)&0x7F) , (Reg_Func)hw_outputs[i] );
		}
	}
}

#endif

//--------------------------------------------------------------------------//

#if AUTOSTART_BRAND_ENABLE == 1

void prg_out_autostart_init( void )
{
	UInt8 i ; 

	for( i = 0 ; i < 15 ; i ++ )
	{
		if( !!(feature_get(AUTO_FEAT_OUTPUT1 + i)&0x80) ) //feature is enable
		{
			reg_func( prg_out_table , (Reg_Id)(feature_get(AUTO_FEAT_OUTPUT1 + i)&0x7F) , (Reg_Func)hw_outputs[i] );
		}
	}
}

#endif

#ifdef PLATFORM_933
//--------------------------------------------------------------------------//
void prg_out_sensor_trigger( Boolean ctrl )
{
    prg_out_execute( 1 + PRG_OUT_SENSOR_TRIGGER , ctrl ) ; 
}

//--------------------------------------------------------------------------//
void prg_out_pulse_after_rs_shutdown( Boolean ctrl )
{
    prg_out_execute( 1 + PRG_OUT_PULSE_AFTER_RS_SHUTDOWN , ctrl ) ; 
}

void prg_out_fob( Boolean ctrl )
{
    prg_out_execute( 1 + PRG_OUT_FOB , ctrl ) ; 
}

void prg_out_iid_engine_running( Boolean ctrl)
{
	prg_out_execute( 1 + PRG_OUT_IID_ENGINE_RUNNING, ctrl);
}

#endif

//--------------------------------------------------------------------------//

// recognized names of function outputs

char *extname[PRG_OUT_COUNT]=
{
// the first set of names match the names of the BOOLEAN output (wire) functions
  "FACTORY_ARM"          ,
	"GWA"            ,
	"FACTORY_DISARM"       ,
	"LOCK"	,
  "GWL"   ,    
	"UNLOCK"                     ,
	"SECOND_UNLOCK"              ,
	"UNLOCK_USER2"               , 
	"TRUNK"              ,
	"AUX1"               ,
  "AUX2"               ,
  "AUX3"               ,
  "AUX4"               ,	    
//    PRG_OUT_GROUND_WHEN_RUNNING_STATUS , 
//    PRG_OUT_GROUND_WHEN_RUNNING_5SEC   ,
//    PRG_OUT_PULSE_BEFORE_START         ,
	"SMARTKEY"           ,
	"PULSE_AFTER_START"          ,	
//	PRG_OUT_PULSE_AFTER_IGN_OFF        ,
	"ACC"                        ,
	"IGN"                        ,
    "START"                      ,    
	"PARKING_LIGHT"              ,    	
//    PRG_OUT_REAR_DEFOGGER              ,
    "DOMELIGHT"                  ,
    "HORN"                       ,
//    PRG_OUT_GROUND_WHEN_TRIGGERED      ,
    "ANTI_GRIND"                 ,
    "DEFROSTER_LATCH"         ,
    "DEFROSTER_PULSE"           ,
    "LED_FLASH"		,
    "COMFORT_CLOSURE"            ,
    "GWREADY"   ,
    "STATUS"                     ,
    "SENSOR_TRIGGER"             ,
    "PULSE_AFTER_RS_SHUTDOWN"    ,
    "AG_STARTER_KILL"    ,   
		"FOB",
	"IID_ENGINE_RUNNING"         ,	
	"SLOPEN",
	"SLCLOSE",
	"PTIM_RLY",
	"STS_TRUNK",
	"STS_BRAKE",
  "STS_PROX",
////////////// the next group is for multivalue non-wire outputs (ex. Siren, ...)
    "SIREN"
		
};

// modify and return the number of places asking for this function
UInt8 prg_out_user_adj(UInt8 funcnum, int amt)
{
	if(funcnum < 1+PRG_OUT_COUNT)
	{
		prg_out_user_cnt[funcnum] += amt;
		return prg_out_user_cnt[funcnum];
	}
	return 0;
}	

UInt8 prg_out_funcname_lookup(char *name)
{
UInt8 len,i;
	
	len = 1+strlen(name);
	for(i=0;extname[i];i++)
	{
		if(memcmp(extname[i],name,len)==0)
			return 1+i; // function name found
	}
	return 0; // not found
}

static UInt8 prg_out_shadow[1+PRG_OUT_COUNT];
UInt8 prg_out_shadow_read(UInt8 funcnum)
{
	return prg_out_shadow[funcnum];
}

static void prg_out_siren_clear(void)
{
	prg_out_shadow[1+PRG_OUT_SIREN] = 0;
}

void prg_out_execute(UInt8 funcnum, UInt8 value)
{
	if( funcnum < 1+PRG_OUT_COUNT )
	  reg_execute( prg_out_table , (Reg_Id) funcnum , (Reg_Arg) value);
	
//	if(prg_out_shadow[funcnum] != value)    // send state all the time like the wired siren
//	{// signal the changes only
		prg_out_shadow[funcnum] = value; // update shadow for possible readers including d2d2

	  if(funcnum == 1+PRG_OUT_SIREN)
		{
			//prg_out_timeout = (value == DEI_SIREN_TRIGGER || value == DEI_SIREN_CHIRP_THEN_TRIGGER) ? dei_feature_get(DEI_FEAT_SIREN_DURATION): 8;
			
			switch(value)
			{
				case DEI_SIREN_TRIGGER:
				case DEI_SIREN_CHIRP_THEN_TRIGGER:
					prg_out_timeout = dei_feature_get(DEI_FEAT_SIREN_DURATION);
					break;
				case DEI_SIREN_REMOTE_PROGRAMMING_ENTER:
					prg_out_timeout = 2;//2 seconds timeout. Longer timeout causes the pair success to be overriden, if user pairs too fast after putting IVU into pairing mode
					break;
				default:
					prg_out_timeout = 8;
					break;
			}
			
			vrtc_stop( siren_vrtc );
			vrtc_update( siren_vrtc  , prg_out_timeout * VRTC_1SEC ); // start timer
			vrtc_start( siren_vrtc );
		}
		else if(funcnum == 1+PRG_OUT_SLOPEN || funcnum == 1+PRG_OUT_SLCLOSE)
		{
			prg_out_timeout = value * VRTC_1SEC;  // 0 or 1 second
			
		}
#ifdef MULTICAST_FUNC_ENABLE	
		d2d2_func_multicast_send(funcnum); // send to external devices
#else
		d2d2_func_send(funcnum); // send to external devices
#endif
//	}
}

void prg_out_siren(UInt8 msg)
{
	prg_out_execute(1+PRG_OUT_SIREN,msg);
}

void prg_out_iid_relay1(UInt8 ctrl)
{
	if(sys_type_iid_enabled)
	{
   prg_out_execute(iid_relay1_funcid, ctrl);
	}
}


void prg_out_slide(UInt8 funcnum)
{
	prg_out_execute(funcnum,1);
	
	vrtc_stop( slide_vrtc );			
	vrtc_update( slide_vrtc, VRTC_1SEC );  // can't do smaller interval with vrtc  TODO: change to per_exec ?
	vrtc_start( slide_vrtc );
	
}

// slide_vrtc expires
static void prg_out_slide_clear(void)
{
		prg_out_execute(1+PRG_OUT_SLOPEN,0);
		prg_out_execute(1+PRG_OUT_SLCLOSE,0);
}
