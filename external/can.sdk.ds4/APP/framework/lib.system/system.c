// .\core\lib.system\system.c

// header files

// source files
#include "dei_system_common_events.c"
#include "dei_system_common_init.c"
#include "dei_system_hybrid_init.c"
#include "dei_system_ign_dome.c"
#include "dei_system_ign_lock.c"
#include "dei_system_ign_stat.c"
#include "dei_system_init.c"
#include "dei_system_oem_lock.c"
#include "dei_system_oem_unlock.c"
#include "dei_system_pre_init.c"
#include "dei_system_prg.c"
#include "dei_system_rf_1btn.c"
#include "dei_system_rf_car_finder.c"
#include "dei_system_rf_ch3.c"
#include "dei_system_rf_ch4.c"
#include "dei_system_rf_ch5.c"
#include "dei_system_rf_ch6.c"
#include "dei_system_rf_error.c"
#include "dei_system_rf_init.c"
#include "dei_system_rf_keyboard_unlock.c"
#include "dei_system_rf_lock.c"
#include "dei_system_rf_panic.c"
#include "dei_system_rf_range_check.c"
#include "dei_system_rf_temp.c"
#include "dei_system_rf_trunk.c"
#include "dei_system_rf_unlock.c"
#include "dei_system_rs_init.c"
#include "dei_system_sense_init.c"
#include "dei_system_smart_start_init.c"
#include "dei_system_ss_init.c"
#include "dei_system_vars.c"
#include "dei_system_convenience_alert.c"

#ifdef PLATFORM_933
	#include "dei_system_headlight_alert.c"
#endif

