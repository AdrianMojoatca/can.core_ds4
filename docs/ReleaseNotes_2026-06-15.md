# Release Notes - 2026-06-15

## Scope

- Repo: can.fw.frd37eu
- Branch: main
- Commits incluse:
	- 7656cdc - chore: add core contract and update project references
	- b5cb6cf - chore: add VEHICLE_SPECIFIC baseline

## Ce s-a livrat

### 1) Core Contract DB3 (nou)

S-a adăugat folderul:

- external/Core_Contract_SDK_DB3

Conține contractele DB3 pentru config:

- core_contract_db3_can_config.[c/h]
- core_contract_db3_dbg_config.[c/h]
- core_contract_db3_firmware_config.[c/h]
- core_contract_db3_gpio_config.[c/h]
- core_contract_db3_obd_can_config.[c/h]
- core_contract_db3_pts_ctl_config.[c/h]
- core_contract_db3_status_config.[c/h]
- core_contract_db3_init.[c/h]

### 2) Actualizare proiect Keil

Au fost introduse/actualizate fișierele de proiect:

- FRD37EU_DB3.uvproj
- FRD37EU_DS4.uvproj

### 3) VEHICLE_SPECIFIC (baseline)

S-au adăugat în versionare directoarele și fișierele de bază din:

- VEHICLE_SPECIFIC/com.lib.detect
- VEHICLE_SPECIFIC/com.lib.func
- VEHICLE_SPECIFIC/com.lib.sense
- VEHICLE_SPECIFIC/lib.config.db3
- VEHICLE_SPECIFIC/lib.config.ds4
- VEHICLE_SPECIFIC/lib.fw.db3
- VEHICLE_SPECIFIC/lib.fw.ds4

### 4) Submodule

- external/can.sdk.db3 este actualizat la commit: 201f15b
	- include bootstrap contract în `core_init()` pentru firmware + dbg
	- include-uri adăugate: `core_contract_db3_firmware_config.h`, `core_contract_db3_gpio_config.h`, `core_contract_db3_dbg_config.h`
	- setări bench adăugate: `set_fw_name`, `set_app_version`, `set_use_trace`

### 5) Update versiune curentă

- APP_VERSION setat în bootstrap la `1.00_12` pentru testul curent de integrare contract

## În afara scope-ului (explicit)

- Fără commit pentru build artifacts
- Fără commit pentru external/can.sdk.ds4 (deocamdată)

## Tag-uri publicate azi (can.sdk.db3)

- db3-contract-v1
- v1.0.0-db3-contract
