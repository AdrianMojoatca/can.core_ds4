// .\core\lib.prg_out\prg_out.c

// header files

// source files
#include "prg_out_acc.c"
#include "prg_out_anti_grind.c"
#include "prg_out_aux1.c"
#include "prg_out_aux2.c"
#include "prg_out_aux3.c"
#include "prg_out_aux4.c"
#include "prg_out_domelight.c"
#include "prg_out_factory_alarm_arm.c"
#include "prg_out_factory_alarm_disarm.c"
#include "prg_out_ground_when_arm.c"
#include "prg_out_ground_when_locked.c"
#include "prg_out_horn.c"
#include "prg_out_hw_ctrl.c"
#include "prg_out_hw_init.c"
#include "prg_out_ign.c"
#include "prg_out_init.c"
#include "prg_out_lock.c"
#include "prg_out_parking_light.c"
#include "prg_out_pulse_after_start.c"
#include "prg_out_rear_defogger.c"
#include "prg_out_second_unlock.c"
#include "prg_out_smartkey_control.c"
#include "prg_out_start.c"
#include "prg_out_trunk_release.c"
#include "prg_out_unlock.c"
#include "prg_out_unlock_user2.c"
#include "prg_out_vars.c"
#include "prg_out_ground_when_running_status.c"

#ifdef PLATFORM_CM800

	#include "prg_out_ground_when_running_5sec.c"
	#include "prg_out_ground_when_triggered.c"
	#include "prg_out_pulse_after_ign_off.c"
	#include "prg_out_pulse_before_start.c"
#else
  #include "prg_out_comfort_closure.c"
	#include "prg_out_defroster_latch.c"
	#include "prg_out_defroster_pulse.c"
	#include "prg_out_ground_when_ready.c"
#endif

