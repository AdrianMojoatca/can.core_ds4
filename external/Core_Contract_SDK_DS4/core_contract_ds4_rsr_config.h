#ifndef __CORE_CONTRACT_DS4_RSR_CONFIG_H__
#define __CORE_CONTRACT_DS4_RSR_CONFIG_H__

#define __CONFIG_RSR_H__

#define CONFIG_RSR_VERSION 5

#define RSR_ACC_ENA                  0
#define RSR_IGN_ENA                  0
#define RSR_STARTER_ENA              1

#define RSR_LOCK_ENA                 1
#define RSR_LOCK_PULSE               1
#define RSR_LOCK_DURATION            400

#define RSR_UNLOCK_ENA               1
#define RSR_UNLOCK_PULSE             1
#define RSR_UNLOCK_DURATION          400

#define RSR_ARM_ENA                  0
#define RSR_ARM_OUTPUT               1

#define RSR_DISARM_ENA               0
#define RSR_DISARM_OUTPUT            1

#define RSR_TRUNK_ENA                1
#define RSR_PARKING_ENA              0
#define RSR_DL_PARKING_FLASH         1

#define RSR_HOOD_SEL                 3
#define RSR_HOOD_POLARITY            0
#define RSR_VALET_SEL                3

#define RSR_DTC_CHECK                0
#define RSR_TRANS_CHECK              0
#define DEFAULT_RSR_CRANK_TIME       5000
#define DEFAULT_RSR_RPM_RUN_VALUE    3
#define DEFAULT_RSR_RPM_CRANK_VALUE  5
#define DEFAULT_RSR_RPM_HIGH_VALUE   30
#define RSR_IDLE_MODE_ENABLE         1

#endif