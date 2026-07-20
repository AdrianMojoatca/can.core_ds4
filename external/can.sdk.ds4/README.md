# DS4 SDK

## Overview

This is a **generic SDK** providing reusable firmware components for automotive security and remote start systems. The SDK implements a three-layer architecture (APP/HAL/BSP) with clear separation of concerns. Vehicle-specific customizations and build configurations are maintained in separate repositories.

### Key Characteristics
- **Generic & Reusable:** Platform-agnostic libraries used across multiple vehicle models
- **Layered Architecture:** Clean separation between application logic, hardware abstraction, and low-level drivers
- **Modular Design:** 195 independent libraries organized by functional domain
- **Communication Support:** CAN, D2D, RF, HOBP, OBD-II protocols
- **Security Features:** AES-128 encryption, DST immobilizer, key management

---

## Architecture

The SDK follows a strict three-layer architecture with enforced dependency rules:

```
┌─────────────────────────────────────────────────┐
│  APP (Application Layer)                        │
│  Features, services, vehicle control logic      │
└─────────────────────────────────────────────────┘
                      ↕
┌─────────────────────────────────────────────────┐
│  HAL (Hardware Abstraction Layer)               │
│  Protocols, OS services, I/O abstraction        │
└─────────────────────────────────────────────────┘
                      ↕
┌─────────────────────────────────────────────────┐
│  BSP (Board Support Package)                    │
│  Peripheral drivers, MCU-specific code          │
└─────────────────────────────────────────────────┘
```

### Layer Responsibilities

| Layer | Purpose | Dependencies |
|-------|---------|--------------|
| **APP** | High-level application logic | HAL, BSP (through HAL) |
| **HAL** | Hardware abstraction & protocols | BSP only |
| **BSP** | Low-level drivers & peripherals | None (lowest layer) |
| **ENTRY** | Application entry points | APP, HAL, BSP |
| **INC** | Configuration headers | Included by all layers |

---

## Directory Structure

```
SDK/
├── APP/                    # Application Layer (81 libraries)
│   ├── features/           # Product features (DEI, Falcon, NGMM)
│   ├── framework/          # System framework and status management
│   ├── services/           # Core services (queues, events, sensors)
│   ├── utilities/          # Utility libraries (CRC, bitfield)
│   └── vehicle/            # Vehicle-specific logic
│       ├── diagnostics/    # VIN, OBD-CAN diagnostics
│       ├── programming/    # Device programming
│       └── security/       # DST, key management
│
├── BSP/                    # Board Support Package (37 libraries)
│   ├── bootloader/         # Memory scatter/linker configuration
│   ├── mcu/                # MCU-specific implementations (LPC1700)
│   └── peripherals/        # Drivers (CAN, GPIO, UART, timers, etc.)
│
├── HAL/                    # Hardware Abstraction Layer (24 libraries)
│   ├── communication/      # CAN, D2D, HOBP, RF protocols
│   ├── io/                 # I/O abstraction (LED, input drivers)
│   ├── os/                 # Task scheduler, reset handler
│   ├── power/              # Power/wake management
│   ├── security/           # AES-128, authentication
│   └── storage/            # NVFS (non-volatile file system)
│
├── ENTRY/                  # Entry Points (2 libraries)
│   ├── lib.main_1700/      # Main application entry
│   └── lib.ngmm_task_main/ # NGMM task initialization
│
├── INC/                    # Configuration Headers (9 collections)
│   ├── cmall/              # Project-specific config
│   └── common/             # Shared configuration
│
└── UNUSED/                 # Deprecated/Legacy (42 libraries)
```

---

## Library Components Summary

### Application Layer (APP) - 81 Libraries
**Features:** DEI security systems, Falcon telematics, NGMM management, display/brand control  
**Services:** Command processing, events, outputs (LED/siren), queues, sensors, status, timing, trace  
**Utilities:** CRC, bitfield, parking light, registry, NPC, EIPS  
**Vehicle:** VIN/OBD-II diagnostics, device programming, DST security, key management  

### Hardware Abstraction Layer (HAL) - 24 Libraries
**Communication:** CAN protocol stack, D2D multi-port, HOBP immobilizer bypass, RF (Keeloq)  
**Services:** I/O abstraction, task scheduler, power/wake management, AES-128 security, NVFS storage  

### Board Support Package (BSP) - 37 Libraries
**Peripherals:** ADC/DAC, CAN controller, GPIO, FIQ/IRQ/VIC interrupts, UART/SIO/HOBP bus, RTC/timers, watchdog  
**MCU-Specific:** LPC1700 device drivers, LED/output controllers, NVFS/OS implementations

---

## Library Naming Convention

| Pattern | Purpose | Example |
|---------|---------|---------|
| `lib.<name>` | Core library | `lib.led`, `lib.queue` |
| `com.lib.<name>` | Common/shared library | `com.lib.dst` |
| `com.ext.lib.<name>` | External/vendor library | `com.ext.lib.can` |
| `lib.<name>_<mcu>` | Hardware-specific | `lib.main_1700`, `lib.os_1700` |
| `inc.<category>` | Configuration collection | `inc.config_fw` |
| `config_<feature>.h` | Feature configuration | `config_can.h` |

---

## Library Categories

### By Layer Distribution
- **APP Layer:** 81 libraries (application features, services, utilities, vehicle logic)
- **HAL Layer:** 24 libraries (communication protocols, I/O, OS, security, storage)
- **BSP Layer:** 37 libraries (peripheral drivers, MCU-specific implementations)
- **ENTRY Layer:** 2 libraries (application entry points)
- **INC Layer:** 9 configuration collections (headers and build-time constants)
- **UNUSED Layer:** 42 libraries (deprecated/legacy code retained for reference)

### By Functional Category
- **Communication:** CAN, D2D, HOBP, RF (Keeloq), serial (UART, SIO)
- **Services:** Queues, events, outputs, sensors, status, timing, trace
- **Features:** DEI security systems, Falcon telematics, NGMM management
- **Vehicle:** Diagnostics (VIN, OBD-II), programming, security (DST, keys)
- **Peripherals:** ADC/DAC, GPIO, timers, interrupts, watchdog
- **Utilities:** CRC, bitfield, registry, parking light control

---

## Usage

This SDK is designed to be integrated into vehicle-specific firmware projects. Vehicle customization is achieved through:

1. **Configuration Headers** (`INC/` layer) - Compile-time feature selection and GPIO mapping
2. **Vehicle-Specific Libraries** (maintained separately) - Detection, control functions, sensor reading
3. **Build System Integration** (separate repository) - Project-specific build configurations and toolchain setup

For build instructions and vehicle-specific implementations, refer to the corresponding vehicle firmware repository

---

## Configuration Management

The SDK uses compile-time configuration headers organized in three categories:

- **Project-Specific:** `INC/cmall/inc.config_fw/` - Feature flags, GPIO mappings, module-specific settings
- **Hardware-Specific:** `INC/cmall/core/` - MCU configuration, D2D settings, core version definitions

---

## Architecture Documentation

- **Proposed architecture:** `docs\current_architecture.txt`

---

## Document History

| Date | Modified By | Changes |
|------|------------|---------|
| 18.03.2026 | Copilot | Created README.md from current_architecture.txt - Documented generic SDK architecture with 195 libraries organized in three-layer design (APP/HAL/BSP). Removed vehicle-specific and build sections to emphasize SDK's generic, reusable nature. |


