# unirtos-lvgl-demos

[中文](README.zh.md) | English

This repository is recommended to be used via the `unirtos-cli` demo workflow
to ensure a consistent process for project creation, environment setup, and
compilation.

## Feature Description

This demo demonstrates how to integrate the independently versioned
`unirtos-lvgl` library with an EG800ZCN_LA ST7796S display. It is a reference
for LVGL graphics porting and LCD application development on UniRTOS.

- Uses LVGL v8.3.11 with RGB565 colour format and a 320x40-pixel partial-refresh buffer
- Supports an ST7796S 320x480 panel through the EG800Z LSPI interface
- Demonstrates display initialization, local refresh, title, status label,
  progress bar, visual button, RGB colour blocks, and timer animation
- Does not include touch, keypad, filesystem images, network UI, or Chinese fonts
- Keeps LVGL source in the independent `unirtos-lvgl` library; the demo only
  contains the ST7796S driver, display port, hardware mapping, and UI

## Hardware Requirements

- EG800ZCN_LA module and an ST7796S 320x480 panel
- ST7796S connected through the validated LSPI wiring for the target board
- UniRTOS SDK `v1.0.4` with an LCD-enabled `gccout.7z` base firmware archive

> **Note:** The default SDK base archive may not include the required LCD API.
> Replace it with the platform-approved LCD-enabled `gccout.7z` before building.
> The archive validated during development was 14,181,489 bytes with SHA-256
> `FED45CB25E18ECB7311524651D58C6EEE580A841C73A8ECEC70410F03EC19421`.

## Quick Start

### 1. Install the UniRTOS Toolchain

- [Development Preparation](https://www.quectel.com.cn/unirtos/docs?docs_page=快速上手/开发准备/开发准备.html)
- [Install the Cross-Compilation Toolchain](https://www.quectel.com.cn/unirtos/docs?docs_page=快速上手/环境搭建/环境搭建.html)
- [Install Python3](https://www.python.org/downloads/)
- [Install git](https://git-scm.com)
- Install `unirtos-cli`: `pip install unirtos-cli`

Once all the above tools are installed, verify the following commands are available:

```bash
python --version    # Python3
git --version
unirtos --version   # version 1.0.5 or above
unirtos-cli version # version 1.0.11 or above
```

### 2. Pull the Demo Using unirtos-cli

List available demos and versions:

```bash
unirtos-cli ls-demos
```

Create this demo project:

```bash
unirtos-cli new -r unirtos-lvgl-demos
```

To specify a version:

```bash
unirtos-cli new -r unirtos-lvgl-demos -v 1.0.0
```

### 3. Enter the Project and Build

```bash
cd unirtos-lvgl-demos-1.0.0
unirtos-cli env-setup
unirtos-cli build
```

`env-setup` obtains `unirtos-lvgl` v8.3.11 as declared in `env_config.json`.
The SDK compiles and links LVGL only for this project; projects that do not
declare this dependency do not include LVGL.

The build output is located at:

```text
qos_build/release/<project-name>/at_command.hbinpkg
```

## Configuration Notes

- Keep `CONFIG_QOSA_LCD_FUNC=y` enabled in `menuconfig/.config`.
- The display port allocates its RGB565 draw buffer with `qosa_malloc()` so it
  is visible to the LSPI DMA engine.
- The demo uses a 5 ms LVGL tick and yields between display transfers to avoid
  starving system tasks.

## Common Commands

```bash
# Open the SDK menu configuration
unirtos-cli menuconfig

# Clean build artifacts
unirtos-cli clean
```

## Technical Community

Technical Community: https://forumschinese.quectel.com/c/66-category/66

## Contribution Guidelines

Contributions are welcome. Please follow these guidelines when submitting:

- Run a basic validation before submitting: `env-setup`, `build`, and `clean`.
- Use clear commit messages describing the purpose of the change, its scope of
  impact, and verification results.
- When changing a driver, pin mapping, SDK dependency, or display behaviour,
  update this README and record the hardware verification result.
- Submit bug fixes and feature improvements via Issues or Pull Requests.
