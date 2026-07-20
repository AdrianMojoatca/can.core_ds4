# Release Notes - 2026-07-02

## Scope (DB3 Core/FW split foundation)
- Implementare infrastructură pentru separare Core/FW cu update independent de Core.
- Introducere contract ABI + handshake la boot + API table pentru boundary controlat.
- Enforcement inițial non-core boundary prin guard dedicat în pipeline DB3.
- Migrare incrementală `gpio_iface` către strat FW -> ABI table.

## Changes included

### 1) ABI contract și metadata (Core Contract SDK)
- Adăugate headere noi:
  - `external/Core_Contract_SDK_DB3/core_contract_db3_abi.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_capabilities.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_api_table.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_boot_handshake.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_image_layout.h`
  - `external/Core_Contract_SDK_DB3/core_contract_db3_fw_api.h`
- Definite versiune ABI/API, capabilities mask și validare compatibilitate.

### 2) Split-image artifacts (Core/FW)
- Adăugat script de split HEX:
  - `build/scripts/split_db3_hex_images.ps1`
- Integrat în `AfterMake` (target DB3) pentru generare automată:
  - `main_db3_release.core.hex`
  - `main_db3_release.fw.hex`

### 3) Handshake la boot și fail-safe
- În `external/can.sdk.db3/ENTRY/lib.task_main/task_main.c` s-a adăugat validare ABI/capabilities după `core_init()`.
- La incompatibilitate: fail-safe explicit (LED roșu + loop controlat).

### 4) Boundary enforcement (non-core)
- Adăugat guard dedicat:
  - `build/scripts/check_noncore_core_boundary.ps1`
  - `build/scripts/noncore_core_boundary_allowlist.txt`
- Integrat în `BeforeMake` (target DB3), blocând doar violări noi față de baseline.

### 5) Migrare incrementală `gpio_iface` la ABI table
- `gpio_iface` nu mai apelează direct semantic helpers core, ci folosește wrapper-ul FW (`core_contract_db3_fw_api.h`) care merge prin API table.
- Fișiere migrate (submodule `external/can.sdk.db3`):
  - `APP/services/gpio_iface/gpio_iface_private.[ch]`
  - `APP/services/gpio_iface/gpio_iface_clutch.c`
  - `APP/services/gpio_iface/gpio_iface_handbrake.c`
  - `APP/services/gpio_iface/gpio_iface_doors.c`
  - `APP/services/gpio_iface/gpio_iface_lights.c`
  - `APP/services/gpio_iface/gpio_iface_trunk.c`
  - `APP/services/gpio_iface/gpio_iface_pts_1.c`
  - `APP/services/gpio_iface/gpio_iface_pts_2.c`
  - `APP/services/gpio_iface/gpio_iface_rap.c`
  - `APP/services/gpio_iface/gpio_iface_rf_loop.c`

### 6) Build identity
- Actualizat `VEHICLE_SPECIFIC/lib.config.db3/com.inc.config.fw/build_identity.h`:
  - `BUILD_FW_VERSION = "1.00_91"`

## Validation
- Build DB3 (Keil UV4): `UV4_EXIT_CODE=0` după fiecare etapă majoră.
- Test bench utilizator (build `1.00_91`): flash + execuție funcțională -> OK.
- Guard core-boundary: `OK: no new non-core core-boundary violations`.

## Notes
- În stadiul actual, split-ul livrează infrastructură și artefacte separate; distribuția efectivă a componentelor între imaginea Core și FW continuă incremental în pașii următori.
