#include "aid_dhp.h"
#include "intl_port.h"
#include "sensor_proto.h"   //SENSOR_PROTO_SENSOR_TYPE_REQ
#include "nvfs.h"
#include "nvfs_usr.h"
#include "dhp_math.h"
#include "aes128.h"
#include "timestamp.h"
#include "vrtc.h"
#include "vrtc_f.h"
#include "per_exec.h"
#include "temperature.h"
#include "display.h"
#include "dei_feature.h"
#include <string.h>
#include "status_system.h"
#include "system_ui.h"
#include "dei_led.h"
#include "dei_system.h"

unsigned dhp_timer_var;

UInt16 my_appcap;
UInt8  ble_card_aid;
UInt8 ble_card_sn[4];  //4 bytes SN of BLE
UInt8 sys_in_low_voltage_mode;
UInt8 dhp_timer_running;
UInt16 re_searching_units=0;    // will be shown at then of the power-up
UInt8 aid_scan_in_progress;
static UInt8 owe_mainp_scanning = 0;

static void master_appcap(void);
static Boolean is_ccm_device(UInt8 n);

#define DHP_HZ  100  //10ms timebase

void dhp_timer(UInt16 ms)
{     
  dhp_timer_var = (ms * DHP_HZ + 999)/1000;
	dhp_timer_running = 1;
}

// return true if there is less than argument milliseconds remaining on the dhp_timer
int dhp_timer_lessthan(UInt16 ms)
{
	return 1000*dhp_timer_var < DHP_HZ * (unsigned int)ms;
}

void dhp_timer_stop(void)
{
    dhp_timer_var = 0;
	  dhp_timer_running = 0;
}


#define  AID_EXCEPTION_VRTC_DELTA (60*VRTC_1SEC)
#define  AID_EXCEPTION_LIMIT      3

UInt32 aid_idbit[256/32];  //aligned-32 for faster access
UInt8  aid_exception_count;
UInt8 have_devices=0;
UInt8 have_wireless=0;

Vrtc aid_except_vrtc;

void dhp_timer(UInt16 ms);
void dhp_timer_stop(void);
void dhp_sendparams(void);
UInt8 uuid_subsearch(UInt8 *text,UInt8 ofs,UInt8 len);
void aid_except_vrtc_timetout(void);

struct uuid_record
{
    UInt8 uuid[UUID_SIZE];
    UInt8 skey[KEY_SIZE];
};


UInt8 aid_state = AID_STATE_CLOSED;

struct aid_entry aid_db[UNIT_MAX];
UInt8  sort_slot[UNIT_MAX];

UInt8 device_verif_state = DEVICE_VERIF_DONE;
UInt8 device_verif[UNIT_MAX];
UInt8 device_search_idx=0;
UInt8 device_search_cur=0;
UInt8 device_in_check_uuid[UUID_SIZE];


/* these 2 routines are because we are a little-endian machine */
#define ntohs htons
UInt16 htons(UInt16 x)
{
    return (x<<8) | (x>>8);
}

#define ntohl htonl
UInt32 htonl(UInt32 x)
{
    return (((UInt32)htons(x)<<16) | (UInt32)htons(x>>16));
}



const UInt8 dhp_constants[17]=
{
    2,
    
    0x54, 0xDB, 0x41, 0xA4,
    0x6E, 0x47, 0x93, 0x7C,
    0xB9, 0xEB, 0xB2, 0x12,
    0xA9, 0x94, 0x8B, 0xC9  
};

#define dh_G (dhp_constants[0])
#define dh_P (dhp_constants + 1)

// get nvfs name for unit i
char *unit_name(UInt8 i)
{
static char name[4];

    memcpy(name, ".0U", 4);
    name[1] += i;
    return name;
}

// get first unit number with uuid matching substring
UInt8 uuid_subsearch(UInt8 *text,UInt8 ofs,UInt8 len)
{
UInt8 i;
UInt8 buf[UUID_SIZE];
    if(ofs < UUID_SIZE)
    {
        if(len+ofs >= UUID_SIZE)
            len = UUID_SIZE - ofs;
            
        for (i = 0; i < UNIT_MAX; i++)
        {
            if( (aid_db[i].status & (1<<AID_STATUS_BIT_UUID))
                && nvfs_rd_at(unit_name(i), 0, buf, UUID_SIZE) == UUID_SIZE
                && memcmp(buf+ofs, text, len) == 0
                )
                return i; // found
        }
    }
    return 0xFF;
}

// only works for 2 or 3 !!!
UInt8 tempsens_aid(UInt8 e_num)
{
UInt8 i;
static UInt8 *uname[2] =
{
    "NGSS",
    "IVU "
};

    if(e_num >= 2 && e_num < 3+1)
    {
        i = uuid_subsearch(uname[e_num-2],8,4);
        if(i != 0xFF)
        {
            if(aid_db[i].status & (1<<AID_STATUS_BIT_CONNECTED))
                return aid_db[i].id;
        }
  }
    return 0;
}



// get unit number using uuid
UInt8 uuid_lookup(UInt8 *uuid)
{
    return uuid_subsearch(uuid,0,UUID_SIZE);
}

// get unit number using aid
UInt8 aid_lookup(UInt8 id)
{
UInt8 i;

    for (i = 0; i < UNIT_MAX; i++)
    {
        if (aid_db[i].id == id)
            return i;
    }
    return 0xFF;
}

void aid_delete(UInt8 i)
{
    nvfs_delete(unit_name(i));
    memset(&aid_db[i], 0,sizeof(struct aid_entry));
}

void check_offer_pair(UInt8 i,UInt8 has_sensors)
{
		if(has_sensors) // any sensors at all?
				sensor_proto_reminder(i);	// notify this device if system is armed
	
	
    if (  aid_state <= AID_STATE_CLOSED_WTMR // not when closed
       || i >= UNIT_MAX
       || (aid_db[i].status & ((1<<AID_STATUS_BIT_OFFER))) != 0
       || !IS_SLAVE_ID(aid_db[i].id)
       || (aid_db[i].status & (1<<AID_STATUS_BIT_KEY)) != 0
       )
        return;

    // invite them to try to pair
    aid_db[i].status |= (1<<AID_STATUS_BIT_OFFER);
    proto_sendmsg(PROTO_DHP, &aid_db[i].id, 1); // mention we don't have a session key
		save_normal_fail_dbg_msg(aid_db[i].id);  // save pairing counter
}

/* aid read/write */
unsigned rd_idbit(UInt8 num)
{
  return(aid_idbit[num>>5] & (1<<(num & 31)));
}

void wr_idbit(UInt8 num,unsigned val)
{
  if(val)
    aid_idbit[num>>5] |= (1<<(num & 31));
  else
    aid_idbit[num>>5] &= ~(1<<(num & 31));
}

UInt8 aid_replacement(UInt8 id)
{
    UInt8 x;

    for(x = id+1; x != id; x++)
    {
			if(!IS_SLAVE_ID(x))
				x = 0x20;
			
      if(rd_idbit(x)==0)
        break; // found next free id
    }
    if(x == id)
    {// bitmap table is full!
      for(x = id+1; x != id; x++)
      {
        if(aid_lookup(x) == 0xFF)    //not valid
        {
          wr_idbit(x,0); // MAKE this a free bit!
          break; // found victim
        }
      }
    }
    return x;
}
//
#define AID_DIAG_NOT_EXIST 0xFF
#define AID_DIAG_NO_INFO  0xFE
UInt8 aid_get_diagnostic(UInt8 id)
{
UInt8 i;
	
	for(i=0;i<UNIT_MAX;i++)
	{
		if(id == aid_db[i].id)
			return aid_db[i].diagnostic;
	}
	return(rd_idbit(id) ? AID_DIAG_NO_INFO : AID_DIAG_NOT_EXIST);
}

void aid_set_diagnostic(UInt8 id,UInt8 val)
{
UInt8 i;
	for(i=1;i<UNIT_MAX;i++)
	{
		if(id == aid_db[i].id)
		{
			aid_db[i].diagnostic = val;
			return;
		}
	}
	wr_idbit(id, 1);  // unregistered id seems to be in use	
}


/**
* @brief  void check_sensor_capability(UInt8 id)
* @param
* @return
*/
void check_tempsens_capability(UInt8 id)
{
    UInt8 sens_msg[16];
    UInt8 sens_msg_len=0;

    sens_msg[sens_msg_len++] = id;
    sens_msg[sens_msg_len++] = 0;  //channel
    sens_msg[sens_msg_len++] = SENSOR_PROTO_SENSOR_TYPE_REQ;
    sens_msg[sens_msg_len++] = 0;
    sens_msg[sens_msg_len++] = 'T';
    sens_msg[sens_msg_len++] = 'E';
    sens_msg[sens_msg_len++] = 'M';
    sens_msg[sens_msg_len++] = 'P';
    proto_sendmsg(PROTO_SENS, sens_msg, (sens_msg_len & (16-1)));
}


void check_shocksens_capability(UInt8 id)
{
    UInt8 sens_msg[16];
    UInt8 sens_msg_len=0;

    sens_msg[sens_msg_len++] = id;
    sens_msg[sens_msg_len++] = 0;  //channel
    sens_msg[sens_msg_len++] = SENSOR_PROTO_SENSOR_TYPE_REQ;
    sens_msg[sens_msg_len++] = 0;
    sens_msg[sens_msg_len++] = 'S';
    sens_msg[sens_msg_len++] = 'H';
    sens_msg[sens_msg_len++] = 'O';
    sens_msg[sens_msg_len++] = 'C';
    sens_msg[sens_msg_len++] = 'K';
    proto_sendmsg(PROTO_SENS, sens_msg, (sens_msg_len & (16-1)));
}


/**
* @brief  void aid_new_connection_notify(UInt8 i) 
*         send proto_sens to internal port
* @param
* @return
*/
void aid_new_connection_notify(UInt8 i)
{
    UInt8 sens_num[1];
    UInt8 sens_msg[16];
    UInt8 sens_msg_len=0;
    
    nvfs_rd_at(unit_name(i), UUID_SIZE-3, sens_num, 1);

    if (aid_state == AID_STATE_CLOSED && sens_num[0])   //has sensor
    {
       if( aid_db[i].id == tempsens_aid(2) && (tempsens_config == TEMPSENS_NGSS) )  //NGSS
       {
           check_tempsens_capability(aid_db[i].id);
       }
       
       if( aid_db[i].id == tempsens_aid(2) /*!dei_feature_get(DEI_FEAT_SENSOR_TYPE)*/ )
       {
           check_shocksens_capability(aid_db[i].id);
       }
    }
}



/**
* @brief  UInt8 check_aid_info(UInt8 aid) 
*         check the type and capablity of the device aid
*         represented.
* @param
* @return  2: NGSS, 3: IVU
*/
UInt8 check_aid_info(UInt8 aid)
{
    UInt8 i;
    UInt8 ret=0;
    for(i = 1; i < UNIT_MAX; i++)  //0: master
    {
        if(aid == aid_db[i].id)
        {
            if(tempsens_aid(2))
                ret = 2;
            else if(tempsens_aid(3))
                ret = 3;

            break;
        }
    }
    return ret;
}

// return which ble name is detected?
UInt8 is_ble_claiming(UInt8 *ptr)
{
static const UInt8 *uname[] =
{
    "DEI-BLE ",
    "DEI-SKEY",
	  NULL
};

UInt8 i;
    for(i=0;uname[i];i++)
    {
      if(  memcmp(ptr+7, uname[i], 8) == 0)
				return i+1;
	  }

    return 0;
}

// master send this full AID packet
void sendback(UInt8 *ptr, UInt8 len)
{
    proto_sendmsg(PROTO_AID, ptr, len);  //normal: len=1 + UUID_SIZE
}

void send_master_claim(void)
{
	  //memcpy(my_aid_content, default_aid_content,1+UUID_SIZE);  // use updated my_aid_content (in ngmm_task_main)
    sendback(my_aid_content, 1+UUID_SIZE);
}

void aid_antiprocess(UInt8 id)
{
    UInt8 n;

    wr_idbit(id, 1);
    n =aid_lookup(id);
    if(n != 0xFF)
    {
        aid_db[n].status |= (1 << AID_STATUS_BIT_CONNECTED);
    }
}



// check if a buffer is binary zero
int is_zero(void *buf,unsigned size)
{
unsigned i;
    for(i=0;i<size;i++)
    {
        if(((UInt8 *)buf)[i] != 0)
            return FALSE;
    }
    return TRUE;
}


//////////////// delayed deleting units from database by aid-revoke /////////////////
Vrtc timed_delete_timer = 0;
unsigned char deleting_units = 0;  // bitmap of database slots to delete
static void timed_delete_timeout(void)
{
UInt8 buf[1+UUID_SIZE];
UInt8 u;
	
	for(u=1; deleting_units>>=1 ; u++)
	{
		if((deleting_units & 1) && !(aid_db[u].status & (1<<AID_STATUS_BIT_CONNECTED)))
		{
			// announce we are revoking this unit forever....
			memset(buf,0,sizeof(buf));
			nvfs_rd_at(unit_name(u),0,buf+1,UUID_SIZE);
			if(!is_zero(buf+1,UUID_SIZE))
			{
				buf[0]=0;
				proto_sendmsg(PROTO_AID,buf,1+UUID_SIZE); // announce
			}
			
			d2d2_func_delete_unit(u);  // delete all FUNC
			sensor_proto_delete_unit(u); // delete all SENS
			
			// delete from database
			memset(buf,0,sizeof(buf));
			
			nvfs_wr_at(unit_name(u),0,buf,UUID_SIZE);
			nvfs_wr_at(unit_name(u),UUID_SIZE,buf,KEY_SIZE);
			aid_db[u].status = 0;
			// aid_db[u].aid = 0;
		}
	}
}

void timed_delete_unit(UInt8 unit) // call to begin waiting for timed_delete operation
{
	deleting_units |= 1<<unit;
	vrtc_stop(timed_delete_timer);
	vrtc_update(timed_delete_timer,3*VRTC_1SEC);
	vrtc_start(timed_delete_timer);
}

void timed_delete_stop(UInt8 unit) // cancel any pending timed_delete for this unit
{
	deleting_units &= ~(1<<unit); // clear this unit's pending delete
	// no need to stop timer
}

void timed_delete_init(void) // called once from dhp_powerup()
{
  timed_delete_timer = vrtc_f_create((Exec_Func)timed_delete_timeout);
}


/* AID packet formats and meanings, grouped by dest value, with notes on expected response
--------------------------------------------------------------------------------------------------------------------
SENDER   AID     DEST + UUID |    Event
--------------------------------------------------------------------------------------------------------------------
  ss      04      00         |   SELF REVOKE WITHOUT UUID (only defined when original claim was without uuid)
                             |   if ss is a registered slave, then this packet is ignored by master
--------------------------------------------------------------------------------------------------------------------
  FE      04      00         |   AID REVOKE ALL (only master may issue this) All slaves should change id.
--------------------------------------------------------------------------------------------------------------------
  ss      04      00 + UUID  |   SELF REVOKE WITH UUID (normal self-revoke)
                             |   the named uuid IS revoking whatever id it had, even if ss is not correct.
                             |   connected slaves referenced by this packet should re-Claim asap. 
--------------------------------------------------------------------------------------------------------------------
  FE      04      00 + UUID  |   AID REVOKE  (only master may issue this) Slave MUST stop using old id.
                             |   Slave is permitted to try to Claim a different id than what it had before.
--------------------------------------------------------------------------------------------------------------------
  xx      04      yy         |   AID POLL (owner of yy should respond with Claim)
--------------------------------------------------------------------------------------------------------------------
  ss      04      ss         |   AID CLAIM WITHOUT UUID (intended only for slaves without UUID)
                             |   a registered slave previously owning id ss will be re-assigned another id by master
                             |   Note that master cannot resolve conflicts between unregistered stations
                             |   Even if they have different UUIDs.
--------------------------------------------------------------------------------------------------------------------
  xx      04      xx + UUID  |   AID CLAIM (normal claim)
                             |   any conflict will be resolved by master.
                             |   Note that master might not always resolve conflicts between unregistered stations
                             |   Even if they have different UUIDs.
--------------------------------------------------------------------------------------------------------------------
  ss      04      yy + UUID  |   Illegal assignment packet, ignored by master!
                             |   connected slaves referenced by this packet should re-Claim asap. 
--------------------------------------------------------------------------------------------------------------------
  FE      04      ss + UUID  |   AID ASSIGN (or Bounce) (only master may issue) Slave MUST change to this id ss.
--------------------------------------------------------------------------------------------------------------------
  xx      04      FF         |   AID SCAN  (all stations with and without UUIDs should respond with Claim)
--------------------------------------------------------------------------------------------------------------------
  xx      04      FF + UUID  |   AID SEARCH (station named by uuid should respond with Claim)
--------------------------------------------------------------------------------------------------------------------

xx, yy: are slave or master id
ss:     is slave only id

The remark "connected slaves referenced should re-Claim asap" is meant to protect against error or malice.
*/
#ifdef SIREN_TEST
extern void sirentest_claim(UInt8 *pkt,UInt8 len);
#endif

static struct uuid_record unit_record;
// process an AID packet
void aid_process(UInt8 *packet,UInt8 len)
{
UInt8 dest,src;
UInt8 x=0;
UInt8 u;
UInt8 n;    
UInt8 has_sensors = 0;
	
    if (len < 3)
        return; /// nothing to understand

    if(dhp_timer_var && dhp_timer_lessthan(100)) // are we scanning?
			 dhp_timer(100); // extend claim period
		
    // gather information from packet
    src = packet[0];
    dest = packet[2];
        
    n = aid_lookup(dest);

    // set u based on database
    u= 0xFF;        
    if(len == 3 + UUID_SIZE)
    {
#ifdef SIREN_TEST
			  if(memcmp(packet+3+8,"SIRN",4)==0)
				{
					sirentest_claim(packet,len);
					return;
				}
#endif
			  if(memcmp(packet+7,"DEI-IV",6)==0) // IVU or IVS?
					body_forbid(); // bodydance not allowed, use IVU button instead
			
        u = uuid_lookup(packet + 3);
        if(  u == 0xFF                      // not registered?
           && (aid_state > AID_STATE_CLOSED_WTMR || mainp_last_staus_get() == 0x81 )    // we are open to new registrants?
           && dest == src                   // sender sent a Claim packet?
           && (dest >= 3 && dest < 0xFB)    // chosen id is reasonable looking?
          )
        {// try to create a new entry before continuing...
            for(u=1;u<UNIT_MAX;u++)
            {
                if((aid_db[u].status & (1 << AID_STATUS_BIT_UUID)) == 0)
                {// found slot
                    memset(aid_db + u, 0, sizeof(aid_db[u])); // start clean
                    aid_db[u].status = (1 << AID_STATUS_BIT_UUID); // mark new entry
                    nvfs_wr_create(unit_name(u),packet+3,UUID_SIZE+KEY_SIZE); // write to database
                    dh_set(0);
                    nvfs_wr_at(unit_name(u), UUID_SIZE, dh_acc, KEY_SIZE);
                    nvfs_set_attributes(unit_name(u), NVFS_ATTRIB_HIDDEN);
                    have_devices = 1;
									  if(aid_state <= AID_STATE_CLOSED_WTMR)  //are we here only because of status 0x81?
											nvram_usr_commit();
                    break;
                }
            }
            if(u == UNIT_MAX) // database full?
                u = 0xFF; // sorry, cannot we register this.
        }
        
        if(u && u < UNIT_MAX) // registered device claim
        {
					has_sensors = packet[3+13]; // any sensors at all?
          aid_db[u].sensors = packet[3 + 13]>>4;  // any adjusrable sensors?
					if(aid_db[u].sensors )
					{
            my_appcap |= APPCAP_SENSADJ;						
					}

					if( memcmp(packet+3+8,"NGSS",4)==0)
					{
						set_temp_sensor_db_num(u);
						set_temp_chanl_num(3);  // TODO: fix it the "right way" later
					}
        }
    }				

    if(u == 0)  // master's uuid mentioned in any kind of packet?
    {
        send_master_claim(); // always re-Claim myself to be master!
        return;
    }
    
    // process AID packet based on dest...
    if(dest < UNIT_MAX || dest >= 0xFB)  // illegal dest for non-master?
    {       
        if ( (dest == AID_MASTER )        // master id is mentioned?
            || (dest == 0xFF && len == 3) // scan?
            )
        {
            if(dest == 0xFF) // scan?
            {// all devices (even my registered ones) should respond to this.
                memset(aid_idbit,0,sizeof(aid_idbit)); // clear all assignments (mostly unregistered)

                for(n=1;n<UNIT_MAX;n++)
                {
                    wr_idbit(aid_db[n].id, 1); // our registered slaves preserve their id and idbits...
                    aid_db[n].status &= ~(1 << AID_STATUS_BIT_CONNECTED); // but we mark as disconnected.
                }
                // whoever responds will be marked as connected and have their idbit set
            }
            // all the other reasons for master sending claim
            send_master_claim();
            return;
        }
        
        if(dest == 0xFF) // this is a search WITH UUID which was not ours (see above)
        {
            if(u != 0xFF) // we recognize this uuid?
            {
                aid_db[u].status &= ~(1 << AID_STATUS_BIT_CONNECTED); // we will reconnect if they answer
                // but don't touch their id or idbit
            }
            return; // we don't reply
        }


        if(dest == 0)   //revoke
        {
            n = aid_lookup(src); // lookup src id to see if it's one of ours

            if(len == 3) // revoke without uuid?
            {
                if(n != 0xFF)
								{// revoke from our registered device (it is now offline)
                  aid_db[n].id = 0;
                  aid_db[n].status &= ~(1 << AID_STATUS_BIT_CONNECTED);
									if (is_ccm_device(n))
									{
										sensor_proto_device_lost_force_notify(n);
									}
									else
									{
										sensor_proto_device_lost(n); // let sensor_proto decide whether to send alarm
									}
								}
                
                wr_idbit(src,0); // we believe it.
                return;
            }
            
            // revoke with uuid
            if(u == n) // this works for registered and unregistered devices
            {
                wr_idbit(src,0); // we believe the revoke.
                if(u!=0xFF)
                {// our registered uuid did a self-revokes...
                    aid_db[u].id = 0;
                    aid_db[u].status &= ~(1 << AID_STATUS_BIT_CONNECTED);
									if(len == 3+UUID_SIZE)
									  timed_delete_unit(u);
                }
                return;
            }
            
            // revoke problems...
                        
            if (u != 0xFF) // our registered uuid is being revoked but the sender id is wrong
            {// if not true, the device with that uuid will re-Claim right away
                wr_idbit(aid_db[u].id,0);  // meanwhile clear any id it was previously using
                aid_db[u].id = 0;  // for sure it has no id anymore
                aid_db[u].status &= ~(1 << AID_STATUS_BIT_CONNECTED); // and is not connected
							
// send a AID_SEARCH to give this unit a chance to respond
							  if(len == 3+UUID_SIZE)
								{
									packet[2] = 0xFF;
								  proto_sendmsg(PROTO_AID,packet+2,1+UUID_SIZE);
									timed_delete_unit(u);
								}
                return;         
            }
            
            // unregistered uuid is trying to revoke using a src id belonging to our registered user!
            return; // don't touch our registered user's id or idbit!
                    // if registered device wanted to self-revoke, they would use their correct uuid!
                    // if they are not revoking, they will re-claim and we will clear any idbits then.
            
        }
				else if(dest < UNIT_MAX)
				{// let's interpret this as a database query...
					  memset(packet+3,0,UUID_SIZE);
						len = nvfs_rd_at(unit_name(dest),0,packet+3,UUID_SIZE);
					  if(len && !is_zero(packet+3,UUID_SIZE)) // don't send empty slots
						{
							packet[2] = 0xFF;
							proto_sendmsg(PROTO_AID, packet+2,1+UUID_SIZE);
						}
						return;
				}
        
        // any other illegal dest id is either invalid Claim, or illegal assignment
        if(len > 3)
        {
            packet[2] = 0;
            sendback(packet+2,len-2);
        }
        return;
    }
    
    // dest IS a valid non-master id.
    if(len == 3) // no uuid?
    {
        if(dest == src)
        {// claim without uuid
            if(n != 0xFF)
            {// conflict with my registered slave!
                nvfs_rd_at(unit_name(n),0,packet+3,UUID_SIZE);
                dest = aid_replacement(src);
                aid_db[n].id = dest;
                packet[2] = dest;
                wr_idbit(dest,1);
                sendback(packet+2,1+UUID_SIZE); // re-assign my registered slave.
            }
            // conflict or not, there isn't enough here for us to attempt to resolve.
            wr_idbit(src,1);  // just accept this id is now in use.
        }
        else // someone is polling someone else
        {
            if(n != 0xFF) // are they polling our slave?
            {
                aid_db[n].status &= ~(1 << AID_STATUS_BIT_CONNECTED); // assume disconnected
                // but keep assigned id
                // as soon as they respond with ANYTHING, we will "re-connect" them.
                // this solves the "polling during DHP issue"
            }
            
            if(dest == 0xFF)
            {
                packet[2] = AID_MASTER;
                nvfs_rd_at(unit_name(0),0,packet+3,UUID_SIZE);
                sendback(packet+2,1+UUID_SIZE); // response to scan
            }
        }
        return;
    }
        
    // this is a packet WITH UUID
    if(dest != src) // illegal assignment packet!
    {
        return; // NOTE: illegal packet is "ignored by master"
                // and referenced slaves should re-Claim asap
    }
    
    // normal claim with uuid
		
    if(len == 3 + UUID_SIZE)
    {
			switch(is_ble_claiming(packet))
			{
				case 1: // built-in ble card
					ble_card_aid = packet[0];
					memcpy(ble_card_sn, packet + 3, 4);
					if(ble_was_detected != BLE_WAS_DETECTED)  // once per powerup
					{
						display_once(BLUE_LED, 1000);
						request_ble_info();  // we only request once per powerup
					}
					break;

#ifdef SKEY_TEST					
				case 2: // pluggable bluetooth ivu
					display_once(BLUE_LED, 250);   // every time
				  break;
#endif					
			}
    }

    if(u == 0xFF) // unregistered device trying to claim?
    {
        if(n != 0xFF)
        {// conflict with our registered device
            dest = aid_replacement(src); // assign new id for unregistered device
            wr_idbit(dest,1);  // reserve their bit
            packet[2] = dest;
            sendback(packet+2,len-2); // re-assign the sender
            
            aid_db[n].status &= ~(1 << AID_STATUS_BIT_CONNECTED);
            packet[2] = src;
            sendback(packet+2,1); // re-poll our device
            return;
        }

        // no conflict with registered devices...
        if(aid_state == AID_STATE_CLOSED_WTMR) 
        {// here it is worth trying to avoid conflicts
            if(rd_idbit(dest))
            {// we can try to re-assign since sender included a uuid
                dest = aid_replacement(dest);
                wr_idbit(dest, 1);
                packet[2] = dest;
                sendback(packet+2,len-2);
                return;
            }
            // dest bit is free
        }
        // outside of powerup and pairing, idbit checking won't help for unregistered
        // since unregistered might just be trying to re-Claim what it already has.
        wr_idbit(dest, 1); // just allow the Claim as is, and record the bit in use.            
        sendback(packet+2, len-2);  //bounce to sender
        return;
    }
    
		timed_delete_stop(u); // in case we were waiting to delete, now we won't delete...
		
    // registered device is trying to claim
    aid_db[u].status |= (1 << AID_STATUS_BIT_CONNECTED); // at least it's connected!
		if(  (aid_db[u].status & (1 << AID_STATUS_BIT_UUID)) )
		{
			if( device_verif_state > DEVICE_VERIF_DONE &&
				  (memcmp(packet+3, device_in_check_uuid, UUID_SIZE) == 0))     //verify device from 0x82
			{
				device_verif[device_search_cur] = 1;    //verified
			}
			
			if(aid_scan_in_progress && (aid_db[u].status & (1 << AID_STATUS_BIT_UUID)))
				re_searching_units &= ~(1<<u);   // no need re-search
		}
    
    if(dest == aid_db[u].id)
    {// claiming what they already have
        check_offer_pair(u,has_sensors);
        return;
    }
    
    // trying to change id to something else
    if(rd_idbit(dest)) // is id in use by anyone else?
    {// conflict with registered OR unregistered
        dest = aid_replacement(src); // choose something better for claimant
        wr_idbit(dest,1);
        aid_db[u].id = dest;
        packet[2] = dest;
        sendback(packet+2,len-2); // re-assign claimant
        
        check_offer_pair(u,has_sensors);
        
        n = aid_lookup(src);
        if(n != 0xFF)
            aid_db[n].status &= ~(1 << AID_STATUS_BIT_CONNECTED); // if it doesn't answer...

        packet[2] = src;
        sendback(packet+2,1); // poll the conflicted id (registered or not)
        return;
    }
    
    // claim with no conflict
    wr_idbit(aid_db[u].id, 0); // clear the old id (if any)
    aid_db[u].id = src; // now assign the new
    wr_idbit(src, 1); // mark new id as in use
    check_offer_pair(u,has_sensors);
}

void aid_search(UInt8 x)
{
	UInt8 msg[32];
	UInt8 sum_aid_w_key=0;
	
	msg[0] = 0xFF;

	nvfs_rd_at(unit_name(x), 0, msg + 1, UUID_SIZE); 
	sendback(msg, 1 + UUID_SIZE);
	re_searching_units &= ~(1<<x);  // mark as done
}

int is_acceptable_random(UInt8 *ptr)
{
unsigned msw;
unsigned weight,i;

    weight = 0;
    
    msw = (ptr[0] << 8) | ptr[1];

    if (msw == 0xFFFF || msw == 0)
        return 0;

    weight = 0;
    for (i = 0; i < 16; i++)
        weight += ham8[ptr[i]];

    if (weight < 24 || weight > 128 - 24)
        return 0;
    
    return 1;
}

static Boolean is_ccm_device(UInt8 n)
{
	Boolean ret_val = FALSE;
	UInt8 buf[UUID_SIZE];

	if (nvfs_rd_at(unit_name(n), 0, buf, UUID_SIZE) == UUID_SIZE
			&& !is_zero(buf, UUID_SIZE)
			)
	{
		if (memcmp(buf+4,"DEI-CCM ",8)==0)
		{
			ret_val = TRUE;
		}
	}	
  return ret_val;	
}

// process a DHP packet
void dhp_process(UInt8 *packet, UInt8 len)
{
UInt8 i;
static DH_VAR dh_temp;

    if (len != 3 + KEY_SIZE
        || aid_state == AID_STATE_CLOSED
        || packet[2] != AID_MASTER  // id is not sender?
        || !IS_SLAVE_ID(packet[0])
        || (i = aid_lookup(packet[0])) == 0xFF  // unknown id?
        || (aid_db[i].status & (1 << AID_STATUS_BIT_UUID)) == 0 // unknown uuid?
        )
        return; // I can't do anything with this.

    // compute session key
    if (!is_acceptable_random(packet + 3))
    {
        proto_sendmsg(PROTO_DHP, packet + 2, 1);  // invite to try again...
			  save_normal_fail_dbg_msg(packet[2]);  // save pairing counter
        return;
    }
    for (;;)
    {
        dh_random();
        if (is_acceptable_random((UInt8 *)dh_acc))
            break;
    }
    
    mov128(dh_temp,dh_acc);  // secret exponent
    
    dh_load_nbo(packet + 3);
    dh_exp(dh_temp);    // compute session key
    dh_store_nbo(packet + 3);
    nvfs_wr_at(unit_name(i), UUID_SIZE, packet + 3, KEY_SIZE); // save secret session key into nvfs IN HOST128 FORMAT
                                                               //NBO
    aid_db[i].status |= (1 << AID_STATUS_BIT_KEY); // remember we have a key now

    dh_set(dh_G);
    dh_exp(dh_temp);
    dh_store_nbo(packet+3);
    
    packet[2] = packet[0];
    proto_sendmsg(PROTO_DHP,packet+2,len-2);
    dhp_timer(3000);    //prolonge auth reset to avoid collision with SCPP feedback
}



void device_verify_process(void)
{
	UInt8 msg[32];
	UInt8 i;

	switch(device_verif_state)
	{
		case DEVICE_VERIF_SEARCHING:
		  device_search_cur = device_search_idx;
			msg[0] = 0xFF;
			nvfs_rd_at(unit_name(device_search_cur), 0, msg + 1, UUID_SIZE); 
		  nvfs_rd_at(unit_name(device_search_cur), 0, device_in_check_uuid, UUID_SIZE); 
		  if(aid_db[device_search_cur].id && device_search_cur < UNIT_MAX)   //not empty
			  sendback(msg, 1 + UUID_SIZE);
			if(device_search_idx++ >= UNIT_MAX)
			{
				device_verif_state = DEVICE_VERIF_DBLCHECKING;
				device_search_idx = 1;
			}
			dhp_timer(200);
			break;
		case DEVICE_VERIF_DBLCHECKING:
			device_search_cur = device_search_idx;
			msg[0] = 0xFF;
		  if(device_verif[device_search_cur] == 0)  //try another time
			{
				nvfs_rd_at(unit_name(device_search_cur), 0, msg + 1, UUID_SIZE); 
				nvfs_rd_at(unit_name(device_search_cur), 0, device_in_check_uuid, UUID_SIZE); 
				if(aid_db[device_search_cur].id && device_search_cur < UNIT_MAX)   //not empty
					sendback(msg, 1 + UUID_SIZE);
			}
			if(device_search_idx++ >= UNIT_MAX)
			{
				device_verif_state = DEVICE_VERIF_DELETING;
				device_search_idx = 1;
			}
			dhp_timer(200);
			
			break;
		case DEVICE_VERIF_DELETING:
			for(i=1; i<UNIT_MAX; i++)  //not 
		  {
				if(device_verif[i] == 0)
				{
					aid_delete(i);
					d2d2_func_delete_unit(i);  // delete all FUNC
			    sensor_proto_delete_unit(i); // delete all SENS
				}
		  }
			device_verif_state = DEVICE_VERIF_DONE;
			dhp_timer(100);
			break;
		default:
			break;
	}
}

void finish_scan(void)
{
  finish_ble_info_scan();
		
	aid_scan_in_progress = 0;
	master_appcap(); 

	//if ((mainp_last_staus_get() & 0x81) != 0x81)  // DO NOT reset if it was in pairing mode already
	{
		master_auth_reset(); // just send reset and let others reset if they need
		aid_state = AID_STATE_CLOSED;
	}

	// polling the capability of temperature sense in the end of 933 core initialization
	tempsens_cap_polling();	
}


UInt8 dbg_key[KEY_SIZE];
void master_auth_reset(void); // forward reference
Per_Exec_Wake dhp_timeout(void)
{
	  UInt8 idx, cur;
	
    if(dhp_timer_var == 0)
		{
			dhp_timer_running = 0;
			return PER_EXEC_SLEEP;
		}

    if(--dhp_timer_var != 0)
       return PER_EXEC_WAKE;      

		if(ble_card_aid && (ble_was_detected != BLE_WAS_DETECTED))  //blue LED just on, don't send reset yet
		{
			if(aid_state == AID_STATE_CLOSED_WTMR && have_wireless==0 && aid_scan_in_progress)
			{// shorten the delay before reset when there are no wireless devices			
				finish_scan();
			}

			ble_was_detected = BLE_WAS_DETECTED;
			dhp_timer(1000);      //wait for blue LED to turn off
			return PER_EXEC_WAKE;
		}

    if(device_verif_state != DEVICE_VERIF_DONE)
		{
			device_verify_process();
			return PER_EXEC_WAKE;
		}
			
    if(aid_state == AID_STATE_CLOSED_WTMR || aid_state == AID_STATE_CLOSED) 
    {
        if(have_devices == 0 && !system_in_used_status_get())
				{
           mainp_status(0x80 | MAINP_SCANNING);
				}
        else
				{
					  /* // TODO
					  if(driving)
					    mainp_status(0x0D);
					  else
					  */
						if ((mainp_last_staus_get() & 0x81) != 0x81)   // DO NOT stop the pairing
						{
					    mainp_status(get_ss_arm() ? MAINP_ARMED : MAINP_DISARMED);
						}
				}
    }
    else
    {
        if(aid_state == AID_STATE_SCANNING)
        {
            aid_state = AID_STATE_ADDING;
            dhp_sendparams();
            dhp_timer(4000);
            return PER_EXEC_WAKE;
        }
        //end of AID_STATE_ADDING
        if(aid_state > AID_STATE_CLOSED_WTMR &&
					 aid_state < AID_STATE_CLOSED_NEAR)
        {
          nvram_usr_commit();
        }

        if(have_devices)
        {
          if(aid_state != AID_STATE_ADDING)  //not pairing modeS
            mainp_status(0x00);
        }
        
				if(aid_state != AID_STATE_CLOSED_NEAR)  //change later
          aid_state = AID_STATE_CLOSED; // at end of timeout, always goes back to closed.
				
    }
    
		if(!have_devices &&
			(aid_state == AID_STATE_CLOSED_NEAR && owe_mainp_scanning)
			)
		{
				owe_mainp_scanning = 0;
				mainp_status(MAINP_SCANNING);
				change_mainp_state(MAINP_SCANNING);
				rewind_scanning_timer(); 
			  aid_state = AID_STATE_CLOSED; // at end of timeout, always goes back to closed.
			  return PER_EXEC_WAKE;
		}

		if(!re_searching_units)
		{
			if(aid_scan_in_progress)
			{
				finish_scan();
			}
		}
		else
		{
			if(aid_scan_in_progress)
			{
				for(idx=1; idx < UNIT_MAX; idx++)
				{
				 cur = (re_searching_units & (1<<idx));
					if(cur)
						break;
				}
				if(cur)
				  aid_search(idx);
				dhp_timer(200); //wait for slave claim
			}
		}
		
		if(owe_mainp_scanning  && !have_devices && (aid_state == AID_STATE_CLOSED_WTMR))
		{
			dhp_timer(200);
			aid_state = AID_STATE_CLOSED_NEAR;
		}

    return PER_EXEC_WAKE;
}


void dhp_sendparams(void)
{
  proto_sendmsg(PROTO_DHP,dhp_constants,sizeof(dhp_constants));
}

static UInt8 seed[16];
#define randkey (&my_aid_content[1])
// choose a random exponent
void dh_random(void)
{
//  memxor(seed,timestamp_variable,sizeof(timestamp_variable));
    aes_encrypt(randkey,seed, seed);
    
    dh_load_nbo(seed);
    dh_acc[DH_VAR_MSW] &= ~(3U<<30);  // clear 2 msbits to be less than modulus P
}

// API to begin scanning/pairing ----

void master_auth_init(void); // forward reference
// erase all units. start over
void dhp_eraseall(void)
{
UInt8 i;
	
	  mainp_status(0x82);
	
    have_devices = 0;
  
    dh_random();

    master_auth_init();
	
		aid_scan_in_progress = 1;
	
	  aid_state = AID_STATE_CLOSED_WTMR;
	  
		memset(device_verif, 0, sizeof(device_verif));
		device_search_idx = 1;
	  device_verif_state = DEVICE_VERIF_SEARCHING;
	
    dhp_timer(200);
}

void dhp_pairmode(void)
{
UInt8 i;
  
    mainp_status(0x81);  //installer allowed, pairing mode
  
    dh_random();
    master_auth_init();

    for (i = 1; i < UNIT_MAX; i++)
        aid_db[i].status &= ~((1 << AID_STATUS_BIT_CONNECTED) | (1<<AID_STATUS_BIT_OFFER));

  
	  send_master_claim();  // remind everyone who I am...
	
    aid_state = AID_STATE_SCANNING;
		aid_scan_in_progress = 1;
    ble_card_aid = 0;
  	i = 0xFF;
    proto_sendmsg(PROTO_AID, &i, 1);  // send AID_SCAN and see who replies
    
    dhp_timer(2000); // give time for 8051 and KIB to cook up some new pairdata
}



/**
* @brief  update aid content with nvfs id
* @param
* @return
*/
void update_aid_content(void)
{
      UInt8 i;
    UInt8 id[19];
    UInt32 n;

    memcpy(my_aid_content, default_aid_content, sizeof(my_aid_content));

    nvfs_rd_at( nvfs_id ,0, &id, sizeof(id)); 

      n = 0;
      for(i=12;i<(12+7);i++)  // starting from 
      {
            n *= 10;
            n += id[i]-'0';
      }
        
    n = htonl(n); // network byte order
    memcpy(my_aid_content+1,&n, sizeof(n));
}

// called at powerup only
void dhp_powerup(void)
{
UInt8 buf[KEY_SIZE];
UInt8 i;
	
	  timed_delete_init();
		

    per_exec_register((Per_Exec_Func)dhp_timeout);
    
    aid_exception_count = 0;
    aid_except_vrtc = vrtc_f_create((Exec_Func)aid_except_vrtc_timetout);
    
    memset(&aid_idbit, 0, sizeof(aid_idbit));   //initialize aid bitmap

//    update_aid_content();


    // be sure some master record exists
    if(nvfs_rd_at(unit_name(0),0,&unit_record,sizeof(unit_record)) != sizeof(unit_record))
    {
        memset(&unit_record,0,sizeof(unit_record)); // create blank record
        nvfs_wr_create(unit_name(0),&unit_record,sizeof(unit_record));
        nvfs_wr_at(unit_name(0), 0, my_aid_content + 1, UUID_SIZE);
    }
    
		owe_mainp_scanning = 0;
    have_devices = 0;
		aid_scan_in_progress = 1;
		re_searching_units = 0;
		have_wireless = 0;
    for (i = 0; i < UNIT_MAX; i++)
    {
        memset(&aid_db[i], 0, sizeof(struct aid_entry));
        if (nvfs_rd_at(unit_name(i), 0, buf, UUID_SIZE) == UUID_SIZE
            && !is_zero(buf, UUID_SIZE)
            )
        {           
            if(i)
              have_devices = 1;
            aid_db[i].status = (1 << AID_STATUS_BIT_UUID); // we have a uuid for this unit

						if(memcmp(buf+4,"DEI-",4)==0)    // if this is a DEI- UUID...
							aid_db[i].sensors = buf[13]>>4;  // then this is the count of adjustable sensors
						
						// KLUDGE: check for wireless devices.
						if(   memcmp(buf+4,"DEI-SIRN",8)==0
							 || memcmp(buf+4,"DEI-CCM ",8)==0
					     || memcmp(buf+4,"DEI-WSS ",8)==0
							)
						  have_wireless++; // count another wireless device
						
            if (nvfs_rd_at(unit_name(i), UUID_SIZE, buf, KEY_SIZE) == KEY_SIZE
                && !is_zero(buf, KEY_SIZE)
                )
            {
                aid_db[i].status |= (1 << AID_STATUS_BIT_KEY);
            }
						if(i)
						{    //don't count DS4 itself
							re_searching_units |= (1<<i);
						}

            nvfs_set_attributes(unit_name(0), NVFS_ATTRIB_HIDDEN);
        }
    }

		if(!have_devices)
		{
			nvfs_rd_at(unit_name(0),UUID_SIZE,seed,KEY_SIZE);   //current broadcastkey as seed for next
			dh_random();
			nvfs_wr_at(unit_name(0),UUID_SIZE,seed,KEY_SIZE);
		}
    master_auth_init(); // be sure we have a broadcast key
		
    aid_db[0].id = AID_MASTER;

    aid_state = AID_STATE_CLOSED_WTMR;

    ble_card_aid = 0;
    i = 0xFF;
		if(have_devices)
		{	
		  mainp_status(MAINP_SCANNING);
      dhp_timer(have_wireless ? 4100 : 4100); // for now, use same value
		}
		else
		{
			// TODO: test if we are driving
			/* if(driving)
			     mainp_status(0x0D);
			   else
			*/
			mainp_status(get_ss_arm() ? 0x06 : 0x80);
			owe_mainp_scanning = 1;
			dhp_timer(100);
		}
		send_master_claim(); // show myself before asking others
    proto_sendmsg(PROTO_AID, &i, 1);  // send AID_SCAN and see who replies
		
		device_verif_state = DEVICE_VERIF_DONE;
		memset(device_verif, 0, sizeof(device_verif));
		device_search_idx = 1;   // no need to search myself
}


//aid_except_vrtc timeout, per minute
void aid_except_vrtc_timetout(void)
{
    if(aid_exception_count && --aid_exception_count)
    {
        vrtc_stop(aid_except_vrtc);
        vrtc_update(aid_except_vrtc, AID_EXCEPTION_VRTC_DELTA);
        vrtc_start(aid_except_vrtc);
    }
}


/////////////////////////////////// auth ////////////////////////////////////////////


// this is the basic auth computation which is used when generating AND when verifying...
// return the implied timestamp (in host format) of this packet, using the presumed prev_auth (in network byte order) 
UInt32 auth_compute(UInt8 *key,const UInt8 *pkt, unsigned len, const UInt8 *prev_auth)
{
#define BUFSIZE KEY_SIZE
static UInt32 buf_align32[BUFSIZE/sizeof(UInt32)];
#define buf ((UInt8 *)buf_align32)

unsigned plen,ofs,amt;

  plen = len - AUTH_SIZE; // size of data without auth bytes

  memset(buf,0,BUFSIZE);
  memcpy(buf,prev_auth,AUTH_SIZE); // pre-fill using the prev_auth contents

  ofs = AUTH_SIZE; // current packet data starts here
  
  while(plen)
  {
    amt = BUFSIZE - ofs;  // how much room left in buffer?
    if(plen < amt)
      amt = plen;  // only use rest of message

    memxor(buf+ofs,pkt,amt);
    
    aes_encrypt(key,buf,buf);  // encrypt in place

    ofs += amt;
    if(ofs >= BUFSIZE)
      ofs = 0;

    pkt += amt;
    plen -= amt;
  }

  // fold in half, twice
  memxor(buf,buf+2*AUTH_SIZE,2*AUTH_SIZE);
  memxor(buf,buf+AUTH_SIZE,AUTH_SIZE);


  memxor(buf,pkt+plen,AUTH_SIZE); // xor with the auth bytes from the packet (in network byte order)
  return ntohl(buf_align32[0]); // return the implied timestamp in host format for easy computation

#undef buf
#undef BUFSIZE
}



//#define DBGFAIL
#ifdef DBGFAIL
unsigned dbgfail[4];
#define DBGFAIL_THIS_IDX 1
#define DBGFAIL_OTHER_IDX 2
#define DBGFAIL_TIME_IDX  3
#endif



void auth_generate(UInt8 *key,UInt8 *pkt, unsigned len,UInt8 *prev_auth)
{
UInt32 ts;
unsigned plen;
unsigned tnow;
    
  plen = len - AUTH_SIZE;

  memset(pkt + plen,0,AUTH_SIZE); // clear the "received" syndrome.

  ts = auth_compute(key,pkt,len,prev_auth); // the returned "implied timestamp" is actually the syndrome in host order
	tnow = time(NULL);
  ts ^= tnow; // exclusive-or with current time in host order

#ifdef DBGFAIL
// DEBUG only
	if(dbgfail[DBGFAIL_OTHER_IDX])
	{
		dbgfail[DBGFAIL_TIME_IDX] = ntohl(tnow); // convert to nbo
		//dbgfail[0]= 0xEEEEEEEE;
		// hide details  (non-revealing debug!!!)
		//dbgfail[1]= 0x55555555;
		//dbgfail[2]= 0x55555555;
		//dbgfail[3]= 0x55555555;
		proto_sendmsg(PROTO_DBG,3+(UInt8 *)dbgfail,13); // send debug packet to 0xEE
	}
// end of DEBUG
#endif
	
  ts = ntohl(ts); // convert to network byte order
  memcpy(pkt + plen,&ts,AUTH_SIZE); // write syndrome ^ timestamp into packet
  memcpy(prev_auth,&ts,AUTH_SIZE);  // save copy for authenticating next packet
}







///////////////// this is needed only if we want to verify the auth bytes in a packet //////////////////////////

UInt32 curtime;

#define ACCEPTANCE_WINDOW (4) // seconds
int within_window(UInt32 delta)
{
  if(  delta <= ACCEPTANCE_WINDOW
        || delta >= (UInt32)(-ACCEPTANCE_WINDOW)
        )
	{
#ifdef DBGFAIL		
		memset(dbgfail,0,sizeof(dbgfail));
#endif		
	  return 1;
	}
#ifdef DBGFAIL	
	dbgfail[DBGFAIL_THIS_IDX] = dbgfail[DBGFAIL_OTHER_IDX];
	dbgfail[DBGFAIL_OTHER_IDX] = htonl(delta + curtime);
#endif	
	return 0;
}

// NCC-DIRE002-001 (versus conceptual timing side-channel attack)
// timing-invariant replacement for memcmp.  return 0 only if memory blocks are the same
int memcmp_TI(const void *a,const void *b,unsigned len)
{
unsigned result = 0;
const char *ma = a;
const char *mb = b;
	
	while(len--)
		result |= *ma++ ^ *mb++;
		
	return result; // not a "true" memcmp result because we don't solve the inequality direction
}

UInt8 broadcastkey[KEY_SIZE];
static UInt8 authkey[KEY_SIZE];
// verify this packet which was received very recently (ie "now")
int auth_verify(UInt8 *pkt,unsigned len)
{
UInt32 pkttime;
unsigned i;
    
  i = aid_lookup(pkt[0]); // which unit is this?

  if(i>0 && i<UNIT_MAX)
  {
        if(aid_db[i].status & (1<<AID_STATUS_BIT_KEY))
        {

            // we have a key

            curtime = time(NULL); // host order current time

            nvfs_rd_at(unit_name(i),UUID_SIZE,authkey,KEY_SIZE);  //

            // try both ways...
            if(  within_window( auth_compute(authkey,pkt,len,aid_db[i].prev_auth) - curtime )
                || within_window( auth_compute(authkey,pkt,len,aid_db[i].other_auth) - curtime )
                )
            {
							  aid_db[i].status &= ~(AID_STATUS_FAILMASK<<AID_STATUS_BIT_FAILCOUNT); // device is healthy (set failcount = 0)
                memcpy(aid_db[i].prev_auth, pkt+len-AUTH_SIZE,AUTH_SIZE); // save successful auth for next time
                memcpy(aid_db[i].other_auth, pkt+len-AUTH_SIZE,AUTH_SIZE); // there IS no "other auth" after success
                return TRUE;
            }

            // auth failed, but save this auth in case the real unit tries to recover on next packet
            memcpy(aid_db[i].other_auth, pkt+len-AUTH_SIZE,AUTH_SIZE); // save successful auth for next time        
            if(pkt[1] == PROTO_FAIL + PROTO_AUTH && pkt[0] == pkt[2])
            {
                aes_decrypt(authkey, pkt+3, pkt+3);
                if(memcmp_TI(authkey, pkt+3, KEY_SIZE) != 0)
                {
#if 1 // show that we are deleting the key for this unit
									  pkt[2] = 0;
									  memcpy(pkt + 3, authkey, KEY_SIZE);
									  proto_sendmsg(PROTO_DBG, pkt + 2, 1 + KEY_SIZE);  // debug send previous bad key.
#endif
									//lockout this unit. This unit must re-pair to main
                    aid_db[i].status &= ~(1<<AID_STATUS_BIT_KEY);
                    memset(pkt+3, 0, KEY_SIZE); // at next pairing opportunity, we will send the short DHP invite
                    nvfs_wr_at(unit_name(i), UUID_SIZE, pkt+3, KEY_SIZE);
                }
            }
      }
      else // no key for this unit
        memcpy(authkey,broadcastkey,KEY_SIZE); // important side effect, one of our caller needs SOME real key

      failing_units |= (1<<i);
  }
  return FALSE;   
}

// process FAIL packets
void master_fail_process(UInt8 *pkt,UInt8 len)
{
#if 0	
UInt8 i;
UInt8 buf[KEY_SIZE];
    
  i = aid_lookup(pkt[0]);
  if(  i>=1 && i<UNIT_MAX
    && pkt[0]==pkt[2]   // this is an AUTH-RESET packet
    && (aid_db[i].status & (1<<AID_STATUS_BIT_KEY))
    && (pkt[1] & PROTO_AUTH)
    && len == 3+KEY_SIZE+AUTH_SIZE
    )
  {// AUTH_RESET from unit i, which PASSED authentication!
    memcpy(buf,pkt+3,KEY_SIZE);
    aes_decrypt(authkey,buf,buf); // decrypt in place
    if(memcmp_TI(buf,authkey,KEY_SIZE)==0)
      proto_sendmsg(PROTO_FAIL + PROTO_AUTH, &pkt[2], 1); // short reply to AUTH_RESET means success.
  }
#endif // why should we send anything when auth was passed?
}



// master knows nothing except who has session keys
void master_auth_init(void)
{ 
    // set P
    dh_load_nbo(dh_P);
    dh_setP();
    // read key
  nvfs_rd_at(unit_name(0),UUID_SIZE,broadcastkey,KEY_SIZE); // success or not, doesn't matter

  if(!is_acceptable_random(broadcastkey))
  {     
        dh_load_nbo(broadcastkey);
        while(!is_acceptable_random((UInt8 *)dh_acc))
        {
            dh_random();
        }
        dh_store_nbo(broadcastkey);
        nvfs_wr_at(unit_name(0),UUID_SIZE,broadcastkey,KEY_SIZE);
  }

	sys_in_low_voltage_mode = 0;
}


UInt8 dbg_buf[KEY_SIZE];
// send AUTH-RESET
void master_auth_reset(void)
{
//    UInt8 dbg_key[KEY_SIZE] =
//{ 0x4F, 0x23, 0xEA, 0xEA,
//    0xF3, 0xFA, 0xF4, 0x5B,
//    0xCD, 0x54, 0xA5, 0x45,
//    0x86, 0x48, 0x40, 0x26
//};

UInt8 buf[1+KEY_SIZE+AUTH_SIZE];
UInt8 i;
    
//    memcpy(dbg_buf, dbg_key, KEY_SIZE); 
//    aes_encrypt(dbg_key, dbg_buf, dbg_buf); 
	  for(i = 1; i < UNIT_MAX; i++)
	  {
	     memset(aid_db[i].prev_auth, 0, AUTH_SIZE);    //clear all the previous auth bytes because the global RESET
	     memset(aid_db[i].other_auth, 0, AUTH_SIZE);    //clear all the previous auth bytes because the global RESET
			 aid_db[i].status &= ~(1<<AID_STATUS_BIT_FAILCOUNT); // clear lsbit of FAIL counter
		}
	
    memset(buf,0,sizeof(buf));
    memcpy(buf+1,broadcastkey,KEY_SIZE);
    aes_decrypt(broadcastkey,buf+1,buf+1);
    buf[0] = AID_MASTER;                   //global RESET
  memset(master_prev_auth, 0, AUTH_SIZE);
  proto_sendmsg(PROTO_FAIL + PROTO_AUTH,buf,sizeof(buf));
}

static void master_appcap(void)
{ //today only have 1 protocol: suppercode++
  UInt8 buf[4];
  buf[0] = PROTO_SCPP;
  buf[1] = 2;
  buf[2] = my_appcap>>8;
  buf[3] = my_appcap;
  proto_sendmsg(PROTO_APPCAP, buf, sizeof(buf));
}
