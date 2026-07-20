/*==========================================================================*/
// $Id: rf_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rf_private.h"
#include "rftd2.h"
#include "bitfield.h"
#include "cmd.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void rf_init_cmd   ( void);

static void rf_oem_disarm ( void * arg );
static void rf_oem_arm    ( void * arg );
static void rf_hood_open  ( void * arg );
static void rf_hood_close ( void * arg );
static void rf_trunk_open ( void * arg );
static void rf_trunk_close( void * arg );
static void rf_door_open  ( void * arg );
static void rf_door_close ( void * arg );
static void rf_horn_set   ( void * arg );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void rf_init (void)
{
  rf_res_update = os_res_create () ;

  bitfield_wr(rf_data.icon.bit , ICON_LOCK     , (RF_ICON_COUNT - 1), FALSE );

  rf_data.sound = NO_SOUND ;

  rftd2_init () ;

  rf_init_cmd ();
}

//--------------------------------------------------------------------------//
void rf_init_cmd (void)
{
    static const Cmd_Array1 cmd_array[] =
        {
          {INPUT_OEM_DISARM               , rf_oem_disarm},
          {INPUT_OEM_ARM                  , rf_oem_arm},

          {INPUT_ALL_DOOR_STATUS_OPEN     , rf_door_open},
          {INPUT_ALL_DOOR_STATUS_CLOSE    , rf_door_close},

          {INPUT_HOOD_STATUS_OPEN         , rf_hood_open},
          {INPUT_HOOD_STATUS_CLOSE        , rf_hood_close},

          {INPUT_TRUNK_STATUS_OPEN        , rf_trunk_open},
          {INPUT_TRUNK_STATUS_CLOSE       , rf_trunk_close} ,

          {INPUT_TRIGGER_ON               , rf_horn_set },


        } ;
     CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
}

//--------------------------------------------------------------------------//
void rf_oem_disarm ( void * arg )
{
    rf_icon_clr  (ICON_HORN     , FALSE) ;
    rf_icon_clr  (ICON_ARMED    , TRUE)  ;
#if DBG_RF == 1
    rf_dbg_print("\nrf Disarm", NULL, NULL);
#endif
}

//--------------------------------------------------------------------------//
void rf_oem_arm ( void * arg )
{
    rf_icon_clr  (ICON_HORN  , FALSE) ;
    rf_icon_set  (ICON_ARMED , TRUE)  ;
#if DBG_RF == 1
    rf_dbg_print("\nrf Arm", NULL, NULL);
#endif
}

//--------------------------------------------------------------------------//
void rf_hood_open ( void * arg )
{
    rf_icon_set (ICON_HOOD, FALSE) ;
#if DBG_RF == 1
    rf_dbg_print("\nrf hood open", NULL, NULL);
#endif
}

//--------------------------------------------------------------------------//
void rf_hood_close ( void * arg )
{
    rf_icon_clr (ICON_HOOD, FALSE) ;
#if DBG_RF == 1
    rf_dbg_print("\r\nrf hood close", NULL, NULL);
#endif
}

//--------------------------------------------------------------------------//
void rf_trunk_open ( void * arg )
{
    rf_icon_set (ICON_TRUNK, FALSE) ;
#if DBG_RF == 1
    rf_dbg_print("\r\nrf trunk open", NULL, NULL);
#endif
}

//--------------------------------------------------------------------------//
void rf_trunk_close ( void * arg )
{
    rf_icon_clr (ICON_TRUNK, FALSE) ;
#if DBG_RF == 1
    rf_dbg_print("\r\nrf trunk close", NULL, NULL);
#endif
}

//--------------------------------------------------------------------------//
void rf_door_open ( void * arg )
{
    rf_icon_set (ICON_DOOR, FALSE) ;
#if DBG_RF == 1
    rf_dbg_print("\r\nrf all door open", NULL, NULL);
#endif
}

//--------------------------------------------------------------------------//
void rf_door_close ( void * arg )
{
    rf_icon_clr (ICON_DOOR, FALSE) ;
#if DBG_RF == 1
    rf_dbg_print("\r\nrf all door close", NULL, NULL);
#endif
}
//--------------------------------------------------------------------------//
void rf_horn_set ( void * arg )
{
    rf_icon_set  (ICON_HORN , TRUE)  ;
#if DBG_RF == 1
    rf_dbg_print("\r\nrf horn set", NULL, NULL);
#endif
}
