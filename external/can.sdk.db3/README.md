# DBALL3 (DB3) Firmware

## Overview

This is the **DBALL3 embedded firmware** providing automotive security, remote start, and vehicle integration functionality. The firmware implements a three-layer architecture (APP/HAL/BSP) with clear separation of concerns and is built for the NXP LPC1766 (ARM Cortex-M3) microcontroller.

### Key Characteristics
- **Three-Layer Architecture:** Clean separation between application logic, hardware abstraction, and low-level drivers
- **Modular Design:** 77 independent libraries organized by functional domain
- **Remote Start System:** Complete RSR state machine with timer extension and feature management
- **Communication Support:** CAN, D2D, D2DI (UART), RF, HOBP protocols
- **Security Features:** DST immobilizer, key management, secure lock mechanisms
- **MCU Target:** NXP LPC1766 (ARM Cortex-M3)

---

## Architecture

The firmware follows a strict three-layer architecture with enforced dependency rules:

```
┌─────────────────────────────────────────────────┐
│  APP (Application Layer)                        │
│  Features, services, vehicle control logic      │
│  ~34 libraries                                  │
└─────────────────────────────────────────────────┘
                      ↕
┌─────────────────────────────────────────────────┐
│  HAL (Hardware Abstraction Layer)               │
│  Protocols, OS services, I/O abstraction        │
│  ~15 libraries                                  │
└─────────────────────────────────────────────────┘
                      ↕
┌─────────────────────────────────────────────────┐
│  BSP (Board Support Package)                    │
│  Peripheral drivers, MCU-specific code          │
│  ~26 libraries                                  │
└─────────────────────────────────────────────────┘
```

### Layer Responsibilities

| Layer | Purpose | Dependencies |
|-------|---------|--------------|
| **APP** | High-level application logic, DBALL3 features | HAL, BSP (through HAL) |
| **HAL** | Hardware abstraction & protocols | BSP only |
| **BSP** | Low-level drivers & peripherals | None (lowest layer) |
| **ENTRY** | Application entry points | APP, HAL, BSP |
| **INC** | Configuration headers | Included by all layers |

---

## Directory Structure

```
SDK/
├── APP/                    # Application Layer (~34 libraries)
│   ├── features/           # Product features
│   │   ├── dball3/         # DBALL3-specific (remote start, W2W, display)
│   │   └── falcon/         # Falcon telematics (minimal implementation)
│   ├── framework/          # System framework and status management
│   ├── services/           # Core services
│   │   ├── control/        # Command processing, datalink control
│   │   ├── events/         # Event notification
│   │   ├── outputs/        # LED and output control
│   │   ├── queues/         # Queue and execution services
│   │   ├── sensors/        # Tachometer interface
│   │   ├── status/         # Status monitoring
│   │   ├── timing/         # Timeout management
│   │   └── trace/          # Debug and trace logging
│   ├── utilities/          # Utility libraries (CRC, bitfield, etc.)
│   └── vehicle/            # Vehicle-specific logic
│       ├── diagnostics/    # VIN, OBD-CAN diagnostics
│       ├── programming/    # Device programming
│       └── security/       # DST, key management
│
├── BSP/                    # Board Support Package (~26 libraries)
│   ├── bootloader/         # Memory scatter/linker configuration
│   ├── mcu/                # MCU-specific implementations
│   │   ├── device/         # Device abstraction (3 libraries)
│   │   └── lpc1700/        # LPC1700-specific drivers (7 libraries)
│   └── peripherals/        # Peripheral drivers
│       ├── analog/         # ADC/DAC drivers
│       ├── can/            # CAN controller
│       ├── gpio/           # GPIO drivers
│       ├── interrupts/     # FIQ/IRQ/VIC handlers
│       ├── serial/         # UART, SIO, HOBP bus
│       ├── timers/         # Timer peripherals
│       └── watchdog/       # Watchdog timer
│
├── HAL/                    # Hardware Abstraction Layer (~15 libraries)
│   ├── communication/      # Communication protocols
│   │   ├── can/            # CAN vehicle protocol (2 libraries)
│   │   ├── d2d/            # Device-to-Device (2 libraries)
│   │   ├── d2di/           # D2DI over UART (2 libraries)
│   │   ├── hobp/           # Honda immobilizer (1 library)
│   │   └── rf/             # RF communication (4 libraries)
│   ├── io/                 # I/O abstraction (LED, input drivers)
│   ├── os/                 # Task scheduler
│   ├── power/              # Power/wake management
│   ├── security/           # Secure lock mechanism
│   └── storage/            # NVFS abstraction
│
├── ENTRY/                  # Entry Points (2 libraries)
│   ├── lib.main_1700/      # Main application entry (crt0.s + main.c)
│   └── lib.task_main/      # DBALL3 task initialization
│
├── INC/                    # Configuration Headers
│   ├── common/             # Common core headers
│   ├── dball/              # DBALL3 configurations (D2D, firmware)
│   └── falcon/             # Falcon configurations
│
└── UNUSED/                 # Archived Code (historical preservation)
```

---

## Library Components Summary

### Application Layer (APP) - ~34 Libraries
**Features:** 
- **DBALL3 Features:** Remote Start (RSR/RXT), Wire-to-Wire (W2W), Display management, Product info
- **Falcon Telematics:** Minimal core implementation

**Services:** 
- Command processing and control
- Event notification bridge
- Output management (LED, siren, generic outputs)
- Queue infrastructure (FIFO, execution queues)
- Sensor reading (hardware tachometer)
- Status monitoring and reporting
- Timing and timeout services
- Debug trace and logging

**Utilities:** 
- CRC calculation, bitfield manipulation, EIPS, registry management

**Vehicle Integration:** 
- Diagnostics: VIN reading, OBD-II CAN
- Programming: Device programming and configuration
- Security: DST immobilizer, DST encryption, key management

### Hardware Abstraction Layer (HAL) - ~15 Libraries
**Communication:** 
- CAN vehicle protocol stack (2 libraries)
- D2D multi-device communication (2 libraries)
- D2DI protocol over UART - DBALL3-specific (2 libraries)
- HOBP Honda immobilizer bypass (1 library)
- RF wireless (Keeloq) communication (4 libraries)

**Services:** 
- I/O abstraction (input debounce, edge detection, LED control)
- Task scheduler and OS services
- Power/wake management
- Secure lock mechanism
- NVFS storage interface

### Board Support Package (BSP) - ~26 Libraries
**Peripherals:** 
- Analog: ADC/DAC drivers (3 libraries)
- CAN: Controller driver with lookup table (2 libraries)
- GPIO: Platform layer and wake-up handling (2 libraries)
- Interrupts: FIQ, IRQ, ISR, VIC (4 libraries)
- Serial: UART, SIO, HOBP bus (3 libraries)
- Timers: Timer peripheral driver (1 library)
- Watchdog: Watchdog timer driver (1 library)

**MCU-Specific:** 
- Device identification and hardware version detection (3 libraries)
- LPC1700 implementations: device driver, GPIO, NVFS, OS, system, wake (7 libraries)

**Bootloader:** 
- Memory scatter/linker configuration for LPC1766 (1 library)

---

## DBALL3-Specific Features

The DB3 firmware includes unique features designed for remote start and security systems:

### Remote Start System (RSR/RXT)
- **lib.rsr:** Complete remote start state machine and control logic
- **lib.rxt:** Remote start timer extension and scheduling
- **lib.rsr_feature:** Feature flag management for remote start options
- **Integration:** Works with vehicle detection, sensing, and function control

### Wire-to-Wire (W2W)
- **lib.w2w:** Wire-to-wire integration for cascaded module installations
- **Purpose:** Enables multiple DBALL3 units to share vehicle wire connections

### D2DI Protocol (Device-to-Device over UART)
- **lib.d2di:** D2DI protocol layer
- **lib.d2di_uart:** D2DI UART driver
- **Purpose:** DBALL3-specific D2D variant providing bidirectional module communication via serial link

### Display Management
- **lib.display:** LED pattern-based user feedback system
- **Purpose:** Programming mode visual indicators and status display through on-board LEDs

### Hardware Tachometer Interface
- **lib.tach:** Hardware tachometer reading interface (DB3-specific)
- **Purpose:** Direct hardware interface for tachometer signal reading

### Product Information
- **lib.about:** Product identification, version reporting, and feature capability advertisement

---

## Library Naming Convention

| Pattern | Purpose | Example |
|---------|---------|---------|
| `lib.<name>` | Core library | `lib.led`, `lib.queue` |
| `com.lib.<name>` | Common/shared library | `com.lib.dst` |
| `com.ext.lib.<name>` | External/vendor library | `com.ext.lib.can` |
| `com.ext.<name>` | External protocol/bus | `com.ext.hobp_type6` |
| `lib.<name>_<mcu>` | Hardware-specific | `lib.main_1700`, `lib.os_1700` |
| `inc.config.*` | Configuration collection | `inc.config.fw` |
| `config_<feature>.h` | Feature configuration | `config_rsr.h` |

**Suffix Conventions:**
- `*_1700` / `*_1766` - LPC17xx MCU specific implementation
- `*.usr` - User-level interface/wrapper
- `*.v` - Vehicle-specific variant
- `*.f` - Fast variant (optimized implementation)
- `*.machine` - Machine/assembly-level code

---

## Library Categories

### By Layer Distribution
- **APP Layer:** ~34 libraries (features, services, utilities, vehicle logic)
- **HAL Layer:** ~15 libraries (communication protocols, I/O, OS, security, storage)
- **BSP Layer:** ~26 libraries (peripheral drivers, MCU-specific implementations)
- **ENTRY Layer:** 2 libraries (application entry points)
- **INC Layer:** Configuration collections (headers and build-time constants)
- **UNUSED Layer:** ~77 libraries (archived historical code)

### By Functional Category
- **Communication:** CAN, D2D, D2DI, HOBP, RF (13 libraries across HAL/BSP)
- **Remote Start:** RSR state machine, timer extension, feature management (3 libraries)
- **Services:** Queues, events, outputs, sensors, status, timing, trace (17 libraries)
- **Vehicle Integration:** Diagnostics, programming, security (8 libraries)
- **Peripherals:** ADC/DAC, GPIO, timers, interrupts, watchdog (15 libraries)
- **Utilities:** CRC, bitfield, registry, EIPS (4 libraries)

---

## Configuration Management

The firmware uses compile-time configuration headers organized in three categories:

- **Common Core:** `INC/common/core_release/` - Shared configuration templates
- **DBALL3-Specific:** `INC/dball/` - D2D settings, firmware config, RSR/RXT config
- **Falcon-Specific:** `INC/falcon/` - Falcon telematics configuration

### Key Configuration Files
- `config_dball.h` - DBALL3 main configuration
- `config_rsr.h` - Remote Start System configuration
- `config_rsr_feature.h` - Remote Start features
- `config_rxt.h` - Remote Start Timer configuration
- `config_d2d.h` - D2D protocol configuration

---

## CMake Migration Notes

The project is in the process of migrating from Keil uVision 4 to CMake. Key considerations:

### Layer-Based Build Targets
- **db3_bsp** - BSP layer libraries (lowest level)
- **db3_hal** - HAL layer libraries (depends on BSP)
- **db3_app** - APP layer libraries (depends on HAL)
- **db3_entry** - Application entry points

### MCU-Specific Configuration
- MCU Family: LPC1700
- MCU Variant: LPC1766
- Conditional compilation for MCU-specific code

### Feature Flags
Optional features that can be enabled/disabled:
- `ENABLE_RSR` - Remote start system
- `ENABLE_W2W` - Wire-to-wire integration
- `ENABLE_D2DI` - D2D over UART
- `ENABLE_HOBP` - Honda immobilizer bypass
- `ENABLE_FALCON` - Falcon telematics (minimal in current build)

---

## Architecture Documentation

- **Current architecture:** [db3_architecture_updated.txt](db3_architecture_updated.txt)
- **Original proposed structure:** [db3_architecture.txt](db3_architecture.txt)

---

## Critical Observations

### Architectural Strengths
✓ Clear three-layer separation (APP/HAL/BSP)  
✓ Consistent naming conventions across all libraries  
✓ MCU abstraction with generic + specific implementations  
✓ Vendor code clearly marked (`com.ext.*` prefix)  
✓ Multi-file libraries organized by function (init/get/set/vars)  
✓ Historical preservation in UNUSED/ folder  

### Communication Stack Emphasis
The communication layer is the most developed part of the system with 13 libraries spanning CAN, D2D, D2DI, RF, and HOBP protocols across HAL and BSP layers.

### LPC1700 Specialization
Strong coupling to NXP LPC1700 MCU family with 14 LPC-specific libraries. Porting to another MCU would require replacing BSP/mcu/ and updating ENTRY/lib.main_* and peripheral drivers.

### Falcon Minimalism
Unlike other variants, Falcon features are minimally implemented in this DB3 project, suggesting telematics is not a primary feature in this firmware variant.

---

## Document History

| Date | Modified By | Changes |
|------|------------|---------|
| 25.03.2026 | Copilot | Created README_DB3.md from db3_architecture_updated.txt - Documented DBALL3 firmware architecture with 77 active libraries organized in three-layer design (APP/HAL/BSP). Highlighted DB3-specific features including remote start system, W2W, D2DI protocol, and hardware tachometer interface. |
