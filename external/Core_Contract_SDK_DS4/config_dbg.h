#ifndef __CONFIG_DBG_H__
#define __CONFIG_DBG_H__

#define DEBUG_TEXT              1
#define USE_TRACE               DEBUG_TEXT

#if DEBUG_TEXT == 1
    #define DBG_PRINT_MEM       0
#endif

#if DEBUG_TEXT == 1
    #define DBG_DETECT          1
    #define DBG_SENSE           1
    #define DBG_FUNCTION        1
#endif

#if DEBUG_TEXT == 1
    #define DBG_CMD             0
    #define DBG_CTL_DL          0
    #define DBG_D2D             0
    #define DBG_EIPS            0
    #define DBG_OBD_CAN         0
    #define DBG_PROG            0
    #define DBG_RESET           0
    #define DBG_RSR_FEAT        0
    #define DBG_RSR             0
    #define DBG_RXT             0
    #define DBG_SECURE_LOCK     0
    #define DBG_STS_R           0
    #define DBG_VIN             0
    #define DBG_W2W             0
    #define DBG_WDOG_V          1
    #define DBG_WAKE            0
    #define DBG_GPIO            0
    #define DBG_PTS_CTL         0
#endif

#endif
