---
name: ti-clang-code-coverage
description: Generate a code coverage or profiling report for a TI Clang project. Use when the user asks for code coverage, coverage report, profiling instrumentation, or wants to measure test coverage on an embedded target.
allowed-tools: Bash, Read, Glob, Grep
---

# Code Coverage for TI Arm Clang Projects

Generate a code coverage or profiling report for a project compiled with the TI Arm Clang compiler.

## High-Level Flow

1. Add code coverage compiler flags to the project `.cproject` file
2. Build the project
3. Debug/run the application on target for a user-determined amount of time
4. Extract profiling counters from target memory (produces a `.cnt` file)
5. Generate the .profdata file `tiarmprofdata`
6. Generate the coverage report using `tiarmcov`
7. Ask user if coverage flags should be removed

## Step 1 - Add Compiler Flags to .cproject

### Mandatory Flags

Add BOTH of these flags to the project's `.cproject` file under the compiler options:

- `com.ti.ccstudio.buildDefinitions.TMS470_TICLANG_4.0.compilerID.FCOVERAGE_MAPPING`
- `com.ti.ccstudio.buildDefinitions.TMS470_TICLANG_4.0.compilerID.FPROFILE_INSTR_GENERATE`

These correspond to `-fcoverage-mapping` and `-fprofile-instr-generate` compiler flags.

### Optional Flags

Ask the user if they want any of these optional flags:

| Flag ID | CLI Equivalent | Purpose |
|---------|---------------|---------|
| `FMCDC` | `-fmcdc` | Enable MC/DC (Modified Condition/Decision Coverage). Increases instrumentation cost. |
| `FFUNCTION_COVERAGE_ONLY` | `-ffunction-coverage-only` | Function-level coverage only. Decreases instrumentation cost. |
| `FPROFILE_COUNTER_SIZE` | `-fprofile-counter-size=[32\|64]` | Use 32-bit counters instead of default 64-bit. Decreases instrumentation cost. |

### XML Templates

Add `<option>` elements inside the `<tool>` element for the Arm Compiler (the `<tool>` whose `superClass` contains `compilerDebug`). Use unique numeric IDs (e.g., `100000001`, `100000002`, ...) that do not collide with existing option IDs in the file.

**Mandatory flags** (boolean, `value="true"`):

```xml
<option id="...FCOVERAGE_MAPPING.100000001"
        superClass="com.ti.ccstudio.buildDefinitions.TMS470_TICLANG_4.0.compilerID.FCOVERAGE_MAPPING"
        value="true" valueType="boolean"/>
<option id="...FPROFILE_INSTR_GENERATE.100000002"
        superClass="com.ti.ccstudio.buildDefinitions.TMS470_TICLANG_4.0.compilerID.FPROFILE_INSTR_GENERATE"
        value="true" valueType="boolean"/>
```

**Optional flags:**

```xml
<!-- MC/DC (boolean) -->
<option id="...FMCDC.100000003"
        superClass="com.ti.ccstudio.buildDefinitions.TMS470_TICLANG_4.0.compilerID.FMCDC"
        value="true" valueType="boolean"/>

<!-- Function coverage only (boolean) -->
<option id="...FFUNCTION_COVERAGE_ONLY.100000004"
        superClass="com.ti.ccstudio.buildDefinitions.TMS470_TICLANG_4.0.compilerID.FFUNCTION_COVERAGE_ONLY"
        value="true" valueType="boolean"/>

<!-- Profile counter size (string) — IMPORTANT: value MUST include the leading '=' sign -->
<!-- CCS concatenates the flag name with the value directly, so value="32" produces   -->
<!-- the INVALID flag -fprofile-counter-size32. Use value="=32" to get the correct     -->
<!-- flag -fprofile-counter-size=32.                                                    -->
<option id="...FPROFILE_COUNTER_SIZE.100000005"
        superClass="com.ti.ccstudio.buildDefinitions.TMS470_TICLANG_4.0.compilerID.FPROFILE_COUNTER_SIZE"
        value="=32" valueType="string"/>
```

**Reference**: Chapter 12 of the TI Arm Clang Compiler Tools User's Guide describes the full code coverage tools and collection process.

### Resolving Tool Paths

Several steps require paths that can be derived from the project and CCS installation:

- **CCS installation path** (`<ccspath>`): Read from `.claude/ccs.settings.md`.
- **Compiler tools path** (`<tiarmclang path>`): To determine compiler version, read the project's `.cproject` file and find the `OPT_CODEGEN_VERSION` option (e.g., `value="TICLANG_4.0.4.LTS"`). Convert the version to the directory name by indexing the compilers returned by the getCompilers MCP tool call. Note: `device.toolVersions` from the project descriptor lists all *compatible* compilers for the device, not the one actually configured for the project — do not use it for path resolution.
- **CCXML path**: Use the project descriptor's `activeTargetConfiguration` field, resolved relative to the project `location` (e.g., `<project location>/targetConfigs/MSPM0G3507.ccxml`).
- **Executable path**: `<project location>/<activeBuildConfiguration>/<project name>.out` (e.g., `Debug/adc_to_uart_LP_MSPM0G3507_nortos_ticlang.out`).
- **Core pattern** for `--core`: Derive from the project descriptor's `device.compatibleCores[0].id`. Map the core ID to a short pattern: `CORTEX_M0P` → `M0`, `CORTEX_M4` → `M4`, `CORTEX_M33` → `M33`, `CORTEX_R5` → `R5`. If the mapping is unclear, fall back to the first word after the underscore in the core ID.

## Step 2 - Build the Project

Use `buildProject` to compile the project with the coverage flags enabled. Verify the build succeeds before proceeding.

## Step 3 - Run the Application

The application must be loaded and run on the target for a sufficient duration to exercise the code paths the user wants to measure. The user determines when enough execution has occurred.

- Use `debugProject` to start a debug session
- Use `continue` to run the application
- Wait for the user to indicate the application has run sufficiently
- Use `pause` to halt execution before extracting data

## Step 4 - Extract Coverage Data from Target Memory

### Prerequisites
- Project was built with `-fprofile-instr-generate` and `-fcoverage-mapping`
- Terminate any active debug session

For Linux or MacOS: execute <ccspath>/ccs/scripting/run.sh with arguments:
For Windows: execute <ccspath>/ccs/scripting/run.bat with arguments:

1) the path to the script in ./scripts/extract_profile_counters.js
2) the path to the ccxml file used to debug,
3) the path to the executable file used to debug,
4) the path to the generated file with counts <.cnt>
5) the option --core <corePattern>, like M0, M4, R5, etc

## Step 5 - Generate the .profdata

Execute <tiarmclang path>/tiarmprofdata merge -sparse -obj-file=<.out file> <.cnt file> -o <.profdata file>

- `tiarmprofdata` to merge raw profile data

Multiple coverage runs can be made and combined with tiarmprofdata and a report generated for the union of the data with tiarmcov.

## Step 6 - Generate the Report

Execute <tiarmclang path>/tiarmcov show --format=html --show-instantiations --show-branches=count
   --object=<.out file> -instr-profile=<.profdata file> --output-dir=<cov report directory>

if the MCDC option was selected, then add the --show-mcdc-summary option to the tiarmcov command

- `tiarmcov`      to generate the coverage report

Present the final report path to the user.

## Step 7 - Ask user if the coverage compiler options should be removed
