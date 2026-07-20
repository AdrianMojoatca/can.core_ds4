# Release Notes - 2026-06-24

## Scope
- External SDK synchronization and vehicle-specific configuration isolation alignment.

## Changes included
- Updated external SDK integration state for DB3 and DS4 via submodule repositories:
  - `external/can.sdk.db3`
  - `external/can.sdk.ds4`
- Added/updated external knowledge artifacts under:
  - `external/WIKI/`

## Context and intent
- Continue transition toward a core-independent architecture where runtime and firmware behavior are configured from vehicle-specific layers.
- Preserve and checkpoint external-related integration work to avoid local loss and to keep traceability for subsequent isolation steps.

## Validation
- Keil build executed for DB3 target from `FRD37EU_DB3.uvproj`.
- Result: build command completed successfully (exit code 0).

## Risks / Notes
- External SDK updates may introduce behavioral differences depending on submodule commit alignment with downstream consumers.
- Follow-up validation is recommended on DS4 target and key runtime smoke scenarios.
