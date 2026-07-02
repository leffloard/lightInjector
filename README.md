# lightInjector

A lightweight Windows injector utility built in C++ with minimal dependencies.

## Overview

`lightInjector` is designed to be a clean, maintainable injector prototype for research and tooling around process injection on Windows. It focuses on readable code, standard Visual Studio build conventions, and a small footprint.

## Repository Layout

- `injector/` - main injector application source and project files.
- `shared/` - shared libraries, helper code, and third-party dependencies used by the injector.

## What This Project Includes

- Visual Studio C++ solution and project files.
- A modular code base for a Windows injector.
- Cryptographic and helper libraries imported under `shared/`.

## Build Instructions

1. Open `injector.sln` in Visual Studio.
2. Set the desired configuration (`Debug`, `Release`, `x86`, `x64`).
3. Build the solution.

### Recommended settings

- Use the latest supported Windows SDK.
- Enable `Multi-Byte Character Set` only if required.
- Prefer `Runtime Library` set to `Multi-threaded DLL (/MD)` for release and `Multi-threaded Debug DLL (/MDd)` for debug.

## Project Structure

### `injector/`

- `entry.cpp` - likely the injector's main entry point.
- `injector.vcxproj` - Visual Studio project definition.
- `json.hpp` - single-header JSON parser.
- `utils.hpp` - utility functions and helpers.
- `xor.hpp` - XOR encoding/decoding utilities.

### `shared/`

This directory contains external libraries and crypto implementations. It is intentionally preserved under source control for reproducibility and to avoid external dependency resolution.

## Usage Guidance

This repository appears to be a development scaffold and not a packaged release. It is suitable for:

- investigation and learning about process injection patterns.
- integrating injector functionality into larger tooling.
- experimenting with Windows native APIs in a small, self-contained C++ project.

## Best Practices

- Keep generated build artifacts out of version control.
- Audit any external dependencies in `shared/` before using them in security-sensitive environments.
- Maintain clear project boundaries: injector-specific code stays in `injector/`, while shared utilities and libraries remain under `shared/`.

## Notes

- No `.git` repository metadata was detected in this workspace root, so this readme is written for a standalone source tree.
- If you intend to use Git, initialize the repository at the workspace root and create appropriate branches for development and release.
