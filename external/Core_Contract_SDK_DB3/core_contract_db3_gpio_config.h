/*<#======================================================================#>*/
/*              CONTRACT WRAPPER PENTRU CONFIG_GPIO.H (DB3)              */
/*<#======================================================================#>*/

#ifndef __CORE_CONTRACT_DB3_GPIO_CONFIG_H__
#define __CORE_CONTRACT_DB3_GPIO_CONFIG_H__

typedef enum {
    GPIO_IFACE_OUTPUT_NONE = 0U,
    GPIO_IFACE_OUTPUT_NEG_OUT1,
    GPIO_IFACE_OUTPUT_NEG_OUT2,
    GPIO_IFACE_OUTPUT_NEG_OUT3,
    GPIO_IFACE_OUTPUT_NEG_OUT4,
    GPIO_IFACE_OUTPUT_NEG_OUT5,
    GPIO_IFACE_OUTPUT_POS_OUT1,
    GPIO_IFACE_OUTPUT_POS_OUT2,
    GPIO_IFACE_OUTPUT_RELAY1,
    GPIO_IFACE_OUTPUT_RELAY2,
    GPIO_IFACE_OUTPUT_RF_LOOP_POWER,
    GPIO_IFACE_OUTPUT_LED_GREEN,
    GPIO_IFACE_OUTPUT_LED_RED,
    GPIO_IFACE_OUTPUT_LED_BLUE,
    GPIO_IFACE_OUTPUT_DAC_POWER
} gpio_iface_output_selector_t;

#define CORE_DB3_GPIO_IFACE_HANDBRAKE_OUTPUT   GPIO_IFACE_OUTPUT_NEG_OUT1
#define CORE_DB3_GPIO_IFACE_BRAKE_OUTPUT       GPIO_IFACE_OUTPUT_POS_OUT1
#define CORE_DB3_GPIO_IFACE_IGNITION_OUTPUT    GPIO_IFACE_OUTPUT_POS_OUT2
#define CORE_DB3_GPIO_IFACE_HOOD_OUTPUT        GPIO_IFACE_OUTPUT_NONE
#define CORE_DB3_GPIO_IFACE_KEY_WRAP_OUTPUT    GPIO_IFACE_OUTPUT_NONE
#define CORE_DB3_GPIO_IFACE_LIGHTS_OUTPUT      GPIO_IFACE_OUTPUT_NEG_OUT2
#define CORE_DB3_GPIO_IFACE_DOORS_OUTPUT       GPIO_IFACE_OUTPUT_NEG_OUT3
#define CORE_DB3_GPIO_IFACE_TRUNK_OUTPUT       GPIO_IFACE_OUTPUT_NEG_OUT4
#define CORE_DB3_GPIO_IFACE_PTS_1_OUTPUT       GPIO_IFACE_OUTPUT_NEG_OUT5
#define CORE_DB3_GPIO_IFACE_PTS_2_OUTPUT       GPIO_IFACE_OUTPUT_POS_OUT1
#define CORE_DB3_GPIO_IFACE_CLUTCH_OUTPUT      GPIO_IFACE_OUTPUT_RELAY1
#define CORE_DB3_GPIO_IFACE_RAP_OUTPUT         GPIO_IFACE_OUTPUT_RELAY2
#define CORE_DB3_GPIO_IFACE_RF_LOOP_OUTPUT     GPIO_IFACE_OUTPUT_RF_LOOP_POWER
#define CORE_DB3_GPIO_IFACE_RSR_LOCK_OUTPUT    GPIO_IFACE_OUTPUT_NEG_OUT5
#define CORE_DB3_GPIO_IFACE_RSR_UNLOCK_OUTPUT  GPIO_IFACE_OUTPUT_NEG_OUT3
#define CORE_DB3_GPIO_IFACE_RSR_TRUNK_OUTPUT   GPIO_IFACE_OUTPUT_NEG_OUT3

#define get_gpio_iface_handbrake_output()   ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_HANDBRAKE_OUTPUT)
#define get_gpio_iface_brake_output()       ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_BRAKE_OUTPUT)
#define get_gpio_iface_ignition_output()    ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_IGNITION_OUTPUT)
#define get_gpio_iface_hood_output()        ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_HOOD_OUTPUT)
#define get_gpio_iface_key_wrap_output()    ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_KEY_WRAP_OUTPUT)
#define get_gpio_iface_lights_output()      ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_LIGHTS_OUTPUT)
#define get_gpio_iface_doors_output()       ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_DOORS_OUTPUT)
#define get_gpio_iface_trunk_output()       ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_TRUNK_OUTPUT)
#define get_gpio_iface_pts_1_output()       ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_PTS_1_OUTPUT)
#define get_gpio_iface_pts_2_output()       ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_PTS_2_OUTPUT)
#define get_gpio_iface_clutch_output()      ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_CLUTCH_OUTPUT)
#define get_gpio_iface_rap_output()         ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_RAP_OUTPUT)
#define get_gpio_iface_rf_loop_output()     ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_RF_LOOP_OUTPUT)
#define get_gpio_iface_rsr_lock_output()    ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_RSR_LOCK_OUTPUT)
#define get_gpio_iface_rsr_unlock_output()  ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_RSR_UNLOCK_OUTPUT)
#define get_gpio_iface_rsr_trunk_output()   ((gpio_iface_output_selector_t)CORE_DB3_GPIO_IFACE_RSR_TRUNK_OUTPUT)

#define GPIO_IFACE_CFG_HANDBRAKE_OUTPUT  (get_gpio_iface_handbrake_output())
#define GPIO_IFACE_CFG_BRAKE_OUTPUT      (get_gpio_iface_brake_output())
#define GPIO_IFACE_CFG_IGNITION_OUTPUT   (get_gpio_iface_ignition_output())
#define GPIO_IFACE_CFG_HOOD_OUTPUT       (get_gpio_iface_hood_output())
#define GPIO_IFACE_CFG_KEY_WRAP_OUTPUT   (get_gpio_iface_key_wrap_output())
#define GPIO_IFACE_CFG_LIGHTS_OUTPUT     (get_gpio_iface_lights_output())
#define GPIO_IFACE_CFG_DOORS_OUTPUT      (get_gpio_iface_doors_output())
#define GPIO_IFACE_CFG_TRUNK_OUTPUT      (get_gpio_iface_trunk_output())
#define GPIO_IFACE_CFG_PTS_1_OUTPUT      (get_gpio_iface_pts_1_output())
#define GPIO_IFACE_CFG_PTS_2_OUTPUT      (get_gpio_iface_pts_2_output())
#define GPIO_IFACE_CFG_CLUTCH_OUTPUT     (get_gpio_iface_clutch_output())
#define GPIO_IFACE_CFG_RAP_OUTPUT        (get_gpio_iface_rap_output())
#define GPIO_IFACE_CFG_RF_LOOP_OUTPUT    (get_gpio_iface_rf_loop_output())
#define GPIO_IFACE_CFG_RSR_LOCK_OUTPUT   (get_gpio_iface_rsr_lock_output())
#define GPIO_IFACE_CFG_RSR_UNLOCK_OUTPUT (get_gpio_iface_rsr_unlock_output())
#define GPIO_IFACE_CFG_RSR_TRUNK_OUTPUT  (get_gpio_iface_rsr_trunk_output())

#define CORE_DB3_GPIO_CONFIG_VERSION      2

#endif /* __CORE_CONTRACT_DB3_GPIO_CONFIG_H__ */
