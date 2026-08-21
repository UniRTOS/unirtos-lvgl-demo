# unirtos-lvgl-demos

[English](README.md) | 中文

推荐通过 `unirtos-cli` 的 demo 工作流使用本仓库，以保证工程创建、环境拉取和编译流程一致。

## 功能描述

本 Demo 演示如何在 EG800ZCN_LA + ST7796S 显示屏上集成独立版本的
`unirtos-lvgl` 库，可作为 UniRTOS 上 LVGL 图形移植和 LCD 应用开发的参考。

- 使用 LVGL v8.3.11、RGB565 色彩格式和 320×40 像素局部刷新缓冲区
- 通过 EG800Z LSPI 接口支持 ST7796S 320×480 面板
- 演示显示初始化、局部刷新、标题、状态标签、进度条、视觉按钮、RGB 色块和定时动画
- 不包含触控、按键、文件系统图片、网络 UI 或中文字体
- LVGL 源码由独立的 `unirtos-lvgl` 库提供；本 Demo 仅包含 ST7796S 驱动、显示 port、硬件映射和 UI

## 硬件要求

- EG800ZCN_LA 模组和 ST7796S 320×480 显示面板
- ST7796S 按目标板已验证的 LSPI 连接方式接线
- UniRTOS SDK `v1.0.4`，并使用启用 LCD 功能的 `gccout.7z` 基础固件包

> **注意：** SDK 默认基础包可能不包含本 Demo 所需的 LCD API。编译前必须替换为平台批准的、已启用 LCD 功能的 `gccout.7z`。开发验证使用的 archive 大小为 14,181,489 bytes，SHA-256 为 `FED45CB25E18ECB7311524651D58C6EEE580A841C73A8ECEC70410F03EC19421`。

## 快速上手

### 1. 安装 UniRTOS 工具链

- [开发准备](https://www.quectel.com.cn/unirtos/docs?docs_page=快速上手/开发准备/开发准备.html)
- [安装交叉编译工具链](https://www.quectel.com.cn/unirtos/docs?docs_page=快速上手/环境搭建/环境搭建.html)
- [安装 Python3](https://www.python.org/downloads/)
- [安装 git](https://git-scm.com)
- 安装 `unirtos-cli`：`pip install unirtos-cli`

以上工具安装完成后，确认以下命令可用：

```bash
python --version # Python3
git --version
unirtos --version # 1.0.5 及以上版本
unirtos-cli version # 1.0.11 及以上版本
```

### 2. 使用 unirtos-cli 拉取 demo

先查看可用 demo 与版本：

```bash
unirtos-cli ls-demos
```

创建本 demo 工程：

```bash
unirtos-cli new -r unirtos-lvgl-demos
```

如需指定版本：

```bash
unirtos-cli new -r unirtos-lvgl-demos -v 1.0.0
```

### 3. 进入工程并编译

```bash
cd unirtos-lvgl-demos-1.0.0
unirtos-cli env-setup
unirtos-cli build
```

`env-setup` 会按 `env_config.json` 拉取 `unirtos-lvgl` v8.3.11。SDK 仅在声明该依赖的工程中编译并链接 LVGL；未声明依赖的工程不会引入 LVGL。

构建产物位于：

```text
qos_build/release/<project-name>/at_command.hbinpkg
```

## 配置说明

- 保持 `menuconfig/.config` 中的 `CONFIG_QOSA_LCD_FUNC=y`。
- 显示 port 使用 `qosa_malloc()` 分配 RGB565 绘制缓冲区，确保 LSPI DMA 可访问。
- Demo 使用 5 ms LVGL tick，并在显示传输之间让出调度时间，避免影响系统任务。

## 常用命令

```bash
# 打开 SDK 菜单配置
unirtos-cli menuconfig

# 清理构建产物
unirtos-cli clean
```

## 技术社区

技术社区：https://forumschinese.quectel.com/c/66-category/66

## 贡献指南

欢迎参与共建，建议按以下方式提交：

- 提交前先执行基础验证：`env-setup`、`build`、`clean`。
- 使用清晰的提交说明，描述改动目的、影响范围和验证结果。
- 修改驱动、引脚映射、SDK 依赖或显示行为时，同步更新 README，并记录硬件验证结果。
- 通过 Issue 或 Pull Request 提交问题修复与功能改进。
