/*!
 * @file   intl_port_handler.c
 * @brief
 * @author  DIRECTED ELECTRONICS
 * @date   7/20/2015
 */

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "intl_port_private.h"
#include "aid_dhp.h"
#include "cmd.h"
#include "rf.h"

#include "crc.h"
#include "dei_sc.h"         //set scpp_cmd

#include "status_system.h"  //set_temp_sensor_present/value
#include "config_d2d.h"
#include "d2d1.h"
#include "d2d.h"
#include "d2d2.h"
#include "d2d_valet.h"
#include "evt.h"
#include "bootservice.h"
#include "nvram.h"
#include "bitfield.h"
#include "sensor_proto.h"

#include "about.h"
#include <string.h>

#include "nvfs_usr.h"
#include "system_type.h"

#include "system_ui.h"
#include "dei_system.h"
#include "display.h"
#if USE_TIMESTAMP == 1
#warning "For troubleshooting only. Disable in production release!"
#endif
#include "timeout_f.h"
#include "vrtc.h"
#include "notify.h"
#include "prg_out.h"
#include "dei_siren.h"
#include "bus_status.h"

#include "timestamp.h"
#include "status.h"
#include "dei_prk.h"

#include "prg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define AID_SCAN_CMD  0xFF
#define AID_CLEAR_CMD 0x00

#define SC_CMD_LEN_NORMAL       4    //pid, header, cmd, pld
#define SC_CMD_LEN_MAX          (2*SC_CMD_LEN_NORMAL)
#define SC_FEEDBACK_LEN_NORMAL  4
#define SC_FEEDBACK_LEN_MAX     10


#define INFO_SIZE               23     //maximum handle by web is 50; max 31 on switch
static const UInt8 default_info_content[1 + INFO_SIZE] = {
    0,      // [0] src
    0,      // [1] hardware version or MSB number binary
    0,      //     hardware revision or LSB number binary
    0,      // [3] software version or MSB number binary
    0,      //     software revision or LSB number binary
    'D',    // [5] firmware name
    'E','I','-','N', 'G', 'M', 'M',     // optional additional printable descriptive text
    0,      // [13]
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0       // [23]
};

UInt8 my_info_content[1 + INFO_SIZE]; // first byte is current aid

#define BLE_INFO_SIZE_MAX   26      // maximum size
//    0,      // serial number from uuid hopefully
//    0,      // hardware version or MSB number binary
//    0,      // hardware revision or LSB number binary
//    0,      // software version or MSB number binary
//    0,      // software revision or LSB number binary
//    "whatever text string"      // optional additional printable descriptive text
//    0                         // string terminator needed only if something else might follow
//};
UInt8 ble_info_content[4 + BLE_INFO_SIZE_MAX];   //ble_card_sn + info


//FIO
#define FIOHDR_DATA   0x00
#define FIOHDR_LENGTH 0x10
#define FIOHDR_NAME   0x20
#define FIOHDR_OFFSET 0x30
#define FIO_ABORT (FIOHDR_DATA + 2)
#define FIO_ERROR (FIOHDR_LENGTH + 2)
#define FIOHDR_MASK    0x70

#define FIOCMD 0x80
#define FIOCMD_ABORT (FIOCMD + 2)
#define FIOCMD_REPEAT (FIOCMD + 4)
#define FIOCMD_MASK    0x0E

#define FIOCMD_MOREDATA_BIT   0x01

#define FIOCMD_DIR (FIOCMD + FIOHDR_NAME + 2)
#define FIOCMD_WRITE (FIOCMD + FIOHDR_OFFSET + 2)
#define FIOCMD_READ (FIOCMD + FIOHDR_OFFSET + 4)
#define FIOCMD_INFO (FIOCMD + FIOHDR_OFFSET + 6)
#define FIOCMD_DELETE (FIOCMD + FIOHDR_OFFSET + 8)

#define FIOERR_NONE       0
#define FIOERR_GENERAL    1
#define FIOERR_CONGESTION 2
//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void alert_key2go_ready(void);
static void pke_sm(void);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern UInt8 dbg_never;

extern Boolean d2d_needs_auth;
extern Boolean block_external_controllers;


UInt8 d2d_sc_cmd_header;

#define APP_FLASH_SIZE            256 * 1024 //0x00040000
extern  unsigned Image$$ER_ABOUT_FLASH$$Base                    ;
#define APP_FLASH_BASE_ADDR  ((void *) (& Image$$ER_ABOUT_FLASH$$Base))
#define about_for_bootloader ((char *)APP_FLASH_BASE_ADDR)

UInt8 fw_init_is_done=0;
System_Type ngmm_system_type;
#define WIRELESS_SCAN_TIMEOUT    (4000)  //4 seconds
Timeout intl_port_scanning_timer;
#if USE_TIMESTAMP == 1
Timeout intl_port_warning_timer;
#endif

UInt32 ble_was_detected __attribute__((at(0x20083FFC)));
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

D2D_Lut_Entry * d2d_lut_search (UInt8 d2d_code)
{
    CRITICAL ;

    C_ENTER ;
    {
        D2D_Lut_Entry * curr ;

        for (curr = d2d_lut_list ; curr ; curr = curr->next)
        {
            if (curr->code == d2d_code)
                return curr ;
        }
    }
    C_LEAVE ;

    return 0 ;
}


void d2d_lut_insert (UInt8 d2d_code , D2D_Lut_Func func)
{
    CRITICAL ;

    C_ENTER ;
    {
        D2D_Lut_Entry * d2d_lut_entry = d2d_lut_search (d2d_code) ;

        if (!d2d_lut_entry)
        {
            d2d_lut_entry = (D2D_Lut_Entry *) os_mem_request (sizeof (D2D_Lut_Entry)) ;

            d2d_lut_entry->code = d2d_code ;
            d2d_lut_entry->next = d2d_lut_list ;

            d2d_lut_list = d2d_lut_entry ;
        }

        d2d_lut_entry->func = func ;
    }
    C_LEAVE ;
}

Boolean d2d_lut_service (UInt8 d2d_code)
{
    D2D_Lut_Entry * d2d_lut_entry = d2d_lut_search (d2d_code) ;

    if (d2d_lut_entry)
    {
        if (d2d_lut_entry->func)
        {
            d2d_lut_entry->func () ;
            return TRUE;
        }
    }
    return FALSE;
}

Boolean d2d_get_mask(UInt8 code)
{
    return (bitfield_rd(d2d_ena_table.d2d_mask , code , code)) ;
}

Boolean d2d_get_ena(UInt8 code)
{
    return (bitfield_rd(d2d_ena_table.d2d_ena , code , code)) ;
}

void d2d_convert(D2D_Entry * d2d_entry)
{
  UInt8 index = 0 ;

  d2d_entry -> input_code = INPUT_INVALID ;

  do
    {
      if(input_table [index].code == d2d_entry -> code)
        {
            d2d_entry -> nbyte      = input_table [index].nbyte      ;

            d2d_entry -> input_code = input_table [index].input_code  ;

         return;
       }
    }
  while (++index < INPUT_NUM_FUNC ) ;
}

Boolean d2d_cmd_enabled(UInt8 cmd)
{
   Boolean ret = FALSE;
   #define MASK bitfield_rd(d2d_ena_table.d2d_mask , cmd , cmd)
   #define ENA  bitfield_rd(d2d_ena_table.d2d_ena  , cmd , cmd)
   if(MASK & ENA)
       ret = TRUE;

   #undef MASK
   #undef ENA

   return ret;
}


UInt8 *d2d_rx_pkt;
UInt8 d2d_rx_len;
UInt8 d2d_rx_ofs;

SInt16 d2d_rx(TskTimeout   timeout)
{
    if(d2d_rx_ofs < d2d_rx_len)
      return d2d_rx_pkt[d2d_rx_ofs++];

    return -1;
}


//-----------------------------------------------------//

UInt8 master_prev_auth[sizeof(UInt32)] = { 0,0,0,0 };
/**
 *Function Name:proto_sendmsg(UInt8 *pdata, UInt 8 plen)
 *Parameters:
 *Description:
 *Returns:
*/
void proto_sendmsg(UInt8 proto,const UInt8 *p_data, UInt8 plen)
{
    Smsg txmsg;
    UInt8 *ptr;

    while((txmsg = smsg_alloc(0)) == 0)
    {
        intl_port_is_waiting = 1;
        os_tsk_wait(20);  //TODO: verify the switch is full
    }
    intl_port_is_waiting = 0;

    ptr = smsg_getptr(txmsg);
    ptr[0] = AID_MASTER;
    ptr[1] = proto;
    memcpy(ptr+2, p_data, plen);
    plen += 2;

    if(proto & PROTO_AUTH) // use auth when sending?
    {
        plen+=AUTH_SIZE; // append 4 bytes
        auth_generate(broadcastkey, ptr, plen, master_prev_auth);       // write auth in last 4 bytes,
    }

    smsg_setlen(txmsg, plen);
    smsg_submit(txmsg);
}

int proto_sendmsg_wouldblock(void)
{
	return smsg_is_lowmemory();
}

/**
 *Function Name:build_feedback_frame(D2d2_Port *port, UInt16 input_code)
 *Parameters:
 *Description:
 *Returns:
*/
static void build_feedback_frame(Intl_Port *port, UInt16 input_code)
{
    Smsg txmsg;
    Smsg *ptr;

    rf_sound_set((Rf_Sound_Def)0, FALSE);
    rf_icon_clr(ICON_RS_PANIC, FALSE);
    rf_icon_clr(ICON_PANIC, FALSE);
    rf_icon_clr(ICON_VALET, FALSE);
    rf_icon_clr(ICON_STARTED, FALSE);
    rf_icon_clr(ICON_SUCCESS, FALSE);
    rf_icon_clr(ICON_NOT_READY, FALSE);
    rf_icon_clr(ICON_TIME, FALSE);
    rf_icon_clr(ICON_TRUNK, FALSE);
    rf_icon_clr(ICON_DOOR, FALSE);
    rf_icon_clr(ICON_HOOD, FALSE);
    rf_icon_clr(ICON_IGNITION, FALSE);
    rf_icon_clr(ICON_SHOCK, FALSE);
    rf_icon_clr(ICON_HORN, FALSE);
    rf_icon_clr(ICON_WARN, FALSE);
    rf_icon_clr(ICON_PARKING, FALSE);

    switch(input_code)
    {
    case (INPUT_LOCK1 | INPUT_ON):
        rf_icon_set(ICON_LOCK, TRUE);

        break;
    case (INPUT_UNLOCK_DRIVER1 | INPUT_ON):
        rf_icon_clr(ICON_LOCK, TRUE);

        break;
    case (INPUT_START | INPUT_ON):
        rf_icon_set(ICON_SUCCESS, TRUE);

        break;
    case (INPUT_STOP | INPUT_ON):
        rf_icon_clr(ICON_SUCCESS, TRUE);

        break;
    }

    proto_sendmsg(proto_d2d_out(), d2d2_rftd_carrier, sizeof(d2d2_rftd_carrier));
}

//--------------------------------------------------------------------------//
void d2d_adaptor_scpp(UInt8 *p_cmd, UInt8 len)
{
    dei_scpp_cmd(p_cmd, len);
    if(p_cmd[2] != 0xEA)  //do not override the header byte by button command
        d2d_sc_cmd_header = p_cmd[1];
}



//--------------------------------------------------------------------------//

void intl_port_sensor_proto_req( UInt8 *payload, UInt8 size)
{
    proto_sendmsg(PROTO_SENS, payload, size);
}
//--------------------------------------------------------------------------//

void intl_port_scpp_feedback(UInt8 hhu_no, UInt8 *payload, UInt8 plen)
{
    UInt8 d2d_fdback[32];

    d2d_fdback[0] = d2d_sc_cmd_header;
    if(hhu_no == 0xFF)  //broadcast from dei_sc
      d2d_fdback[0] = 0xC0; //traffic and broadcast bits
    else if(hhu_no & (1<<3)) // non-FF hhu_no has bit 3 set?
      d2d_fdback[0] |= (1<<3); // set STANDBYMASK
    else
      d2d_fdback[0] &= ~(1<<3); // clr STANDBYMASK

    if( (ngmm_system_type & SYS_TYPE_SS) == SYS_TYPE_SS )
      d2d_fdback[0] |= 1<<2;    // 5x or 3x
    else
      d2d_fdback[0] &= ~(1<<2);


    memcpy(d2d_fdback+1, payload, plen);
#if 0       //test long message pass through the IVU
        if(d2d_fdback[1] == 0xAD)
        {
              d2d_fdback[D2D_SCPP_TX_LEN+18] = 0x74; //'t'
              d2d_fdback[D2D_SCPP_TX_LEN+19] = 0x65; //'e'
              d2d_fdback[D2D_SCPP_TX_LEN+20] = 0x73; //'s'
              d2d_fdback[D2D_SCPP_TX_LEN+21] = 0x74; //'t'

              proto_sendmsg(PROTO_SCPP, d2d_fdback, D2D_SCPP_TX_LEN+22);
        }
        else
#endif

    proto_sendmsg(PROTO_SCPP + PROTO_AUTH, d2d_fdback, plen+1);   //SCPP msg must be authenticated
}

void intl_port_intl_feedback(UInt8 payload)
{
    proto_sendmsg(PROTO_INTL, &payload, 1);
}

#if USE_TIMESTAMP == 1
//proto timestamp
void timestamp_send(void)
{
  UInt8 buf[5];
  UInt32 x;

  buf[0] = TMSTAMP_FMT_U32SEC;
  x = get_vrtc_sys_tick();
  buf[1] = x >> 24;
  buf[2] = x >> 16;
  buf[3] = x >> 8;
  buf[4] = x >> 0;
  proto_sendmsg(PROTO_TIME, buf, 5);
}

void timestamp_warning(void)
{
  timestamp_send();
  timeout_start(intl_port_warning_timer);
}


void intl_port_wake(Wake_Status wake_status)
{
  if(wake_status == s_WAKE)
    timestamp_send();
}

void intl_port_shutdown(void)
{
  wake_set_timeout(50);
}

#endif

//proto_fio
UInt8 fio_rxbuf[160]; // big buffer for concatenating multiple packets into one message
unsigned fio_rxlen=0;
unsigned fio_ofs=0;
unsigned fio_client;

UInt8 fio_getc(void)
{
  if(fio_ofs < fio_rxlen)
    return fio_rxbuf[fio_ofs++];
  return 0;
}

void fio_putc(UInt8 c)
{
  if(fio_ofs < sizeof(fio_rxbuf)-1)
    fio_rxbuf[fio_ofs++] = c;
}


unsigned fio_getv(void)
{
unsigned x;
UInt8 b;

  x = 0;
  do
  {
    x <<= 7;
    b = fio_getc();
    x |= (b & 0x7F);
  } while(b & 0x80);
  return x;
}

void fio_putv(unsigned x)
{
  UInt8 b, n;

  if(x < 0x80)
    n = 7;
  else if(x < 0x4000)
    n = 14;
  else if(x < 0x200000)
    n = 21;
  else if(x < 0x10000000)
    n = 28;
  else
    n = 35;

  do
  {
    n -= 7;
    b = (x >> n) & 0x7F;

    if(n)
      b |= 0x80;

    fio_putc(b);
  } while(n);
}

void fio_putv_prefix(unsigned x)
{
  UInt8 b;
  b = 0;
  do
  {
    fio_rxbuf[--fio_ofs] = b | (x & 0x7F);
    b = 0x80;
    x >>= 7;
  } while(x);
}


// hypothetical api assumes ptr and len include any relevant headers
void fio_sendmsg(UInt8 dst,UInt8 opcode,UInt8 *ptr, unsigned len)
{
UInt8 buf[28];
UInt8 amt;

  opcode &= ~FIOCMD_MOREDATA_BIT; // assume this is the last buffer

  while(len)
  {
    amt = len;
    if(amt >= 26)
       amt = 26;

    len -= amt; // reduce len for next time (if needed)
    if(len)
      opcode |= FIOCMD_MOREDATA_BIT; // more will follow

    buf[0] = dst;
    buf[1] = opcode;
    memcpy(buf+2,ptr,amt);
    ptr += amt;

    proto_sendmsg(PROTO_FIO,buf,amt+2);

    opcode &= FIOCMD; // preserve direction only
  }
}

void fio_senderr(UInt8 err)
{
  fio_ofs = 0;
  fio_putv(err);
  fio_sendmsg(fio_client, FIO_ERROR, fio_rxbuf, fio_ofs); // message too long
}


// hypothetical receive (one message at a time) receive process
void fio_process(UInt8 *pkt,unsigned len)
{
  UInt8 opcode;
  UInt8 result;
  char *name = NULL;
  UInt16 user_len = 0;
  UInt8  user_ofs = 0;

  if(len < 4 || pkt[2] != my_id)
    return;

  opcode = pkt[3];
  if(opcode & 0x3E)
  {// the start of something new?
    if( fio_rxlen            // message in progress?
       && pkt[0]==fio_client  // from same user?
       && (fio_rxbuf[0] & ~1) == FIOHDR_NAME // message was a Name header
       && (opcode & ~1) == FIOHDR_NAME //continuation is a name?
      )
    {// continuation of Name list... keep appending
    }
    else
    {
      if(fio_client)
      {
        fio_senderr(FIOERR_CONGESTION);
      }
      fio_client = pkt[0]; // forget whatever we were doing before

      fio_rxbuf[0] = opcode; // save first opcode (minus continuation bit)
      fio_rxlen = 1;         // we have 1 byte so far
    }
  }
  else
  {// continuation packet
    if(fio_client != pkt[0])
      return; // ignore this
  }
  len -= 4;
  if(fio_rxlen + len >= sizeof(fio_rxbuf))
  {
    fio_senderr(0x42);
    fio_client = 0; // forget sender
    return;
  }
  memcpy(fio_rxbuf + fio_rxlen,pkt+4,len); // append data
  fio_rxlen += len; // increase size
  if(opcode & FIOCMD_MOREDATA_BIT)
    return; // more will come

  // we have a complete message
  fio_rxbuf[fio_rxlen] = 0; // force zero beyond end of message, just in case

  fio_ofs = 1; // start after opcode

  // extract parameters
  switch(fio_rxbuf[0] & FIOHDR_MASK)
  {
  case FIOHDR_OFFSET:
    user_ofs = (UInt8)fio_getv(); //user_offset
    // fall through
  case FIOHDR_NAME:
    user_len = (UInt16)fio_getv();  //user_len
    name =(char *)(fio_rxbuf + fio_ofs);
    fio_ofs += strlen(name)+1;
    break;
  case FIOHDR_LENGTH:
    user_len = (UInt16)fio_getv();
    break;
  }

  switch(fio_rxbuf[0] & ~FIOCMD_MOREDATA_BIT)
  {
  case FIOCMD_WRITE:
//TODO:
#if 0
    if(nvfs_rd_at(name,fio_ofs,NULL,len) != len)
    {// error in read

      nvfs_delete(name);
      nvfs_wr_create(name,NULL,user_ofs+len);
    }
    // should be safe to read, try to write
    if(nvfs_wr_at(name,user_ofs,fio_rxbuf + fio_ofs,len) == len)
    {// success
      fio_senderr(FIOERR_NONE);
    }
    else
    {
      fio_senderr(FIOERR_GENERAL);
    }
#endif
    break;

  case FIOCMD_READ:
//    if(len==0)
//    {
//      fio_ofs = 0;
//      nvfs_rd_size(name, &user_len);
//      len = user_len;
//    }
    result = 0;
    if(user_len <= sizeof(fio_rxbuf)-5)  //5 is worst case of fio_putv
    {
      if(nvfs_get_attributes(name, &opcode) == NVFS_OK  && (opcode & NVFS_ATTRIB_HIDDEN) == 0)  //don't show hidden files
      {
        result = (nvfs_rd_at(name,user_ofs,fio_rxbuf+5,user_len));
        if(result)
        {
          fio_ofs = 5;
          fio_putv_prefix(result);
          fio_sendmsg(fio_client, FIOHDR_LENGTH, fio_rxbuf+fio_ofs, result+5-fio_ofs);
        }
      }
    }
    if(result == 0)
    {// utter fail
      fio_senderr(FIOERR_GENERAL);
    }
  }
  fio_rxlen = 0; // msg is now done
  fio_client = 0; // not from anyone
}

//end of proto_fio


#define BUILD_UNLOCKED  //bootloader

static Boolean app_is_valid(void)
{
  Id_Entry id ;

#ifdef BUILD_UNLOCKED
  UInt8 * ptr = (UInt8 *)(0x8100);
#else
  UInt8 * ptr = (UInt8 *)((APP_FLASH_SIZE) - sizeof_array(id.buff)) ;
#endif

  UInt8 i ;

  for (i = 0 ; i < sizeof_array(id.buff) ; i++ )
  {
    if(ptr[i] == 0xFF)
      {
        return(FALSE);
      }
  }

  return TRUE ;
}

static UInt8 get_about_info(char * info_text, UInt8 about_item)
{
    const char * text ;
    UInt8      info_len=0;

    if(app_is_valid())
    {
      UInt8 item_count = 0     ;
      int n ;

      text  = about_for_bootloader ;

      for (n = 0 ; n < ABOUT_SIZE ; n++)
      {
        if(item_count == about_item)
        {
            do
            {
              *info_text++ = *text++ ;
              info_len++;
            }while(*text);

          break ;
        }

        if((*text == 0) || (*text == (char)0xFF))
        {
          item_count++ ;
        }

        text++ ;
      }
    }

    return info_len;
}


void finish_ble_info_scan(void)
{
UInt16 x;

  if(ble_card_aid == 0 || memcmp(ble_info_content, ble_card_sn,4)!=0)  // no ble info found?
  {
    if(nvfs_rd_size(nvfs_ble_info,&x)==NVFS_OK)  // file exists?
      nvfs_delete(nvfs_ble_info);  // delete the file
  }

}
/**
* @brief  set info of the firmware
* @param  N/A
* @retval N/A
*/
static void info_process(UInt8 *pkt, UInt8 len)
{
    UInt8 sw_ver[6];
    UInt16 ver_tmp=0;
    char  req_info[256];
    UInt8 req_info_len=0;
    UInt8 info_len;
    UInt16 ble_sw_ver_size=0;
    UInt8 hw_id=0;

    if(len > 3)  //info reponse from other device
    {
      len -= 3;

        if(pkt[0] == ble_card_aid)
        {
          if(nvfs_rd_size(nvfs_ble_info, &ble_sw_ver_size) == NVFS_OK)
          {
            if(ble_sw_ver_size > sizeof(ble_info_content))
              ble_sw_ver_size = sizeof(ble_info_content);

            if(nvfs_rd_s(nvfs_ble_info,ble_info_content,ble_sw_ver_size) == NVFS_OK)
            {
              if(  memcmp(ble_info_content,ble_card_sn,4) == 0
                && memcmp(ble_info_content+4,pkt+3,len) == 0
                )
              {
                return;  // BLE_INFO is still good
              }
            }
            nvfs_delete(nvfs_ble_info);
            // fall through
          }

          if(len > sizeof(ble_info_content)-4)  // BLE_INFO_SIZE_MAX)
            len = sizeof(ble_info_content)-4;  // take only what we can hold

          memcpy(ble_info_content, ble_card_sn, 4); // always 4 bytes
          memcpy(ble_info_content+4, pkt+3, len);    //grep ble_info_content
          nvfs_wr_create(nvfs_ble_info, ble_info_content, 4+len);
        }

        return;
    }

    if(pkt[0] == AID_MASTER)  //I am asking slave's info
      return;

    memcpy(my_info_content, default_info_content, 1 + INFO_SIZE);

    if(nvfs_rd_size(nvfs_hw_id, &ble_sw_ver_size) ==  NVFS_OK)
      nvfs_rd_s(nvfs_hw_id, &hw_id, sizeof(hw_id));

    sw_ver[1] = about_hw_info();
    sw_ver[0] = hw_id;
    memcpy(my_info_content+1, sw_ver, 2);   //LSB of hardware
        info_len = 2;
    req_info_len = get_about_info(req_info, ABOUT_VER);   //firmware version
    if(req_info_len)
    {
      ver_tmp = (req_info[0] - '0')*100;
      ver_tmp += (((req_info[2] - '0')*10) + (req_info[3] - '0'));
      sw_ver[1] = ver_tmp & 0xFF;
      sw_ver[0] = (ver_tmp >> 8) & 0xFF;
      memcpy(my_info_content+3, sw_ver, 2);
            info_len += 2;

      if(req_info_len > 4 && req_info_len <= 15)   //firmware ver + core ver (2.29080
      {
                  ver_tmp = req_info_len - 4;
          memcpy(sw_ver, req_info+4, ver_tmp);
      }
            else
                ver_tmp = 0;
    }

    req_info_len = get_about_info(req_info, ABOUT_NAME);   //firmware name
    memcpy(req_info + req_info_len, sw_ver, ver_tmp);          //append core
        ver_tmp += req_info_len;
        if(ver_tmp > ((1 + INFO_SIZE) - 5))
            ver_tmp = 1 + INFO_SIZE - 5;
    memcpy(my_info_content+5, req_info, ver_tmp);
        info_len += ver_tmp;

        req_info_len = get_about_info(req_info, ABOUT_INFO);
        if(req_info_len > (1 + INFO_SIZE - 5 - ver_tmp))   //make sure there is no overlap
            req_info_len = 1 + INFO_SIZE - 5 - ver_tmp;
        memcpy(my_info_content+5+ver_tmp, req_info, req_info_len);
        info_len += req_info_len;

    my_info_content[0] = pkt[0];
    proto_sendmsg(PROTO_INFO, my_info_content, sizeof(my_info_content));
}

//-----------------------------------------------//
// support BLE siren
//  DS4 uses PROTO_FUNC send warning/trigger message to BLE siren
enum
{
   FUNC_PLD_F0 = 0,
   FUNC_PLD_F1,
   FUNC_PLD_F2,
   FUNC_PLD_F4,
   FUNC_PLD_F8,
   FUNC_PLD_FF,

   FUNC_PLD_NUM
};

#define FUNC_REP_CMD1 0xF0
#define FUNC_REP_CMD2 0xF1
#define FUNC_REP_CMD3 0xF2
#define FUNC_REP_CMD4 0xF4
#define FUNC_REP_CMD5 0xF8
#define FUNC_REP_CMD6 0xFF

enum
{
  CHANNEL_GLOBAL  = 0,
  CHANNEL_SHOCK      ,
  CHANNEL_TILT       ,
  CHANNEL_TEMP       ,

  CHANNEL_NUM
};
//FIXME: define zones to channel enum for IO_STATE in BLE

#define ZONE_STATUS_NORMAL  (1<<0)
#define ZONE_STATUS_WARNING (1<<1)
#define ZONE_STATUS_TRIGGER (1<<2)

UInt8 ds4_func2ble_payload_build(UInt8 pld_no, UInt8 aid_dst, UInt8 *bytes, UInt8 bytes_len, UInt8 *pld)
{
  UInt8 sdata_len;
  Boolean ret_val=FALSE;
  UInt8 i;

  sdata_len = 0;
  if(pld_no < FUNC_PLD_NUM)
  {
    pld[sdata_len++] = aid_dst;
    switch(pld_no)
    {
      case FUNC_PLD_F1:
        pld[sdata_len++] = FUNC_REP_CMD2;
        break;
      case FUNC_PLD_F2:
        pld[sdata_len++] = FUNC_REP_CMD3;
        break;
      case FUNC_PLD_F0:
      default:
        pld[sdata_len++] = bytes[0];
        pld[sdata_len++] = FUNC_REP_CMD1;
        for(i = 1; i < bytes_len; i++)
          pld[sdata_len++] = bytes[i];
        break;
    }
  }

  return sdata_len;
}

void d2d2_update_siren_to_ble(UInt8 num, UInt8 delay)
{
  UInt8 buf[16];
  UInt8 len;
  UInt8 aid_dst;
  UInt8 pld_bytes[4];
  UInt8 pld_len;
  UInt8 warning_channel;
  UInt8 chan_stat, stat_pld;

  aid_dst = ble_card_aid;
  pld_len = 0;
  pld_bytes[pld_len++] = CHANNEL_SHOCK; //FIXME, definition with sensors (1, 2, 3 for shock, tilt, temp) and zones. Currently test with shock only
//  if(num == 8)  //trigger
//    pld_bytes[pld_len++] = ZONE_STATUS_TRIGGER;
//  else if(num == 9)  //warning
//    pld_bytes[pld_len++] = ZONE_STATUS_WARNING;
//  else
//    pld_bytes[pld_len++] = ZONE_STATUS_NORMAL;

  pld_bytes[pld_len++] = num;
  pld_bytes[pld_len++] = delay;

  len = ds4_func2ble_payload_build(FUNC_PLD_F0, aid_dst, pld_bytes, pld_len, buf);
  if(len)
  {
    proto_sendmsg(PROTO_FUNC, buf, len);
  }
}

#define MAX_FUNCREG 16
UInt16 funcreg[MAX_FUNCREG];
UInt8 funcreg_count = 0;


void d2d2_func_send(UInt8 funcid)
{
UInt8 pkt[5];
UInt8 i,len;
UInt8 p;

  for(i=0;i<funcreg_count;i++)
  {
    if( (funcreg[i]>>8) == funcid
      && (aid_db[(funcreg[i]>>4) & 0xF].status & (1<<AID_STATUS_BIT_CONNECTED))
      )
    { // found someone registered who is alive
      pkt[0] = aid_db[(funcreg[i]>>4) & 0xF].id; // to this aid
      pkt[1] = funcreg[i] & 0xF; // to this channel
      pkt[2] = FUNC_PROTO_WRITE_STAT; // write this value????
      pkt[3] = prg_out_shadow_read(funcid);

      len = 4;
      if(funcid == 1+PRG_OUT_SIREN)
      {// special case for siren
        pkt[4] = prg_out_timeout;  // use whatever value prg_out decided for siren
        len = 5;
      }
      p = PROTO_FUNC;
      if(funcid == 1+PRG_OUT_FOB)
        p |= PROTO_AUTH;
      proto_sendmsg(p,pkt,len);

    }
  }
}

// delete functions associated with unit number
void d2d2_func_delete_unit(UInt8 u)
{
UInt8 i,j;

  if(u && u<UNIT_MAX)
  {
    j=0; // nothing kept so far
    for(i=0; i<funcreg_count; i++)
    {
      if(((funcreg[i]>>4) & 0xF) != u)  // this registration made by some other unit?
      {
        funcreg[j++] = funcreg[i];  // we keep this one.
      }
    }
    funcreg_count = j; // whatever was not kept... is now gone.
  }
}

#define MULTICAST_FUNC 0xFF

void d2d2_func_multicast_send(UInt8 funcid)
{
UInt8 pkt[5];
UInt8 i,len;

  for(i=0;i<funcreg_count;i++)
  {
    if( (funcreg[i]>>8) == funcid
      && (aid_db[(funcreg[i]>>4) & 0xF].status & (1<<AID_STATUS_BIT_CONNECTED))
      )
    { // found someone registered who is alive
      pkt[0] = MULTICAST_FUNC; // to this aid
      pkt[1] = funcid; // to this channel
      pkt[2] = FUNC_PROTO_WRITE_STAT; // write this value????
      pkt[3] = prg_out_shadow_read(funcid);

      len = 4;
      if(funcid == 1+PRG_OUT_SIREN)
      {// special case for siren
        pkt[4] = prg_out_timeout;  // use whatever value prg_out decided for siren
        len = 5;
      }
      proto_sendmsg(PROTO_FUNC,pkt,len);
      break;
    }
  }
}


void d2d2_send_remap(UInt8 unit, UInt8 chan, UInt8 funcid)
{
  UInt8 pkt[6];
  UInt8 i,len;

  pkt[0] = aid_db[unit].id; // to this aid
  pkt[1] = chan & 0xF; // to this channel
  pkt[2] = FUNC_PROTO_MULTICAST_REMAP; // write this value????
  pkt[3] = funcid;
  pkt[4] = prg_out_shadow_read(funcid);

  len = 5;
  if(funcid == 1+PRG_OUT_SIREN)
  {// special case for siren
    pkt[5] = prg_out_timeout;  // use whatever value prg_out decided for siren
    len = 6;
  }

  proto_sendmsg(PROTO_FUNC,pkt,len);

}

void d2d2_func_wrconfig(UInt8 unit, UInt8 chan, UInt8 funcid)
{
  UInt8 pkt[6];
  UInt8 v;

  if(funcid == 1+PRG_OUT_FOB)
  {
    v = dei_feature_get(DEI_FEAT_FOB_VOLTAGE);
    if(v==0)
      return;
    v = (v-1)*0x18; // multiply by 1.5 (fixed point x4)

    pkt[0] = aid_db[unit].id; // to this aid
    pkt[1] = chan & 0xF; // to this channel
    pkt[2] = FUNC_PROTO_WRITE_CONFIG_REQ; // write config
    pkt[3] = v;

    proto_sendmsg(PROTO_FUNC+PROTO_AUTH,pkt,4);
  }
}


void d2d2_func_handler(UInt8 *pkt, UInt8 len)
{
UInt8 u,f,i;
UInt8 chan;
	

  if(pkt[2] != AID_MASTER)
    return; // ignore if not addressed to me

  u = aid_lookup(pkt[0]); // who sent this?
  if(u >= UNIT_MAX)
    return; // I don't recognize this id
  chan = pkt[3];
  if(chan > 15)
    return; // I don't like channel numbers this big

  switch(pkt[4])
  {
  case FUNC_PROTO_FUNC_TYPE_RESP: // message contains an id string?
    for(i=0;i<funcreg_count;i++)
    {
      if((funcreg[i] & 0xFF) == ((u<<4) | chan))
			{
				if(funcreg[i] >> 8) // was there a function defined previously?
					prg_out_user_adj(funcreg[i]>>8, -1); // remove 1 user for this previous function
				
        break; // found a registration entry for this unit/channel
			}
    }

    f = prg_out_funcname_lookup((char *)pkt+6);
    if(f)
    {// register to this function

      if(i == funcreg_count)
        funcreg_count++;  // a new entry at end

      if(funcreg_count >= MAX_FUNCREG)//slots maxed out
      {
        funcreg_count--;
        f = 0;
      }
      else
      {
			  prg_out_user_adj(f,+1); // increment users for this function
				
        funcreg[i] = (f<<8) | (u<<4) | chan; // now registered for this function
      }


      d2d2_func_wrconfig(u,chan,f); // always send write config before sending value

#ifdef MULTICAST_FUNC_ENABLE
      if(f != 1+PRG_OUT_FOB)
        d2d2_send_remap(u, chan, f);  // send the current value of this function to all registered devices
      else
#endif
        d2d2_func_send(f);  // send the current value of this function to all registered devices
    }
    else
    {// function name does not exist.  delete any pre-existing entry for this unit/channel
			
      if(i < funcreg_count)
      {
        while(i<funcreg_count)
        {
          funcreg[i] = funcreg[i+1];
          i++;
        }
        funcreg_count--; // one less function registration entry
      }
    }
    break;
  }

}

//-----------------------------------------------//

static Cmd timed_release_command = INPUT_INVALID;
static Timeout timed_release_timer;

static void timed_release_timeout(void)
{
  if(timed_release_command)
  {
    cmd_q_insert(timed_release_command, D2D_Q_TIMEOUT);  //?blocking
    timed_release_command = INPUT_INVALID;
  }
}

#define FAIL_DBG_MSG_LEN_MAX   16    // AUTH_FAIL debug message
typedef struct
{
  UInt8 len;
  UInt8 data[FAIL_DBG_MSG_LEN_MAX];
} Fail_Dbg_Msg;
Fail_Dbg_Msg fail_dbg_msg1;
Fail_Dbg_Msg fail_dbg_msg2;

void save_critical_dbg_msg(UInt8 who)
{
  fail_dbg_msg1.data[fail_dbg_msg1.len] = who;
  fail_dbg_msg1.len += 1;
  if(fail_dbg_msg1.len < (FAIL_DBG_MSG_LEN_MAX -1))   // don't overwrite
    nvfs_wr(nvfs_dbg_msg1, &fail_dbg_msg1);
}

void save_normal_fail_dbg_msg(UInt8 who)
{
  fail_dbg_msg2.data[fail_dbg_msg2.len] = who;
  fail_dbg_msg2.len += 1;
  if(fail_dbg_msg2.len < (FAIL_DBG_MSG_LEN_MAX -1))   // don't overwrite
    nvfs_wr(nvfs_dbg_msg2, &fail_dbg_msg2);
}


void diag_send(Intl_Port *port,UInt8 reason,Smsg msg)
{
UInt8 *pkt;
UInt8 len,i;

  pkt = smsg_getptr(msg);
  len = smsg_getlen(msg);
  if(pkt[1] == PROTO_DIAG)
  {
    smsg_free(msg);
    return; // don't send diag response to diag
  }

  if(len>8)
    len=8; // dont return more than 8 bytes from original packet

  for(i=len;--i;)
    pkt[i+3] = pkt[i];  // copy upto 7 bytes but not sender id

  pkt[3] = reason;
  pkt[2] = pkt[0]; // sender id goes here
  pkt[1] = PROTO_DIAG;
  pkt[0] = AID_MASTER;

  smsg_setlen(msg,len+3);
  smsg_enq(&port->txpending, msg);
  os_evt_signal( intl_port_evt , INTL_EVT_RX_MSG ) ;  // send diag feedback for trace
}

Boolean origin_check(Intl_Port *port,Smsg msg)
{
  if(block_external_controllers)
  {
    if(smsg_getport(msg) != UART_PORT0) // BLE 3.3V port
    {
      diag_send(port,DIAG_ORIGIN,msg);
      return FALSE;
    }
  }
  return TRUE;
}


extern UInt8 aid_scan_in_progress;

/**
* @brief  receiving msg from switch, if d2d command, call d2d
*         adaptor, if scpp command, pass to scpp_cmd
* @param
* @return
*/
void intl_port_rx(Intl_Port *port, Smsg msg)
{
    UInt8 len;
    UInt8 *pkt;
    UInt8 rxprotoid;
    UInt8 sc_cmd_msg[SC_CMD_LEN_MAX];
    UInt8 i;

    D2D_Entry intl_port_d2d1_entry;

    len = smsg_getlen(msg);
    pkt = smsg_getptr(msg);


    if(len >= 3)
    {
        rxprotoid = pkt[1];
        if(rxprotoid & PROTO_AUTH)
        {
            if(len > AUTH_SIZE && auth_verify(pkt,len)) // NCC-DIRE002-020 fix
            {
                len -= AUTH_SIZE;
            }
            else // auth fail
            {
                i = aid_lookup(pkt[0]);
                if(i>0 && failing_units)
                {
                  smsg_free(msg);
                  os_evt_signal( intl_port_evt , INTL_EVT_RX_AUTHFAIL ) ;
                }
                else
                {
//#define DEBUG_SKLIST  // DEBUG ONLY: turn off for production!
#ifdef DEBUG_SKLIST
                  if(pkt[1] == PROTO_AID + PROTO_AUTH && pkt[2] < UNIT_MAX && len==3+4)
                  {// DEBUG ONLY!
                    memset(pkt+3,0xFF,KEY_SIZE);
                    nvfs_rd_at(unit_name(pkt[2]),UUID_SIZE,pkt+3,KEY_SIZE); // read key
                    memset(pkt+3+KEY_SIZE,0,4);
                    pkt[3+KEY_SIZE] = pkt[2]; // return unit number
                    pkt[2] = pkt[0]; // back to sender
                    pkt[1] = PROTO_FAIL + PROTO_AUTH;
                    pkt[0] = AID_MASTER;

                    smsg_setlen(msg,3+KEY_SIZE+4);
                    smsg_enq(&port->txpending, msg);
                    os_evt_signal( intl_port_evt , INTL_EVT_RX_MSG ) ;  // send special FAIL to sender with key
                    return;
                  }
#endif

                  {// this is the RIGHT thing to do normally
                    pkt[2] = pkt[0];
                    pkt[0] = AID_MASTER;
                    pkt[1] = PROTO_FAIL;
                    smsg_setlen(msg, 3);
                    smsg_enq(&port->txpending, msg);
                    os_evt_signal( intl_port_evt , INTL_EVT_RX_MSG ) ;
                  }
                }
                return;
            }
        }

        switch(rxprotoid & ~PROTO_AUTH)
        {
        case PROTO_D2D:
            if(!origin_check(port,msg))
              return;

            if(d2d_needs_auth && rxprotoid != PROTO_D2D+PROTO_AUTH)
            {
              diag_send(port,DIAG_NEEDAUTH,msg);
              return; // sorry, auth is needed
            }

            intl_port_d2d1_entry.code = pkt[2];

            d2d_rx_pkt = pkt; // set this up in case...
            d2d_rx_len = len; // .. a d2d_lut_service client calls for extra rx bytes
            d2d_rx_ofs = 3;   // next offset to read

            if(! d2d_lut_service (intl_port_d2d1_entry.code)) // look for registered special-case interceptors first
            {
              d2d_convert(&intl_port_d2d1_entry); // convert vanilla d2d code to cmd input
              if(intl_port_d2d1_entry.input_code != INPUT_INVALID)
              {
                  cmd_q_insert((Cmd)intl_port_d2d1_entry.input_code, D2D_Q_TIMEOUT);  //?blocking
                  if(intl_port_d2d1_entry.input_code & INPUT_RF_EXT)
                  {
                    timed_release_command = (Cmd)((intl_port_d2d1_entry.input_code & ~INPUT_RF_EXT) | INPUT_RF_RELEASE);
                    timeout_start(timed_release_timer);
                  }
                  else if(intl_port_d2d1_entry.input_code == timed_release_command)
                  {
                    timed_release_command = INPUT_INVALID;
                    timeout_stop(timed_release_timer);
                  }
                  set_cmd_source_d2d(TRUE);
              }
            }
            break;
        case PROTO_SCPP:
            if(pkt[3] != 0xEA)
            {
              if(!origin_check(port,msg))
                return;
            }


              if(  ((rxprotoid & PROTO_AUTH) && !dei_system_prg_mode_get() && fw_init_is_done)
                 || ((pkt[3]==0xA0 || pkt[3]==0xE5) && dei_system_prg_mode_get())
                 || pkt[3]==0xEA || pkt[3]==0xD6)   //allow DEI_SC_TYPE_2B_SN/LOCK even without auth
              {
              //pass command payload with variable len (current: max 4, case of EA command)
                  len--;
                  if(len <= SC_CMD_LEN_MAX)  //most sc command hasn't pld, except 0xE0/E2/E4 and (0xEA + btn + timestamp(2 bytes))
                  {
                      memcpy(sc_cmd_msg, pkt + 1, len); //ignore aid
                      d2d_adaptor_scpp(sc_cmd_msg, len);
                  }
              }
              else
              {
                diag_send(port,DIAG_NEEDAUTH,msg);
                return; // sorry, auth is needed
              }
            break;

    case PROTO_M2M:
      // prevent action on restricted messages
      if(is_m2m_restricted(pkt,len) && !origin_check(port,msg))
        return;

      {
        if(!origin_check(port,msg))
          return;
      }

      d2d2_m2m_handler(pkt, len); //  otherwise everything else should be safe
        break;

        default:
            smsg_enq(&port->txpending, msg);
            os_evt_signal( intl_port_evt , INTL_EVT_RX_MSG ) ;

            return;    //do not free the message, internal port task will free it
        }
    }
    smsg_free(msg);
}

void dbg_char(UInt8 c)
{
  dbg_rx_buf[dbg_rx_nw++ & (DBGCLI_BUF_SIZE-1)] = c;
}


UInt8   mainp_last_staus_get(void)
{
  return last_status_sent;
}

static UInt8 installer_is_connected = 0;

Boolean is_stopped_driving(void)
{
	return(!!status_get_trans(STATUS_TRANS_PARK) || !get_ign_status()); 
}

void trans_change_event(void)
{
	// send legacy d2d park/neutral advisory
	if(status_get_trans(STATUS_TRANS_PARK) || status_get_trans(STATUS_TRANS_NEUTRAL))
	{
		d2d_tx(SET_PRK_NTRL_ACTIVE);
	}
	else
	{
		d2d_tx(SET_PRK_NTRL_NOT_ACTIVE);
	}
	
	// TODO: (oneday) send actual gear position on d2d or m2m
	
	
	if((last_status_sent & 0xF)!=MAINP_DRIVING && !status_get_trans(STATUS_TRANS_PARK))
	{
		mainp_status(MAINP_DRIVING);
	}
	else if((last_status_sent & 0xF)==MAINP_DRIVING && is_stopped_driving())
	{// stopping driving now
		mainp_status(get_ss_arm() ? MAINP_ARMED : MAINP_DISARMED);
	}
	
}

void mainp_status(UInt8 b)
{
  b &= ~0x80; // top level doesn't decide installer-allowed anymore
	if((last_status_sent & 0xF)==MAINP_DRIVING && !is_stopped_driving())
		return; // stays latched until ignition is off
		
  switch(b & 0xF)
  {
    case MAINP_MAINT: //0x0F: // maint
    case 0x01: // pairing
    case 0x02: // erase all
    case 0x03:
    case 0x04:
        b |= 0x80; // installer allowed
        break;
    case MAINP_ARMED: //0x06: // armed
    case MAINP_DRIVING: //0x0D: // driving
        break;     // no-installer allowed
    default: // other (disarmed) states
        if(have_devices == 0 && !system_in_used_status_get())
          b |= 0x80;
        break;
  }
  last_status_sent = b;
  proto_sendmsg(PROTO_MAINP, &b, 1);
}

UInt8 install_conn_hist[8];
UInt8 install_conn_idx=0;

extern Irq dbg_irq_rx ;
UInt8 failing_units = 0;


// change state, but don't send yet
void change_mainp_state(UInt8 newstate)
{
  last_status_sent = (last_status_sent & ~MAINP_STATE_MASK) | (newstate & MAINP_STATE_MASK);
}


// scanning timer
void rewind_scanning_timer(void)
{
  timeout_start(intl_port_scanning_timer);
}

// after timeout, return to armed or disarmed state
void scanning_timeout(void)
{
  change_mainp_state(get_ss_arm() ? MAINP_ARMED : MAINP_DISARMED);
  mainp_status(last_status_sent);
}


////////////// time server (modified from ngmmble)

unsigned timeserv_boot = 0; // (non-zero) the timestamp of my boot, according to the most-recent indication
unsigned timeserv_ind = 0; // the timestamp of the indication I am basing myself on

// sys_seconds() is the free-running number of seconds since boot.
UInt32 sys_seconds(void)
{
  return get_vrtc_sys_tick();
}


void timeserv_set(UInt32 now_seconds)
{
  timeserv_boot = now_seconds - sys_seconds();
  timeserv_ind = now_seconds;
}
UInt32 timeserv_get(void)
{
  return timeserv_boot + sys_seconds();
}
// how old is the information which determined my boot time

UInt32 timeserv_age(void)
{
  if(timeserv_ind)
    return timeserv_get() - timeserv_ind;
  return (UInt32)(-1); // really ancient.  ANYTHING is more recent than this.
}


static void u16be_write(UInt8 *buf,unsigned h)
{
  buf[0] = h>>8;
  buf[1] = (UInt8)h;
}

static void u32be_write(UInt8 *buf,unsigned x)
{
  u16be_write(buf,x>>16);
  u16be_write(buf+2,x & 0xFFFF);
}

unsigned timeserv_indwrite(UInt8 *buf) // note: will still need AA/BB/CC framing and escaping
{
  if(timeserv_ind == 0)
  {
    u32be_write(buf,sys_seconds()); // I only have time since boot
    return 4;
  }

  u32be_write(buf, timeserv_age()); // recency
  u32be_write(buf+4, timeserv_get()); // seconds since epoch
  return 8;
}





static unsigned get1(char *ptr)
{
  return (*ptr - '0') & 0xF;
}

static unsigned get2(char *ptr)
{
  return get1(ptr)*10 + get1(ptr+1);
}

static unsigned get4(char *ptr)
{
  return get2(ptr)*100 + get2(ptr+2);
}

// return days since epoch
static UInt32 get_date(char *ptr)
{
unsigned y,m,d,q;
static UInt8 mday[12] = { 0,1,1,2,2,3,4,4,5,5,6,7 };
  y = get4(ptr) - 2000; // years since epoch

  m = get2(ptr+5) - 3; // convert 1..12 to -2.. 9
  if((int)m < 0) // jan or feb ?
  {
    m += 12; // rename as months 10 and 11...
    y--;     // ... of the previous year
  } // mar..dec are 0..9

  d = 0;
  while(y >= 100)
  {
    y -= 100; // subtract centuries
    d += 36524; // accurate within the first 399 years
  }

  q = y>>2; // how many final quadyears?
  d += q*(4*365+1); // add that many days.
  y -= (q<<2);

  d += y*365; // y is the remainder 0..3
  if(m < 12) // (should be)
    d += m*30 + mday[m];

  return d + get2(ptr+8) - 1;  // 1st of the month is day 0
}

// seconds since midnight
static UInt32 get_time(char *ptr)
{
  return get2(ptr)*3600 + get2(ptr+3)*60 + get2(ptr+6);
}

static unsigned get_u16be(UInt8 *ptr)
{
  return ((unsigned)(ptr[0])<<8) | ptr[1];
}
static unsigned get_u32be(UInt8 *ptr)
{
  return (get_u16be(ptr)<<16) | get_u16be(ptr+2);
}

// process timeind ack payload and update our time server if necessary
int timeserv_indread(UInt8 *ptr,unsigned len)
{
unsigned age,stamp;

  age = get_u32be(ptr);   // get age
  ptr+=sizeof(UInt32);

  if(len >= 4+19 && ptr[4]=='-' && ptr[7]=='-' && ptr[13]==':' && ptr[16]==':')
  {// looks like printable string to me!
    stamp = get_date((char *)ptr); // days since epoch
    ptr+=5+3+3; // skip to time
    stamp = 86400*stamp + get_time((char *)ptr); // seconds since epoch
  }
  else if(len == 4+4)
    stamp = get_u32be(ptr);  // yay binary timestamp, we can process faster
  else
    return 0;  // no time indication here

  // should we update our own server?
  if(timeserv_age() < age) // is my own indication more recent?
    return 0; // don't accept.  Maybe they should use our timestamp instead...

  // this new timestamp was made more recently than ours so we must accept it.
  timeserv_ind = (stamp - age); // this is when the new timestamp was initially created.
  timeserv_boot = stamp - sys_seconds(); // this becomes the new estimate of the time when I booted.
  return 1; // we updated
}




/////////////// end of time server


// accept the diag byte from peripheral
void d2d_diag_process(UInt8 *pkt, UInt8 len)
{
	if (len == 4)  // diag byte 1
	{
		aid_set_diagnostic(pkt[0], pkt[3]);
	}
}

void intl_port_task(void)
{
    UInt8 *pkt;
    UInt8 len;
    UInt8 evt;
    Smsg  msg;
    UInt8 i;
    UInt8 failcount;
	
    UInt8 key[KEY_SIZE];
    UInt8 buf[1+KEY_SIZE+AUTH_SIZE];

    #define  EVT  (INTL_EVT_RX_MSG | INTL_EVT_RX_AUTHFAIL)

    for(;;)
    {
        evt = os_evt_wait(intl_port_evt, EVT, EVT, 0);

        if(evt & INTL_EVT_RX_AUTHFAIL)
        {
            while(failing_units)
            {
                for(i = 1; i< UNIT_MAX; i++)
                {
                    if(failing_units & (1<<i))
                    {
											  failcount = (aid_db[i].status>>AID_STATUS_BIT_FAILCOUNT) & AID_STATUS_FAILMASK;
											  if(failcount < AID_STATUS_FAILMASK)
												{
													failcount++;
													aid_db[i].status = (aid_db[i].status & ~(AID_STATUS_FAILMASK<<AID_STATUS_BIT_FAILCOUNT))
                  													| (failcount<<AID_STATUS_BIT_FAILCOUNT); //
													
                          if((failcount & 1)==0) // 2nd, 4th, or 6th fail for this unit?
                            master_auth_reset(); // send RESET+ in an effort to give "help"
												}

                        // but ALWAYS send a FAIL of one kind or another...
                        buf[0]=aid_db[i].id;
                        if(aid_db[i].status & (1<<AID_STATUS_BIT_KEY))
                        {


                          nvfs_rd_at(unit_name(i),UUID_SIZE,key,KEY_SIZE);

                          memcpy(buf+1,broadcastkey,KEY_SIZE);
                          aes_decrypt(key,buf+1,buf+1);
                          memcpy(buf+1+KEY_SIZE,master_prev_auth, AUTH_SIZE);

                          proto_sendmsg(PROTO_FAIL + PROTO_AUTH,buf,sizeof(buf));

                        }
                        else // I have no key for this...
                        {
                          //if(!(sys_in_low_voltage_mode || aid_scan_in_progress))
                          {
                            proto_sendmsg(PROTO_FAIL + PROTO_AUTH,buf,1);    // in my database, but no key!
                            save_critical_dbg_msg(buf[0]);
                          }
                        }

                        failing_units &= ~(1<<i);
                    }
                }
            }
        }
        if(evt & INTL_EVT_RX_MSG)
        {
            for(;;)
            {
                msg = smsg_deq(&intl_port.txpending);
                if(!msg)
                  break;

                pkt = smsg_getptr(msg);
                len = smsg_getlen(msg);

                if(pkt[0] == AID_MASTER )  //something Master should send
                {
                  if(pkt[1] == PROTO_FAIL && len == 3)
                  {
                    len = pkt[2]; // remember the id we want to ask about
                    //fail auth msg from stranger
                    if(sys_in_low_voltage_mode || aid_scan_in_progress)
                    {
                      pkt[2] = 0;  // don't hurt anyone during low voltage or power up!

                    // THIS IS WRONG, however, if DS4 resets and IVU did not,  and if IVU sends a command with AUTH before responding
                    // to DS4's scan, then DS4 sending short FAIL to otherwise good AUTH will cause IVU to drop pairing (TP#32408, TP#30982,
                    // TP#16310, TP#17657).
                    // Keep this "wrong" code until all IVUs have been updated with fix to make AID claim highest priority.
                       proto_sendmsg(PROTO_FAIL,pkt+2,1); // send short fail to 0  (no-one)

                    }
                    else
                    {
                      proto_sendmsg(PROTO_FAIL,pkt+2,1); // go ahead and hurt someone's feelings
                      save_critical_dbg_msg(pkt[2]);
                    }

                     // in both cases follow this up with a short poll to find out who this is...
                     pkt[1] = PROTO_AID;
                     pkt[2] = len;
                     len = 3;
                  }

                  if(pkt[1] == PROTO_DIAG)
                    len = 4;
                  proto_sendmsg(pkt[1], pkt+2, len-2);        // No matter what PROTO_ ?

                }
                else
                {
                  if((pkt[1] & ~PROTO_AUTH) != PROTO_AID)
                      aid_antiprocess(pkt[0]);    //make sure this ID won't be assigned to anyone else

                  switch(pkt[1] & ~PROTO_AUTH)
                  {
											case PROTO_CMDQ:
													cmdq_process(pkt,len);
											    break;
                      case PROTO_AID:
                          aid_process(pkt, len);
                          break;
                      case PROTO_FAIL:
                          master_fail_process(pkt,len);
                          break;
                      case PROTO_DHP:
                          dhp_process(pkt,len);
                          break;
                      case PROTO_INFO:
                          info_process(pkt, len);
                          break;
                      case PROTO_FIO:
                          fio_process(pkt, len);
                          break;
                    case PROTO_SENS:
                          sens_process(pkt, len);
                        break;
                    case PROTO_FUNC:
                          d2d2_func_handler(pkt, len);
                        break;

                      case PROTO_DBG:
                          if(pkt[2] == AID_MASTER)
                          {
                              if(pkt[3] == 0x03)   //^C?
                              {
                                dbg_dest = 0;
                                break;
                              }

                              dbg_dest = pkt[0];
                              for (i = 3; i < len; i++)
                              {
                                  dbg_char(pkt[i]);
                              }
                              if(dbg_irq_rx)
                                os_irq_set(dbg_irq_rx) ;                // Signal task that data has arrived
                          }
                          break;
											case PROTO_DIAG:
													// set the diag
											    d2d_diag_process(pkt, len);
													break;
                      case PROTO_MAINP:
                          // check for change in installer connected
                          if(installer_is_connected ^ (pkt[2] & 0x80))
                          {
                            installer_is_connected ^= 0x80;
                            if(installer_is_connected)
                            {
                              //installer just conencted now
                              //
                            }
                            else
                            {
                              //installer just disconnected
                              //TODO: cancel sensor adjust
                            }
                          }

                          // handle state change request
                          switch(pkt[2] & MAINP_STATE_MASK) // what is installer asking for?
                          {
                          case MAINP_ARMED:
                          case MAINP_DISARMED:
                            break; // we don't change to these states using mainp

                          case MAINP_MAINT:
                              if((last_status_sent & MAINP_STATE_MASK) == MAINP_ARMED
                              || (last_status_sent & MAINP_STATE_MASK) == MAINP_DRIVING
                              )
                              {
                                break; // no
                              }
                              change_mainp_state(MAINP_MAINT);
                              break; // done processing maint request

                              // asking for system ui states...
                          case MAINP_PAIRING:
                          case MAINP_ERASEALL:
                          case MAINP_CLRTACH:
                          case MAINP_ZAP:
                              if((last_status_sent & MAINP_STATE_MASK) != MAINP_ARMED)
                              {
                                system_ui_prg_via_ble(pkt[2] & MAINP_STATE_MASK);
                                change_mainp_state(pkt[2]);
                              }
                              // else refuse
                              break;

                          case MAINP_SCANNING: // ble card or d2d2 sniffer can ask for this
                            if(
                                 ((last_status_sent & MAINP_STATE_MASK) == MAINP_ARMED)
                                 ? (pkt[1] & PROTO_AUTH) // we require auth to prevent spoofing
                                 : ((last_status_sent & MAINP_STATE_MASK) != MAINP_SCANNING)
                              )
                            {// let's do it
                                change_mainp_state(MAINP_SCANNING);
                                rewind_scanning_timer(); // begin
                            }
                            // else refuse
                            break;
                          }

                          //hard reset request from phone
                          if(pkt[2] == 0xC0)
                          {
                            system_ui_prg_via_ble(pkt[2]);
                            break;
                          }

#if 1    //TODO: check that protection will not allowed getting to Virtual Cluster from body dancing
                          mainp_status(last_status_sent); // send feedback (changed or not)
#endif

                          // are we now going to maintenance mode?
                          if((last_status_sent & MAINP_STATE_MASK) != MAINP_MAINT)
                            break; // no

                          //protect pairing and eraseall and other ui states
                          system_ui_prg_via_ble(0);   //inform all slave exit programming mode safely before switch to maintenance
                          os_tsk_wait(20);   //let D4 msg out

                          //switch to maintenance
                          pkt[2] = 0xEB;  //so we can fall through

                      case PROTO_INTL:
                          if((pkt[2] & 0xFE) == 0xEA )//get detection from any port
                          {
                              //mainp_status(0xF0); //BLE card will inform locally
                              intl_port_intl_feedback(0xD8);
                              os_tsk_wait(10);
                              ATOMIC
                              (
                                  nvram_usr_commit();
                                  BootReset(pkt[2] & 1);   //0: reset, 1: maint
                              );
                          }
                          break;

                  case PROTO_TIME: // copied from ngmmble (but no time_guard flag, though)

                    // IMPORTANT: to avoid network confusion, we should only reply IF OUR TIME IS BETTER than what is given
                    i = 0; // assume we reply
                    if(len > 2 && pkt[2]==0x02) // is there a full timestamp here?
                      i = timeserv_indread(pkt+3,len-3); // if we updated, then this new one was better

                    if(i==0) // if we didn't update it means our own timestamp *should be* better
                    {
                      len = timeserv_indwrite(pkt+3);
                      pkt[2] = len>>2; // 4->0x01,  8->0x02

                      // send our timestamp
                      proto_sendmsg(PROTO_TIME,pkt+2,1+8);

                    }
                    break;
#ifdef PROTO_PARROT  // DEBUG ONLY!
									case PROTO_PARROT:
										if(pkt[2] == AID_MASTER && len > 4 && pkt[3] != PROTO_PARROT) // we can parrot anything except parrot packets!
										{
											proto_sendmsg(pkt[3],pkt+4,len-4);  // note this will add AUTH if parrot protocol indicates auth
										}
										break;
#endif
										

                  }
                }

                smsg_free(msg);
            }
      }
    };
}


void request_ble_info(void)
{
  UInt8 req_msg;
  req_msg = ble_card_aid;
  proto_sendmsg(PROTO_INFO, &req_msg, 1);
}


static void alert_key2go_ready(void)
{
  d2d_tx(AL_K2G_READY);
}

void intl_k2g_status_init(void)
{
  static const Cmd_Array1 cmd_array[] =
  {
    {INPUT_KEY2GO_LOG_DONE, (Exec_Func)alert_key2go_ready },
  };

  CMD_LUT_INSERT1(cmd_array, EXEC_MODE_NORMAL);
}

//BLE PKE logic
enum
{
  DEVICE_ZONE_NULL = 0,   // Imply far away
  DEVICE_ZONE_1,
  DEVICE_ZONE_2,
  DEVICE_ZONE_3,
  DEVICE_ZONE_4,
  DEVICE_ZONE_5,
  DEVICE_ZONE_6,
  DEVICE_ZONE_7,
  DEVICE_ZONE_8,
  DEVICE_ZONE_9,
  DEVICE_ZONE_A,
  DEVICE_ZONE_B,
  DEVICE_ZONE_C,           // Trunk zone, check stability for 5 sec?
  DEVICE_ZONE_D,           // Passenger zone
  DEVICE_ZONE_E,           // Driver zone
  DEVICE_ZONE_MAX          // Imply inside vehicle
};

#define PKE_ZONE_MASK   0x0F
#define PKE_SRC_MASK    0x30     // higher nibble: lower 2 bits stand for number of mobile devices, higher 2 bits are reserved
#define MULTI_BYTE_TIMEOUT 100
#define PKE_TRUNK_ZONE_MONITOR_PERIOD   (VRTC_1SEC*5)  //5 sec. User stays in TRUNK ZONE for amount sec to confirm intention of pop trunk
#define PKE_SUPERVISION_CHECK_PERIOD    (VRTC_1SEC*9)  //mobile device notify zone info every 4 secs now
#define PKE_RELOCK_CHECK_PERIOD         (VRTC_1SEC*8)

UInt8 pke_decision_zone[PKE_USER_MAX] = {0};  //mapping zone_C, D, and E
UInt8 pke_relock_obligation;

Vrtc pke_trunk_zone_mon_timer;
Vrtc pke_supervision_update_timer;
Vrtc pke_relock_check_timer;
void pke_supervision_check(void);
void pke_trunk_zone_verify(void);
void pke_relock_check_timeout(void);

void pke_response_update(void);
void pke_door_event(void);
void pke_brake_event(void);

void pke_module_notify_init(void);
void pke_lock_event(void);
void pke_unlock_event(void);
void pke_rs_event(void);
void ignition_on_event(void);
void ignition_off_event(void);


#define NEW_METHOD
// body dance to enter pairing mode ONLY when we don't have an ivu or valet switch
#define BODY_IGN 1
#define BODY_BRAKE 2
#define BODY_DOOR 3
#define BODY_ON  0x80
const UInt8 body_sequence[]=
{
#ifdef NEW_METHOD
	BODY_IGN+BODY_ON,	                // ignition on		
	BODY_DOOR+BODY_ON, BODY_DOOR,     // door open then close
	BODY_BRAKE+BODY_ON,               // brake on...
	BODY_DOOR+BODY_ON,                // door open...
	BODY_BRAKE                        // brake off
#else
	BODY_IGN+BODY_ON,
	BODY_BRAKE+BODY_ON, BODY_BRAKE,
  BODY_BRAKE+BODY_ON, BODY_BRAKE,
  BODY_BRAKE+BODY_ON, BODY_BRAKE,
	BODY_IGN,
	
	BODY_IGN+BODY_ON,
	BODY_BRAKE+BODY_ON, BODY_BRAKE	
#endif	
};
#define BODY_LEN sizeof(body_sequence)
	
#ifdef NEW_METHOD
#define BODY_TIME_LIMIT 60
#else
#define BODY_TIME_LIMIT 30 //  seconds, from door open to finish the body_sequence
#endif

UInt8 body_buf[BODY_LEN+1];
UInt8 body_idx; // next write index in body_buf
UInt8 body_allowed;
UInt32 body_tstamp;



UInt8 body_prk_level;
struct cssos_timer *body_prk_timer;


void body_prk_timeout(void)
{	
	if(!body_allowed)
		body_prk_level = 0;
	
	switch(body_prk_level)
	{
		case 1:
			dei_prk_flash(DEI_PRK_FLASH1);
			break;
		case 2:
			dei_prk_flash(DEI_PRK_FLASH2);
			break;
		case 3:
			dei_prk_flash(DEI_PRK_FLASH3);
			break;
		case 4:
			dei_prk_flash(DEI_PRK_FLASH4);
			break;
		default:
			body_prk_level = 0; // turn off
		  dei_prk_flash(DEI_PRK_OFF);  // To be verified: normal ignition, brake events affect parking lights?
			return; // don't flash anything
	}
  cssos_timer_rewind(body_prk_timer, RT_MILLI(1500) + body_prk_level*RT_SECOND);
}

void body_prk_set_level(UInt8 level)
{
	body_prk_level = level;
	// start the machine
	if (level) 
	{
	  cssos_timer_rewind(body_prk_timer,RT_MILLI(100 + 2000));  // add 2 more seconds (measured) to let the AUTH_RESET done.
	}
	else
	{
		cssos_timer_rewind(body_prk_timer,RT_MILLI(100));
	}
}


// log the event into body_buf history
void body_evt(UInt8 evt)
{
	if(body_idx < BODY_LEN && (get_rpm() == 0))
		body_buf[body_idx++] = evt;
}

/**
* @brief set the initialization before the call of body_forbid
* @return void
*/
void body_preinit(void)
{
	body_allowed = 1; // assume bodydance for pairing is allowed	
	set_prg_use_door(1);
}

void body_init(void)
{
#ifndef NEW_METHOD
	body_idx = BODY_LEN+2; // it takes door open to begin body dance
#endif	
	
	body_prk_timer = cssos_timer_create(body_prk_timeout);
}

void body_forbid(void) // ivu or valet button present on system
{
	body_allowed = 0;
	set_prg_use_door(0);
}




#define ZONE_HIST_BUF_SIZE   16
UInt8 zone_hist[PKE_USER_MAX][ZONE_HIST_BUF_SIZE];  //history information used to decide the "ZONE 0" (far away)
UInt8 zone_hist_idx=0;
/**
* @brief receive the location information of mobile device(s)
*
* @return void
*/
void pke_zone_info(void)
{
  SInt16 zone_byte=0;
  UInt8 src, zone;

  zone_byte = d2d_rx(MULTI_BYTE_TIMEOUT);

  zone = (UInt8)zone_byte & PKE_ZONE_MASK;
  src = ((UInt8)zone_byte & PKE_SRC_MASK) >>4;
  if(src < PKE_USER_MAX)
  {
    pke_zone[src] = zone;
    zone_hist[src][zone_hist_idx++ & (ZONE_HIST_BUF_SIZE -1)] = zone;
    switch(zone)
    {
      case DEVICE_ZONE_E:
        pke_decision_zone[src] = 1;  //bit number
        break;
      case DEVICE_ZONE_D:
        pke_decision_zone[src] = 2;  //bit number
        break;
      case DEVICE_ZONE_C:
        pke_decision_zone[src] = 3;  //bit number
        break;
      default:
        break;
    }
  }
  pke_sm();
}

void pke_trunk_zone_verify(void)
{
  if(pke_zone_cur == pke_zone_pre
    && pke_zone_cur == 0x04)
  {
    cmd_q_rs_insert(INPUT_RF_TRUNK | INPUT_RF_EXT, 100, 0);
  }
}

static void rewind_supervision_timer(void)
{
    vrtc_stop(pke_supervision_update_timer);
    vrtc_update(pke_supervision_update_timer, PKE_SUPERVISION_CHECK_PERIOD);
    vrtc_start(pke_supervision_update_timer);
}

static void rewind_relock_check_timer(void)
{
    vrtc_stop(pke_relock_check_timer);
    vrtc_update(pke_relock_check_timer, PKE_RELOCK_CHECK_PERIOD);
    vrtc_start(pke_relock_check_timer);
}

void pke_supervision_check(void)
{
    if(pke_state & (1<<PKE_IS_RESPONSIBLE))
    {
      pke_state &= ~(1<<PKE_IS_RESPONSIBLE);
      cmd_q_rs_insert(INPUT_RF_LOCK, 100, 0);
    }
    pke_zone_cur = 0;
    pke_state &= ~(1<<PKE_STATUS_TIMER_RUN);

    if(last_status_sent)
    {
      mainp_status(last_status_sent);
    }
}

void pke_relock_check_timeout(void)
{
    if(pke_state & (1<<PKE_IS_RESPONSIBLE))
    {
      pke_state &= ~(1<<PKE_IS_RESPONSIBLE);
      cmd_q_rs_insert(INPUT_RF_LOCK, 100, 0);
    }
    pke_zone_cur = 0;
    pke_state &= ~(1<<PKE_STATUS_RELOCK_TIMER_RUN);

}

/**
* @brief state machine to handle zone info
*
* @return void
*/
void pke_sm(void)
{
  UInt8 ds4_state=0;
  UInt8 i;

  pke_zone_cur = 0;  //fresh
  for(i = 0; i < PKE_USER_MAX; i++)
  {
    if(pke_decision_zone[i])
    {
      pke_zone_cur |= (1<<(pke_decision_zone[i] - 1));  //bit 2, 1, 0 store ZONE C, D, E (trunk, passenger, driver)
      pke_decision_zone[i] = 0;  //ready for next  //FIXME soon, issue for multiple phones
    }
  }

  mainp_status(last_status_sent);

  rewind_supervision_timer();
  pke_state |= (1<<PKE_STATUS_TIMER_RUN);

  switch(pke_zone_cur)
  {
    case 0x01:    //driver zone
      if(!(pke_state & (1<<PKE_IS_RESPONSIBLE)))
      {
        cmd_q_rs_insert(INPUT_RF_UNLOCK, 100, 0);
        pke_state |= (1<<PKE_IS_RESPONSIBLE);
      }
      pke_relock_obligation = 1;
      break;
    case 0x02:    //passenger zone
    case 0x03:    //driver and passenger by multiple user
      if(!(pke_state & (1<<PKE_IS_RESPONSIBLE)))
      {
        cmd_q_rs_insert(INPUT_RF_UNLOCK, 100, 0);  //TODO: unlock all
        pke_state |= (1<<PKE_IS_RESPONSIBLE);
      }
      pke_relock_obligation = 1;
      break;
    case 0x04:    //trunk zone
      vrtc_stop(pke_trunk_zone_mon_timer);
      vrtc_update(pke_trunk_zone_mon_timer, PKE_TRUNK_ZONE_MONITOR_PERIOD);
      vrtc_start(pke_trunk_zone_mon_timer);
      break;
    default:
      if(pke_relock_obligation)
      {
        pke_relock_obligation = 0;
        rewind_relock_check_timer();
        pke_state |= (1<<PKE_STATUS_RELOCK_TIMER_RUN);
      }
      break;
  }

  if((pke_state & (1<<PKE_IS_RESPONSIBLE))
    &&  pke_relock_obligation)
  {
    pke_relock_obligation = 0;
    rewind_relock_check_timer();
    pke_state |= (1<<PKE_STATUS_RELOCK_TIMER_RUN);
  }

  pke_zone_pre = pke_zone_cur;
}

void pke_response_update(void)
{
  pke_state &= ~(1<<PKE_IS_RESPONSIBLE);   //lock by user already, I am not resp.
}

void pke_door_event(void)
{
  pke_state |= (1<<PKE_IS_RESPONSIBLE);

	mainp_status(last_status_sent);
}

void pke_door_open_event(void)
{
#ifdef NEW_METHOD
	body_evt(BODY_DOOR+BODY_ON);
#else	
	body_idx = 0;
	body_tstamp = time(NULL);
#endif	
	pke_door_event();
}

void pke_door_close_event(void)
{
#ifdef NEW_METHOD
  body_evt(BODY_DOOR);
#endif

#ifndef NEW_METHOD	
	body_idx = BODY_LEN+2; // invalid index and no further input possible
#endif	
	pke_door_event();
	
	if (get_ble_prg_menu())
	{
		system_ui_prg_via_ble(0);		
		body_prk_set_level(0); // stop the prk lights too
	}
}

void pke_brake_event(void)
{
UInt8 newstatus = last_status_sent;
	
	 if(get_ign_status() && !get_rs_gwr() && !get_hood_status() && !status_get_trans(STATUS_TRANS_PARK))
	 {// yes, some ppl drive with trunk open, door open, etc.  hood open is a tougher call...
		  if (get_ble_prg_menu())
		  {
				body_idx = BODY_LEN+2;
				system_ui_prg_via_ble(0);		
				body_prk_set_level(0); // stop the prk lights too
		  }
			else
			{
				newstatus = MAINP_DRIVING;
				pke_state |= (1<<PKE_STATUS_IN_DRIVING);
			}
	 }
	 
#if 0	 
   if(!get_door_status() && !get_hood_status() && !get_trunk_status()
     && get_ign_status() && !get_rs_gwr() && get_rpm()
     )  //zones close, ignition is on, GWR off, rpm > 0
   {   // in driving status
      pke_state |= (1<<PKE_STATUS_IN_DRIVING);

   }
#endif
	 
	 if(newstatus != last_status_sent)
     mainp_status(newstatus );
}

void pke_brake_on_event(void)
{
	body_evt(BODY_BRAKE+BODY_ON);
	pke_brake_event();
}

void pke_brake_off_event(void)
{
	body_evt(BODY_BRAKE);
	
	if(body_allowed
		&& body_idx == BODY_LEN
	  && time(NULL) - body_tstamp < BODY_TIME_LIMIT
	  && memcmp(body_buf,body_sequence,BODY_LEN)==0
	  )
	{
		// is it safe to pair now?
		if( !get_ss_arm() && !get_ss_lock() && !get_rs_gwr() && get_door_status() )
		{
			change_mainp_state(0x81);
			system_ui_prg_via_ble(0x81);
			return;    // brake off event is done
		}		
	}

	pke_brake_event();
	
}


void pke_lock_event(void)
{
//  UInt8 cur_status;
//  cur_status = (last_status_sent & ~MAINP_STATE_MASK);

//  if(get_ign_status())  // lock command is rejected
//  {
//    cur_status &= ~MAINP_ARMED_IGN;     /**< prepare */
//    mainp_status(cur_status | MAINP_ARMED_IGN);
//  }
//  else
  {
    mainp_status((last_status_sent & ~MAINP_STATE_MASK) | MAINP_ARMED);
  }
}

void pke_unlock_event(void)
{
  UInt8 cur_status;
  cur_status = (last_status_sent & ~MAINP_STATE_MASK);

  if(get_ign_status())
  {
    cur_status &= ~MAINP_DISARMED_IGN;
    mainp_status(cur_status | MAINP_DISARMED_IGN);
  }
  else
  {
    mainp_status(cur_status | MAINP_DISARMED);
  }
}

void pke_rs_on_event(void)
{
  mainp_status(last_status_sent);
}

void pke_rs_off_event(void)
{
  mainp_status(last_status_sent);
}

void ignition_on_event(void)
{
  UInt8 cur_status;

#ifdef NEW_METHOD
	body_idx = 0;
	body_tstamp = time(NULL);
#endif	
	body_evt(BODY_IGN+BODY_ON);
	
  cur_status = last_status_sent;
	
//  if(get_ss_arm())      // could not happen, will cause alarm
//  {
//    cur_status &= ~MAINP_ARMED_IGN;     /**< prepare */
//    mainp_status(cur_status | MAINP_ARMED_IGN);
//  }
//  else
  {
    mainp_status(cur_status | MAINP_DISARMED_IGN);
  }
}

void ignition_off_event(void)
{
	body_evt(BODY_IGN);
	
  if(get_ss_arm())
  {
    mainp_status(MAINP_ARMED);
  }
  else
  {
    mainp_status(MAINP_DISARMED);
  }
}

void pke_module_notify_init(void)
{
    static const Notify_Array notify_array[] =
    {
        {NOTIFY_LOCK    ,          (Notify_Func)pke_lock_event         },
        {NOTIFY_FULL_SILENT_ARM,   (Notify_Func)pke_lock_event         },
        {NOTIFY_SENSOR_SILENT_ARM, (Notify_Func)pke_lock_event         },
        {NOTIFY_UNLOCK  ,          (Notify_Func)pke_unlock_event       },
        {NOTIFY_RS_START,          (Notify_Func)pke_rs_on_event        },
        {NOTIFY_RS_STOP,           (Notify_Func)pke_rs_off_event       }
    };

    NOTIFY_ADD(notify_array);
}

// should be called after cmd_init, notify_init (core_common2_init, core_main_933_init in ngmm_task_main)
void pke_module_init(void)
{
    static const Cmd_Array1 cmd_array[] =
    {
       {INPUT_RF_LOCK,               (Exec_Func)pke_response_update},
       {INPUT_OEM_LOCK,              (Exec_Func)pke_response_update},
       {INPUT_ALL_DOOR_STATUS_OPEN,  (Exec_Func)pke_door_open_event},
       {INPUT_ALL_DOOR_STATUS_CLOSE, (Exec_Func)pke_door_close_event},
       {INPUT_BRAKE_STATUS_PRESS,    (Exec_Func)pke_brake_on_event},
       {INPUT_BRAKE_STATUS_RELEASE,  (Exec_Func)pke_brake_off_event},
       {INPUT_IGN_STATUS_ON,         (Exec_Func)ignition_on_event},
       {INPUT_IGN_STATUS_OFF,        (Exec_Func)ignition_off_event},
			 {INPUT_TRANS_CHANGE,          (Exec_Func)trans_change_event}
    };
		
		body_init();

    memset(pke_zone, 0, PKE_USER_MAX);   //default all ZONE_NULL
    zone_hist_idx = 0;
    pke_trunk_zone_mon_timer = vrtc_f_create((Exec_Func)pke_trunk_zone_verify);
    pke_supervision_update_timer = vrtc_f_create((Exec_Func)pke_supervision_check);
    pke_relock_check_timer = vrtc_f_create((Exec_Func)pke_relock_check_timeout);
    pke_state=0;
    pke_relock_obligation=0;

    CMD_LUT_INSERT1(cmd_array, EXEC_MODE_NORMAL);

    pke_module_notify_init();
}

void fail_dbg_msg_nvfs_init(void)
{
  UInt16 size;

  if(nvfs_rd_size(nvfs_dbg_msg1, &size) == NVFS_OK && size <= sizeof(Fail_Dbg_Msg)) // NCC-DIRE002-018
  {
    // update the msg
    nvfs_rd_s(nvfs_dbg_msg1, &fail_dbg_msg1, sizeof(fail_dbg_msg1));
  }
  else   // new
  {
    memset(&fail_dbg_msg1, 0, sizeof(Fail_Dbg_Msg));
    nvfs_wr_create(nvfs_dbg_msg1, &fail_dbg_msg1, sizeof(Fail_Dbg_Msg));
  }

  // dbg_msg2
  if(nvfs_rd_size(nvfs_dbg_msg2, &size) == NVFS_OK && size <= sizeof(Fail_Dbg_Msg))  // NCC-DIRE002-018
  {
    // update the msg
    nvfs_rd_s(nvfs_dbg_msg2, &fail_dbg_msg2, sizeof(fail_dbg_msg2));
  }
  else   // new
  {
    memset(&fail_dbg_msg2, 0, sizeof(Fail_Dbg_Msg));
    nvfs_wr_create(nvfs_dbg_msg2, &fail_dbg_msg2, sizeof(Fail_Dbg_Msg));
  }
}

/**
* @brief
* @param
* @return
*/
void intl_port_init(void)
{
    Intl_Port *port;
    UInt8 i;
    Tsk tsk;


    fail_dbg_msg_nvfs_init();

    timed_release_timer = timeout_f_create(25000,(Exec_Func)timed_release_timeout); // 25 seconds to be longer than HHU  < 23 seconds afrer

    intl_port_evt = os_evt_create();

    port = &intl_port;

    memset(port, 0, sizeof(intl_port));
    port->smsgbase.sendfunc = (VP2A *)intl_port_rx;             //pass tx function
    port->smsgbase.number = smsg_register(0, port);   //0: INTERNAL PORT 1: UNDER_DASH
    port->smsgbase.timeoutfunc = (VP1A *)d2d_led_msg_timeout_handler;
    port->smsgbase.filterfunc = NULL;

    tsk = os_tsk_create(intl_port_task, TASK_INTL_PRI, TASK_INT_STK, 0, TASK_INTL_NAME);
    os_tsk_start(tsk, 0);

    fw_init_is_done = 0;

    if(nvfs_rd_s( nvfs_sys_type , &ngmm_system_type, sizeof(ngmm_system_type) ) != NVFS_OK)
      ngmm_system_type = sys_type_assumed;

    intl_port_scanning_timer = timeout_f_create(WIRELESS_SCAN_TIMEOUT, (Exec_Func)scanning_timeout);

    //init fn_zone_info
    d2d_lut_insert (FN_ZONE_INFO, pke_zone_info) ;

		body_preinit();
		
#if USE_TIMESTAMP == 1
    wake_warning_register((Wake_Func)timestamp_warning);
    intl_port_warning_timer  = timeout_f_create(50 , (Exec_Func)intl_port_shutdown);
    wake_register(intl_port_wake);
#endif
}

