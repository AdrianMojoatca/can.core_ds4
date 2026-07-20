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
#include "d2d.h"
#include "d2d2.h"
#include "d2d_valet.h"
#include "evt.h"
#include "bootservice.h"
#include "nvram.h"
#include "bitfield.h"
#include "sensor_proto.h" 

#include "about.h"
#include "dbg.h"
#include "dei_system_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define AID_SCAN_CMD  0xFF
#define AID_CLEAR_CMD 0x00

#define SC_CMD_LEN_NORMAL       4    //pid, header, cmd, pld
#define SC_CMD_LEN_MAX          (2*SC_CMD_LEN_NORMAL)
#define SC_FEEDBACK_LEN_NORMAL  4
#define SC_FEEDBACK_LEN_MAX     10

#define SENS_CMD_LEN_NORMAL     5

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

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static int req_dtc_index=0xFF;
UInt8 d2d_sc_cmd_header;

#define APP_FLASH_SIZE            256 * 1024 //0x00040000
extern  unsigned Image$$ER_ABOUT_FLASH$$Base                    ;
#define APP_FLASH_BASE_ADDR  ((void *) (& Image$$ER_ABOUT_FLASH$$Base))  
#define about_for_bootloader ((char *)APP_FLASH_BASE_ADDR)

UInt8 fw_init_is_done=0;
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

void d2d_lut_service (UInt8 d2d_code)
{
    D2D_Lut_Entry * d2d_lut_entry = d2d_lut_search (d2d_code) ;

    if (d2d_lut_entry)
    {
        if (d2d_lut_entry->func)
            d2d_lut_entry->func () ;
    }       
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

SInt16 d2d_rx(TskTimeout   timeout)
{
    return (req_dtc_index);
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
        os_tsk_wait(20);  //TODO: verify the switch is full
    
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

    proto_sendmsg(PROTO_D2D, d2d2_rftd_carrier, sizeof(d2d2_rftd_carrier));
}

//--------------------------------------------------------------------------//
void d2d_adaptor_scpp(UInt8 *p_cmd, UInt8 len)
{
    dei_scpp_cmd(p_cmd, len);
    if(p_cmd[2] != 0xEA)  //do not override the header byte by button command
        d2d_sc_cmd_header = p_cmd[1];
}

//--------------------------------------------------------------------------//
void d2d2_adaptor_ngss(UInt8 len, UInt8 *p_cmd )
{
    memcpy(&d2d_sensor_cmd, p_cmd, SENS_CMD_LEN_NORMAL);

    sensor_proto_cmd(d2d_sensor_cmd.aid, d2d_sensor_cmd.cmd, d2d_sensor_cmd.chanl, (len-5), (p_cmd+5));
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

		if( dei_system_type != SYS_TYPE_RS )
			d2d_fdback[0] |= 1<<2;		// 5x or 3x
	

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

    if(len != 3)  //info reponse from other device
    return;

    memcpy(my_info_content, default_info_content, 1 + INFO_SIZE);

    sw_ver[1] = about_hw_info() & 0xFF;
    sw_ver[0] = 0;
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
    
    D2D_Entry intl_port_d2d1_entry;

    len = smsg_getlen(msg);
    pkt = smsg_getptr(msg);

    if(len >= 3)
    {
        rxprotoid = pkt[1];
        if(rxprotoid & PROTO_AUTH)
        {
            if(auth_verify(pkt,len))
            {
                len -= AUTH_SIZE;
            }
            else
            {
                if(failing_units)
                {
                  smsg_free(msg);
                  os_evt_signal( intl_port_evt , INTL_EVT_RX_AUTHFAIL ) ;
                }
                else
                {
                  //fail auth cmd from stranger
                    pkt[2] = pkt[0];
                    pkt[0] = AID_MASTER;
                    pkt[1] = PROTO_FAIL;
                    smsg_setlen(msg, 3);
                    smsg_enq(&port->txpending, msg);
                    os_evt_signal( intl_port_evt , INTL_EVT_RX_MSG ) ;
                }
                return;
            }
        }

        switch(rxprotoid & ~PROTO_AUTH)
        {
        case PROTO_D2D:
            intl_port_d2d1_entry.code = pkt[2];
            d2d_convert(&intl_port_d2d1_entry);
            if(intl_port_d2d1_entry.input_code != INPUT_INVALID)
            {
                cmd_q_insert((Cmd)intl_port_d2d1_entry.input_code, D2D_Q_TIMEOUT);  //?blocking
            }
            else
            {
                if(pkt[2] == REQ_DTC_CODE)
                    req_dtc_index = pkt[3];

                d2d_lut_service (intl_port_d2d1_entry.code) ;
            }

            break;
        case PROTO_SCPP:
            if( fw_init_is_done )
            {
              if((rxprotoid & PROTO_AUTH) || pkt[3]==0xA0 || pkt[3]==0xEA || pkt[3]==0xD6)   //allow DEI_SC_TYPE_2B_SN/LOCK even without auth
              {
              //pass command payload with variable len (current: max 4, case of EA command)
                  len--;
                  if(len <= SC_CMD_LEN_MAX)  //most sc command hasn't pld, except 0xE0/E2/E4 and (0xEA + btn + timestamp(2 bytes))
                  {
                      memcpy(sc_cmd_msg, pkt + 1, len); //ignore aid
                      d2d_adaptor_scpp(sc_cmd_msg, len);
                  }
              }
            }
            break;
        case PROTO_SENS:
            d2d2_adaptor_ngss(len, pkt);
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

static UInt8 last_status_sent;

void mainp_status(UInt8 b)
{
      last_status_sent = b;
    proto_sendmsg(PROTO_MAINP, &b, 1);
}


extern Irq dbg_irq_rx ;
UInt8 failing_units = 0;

void intl_port_task(void)
{
    UInt8 *pkt;
    UInt8 len;
    UInt8 evt;
    Smsg  msg;
    UInt8 i;
    
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
                        if(aid_db[i].status & (1<<AID_STATUS_BIT_KEY))
                          nvfs_rd_at(unit_name(i),UUID_SIZE,key,KEY_SIZE);
                        else
                            memcpy(key,broadcastkey,KEY_SIZE);
                        buf[0]=aid_db[i].id;
                        memcpy(buf+1,broadcastkey,KEY_SIZE);                        
                        aes_decrypt(key,buf+1,buf+1);
                        memcpy(buf+1+KEY_SIZE,master_prev_auth, AUTH_SIZE);
                        proto_sendmsg(PROTO_FAIL + PROTO_AUTH,buf,sizeof(buf));
                        
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
                  proto_sendmsg(pkt[1], pkt+2, len-2);
                else
                {
                  if((pkt[1] & ~PROTO_AUTH) != PROTO_AID)
                      aid_antiprocess(pkt[0]);    //make sure this ID won't be assigned to anyone else
                  
                  switch(pkt[1] & ~PROTO_AUTH)
                  {
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
                      case PROTO_MAINP:
                          if((get_ss_arm() || get_ss_lock())) 
                          {
                              //TODO: take care pairing, erase all, delete tach, and zap later
                              mainp_status(0x00); //TODO: No installer allowed
                              break;
                          }
                          
                          if((pkt[2] & 0xF) != 0xF)   //non maintenance
                          {
                              switch(pkt[2] & 0xF)
                              {
                                default:
                                  if(last_status_sent < 0x80 && (pkt[2] >= 0x80))
                                  {
                                    if((pkt[1] & PROTO_AUTH) == 0 && have_devices)
                                    {
                                        //TODO: handle the case of BLE is not a registered device but we have other devices
                                        mainp_status(0x00);        //refuse if no auth
                                        break;
                                    }
                                  }
                                  if(have_devices == 0)
                                      pkt[2] |= 0x80;
                                  
                                  mainp_status(pkt[2]);
                                  break;
                              }
                              break;
                          }
                          //switch to maintenance
                          pkt[2] = 0xEB;  //so we can fall through
                          
                      case PROTO_INTL:
                          if(pkt[2] == 0xEB)//get detection from any port
                          {
                              //mainp_status(0xF0); //BLE card will inform locally
                              intl_port_intl_feedback(0xD8);
                              os_tsk_wait(10);
                              ATOMIC
                              (
                                  nvram_usr_commit();
                                  BootReset(0);
                              );
                          }
                          break;
                  }
                }
                
                smsg_free(msg);
            }
      }
    };
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
}

