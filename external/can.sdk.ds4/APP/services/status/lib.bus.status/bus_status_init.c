/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: bus_status_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/
//
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "bus_status_private.h"
#include "timeout_f.h"
#include "status.h"
#include "d2d_switch.h"
#include "d2d_code.h"
#include <string.h>
#include "per_exec.h"
#include "aid_dhp.h" // for AUTH_SIZE

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static SInt8 bus_status_is_armed = -1;  // neither 1=yes or 0=no
static SInt8 bus_status_is_tampered = -1;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void bus_status_tamper(SInt8 do_tamper)
{
	if(bus_status_is_tampered != do_tamper)
	{// change! no action
	}
	bus_status_is_tampered = do_tamper;
}

SInt8 bus_status_tampered(void)
{
	return bus_status_is_tampered;
}

void bus_status_arm(SInt8 do_arm)
{
	if(bus_status_is_armed != do_arm)
	{// change!
		bus_status_tamper(0); // clear tamper flag
	}
	bus_status_is_armed = do_arm;
}

SInt8 bus_status_armed(void)
{
	return bus_status_is_armed;
}

void bus_status_notify_tamper_open(void *arg)
{
		(void)arg;
	d2d_tx(SET_TAMPER_OPEN);
}


void bus_status_notify_tamper_close(void *arg)
{
	(void)arg;
	d2d_tx(SET_TAMPER_CLOSE);
}


/////////////////////////////////////////

Res cssos_res_access = NULL;

// mutex vs os timeslice - does NOT protect against isr!
void os_disable(void)
{
	if(!cssos_res_access)
		cssos_res_access = os_res_create();

	while(os_res_request(cssos_res_access , 0)==0)
	{
		;// keep waiting
	}

}
void os_enable(void)
{
	os_res_release(cssos_res_access) ;	
}

//////////////// cssos timers

unsigned cssos_systime = 0;
struct cssos_timer *cssos_timer_first = NULL;



#define CSSOS_HEAP_SIZE (12*sizeof(struct cssos_timer))
UInt32 cssos_heap[CSSOS_HEAP_SIZE/sizeof(UInt32)];
UInt16 cssos_memofs = 0;

void *cssos_memget(unsigned how_much)
{
void *ptr;
	
	how_much = (how_much + 3) & ~3; // round up to neared UInt32 boundary
	
	ptr = cssos_memofs + ((UInt8 *)cssos_heap);
	cssos_memofs+=how_much;
	return ptr;	
}

struct cssos_timer *cssos_timer_create(VOIDFUNC *handler)
{
struct cssos_timer *t;
  t =  (struct cssos_timer *)cssos_memget(sizeof(struct cssos_timer));
  memset(t,0,sizeof(struct cssos_timer)); // clear everything expecially t->running

  t->func = handler;
  return t;
}

// whether it's on the list or not, make sure this timer is OFF the list
void cssos_timer_stop(struct cssos_timer *t)
{
struct cssos_timer *p;
struct cssos_timer *q;

  os_disable();
  if(t->running)
  {
    for(p=NULL,q=cssos_timer_first;q;p=q,q=q->next)
    {
      if(q==t)
      {
        q = t->next;

        if(p)      
          p->next = q;
        else
          cssos_timer_first = q;

        if(q)
          q->delta += t->delta;
        break;       
      }
    }
  }
  os_enable();
  t->delta = t->running = 0;
  t->next = NULL;
}

// only safe when we KNOW this timer is NOT on the list
static void cssos_timer_insert(struct cssos_timer *t,unsigned ticks)
{
struct cssos_timer *p;
struct cssos_timer *q;

  os_disable();

  for(p=NULL,q=cssos_timer_first;q;p=q,q=q->next)
  {
    if(ticks < q->delta)
      break;

    ticks -= q->delta;
  }
  t->next = q;
  if(q)
    q->delta -= ticks;
  t->delta = ticks;
  if(p)
    p->next = t;
  else
    cssos_timer_first = t;

  t->running = 1;
  os_enable();
}

void cssos_timer_rewind(struct cssos_timer *t,unsigned ticks)
{
	cssos_timer_stop(t);
	if(ticks)
		cssos_timer_insert(t,ticks);
}

// start timer only if its not already running
void cssos_timer_cstart(struct cssos_timer *t,unsigned ticks)
{
	if(!t->running)
		cssos_timer_rewind(t,ticks);
	
}

void os_crash(void)
{
	for(;;);
}

void cssos_timers_elapse(unsigned ticks)
{
struct cssos_timer *q;
struct cssos_timer *copy[8];
unsigned e = 0; // no timers expired

  if(ticks==0)
    return;
  
  os_disable();
  
  while((q=cssos_timer_first)!=NULL)
  {
    if(ticks < q->delta)
    {
      q->delta -= ticks;
      break;
    }
    ticks -= q->delta;
    cssos_timer_first = q->next; // removed from list

    q->next = NULL;
    q->running = 0; // show it is expired
    q->delta = 0; // and no more time

    if(e<8)
      copy[e++] = q;
    else
      os_crash();
  }
  
  os_enable();


  // call all the handlers which expired.
  while(e)
  {
    q = copy[--e];  // address of timer but don't touch linkage or delta

    if(q->running==0 && q->func) // still expired, still has handler?
		{
      q->func(); // TODO: use ExecFunc mechanism in case some clients block waiting
		}
  }
}




void os_delay(unsigned ticks)
{// TODO: fixme
 os_tsk_wait((1000*ticks)/RT_SECOND); // os_tsk_wait() wants milliseconds
}

Per_Exec_Wake cssos_tick(void)
{
	cssos_systime++;
	cssos_timers_elapse(1);
	
	if( cssos_timer_first )
		return PER_EXEC_WAKE;
	return PER_EXEC_SLEEP;	
}
	

void cssos_init(void)
{
  // don't initialize any variables because some cssos_timer_create() and possibly cssos_timer_rewind() has already been called	
	per_exec_register((Per_Exec_Func)cssos_tick);
}

/////////// M2M function drivers ///////////

static cmd_insert(unsigned cmd)
{
	cmd_q_insert(cmd,COMMAND_INSERT_TIMEOUT);
}

#include "dei_siren.h"  // in cmall core...

static struct cssos_timer *honk_timer;
static UInt8 honk_pattern = 0;
void func_set_honks(UInt8 pattern)
{
	honk_pattern = pattern;
	cssos_timer_rewind(honk_timer,1);
}
static void honk_handler(void)
{
Dei_Siren_Chirp chirp;
	
	if(honk_pattern)
	{
		  chirp = DEI_SIREN_VALET_MODE_OFF;  // this is the only chirp value unconditionally doing DEI_OUT_HORN (50, 50, 20, 1)
			cmd_q_rs_insert( INPUT_CHIRP , 100 , (void*)chirp ); // has to be done via INPUT_CHIRP so firmware can also see it
		  if(--honk_pattern)
			  cssos_timer_rewind(honk_timer,RT_SECOND);  // once per second is good
	}
}

static void honk_init(void)
{
	honk_pattern = 0;
	honk_timer = cssos_timer_create(honk_handler);
}





static struct cssos_timer *headlight_timer;
static UInt8 headlight_pattern = 0; 
void func_set_headlights(UInt8 pattern)
{
	headlight_pattern = pattern<<1;
	cssos_timer_rewind(headlight_timer,1);
}
static void headlight_handler(void)
{
	if(headlight_pattern==0 || (headlight_pattern-- & 1))
	{
		cmd_insert(INPUT_PARKING_OFF); // turn it off
		if(headlight_pattern)
			cssos_timer_rewind(headlight_timer,RT_SECOND/2);
	}
	else
	{
		if(headlight_pattern)
		{			
			cmd_insert(INPUT_PARKING_ON);
			cssos_timer_rewind(headlight_timer,RT_SECOND/2);
		}		
	}
}

static void headlight_init(void)
{
	headlight_pattern = 0;
	headlight_timer = cssos_timer_create(headlight_handler);
}


static void func_set_rib(Boolean on)
{
	cmd_insert(INPUT_RIB | (on ? INPUT_ON : 0));
}

static struct cssos_timer *tire_pressure_timer;
void tire_pressure_handler(void); // forward reference
static void tire_pressure_init(void)
{
	tire_pressure_timer = cssos_timer_create(tire_pressure_handler);
}

////////////////// SAR transmit layer //////////////////////

#define SAR_MAX_PAYLOAD 26
struct sar_handle
{
	UInt8 ofs;
	UInt8 buf[4+SAR_MAX_PAYLOAD];
};
struct sar_handle sarhandle;
struct sar_handle *sar_open(UInt8 proto,UInt8 dest,UInt8 hdr)
{
struct sar_handle *h = &sarhandle; // we only have 1 buffer
	h->buf[0]=0xFE; // mark as in use!
	h->buf[1]=proto;
	h->buf[2]=dest;
	h->buf[3]=hdr | 1; // assume M bit = 1;
	h->ofs = 0;       // 0 data bytes so far
	return h;
}

static sar_flush(struct sar_handle *h)
{
	proto_sendmsg(h->buf[1],h->buf+2,h->ofs+2);
	h->ofs = 0;
}

void sar_append(struct sar_handle *h,const UInt8 *ptr,unsigned len)
{
unsigned amt;
	
	while(len)
	{
		amt = SAR_MAX_PAYLOAD - h->ofs;
		if(amt == 0)
		{// time to send
			h->buf[3] |= 0x01; // set M-bit
			sar_flush(h);
			h->buf[3] &= 0x81; // clear the middle bits
			continue;
		}
		
		if(amt > len)
			amt = len;
		memcpy(h->buf+4+h->ofs,ptr,amt);
		ptr += amt;				
		len -= amt;
		
		h->ofs+=amt;
	}
}

void sar_close(struct sar_handle *h)
{
	if(h->buf[0]==0)
		return; // not open
	
	h->buf[3] &= ~1; // clear M-bit
	sar_flush(h);
	memset(h->buf,0,4); // clear header to show it's not in use anymore
}

// abort my own stream
void sar_abort(struct sar_handle *h,UInt8 reason, const char *description)
{
unsigned limit,len;
	
	h->buf[3] |= 0x7E; // set all bits in header
	h->ofs = 0; // drop all payload
	if(reason)
	{
		h->buf[4] = reason;
		if(description)
		{
			limit = (h->buf[1] & PROTO_AUTH) ? SAR_MAX_PAYLOAD-1-AUTH_SIZE : SAR_MAX_PAYLOAD-1;
			len = strlen(description); // don't count trailing nul byte
			if(len > limit)
				len = limit;
			memcpy(h->buf+5,description,len);
			h->ofs = len;
		}
		h->ofs++;
	}
	sar_close(h);
}


////////////////// M2M additions ///////////////////

// command headers
#define M2M_CMD_READVALUES 0x82
#define M2M_CMD_READTAGGED 0x84
#define M2M_CMD_WRITETAGGED 0x86
#define M2M_CMD_READNAMES   0x88

// response headers
#define M2M_HDR_VALUE    0x02
#define M2M_HDR_TAG      0x04
#define M2M_HDR_TAGVALUE 0x06
#define M2M_HDR_TAGNAME  0x08


static UInt8 seat_alarm = 0; // who doesn't have their belt buckled?

// id allows 12 bits, but we'll try to live in 8.  Per page #x, we have id regions:
// x00..x4F allows 80 single bit read status  (grouping saves bandwidth)
#define M2M_ID_OIL_ALARM 0x020
#define M2M_ID_TIRE_ALARM 0x021
#define M2M_ID_KIB_STATE  0x4E
#define M2M_ID_TRIKE      0x4F

// x50..xCF allows 128 byte and multibyte read status (order unimportant)
#define M2M_ID_SEAT_BELTS 0x050
#define M2M_ID_SEAT_WEIGHTS 0x051
#define M2M_ID_EV_BATTERY_LVL 0x052
#define M2M_ID_EV_RANGE     0x053
#define M2M_ID_OIL_LIFE     0x054
#define M2M_ID_FUEL_LVL   0x055
#define M2M_ID_FUEL_CAP     0x056
#define M2M_ID_CABIN_TEMP   0x057
#define M2M_ID_TIRE_PRESSURE 0x058
#define M2M_ID_ODOMETER      0x059
#define M2M_ID_BATTERY_VOLTAGE 0x05A
#define M2M_ID_SPEED          0x05B

// xD0..xEF allows 32 single bit on/off control functions (grouping unlikely to save bandwidth)
#define M2M_ID_RIB 0x0D0

// xF0..xFF allows 16 byte and multibyte control functions (order unimportant)
#define M2M_ID_HONKS      0x0F0
#define M2M_ID_HEADLIGHTS 0x0F1

// fmt is 4 bits
#define M2M_FMT_BIT0 0
#define M2M_FMT_U8  1
#define M2M_FMT_U16 2
#define M2M_FMT_X8  3
#define M2M_FMT_U32 4
#define M2M_FMT_X16 5
#define M2M_FMT_CSTR 6
#define M2M_FMT_BSTR 7
#define M2M_FMT_BIT1 8
#define M2M_FMT_S8  9
#define M2M_FMT_S16 0xA
#define M2M_FMT_HPREC 0xB
#define M2M_FMT_S32 0xC
#define M2M_FMT_SPREC 0xD
#define M2M_FMT_JSTR  0xE
#define M2M_FMT_LARGE 0xF // reserved for U32-size tags: 256 values and 1 million IDs


// single fixed format for tags
#define TAG16(fmt,id) (((fmt)<<12) | (id))
static UInt8 tag_fmt(unsigned tag)
{
	if(tag >= (M2M_FMT_LARGE<<20))
		return (tag>>20) & 0xFF; // FUTURE
	
	return (tag>>12) & 0xF; // today
}

static unsigned tag_id(unsigned tag)
{
	if(tag >= (M2M_FMT_LARGE<<20))
		return tag & 0xFFFFF; // FUTURE
	
	return tag & 0xFFF; // today	
}
static UInt8 write_tag(UInt8 *buf,unsigned fmt, unsigned id)
{
unsigned tag = 0;
UInt8 len,i;
	
		if(fmt<M2M_FMT_LARGE && id<4096)
		{
			tag = (fmt<<12) | id;
			i=2;
		}
		else
		{
			tag = 0xF0000000 | (fmt<<20) | id;
			i=4;
		}
				
		len = i; // save length so far
		while(i)
		{
			buf[--i] = tag; // write tag in network-byte-order
			tag>>=8;
		}
		return len;
}

UInt8 fmt_len(UInt8 fmt)
{
	switch(fmt & 6)
	{
	case (0<<1):
	  return(fmt & 1);  // 1 for bytes, 0 for bits
	case (1<<1):
		return sizeof(UInt16); // half
	case (2<<1):
		return sizeof(UInt32); // full
	default:
		return 0xFF; // some kind of string (variable length)
	}
}

static UInt8 write_value(UInt8 *buf,unsigned fmt,unsigned value)
{
UInt8 len,i;
	
		len = fmt_len(fmt); // var, byte, half, full
		if(len && len != 0xFF)
		{
			i = len;
			while(i)
			{
				buf[--i] = (UInt8)value;
				value>>=8;
			}
		}
//		else // TODO: variable length data
//		{
//		}
		return len;	
}

typedef Boolean VFRU1(void);
typedef UInt8 VFRU8(void);
typedef SInt8 VFRS8(void);
typedef UInt16 VFRU16(void);
typedef UInt32 VFRU32(void);
typedef int IFRI(int);

typedef union anyfunc_rd
{
	VFRU1 *fp_u1;
	VFRU8 *fp_u8;
	VFRS8 *fp_s8;
	VFRU16 *fp_u16;
	VFRU32 *fp_u32;
	IFRI *fp_bstr; // positive index refers to a byte, negative index asks for the size, negative size means error.
	// insert more function types here, as needed
} ANYFUNC_RD;

typedef void VFRV(void);
typedef void U1FRV(Boolean);
typedef void U8FRV(UInt8);
typedef void S8FRV(SInt8);
typedef void U16FRV(UInt16);
typedef void U32FRV(UInt32);
	
typedef union anyfunc_wr
{
	U1FRV *fp_u1;
	U8FRV *fp_u8;
	S8FRV *fp_s8;
	U16FRV *fp_u16;
	U32FRV *fp_u32;
	VFRV  *fp_v;
	// insert more function types here, as needed
} ANYFUNC_WR;


#define TAG_EVT(n) ((1<<(n))<<24)

#define PUSHN TAG_EVT(0) // notify on all changes

#define EVNT_IGN_ON TAG_EVT(1)
#define EVNT_IGN_OFF TAG_EVT(2)
#if 0 // TODO later maybe
#define EVNT_DOOR_OPEN TAG_EVT(3)
#define EVNT_DOOR_CLOSE TAG_EVT(4)
#endif


#define TAG_BIT(id) (TAG16(M2M_FMT_BIT0,(id)))
#define TAG_BYTE(id) (TAG16(M2M_FMT_U8,(id)))
#define TAG_SBYTE(id) (TAG16(M2M_FMT_S8,(id)))
#define TAG_HALF(id) (TAG16(M2M_FMT_U16,(id)))
#define TAG_SALF(id) (TAG16(M2M_FMT_S16,(id)))
#define TAG_WORD(id)  (TAG16(M2M_FMT_U32,(id)))
#define TAG_BSTR(id)  (TAG16(M2M_FMT_BSTR,(id)))
#define TAG_CSTR(id)  (TAG16(M2M_FMT_CSTR,(id)))


#if STATUS_OIL_ALARM_ENABLE < 2
#define status_req_oil_alarm NULL
#else
extern void status_req_oil_alarm(void);
#endif
#if STATUS_TIRE_ALARM_ENABLE < 2
#define status_req_tire_alarm NULL
#else
extern void status_req_tire_alarm(void);
#endif
#if STATUS_SEAT_BELTS_ENABLE < 2
#define status_req_seat_belts NULL
#else
extern void status_req_seat_belts(void);
#endif
#if STATUS_SEAT_WEIGHTS_ENABLE < 2
#define status_req_seat_weights NULL
#else
extern void status_req_seat_weights(void);
#endif
#if STATUS_EV_BATTERY_LVL_ENABLE < 2
#define status_req_ev_battery_lvl NULL
#else
extern void status_req_ev_battery_lvl(void);
#endif
#if STATUS_EV_BATTERY_RANGE_ENABLE < 2
#define status_req_ev_range NULL
#else
extern void status_req_ev_range(void);
#endif
#if STATUS_OIL_LIFE_ENABLE < 2
#define status_req_oil_life NULL
#else
extern void status_req_oil_life(void);
#endif
#if STATUS_FUEL_LVL_ENABLE < 2
#define status_req_fuel_lvl NULL
#else
extern void status_req_fuel_lvl(void);
#endif
#if STATUS_FUEL_CAP_ENABLE < 2
#define status_req_fuel_cap NULL
#else
extern void status_req_fuel_cap(void);
#endif
#if STATUS_CABIN_TEMP_ENABLE < 2
#define status_req_cabin_temp NULL
#else
extern void status_req_cabin_temp(void);
#endif
#if STATUS_TIRE_PRESSURE_ENABLE < 2
#define status_req_tire_pressure NULL
#else
extern void status_req_tire_pressure(void); // requests tire pressure for all tires
#endif
#if STATUS_ODOMETER_ENABLE < 2
#define status_req_odometer NULL
#else
extern void status_req_odometer(void);
#endif
#if STATUS_BATTERY_VOLTAGE_ENABLE < 2
#define status_req_battery_voltage NULL
#else
extern void status_req_battery_voltage(void);
#endif
#if STATUS_SPEED_ENABLE < 2
#define status_req_m2m_speed NULL
#else
extern void status_req_m2m_speed(void);  // request speed from digital source
#endif

struct m2m_item
{
	UInt32 tag;
	ANYFUNC_RD addr_rd;
	ANYFUNC_WR addr_wr;
	const char *name;	
} m2m_table[]=
{ // This table MUST be sorted in id order!
	// Just OR all the events when you want the data item to be sent
	// PUSHN just means to send it immediately whenever it changes.
  // grouping single bits saves bandwidth!
	
// ids 0x01..0x4F allows 80 single bit readable status	
#if STATUS_OIL_ALARM_ENABLE > 0	
 {PUSHN | EVNT_IGN_ON | EVNT_IGN_OFF     | TAG_BIT(M2M_ID_OIL_ALARM),
   status_get_oil_alarm, (U1FRV *)status_req_oil_alarm,"oil_alarm"},
#endif
#if STATUS_TIRE_ALARM_ENABLE > 0 
 {PUSHN | EVNT_IGN_ON | EVNT_IGN_OFF          | TAG_BIT(M2M_ID_TIRE_ALARM),
   status_get_tire_alarm, (U1FRV *)status_req_tire_alarm,"tire_alarm"},
#endif	 
  // kib_state is unconditionally supported, even when there is no kib (could be kib by io)
	{ 0 | TAG_BIT(M2M_ID_KIB_STATE),
		status_get_kib_state, NULL, "KIB_state"},

 // ids 0x50..0xCF allows 128 byte- and multibyte status
#if STATUS_SEAT_BELTS_ENABLE > 0 
 {PUSHN           | TAG_BYTE(M2M_ID_SEAT_BELTS),
   status_get_seat_belts, (U1FRV *)status_req_seat_belts,"seat_belts"},
#endif
#if STATUS_SEAT_WEIGHTS_ENABLE > 0 
 {PUSHN           | TAG_BYTE(M2M_ID_SEAT_WEIGHTS),
   status_get_seat_weights, (U1FRV *)status_req_seat_weights,"seat_weights"},
#endif
#if STATUS_EV_BATTERY_LVL_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF                | TAG_BYTE(M2M_ID_EV_BATTERY_LVL),
   status_get_ev_battery_lvl, (U1FRV *)status_req_ev_battery_lvl,"ev_battery_lvl"},
#endif																
#if STATUS_EV_RANGE_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF                | TAG_HALF(M2M_ID_EV_RANGE),
   status_get_ev_range, (U1FRV *)status_req_ev_range,"ev_range"},
#endif																
#if STATUS_OIL_LIFE_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF                | TAG_BYTE(M2M_ID_OIL_LIFE),
   status_get_oil_life, (U1FRV *)status_req_oil_life,"oil_life"},
#endif																
#if STATUS_FUEL_LVL_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF                | TAG_BYTE(M2M_ID_FUEL_LVL),
   status_get_fuel_lvl, (U1FRV *)status_req_fuel_lvl,"fuel_lvl"},
#endif																
#if STATUS_FUEL_CAP_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF                | TAG_BYTE(M2M_ID_FUEL_CAP),
   status_get_fuel_cap, (U1FRV *)status_req_fuel_cap,"fuel_cap"},
#endif
#if STATUS_CABIN_TEMP_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF                 | TAG_SBYTE(M2M_ID_CABIN_TEMP),
   (VFRU1 *)status_get_cabin_temp, (U1FRV *)status_req_cabin_temp, "cabin_temp"},
#endif
#if STATUS_TIRE_PRESSURE_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF                 | TAG_BSTR(M2M_ID_TIRE_PRESSURE),
   (VFRU8 *)status_get_tire_pressure, (U1FRV *)status_req_tire_pressure, "tire_pressure"},
#endif
#if STATUS_ODOMETER_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF       | TAG_WORD(M2M_ID_ODOMETER),
   (VFRU1 *)status_get_odometer, (U1FRV *)status_req_odometer, "odometer"},
#endif
#if STATUS_BATTERY_VOLTAGE_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF       | TAG_HALF(M2M_ID_BATTERY_VOLTAGE),
   (VFRU1 *)status_get_battery_voltage, (U1FRV *)status_req_battery_voltage, "battery_voltage"},
#endif
#if STATUS_SPEED_ENABLE > 0
 { EVNT_IGN_ON | EVNT_IGN_OFF       | TAG_HALF(M2M_ID_SPEED),
   (VFRU1 *)status_get_m2m_speed, (U1FRV *)status_req_m2m_speed, "speed"},
#endif

#if FUNCTION_RIB_ENABLE == 1
 // ids 0xD0..0xEF allows 32 single bit writeable functions (on/off)
 {                                TAG_BIT(M2M_ID_RIB), NULL, func_set_rib,"RIB"},
#endif 
 // ids 0xF0..0xFF allows 16 byte- and multibyte functions
#if FUNCTION_HONKS_ENABLE == 1 
 {                                TAG_BYTE(M2M_ID_HONKS), NULL, func_set_honks,"Honks"},
#endif
#if FUNCTION_HEADLIGHTS_ENABLE == 1 
 {                                TAG_BYTE(M2M_ID_HEADLIGHTS), NULL, func_set_headlights,"Headlights"},
#endif
 {  TAG_BYTE(0x0FFF), NULL, NULL } // item 4095 marks end of table, it can never be read or written. 
};


// fixed-length formats only!  buf must be large enough!
static UInt8 write_tagged_value(UInt8 *buf,unsigned x, unsigned value)
{
UInt8 len,i,fmt;
UInt32 tag,id;

	  tag = m2m_table[x].tag & 0xFFFF;
		fmt = tag_fmt(tag);
	  id = tag_id(tag);
	
		if((fmt & 7)==0) // single bit?
		{// special case for single-bit economy
			if(!(fmt & 8) != !value) // is data different than fmt?
				fmt ^= 8;  // then flip fmt to match							
			return write_tag(buf,fmt,id); // data is encoded in the tag
	}
		// regular bytes...
		if((fmt & 6) == 6) // all strings
			return 0; // TODO: strings later...
		
	  len = write_tag(buf,fmt,id);
		len +=write_value(buf+len,fmt,value);
		
		return len;
	
}


// get status of m2m_table item at index x
unsigned read_m2m_item_data(UInt8 x)
{
unsigned dat = 0;
UInt8 fmt;
	
	
	fmt = tag_fmt(m2m_table[x].tag);
	switch(fmt_len(fmt))
	{
		case 1: // byte
			dat = (m2m_table[x].addr_rd.fp_u8)();					
			break;
		case 2: // half
			dat = (*m2m_table[x].addr_rd.fp_u16)();
			break;
		case 4: // full
			dat = (*m2m_table[x].addr_rd.fp_u32)();
			break;
		
		case 0: // single bit
			dat = (*m2m_table[x].addr_rd.fp_u1)();
		
		default: // TODO: strings later
			break;
	}
	return dat;
}

int read_m2m_bstr_byte(UInt8 x,int index)
{
	return (*m2m_table[x].addr_rd.fp_bstr)(index);
}



UInt8 m2m_table_entries(void)
{
	return sizeof(m2m_table)/sizeof(m2m_table[0]);
}


#define NO_ITEM 0xFF
static UInt8 find_item(unsigned id)
{
struct sar_handle *h;
unsigned i;
UInt32 dat;
UInt8 hi,lo,x,fmt,len;
UInt8 buf[8]; // large enough for u32 tag and value
	
	hi=m2m_table_entries();
	lo=0;
	while(hi!=lo)
	{
		x = (hi+lo)>>1;
		i = tag_id(m2m_table[x].tag & 0xFFFF);
		if(id == i)
		{// id found in table      			
			return x;
		}
		if(id < i)
			hi = x;
		else
			lo = x+1;
	}
	return NO_ITEM;
}

int is_m2m_string(UInt8 x)
{
	return (((m2m_table[x].tag>>12) & 6) == 6);
}

void send_m2m_tag(struct sar_handle *h,UInt8 x)
{
int len,i;
unsigned fmt,tag;
UInt8 buf[8];
	
	
	fmt = (m2m_table[x].tag >> 12) & 0xF;
	len = write_tag(buf,fmt,m2m_table[x].tag & 0xFFF);
	sar_append(h,buf,len);
}

void send_m2m_string(struct sar_handle *h,UInt8 x)
{
int len,i;
unsigned fmt;
UInt8 buf[8];
	
	
	fmt = m2m_table[x].tag >> 12;	
	len = m2m_table[x].addr_rd.fp_bstr(-1); // get string length in bytes
	
	if(fmt & 1) // fmt uses length prefix?
	{
		if(fmt & 8) // halfword size?
		{
			buf[0] = len>>8;
			sar_append(h,buf,1); // write msb
		}
		buf[0] = len;
		sar_append(h,buf,1); // write lsb
	}
	// write len
	for(i=0;i<len;i++)
	{
		buf[0] = m2m_table[x].addr_rd.fp_bstr(i); // get byte
		sar_append(h,buf,1); // write byte
	}
	if((fmt & 1)==0) // fmt uses trailing zero?
	{
		buf[0] = buf[1] = 0;
		sar_append(h,buf,1+(fmt>>3));
	}	
}


#define OBD_STALE_TIMEOUT (10*1000)
#define EVT_MAX 8 // must be binary
static UInt32 evt_tstamp[EVT_MAX];  // all zero after init
static UInt8 evtcnt[EVT_MAX];
static UInt8 evtq[EVT_MAX];
static UInt8 evt_nr = 0;
static UInt8 evt_nw = 0;


UInt32 bulk_event_mask = 0;
UInt8 bulk_event_number = 0;

// guaranteed only one thread will execute, see m2m_bulk_event() for proof
void m2m_bulk_handler(void *payload)
{
struct sar_handle *h;
int i,len;
UInt8 prev_bit,fmt,x;
UInt32 tag,value,now;
UInt8 buf[8];
	


  os_disable();  // mutex to protect against os timeslice,

  while(evt_nw != evt_nr) // any events pending?
  {
    bulk_event_number = evtq[evt_nr & (EVT_MAX-1)];
		bulk_event_mask = (1<<bulk_event_number)<<24;
		
    os_enable();

    // process event
    now = cssos_systime;
    if(now == 0)
      now++;  // time must never be 0 for us

    if(evt_tstamp[bulk_event_number]==0 || now - evt_tstamp[bulk_event_number] >= OBD_STALE_TIMEOUT)
    {// re-request items via OBD
			
			if(bulk_event_number == 1) // ignition on event?
			{
				// step 0, wait for vehicle to be ready
				os_delay(3*RT_SECOND); // give vehicle time to update our 'automatic' items
				now = cssos_systime;
			}
      			
      evt_tstamp[bulk_event_number] = now; // remember what time we requested this

			// step 1, try to refresh the associated items
			for(x=0; x<m2m_table_entries(); x++)
			{
				if((m2m_table[x].tag & bulk_event_mask) && (m2m_table[x].addr_wr.fp_u8 != NULL))
				{// this item must be requested first
					(*m2m_table[x].addr_wr.fp_u8)(bulk_event_number); // pass the event number to help
				}
			}
	
			// step 2, wait 1 second for all the requests to finish
			for(x=0;x<5;x++)
			{
				os_delay(RT_MILLI(200));
				if(bulk_event_mask == 0)
					return; // cancelled by receiver
			}
			

		} // otherwise, just sent the recent values still in ram
				
		// step 3, now send the broadcast
		h = sar_open(PROTO_M2M, 0xFF, (bulk_event_number<<4) | 0x02); // a values-only broadcast
																														 // interested receivers will know the tags 
		prev_bit = 0;
		for(x=0; x<m2m_table_entries(); x++)
		{
			if(bulk_event_mask == 0)
				break;
			
			if((m2m_table[x].tag & bulk_event_mask) && (m2m_table[x].addr_rd.fp_u8 != NULL))
			{// now we can read

				tag = m2m_table[x].tag & 0xFFFF;
				fmt = tag_fmt(tag);
				if(is_m2m_string(x)) // string?
				{
					send_m2m_string(h,x);
				}
				else
				{
					value = read_m2m_item_data(x);
					
					if((fmt & 7)==0)
					{// writing single bit
						if(prev_bit==0)
						{// this is the first bit
							buf[0] = 0;
							prev_bit = 1; // we wrote bit 1
						}
						else
						{
							prev_bit<<=1;
							if(prev_bit==0)
							{
								sar_append(h,buf,1);
								buf[0]=0;
								prev_bit = 1;
							}
						}
						if(value & 1)
							buf[0] |= prev_bit;
					}
					else
					{// writing a non-bit
						if(prev_bit)
							sar_append(h,buf,1); // finish writing out bits
						prev_bit = 0;
						
						len = write_value(buf,fmt,value);
						sar_append(h,buf,len);
						
					}
				}
			}
		}
			
		if(bulk_event_mask == 0)
		{// cancelled by user
			sar_abort(h,0xff,"client-side");
		}
		else
		{
			if(prev_bit)
				sar_append(h,buf,1);
			sar_close(h); // msg sent
			
			os_delay(RT_MILLI(60));  // give some time for packets to be sent....
		}
	
    // finished event, do we keep it or throw it away?
    os_disable();
    evt_nr++; // delete event_number from evtq
    if(--evtcnt[bulk_event_number]) // did any more of this type occur?
    {// yes, put this event back into the rotation
      evtq[evt_nw & (EVT_MAX-1)] = bulk_event_number;
      evt_nw++;
    }
  } // os still disabled

  os_enable();
	
	
	bulk_event_mask = 0;
	bulk_event_number = 0;
}


void m2m_bulk_event(UInt8 event_number)
{
unsigned busy = 1;

  if(event_number >= EVT_MAX)
    return;

  os_disable();
  if(++evtcnt[event_number] == 1) // first time for this event?
  {
    evtq[evt_nw & (EVT_MAX-1)] = event_number;
    busy = evt_nw - evt_nr;   // 0 iff q was empty
    evt_nw++; // put it on queue
  }
  os_enable();

  if(!busy)
    cmd_insert(INPUT_M2M_BULK_HANDLER);

}


// a push notification has only one tagged-value today
void m2m_may_push(unsigned id)
{
UInt8 x,b;
unsigned value;
int len,i;
UInt8 buf[8];
struct sar_handle *h;
	
	
	x= find_item(id);
	if(x == NO_ITEM || !(m2m_table[x].tag & PUSHN) || (m2m_table[x].tag & bulk_event_mask))
		return; // don't send push notification

	h = sar_open(PROTO_M2M,0xFF, M2M_HDR_TAGVALUE); // push notifications are broadcast
	
	if(is_m2m_string(x))
	{// handle all 4 types of string....
		send_m2m_tag(h,x);
		send_m2m_string(h,x);
	}
	else
	{// words
		value = read_m2m_item_data(x);
		len = write_tagged_value(buf,x,value);
		sar_append(h,buf,len);
	}
	sar_close(h);
}

void bus_status_oil_low_alarm(void *arg)
{
	m2m_may_push(M2M_ID_OIL_ALARM);
}

void bus_status_tire_low_alarm(void *arg)
{
	m2m_may_push(M2M_ID_TIRE_ALARM);
}

void bus_status_seat_belts(void *arg)
{
	m2m_may_push(M2M_ID_SEAT_BELTS);
}

void bus_status_seat_weights(void *arg)
{
	m2m_may_push(M2M_ID_SEAT_WEIGHTS);
}

void bus_status_ev_battery_lvl(void *arg)
{
	m2m_may_push(M2M_ID_EV_BATTERY_LVL);
}

void bus_status_ev_range(void *arg)
{
	m2m_may_push(M2M_ID_EV_RANGE);
}
void bus_status_oil_life(void *arg)
{
	m2m_may_push(M2M_ID_OIL_LIFE);
}

void bus_status_fuel_lvl(void *arg)
{
	m2m_may_push(M2M_ID_FUEL_LVL);
}

void bus_status_fuel_cap(void *arg)
{
	m2m_may_push(M2M_ID_FUEL_CAP);
}

void bus_status_cabin_temp(void *arg)
{
	m2m_may_push(M2M_ID_CABIN_TEMP);
}

extern UInt8 tire_pressure_pending;
struct cssos_timer *tire_pressure_timer;
void tire_pressure_handler(void)  // timer was finally allowed to expire
{
	tire_pressure_pending = 0;
	m2m_may_push(M2M_ID_TIRE_PRESSURE);  // NOW send
}

void bus_status_tire_pressure(void *arg)
{
	tire_pressure_pending = 2; // every time we rewind timer we goto state 2
	cssos_timer_rewind(tire_pressure_timer,RT_MILLI(200)); // refresh this timer (before it expires)
}

void bus_status_odometer(void *arg)
{
	m2m_may_push(M2M_ID_ODOMETER);
}

void bus_status_battery_voltage(void *arg)
{
	m2m_may_push(M2M_ID_BATTERY_VOLTAGE);
}

void bus_status_speed(void *arg)
{
	m2m_may_push(M2M_ID_SPEED);
}


///////////////  d2d2_m2m_handler //////////////////////////////////////


struct pktstream
{
	UInt8 *ptr;
	int len; // must be signed to be able to detect errors
} pktstream;

UInt8 get_byte(struct pktstream *p)
{
	p->len--;
	return *p->ptr++;
}
UInt16 get_half(struct pktstream *p)
{
UInt16 tag;
	tag = get_byte(p);
	return (tag<<8) | get_byte(p);
}

unsigned get_tag(struct pktstream *p)
{
unsigned tag;
	tag = get_half(p);
	if(tag >= (M2M_FMT_LARGE<<12))
	{
		tag = (tag<<16) | get_half(p);
	}
	return tag;
}

Boolean is_evt_readable(UInt8 x,UInt8 evt)
{
	return!!(   m2m_table[x].addr_rd.fp_u32
				  && (evt==0 || m2m_table[x].tag & ((1<<evt)<<24))
	        );
}

Boolean is_m2m_restricted(UInt8 *pkt, UInt8 len)
{
  return ((pkt[3] & 0x8E)==M2M_CMD_WRITETAGGED && len > 4) ; // for now, we only restrict m2m writes
}

UInt8 m2m_deferred_pkt[32];
unsigned m2m_deferred_len;

void d2d2_m2m_handler(UInt8 *pkt, UInt8 len)
{
	
	if(len < 4 || pkt[2] != 0xFE)
		return;
	if((pkt[3] & 0x7E)==0x7E)
	{// error? stop?
		if(bulk_event_mask)
		{
			bulk_event_mask = 0; // abort
			bulk_event_number = 0;
		}
		return;
	}
	if((pkt[3] & 0x81) != 0x80)
		return; // TODO: someday support messages longer than one packet
	
	m2m_deferred_len = len;
	memcpy(m2m_deferred_pkt,pkt,len);
	cmd_insert(INPUT_M2M_DEFERRED);
}


void m2m_deferred_handler(void *arg)
{
UInt8 *pkt;
unsigned len;
UInt8 buf[8];
UInt8 x,fmt,prev_bit,evt;
unsigned tag,value,id;
struct sar_handle *h;
	
	  pkt = m2m_deferred_pkt;
	  len = m2m_deferred_len;
	
	
		pktstream.ptr = pkt+4;
		pktstream.len = len-4;
		
		evt = (pkt[3] >> 4) & 7; // extract event number
		
		switch(pkt[3] & 0x8E)
		{				
			case M2M_CMD_READTAGGED:
				if(pktstream.len == 0) // empty list?
				{// they ask for all readable tags (for this event)
					h = sar_open(PROTO_M2M,(evt==0 ? pkt[0] : 0xFF),M2M_HDR_TAG+(evt<<4)); // event taglist is broadcast
					for(x=0;x<m2m_table_entries();x++)
					{
						if(is_evt_readable(x,evt)) // readable for this event?
						{
							send_m2m_tag(h,x);
						}
					}
					sar_close(h);
					break;
				}

				h = sar_open(PROTO_M2M,pkt[0],M2M_HDR_TAGVALUE); // send only as reply				
				while(pktstream.len)
				{
					tag = get_tag(&pktstream);
					
					if(pktstream.len<0)
						break; // corrupt request, stop here
					
					tag &= 0xFFF; // ignore user-supplied format
					if(tag >= 0xF00)
					{// asking about d2d2 aid status/diagnostic
						
						value = aid_get_diagnostic(tag & 0xFF);
						len = write_tag(buf,1,tag);
						len += write_value(buf+len,1,value);
						sar_append(h,buf,len);
					}
					else
					{
						x = find_item(tag & 0xFFF);
						if(x != NO_ITEM && m2m_table[x].addr_rd.fp_u32)
						{
							if(is_m2m_string(x))
							{
								send_m2m_tag(h,x);								
								send_m2m_string(h,x);
							}
							else
							{
								value = read_m2m_item_data(x);
								len = write_tagged_value(buf,x,value); // show them what the proper format is
								sar_append(h,buf,len);
							}
						}
				  }					
				}
				sar_close(h);
				break;
			
			case M2M_CMD_WRITETAGGED:
				if(pktstream.len==0) // empty list?
				{// they ask for all writeable
					h = sar_open(PROTO_M2M,pkt[0],M2M_HDR_TAG); // send as reply
					for(x=0;x<m2m_table_entries();x++)
					{
						if((m2m_table[x].tag>>24)==0 && m2m_table[x].addr_wr.fp_u32)
						{
							send_m2m_tag(h,x);
						}
					}
					sar_close(h);
					break;
				}
				while(pktstream.len)
				{
					tag = get_tag(&pktstream); // beware! the values are in the user's chosen format!
					if(pktstream.len<0)
						break; // corrupt request, stop here
					fmt = tag_fmt(tag);
					id = tag & 0xFFF;
					
					if((fmt & 6) == 6)
            break;	// TODO: one day we should accept strings from users

					if((fmt & 7)==0)
					{// client wants to write a bit
						value = (fmt >> 3) & 1;
					}
					else
					{// client wants to write byte(s)
						value = 0;
						switch(fmt_len(fmt))
						{
							case 4:
								value = (value<<8) | get_byte(&pktstream);
								value = (value<<8) | get_byte(&pktstream);
							case 2:
								value = (value<<8) | get_byte(&pktstream);
							case 1:
								value = (value<<8) | get_byte(&pktstream);
							default:
								break;
						}
						// apply sign
						if(fmt == M2M_FMT_S8 && (value & 0x80))
							value |= ~0xFF; // set all msbits
						if(fmt == M2M_FMT_S16 && (value & 0x8000))
							value |= ~0xFFFF;						
					}
					if(pktstream.len < 0)
						break;  // client data corrupt, stop here
					
					x = find_item(id);
					if(x == NO_ITEM || !m2m_table[x].addr_wr.fp_u32)
						continue; // we cannot write to this item
					if(is_m2m_string(x))
						continue; // we cannot convert value to string
					
					if(fmt != tag_fmt(m2m_table[x].tag)) // compare against our format for this item
					{
						// TODO: insert any fmt coversions here
					}
					// finally, write the item value
					(*m2m_table[x].addr_wr.fp_u32)(value); // this works for all memory-size types

				}
				break;
			
			case M2M_CMD_READVALUES:
				if(pktstream.len == 0)
				{// they ask to trigger a bulk read
					if(evt<1 || evt>6)
						break; // sorry, can't dump all triggers
					prev_bit = 0;
					h = sar_open(PROTO_M2M,0xFF,M2M_HDR_VALUE + (evt<<4));				
					for(x=0;x < m2m_table_entries();x++)
					{
						if(is_evt_readable(x,evt)) // readable for this event?
						{// matching readable event?
						  tag = m2m_table[x].tag & 0xFFFF;
							fmt = tag_fmt(tag);
							if(is_m2m_string(x))
							{// string
								send_m2m_string(h,x);
							}
							else
							{
								value = read_m2m_item_data(x);
								if((fmt & 7)==0)
								{// sending a bit
									if(prev_bit)
									{
										prev_bit<<=1;
										if(prev_bit==0)
										{
											sar_append(h,buf,1); // 8 bits already?
											
											buf[0]=0;
											prev_bit=1;
										}
									}
									else
									{
										buf[0]=0;
										prev_bit=1; // this is the first
									}
									
									buf[0] |= prev_bit;
								}
								else
								{// sending a value
									if(prev_bit)
										sar_append(h,buf,1); // flush previous bits out
									prev_bit = 0;
									
									len = write_value(buf,fmt,value);
									sar_append(h,buf,len);
								}
							}
						}
					}
					sar_close(h);
					break;
				}
				
			  break;
			case M2M_CMD_READNAMES:
					h = sar_open(PROTO_M2M,pkt[0],M2M_HDR_TAGNAME + (evt<<4));				
					for(x=0;x < m2m_table_entries();x++)
					{
						  if(!m2m_table[x].name)
								continue; // no name defined
						
						  tag = 0x6000 + (m2m_table[x].tag & 0x0FFF); // tag contains id but here we use fmt 6
						  buf[0] = tag>>8;
						  buf[1] = tag;
						  sar_append(h,buf,2);
						  len = strlen(m2m_table[x].name);
						  sar_append(h,(const UInt8 *)m2m_table[x].name,1+len); // copy trailing zero also
					}
					sar_close(h);
					break;
				
		}
}

void m2m_event_ignition_on(void *dummy)
{
	m2m_bulk_event(1);
}
void m2m_event_ignition_off(void *dummy)
{
	m2m_bulk_event(2);
}
#if 0 // TODO later maybe
void m2m_event_door_open(void *dummy)
{
	m2m_bulk_event(3);
}
void m2m_event_door_closed(void *dummy)
{
	m2m_bulk_event(4);
}
#endif



void m2m_init(void)
{
	cssos_init();
	honk_init();
	headlight_init();
	tire_pressure_init();
}
////////////////////////////////////////////////////


void bus_status_init (void)
{
  
	static const Cmd_Array1 cmd_array[] = 
    {
        {INPUT_GWR_ON 						, bus_status_notify_input_gwr_on				}, 
    	{INPUT_GWR_OFF				  		, bus_status_notify_input_gwr_off				}, 
    	{INPUT_BRAKE_STATUS_PRESS 			, bus_status_notify_brake_on  	    			}, 
    	{INPUT_BRAKE_STATUS_RELEASE    		, bus_status_notify_brake_off      				}, 
    	{INPUT_PRK_NEUTRAL_STATUS_ON  		, bus_status_notify_prk_neutral_active    	 	}, 
    	{INPUT_PRK_NEUTRAL_STATUS_OFF	 	, bus_status_notify_prk_neutral_not_active  	}, 
    	{INPUT_HANDBRAKE_STATUS_ON	 		, bus_status_notify_handbrake_on    			}, 
    	{INPUT_HANDBRAKE_STATUS_OFF	 		, bus_status_notify_handbrake_off   			}, 
    	{INPUT_HOOD_STATUS_OPEN	 			, bus_status_notify_hood_open 					}, 
    	{INPUT_HOOD_STATUS_CLOSE	 		, bus_status_notify_hood_close		 			}, 
    	{INPUT_TRUNK_STATUS_OPEN	 		, bus_status_notify_trunk_open		 			}, 
    	{INPUT_TRUNK_STATUS_CLOSE	 		, bus_status_notify_trunk_close 	 			}, 
    	{INPUT_ALL_DOOR_STATUS_OPEN 		, bus_status_notify_other_door_open 			}, 
     	{INPUT_ALL_DOOR_STATUS_CLOSE 	    , bus_status_notify_other_door_close			}, 
    	{INPUT_GLOWPLUG_STATUS_ON 			, bus_status_notify_glowplug_on				  	}, 
    	{INPUT_GLOWPLUG_STATUS_OFF 			, bus_status_notify_glowplug_off		  		}, 
    	{INPUT_KEY_IN_STATUS_ON 			, bus_status_notify_key_in				  		}, 
    	{INPUT_KEY_IN_STATUS_OFF 			, bus_status_notify_key_out	  					}, 
    	{INPUT_IGNITION 					, bus_status_notify_input_ignition_off	  		}, 
    	{INPUT_IGNITION_ON 		            , bus_status_notify_input_ignition_on			}, 
    	{INPUT_LOCK1 			| INPUT_ON  , bus_status_notify_input_lock					}, 
    	{INPUT_LOCK2 			| INPUT_ON  , bus_status_notify_input_lock					}, 
    	{INPUT_LOCK3 			| INPUT_ON  , bus_status_notify_input_lock					}, 
    	{INPUT_UNLOCK_DRIVER1	| INPUT_ON 	, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_DRIVER2	| INPUT_ON 	, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_DRIVER3	| INPUT_ON 	, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_ALL1 	| INPUT_ON 		, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_ALL2	| INPUT_ON 		, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_ALL3	| INPUT_ON 		, bus_status_notify_input_unlock	    		}, 
    	{INPUT_OEM_LOCK 					, bus_status_notify_input_keyless_lock			}, 
    	{INPUT_OEM_UNLOCK 					, bus_status_notify_input_keyless_unlock		}, 
    	{INPUT_OEM_ARM 						, bus_status_notify_input_keyless_arm			}, 
    	{INPUT_OEM_DISARM 					, bus_status_notify_input_keyless_disarm		}, 
    	{INPUT_OEM_TRUNK 					, bus_status_notify_input_keyless_trunk	  		}, 
    	{INPUT_OEM_PANIC 					, bus_status_notify_input_keyless_panic	  		}, 
    	{INPUT_OEM_AUX1 					, bus_status_notify_input_keyless_aux1	  		}, 
    	{INPUT_OEM_AUX2 					, bus_status_notify_input_keyless_aux2	  		}, 
    	{INPUT_OEM_AUX3 					, bus_status_notify_input_keyless_aux3	  		}, 
    	{INPUT_OEM_AUX4 					, bus_status_notify_input_keyless_aux4	  		}, 
    	{INPUT_OEM_RS_ON 					, bus_status_notify_input_keyless_start			}, 
    	{INPUT_OEM_RS_OFF					, bus_status_notify_input_keyless_stop  	  	}, 
    	{INPUT_EIPS_ALERT					, bus_status_notify_eips_alert					}, 
    	{INPUT_EIPS_SHUTDOWN         	    , bus_status_notify_eips_shutdown    			}, 
    	{INPUT_EIPS_SHUTDOWN_FAIL_ON       	, bus_status_notify_eips_shutdown_fail			},
    	{ INPUT_HEADLIGHT_STATUS_ON         , bus_status_notify_headlight_on                },
		{ INPUT_HEADLIGHT_STATUS_OFF        , bus_status_notify_headlight_off               },
		{ INPUT_TAMPER | INPUT_ON           , bus_status_notify_tamper_open }, // alarm
		{ INPUT_TAMPER                      , bus_status_notify_tamper_close }, // normal again


		// M2M only
		 { INPUT_OIL_ALARM,  bus_status_oil_low_alarm },  // dash light for oil
		 { INPUT_TIRE_ALARM, bus_status_tire_low_alarm },		 // dash light for tire pressure
		 { INPUT_SEAT_BELTS, bus_status_seat_belts },  // each 1 in get_seat_belts() is buckled
		 { INPUT_SEAT_WEIGHTS, bus_status_seat_weights }, // each 1 in get_seat_weights() is occupied
	   { INPUT_EV_BATTERY_LVL, bus_status_ev_battery_lvl },
		 { INPUT_EV_RANGE,   bus_status_ev_range },
		 { INPUT_OIL_LIFE,   bus_status_oil_life },
		 { INPUT_FUEL_LVL,   bus_status_fuel_lvl },
		 { INPUT_FUEL_CAP,   bus_status_fuel_cap },
		 { INPUT_CABIN_TEMP, bus_status_cabin_temp },
		 { INPUT_TIRE_PRESSURE, bus_status_tire_pressure },
		 { INPUT_ODOMETER,    bus_status_odometer },
		 { INPUT_BATTERY_VOLTAGE, bus_status_battery_voltage },
		 { INPUT_SPEED_SET,       bus_status_speed }, // need 2 of these because INPUT_SPEED is split for eips and doorlock
		 { INPUT_SPEED_CLR,       bus_status_speed }, // need 2 of these because INPUT_SPEED is split for eips and doorlock
     { INPUT_IGNITION_ON, m2m_event_ignition_on },
     { INPUT_IGNITION_OFF, m2m_event_ignition_off },
#if 0 // TODO later maybe		 
     { INPUT_OTHER_OPEN, m2m_event_door_open },
     { INPUT_OTHER_CLOSE, m2m_event_door_close },
#endif		 
		 { INPUT_M2M_DEFERRED, m2m_deferred_handler },
		 { INPUT_M2M_BULK_HANDLER, m2m_bulk_handler },
    };                                                                                       

    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL|EXEC_MODE_PROG);

    d2d_delay_timeout = timeout_f_create(D2D_TIMEOUT , bus_status_notify_brake_d2d_delay) ;

		m2m_init();
}


