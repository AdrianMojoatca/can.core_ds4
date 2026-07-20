#include "aid_dhp.h"
#include "intl_port.h"
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
#include <string.h>


unsigned dhp_timer_var;

UInt8 ble_card_present=0;

#define DHP_HZ  100  //10ms timebase

void dhp_timer(UInt16 ms)
{     
      dhp_timer_var = (ms * DHP_HZ + 999)/1000;
}

// return true if there is less than argument milliseconds remaining on the dhp_timer
int dhp_timer_lessthan(UInt16 ms)
{
	return 1000*dhp_timer_var < DHP_HZ * (unsigned int)ms;
}

void dhp_timer_stop(void)
{
    dhp_timer_var = 0;
}


#define  AID_EXCEPTION_VRTC_DELTA (60*VRTC_1SEC)
#define  AID_EXCEPTION_LIMIT      3

UInt32 aid_idbit[256/32];  //aligned-32 for faster access
UInt8  aid_exception_count;
UInt8 have_devices=0;

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

void check_offer_pair(UInt8 i)
{
    if (  aid_state <= AID_STATE_CLOSED_WTMR // not when closed
       || i >= UNIT_MAX
       || (aid_db[i].status & ((1<<AID_STATUS_BIT_OFFER))) != 0
       || !IS_SLAVE_ID(aid_db[i].id)
       || (aid_db[i].status & (1<<AID_STATUS_BIT_KEY)) != 0
       )
        return;

    // invite them to try to pair
    aid_db[i].status |= (1<<AID_STATUS_BIT_OFFER);
    dhp_timer(100);
    proto_sendmsg(PROTO_DHP, &aid_db[i].id, 1); // mention we don't have a session key
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
       if( (aid_db[i].id == tempsens_aid(3) && (tempsens_config == 3))  //IVU
        || (aid_db[i].id == tempsens_aid(2) && (tempsens_config == 2)) )  //NGSS
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

UInt8 is_ble_claiming(UInt8 *ptr)
{
UInt8 ret=0;
static UInt8 *uname =
{
    "DEI-BLE "
};

    if(memcmp(ptr+7, uname, 8) == 0)
        ret = 1;

    return ret;
}

// master send this full AID packet
void sendback(UInt8 *ptr, UInt8 len)
{
    proto_sendmsg(PROTO_AID, ptr, len);  //normal: len=1 + UUID_SIZE
}

void send_mine(void)
{
    memcpy(my_aid_content, default_aid_content,1+UUID_SIZE);
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

static struct uuid_record unit_record;
// process an AID packet
void aid_process(UInt8 *packet,UInt8 len)
{
UInt8 dest,src;
UInt8 x=0;
UInt8 u;
UInt8 n;    
    
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
        u = uuid_lookup(packet + 3);
        if(  u == 0xFF                      // not registered?
           && aid_state > AID_STATE_CLOSED_WTMR       // we are open to new registrants?
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
                    aid_db[u].sensors = packet[3 + 13]>>4;
                    nvfs_wr_create(unit_name(u),packet+3,UUID_SIZE+KEY_SIZE); // write to database
                    dh_set(0);
                    nvfs_wr_at(unit_name(u), UUID_SIZE, dh_acc, KEY_SIZE);
                    nvfs_set_attributes(unit_name(u), NVFS_ATTRIB_HIDDEN);
                    have_devices = 1;
                    break;
                }
            }
            if(u == UNIT_MAX) // database full?
                u = 0xFF; // sorry, cannot we register this.
        }
    }

    if(u == 0)  // master's uuid mentioned in any kind of packet?
    {
        send_mine(); // always re-Claim myself to be master!
        return;
    }
    
    // process AID packet based on dest...
    if(dest < 3 || dest >= 0xFB)  // illegal dest for non-master?
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
            send_mine();
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
                  return;  // a real revoke by our registered station should be WITH uuid
                           // so don't clear the bit
                
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
                }
                return;
            }
            
            // revoke problems...
                        
            if (u != 0xFF) // our registered uuid is trying to self-revoke but the id is wrong
            {// if not true, it will re-Claim right away
                wr_idbit(aid_db[u].id,0);  // clear any id it was previously using
                aid_db[u].id = 0;  // for sure it has no id anymore
                aid_db[u].status &= ~(1 << AID_STATUS_BIT_CONNECTED); // and is not connected
                return;         
            }
            
            // unregistered uuid is trying to revoke using a src id belonging to our registered user!
            return; // don't touch our registered user's id or idbit!
                    // if registered device wanted to self-revoke, they would use their correct uuid!
                    // if they are not revoking, they will re-claim and we will clear any idbits then.
            
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
    if((len == 3 + UUID_SIZE) && is_ble_claiming(packet))
    {
      ble_card_present = 1;
      if(fw_init_is_done)
        display_flash(BLUE_LED, 2, BLUE_LED_FLASH_ACK_ON, BLUE_LED_FLASH_ACK_OFF); //BLE is herer: flash blue fast twice
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
    
    // registered device is trying to claim
    aid_db[u].status |= (1 << AID_STATUS_BIT_CONNECTED); // at least it's connected!
    
    if(dest == aid_db[u].id)
    {// claiming what they already have
        check_offer_pair(u);
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
        
        check_offer_pair(u);
        
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
    check_offer_pair(u);
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
        weight += ham8[i];

    if (weight < 24 || weight > 128 - 24)
        return 0;
    
    return 1;
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


UInt8 dbg_key[KEY_SIZE];
void master_auth_reset(void); // forward reference
Per_Exec_Wake dhp_timeout(void)
{

    if(dhp_timer_var == 0)
			return PER_EXEC_SLEEP;

    if(--dhp_timer_var != 0)
       return PER_EXEC_WAKE;      
    
    if(aid_state == AID_STATE_CLOSED_WTMR || aid_state == AID_STATE_CLOSED) 
        {
            if(have_devices == 0)
            {
                mainp_status(0x80);
            }
        }
        else
    {
        if(aid_state == AID_STATE_SCANNING)
        {
            aid_state = AID_STATE_ADDING;
            dhp_sendparams();
            dhp_timer(3000);
            return PER_EXEC_WAKE;
        }
        //end of AID_STATE_ADDING
                if(aid_state > AID_STATE_CLOSED_WTMR)
                {
                  nvram_usr_commit();
                }
                
        aid_state = AID_STATE_CLOSED; // at end of timeout, always goes back to closed.
                mainp_status(have_devices ? 0x00 : 0x80);
    }
    
    master_auth_reset(); // just send reset and let others reset if they need
        
        return PER_EXEC_WAKE;
}


void dhp_sendparams(void)
{
  proto_sendmsg(PROTO_DHP,dhp_constants,sizeof(dhp_constants));
}

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
    have_devices = 0;
    
    dh_random();

    for (i = 0; i < UNIT_MAX; i++)
        aid_delete(i); // really forget everything

    master_auth_init();
    aid_state = AID_STATE_SCANNING;

    i = 0;
    proto_sendmsg(PROTO_AID, &i, 1);  // send AID_CLEARALL
    dhp_timer(100);
}

void dhp_pairmode(void)
{
UInt8 i;
    
    dh_random();
    master_auth_init();

    for (i = 1; i < UNIT_MAX; i++)
        aid_db[i].status &= ~((1 << AID_STATUS_BIT_CONNECTED) | (1<<AID_STATUS_BIT_OFFER));

  
    aid_state = AID_STATE_SCANNING;

    i = 0xFF;
    proto_sendmsg(PROTO_AID, &i, 1);  // send AID_SCAN and see who replies
    
    dhp_timer(100);
}



/**
* @brief  update aid content with nvfs id
* @param
* @return
*/
void update_aid_content(void)
{
      UInt8 i;
    UInt8 id[6];
    UInt32 n;

    memcpy(my_aid_content, default_aid_content, sizeof(my_aid_content));

    nvfs_rd_at( nvfs_id ,0, &id,6 ); 

      n = 0;
      for(i=0;i<6;i++)
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

    per_exec_register((Per_Exec_Func)dhp_timeout);
    
    aid_exception_count = 0;
    aid_except_vrtc = vrtc_f_create((Exec_Func)aid_except_vrtc_timetout);
    
    memset(&aid_idbit, 0, sizeof(aid_idbit));   //initialize aid bitmap

    update_aid_content();


    // be sure some master record exists
    if(nvfs_rd_at(unit_name(0),0,&unit_record,sizeof(unit_record)) != sizeof(unit_record))
    {
        memset(&unit_record,0,sizeof(unit_record)); // create blank record
        nvfs_wr_create(unit_name(0),&unit_record,sizeof(unit_record));
        nvfs_wr_at(unit_name(0), 0, my_aid_content + 1, UUID_SIZE);
    }
    
    master_auth_init(); // be sure we have a broadcast key
    
        have_devices = 0;
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
                      aid_db[i].sensors = buf[13]>>4;
            if (nvfs_rd_at(unit_name(i), UUID_SIZE, buf, KEY_SIZE) == KEY_SIZE
                && !is_zero(buf, KEY_SIZE)
                )
                aid_db[i].status |= (1 << AID_STATUS_BIT_KEY);
                nvfs_set_attributes(unit_name(0), NVFS_ATTRIB_HIDDEN);
        }
    }
        
        aid_db[0].id = AID_MASTER;

    aid_state = AID_STATE_CLOSED_WTMR;
        
    memcpy(seed,my_aid_content+1,KEY_SIZE);

    i = 0xFF;
    proto_sendmsg(PROTO_AID, &i, 1);  // send AID_SCAN and see who replies
    dhp_timer(100);
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





void auth_generate(UInt8 *key,UInt8 *pkt, unsigned len,UInt8 *prev_auth)
{
UInt32 ts;
unsigned plen;
    
  plen = len - AUTH_SIZE;

  memset(pkt + plen,0,AUTH_SIZE); // clear the "received" syndrome.

  ts = auth_compute(key,pkt,len,prev_auth); // the returned "implied timestamp" is actually the syndrome in host order
  ts ^= time(NULL); // exclusive-or with current time in host order
  ts = ntohl(ts); // convert to network byte order
  memcpy(pkt + plen,&ts,AUTH_SIZE); // write syndrome ^ timestamp into packet
  memcpy(prev_auth,&ts,AUTH_SIZE);  // save copy for authenticating next packet
}







///////////////// this is needed only if we want to verify the auth bytes in a packet //////////////////////////



#define ACCEPTANCE_WINDOW (4) // seconds
int within_window(UInt32 delta)
{
  return(  delta <= ACCEPTANCE_WINDOW
        || delta >= (UInt32)(-ACCEPTANCE_WINDOW)
        );
}


UInt8 broadcastkey[KEY_SIZE];
static UInt8 authkey[KEY_SIZE];
// verify this packet which was received very recently (ie "now")
int auth_verify(UInt8 *pkt,unsigned len)
{
UInt32 pkttime;
UInt32 curtime;
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
                memcpy(aid_db[i].prev_auth, pkt+len-AUTH_SIZE,AUTH_SIZE); // save successful auth for next time
                return TRUE;
            }

            // auth failed, but save this auth in case the real unit tries to recover on next packet
            memcpy(aid_db[i].other_auth, pkt+len-AUTH_SIZE,AUTH_SIZE); // save successful auth for next time        
            if(pkt[1] == PROTO_FAIL + PROTO_AUTH && pkt[0] == pkt[2])
            {
                aes_decrypt(authkey, pkt+3, pkt+3);
                if(memcmp(authkey, pkt+3, KEY_SIZE) != 0)
                {    //lockout this unit. This unit must re-pair to main
                    aid_db[i].status &= ~(1<<AID_STATUS_BIT_KEY);
                    memset(pkt+3, 0, KEY_SIZE);
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
    if(memcmp(buf,authkey,KEY_SIZE)==0)
      proto_sendmsg(PROTO_FAIL + PROTO_AUTH, &pkt[2], 1); // short reply to AUTH_RESET means success.
  }
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
}


UInt8 dbg_buf[KEY_SIZE];
// send AUTH-RESET
void master_auth_reset(void)
{
    UInt8 dbg_key[KEY_SIZE] =
{ 0x4F, 0x23, 0xEA, 0xEA,
    0xF3, 0xFA, 0xF4, 0x5B,
    0xCD, 0x54, 0xA5, 0x45,
    0x86, 0x48, 0x40, 0x26
};

UInt8 buf[1+KEY_SIZE+AUTH_SIZE];
    
    memcpy(dbg_buf, dbg_key, KEY_SIZE); 
    aes_encrypt(dbg_key, dbg_buf, dbg_buf); 
    memset(buf,0,sizeof(buf));
    memcpy(buf+1,broadcastkey,KEY_SIZE);
    aes_decrypt(broadcastkey,buf+1,buf+1);
    buf[0] = AID_MASTER;
  memset(master_prev_auth, 0, AUTH_SIZE);
  proto_sendmsg(PROTO_FAIL + PROTO_AUTH,buf,sizeof(buf));
}
