/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_private.h"
#include "status.h"
#include "wake.h"
#include "d2di_uart.h"
#include "d2d_code.h"
#include "timeout_f.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define D2DI_TACH_DATA_COUNT      2
#define D2DI_ALARM_DATA_COUNT     2
#define D2DI_BRAKE_DATA_COUNT     2
#define D2DI_OEM_ALARM_DATA_COUNT 1

#define D2DI_NOT_SUPPORTED        0xFF

#define D2DI_TIMEOUT_INIT         10
#define D2DI_TIMEOUT_SLEEP_MS     (10 * 60 * 1000) //this value is not deterministic
#define D2DI_DEFAULT_STAGGER      10 //average time for packets sent. Most don't have more than 2-3 data bytes

#define D2DI_OUTFREQ_HZ(f)    (1000/(f))
#define D2DI_OUTPERIOD_MS(ms) (ms)
#define D2DI_MAX_OUTITEM ((sizeof(d2di_out_action)/sizeof(d2di_out_action[0])))

#define D2DI_NOT_SUPPORTED         0xFF

//this delta is to push to the end of the schedule.
//It's faster and easier than to initialize the array depending on values from NVFS
#define D2DI_NOT_SUPPORTED_DELTA   65535U

#define D2DI_IGNITION_IS_ON     (d2di_vars.ignition | d2di_vars.rs_ignition)

//--------------------------------------------------------------------------//
typedef enum
{
    D2DI_NONE,
    D2DI_TACH,             //contains RPM value
    D2DI_ALARM_TRIGGER,    //contains door, trunk, hood, key sense status
    D2DI_BRAKE_STATUS,     //contains brake and handbrake
    D2DI_OEM_ALARM_STATUS, //contains OEM alarm status (if alarm is triggered only)

} D2Di_Broadcast_Status;
//--------------------------------------------------------------------------//
typedef struct s_D2Di_Out_Item
{
    UInt8 next;
    UInt16 delta_ms;
    Boolean supported;
}D2Di_Out_Item;
//--------------------------------------------------------------------------//
typedef struct s_D2Di_Out_Action
{
    UInt16 period_ms;
    D2Di_Broadcast_Status status;
}D2Di_Out_Action;
//--------------------------------------------------------------------------//
static const D2Di_Out_Action d2di_out_action[] =
{
    { 0                   , D2DI_NONE             }, // don't have any item #0
    { D2DI_OUTFREQ_HZ(10) , D2DI_TACH             },
    { D2DI_OUTFREQ_HZ(4)  , D2DI_ALARM_TRIGGER    },
    { D2DI_OUTFREQ_HZ(4)  , D2DI_BRAKE_STATUS     },
    { D2DI_OUTFREQ_HZ(1)  , D2DI_OEM_ALARM_STATUS },
};
//--------------------------------------------------------------------------//
typedef union s_D2Di_Alarm_Trigger
{   
  struct
    {
      unsigned front_driver : 1 ;
      unsigned front_pass   : 1 ;
      unsigned rear_driver  : 1 ;
      unsigned rear_pass    : 1 ;
      unsigned trunk        : 1 ;
      unsigned trunk_glass  : 1 ;
      unsigned hood         : 1 ;
      unsigned key_in       : 1 ;
    } bit ;

  UInt8 byte ;

} D2Di_Alarm_Trigger;
//--------------------------------------------------------------------------//
typedef struct s_D2Di_Scheduler_Vars
{
    UInt8         first_item;
    UInt16        stagger_ms;
    
    D2Di_Out_Item out_item[D2DI_MAX_OUTITEM]; // one ram item for each outaction
    Timeout       schedule_timeout ;
    Timeout       sleep_timeout    ;
}D2Di_Scheduler_Vars;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

//                                      SOF     , Source         , Destination           , Code                      , Data count               , Data (multiple bytes)                 , checksum             , EOF
static UInt8 d2di_tach[]             = {D2DI_SOF, D2DI_ADDRESS_DL, D2DI_ADDRESS_BROADCAST, D2DI_CODE_TACH            , D2DI_TACH_DATA_COUNT     , D2DI_NOT_SUPPORTED, D2DI_NOT_SUPPORTED, D2DI_DEFAULT_CHECKSUM, D2DI_EOF };
static UInt8 d2di_alarm_trigger[]    = {D2DI_SOF, D2DI_ADDRESS_DL, D2DI_ADDRESS_BROADCAST, D2DI_CODE_ALARM_TRIGGER   , D2DI_ALARM_DATA_COUNT    , 0                 , D2DI_NOT_SUPPORTED, D2DI_DEFAULT_CHECKSUM, D2DI_EOF };
static UInt8 d2di_brake_status[]     = {D2DI_SOF, D2DI_ADDRESS_DL, D2DI_ADDRESS_BROADCAST, D2DI_CODE_BRAKE_INFO      , D2DI_BRAKE_DATA_COUNT    , 0                 , D2DI_NOT_SUPPORTED, D2DI_DEFAULT_CHECKSUM, D2DI_EOF };
static UInt8 d2di_oem_alarm_status[] = {D2DI_SOF, D2DI_ADDRESS_DL, D2DI_ADDRESS_BROADCAST, D2DI_CODE_OEM_ALARM_STATUS, D2DI_OEM_ALARM_DATA_COUNT, D2DI_NOT_SUPPORTED                    , D2DI_DEFAULT_CHECKSUM, D2DI_EOF };

static D2Di_Scheduler_Vars d2di_scheduler_vars;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

//Init
static void d2di_scheduler_wake (Wake_Status wake_status);

//sleep timeout
static void d2di_sleep_timeout_start  (void);
static void d2di_sleep_timeout_kick   (void);

//schedule
static void d2di_output_schedule_next (void * arg);
static void d2di_output_schedule_init (void);
static void d2di_out_item_insert      (UInt8 item);
static Boolean d2di_status_is_enabled (D2Di_Broadcast_Status status);

//send packets
static void d2di_send_status(D2Di_Broadcast_Status status);
static void d2di_scheduler_tx(UInt8 *d2di_frame, UInt8 frame_size);

static void d2di_send_tach(void);
static void d2di_send_alarm_trigger(void);
static void d2di_send_brake_status(void);
static void d2di_send_oem_alarm_status(void);

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void d2di_scheduler_init(void)
{
    #if DBG_D2D
        dbg_tx_text("\n[D2DI_SCHEDULER]init");
    #endif
    
    wake_register(d2di_scheduler_wake); //todo validate
    
    d2di_vars.timeout_ended = TRUE;
    d2di_status_init();
    d2di_output_schedule_init();
    
    d2di_scheduler_vars.schedule_timeout = timeout_f_create(D2DI_TIMEOUT_INIT, d2di_output_schedule_next);
    d2di_scheduler_vars.sleep_timeout = timeout_f_create(D2DI_TIMEOUT_SLEEP_MS, d2di_sleep_timeout_stop);
    
    timeout_start(d2di_scheduler_vars.schedule_timeout);
}

//--------------------------------------------------------------------------//

static void d2di_scheduler_wake (Wake_Status wake_status)
{
    
    switch(wake_status)
    {
        case s_SLEEP:
        {
            timeout_stop(d2di_scheduler_vars.sleep_timeout);
            timeout_stop(d2di_scheduler_vars.schedule_timeout);
            break;
        }
        case s_WAKE:
        {
            timeout_start(d2di_scheduler_vars.schedule_timeout);
            break;
        }
    }
}

//--------------------------------------------------------------------------//

/*Initializes the schedule in the right order*/
static void d2di_output_schedule_init(void)
{
    UInt8 item;
    d2di_scheduler_vars.stagger_ms = 0;
    d2di_scheduler_vars.first_item = 0;
    
    for (item = 1; item < D2DI_MAX_OUTITEM; item++)
    {
        d2di_scheduler_vars.out_item[item].supported = d2di_status_is_enabled(d2di_out_action[item].status);
        d2di_scheduler_vars.out_item[item].delta_ms  = d2di_out_action[item].period_ms;
        d2di_out_item_insert(item);
    }
}

//--------------------------------------------------------------------------//

/*Inserts items to be scheduled to go out in the right order*/
static void d2di_out_item_insert(UInt8 item)
{
    UInt8 p, q;

    for (p = 0, q = d2di_scheduler_vars.first_item; q; p = q, q = d2di_scheduler_vars.out_item[q].next)
    {
        if (d2di_scheduler_vars.out_item[item].delta_ms < d2di_scheduler_vars.out_item[q].delta_ms)
        {
            d2di_scheduler_vars.out_item[q].delta_ms -= d2di_scheduler_vars.out_item[item].delta_ms; // reduce successor's delta time
            break; // insert item here between p and q
        }
        d2di_scheduler_vars.out_item[item].delta_ms -= d2di_scheduler_vars.out_item[q].delta_ms; // reduce this by successor's delta
    }
    d2di_scheduler_vars.out_item[item].next = q;
    if (p)
    {
        d2di_scheduler_vars.out_item[p].next = item;
    }
    else
    {
        d2di_scheduler_vars.first_item = item;
    }
}

//--------------------------------------------------------------------------//

/*When the timer expires, the next item is sent and inserted back in the queue
to where it will next be required to be sent*/ 
void d2di_output_schedule_next(void * arg)
{
    UInt8 item;

    while (d2di_scheduler_vars.first_item && d2di_scheduler_vars.out_item[d2di_scheduler_vars.first_item].delta_ms == 0 && d2di_uart_tx_q_is_empty())
    {
        item = d2di_scheduler_vars.first_item;
        d2di_scheduler_vars.first_item = d2di_scheduler_vars.out_item[item].next; // remove from list

        // we put it back into the schedule for next time
        if(d2di_scheduler_vars.out_item[item].supported)
        {
            d2di_scheduler_vars.out_item[item].delta_ms = d2di_out_action[item].period_ms + d2di_scheduler_vars.stagger_ms;
        }
        else
        {
            d2di_scheduler_vars.out_item[item].delta_ms = D2DI_NOT_SUPPORTED_DELTA;
        }
        d2di_scheduler_vars.stagger_ms = 0;
        d2di_out_item_insert(item);
        if(d2di_scheduler_vars.out_item[item].supported)
        {
            d2di_send_status(d2di_out_action[item].status);
        }
    }

    if (d2di_scheduler_vars.first_item)
    {
        if (d2di_scheduler_vars.out_item[d2di_scheduler_vars.first_item].delta_ms)
        {
            timeout_update(d2di_scheduler_vars.schedule_timeout, d2di_scheduler_vars.out_item[d2di_scheduler_vars.first_item].delta_ms); // set_timer
            timeout_start(d2di_scheduler_vars.schedule_timeout);
            d2di_scheduler_vars.out_item[d2di_scheduler_vars.first_item].delta_ms = 0; // when timeout happens this will be zero
        }
        else // we didn't do this because output q was not empty
        {
            d2di_scheduler_vars.stagger_ms = D2DI_DEFAULT_STAGGER;
            timeout_update(d2di_scheduler_vars.schedule_timeout, D2DI_DEFAULT_STAGGER);
            timeout_start(d2di_scheduler_vars.schedule_timeout);
        }
    }
}

//--------------------------------------------------------------------------//

static void d2di_send_status(D2Di_Broadcast_Status status)
{    
    switch(status)
    {
        case D2DI_TACH             : d2di_send_tach()             ; break ;
        case D2DI_ALARM_TRIGGER    : d2di_send_alarm_trigger()    ; break ;
        case D2DI_BRAKE_STATUS     : d2di_send_brake_status()     ; break ;
        case D2DI_OEM_ALARM_STATUS : d2di_send_oem_alarm_status() ; break ;
    }
}

//--------------------------------------------------------------------------//

static void d2di_send_tach(void)
{    
    if(!d2di_vars.status_support.tach)
    {
        if(status_get_rpm(STATUS_RPM_WORD) != 0)
        {
            d2di_vars.status_support.tach = 1;
        }
    }
    if(D2DI_IGNITION_IS_ON && d2di_vars.status_support.tach)
    {
        d2di_tach[D2DI_FIRST_DATA_BYTE]     = status_get_rpm(STATUS_RPM_HIGH);
        d2di_tach[D2DI_FIRST_DATA_BYTE + 1] = status_get_rpm(STATUS_RPM_LOW);
        d2di_insert_checksum(d2di_tach);
        d2di_scheduler_tx(d2di_tach, D2DI_MIN_PACKET_SIZE + D2DI_TACH_DATA_COUNT);
    }
}

//--------------------------------------------------------------------------//

/*Sends alarm trigger message. Contains the doors/hood/trunk/keyin status.
Does not follow spec but follows interface module behavior.
Message is broadcasted on ignition off if status is open.
*/
static void d2di_send_alarm_trigger(void)
{
    UInt8 last_alarm_trigger;
    if(d2di_vars.status_support.alarm_trigger.byte)
    {
        Door               doors_source = {0};
        D2Di_Alarm_Trigger cur_alarm    = {0};
                
        doors_source.byte = (UInt8)status_get_door(STATUS_DOOR_BYTE);
        cur_alarm.byte = doors_source.byte;
        
        //Need to swap the hood and trunk_glass status for d2di struct format
        cur_alarm.bit.hood = doors_source.bit.hood;
        cur_alarm.bit.trunk_glass = doors_source.bit.trunk_glass;
        
        //Last bit from the d2di struct is key_in while ours is gas cap, so we remove the gas cap value and put the key_in value.
        cur_alarm.bit.key_in = status_get_key(STATUS_KEY_IN);
        
        last_alarm_trigger = d2di_alarm_trigger[D2DI_FIRST_DATA_BYTE];    
        
        //need to mask to make sure we don't send a value that is not supposed to be supported or activated in D2D config.
        cur_alarm.byte &= d2di_vars.status_support.alarm_trigger.byte;  
        
        d2di_alarm_trigger[D2DI_FIRST_DATA_BYTE]     = cur_alarm.byte;
        //for support, a 1 means not supported.
        d2di_alarm_trigger[D2DI_FIRST_DATA_BYTE + 1] = ~d2di_vars.status_support.alarm_trigger.byte;
        d2di_insert_checksum(d2di_alarm_trigger);        
                
        //need to mask to make sure we don't send a value that is not supposed to be supported or activated in D2D config.
        cur_alarm.byte &= d2di_vars.status_support.alarm_trigger.byte;
        
        if(D2DI_IGNITION_IS_ON) //when ignition is on, it will always be broadcasted
        {
            d2di_scheduler_tx(d2di_alarm_trigger, D2DI_MIN_PACKET_SIZE + D2DI_ALARM_DATA_COUNT);
        }
        else if(last_alarm_trigger != cur_alarm.byte) //if there is a change, we need to see the change update.
        {
            d2di_vars.block_sleep_restart = FALSE;
            d2di_sleep_timeout_kick();
            d2di_scheduler_tx(d2di_alarm_trigger, D2DI_MIN_PACKET_SIZE + D2DI_ALARM_DATA_COUNT);
        }
        else if(cur_alarm.byte) //if there any status on, even in ignition off, we broadcast (not spec compliant) but only for TIMEOUT_SLEEP_MS
        {
            d2di_sleep_timeout_start();
            if(!d2di_vars.timeout_ended)
            {
                d2di_scheduler_tx(d2di_alarm_trigger, D2DI_MIN_PACKET_SIZE + D2DI_ALARM_DATA_COUNT);
            }
        }
        else //else we stop the timeout since we don't need to send. Doors are now closed.
        {
            if(!d2di_vars.timeout_ended)
            {
                d2di_sleep_timeout_stop(NULL);
            }
        }
    }
}

//--------------------------------------------------------------------------//

static void d2di_send_brake_status(void)
{
    if(d2di_vars.status_support.brake.byte)
    {
        UInt8 last_brake_status, cur_brakes;
        cur_brakes = status_get_brake(STATUS_BRAKE_BYTE);
        last_brake_status = d2di_brake_status[D2DI_FIRST_DATA_BYTE];
        
        if((last_brake_status != cur_brakes) || D2DI_IGNITION_IS_ON)
        {
            //need to mask to make sure we don't send a value that is not supposed to be supported or activated in D2D config.
            cur_brakes &= d2di_vars.status_support.brake.byte;
            
            d2di_brake_status[D2DI_FIRST_DATA_BYTE]     = cur_brakes;
            //for support, a 1 means not supported.
            d2di_brake_status[D2DI_FIRST_DATA_BYTE + 1] = ~d2di_vars.status_support.brake.byte;
            d2di_insert_checksum(d2di_brake_status);
            d2di_scheduler_tx(d2di_brake_status, D2DI_MIN_PACKET_SIZE + D2DI_BRAKE_DATA_COUNT);
        }
    }
}

//--------------------------------------------------------------------------//

static void d2di_send_oem_alarm_status(void)
{
    Boolean alarm_triggered = status_get_alarm(STATUS_ALARM_TRIGGERED);
    UInt8 last_alarm_status = d2di_oem_alarm_status[D2DI_FIRST_DATA_BYTE];
    
    if(alarm_triggered)
    {   
        d2di_vars.status_support.oem_alarm_trigger = 1;
    }
    
    if(d2di_vars.status_support.oem_alarm_trigger)
    {
        
        if(D2DI_IGNITION_IS_ON || alarm_triggered || (alarm_triggered != last_alarm_status))
        {
            d2di_oem_alarm_status[D2DI_FIRST_DATA_BYTE] = alarm_triggered;
            d2di_insert_checksum(d2di_oem_alarm_status);
            d2di_scheduler_tx(d2di_oem_alarm_status, D2DI_MIN_PACKET_SIZE + D2DI_OEM_ALARM_DATA_COUNT);
        }
    }
}

//--------------------------------------------------------------------------//

static void d2di_scheduler_tx(UInt8 *d2di_frame, UInt8 frame_size)
{
    d2di_uart_tx_q_insert_frame(d2di_frame, frame_size);
}

//--------------------------------------------------------------------------//

/*If at least one of the statuses in the message is enabled in D2D,
the packet will be sent (but masked) when sent to remove unwanted statuses*/
static Boolean d2di_status_is_enabled(D2Di_Broadcast_Status status)
{
    switch(status)
    {
        case D2DI_TACH :
            return d2di_mask_is_enabled(SET_RPM) || d2di_mask_is_enabled(REQ_RPM);
        case D2DI_ALARM_TRIGGER :
            return d2di_mask_is_enabled(SET_DRIVER_OPEN) || d2di_mask_is_enabled(SET_OTHER_OPEN) || 
                    d2di_mask_is_enabled(SET_KEY_IN) || d2di_mask_is_enabled(SET_TRUNK_OPEN) || d2di_mask_is_enabled(SET_HOOD_OPEN);
        case D2DI_BRAKE_STATUS :
            return d2di_mask_is_enabled(SET_HANDBRAKE_ON) || d2di_mask_is_enabled(SET_BRAKE_ON);
        case D2DI_OEM_ALARM_STATUS :
            return TRUE; //no D2D message for sense alarm triggered. Cannot determine based on NVFS
        default:
            return FALSE;
    }
}



//--------------------------------------------------------------------------//

/*Starts the timeout only if it is not already active.*/
static void d2di_sleep_timeout_start(void)
{
    Boolean switched_timeout = FALSE;
    ATOMIC
    (
        if(!d2di_vars.block_sleep_restart && d2di_vars.timeout_ended)
        {
            d2di_vars.timeout_ended = FALSE;
            switched_timeout = TRUE;
        }
    )
    if(switched_timeout)
    { 
        wake_enter();
        timeout_start(d2di_scheduler_vars.sleep_timeout);
        #if DBG_D2D
            dbg_tx_text("\n[D2DI_SCHEDULER]Starting timer");
        #endif
    }
}
//--------------------------------------------------------------------------//

/*Kicks the timer back to TIMEOUT_SLEEP_MS. This is used when a trigger is on
and a second triggers kicks in. We reset the the TIMEOUT_SLEEP_MS*/
static void d2di_sleep_timeout_kick(void)
{
    timeout_start(d2di_scheduler_vars.sleep_timeout);
    #if DBG_D2D
        dbg_tx_text("\n[D2DI_SCHEDULER]Kicked timer");
    #endif
}

//--------------------------------------------------------------------------//
/*When the TIMEOUT_SLEEP_MS is over, we stop sending the scheduled messages
to allow the module to go to sleep*/
void d2di_sleep_timeout_stop(void * arg)
{
    timeout_stop(d2di_scheduler_vars.sleep_timeout);
    ATOMIC
    (
        if(!d2di_vars.timeout_ended)
        {
            d2di_vars.timeout_ended = TRUE;
            d2di_vars.block_sleep_restart = TRUE;
        }
    )
    
    wake_leave();
    #if DBG_D2D
        dbg_tx_text("\n[D2DI_SCHEDULER]Timeout ended timer");
    #endif
}
