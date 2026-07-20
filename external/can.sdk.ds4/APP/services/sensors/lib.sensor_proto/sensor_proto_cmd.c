/*==========================================================================*/
// $Id: dei_scpp_cmd.c 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sensor_proto_private.h"
#include "d2d.h"
#include "temperature.h"
#include "aid_dhp.h"
#include "status_system.h"
#include "dei_ss_private.h"
#include "dei_sc.h"
#include "dei_siren.h"
#include "dei_prk.h"
#include "sense_rs.h"
#include "dei_rs.h"
#include "sense_rs_private.h"

#include "vrtc.h"
#include "vrtc_f.h"
#include "prg_out.h"


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define INVALID 0xFF

enum
{
	DS_SEVERITY_LEVEL_0,
	DS_SEVERITY_LEVEL_1,
	DS_SEVERITY_LEVEL_2,
	DS_SEVERITY_LEVEL_3,
	DS_SEVERITY_LEVEL_4,
	DS_SEVERITY_LEVEL_5,
	DS_SEVERITY_LEVEL_6,
	DS_SEVERITY_RESERVED
};

#define WIRELESS_PERIPH_PAIR_SUCCESS  (0xBE)
#define MAX_CNT_UNLOCK_4_CONFIRMATION (3U)

#define DS_SEVERITY_POS     					(5U)   // 5 upper bits of report byte
#define DS_PERIPHERAL_LOST            (2<<DS_SEVERITY_POS | 29)
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void sense_diagnostic_services(UInt8 code);
static void wireless_periph_pair_confirm_timeout(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

UInt8 ble_siren_temp_avail = 0;

UInt8 requested_bits = 0;
UInt8 factory_bits = 0;

Vrtc vrtc_wireless_periph_pair_confirm;
static UInt8 cnt_unlock_4_confirmation;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
//--------------------------------------------------------------------------//





struct cmdsense_info
{
	const unsigned cmd_on;
	const unsigned cmd_off;
	const UInt8 implied;  // safety or security implications?
	const UInt8 trigger_zone;
	const char *name;
};


const NVFS_Entry_Name nvfs_safety_file  = "SAFETY" ; // and SECURITY too
// bit mask of signal implications
#define IMPLIED_NONE 0
#define IMPLIED_SAFETY (1<<0)
#define IMPLIED_SECURITY (1<<1)
#define MAX_IMPLIED 2  // two types so far


const struct cmdsense_info cmdsense[] =
{
	
	{ 0,0,IMPLIED_SECURITY,DEI_SS_TRIGGER_ZONE_SENSOR , "SHOCK"},  // must be index 1 in this table
#define SHOCK_INDEX 0
	{ 0,0,IMPLIED_SECURITY,DEI_SS_TRIGGER_ZONE_SENSOR2,  "TILT"},   // must be index 2 in this table
#define TILT_INDEX 1
	{ 0,0,IMPLIED_NONE,DEI_SS_NO_TRIGGER          ,  "TEMP"},   // must be index 3 in this table
#define TEMPERATURE_INDEX 2	
	{ 0,0,IMPLIED_SAFETY,DEI_SS_NO_TRIGGER          , "TACH"},
#define TACH_INDEX 3
	
	{ INPUT_START | INPUT_ON, INPUT_START,IMPLIED_NONE,DEI_SS_TRIGGER_ZONE_IGN    , "ACTIVATION" },
#define START_TRIGGER_INDEX 4	
	{ INPUT_BRAKE_STATUS_PRESS, INPUT_BRAKE_STATUS_RELEASE,IMPLIED_SAFETY,DEI_SS_NO_TRIGGER          , "BRAKE" },
#define BRAKE_INDEX 5	
	{ INPUT_ALL_DOOR_STATUS_OPEN, INPUT_ALL_DOOR_STATUS_CLOSE,IMPLIED_SECURITY,DEI_SS_TRIGGER_ZONE_DOOR   , "DOOR" },
#define DOOR_INDEX 6	
	{ INPUT_INST_TRIG_STATUS_ON, INPUT_INST_TRIG_STATUS_OFF,IMPLIED_NONE,DEI_SS_TRIGGER_ZONE_HOOD   , "INSTANT_TRIGGER" },	
	{ INPUT_HANDBRAKE_STATUS_ON, INPUT_HANDBRAKE_STATUS_OFF,IMPLIED_SAFETY,DEI_SS_NO_TRIGGER          , "EBRAKE" },
#define HANDBRAKE_INDEX 8	
	{ INPUT_HEADLIGHT_STATUS_ON, INPUT_HEADLIGHT_STATUS_OFF,IMPLIED_NONE,DEI_SS_NO_TRIGGER          , "HEADLIGHT_DETECT" },
#define HEADLIGHT_INDEX 9
	{ INPUT_TRUNK_STATUS_OPEN, INPUT_TRUNK_STATUS_CLOSE,IMPLIED_SECURITY,DEI_SS_TRIGGER_ZONE_TRUNK  ,  "TRUNK" },
#define TRUNK_INDEX 10	
	{ INPUT_PUSH_VALET_PRESS, INPUT_PUSH_VALET_RELEASE,IMPLIED_NONE,DEI_SS_NO_TRIGGER          , "VALET" },
	{ INPUT_IGNITION | INPUT_ON, INPUT_IGNITION,IMPLIED_SECURITY,DEI_SS_TRIGGER_ZONE_IGN    , "IGNITION" },
#define IGN_INDEX 12	
	{ INPUT_CUSTOM_POS1 | INPUT_ON, INPUT_CUSTOM_POS1,IMPLIED_NONE,DEI_SS_NO_TRIGGER          , "CUSTOM_POS1" },
#define CUSTOM_POS1_INDEX 13	
	{ INPUT_SENSOR_TRIGGER, 0,IMPLIED_SECURITY,DEI_SS_TRIGGER_ZONE_SENSOR3, "EXT_SENS_TRIG" },
	
// missing from document:
	
  { INPUT_HOOD_STATUS_OPEN, INPUT_HOOD_STATUS_CLOSE,IMPLIED_SAFETY,DEI_SS_TRIGGER_ZONE_HOOD   , "HOOD" },
#define HOOD_INDEX 15

  { INPUT_TAMPER | INPUT_ON, INPUT_TAMPER, IMPLIED_SECURITY, DEI_SS_TRIGGER_ZONE_IGN  , "TAMPER" },
#define TAMPER_INDEX 16
	
	{ 0,0,IMPLIED_SECURITY,DEI_SS_TRIGGER_ZONE_SENSOR3 , "FIELD" },
#define FIELD_INDEX 17

	{ 0,0,IMPLIED_SECURITY,DEI_SS_TRIGGER_ZONE_SENSOR3 , "GLASS" },
#define GLASS_INDEX 18
	
	{ 0,0,IMPLIED_SECURITY,DEI_SS_TRIGGER_ZONE_SENSOR3, "ULTRA" },
#define ULTRA_INDEX 19

	{ 0, 0, IMPLIED_NONE,  DEI_SS_NO_TRIGGER, "PTIM_STOP" },
#define PTIM_STOP_INDEX 20	
	{ 0, 0, IMPLIED_NONE,  DEI_SS_NO_TRIGGER, "UOPEN" },
#define EXT_OPEN_INDEX 21	
	{ 0, 0, IMPLIED_NONE,  DEI_SS_NO_TRIGGER, "UCLOSE" },
#define EXT_CLOSE_INDEX 22	
	{ 0, 0, IMPLIED_NONE,  DEI_SS_NO_TRIGGER, "UTRUNK" },
#define EXT_TRUNK_INDEX 23	
	
	{ 0, 0,IMPLIED_NONE,DEI_SS_NO_TRIGGER          , NULL },
};



UInt16 implied[MAX_IMPLIED][UNIT_MAX]; // boolean per channel, per unit, per type of implication we care about

unsigned implied_filesize(void)
{
UInt16 len;
	if(nvfs_rd_size(nvfs_safety_file,&len) == NVFS_OK)
		return len;
	return 0;
}

// read from nvfs
void implied_init(void)
{
unsigned rlen,mylen,i;

	memset(implied,0,sizeof(implied));
	
	rlen = ~3 & implied_filesize(); // how long is the file?
  //(rlen is always even)
	
	if(rlen < MAX_IMPLIED * sizeof(UInt16))
		return; // this is not a file
    rlen /= MAX_IMPLIED;  // this is the size per implied record

	// how much can I read from each record?
    mylen = (rlen < UNIT_MAX*sizeof(UInt16) ? rlen : UNIT_MAX*sizeof(UInt16));

	for(i=0;i<MAX_IMPLIED;i++)
	{		
	  nvfs_rd_at(nvfs_safety_file,i*rlen,implied[i],mylen); // read what I can from each record
	}
}	

// save to nvfs if changed
void implied_save(void)
{
	{
		if(implied_filesize() == sizeof(implied))
		{
			nvfs_wr(nvfs_safety_file,implied); // just overwrite
		}
		else
		{
			nvfs_delete(nvfs_safety_file);
			nvfs_wr_create(nvfs_safety_file,implied,sizeof(implied));
		}
	}
}

// remember the implications for this unit's sensor channel
void set_implied_for_unit_channel(UInt8 unit,UInt8 ch,UInt8 imp)
{
UInt16 val;
UInt16 mask;
unsigned i;

	if(ch == 0 || ch > 15 || unit >= UNIT_MAX)
		return; // bad channel number or unit number

	mask = 1<<ch;
	
	for(i=0;i<MAX_IMPLIED;i++)
	{
		val = (imp & (1<<i)) ? mask : 0; // get new value for bit
		if((implied[i][unit] & mask) != val)
		{			
		  implied[i][unit] = (implied[i][unit] & ~mask) | val; // set new value
		  implied_save();		
		}
	}
}

extern UInt8 extsensor_0status[UNIT_MAX];


Boolean is_implied_missing(UInt8 imp)
{
unsigned test,match,unit,i;
	
	test = (1<<AID_STATUS_BIT_UUID) | (1<<AID_STATUS_BIT_CONNECTED); // test both bits
	match = (1<<AID_STATUS_BIT_UUID); // ie, registered device not connected?
	
	for(i=0;i<MAX_IMPLIED;i++)
	{
		if(imp & (1<<i))
		{// check for this type of loss
			for(unit = 1; unit<UNIT_MAX; unit++)
			{
				if(  (aid_db[unit].status & test) == match  // missing device has...
					&& implied[i][unit] != 0  // ...any channel with this implication?
					)
				  return 1; // yes, something is missing
				if((extsensor_0status[unit]&DEFECT_MASK) &&
					 (implied[i][unit] != 0)
					)
				  return 1; // yes, we have peripheral defect
			}
		}
	}
	return 0; // no, nothing is missing
}

// not safe to remote start?
Boolean sensor_missing_safety(void)
{
	return is_implied_missing(IMPLIED_SAFETY);
}

// system is not completely online when arming?
Boolean sensor_missing_security(void)
{
	return is_implied_missing(IMPLIED_SECURITY);
}



UInt8 cmdsense_lookup(const UInt8 *name, UInt8 namelen)
{
UInt8 i;

	while (namelen && name[namelen - 1] <= ' ')
		namelen--;
	if (namelen == 0)
		return INVALID;

	for (i = 0; cmdsense[i].name; i++)
	{
		if (memcmp(name, cmdsense[i].name, namelen) == 0)
			return i;
	}
	return INVALID;
}


// what we keep in ram for each external sensor
struct extsensor_info
{
	UInt8 devchannel;
	UInt8 status;  // reported status bits
	UInt8 factory_bits; // what bits does it support?
	UInt8 cmdsense_index;   // 1+index for this signal
};


#define MAX_EXTSENSOR 16
UInt8 extsensor_count = 0;
struct extsensor_info extsensor[MAX_EXTSENSOR];
UInt8 extsensor_0status[UNIT_MAX];

UInt8 extsensor_lookup(UInt8 aid, UInt8 channel)
{
	UInt8 c, i;

	c = aid_lookup(aid);
	if (c == INVALID)
		return INVALID; // stranger device

	c = (c << 4) | (channel & 0xF); // devchannel format

	for (i = 0; i<extsensor_count; i++)
	{
		if (extsensor[i].devchannel == c)
			return i;
	}
	// a reporting channels from a registered device needs to be in our table!
	if (extsensor_count >= MAX_EXTSENSOR)
		return INVALID;  // won't fit?!?

	extsensor[i].cmdsense_index = INVALID;
	extsensor[i].factory_bits = 0;
	extsensor[i].status = 0;
	extsensor[i].devchannel = c;

	return extsensor_count++;
}

// when informing ds4, which source number do we use for this signal
UInt8 ext_from(UInt8 ext)
{
UInt8 from,idx,i;
	
	idx = extsensor[ext].cmdsense_index;
	
	from = 16;
	for(i=0;i<ext;i++)
	{
		if(extsensor[i].cmdsense_index == idx)
			from++; // some previous ext uses reports on the same cmdsense as I do
	}
	return from;
}
		
void ext_notify(UInt8 ext, UInt8 status)
{
	unsigned cmd;
	UInt8 idx;
	UInt8 from;

	if (ext == INVALID)
		return;
	
	idx = extsensor[ext].cmdsense_index;
	if (idx == INVALID)
		return;
	
	from = ext_from(ext);
	
	switch(idx)
	{
		case DOOR_INDEX: // door
			sense_rs_door(FALSE, status, from); // this will cmd_q_rs_insert if needed
			break;
		case HOOD_INDEX: // hood
			sense_rs_hood(FALSE, status, from); // this will cmd_q_rs_insert if needed
			break;
		case TRUNK_INDEX: // trunk
			sense_rs_trunk(FALSE, status, from); // this will cmd_q_rs_insert if needed
			break;
		case HANDBRAKE_INDEX:
			sense_rs_handbrake(FALSE, status, from); // this will cmd_q_rs_insert if needed
			break;
		case BRAKE_INDEX:
			sense_rs_brake(FALSE, status, from); // this will cmd_q_rs_insert if needed
			break;
		case IGN_INDEX:
			sense_rs_ign(FALSE, status, from); // this will cmd_q_rs_insert if needed
			break;
		case START_TRIGGER_INDEX:
			sense_rs_start_trigger(FALSE, status, from); // this will cmd_q_rs_insert if needed
			break;
		case CUSTOM_POS1_INDEX:
			sense_rs_custom_pos1(FALSE, status, from); // this will cmd_q_rs_insert if needed
			break;
		case HEADLIGHT_INDEX:
			sense_rs_headlight_detect(FALSE, status, from); // this will cmd_q_rs_insert if needed
			break;
		case TAMPER_INDEX:
			sense_rs_tamper(FALSE, status, from);
		  break;
		
		case PTIM_STOP_INDEX:
			sense_rs_ptim_stop(FALSE, status, from);
		  break;
		case EXT_OPEN_INDEX:
			//diag(0,(status ? 0xA1 : 0xA0) );
			sense_rs_ext_open(FALSE, status, from);
		  break;
		case EXT_CLOSE_INDEX:
			sense_rs_ext_close(FALSE, status, from);
		  break;		
		case EXT_TRUNK_INDEX:
			sense_rs_ext_trunk(FALSE, status, from);
		  break;
		
		default:
			cmd = (status ? cmdsense[idx].cmd_on : cmdsense[idx].cmd_off);
			if(cmd)
				cmd_q_rs_insert(cmd, 100);
			break;
	}
}


UInt32 *idx_pbits(UInt8 idx)
{
	switch(idx)
	{
		case DOOR_INDEX:
			return &sense_rs_doors_sts;
		case HOOD_INDEX:
      return &sense_rs_hoods_sts;
		case TRUNK_INDEX:
      return &sense_rs_trunks_sts;
    case IGN_INDEX:
      return &sense_rs_igns_sts;
		case BRAKE_INDEX:
      return &sense_rs_brakes_sts;
		case HANDBRAKE_INDEX:
      return &sense_rs_handbrakes_sts;			
		default:
			return NULL;
	}
}

void ext_notify_insert(UInt8 ext)
{
	UInt8 idx;
	UInt8 from;
	UInt32 *pbits;
  UInt32 mask;
	
	if (ext == INVALID)
		return;
	
	idx = extsensor[ext].cmdsense_index;
	if (idx == INVALID)
		return;
	
	pbits = idx_pbits(idx);
	from = ext_from(ext); // new bit number
	if(!pbits || !from)
		return;
	
	mask = (1<<from)-1; // everything below
	*pbits = (*pbits & mask) // everything below is preserved
	       | ((*pbits & ~mask)<<1) // everything above is shifted up by one
	       ;
}

void ext_notify_delete(UInt8 ext)
{
	UInt8 idx;
	UInt8 from;
	UInt32 *pbits;
  UInt32 mask;
	
	if (ext == INVALID)
		return;
	
	idx = extsensor[ext].cmdsense_index;
	if (idx == INVALID)
		return;
	
	pbits = idx_pbits(idx);
	from = ext_from(ext); // new bit number
	if(!pbits || !from)
		return;
	
	
	mask = (1<<from)-1; // everything below
	
	*pbits = (*pbits & mask) // everything below is preserved
	       | ((*pbits & ((~mask)<<1))>>1) // everything above is shifted down by one
	       ;
}


// how many security sensors does this device have?
static UInt8 has_security_sensors(UInt8 dev)
{
UInt8 i,cnt;

  cnt	= 0;
	for(i=0;i<extsensor_count;i++)
	{
		if((extsensor[i].devchannel >> 4) == dev && (extsensor[i].factory_bits & 0x06))
			cnt++; // yes
	}
	return cnt;
}

void sensor_proto_device_lost(UInt8 dev)
{	
	extsensor_0status[dev] = 0; // we lost this device
	
	// are there any security sensor channels associated with this device?
	if(get_ss_arm() && !get_ss_valet() && has_security_sensors(dev))
	{
		cmd_q_rs_insert( INPUT_SENSOR_TRIGGER , 100 );	
	}
    
    // is rs running?
  if(dei_rs_is_active() && sensor_missing_safety())
	{
		cmd_q_rs_insert( INPUT_BRAKE_STATUS_PRESS , 100 );	
	}
}

void sensor_proto_device_lost_force_notify(UInt8 dev)
{
	aid_set_diagnostic(aid_db[dev].id, DS_PERIPHERAL_LOST);
	sense_diagnostic_services(DS_PERIPHERAL_LOST);
}


UInt8 sensor_proto_zonecheck(void)
{
UInt8 zones,dev,i;
	
	zones = 0;
	// check connected zones
	for(i=0;i<extsensor_count;i++)
	{
		dev = extsensor[i].devchannel>>4;
		if(  !(aid_db[dev].status & (1<<AID_STATUS_BIT_CONNECTED))
			|| (extsensor_0status[dev] & DEFECT_MASK) // trouble
			)
		{
			
			if(extsensor[i].cmdsense_index != INVALID)
			{
				zones |= 1<<cmdsense[extsensor[i].cmdsense_index].trigger_zone;
				if(extsensor_0status[dev] & DEFECT_MASK)
					zones |= 1<<DEI_SS_TRIGGER_ZONE_SENSOR3;
			}
		}
	}
	// check for unconnected devices
	if(sensor_missing_security())
		zones |= (1<<DEI_SS_TRIGGER_ZONE_SENSOR3); // call it "other" for now
	return zones & ~(1<<DEI_SS_NO_TRIGGER); // here are the zones in trouble, but there is no zone 0
}


void diag(UInt8 who, UInt8 what)
{
UInt8 pld[2];
	pld[0] = who;
	pld[1] = what;
	proto_sendmsg(PROTO_DIAG,pld,2);
}

void send_requested_E1(void)
{
	UInt8 id,chan;
	UInt8 pld[2];
	 sensadj_get_current_idchan(&id,&chan);
	 if(id)
	 {
		 pld[0] = requested_bits;
		 pld[1] = (requested_bits & TEST_MASK) ? sensadj_get_current_index() : 0;  // for LED pattern, or OFF
		 aid_db[aid_lookup(id)].status &= ~(1<<AID_STATUS_BIT_ENABLED);

     sensor_proto_tx(PLD_E1, id, chan , pld, 2 );
	 }
}

void send_identify_request(void)
{
UInt8 id,chan;
	
	sensadj_get_current_idchan(&id,&chan); // which one is it?
  sensor_proto_tx(PLD_E4, id, chan, NULL, 0);  // ask about their factory bits	
	factory_bits = 0; // because I know nothing
	requested_bits = 0xFF; // I'll request as many messasge types as there are
}

void sensadj_exit_actions(void)
{
	UInt8 id,chan;
	
	sensadj_get_current_idchan(&id,&chan);
	if(id)
	{				
		requested_bits = (factory_bits ? factory_bits : 0x06);	// nevermind what installer asks, we always restore factory bit or best guess
		send_requested_E1();
		
		factory_bits = 0;
		sensor_proto_via_ble_exit();
		
		sensadj_exittest();
		
	}
}

UInt8 ngss_sensor_alertbits=0;
void d2d2_ble_sense_handler(UInt8 *pkt, UInt8 len)
{
  UInt8 cmd_pkt[8];
  UInt8 cmd_pkt_len;
	UInt8 id,chan;
	UInt8 pld[2];
	
	
	sensadj_get_current_idchan(&id,&chan); // get current test channel, if any
	
  if(pkt[4] == SENSOR_PROTO_WRITE_REP_BIT)   // WRITE_REPORT_BITS
  {
    if(pkt[5] & TEST_MASK)
    {// is packet setting test mode?
			if(get_ss_arm())
			{
				diag(pkt[0],DIAG_MODE);  // we can't do that when armed
				return;
			}
			
			if(id != pkt[2] || chan != pkt[3])
			{
				if(!sensadj_entertest_idchan(pkt[2],pkt[3]))  // meanwhile try to put this channel in test mode
				{// failed!
						if(id) // were we previously in testmode with a real channel?
							sensadj_entertest_idchan(id,chan);  // yes, restore previous test channel						
						
						diag(pkt[0],DIAG_PARAM); // meanwhile, complain to sender
						return;
				}
				// ok, we were able to set test mode on the new channel...
			
				requested_bits = pkt[5]; // save what the installer asks for
				
				if(id && factory_bits)
				{// say goodbye to old test channel
					pld[0] = factory_bits;
					pld[1] = 0; // turn off LED
					sensor_proto_tx(PLD_E1,id,chan,pld,2);
				}
				factory_bits = 0;
				
				// we need to get factory bits for this new channel
				send_identify_request();
				return; // wait for device reply				
		  }
			// this is (possibly different) bit settings within the current test channel
			
		  requested_bits = pkt[5];  // update the installer's request
			if(factory_bits == 0) // still waiting for device reponse?
			{
				send_identify_request();
				return;
			}
			
      requested_bits &= (factory_bits | TEST_MASK); // DS4 never asks for bits the device does not have
      send_requested_E1();
			      
    }
    else
    {// packet exiting test mode
			requested_bits = 0;
			if(id) // are we testing some device right now?
			{// installer exits test mode				
				sensadj_exit_actions();
			}
			else
			{
				diag(pkt[0],DIAG_MODE);
			}
    }
  }
}

//Boolean is_ignore_sensors(void)
//{
//	UInt8 chirp;
//	
//	chirp = prg_out_shadow_read(1+PRG_OUT_SIREN);
//	if(get_ss_valet()
//		|| get_rs_gwr()
//	  || chirp == DEI_SIREN_TRIGGER  //siren is currently playing something important
//	  || chirp == DEI_SIREN_CHIRP_THEN_TRIGGER
//    || (sensadj_get_current_index()==0 && !get_ss_arm())  // disarmed and not sensor adjust?
//	  )
//		return TRUE; // ignore sensor
//	
//	return FALSE; // process normally
//}

void sens_process(UInt8 *pkt, UInt8 len)
{
	UInt16 val;
	UInt8 aid_sender, cmd, chanl;
	UInt8 *pld;
	
	UInt8 ext,idx;
	UInt8 buf[8];
	UInt8 id, chan;

	if(len < 5)
		return;  // there's no command there
	
	if(pkt[2] != AID_MASTER)
	{
		// not for me		
		d2d2_ble_sense_handler(pkt,len);		
		return;
	}
	
	aid_sender = pkt[0]; // who sent this
	chanl = pkt[3];
	cmd = pkt[4];
	pld = pkt+5;  // point to rest of payload
	len -= 5;     // length of rest of payload
	
	// prepare response buffer
	buf[0] = aid_sender;
	buf[1] = chanl;

	ext = extsensor_lookup(aid_sender, chanl); // do I know you?
	sensadj_get_current_idchan(&id,&chan); // also get current channel-under-test information

	switch(cmd)
	{
		case SENSOR_PROTO_READ_STAT_RESP:
		{
			if(chanl == 0)
			{
				idx = aid_lookup(aid_sender); // which device is this?
				if(idx != INVALID) // it is our device
				{
					val = ((extsensor_0status[idx] ^ (*pld)) & (*pld));  // which status bits became high?
					extsensor_0status[idx] = *pld;                   // (meanwhile update status). This is also used in implied safety now wor checking fault/defect.
					
					
					if(get_ss_arm() && !get_ss_valet() && has_security_sensors(idx))
					{
						if((val & 0x8c)) // // alarm, test, or fault while armed? 
						{
							cmd_q_rs_insert( INPUT_SENSOR_TRIGGER , 100 );
							aid_set_diagnostic(aid_sender,pld[0]);  // save first byte (a binary code readable by computers)
						}
						else if(val & 0x02) // warning?
							cmd_q_rs_insert( INPUT_SENSOR_WARNING, 100 );							
					}
					// else not armed, so we should do what???  TODO: other warnings?
				}	
				break;  // end of status for channel 0
			}
			
			// check for alarm or warning
			if(WARN_MASK & *pld )
			{
				if(ext != INVALID)
				  extsensor[ext].status = *pld;  // DO save reported status
				
//				if(is_ignore_sensors())
//					break;  // don't do anything more
				
        if(!id)
        {
          set_sensor_status( TRUE );
          sensor_proto_tx(PLD_E4, aid_sender, chanl, 0, 0);
          cmd_q_rs_insert( INPUT_SENSOR_WARNING , 100 );	
				}
				
				if(id || dei_sc_get_mode() == DEI_SC_MODE_SENSOR_ADJUST) 
				{
					dei_siren_chirp( DEI_SIREN_SENSOR_WARNING ); 				
					dei_prk_flash( DEI_PRK_LOCK );
				}
			}
		
			if(ALARM_MASK & *pld)
			{
				if(ext != INVALID)
				{
				  extsensor[ext].status = *pld;			// DO save reported status		
				}
//				if(is_ignore_sensors())
//					break;  // don't do anything more
				
        if(id == 0)
        {
          set_sensor_status( TRUE );	
          // send request to check for SENSOR TYPE
          sensor_proto_tx(PLD_E4, aid_sender, chanl, 0, 0);
          cmd_q_rs_insert( INPUT_SENSOR_TRIGGER , 100 );	
        }
				
				if(id || dei_sc_get_mode() == DEI_SC_MODE_SENSOR_ADJUST) 
				{
					dei_siren_chirp( DEI_SIREN_SENSOR_TRIGGER ); 				
					dei_prk_flash( DEI_PRK_UNLOCK_NO_EVENT );
				}
				
				set_trig_sensor_index(0);
				set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_UNKNOWN);
			}
			

			// maybe its just an I/O or value change (general case)
			if (ext != INVALID)   // strangers cannot inject vehicle signals
			{
				idx = extsensor[ext].cmdsense_index; // is the command defined yet?
				if (idx != INVALID 
					|| (idx == INVALID && chanl == 3 && ((tempsens_cap & TEMPSENS_NGSS_AVAIL) == 0)))
				{ 
					if(idx == INVALID && chanl == 3 && ((tempsens_cap & TEMPSENS_NGSS_AVAIL) == 0))   // not in the database yet during sens_cap_polling
						idx = TEMPERATURE_INDEX;
					
					// yes, we know the command
					if(len <= 2)
					{
						if ((*pld ^ extsensor[ext].status) & EXCEPTION_MASK) // changing exception value?
						{
							ext_notify(ext, *pld & EXCEPTION_MASK);
						}
				  }
					else
					{
						if (len > 2) // value supplied?
						{
							val = pld[1];
							if (len >= 3) // 2 byte value supplied?
								val = (val << 8) | pld[2];

							// special known cases
							if (idx == TACH_INDEX)
								set_rpm_wire(val);  // philosophically, is this rpm "data" or "wire"?
							else if (idx == TEMPERATURE_INDEX)
							{
								if((!ble_siren_temp_avail)&&(aid_sender != tempsens_aid(2)))
								{
								  ble_siren_temp_avail = 1;
									tempsens_cap |= TEMPSENS_WSS_AVAIL;
								}
								sensor_temp_update((SInt16)val);   //degC_x8
							}
						}
					}
				}

				extsensor[ext].status = *pld; // finally update status from registered sensors only
				if (extsensor[ext].cmdsense_index == INVALID && extsensor[ext].factory_bits == 0)  // have we never seen an identify for this?
				{
					buf[2] = SENSOR_PROTO_SENSOR_TYPE_REQ; //SENS_IDENTIFY_CMD;
					proto_sendmsg(PROTO_SENS, buf, 3); // meanwhile ask who/what there are. 
				}
			}
			
			// this code works only for 1 sensor, not many. FIXME: this is still needed for shock and tilt, but wrong for general i/o
			if((NORMAL_MASK & *pld) && id == 0)
			{
				set_sensor_status( FALSE );
			}
				
		}
		break;
		case SENSOR_PROTO_WRITE_REP_BIT_RESP:
		{
//			if( (*pld) | 0x08)		// if testmode bit is set , then ignore the new settings
//				set_shock_sensor_alert_bits( *(pld+1) );
//			else
//				set_shock_sensor_alert_bits( *(pld) );
				
		}
		break;
		case SENSOR_PROTO_READ_CONFIG_RESP:
		{
		//	if((aid_sender == tempsens_aid(2)) & ( chanl == 1/*get_shock_chanl_num()*/))			// if it is shock sensor
			{
				os_evt_signal(sensor_proto_evt, SENSOR_PROTO_CONFIG_RECV);
				// update the configuration for Shock sensor
				set_shock_sensor_detect_bits(*pld);
				//set_shock_sensor_alert_bits( *(pld+1));
				set_shock_sensor_level(*(pld+2));
			}
		}
			break;
		case SENSOR_PROTO_ALARM_LEVEL_RESP:
		{
			//if((aid_sender == tempsens_aid(2)) & ( chanl == 1/*get_shock_chanl_num()*/))			// if it is shock sensor
			{
				// update the configuration for Shock sensor
				set_shock_sensor_level(*pld);
				os_evt_signal(sensor_proto_evt, SENSOR_PROTO_WRITE_CONFIG);
				os_evt_signal(sensor_proto_evt, SENSOR_PROTO_CONFIG_RECV);
			}
		}
			break;
		case SENSOR_PROTO_SENSOR_TYPE_RESP: // identify
		{
			if(id == pkt[0] && chan == pkt[3])
			{// device channel under test reports factory bits to us
				if(factory_bits == 0) // first report for this channel?
				{
		      sensor_proto_via_ble_enter();					
				}
				factory_bits = pkt[5];
				
				requested_bits &= (factory_bits | TEST_MASK); // DS4 never asks for bits the device does not have

				if(requested_bits)
				{
					send_requested_E1();  // send what was requested
					// ask for current level
					if((requested_bits & TEST_MASK) && dei_ss_sensor_adjust_src) // did we just ask for testmode?
					{
							sensor_proto_tx(PLD_E8,id,chan,0,0); // ask for current level
					}
					requested_bits = 0; // nothing pending anymore					
				}
			}
						
			idx = cmdsense_lookup(pld+1,len-1);
			
			if(idx == FIELD_INDEX)
			{// set icon type for next supercode message
				notify_send(NOTIFY_SENSOR3_TYPE, NOTIFY_PLD_SENSOR_FIELD);
			}
			else if(idx == GLASS_INDEX)
			{// set icon type for next supercode message
				notify_send(NOTIFY_SENSOR3_TYPE, NOTIFY_PLD_SENSOR_GLASS);
			}
			else if(idx == ULTRA_INDEX)
			{// set icon type for next supercode message
				notify_send(NOTIFY_SENSOR3_TYPE, NOTIFY_PLD_SENSOR_ULTRA);
			}

			if (ext != INVALID) // registered devices only
			{				
				extsensor[ext].factory_bits = *pld;
				
				if(idx == HEADLIGHT_INDEX)
						set_headlight_source (SOURCE_WIRE);

				if (extsensor[ext].cmdsense_index != idx)
				{// changed name? or we are just learning or losing the name of this input?
					if (extsensor[ext].cmdsense_index != INVALID)
					{// old name was valid
						if (extsensor[ext].status & (EXCEPTION_MASK | WARN_MASK | ALARM_MASK))
						{
							ext_notify(ext, 0); // clear whatever previous status was set
						}
						ext_notify_delete(ext); // tell DS4 this signal no longer exists
						extsensor[ext].cmdsense_index = idx; // NOW we change the name
						extsensor[ext].status = 0;  // we don't know the new signal's status yet

						if (idx != INVALID)
						{
							ext_notify_insert(ext); // tell DS4 this signal now exists
							// TODO: perhaps send a "read status" command to be sure
						}
					}
					else // old name was invalid. this can happen when status arrives before identification (shock/tilt)
					{
						extsensor[ext].cmdsense_index = idx; // change to name FIRST
						ext_notify_insert(ext); // tell DS4 this signal now exists
						
						if (idx != INVALID && (extsensor[ext].status & (EXCEPTION_MASK | WARN_MASK | ALARM_MASK)))
						{
							if ((extsensor[ext].status & (ALARM_MASK | WARN_MASK)) == 0)
							{// there was a previous I/O exception status
								ext_notify(ext, extsensor[ext].status & EXCEPTION_MASK); // then notify whatever status            
							}
							else
							{
								//TODO: previous status is ALARM or WARN, send scpp message
							}
						}
					}
				}
				// else name did not change.  factory bits could have been updated, but so what...			
				set_implied_for_unit_channel( aid_lookup(aid_sender), chanl, (idx==INVALID ? 0 : cmdsense[idx].implied) );
				if(idx == INVALID && (*pld & (WARN_MASK | ALARM_MASK))==0)
				{// safe to shut down unrecognized non-security sensor
					buf[2] = SENSOR_PROTO_WRITE_REP_BIT; //SENS_SET_REPORTBITS;
					buf[3] = 0; // set reporting bits to off 

					proto_sendmsg(PROTO_SENS, buf, 4);   // set report bits to be sure
					
				}
			}
			else
			{ // unregistered device
				if(idx == SHOCK_INDEX || idx ==  TILT_INDEX || idx == FIELD_INDEX || idx == GLASS_INDEX || idx == ULTRA_INDEX) // some sensor channel?
				{
					buf[2] = SENSOR_PROTO_WRITE_REP_BIT; //SENS_SET_REPORTBITS;
					buf[3] = *pld; // set reporting bits to whatever factory bits they have 

					proto_sendmsg(PROTO_SENS, buf, 4);   // set report bits to be sure
				}
			}
			
      // test for shock or tilt seperately because strangers are allowed			
			switch(idx)
			{
				case SHOCK_INDEX: // SHOCK
				{
					if(aid_sender == tempsens_aid(2))
					{
						set_shock_chanl_num(chanl);
					}
					if(get_sensor_status())
					{
						set_sensor_status( FALSE );
						set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_SHOCK);
						set_ss_warn_src(DEI_SS_SENSOR_WARN_SHOCK);
						set_trig_sensor_index(1);
						os_evt_signal(sensor_proto_evt, SENSOR_PROTO_ALRM_SRC);
					}
				}
					break;
				case TILT_INDEX:		// TILT
				{
					if(aid_sender == tempsens_aid(2))
					{
						set_shock_chanl_num(chanl);
					}
					if(get_sensor_status())
					{
						//set_sensor_status( FALSE );
						set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_TILT);
						set_ss_warn_src(DEI_SS_SENSOR_WARN_TILT);
						set_trig_sensor_index(2);
						os_evt_signal(sensor_proto_evt, SENSOR_PROTO_ALRM_SRC);
					}
				}
				break;
				case FIELD_INDEX: // FIELD
				{
					if(aid_sender == tempsens_aid(2))
					{
						set_shock_chanl_num(chanl);
					}
					if(get_sensor_status())
					{
						set_sensor_status( FALSE );
						set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_FIELD);
						set_ss_warn_src(DEI_SS_SENSOR_WARN_FIELD);
						set_trig_sensor_index(0);
						os_evt_signal(sensor_proto_evt, SENSOR_PROTO_ALRM_SRC);
					}
				}
				
				case GLASS_INDEX: // GLASS
				{
					if(aid_sender == tempsens_aid(2))
					{
						set_shock_chanl_num(chanl);
					}
					if(get_sensor_status())
					{
						set_sensor_status( FALSE );
						set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_GLASS);
						set_ss_warn_src(DEI_SS_SENSOR_WARN_GLASS);
						set_trig_sensor_index(0);
						os_evt_signal(sensor_proto_evt, SENSOR_PROTO_ALRM_SRC);
					}
				}
				
				case ULTRA_INDEX: // ULTRA
				{
					if(aid_sender == tempsens_aid(2))
					{
						set_shock_chanl_num(chanl);
					}
					if(get_sensor_status())
					{
						set_sensor_status( FALSE );
						set_ss_trig_src(DEI_SS_SENSOR_TRIGGER_ULTRA);
						set_ss_warn_src(DEI_SS_SENSOR_WARN_ULTRA);
						set_trig_sensor_index(0);
						os_evt_signal(sensor_proto_evt, SENSOR_PROTO_ALRM_SRC);
					}
				}
				
				case 2:		// TEMPSENS
				{
					if(aid_lookup(aid_sender) == get_temp_sensor_db_num() && ext != INVALID)
						set_temp_chanl_num( chanl );
				}
					break;
				default:
					break;
			}
			
		}
		break;
		
		case SENSOR_PROTO_DIAGNOSTIC_RESP:
			if(chanl == 0)
			{
				extsensor[ext].status = *pld;
				if(pld[0] == WIRELESS_PERIPH_PAIR_SUCCESS)
				{
					/* special case to notify the CCM pairing successful, DS4 will do lock then unlock */
					cmd_q_rs_insert(INPUT_UNLOCK_DRIVER1 | INPUT_ON, 100, 0);   // LOCK first
					cnt_unlock_4_confirmation++;
					vrtc_stop(vrtc_wireless_periph_pair_confirm);
					vrtc_update(vrtc_wireless_periph_pair_confirm, VRTC_1SEC *1);
					vrtc_start(vrtc_wireless_periph_pair_confirm);
					// and 1 chirp in diagnostic_services
				}
				sense_diagnostic_services(extsensor[ext].status);

				aid_set_diagnostic(aid_sender,pld[0]);  // save first byte (a binary code readable by computers)
			}
			break;
			
		default:
		break;
	}
}

UInt8 sensor_proto_get_factory_bits(void)
{
	return factory_bits;
}

void sensor_proto_set_factory_bits(UInt8 bits)
{
	factory_bits = bits;
}

void sensor_proto_delete_unit(UInt8 u)
{
UInt8 chg,i,j,ext;

  // first we remove safety and security implications
  chg = 0;
  for(i=0;i<MAX_IMPLIED;i++)
  {
    if(implied[i][u])  // any sense channels having this implication?
    {
      implied[i][u] = 0;
      chg = 1;
    }
  }
  if(chg)
    implied_save(); // write changes to nvfs
    

  // de-register all sensors registered to this device
  for(ext = extsensor_count; ext--; ) // process from last to first, so ext_from() will still work
  {
    if((extsensor[ext].devchannel >> 4) == u)
    {
      if(extsensor[ext].cmdsense_index != INVALID)
      {
        ext_notify(ext,0); // clear whatever previous status there was
        ext_notify_delete(ext); // unassign the bit position for this sense
      }
    }
  }

  // remove all extsensors declared for this unit
  j=0;
  for(i=0;i<extsensor_count;i++)
  {
    if((extsensor[i].devchannel >> 4) != u)
    {// we keep this one
      if(i!=j)
        memcpy(&extsensor[j],&extsensor[i],sizeof(struct extsensor_info));
      j++;
    }
  }
  extsensor_count = j; // reduced count appropriately

  extsensor_0status[u] = 0; // forget any trouble status
}


//--------------------------------------------------------------------------//
void sensor_proto_via_ble_enter(void)
{
    // siren chirp
    dei_siren_chirp( DEI_SIREN_SENSOR_ADJUST_ENTER ); // to give long siren pulse for entering Sensor Adjust mode
}

void sensor_proto_via_ble_exit(void)
{
    dei_siren_chirp( DEI_SIREN_SENSOR_PROGRAMMING_EXIT ); // to give long siren pulse for exit Sensor Adjust mode

}

/* timeout after 1st activity of pairing confirmation for wireless peripheral */
static void wireless_periph_pair_confirm_timeout(void)
{
	cmd_q_rs_insert(INPUT_UNLOCK_DRIVER1 | INPUT_ON, 100, 0);   // UNLOCK after certain sec from lock
	vrtc_stop(vrtc_wireless_periph_pair_confirm);
	if (++cnt_unlock_4_confirmation < MAX_CNT_UNLOCK_4_CONFIRMATION)
	{
		vrtc_update(vrtc_wireless_periph_pair_confirm, VRTC_1SEC * 1);
		vrtc_start(vrtc_wireless_periph_pair_confirm);
	}
	else
	{
		cnt_unlock_4_confirmation = 0; // may get paired again
	}
}


/* sense diagnostic services */
static void sense_diagnostic_services(UInt8 code)
{
	UInt8 severity;
	
	/* To be adjusted per UI definition: level 1 is lowest; level 6 is higest */
  Dei_Prk_Pattern plight_pattern[] = {
		DEI_PRK_OFF, 										// no action
		DEI_PRK_LOCK,    						    // 1 p-light flash
		DEI_PRK_UNLOCK_LOW_BATT,        // 3 p-light flashes
		DEI_PRK_ERROR_START_HOOD,       // 6 p-light flashes
		DEI_PRK_ERROR_LOW_TACH          // 9 p-light flashes
	};
	Dei_Siren_Chirp chirp_pattern[] = {
		DEI_SIREN_ARM,                   // 1 chirp
		DEI_SIREN_REMOTE_DELETE_ENTER    // 2 chirps				
	};
  	
	// handle the code to indicate
	severity = (code >>5) & 0x07;
	switch (severity)  //handle the code of system level only
	{
		case DS_SEVERITY_LEVEL_1:
		case DS_SEVERITY_LEVEL_2:
		case DS_SEVERITY_LEVEL_3:
		case DS_SEVERITY_LEVEL_4:	
			dei_prk_flash(plight_pattern[severity]);
			break;
		case DS_SEVERITY_LEVEL_5:
		case DS_SEVERITY_LEVEL_6:
			dei_siren_chirp(chirp_pattern[severity - DS_SEVERITY_LEVEL_5]);
			break;
		default:
			// do nothing
			break;
	}
	if (code == WIRELESS_PERIPH_PAIR_SUCCESS)
	{
		/* 1 p-light flash for the coonfirmation of wireless perippheral pairing  */
		dei_prk_flash(DEI_PRK_LOCK);
	}
}

void extsensor_init(void)
{
	memset(extsensor_0status,0,UNIT_MAX);
	extsensor_count = 0;

    implied_init();
	
	vrtc_wireless_periph_pair_confirm = vrtc_f_create((Exec_Func)wireless_periph_pair_confirm_timeout);
	cnt_unlock_4_confirmation = 0;
}
