/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ngmm_task_main.c 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "config_gpio.h"
#include "tmr.h"
#include "vrtc_f.h"
#include "cmd.h"
#include "exec.h"
#include "per_exec.h"
#include "out_q.h"
#include "wake.h"
#include "gpio_wake.h"
#include "brand.h"
#include "led.h"
#include "display.h"
#include "notify.h"
#include "voltage.h"
#include "sio_uart.h"
#include "reset.h"
#include "nvram.h"
#include "nvfs.h"
#include "system_type.h"
#include "nvfs_usr.h"
#include "rtc.h"
#include "prog.h"
#include "fob_ctrl.h"

#include "wdog_v.h"

#include "status_req.h"
#include "d2d_switch.h"
#include "d2d1.h"
#include "d2d2.h"
#ifdef DS4_D2D3P_ENABLE
#include "d2d3p.h"
#endif
#include "d2d_valet.h"
#include "intl_port.h"
#include "d2d_valet.h"
#include "aid_dhp.h"
#include "timestamp.h"

#include "rf.h"
#include "config_prog.h"
#include "rftd2.h"


#include "secure_lock.h"
#include "bus_status.h"

#include "rftd2.h"
#include "rf.h"

#include "led_usr.h"
#include "isr.h"

#include "trace.h"
#include "temperature.h"
#include "nvfs_usr.h"
#include "status_system.h"
#include "about.h"
#include "ngmm_feature.h"
#include "system_ui.h"
#include "dei_system.h"
#include "dei_siren.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define UART_PORT_MAX 4  //reserve 1st for internal port
                         //then: uart under/upper dash/dbg_port(NGMM BLE)

#define TEMPSENS_MAX        3       //max value of temperature config

#define HARDWARE_NGBL_VER   7

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void    core_init_reset    ( void );
void    core_common1_init  ( void );
void    core_common2_init  ( void );
void    core_fw_init       ( void );
void    core_main_933_init    ( void );
void    core_933_pre_init( void );
void    core_debug_init    ( void );
Boolean core_nvfs_init     ( void );
Boolean debug_port_avail   ( void );
void    core_common1_933_init      ( void );
static void sort_config_port(UInt8 *conf_port, UInt8 *config_rd);
static Brand_Id ngmm_retrieved_brand_id(void);
static System_Type ngmm_retrieved_sys_type(void);

void fw_init      (void);
void fw_init_reset(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
UInt8 uart_config[3];   //UNDERDASH PORT, UPPERDASH PORT, SIDE PORT
                        //hardware version <  7: uart0, 1, 3
                        //hardware version >= 7: uart3, 1, 0

UInt8 dbg_uart_avail=0;
UInt8 is_validation = 0;
static Vrtc reboot_Det_vrtc;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//please define SIREN_TEST in C/C++ Options tab if you want it.  aid_dhp_handler also needs it.
//please define SKEY_TEST in C/C++ Options tab if you want it.  aid_dhp_handler needs it.

#ifdef SIREN_TEST



static unsigned siren_ticks;
static unsigned siren_tdelta;
// these delays are all in per_exec ticks
#define PER_EXEC_SECOND 100 // 10ms tick
#define RESULT_DELAY  (5*PER_EXEC_SECOND)
#define PAIRING_DELAY (15*PER_EXEC_SECOND)
#define CUTTING_DELAY (10*PER_EXEC_SECOND)
#define CUT_DELAY_MIN (1*PER_EXEC_SECOND)
#define MINIMUM_AUDIO_LENGTH ((PER_EXEC_SECOND * 800)/1000)


#define SIRENTEST_IDLE 0
#define SIRENTEST_RESULT 1 // showing result
#define SIRENTEST_PAIRING 2 // trying to pair
#define SIRENTEST_CUTTING 3 // trying to unpair

static unsigned sirentest_state = SIRENTEST_IDLE;
static UInt8 sirentest_saw_claim = 0;
static UInt32 sirentest_timestamp;


static unsigned now(void)
{
  return siren_ticks;
}

void sirentest_timerwind(unsigned amt)
{
  siren_tdelta = amt;
}


static void sirenred(unsigned led_on)
{
  gpio_pl_pin_write(GPIO_PL_LED_RED, !led_on);
}

static void sirengreen(unsigned led_on)
{
  gpio_pl_pin_write(GPIO_PL_LED_GREEN, !led_on);
}

static void siren12V(unsigned enable)
{
  gpio_pl_pin_write(GPIO_PL_RLY3_CTL, enable);
}

static void mainp(UInt8 cmd)
{
  proto_sendmsg(PROTO_MAINP,&cmd,1);
}

static void fail_ble(void)
{
UInt8 buf = 0xFF; // everybody fail

    proto_sendmsg(PROTO_FAIL,&buf,1);
}

void sirentest_idle(void)
{
    sirentest_state = SIRENTEST_IDLE; // no timer

    // off
    sirenred(0);
    sirengreen(0);
    siren12V(0);
      mainp(0x00);
}


static void sirentest_timeout(void)
{
  if(sirentest_state == SIRENTEST_RESULT || sirentest_state == SIRENTEST_IDLE)
  {
    sirentest_idle();
  }
  else
  {
    sirentest_state = SIRENTEST_RESULT;
    sirentest_timerwind(RESULT_DELAY);

    // red
    sirenred(1);  // fail
    sirengreen(0);

    siren12V(0);
  }
}


void sirentest_button(void)
{
        sirentest_saw_claim = 0;
      sirentest_state = SIRENTEST_PAIRING;
      sirentest_timerwind(PAIRING_DELAY);
      siren12V(1);

      // yellow while test is in progress
      sirenred(1);
      sirengreen(1);

      mainp(0x01); // go to pairing mode
}

void sirentest_claim(UInt8 *pkt,UInt8 len)
{
    sirentest_saw_claim = 1;
}

void sirentest_audio_on(void)
{
    sirentest_timestamp = now();
}

void sirentest_audio_off(void) // end of audio detected
{
static unsigned timecut;

          if((now() - sirentest_timestamp) < MINIMUM_AUDIO_LENGTH)
                return; // ignore small chirps.

      if(sirentest_state == SIRENTEST_PAIRING)  // audio while pairing means key now exists
      {
        sirentest_state = SIRENTEST_CUTTING;

        siren12V(0);
        timecut = now();
        sirentest_timerwind(CUTTING_DELAY);


        fail_ble(); // delete key in blecard

        mainp(0x0E); // dinnerbell
      }
      else if(sirentest_state == SIRENTEST_CUTTING)  // audio while cutting means key is lost
      {
        if((now() - timecut) >= CUT_DELAY_MIN) // real pairing loss
        {
          sirentest_state = SIRENTEST_RESULT;
          sirentest_timerwind(RESULT_DELAY);


                    if(sirentest_saw_claim)
                    {
                        // green means success
                        sirengreen(1);  // test complete
                        sirenred(0);
                        return;
                    }
                    sirengreen(0);  // test complete
                    sirenred(1); // no claim received
        }
      }
}


Per_Exec_Wake sirentest_tick(void)
{
  siren_ticks++;
  if(siren_tdelta && --siren_tdelta==0)
    sirentest_timeout();

  return PER_EXEC_WAKE;
}


void sirentest_init(void)
{
    wake_enter(); // keep awake forever

    per_exec_register((Per_Exec_Func)sirentest_tick);

    cmd_lut_insert(INPUT_HOOD_STATUS_OPEN,(Exec_Func)sirentest_button,EXEC_MODE_NORMAL);
    cmd_lut_insert(INPUT_TRUNK_STATUS_OPEN,(Exec_Func)sirentest_audio_on,EXEC_MODE_NORMAL);
    cmd_lut_insert(INPUT_TRUNK_STATUS_CLOSE,(Exec_Func)sirentest_audio_off,EXEC_MODE_NORMAL);

    sirentest_idle();
}



#endif

void reboot_detection(void)
{
    dei_siren_chirp( DEI_SIREN_VALET_MODE_OFF);
}
/*--------------------------------------------------------------------------*/

void task_main (void * tsk_arg)
{
    UInt8 reboot_cause =0;
// Timer 1
    PCONP |= (1u << 2) ;        // POWER the TIMER

    PCLKSEL0 &= ~(3u << 4) ;
    PCLKSEL0 |=  (1u << 4) ;    // Divide CPU clock by 1


    // Timer 2
    PCONP |= (1u << 22) ;       // POWER the TIMER

    PCLKSEL1 &= ~(3u << 12) ;
    PCLKSEL1 |=  (1u << 12) ;    // Divide CPU clock by 1

    // Timer 3
    PCONP |= (1u << 23) ;       // POWER the TIMER

    PCLKSEL1 &= ~(3u << 14) ;
    PCLKSEL1 |=  (1u << 14) ;    // Divide CPU clock by 1

    timestamp_init();

    reboot_cause = RSIR;        // wdog_v_init clears Reboot cause register. Get it now

    core_common1_init ();//starting main components from common core: wake,nvfs,dbg,wdog_v,rtc

        update_aid_content();

    wake_enter_flagged(WAKE_MAIN_TSK);

    #if DBG_JTAG == 1
        wake_enter_flagged(WAKE_MAIN_TSK);
        #warning "SLEEP DISABLED"
    #endif

    core_init_reset    ();//init all the reset components

    if(!IN_PUSH)
      hold_bus_off = 1;
    reset_process      ();//process the reset
    if(!IN_PUSH)
      hold_bus_off = 1;
    else
      hold_bus_off = 0;

    core_common2_init  ();//starting remaining components from common core

    core_933_pre_init();//starting the core specific components used by firmware(analogical sense)
    d2d_valet_init();   // D2D valet port init with fixed port number.

        // was before  fw_init
        fw_init_is_done = 0;
      system_ui_nvfs_init(); //ORDER is critical to check system_in_used_status!
    led_usr_init    ();

    if(uart_config[0] == D2D_TYPE2 || uart_config[1] == D2D_TYPE2 || uart_config[2] == D2D_TYPE2)
    {
       dhp_powerup();
             if(ble_was_detected != BLE_WAS_DETECTED)
                 os_tsk_wait(1500);     //BLE card will be detected within 200ms, + 1sec of blue LED on
    }

    core_main_933_init();    //init main components

	core_fw_init();       //init firmware
                          //blocking call .WAIT FOR DETECTION DONE
    wake_leave_flagged(WAKE_MAIN_TSK);

    is_validation = 0;
    nvfs_rd_s(nvfs_validation_flag, &is_validation , sizeof( is_validation ));

    // Check reboot Causes RESET pin, Watchdog Reset and  system reset request (Ignored Power On reset, Brown out Detect)
    if (is_validation && (reboot_cause & 0x36))
    {
            reboot_Det_vrtc = vrtc_f_create( (Exec_Func)reboot_detection ) ;
            vrtc_update( reboot_Det_vrtc  , 5 * VRTC_1SEC ); // start timer
            vrtc_start( reboot_Det_vrtc );
    }
	

#ifdef SIREN_TEST

        sirentest_init();

#else    // analog product
#if (DEBUG_TEXT == 1)
      dbgcli_handler(); //trace on D2D2/dbgcli
#else
    os_tsk_stop();
#endif
#endif
}

/*--------------------------------------------------------------------------*/

void core_init_reset( void )
{
    prog_init_reset       () ;
    status_req_init_reset () ;

    rftd2_init_reset () ;

    fw_init_reset();

	//fw_reset();
}

/*--------------------------------------------------------------------------*/

void core_common1_init( void )
{
      tmr_init       ();
    per_exec_init  ();

    wake_init      ();

    core_nvfs_init ();
    core_debug_init(); //Order is critcal: nvfs_init needs TRACE init
    core_common1_933_init  ();
    display_init   ();  //initialized early in case 2sec BLE claim to display BLUE LED

    vrtc_init      (); //start the VRTC because the LIB.WDOG_V is using the VRTC to kick the wdog in IDLE mode
    wdog_v_init    ();

	TRACE("\r\nTMR INIT\r\n");
}

/*--------------------------------------------------------------------------*/

void core_common2_init( void )
{
    TRACE("\n\rStarting common core...\n\r");

    led_init       ();
    //display_init   ();
    gpio_wake_init ();
    voltage_init   (); //starting the ADC
    temperature_init();



    cmd_init     (CMD_BUFFER_MAX_SIZE,CMD_EXEC_Q_MSG_MAX_SIZE);
    exec_init    (EXEC_TSK_NUMBER);

    //register first to be executed last
    wake_register            ( (Wake_Func           )nvram_usr_sleep_commit );//called when the CPU is ready fpr sleep
    wdog_v_register_preamble ( (Wdog_v_Preamble_Func)nvram_usr_commit       );//called when the CPU is restrting by APP


    prog_init       ();
    rf_init         ();
    status_req_init ();
    bus_status_init ();
    secure_lock_init();
}

/*--------------------------------------------------------------------------*/

void core_fw_init( void )
{
	TRACE("\n\rStarting firmware...\n\r");

    fw_init();

    status_req_obdcan_check () ; // display smartstart obd can not active led pattern 

	fw_init_is_done = 1;
}

static Boolean ngmm_defogger_out_is_programmed( void )
{
    UInt8 i ;

    for( i = 0 ; i < DEI_PROG_OUT_COUNT ; i ++ )
    {
        if( (dei_feature_get( DEI_FEAT_OUTPUT_BASE(i) ) == DEI_OUT_PRG_DEFROSTER_LATCH) || ( dei_feature_get( DEI_FEAT_OUTPUT_BASE(i) ) == DEI_OUT_PRG_DEFROSTER_PULSED ) )
        {
            return TRUE ;
        }
    }

    return FALSE ;
}

/*--------------------------------------------------------------------------*/

void core_933_pre_init(void)
{
    System_Type ngmm_sys_type;

      UInt16 size = 0;

      if(nvfs_rd_size(nvfs_hard_reset_req, &size) == NVFS_OK)
        {
            system_ui_prg_via_ble(0xC0);  //reuse the api of BLE hard reset
        }

    if(nvfs_rd_s(nvfs_sys_type , &ngmm_sys_type, sizeof(ngmm_sys_type)) != NVFS_OK)
            ngmm_sys_type = sys_type_assumed;

    vrtc_f_init(VRTC_F_EXEC_Q_MAX_SIZE);   //---VRTC application(timeouts events that can be triggered in sleep mode)

    out_q_create(OUT_Q_MAX_SIZE);          //---main queue of outputs.
    per_exec_register( (Per_Exec_Func)out_q_exec , (Per_Exec_Arg)out_q_main );

    brand_pre_init();

  //build capability
    my_appcap = 0;
    if(ngmm_sys_type & SYS_TYPE_SS)
      my_appcap |= APPCAP_SS;
    if(ngmm_sys_type & SYS_TYPE_RS)
      my_appcap |= APPCAP_RS;
    if(get_gearbox_type() == GEARBOX_MANUAL)
      my_appcap |= APPCAP_MTS;
    if(ngmm_defogger_out_is_programmed())
      my_appcap |= APPCAP_DEFOG;
    if(tempsens_config)
      my_appcap |= APPCAP_TEMP;
    //check for ajustable sensor during scan

    intl_k2g_status_init();
}

/*--------------------------------------------------------------------------*/

void core_main_933_init(void)
{
    TRACE("\n\r\n\rStarting 933 core...\n\r");

    notify_init  ();                       //--- starting the RF notification server
    brand_init   ();                       //    brand init
    pke_module_init();
}

/*--------------------------------------------------------------------------*/

void core_debug_init( void )
{
    dbg_uart_avail = debug_port_avail();  //UART3 is not d2d port?
    dbg_init();    //similar initialization as trace_init
}

static void sort_config_port(UInt8 *conf_port, UInt8 *config_rd)
{
      memcpy(conf_port, config_rd, 3);
      // ALWAYS ASSUME NEW HARDWARE
    //reading: underdash, upperdash, side, default 2, 2, 0
        {
        conf_port[0] = 2;     //config_rd[2];  //side port is always d2d2 for BLE card
        conf_port[2] = config_rd[0];  //underdash port
    }
}

Boolean debug_port_avail(void)
{
    UInt8 uart_config_rd[3];   //reading always: UNDERDASH PORT, UPPERDASH PORT, SIDE PORT
    Boolean ret = FALSE;

    if(nvfs_rd_s(nvfs_config_port, uart_config_rd, sizeof(uart_config_rd)) == NVFS_OK)
    {
              if(uart_config_rd[2] != 0x02)
                    uart_config_rd[2] = 0x02;       //protect the BLE port, it should alwasys be d2d2
        sort_config_port(uart_config, uart_config_rd);
    }
    else
    {
        uart_config_rd[1] = 2;
              // ALWAYS ASSUME NEW HARDWARE
                uart_config_rd[2] = 2;  //side port ==> NGMMBLE
                uart_config_rd[0] = 2;

        nvfs_wr_create(nvfs_config_port, &uart_config_rd, sizeof(uart_config));
        sort_config_port(uart_config, uart_config_rd);
    }

    if(uart_config[2] == 0)
        ret = TRUE;

    if(ret)
      dbg_uart_avail = 1;
    else
      dbg_uart_avail = 0;

    return ret;
}

Boolean core_nvfs_init( void )
{
    volatile Nvram_Status ram_status  ;
    volatile NVFS_Status  nvfs_status ;

    Ram:
    {
        ram_status = nvram_init (FALSE) ;

        if(!(ram_status == NVRAM_OK || ram_status == NVRAM_OK_RECOVERED))
        {
            ram_status = nvram_init (TRUE) ;

            if(!(ram_status == NVRAM_OK || ram_status == NVRAM_OK_RECOVERED))
            {
                return FALSE ;
            }
        }
    }

    Nvfs:
    {
        nvfs_status = nvfs_init (NVFS_MOUNT_DEFAULT) ;

        if(nvfs_status != NVFS_OK)
        {
            nvfs_status = nvfs_init (NVFS_MOUNT_DEFAULT) ;

            if(nvfs_status != NVFS_OK)
                return FALSE ;
        }
    }

    return TRUE ;
}

void core_common1_933_init( void )
{
    UInt8 i;
      System_Type sys_type;
      Brand_Id auth_brand;
      System_Type auth_sys_type;

    volatile NVFS_Status  nvfs_status ;

      sys_type_assumed = SYS_TYPE_NULL;

    gpio_pl_set(GPIO_PL_RF_D2D_EN);         //enable first to have TX

    d2d_switch_init ();     //init switch in advance
    intl_port_init();

    for(i=1;i<UART_PORT_MAX;i++)  //uart under/upper dash/dbg_port(NGMM BLE)
    {
        switch(uart_config[i-1])  //0: reserved for internal port
        {
        case D2D_TYPE1:
              d2d1_init(i);
            break;
        case D2D_TYPE2:
            d2d2_init(i);
            break;
#ifdef DS4_D2D3P_ENABLE
        case D2D_TYPE_3P:
            d2d3p_init(i);
            break;
#endif
        //case D2D_TYPEDBG:    //being init in trace, will be replaced by d2d2 debug protocol
        default:
            break;
        }
    }

    d2d_valet_pre_init();   // D2D valet port init with fixed port number

    gpio_pl_set(GPIO_PL_RF_D2D_EN);         //enable first to have TX

    hold_bus_off = 0;

    d2d_init_nvfs();     //check the default d2d1 configuration(config_d2d.h)

    //end of UART config

    // start temperature sensor config init
    if(nvfs_rd_s(nvfs_tempsens, &tempsens_config, sizeof(tempsens_config)) != NVFS_OK || tempsens_config > TEMPSENS_LAST )
        {
        tempsens_config = TEMPSENS_NONE; // invalid value
              nvfs_wr_create(nvfs_tempsens, &tempsens_config, sizeof(tempsens_config)); // write it back
        }

    // end of temperature module config

        //TRACE("\n\r[ %s %s INIT ]\n\r",FW_NAME , FW_VERSION);     /**< Please comment out this line when release. */
                                                                                                                            /* Otherwise will affect server app. TP#53075 */

    Brand:
    {
            //respect $brand
              onebutton_type = 0;
              auth_brand = ngmm_retrieved_brand_id();
              if(auth_brand != NO_BRAND)
                {
                    nvfs_status = nvfs_rd_s( nvfs_brand , &brand_id, sizeof(brand_id)) ;
                    if(nvfs_status == NVFS_OK)
                    {
                        if(auth_brand == BRAND_VP_SC &&
                              !(brand_id == BRAND_VP_1BTN || brand_id == BRAND_CLIFFORD_1BTN) )   //not compatible brand
                        {
                          nvfs_wr(nvfs_brand, &auth_brand);
                            brand_id = auth_brand;      //brand being overwritten by $brand
                        }
                    }
                    else
                    {
                        nvfs_wr_create(nvfs_brand, &auth_brand, sizeof(Brand_Id));
                      brand_id = auth_brand;      //brand being assigned by $brand
                    }
                    TRACE("\n\r[BRAND FOUND : %02x]\n\r",brand_id);
                }
                else  //leave brand as it is
                {
                    nvfs_status = nvfs_rd_s( nvfs_brand , &brand_id, sizeof(brand_id)) ;
                    if(nvfs_status == NVFS_OK)
                    {
                            TRACE("\n\r[BRAND FOUND : %02x]\n\r",brand_id);
                    }
                    else
                    {
                         brand_id=BRAND_VP_SC;

                         TRACE("\n\r[ASSUME DEFAULT BRAND %02x]\n\r",brand_id);
                    }
                }

                if(brand_id == BRAND_VP_1BTN)
                {
                    onebutton_type = 1;
                    brand_id = BRAND_VP_SC;
                }
    }

    SysType:
    {
        System_Type sys_type ;

        auth_sys_type = ngmm_retrieved_sys_type();
              if(auth_sys_type != SYS_TYPE_INIT)
                {
                    nvfs_status = nvfs_rd_s(nvfs_sys_type , &sys_type, sizeof(sys_type)) ;
                    if(nvfs_status == NVFS_OK)
                    {
                        if((auth_sys_type & sys_type) != sys_type)
                        {
                            sys_type &= auth_sys_type;
                            nvfs_wr(nvfs_sys_type, &sys_type);
                        }
                    }
                    else
                    {
                        sys_type = auth_sys_type;
                        nvfs_wr_create(nvfs_sys_type, &sys_type, sizeof(System_Type));
                    }
                    TRACE("\n\r[MATCHED SYSTEM TYPE: %02x]\n\r",sys_type);
              }
                else  //leve sys_type
                {
                    nvfs_status = nvfs_rd_s(nvfs_sys_type , &sys_type, sizeof(sys_type)) ;
                    if(nvfs_status == NVFS_OK)
                    {
                            TRACE("\n\r[SYSTEM TYPE FOUND : %02x]\n\r",sys_type);
                    }
                    else
                    {
                            sys_type_assumed = (System_Type)(SYS_TYPE_RS  | SYS_TYPE_SS);
                            TRACE("\n\r[ASSUME SYSTEM TYPE %02x]\n\r",sys_type_assumed);
                    }
                }
    }

    nvram_usr_commit();
}

static Brand_Id ngmm_retrieved_brand_id(void)
{
    Brand_Id authed_brand = NO_BRAND;

    nvfs_rd_s(nvfs_auth_brand, &authed_brand, sizeof(authed_brand));

    return authed_brand;
}

static System_Type ngmm_retrieved_sys_type(void)
{
    System_Type authed_sys_type = SYS_TYPE_INIT;

    nvfs_rd_s(nvfs_auth_sys_type, &authed_sys_type, sizeof(authed_sys_type));

    return authed_sys_type;
}

/*---WEAK-FUNC--------------------------------------------------------------*/

void __attribute__((weak))fw_init(void)
{}

void __attribute__((weak))fw_init_reset(void)
{}
