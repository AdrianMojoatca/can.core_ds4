# Release Notes - 2026-07-01

## Scope
- Consolidare workstream DB3 pentru eliminarea dependențelor legacy `config_*.h` din fluxul activ.
- Migrare callsite-uri non-core către API semantic `gpio_iface_*` (fără selectori IO expliciți în feature code).
- Decuplare RSR de `INC/dball/inc.config.fw/config_rsr*.h` prin contract core dedicat.
- Curățare proiect Keil DB3 de referințe directe la headere legacy nefolosite.

## Changes included

### 1) GPIO semantic migration (DB3 SDK + vehicle specific)
- Înlocuite apeluri directe pe output selectors cu wrappers semantice `gpio_iface_*` în zonele:
  - run-safe / rf-loop
  - sense notify (door/trunk/brake/ign/hood/keyin)
  - sense RS CAN
  - key wrap
  - led/display/reset/watchdog/led_usr
  - RSR lock/unlock/trunk și wake paths CAN/D2DI
- Consolidare API semantic în `APP/services/gpio_iface` (inclusiv `gpio_iface_private.c` și mapping-uri asociate).

### 2) Core contract DB3 GPIO decoupling
- Introduse headere contract pentru acces GPIO/can/d2d/dst/tach/misc:
  - `external/Core_Contract_SDK_DB3/core_contract_db3_gpio_output.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_gpio_input.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_can_gpio.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_d2d_gpio.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_dst_gpio.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_tach_gpio.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_misc_gpio.h`
- Simplificat `core_contract_db3_gpio_config.[ch]` la rol de mapare semantică pentru `gpio_iface`.

### 3) Config decoupling în DB3 project
- `com.inc.config.fw` a fost scos din fluxul activ și înlocuit cu variante arhivate `_1.h`.
- Fișiere legacy `config_can.h`, `config_firmware.h`, `config_gpio.h`, `config_obd_can.h`, `config_prog.h`, `config_pts_ctl.h`, `config_status.h` eliminate din utilizarea activă.
- `FRD37EU_DB3.uvproj` curățat de referințe explicite la aceste headere legacy.

### 4) RSR config decoupling (INC/dball/inc.config.fw)
- Noi contracte DB3 pentru RSR:
  - `external/Core_Contract_SDK_DB3/core_contract_db3_rsr_config.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_rsr_feature_config.h`
- Migrat include-urile RSR:
  - `external/can.sdk.db3/APP/features/dball3/lib.rsr/rsr_private.h`
  - `external/can.sdk.db3/APP/features/dball3/lib.rsr_feature/rsr_feature_private.h`
- Fișierele legacy `config_rsr.h` și `config_rsr_feature.h` mutate în variante `_1.h` în submodule.

### 5) Mapping/logic corrections
- Corecție semantică brake output (status brake separat de actuator clutch/relay) în mapping DB3.
- Curățări locale comentarii legacy în fișiere function/sense pentru aliniere pe API semantic.

## Validation
- Build Keil DB3 (`FRD37EU_DB3.uvproj`) executat cu rezultat `UV4_EXIT_CODE=0`.
- Guard legacy GPIO executat:
  - `build/scripts/check_noncore_legacy_gpio.ps1`
  - rezultat: `Tracked files: 0`.
- Bench tests raportate pe parcursul zilei: OK după Save All + Rebuild All.

## Notes
- `config_dbg.h` a rămas în afara acestei decuplări (intenționat).
- Pentru audit/rollback, variantele `_1.h` păstrează snapshot-ul configurațiilor precedente.
- Schimbările din `external/can.sdk.db3` sunt livrate prin update de pointer submodule în repo principal.
