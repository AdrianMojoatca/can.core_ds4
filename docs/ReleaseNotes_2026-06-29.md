# Release Notes - 2026-06-29

## Scope
- DB3: finalizare izolare `config_prog.h` din fluxul activ și trecere pe configurare prin setters din vehicle-specific.

## Changes included
  - `external/Core_Contract_SDK_DB3/core_contract_db3_prog_config.c`
- Configurarea PROG pentru DB3 este injectată explicit din vehicle-specific prin setters:
  - `VEHICLE_SPECIFIC/lib.fw.db3/lib.fw/fw_init.c`
- Eliminată referința `config_prog.h` din proiectul Keil DB3:
- Actualizare identitate build:
  - `VEHICLE_SPECIFIC/lib.config.db3/com.inc.config.fw/build_identity.h` -> `1.00_94`
## Runtime behavior
- Core rămâne generic; valorile pe feature-uri PROG se setează din vehicle-specific (setter-based).
- Enable/disable runtime se aplică prin `PROG_CONFIG_ENABLE_FIELDS(...)` pe baza valorilor injectate.

- Keil DB3 build executat din `FRD37EU_DB3.uvproj`.
- Verificare bench raportată: propagare runtime pentru `custom3_opt2_enable` testată cu valori `0U` și `1U`.

## Notes
- `config_prog.h` poate rămâne doar ca document de referință de calibrare, fără utilizare în fluxul activ DB3.

## Tag

## Update - PTS/STATUS decoupling (DB3)

- Decuplare `config_pts_ctl.h` și `config_status.h` din fluxul activ DB3.
- Stabilizare inițializare runtime pentru PTS config (bench visibility).

### Changes included
  - `external/Core_Contract_SDK_DB3/core_contract_db3_pts_ctl_config.c`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_status_config.h`
- Injectare vehicle-specific prin setters:
  - `VEHICLE_SPECIFIC/lib.fw.db3/lib.fw/fw_init.c`

## Update - RSR config decoupling (DB3)

### Scope
- Decuplare RSR de headerele legacy `config_rsr.h` și `config_rsr_feature.h` din `external/can.sdk.db3/INC/dball/inc.config.fw`.
- Migrare consumatori RSR pe contract DB3 dedicat, fără dependență directă pe `inc.config.fw`.

### Changes included
- Headere contract noi pentru configurare RSR:
  - `external/Core_Contract_SDK_DB3/core_contract_db3_rsr_config.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_rsr_feature_config.h`
- Include-uri RSR migrate pe contract:
  - `external/can.sdk.db3/APP/features/dball3/lib.rsr/rsr_private.h`
  - `external/can.sdk.db3/APP/features/dball3/lib.rsr_feature/rsr_feature_private.h`

### Validation
- Keil DB3 build executat din `FRD37EU_DB3.uvproj`.
- Rezultat: `UV4_EXIT_CODE=0`.
- Guard legacy GPIO executat:
  - `build/scripts/check_noncore_legacy_gpio.ps1`
  - rezultat: `Tracked files: 0`.
- Bench test raportat: OK.

### Notes
- După această migrare, fișierele RSR legacy din `inc.config.fw` pot fi scoase/renumite fizic în siguranță, cu reconfirmare prin rebuild complet.
  - `external/can.sdk.db3/APP/services/control/lib.pts_ctl/pts_ctl_init.c`
  - `external/can.sdk.db3/APP/services/status/lib.status/status.h`
  - `external/can.sdk.db3/APP/vehicle/diagnostics/com.ext.lib.obd_can/obd_can_rx_bat_volt.c`
  - `external/can.sdk.db3/APP/vehicle/diagnostics/com.ext.lib.obd_can/obd_can_rx_fuel_level.c`
  - `external/can.sdk.db3/CONTRACT_SDK_DB3/pts_ctl_config_wrapper.h`
  - `external/can.sdk.db3/CONTRACT_SDK_DB3/status_config_wrapper.h`
- Keil project DB3 actualizat:
  - `FRD37EU_DB3.uvproj` (înlocuire referințe `config_pts_ctl.h`/`config_status.h` cu wrapper-ele contract și include path pentru `CONTRACT_SDK_DB3`)

### Bench note
- Pentru validare runtime pe doors trace, s-a adăugat include-ul contract PTS în:
  - `VEHICLE_SPECIFIC/com.lib.sense/sense_doors.c`
- Pentru a asigura popularea valorilor PTS înainte de test runtime:
  - `core_contract_db3_pts_ctl_config_init_defaults()` este apelat în `fw_identity_init()`.

### Validation
- Keil DB3 build executat din `FRD37EU_DB3.uvproj`.
- Rezultat: `UV4_EXIT_CODE=0`.

### Tag
- Milestone tag publicat: `db3-pts-status-decoupling-2026-06-29`
