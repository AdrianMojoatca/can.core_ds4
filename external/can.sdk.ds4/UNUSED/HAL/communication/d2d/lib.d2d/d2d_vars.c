/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d_vars.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "d2d_private.h"
#include "d2d_code.h"
#include "config_d2d.h"
#include "cmd.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

Tsk       tsk_d2d    = NULL ;
Irq       d2d_irq_tx = NULL ;
Irq       d2d_irq_rx = NULL ;
Tmr       d2d_tmr_rx = NULL ;
Res       d2d_tx_res = NULL ;
D2d_Error d2d_err    = {NULL} ;
D2D_Func  d2d_func   = {NULL} ;

D2d_Ena_Bit     d2d_ena_table = {NULL}    ;
D2D_Lut_Entry * d2d_lut_list  = NULL  ; 

Boolean d2d_do_err_save = FALSE ;

Wake_Status d2d_wake_status = s_WAKE ;

volatile SInt16          d2d_rda_rx                 ;
volatile UInt8          d2d_gpio_state = 0          ;
volatile D2D_First_Byte d2d_first_byte = {0, FALSE} ;

//--------------------------------------------------------------------------//

const D2d_Ena_Bit d2d_ena_default =
{
  {
    D2D_MASK_0  ,D2D_MASK_1  ,D2D_MASK_2  ,D2D_MASK_3  ,D2D_MASK_4  ,D2D_MASK_5  ,D2D_MASK_6  ,D2D_MASK_7  ,
    D2D_MASK_8  ,D2D_MASK_9  ,D2D_MASK_10 ,D2D_MASK_11 ,D2D_MASK_12 ,D2D_MASK_13 ,D2D_MASK_14 ,D2D_MASK_15 ,
    D2D_MASK_16 ,D2D_MASK_17 ,D2D_MASK_18 ,D2D_MASK_19 ,D2D_MASK_20 ,D2D_MASK_21 ,D2D_MASK_22 ,D2D_MASK_23 ,
    D2D_MASK_24 ,D2D_MASK_25 ,D2D_MASK_26 ,D2D_MASK_27 ,D2D_MASK_28 ,D2D_MASK_29 ,D2D_MASK_30 ,D2D_MASK_31 ,
  },

  {
    D2D_ENA_0   ,D2D_ENA_1   ,D2D_ENA_2   ,D2D_ENA_3   ,D2D_ENA_4   ,D2D_ENA_5   ,D2D_ENA_6   ,D2D_ENA_7   ,
    D2D_ENA_8   ,D2D_ENA_9   ,D2D_ENA_10  ,D2D_ENA_11  ,D2D_ENA_12  ,D2D_ENA_13  ,D2D_ENA_14  ,D2D_ENA_15  ,
    D2D_ENA_16  ,D2D_ENA_17  ,D2D_ENA_18  ,D2D_ENA_19  ,D2D_ENA_20  ,D2D_ENA_21  ,D2D_ENA_22  ,D2D_ENA_23  ,
    D2D_ENA_24  ,D2D_ENA_25  ,D2D_ENA_26  ,D2D_ENA_27  ,D2D_ENA_28  ,D2D_ENA_29  ,D2D_ENA_30  ,D2D_ENA_31  ,
  }

};

//--------------------------------------------------------------------------//

const D2D_Entry input_table[]   =
{ 
/***Door Lock Control********/
 {  FN_ARM                  ,       0,      INPUT_ARM            | INPUT_ON  } ,
 {  FN_DISARM               ,       0,      INPUT_DISARM         | INPUT_ON  } ,

#ifdef PLATFORM_CM800
 {  FN_LOCK_REM1_ON         ,       0,      INPUT_RF_LOCK                    } ,
 {  FN_LOCK_REM2_ON         ,       0,      INPUT_RF_LOCK                    } ,
 {  FN_LOCK_REM3_ON         ,       0,      INPUT_RF_LOCK                    } ,
 {  FN_UNLOCK_DR_REM1_ON    ,       0,      INPUT_RF_UNLOCK                  } ,
 {  FN_UNLOCK_DR_REM2_ON    ,       0,      INPUT_RF_UNLOCK                  } ,
 {  FN_UNLOCK_DR_REM3_ON    ,       0,      INPUT_RF_UNLOCK                  } ,
 {  FN_UNLOCK_ALL_REM1_ON   ,       0,      INPUT_RF_UNLOCK                  } ,
 {  FN_UNLOCK_ALL_REM2_ON   ,       0,      INPUT_RF_UNLOCK                  } ,
 {  FN_UNLOCK_ALL_REM3_ON   ,       0,      INPUT_RF_UNLOCK                  } ,
 {  FN_TRUNK_REM1_ON        ,       0,      INPUT_RF_TRUNK | INPUT_RF_EXT    } ,
 {  FN_TRUNK_REM2_ON        ,       0,      INPUT_RF_TRUNK | INPUT_RF_EXT    } ,
 {  FN_TRUNK_REM3_ON        ,       0,      INPUT_RF_TRUNK | INPUT_RF_EXT    } ,
 {  FN_AUX1_REM1_ON         ,       0,      INPUT_RF_AUX3                    }  ,
 {  FN_AUX1_REM2_ON         ,       0,      INPUT_RF_AUX3                    }  ,
 {  FN_AUX1_REM3_ON         ,       0,      INPUT_RF_AUX3                    }  ,
 {  FN_AUX2_REM1_ON         ,       0,      INPUT_RF_AUX4                    }  ,
 {  FN_AUX2_REM2_ON         ,       0,      INPUT_RF_AUX4                    }  ,
 {  FN_AUX2_REM3_ON         ,       0,      INPUT_RF_AUX4                    }  ,
 {  FN_AUX3_REM1_ON         ,       0,      INPUT_RF_AUX5                    }  ,
 {  FN_AUX3_REM2_ON         ,       0,      INPUT_RF_AUX5                    }  ,
 {  FN_AUX3_REM3_ON         ,       0,      INPUT_RF_AUX5                    }  ,
 {  FN_AUX4_REM1_ON         ,       0,      INPUT_RF_AUX6                    }  ,
 {  FN_AUX4_REM2_ON         ,       0,      INPUT_RF_AUX6                    }  ,
 {  FN_AUX4_REM3_ON         ,       0,      INPUT_RF_AUX6                    }  ,
 {  FN_PANIC_REM1_ON        ,       0,      INPUT_RF_LOCK     | INPUT_RF_EXT }  ,
 {  FN_PANIC_REM2_ON        ,       0,      INPUT_RF_LOCK     | INPUT_RF_EXT }  ,
 {  FN_PANIC_REM3_ON        ,       0,      INPUT_RF_LOCK     | INPUT_RF_EXT }  ,
 {  FN_PANIC_REM1_OFF       ,       0,      INPUT_RF_LOCK                    }  ,
 {  FN_PANIC_REM2_OFF       ,       0,      INPUT_RF_LOCK                    }  ,
 {  FN_PANIC_REM3_OFF       ,       0,      INPUT_RF_LOCK                    }  ,
#else
 {  FN_LOCK_REM1_ON         ,       0,      INPUT_LOCK1          | INPUT_ON  } ,
 {  FN_LOCK_REM2_ON         ,       0,      INPUT_LOCK2          | INPUT_ON  } ,
 {  FN_LOCK_REM3_ON         ,       0,      INPUT_LOCK3          | INPUT_ON  } ,
 {  FN_UNLOCK_DR_REM1_ON    ,       0,      INPUT_UNLOCK_DRIVER1 | INPUT_ON  }  ,
 {  FN_UNLOCK_DR_REM2_ON    ,       0,      INPUT_UNLOCK_DRIVER2 | INPUT_ON  }  ,
 {  FN_UNLOCK_DR_REM3_ON    ,       0,      INPUT_UNLOCK_DRIVER3 | INPUT_ON  }  ,
 {  FN_UNLOCK_ALL_REM1_ON   ,       0,      INPUT_UNLOCK_ALL1    | INPUT_ON  }  ,   
 {  FN_UNLOCK_ALL_REM2_ON   ,       0,      INPUT_UNLOCK_ALL2    | INPUT_ON  }  ,   
 {  FN_UNLOCK_ALL_REM3_ON   ,       0,      INPUT_UNLOCK_ALL3    | INPUT_ON  }  , 
 {  FN_TRUNK_REM1_ON        ,       0,      INPUT_TRUNK          | INPUT_ON  }  ,
 {  FN_TRUNK_REM2_ON        ,       0,      INPUT_TRUNK          | INPUT_ON  }  ,
 {  FN_TRUNK_REM3_ON        ,       0,      INPUT_TRUNK          | INPUT_ON  }  ,
 {  FN_AUX1_REM1_ON         ,       0,      INPUT_AUX1           | INPUT_ON  }  ,
 {  FN_AUX1_REM2_ON         ,       0,      INPUT_AUX1           | INPUT_ON  }  ,
 {  FN_AUX1_REM3_ON         ,       0,      INPUT_AUX1           | INPUT_ON  }  ,
 {  FN_AUX2_REM1_ON         ,       0,      INPUT_AUX2           | INPUT_ON  }  ,
 {  FN_AUX2_REM2_ON         ,       0,      INPUT_AUX2           | INPUT_ON  }  ,
 {  FN_AUX2_REM3_ON         ,       0,      INPUT_AUX2           | INPUT_ON  }  ,
 {  FN_AUX3_REM1_ON         ,       0,      INPUT_AUX3           | INPUT_ON  }  ,
 {  FN_AUX3_REM2_ON         ,       0,      INPUT_AUX3           | INPUT_ON  }  ,
 {  FN_AUX3_REM3_ON         ,       0,      INPUT_AUX3           | INPUT_ON  }  ,
 {  FN_AUX4_REM1_ON         ,       0,      INPUT_AUX4           | INPUT_ON  }  ,
 {  FN_AUX4_REM2_ON         ,       0,      INPUT_AUX4           | INPUT_ON  }  ,
 {  FN_AUX4_REM3_ON         ,       0,      INPUT_AUX4           | INPUT_ON  }  ,
 {  FN_PANIC_REM1_ON        ,       0,      INPUT_PANIC          | INPUT_ON  }  ,
 {  FN_PANIC_REM2_ON        ,       0,      INPUT_PANIC          | INPUT_ON  }  ,
 {  FN_PANIC_REM3_ON        ,       0,      INPUT_PANIC          | INPUT_ON  }  ,
 {  FN_PANIC_REM1_OFF       ,       0,      INPUT_PANIC                      }  ,
 {  FN_PANIC_REM2_OFF       ,       0,      INPUT_PANIC                      }  ,
 {  FN_PANIC_REM3_OFF       ,       0,      INPUT_PANIC                      }  ,
#endif


 {  FN_OBD_CAN_ENABLE  		,		0,		INPUT_OBD_CAN_ENABLE | INPUT_ON  }  ,
 {  FN_OBD_CAN_DISABLE		,		0,		INPUT_OBD_CAN_DISABLE| INPUT_ON  }  ,

/***Remote Start control********/

 {  FN_START_REM            ,       0,      INPUT_START          | INPUT_ON  }  ,
 {  FN_STOP_REM             ,       0,      INPUT_STOP           | INPUT_ON  }  ,
                                                                    


 {  FN_LONG_START           ,       0,      INPUT_LONG_START     | INPUT_ON  }  ,
 {  FN_SHORT_TRUNK          ,       0,      INPUT_SHORT_TRUNK    | INPUT_ON  }  ,
/***Bypass/Key Switch control***/
 {  FN_GWR_ON               ,       0,      INPUT_GWR            | INPUT_ON  }  ,
 {  FN_GWR_OFF              ,       0,      INPUT_GWR                        }  ,
 {  FN_ACC_ON               ,       0,      INPUT_RS_ACCESSORY   | INPUT_ON  }  ,
 {  FN_ACC_OFF              ,       0,      INPUT_RS_ACCESSORY               }  ,
 {  FN_IGN_ON               ,       0,      INPUT_RS_IGNITION    | INPUT_ON  }  ,
 {  FN_IGN_OFF              ,       0,      INPUT_RS_IGNITION                }  ,
 {  FN_START_ON             ,       0,      INPUT_RS_STARTER     | INPUT_ON  }  ,
 {  FN_START_OFF            ,       0,      INPUT_RS_STARTER                 }  ,
/***Request control************/
 {  REQ_INPUT_STATUS        ,       0,      INPUT_REQ_INPUT_STATUS           }  ,
 {  REQ_RPM                 ,       0,      INPUT_REQ_RPM                    }  ,
 {  REQ_SPEED               ,       0,      INPUT_REQ_SPEED                  }  ,
 {  REQ_ENG_TEMP            ,       0,      INPUT_REQ_ENG_TEMP               }  ,
 {  REQ_INT_TEMP            ,       0,      INPUT_REQ_EXTERIOR_TEMP          }  ,      
 {  REQ_DTC                 ,       0,      INPUT_REQ_DTC                    }  ,
 {  REQ_CLR_DTC             ,       0,      INPUT_REQ_CLR_DTC                }  ,
 {  REQ_FUEL_LEVEL          ,       0,      INPUT_REQ_FUEL_LEVEL             }  ,
 {  REQ_ODO                 ,       0,      INPUT_REQ_ODO                    }  ,
 {  REQ_BAT_VOLT            ,       0,      INPUT_REQ_BAT_VOLT               }  ,
 {  REQ_TPMS                ,       0,      INPUT_REQ_TPMS                   }  ,
 {  REQ_VIN_1               ,       0,      INPUT_REQ_VIN_1                  }  ,
 {  REQ_VIN_2               ,       0,      INPUT_REQ_VIN_2                  }  ,
 {  REQ_VIN_3               ,       0,      INPUT_REQ_VIN_3                  }  ,
 {  REQ_VIN_4               ,       0,      INPUT_REQ_VIN_4                  }  ,
 {  REQ_VIN_5               ,       0,      INPUT_REQ_VIN_5                  }  ,
 {  REQ_VIN_6               ,       0,      INPUT_REQ_VIN_6                  }  ,
 {  REQ_VIN                 ,       0,      INPUT_REQ_VIN                    }  ,
 {  REQ_PROTOCOL_VER        ,       0,      INPUT_REQ_PROTOCOL_VER           }  ,
 {  REQ_MODULE_INFO         ,       0,      INPUT_REQ_MODULE_INFO            }  ,
 {  REQ_EXTENDED_STATUS_SUPPORT,    0,      INPUT_REQ_EXTENDED_STATUS_SUPPORT}  ,
 {  REQ_EXTENDED_STATUS_SUPPORT_SYNC,0,      INPUT_REQ_EXTENDED_STATUS_SUPPORT_SYNC}  ,
 {  REQ_RUNTIME             ,       0,      INPUT_REQ_RUNTIME                }  ,
 {  REQ_2WAY_STATUS         ,       0,      INPUT_REQ_2WAY_STATUS            }  ,
 {  REQ_EXTENDED_STATUS     ,       0,      INPUT_REQ_EXTENDED_STATUS        }  ,
 {  AL_QUERY                ,       0,      INPUT_AL_QUERY                   }  ,
 {  AL_ALERT_CLEAR          ,       0,      INPUT_AL_ALERT_CLEAR             }  ,
/***Other control**************/
 {  FN_CHIRP                ,       0,      INPUT_CHIRP         | INPUT_ON  } ,
 {  FN_DOME_ON              ,       0,      INPUT_DOME          | INPUT_ON  } ,
 {  FN_DOME_OFF             ,       0,      INPUT_DOME                      } ,
 {  FN_PARKING_ON           ,       0,      INPUT_PARKING       | INPUT_ON  } ,
 {  FN_PARKING_OFF          ,       0,      INPUT_PARKING                   } ,


#ifdef PLATFORM_CM800
 {  FN_EXTEND_RUNTIME       ,       0,      INPUT_RF_RUNTIME_RESET          } ,
 {  FN_CAR_FINDER           ,       0,      INPUT_RF_CAR_FINDER             } ,
#else
 {  FN_EXTEND_RUNTIME       ,       0,      INPUT_EXTEND_RUNTIME            } ,
 {  FN_CAR_FINDER           ,       0,      INPUT_CAR_FINDER                } ,
#endif


 {	FN_VALET_TOGGLE			,		0,		INPUT_VALET_TOGGLE				} ,


#ifdef PLATFORM_CM800
 {  FN_CAR_IGN_OFF          ,       0,      INPUT_RS_IGNITION               } ,
 {  FN_CAR_IGN_ON           ,       0,      INPUT_RS_IGNITION   | INPUT_ON  } ,
 {  SET_KEYLESS_ARM         ,       0,      INPUT_ARM           | INPUT_ON  } ,
 {  SET_KEYLESS_DISARM      ,       0,      INPUT_DISARM        | INPUT_ON  } ,
 {  SET_KEYLESS_LOCK        ,       0,      INPUT_ARM           | INPUT_ON  } ,
 {  SET_KEYLESS_UNLOCK      ,       0,      INPUT_DISARM        | INPUT_ON  } ,
 {  SET_KEYLESS_TRUNK       ,       0,      INPUT_TRUNK         | INPUT_ON  } ,
 {  SET_KEYLESS_PANIC       ,       0,      INPUT_PANIC         | INPUT_ON  } ,
 {  SET_KEYLESS_AUX1        ,       0,      INPUT_AUX1          | INPUT_ON  } ,
 {  SET_KEYLESS_AUX2        ,       0,      INPUT_AUX2          | INPUT_ON  } ,
 {  SET_KEYLESS_AUX3        ,       0,      INPUT_AUX3          | INPUT_ON  } ,
 {  SET_KEYLESS_AUX4        ,       0,      INPUT_AUX4          | INPUT_ON  } ,
 {  SET_KEYLESS_START       ,       0,      INPUT_START         | INPUT_ON  } ,
 {  SET_KEYLESS_STOP        ,       0,      INPUT_STOP          | INPUT_ON  } ,
#endif


#ifdef PLATFORM_CM800
 {  FN_START_ONLY           ,       0,      INPUT_START         | INPUT_ON  } ,
#else
 {  FN_START_ONLY           ,       0,      INPUT_START_ONLY    | INPUT_ON  } ,
#endif

};

//--------------------------------------------------------------------------//

const UInt8 INPUT_NUM_FUNC     = sizeof_array (input_table);


/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

