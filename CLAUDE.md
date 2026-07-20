# CLAUDE.md

Guidance for Claude Code (claude.ai/code) when working in this repository.

> This file merges the project analysis with the operational rules from
> `.github/copilot-instructions.md`. The operational rules (Section "Working
> Agreement") are binding and take precedence over general defaults.

---

## 1. Project Overview

`can.fw.frd37eu` is an **embedded C firmware** for automotive modules (remote
start, immobilizer/security, vehicle integration). It targets the **NXP LPC176x
(ARM Cortex-M3)** MCU and is built with **Keil MDK / uVision 4 (UV4)**.

- **Two Keil targets** (`.uvproj`): `FRD37EU_DB3` and `FRD37EU_DS4`.
- **Device:** LPC1765/LPC1766 (Cortex-M3), 12 MHz clock. See `<Cpu>` in the
  `.uvproj` files for exact memory map.
- Product families: **DB3 (DBALL3)** and **DS4**.

### Repository boundaries (important)

| Path | Nature |
|------|--------|
| `can.fw.frd37eu/` | **The git repository root.** Commit here. |
| `FRD37EU_NEW/` (parent) | Working-dir wrapper only. **Not** a git repo. `.github/` and `out/` live here but are outside version control. |
| `external/can.sdk.db3` | **Git submodule** — shared DB3 SDK (`FalconElectronicsProd/can.sdk.db3`). |
| `external/can.sdk.ds4` | **Git submodule** — shared DS4 SDK (`FalconElectronicsProd/can.sdk.ds4`). |

Most firmware source lives in the **submodules**, not this repo. Vehicle- and
build-specific config lives here under `VEHICLE_SPECIFIC/`.

---

## 2. Architecture (SDK submodules)

Each SDK follows a strict three-layer architecture with enforced dependency
direction (APP -> HAL -> BSP; lower layers never depend on higher ones):

```
APP    Application logic: features (dball3: RSR/RXT, W2W, display), services
       (control, events, outputs, queues, sensors, status, timing, trace),
       utilities (crc, bitfield, eips, reg), vehicle (diagnostics, programming,
       security: DST immobilizer, key mgmt).
HAL    Hardware abstraction: communication (CAN, D2D, D2DI/UART, HOBP, RF),
       io, os (scheduler), power/wake, security (secure lock), storage (NVFS).
BSP    Board support: peripherals (analog, can, gpio, interrupts, serial,
       timers, watchdog) + MCU-specific (device/, lpc1700/) + bootloader scatter.
ENTRY  Entry points: lib.main_1700 (crt0.s + main.c), lib.task_main.
INC    Compile-time configuration headers (common / dball / falcon).
UNUSED Archived historical code — do not build or modify.
```

### Core / FW split (active initiative, 2026)

The DB3 platform is being split into an updatable **FW** image over a stable
**Core** image, connected by a versioned ABI contract:

- Contract headers under `external/can.sdk.db3/.../Core_Contract_SDK_DB3/`
  (`*_abi.h`, `*_capabilities.h`, `*_api_table.h`, `*_boot_handshake.h`,
  `*_image_layout.h`, `*_fw_api.h`).
- Boot-time ABI/capabilities handshake in `ENTRY/lib.task_main/task_main.c`;
  fail-safe (red LED + controlled loop) on incompatibility.
- Split HEX artifacts: `main_db3_release.core.hex` and `.fw.hex`.
- Non-core code must reach Core only through the FW API table
  (`core_contract_db3_fw_api.h`), never by including `core.h` / `lib.core/*`
  directly. This is enforced in the build (see guards below).

Read the latest `docs/ReleaseNotes_*.md` before changing anything in this area.

---

## 3. Build

Build is driven by **Keil UV4** (`Uv4.exe`), not CMake (a CMake migration is
noted as future work in the SDK README but is not the current build).

- Batch build entry point: `build/scripts/build_me.bat` (locates UV4 from the
  registry, builds selected targets, writes `log_build.txt`).
- Build guards / pre-build (run in `BeforeMake`/`AfterMake` and standalone):
  - `build/scripts/check_noncore_core_boundary.ps1` — fails on **new** non-core
    -> Core-boundary include violations vs. `noncore_core_boundary_allowlist.txt`.
  - `build/scripts/check_noncore_legacy_gpio.ps1` — legacy GPIO usage guard.
  - `build/scripts/split_db3_hex_images.ps1` — splits the DB3 HEX into core/fw.
  - `build/scripts/pre-build.pl`, `config_gpio.pl`, `templates_copy.pl`,
    `update_config_status.pl` — Perl pre-build steps.
- UV4 exit codes: `0` = OK, `1` = warnings only, `>=2` = errors.

Build outputs land in `build/db3_build/`, `build/ds4_build/`, `build/out/`.

---

## 4. Configuration

- Build/vehicle config: `VEHICLE_SPECIFIC/lib.config.db3` and
  `VEHICLE_SPECIFIC/lib.config.ds4` (`com.inc.config.fw`, `inc.config.d2d`).
- `build_identity.h` (e.g. `com.inc.config.fw/build_identity.h`) holds
  `BUILD_FW_VERSION`. Bump it on releases.
- SDK config headers: `INC/common`, `INC/dball`, `INC/falcon`; feature configs
  like `config_dball.h`, `config_rsr.h`, `config_rxt.h`, `config_d2d.h`.

---

## 5. Naming conventions (existing code)

Match the surrounding code. The established patterns are:

| Pattern | Meaning | Example |
|---------|---------|---------|
| `lib.<name>` | Library folder | `lib.led`, `lib.rsr` |
| `com.lib.<name>` | Common/shared library | `com.lib.dst` |
| `com.ext.lib.<name>` / `com.ext.<name>` | External/vendor code | `com.ext.lib.obd_can` |
| `lib.<name>_1700` / `_1766` | LPC17xx-specific | `lib.os_1700` |
| `*.usr` | User-level wrapper | |
| `*.v` | Vehicle-specific variant | |
| `*.f` | Fast/optimized variant | `lib.timeout.f` |
| `*.machine` | Assembly-level code | |

---

## 6. Working Agreement (from `.github/copilot-instructions.md` — binding)

These rules govern how work is done here. They override generic defaults.

### Fundamental principle
Deliver **working code + updated tests + coherent integration**. Do **not**
produce docs (README, guides, separate explanations) unless the user explicitly
asks (this CLAUDE.md and the project memory were explicitly requested).

### Language
- Replies to the user: **Romanian**.
- Code comments: English.
- If docs are requested: technical docs -> English, end-user docs -> Romanian.

### Mandatory workflow: Plan -> Confirm -> Implement
- Never write code straight from a request. First present a **numbered plan**.
- Each plan step states: purpose, files created/modified, classes/functions/
  modules affected, impact on the rest of the system, possible breaking changes,
  alternatives + rationale.
- Ask for explicit approval. After approval, implement **only the approved
  step**, summarize what was done, then ask before the next step.
- **API-first:** when starting to write code, first produce the function/method
  signatures with **only comments/docstrings inside (no implementation)** and
  present them for review before filling in the bodies.

### Review gates (independent subagents)
- **After an architectural decision**, launch an **architect subagent** to review
  that decision (soundness, layer/dependency fit, Core/FW boundary impact,
  alternatives, risks) and present its conclusions to the user before
  implementing against it.
- **After each larger coding stage**, launch an **independent code-review agent**
  on the produced changes and present its findings before moving on.
- These reviewers are independent (separate agent invocations), not the same
  context that produced the work. Do not skip them for non-trivial work.

### Implementation defines the contract
- Implementation is the source of truth; update tests to match correct
  implementation (not the reverse) unless the user says tests define the
  contract. Always give a copy/paste-ready test command.

### Scope control
- Do not add unrequested functionality. Mention improvements briefly and ask
  before implementing.

### Data structures
- Use dedicated models (struct/record/schema) with clear (de)serialization.
  Avoid generic dict/map/list for stable models.

### Logging & output
- No emoji, no production `print`. Use standard logging with levels
  (debug/info/warning/error).

### Configuration
- No hardcoded paths, no environment-dependent config in code. Config comes from
  config files, env vars, CLI args, or dependency injection.

### Dependencies
- Use the language's standard manager; separate runtime vs. dev deps; pin
  versions where reproducibility matters.

### Git
- Commit messages in English. Prefixes: `feat:`, `fix:`, `refactor:`, `chore:`,
  `docs:`. Large binaries -> LFS / artifact storage. Remember this repo uses
  **git submodules** — commit submodule changes in the submodule first.

### Backward compatibility
- **Off by default.** Assume data is in the latest format. Add compatibility /
  old-format checks only when explicitly requested. (Aligns with the Core/FW
  contract being versioned rather than legacy-tolerant.)

### Minimum quality
- Compilable/runnable, no obvious warnings, boundary-case input validation,
  explicit error handling, tests for key behaviors and edge cases.

### Explicit prohibitions
- No unrequested README/docs, no announcing internal tools, no implicit
  backward compat, no leftover debug prints, no generic containers instead of
  clear models.

### Audit & traceability (mandatory for test scripts)
Any test script must be fully auditable. Each run writes `artifacts/<run_id>/`
containing: `run_metadata.json` (timestamps, duration, run_id, seed, runtime
versions, effective config, commit/build id, inputs + SHA-256), `summary.txt`
(human-readable), `stats.json` (machine-readable metrics), `events.jsonl`
(append-only structured log: ts, level, component, event_type, message,
context), `outputs/` (raw files), `checksums.txt` (SHA-256 of all artifacts).
Rules: stable file names, relative paths, console shows only the artifacts path
+ a 3-10 line summary. Determinism: save seed, list+hash inputs, save exact
config. Hooks: verify all mandatory artifacts exist; mark run FAIL if any are
missing. Do not use temp without metadata reference; do not make the console the
sole source of truth; keep app logs separate from audit logs.

### Code style & organization
- Explanations before code; comment only where it adds clarity; clear docstrings
  on methods/functions; descriptive names; private methods prefixed `_` where
  the language allows; public methods first then private; `main` last in file.
- Function separator (blank line above and below):
  ```
  # ------------------------------------------------------------
  ```
- Section headers:
  ```
  # ===========================
  # PUBLIC API
  # ===========================

  # ===========================
  # PRIVATE HELPERS
  # ===========================
  ```

---

## 7. TODO & commit conventions

- **`TODO.md`** (repo root, git-tracked) lists tasks proposed but still pending.
  Update it whenever a task is proposed, started, finished, or dropped.
- **Before every commit**, record a representative note of what that commit does
  (intent + impact, not just the file list) in the project memory
  (`.claude/memory/project-context.md`, "Commit log" section).

## 8. Quick reference

- Read `docs/ReleaseNotes_*.md` (latest first) and `external/can.sdk.db3/README.md`
  for architecture detail before non-trivial changes.
- Determine which target (DB3 vs DS4) and layer (APP/HAL/BSP) a change belongs
  to before editing; respect the layer dependency direction.
- For DB3, respect the Core/FW boundary — go through the FW API table, and run
  `check_noncore_core_boundary.ps1` before considering a change done.
