# TODO — can.fw.frd37eu

Tasks that were proposed but are still pending (waiting on approval, blocked, or
deferred). Kept under git. Update this file whenever a task is proposed,
started, finished, or dropped.

Status legend: `[ ]` pending · `[~]` in progress · `[x]` done (move to Done) ·
`[-]` dropped.

---

## Pending

CORE ENC + gap plan (repo: FRD37EU_SPLIT; decided 2026-07-07, architect-reviewed):
- All 5 steps done — plan complete (see Done). Remaining follow-ups below.

Follow-ups (flagged by reviews; not scheduled):
- [ ] Migrate `combine_core_fw_hex.ps1` to the single sources (still hardcodes ABOUT/CORE
  literals) — Step 3 review.
- [ ] Fix literal `\n` in `check_noncore_core_boundary.ps1` Write-Error (line ~196) so the
  violation list prints multi-line — Step 5 review (script now active on CORE_DB3).
- [ ] Reconcile the stale `projects\CORE_DB3\CORE_DB3.uvproj` (paths point at FRD37EU_NEW,
  no -NoProfile) vs the active repo-root `CORE_DB3.uvproj` — Step 5 review.
- [ ] FW image identity: FW ENC name stays degenerate `403...enc` (no ABOUT); decide a
  deterministic FW naming/identity scheme — Step 4 review.
- [ ] Bench: device-side readback/CRC verification for the two-stage ENC flash (the real
  fix for the runtime inconsistency) — architect review.

## In progress

- [~] **Diagnose CORE post-split reset loop** — regular ~8s reset cycle with variable
  trace depth = **hardware WDT** (`wdog_init(...,0x2000000)` ~8s @72MHz, fed by RTOS
  `wdt_task`). Earlier "d2di jumps into FW region" hypothesis DISPROVEN (all init MARK
  01-11 run). Root cause flow: `core_init()` doesn't reach the handoff->FW (where the WDT
  gets fed in steady state), so WDT fires. Findings so far:
    * Single-image "combine A" (combine_core_fw_hex.ps1 -RunEnc, one-pass flash) got
      further than two-stage: reaches "START SEARCHING" but `core_init()` still doesn't
      return (no "START MAIN"); reset persists ~8s.
    * Flash tool likely writes from addr 0, so two-stage (CORE then FW) rewrites CORE ->
      that's why combined single image is the working test path.
    * reset_process() is only the physical factory-reset button; not the cause.
  Diagnostics applied (uncommitted, temporary): 11 `//DIAG` TRACE markers in
  can.sdk.db3 core.c; `DBG_WDOG_V=1` in external/Core_Contract_SDK_DB3/config_dbg.h so
  boot prints the WDT reset reason (RSIR + WDOG_V_RESET_* type) to split "too slow/blocked"
  (TIMEOUT) vs "hard fault / bad address" (DATA_ABORT/PREFETCH/UNDEF).
    * RESULT: RSIR=0x04 + RESET_TYPE=0x01 = **WDOG_V_RESET_TIMEOUT** (plain hardware WDT),
      constant ~7.47s interval -> WDT armed early in wdog_v_init and NEVER kicked; init
      doesn't reach steady-state (handoff->FW) within 8s. NOT a hard-fault -> split code
      + layout are correct.
    * Next test: surgically disabled the HW WDT (commented wdog_init/wdog_enable in
      wdog_v_init.c with /*DIAG*/) to see if the module then completes init -> handoff ->
      FW runs. If yes, root cause = WDT feeding/timing during the extended split init.
    * WDT-disable RESULT: reset gone (RSIR=0007 power-on only). Init now runs to MARK 11
      then HANGS (no reset, FW not launched) -> a REAL hang after MARK 11, previously
      masked by the 8s WDT reset. Confirms code path is otherwise fine; one blocking call
      remains. Prime suspect: the redundant SECOND d2d_init() at core.c:245 (d2d already
      init at 193/196).
    * Added MARK 12-15 in core.c. RESULT: hang moved EARLIER, to wdog_v_init (right after
      the "Not a watchdog reset" print, before "AFTER WD INIT"). 3x reflash of the SAME
      image = IDENTICAL stop point => NOT flash-tool randomness; DETERMINISTIC hang.
    * Code-added-later moving the hang EARLIER + deterministic = layout-sensitive MEMORY
      CORRUPTION (heisenbug), likely split-related. Prime suspect: init_unused_mem()
      (wdog_v_init.c:60) filling RAM via os_mem_info() bounds that may be wrong in the
      split image. Added WV1/WV2/WV3 markers in wdog_v_init to localize.
    * RESULT: none of WV1/2/3 printed -> hang is BEFORE WV1, i.e. between the
      "Not a watchdog reset" print and init_unused_mem completing. Deterministic (3x
      reflash identical). Added WV0 (after log_reset_type) + WV0b (after
      WDOG_V_RESET_APP_CODE write) to isolate: log_reset return vs the RESET_APP_CODE
      write (bad RAM addr in split?) vs init_unused_mem.
    * RESULT: WV0/WV0b added AFTER a print made the print truncate EARLIER -> impossible in
      normal execution -> CONFIRMS layout-sensitive memory corruption; my instrumentation
      became the moving target. Stopped adding markers.
    * PIVOT: reverted core.c + wdog_v_init.c to clean (git checkout from PARENT repo; the
      submodule's own .git is broken - gitdir link invalid, tracked-by-parent). WDT and
      reset_process restored; kept DBG_WDOG_V=1 for reset classification. Testing the CLEAN
      combined image (never tested clean before - markers were present since CORE-only
      debug). If clean works -> instrumentation was the cause. If it resets -> use the
      built-in classifier; user's os_tsk_wait() idea (yields to feed WDT) is the fallback.
  Also-fix-later: external/can.sdk.db3 submodule .git gitdir link is broken (checkout must
  be done from the parent repo).
    * CLEAN combined RESULT: deterministic WDOG_V_RESET_TIMEOUT (RSIR=0x04), reaches
      "START SEARCHING" (core.c:213) every boot then WDT-resets ~8s; never reaches
      DETECT INIT FINISHED (235). Clean build gets FURTHER + consistent than instrumented
      (confirms markers were perturbing). Only executable code between 213 and 235 is the
      redundant SECOND d2d_init() at core.c:230 -> prime blocker. TEST: commented it (one
      surgical change, WDT on, DBG_WDOG_V=1). If it then reaches handoff/FW -> 2nd d2d_init
      was the blocker; RESULT: no change (still START SEARCHING) -> 2nd d2d_init NOT the blocker. Clean init reaches ~START SEARCHING in ~8s and WDT fires there. Next test: clean code (d2d_init restored) + WDT OFF -> does it EVER complete (reach handoff/FW) or hard-hang? Distinguishes slow-init+WDT-starvation (fix: os_tsk_wait feed) vs hard block.
    * CLEAN + WDT OFF RESULT: hangs FOREVER (20+ min) at wdog_v_init (after "Not a
      watchdog reset"), i.e. EARLIER than WDT-on -> hard hang, and symptom still moves
      with any edit -> definitive layout-sensitive memory corruption.
    * ARCHITECT static root-cause (no rebuild): NOT combine (byte-faithful), NOT FW-
      specific, NOT init_unused_mem (correctly bounded to free ER_OS_HEAP_1). ROOT CAUSE:
      **undersized kernel/IRQ stack** OS_STK_SIZE=0x100 (256B) is the MSP for all nested
      IRQ + kernel once scheduling starts; overflows when wdog_v_init starts the tick IRQ
      + wdog_v/wdt tasks amid UART/TRACE IRQs -> corrupts adjacent OS-heap stacks/TCBs.
      Secondary: TASK_MAIN_STK=0x280 (640B) too small for the deep core_init chain.
      Not combined-specific (standalone CORE would fail too).
    * FIX APPLIED (steps 1+2, WDT left off to isolate): OS_STK_SIZE 0x100->0x300
      (config.h), TASK_MAIN_STK 0x280->0x400 (main.c). Headroom in unused ER_OS_HEAP_1.
      Awaiting test: if init now completes -> handoff -> FW, stack was the root cause.
    * RESULT (WDT off): the wdog_v_init corruption is GONE (full "resuming..." print now)
      and it reaches AFTER CMD+RESET INIT, then FREEZES at the d2di_init block (core.c:190).
      Real forward progress -> stack sizing IS the root-cause class. d2di_init is not a
      blocking wait; it creates/starts d2di_task -> more stack/heap pressure.
    * Bumped further: OS_STK_SIZE 0x300->0x500, TASK_MAIN_STK 0x400->0x600 (headroom in
      unused ER_OS_HEAP_1). If still stuck at d2di -> use the Keil linker stack-usage
      report (--info stack / .htm) for precise per-task sizing (TASK_D2DI_STK=0x100,
      TASK_WDT_STK=0x40, TASK_WDOG_V_STK=0xC0 are all small) instead of guessing.
    * d2di bump alone insufficient (AFTER CMD+RESET INIT now prints fully, still freezes
      at d2di block). User chose option B: generous SWEEP of boot-path task stacks.
      Applied (all in can.sdk.db3): OS_STK_SIZE 0x500, TASK_MAIN_STK 0x600, and to 0x300:
      TASK_WDOG_V_STK, TASK_PER_EXEC_STK, TRACE_TSK_STK, TASK_D2D_STK, TASK_D2DI_STK,
      TASK_TIMEOUT_STK, TASK_PROG_STK; TASK_WDT_STK 0x200. (display/exec already 0x200;
      tach/bus_status/status_req/rf/rf_loop/ctl_dl/secure_lock/led_usr create no tasks;
      RSR/HOBP/CAN feature tasks are not on the CORE boot path.) WDT still off to isolate.
      RESULT: sweep worked - now reaches START SEARCHING (past d2di block). Next freeze at the 2nd unconditional d2d_init (core.c:230) - a double-init since d2di already handles d2d on DB3. Commented it (post-sweep, clean) as test; if it reaches handoff/FW, that was the last blocker (proper fix = make d2d_init conditional on !d2di active).
  Uncommitted/temporary diagnostics still in tree: DBG_WDOG_V=1 (config_dbg.h),
  WDT-disable (wdog_v_init.c). The stack-size changes (OS_STK_SIZE, TASK_MAIN_STK, and the
  boot-path TASK_*_STK bumps) are the actual FIX (keep; tune down later with headroom).

## Done

- [x] **Step 5 — enforce guard before ENC + boundary on CORE_DB3** — post_build runs
  `verify_core_image_gap.ps1` on the split core hex before the ENC step (core only,
  separate process, throws on failure); `CORE_DB3.uvproj` BeforeMake now also runs
  `check_noncore_core_boundary.ps1`. Validated + code-reviewed. Completes the plan.
  Done 2026-07-08 (SPLIT).
- [x] **Step 4 — deterministic CORE ENC selection** — `Get-ExpectedEncName` replicates
  enc_rename_403.pl; `post_build_split_and_enc.ps1` selects the exact expected ENC name
  (fail-fast if absent) instead of "newest within 2s". Validated (name matches real
  artifact) + code-reviewed. Done 2026-07-07 (SPLIT).
- [x] **Step 3 — single source of truth** — split + guard read CORE/FW window from
  `image_layout.h` and ABOUT from `app.sct` (new `Get-AboutWindowFromScatter`); address
  params became optional overrides. Validated + code-reviewed. Done 2026-07-07 (SPLIT).
- [x] **Step 2 — gap-aware guard** — `verify_core_image_gap.ps1` + shared
  `core_layout_common.ps1`; asserts gap/sector invariants on `.core.hex`, emits audit
  artifacts, dedups the layout-define parser. Validated (PASS + 3 FAIL cases) and
  code-reviewed. Done 2026-07-07 (in FRD37EU_SPLIT).
- [x] **Step 1 — fix CORE ENC identity** — `split_db3_hex_images.ps1` now includes the
  ABOUT block (0x8000..0x80FF, from app.sct LR_ABOUT) in the CORE image so
  hex2enc_403/enc_rename_403.pl can stamp a deterministic, non-colliding ENC name.
  Verified: core hex min 0x8000 (ABOUT present) / max 0x27ECB (< FwStart), FW unaffected.
  Done 2026-07-07 (in FRD37EU_SPLIT).
- [x] **Commit generated docs & memory** — `CLAUDE.md`,
  `.claude/memory/project-context.md`, and `TODO.md`. Done 2026-07-07.

## Notes / deferred ideas (from architect review)

- **Bench inconsistency fix is device-side**, not build-side: two-stage ENC flashing
  is unreliable because the flash tool has no readback. Real fix = per-image CRC +
  readback/verify on the bench. Verify+sector-align only guarantees the images are
  sector-disjoint (they already are: FwStart 0x28000 is a 32KB sector start).
- `main_db3_release.core.hex` (db3 monolithic, NEW-tree script) is truncated at 0xFFFF
  and must NOT be used as the CORE source; authoritative core hex is
  `build/core_db3_build/core_db3_release.core.hex`.
- CMake migration (Keil UV4 -> CMake) is future work per the SDK README. Not scheduled.

## Notes / deferred ideas

- CMake migration (Keil UV4 -> CMake) is mentioned as future work in
  `external/can.sdk.db3/README.md`. Not scheduled; not an active task.

### 2026-07-08 stack-sweep progress (see In progress)
- Boot-path task-stack sweep reached START SEARCHING (past d2di). Commenting 2nd d2d_init was inconclusive/ambiguous (IDE stopped early) -> restored it. Bumped OS_STK_SIZE 0x800, TASK_MAIN_STK 0x800 as a decisive stack test; user to run FULL (no early IDE stop). If not resolved, pivot to Keil linker --callgraph stack-usage report for exact sizing.
- 0x800 kernel/main stacks did NOT help past START SEARCHING -> freeze at 2nd d2d_init(core.c:230) is a REAL block, not stack. Mechanism: d2d_init overwrites d2di redirection + reconfigs UART0 + clear_d2d_false_code() sends 10 d2d_tx() BEFORE os_tsk_start(tsk_d2d) -> likely blocks. Commented it (full-capture retest). If reaches FW -> proper fix = make d2d_init(230) conditional on !d2di / remove the redundant 2nd call.
- User markers pinned the freeze to prog_init (creates prog task via os_tsk_create) THIS build, but freeze point keeps moving (d2d_init->START SEARCHING->prog_init) = corruption, not a specific fn. Confirmed os_kern.c has NO trace/print (BSP kernel, only os_local.h) -> print-from-kernel excluded (user was right). LEDs (gpio_pl_set, RED/GREEN/BLUE, low-level, kernel-free) are the runtime alt. DECISION: enabled Keil linker --callgraph --info=stack,summarystack on CORE_DB3 to get exact per-task worst-case stack + total-vs-heap, device-free. User to rebuild CORE_DB3; I read core_db3_release.htm.
- LINKER REPORT (core_db3_release.htm): Maximum Stack Usage = 504 bytes + Unknown (fn-pointers). ROOT CAUSE FOUND: display_task Max Depth = 504 bytes (chain display_task->service_display_prog->display_wait->os_irq_wait_v) but TASK_DISPLAY_STK=0x200(512) -> ~8B margin -> overflow once RTX context save added. display_task created early (display_init core.c:174), runs concurrently -> corrupts heap -> the moving-freeze heisenbug. All other task frames small (task_main 40, prog 24, d2d 32, wdog_v 80). FIX: TASK_DISPLAY_STK 0x200->0x400. WDT still off to confirm init completes. After confirm: restore WDT, right-size the over-bumped stacks down (report shows real needs are modest), remove --callgraph.
- Array-overflow ruled out (prog.c: 27 APPEND_FEATURE == FEAT_COUNT 27, fits). prog_init not a hard block (clean builds passed it; user markers perturbed it). Removed the users 11 MARK TRACE lines from core.c and restored the 2nd d2d_init (clean original). State: clean core.c + display fix (TASK_DISPLAY_STK 0x400) + stack bumps + WDT off + DBG_WDOG_V=1 + --callgraph. User to build clean and report how far among the ORIGINAL trace points (START SEARCHING/DETECT INIT FINISHED/START MAIN/handoff).
- KEY: clean build (markers removed + display fix) STILL stops at START SEARCHING, same as before display fix -> display was NOT the corruptor for this point. Clean build stop is STABLE (independent of markers/display) -> the 2nd d2d_init (core.c ~248) is a REAL block, not the moving-heisenbug. (Heisenbug only appeared when adding markers.) d2d_init on DB3: d2di already owns d2d; d2d_init overwrites d2di funcs + reconfigs UART0 + clear_d2d_false_code sends 10 d2d_tx BEFORE os_tsk_start(tsk_d2d) -> likely blocks. Commented it on clean build for definitive let-it-run test. If reaches FW -> proper fix = make 2nd d2d_init conditional on !d2di.
- Verified: CORE_DB3_BUILD is the DB3 macro; DEV_LPC_176x is defined unconditionally (device.h:24) so it is true on BOTH DB3 and DS4 -> the #ifdef DEV_LPC_176x d2di gate in core.c is non-conforming (runs d2di on DB3). On DB3 d2di_init() returns FALSE -> d2d_init ran TWICE (193 fallback + 248 unconditional). FIX applied to can.sdk.db3/core.c (DB3-only; DS4 has its own SDK): replaced the #ifdef d2di block with a single unconditional d2d_init(), and removed the 2nd (duplicate) d2d_init. Now d2d_init runs exactly once, no d2di. Caveat: monolith likely also double-init and worked, so this may be a correct cleanup rather than THE hang fix; test will tell.
- LED/OUT bisection (user idea, JTAG unavailable). Using module OUTs via core_contract_db3_gpio_output_on (direct gpio_pl_set, no API-table, minimal perturbation). Iter 1: NEG_OUT1 after d2d_init, NEG_OUT2 after prog_init, NEG_OUT3 at START SEARCHING. Read: none=block in/before d2d_init; OUT1=between d2d and prog; OUT1+2=between prog and START SEARCHING; all3=past init. Note: single-d2d_init fix moved the freeze to d2d_init(193) (heisenbug persists). display + d2d fixes retained.
