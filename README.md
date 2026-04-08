# Sloop 嵌入式系统框架

## 项目概述

Sloop是一个基于STM32系列微控制器的轻量级嵌入式系统框架，提供完整的硬件驱动、协作式任务调度内核和应用服务层，旨在简化嵌入式系统开发流程，提高代码复用性和可维护性。

### 主要特性

- **轻量级协作式内核**：基于协作式的任务管理系统
- **完整硬件驱动**：支持UART、CAN、GPIO等外设的高效驱动
- **分层架构设计**：清晰的内核-驱动-服务-应用分层结构
- **丰富的服务组件**：提供通信、存储、外设控制等应用服务
- **强大的调试支持**：集成RTT调试和系统监控功能
- **易于扩展**：模块化设计支持功能定制和扩展
- **Flow工作流机制**：基于sloop框架的协作式工作流编程机制，实现类似协程的断点续执行能力

### 应用场景

- 工业自动化控制设备
- 智能仪表和传感器节点
- 嵌入式网关和数据采集系统
- IoT终端设备
- 消费电子和智能硬件

## 系统架构

Sloop采用分层架构设计，各层职责明确，耦合度低，便于开发和维护。

### 架构层次

```
┌─────────────────────────────────────────────────┐
│                    应用层                        │
│  ┌───────────┐  ┌───────────┐  ┌─────────────┐  │
│  │   任务1   │  │   任务2   │  │    ...       │  │
│  └───────────┘  └───────────┘  └─────────────┘  │
├─────────────────────────────────────────────────┤
│                    服务层                        │
│  ┌───────────┐  ┌───────────┐  ┌─────────────┐  │
│  │  通信服务 │  │  参数管理 │  │    ...       │  │
│  └───────────┘  └───────────┘  └─────────────┘  │
├─────────────────────────────────────────────────┤
│                    驱动层                        │
│  ┌───────────┐  ┌───────────┐  ┌─────────────┐  │
│  │   UART    │  │   CAN     │  │    ...      │  │
│  └───────────┘  └───────────┘  └─────────────┘  │
├─────────────────────────────────────────────────┤
│                    内核层                        │
│  ┌───────────┐  ┌───────────┐  ┌─────────────┐  │
│  │ 任务管理  │  │ 时间管理  │  │    ...       │  │
│  └───────────┘  └───────────┘  └─────────────┘  │
└─────────────────────────────────────────────────┘
```

## 核心功能

### Sloop 内核

Sloop内核是系统的核心，提供以下功能：

#### 任务管理
- **并行任务**：同时运行的多个任务
- **互斥任务**：按需跳转的任务
- **单次任务**：只执行一次的任务
- **任务切换**：非抢占式任务切换机制

#### 时间管理
- **超时任务**：延迟指定时间后执行
- **周期任务**：按固定周期重复执行
- **多次任务**：执行指定次数的周期任务

#### 系统监控
- CPU负载计算和监控
- 任务运行时间统计
- 系统心跳和状态监控

### Flow 工作流机制

Flow是基于sloop框架的协作式工作流编程机制，通过宏定义构建可挂起的执行体，实现类似协程的断点续执行能力。它采用线性代码结构，使复杂业务逻辑更加清晰易读。

**主要特性**：
- 线性代码结构，逻辑清晰
- 断点续执行能力，类似协程
- 低资源消耗，适合嵌入式系统
- 易于集成到现有sloop项目中

## 快速开始

### 开发环境

- **硬件平台**：STM32系列微控制器
- **开发工具**：Keil MDK-ARM
- **编译工具链**：ARMCC或GCC
- **调试工具**：J-Link

### 项目结构

```
project/
├── Libraries/           # 第三方库
│   ├── CMSIS/           # Cortex-M内核支持
│   └── STM32F2xx_StdPeriph_Driver/ # STM32标准库
├── MDK-ARM/             # 编译配置
└── user/                # 用户代码
    ├── app/             # 应用层
    │   ├── config/      # 配置文件
    │   ├── tasks/       # 任务实现
    │   ├── main.c       # 程序入口
    │   └── main.h       # 应用头文件
    ├── service/         # 服务层
    │   ├── app_service_pack/ # 应用服务包
    │   └── mcu_service_pack/ # MCU服务包
    └── sloop/           # Sloop内核
        ├── RTT/         # 调试工具
        └── kernel/      # 内核核心
```

### 编译与烧录

1. 打开`project/MDK-ARM/project.uvprojx`工程文件
2. 选择目标设备
3. 配置编译选项和链接脚本
4. 编译工程生成固件文件
5. 使用J-Link或ST-Link烧录固件到目标设备

## 开发指南

### 任务开发流程

Sloop采用**动态注册机制**，任务不需要提前注册，直接调用API开始或停止任务。

1. **定义任务**：在头文件中声明任务函数
2. **实现任务**：在源文件中实现任务逻辑
3. **运行任务**：通过内核API直接调度执行任务


## API参考

### 内核API

#### 初始化函数
- `void sloop_init(void);` - 初始化Sloop内核
- `void sloop(void);` - 运行Sloop主循环

#### 任务管理
- `void sl_task_start(pfunc task);` - 启动并行任务
- `void sl_task_stop(pfunc task);` - 停止并行任务
- `void sl_task_once(pfunc task);` - 执行单次任务
- `void sl_goto(pfunc task);` - 跳转到互斥任务

#### 时间管理
- `void sl_timeout_start(int ms, pfunc task);` - 启动超时任务
- `void sl_timeout_stop(pfunc task);` - 停止超时任务
- `void sl_cycle_start(int ms, pfunc task);` - 启动周期任务
- `void sl_cycle_stop(pfunc task);` - 停止周期任务
- `void sl_multiple_start(int num, int ms, pfunc task);` - 启动多次任务
- `void sl_multiple_stop(pfunc task);` - 停止多次任务

#### 非阻塞等待
- `char sl_wait(int ms);` - 非阻塞等待指定时间
- `char sl_wait_bare(void);` - 非阻塞裸等待
- `char sl_is_waiting(void);` - 查询等待状态
- `void sl_wait_break(void);` - 中断等待
- `void sl_wait_continue(void);` - 忽略等待并继续执行

## 系统控制台

Sloop提供内置的系统控制台，可以在RTT终端执行预设命令，方便调试和系统监控。

### 内置命令
- `reboot` - 重启系统
- `version` - 查询版本
- `task` - 查询当前任务数据
- `cpu` - 查询CPU负载
- `uart` - 串口发送
- `can` - CAN发送
- `gpio` - GPIO控制

## 版本历史

- **V1.0.0** (2024-12-20)：初始版本，支持基本内核功能和硬件驱动
- **V1.1.0**：增加CAN总线支持和参数管理功能
- **V1.2.0**：优化内核性能，增加系统监控功能
- **V1.3.0**：扩展服务层组件，提供更多应用服务
- **V1.4.0**：新增Flow工作流机制，提供类似协程的编程体验

## 贡献指南

欢迎对Sloop项目进行贡献！

1. Fork项目仓库
2. 创建特性分支
3. 提交代码变更
4. 创建Pull Request

## 许可证

Sloop项目采用MIT许可证，详见LICENSE文件。

---

*最后更新：2026-04-08*


# Sloop Embedded System Framework

## Project Overview

Sloop is a lightweight embedded system framework based on STM32 series microcontrollers, providing complete hardware drivers, cooperative task scheduling kernel and application service layer, aiming to simplify the embedded system development process and improve code reusability and maintainability.

### Key Features

- **Lightweight Cooperative Kernel**: Cooperative-based task management system
- **Complete Hardware Drivers**: Efficient drivers for UART, CAN, GPIO and other peripherals
- **Layered Architecture Design**: Clear kernel-driver-service-application layered structure
- **Rich Service Components**: Communication, storage, peripheral control and other application services
- **Powerful Debugging Support**: Integrated RTT debugging and system monitoring functions
- **Easy to Extend**: Modular design supports function customization and extension
- **Flow Workflow Mechanism**: Cooperative workflow programming mechanism based on sloop framework, providing coroutine-like breakpoint continuation capabilities

### Application Scenarios

- Industrial automation control equipment
- Intelligent instruments and sensor nodes
- Embedded gateways and data acquisition systems
- IoT terminal devices
- Consumer electronics and smart hardware

## System Architecture

Sloop adopts a layered architecture design with clear responsibilities for each layer, low coupling, and easy development and maintenance.

### Architecture Layers

```
┌─────────────────────────────────────────────────┐
│                    Application Layer            │
│  ┌───────────┐  ┌───────────┐  ┌─────────────┐  │
│  │  Task 1   │  │  Task 2   │  │    ...       │  │
│  └───────────┘  └───────────┘  └─────────────┘  │
├─────────────────────────────────────────────────┤
│                    Service Layer                │
│  ┌───────────┐  ┌───────────┐  ┌─────────────┐  │
│  │Comm Service│  │Param Mngt │  │    ...       │  │
│  └───────────┘  └───────────┘  └─────────────┘  │
├─────────────────────────────────────────────────┤
│                    Driver Layer                 │
│  ┌───────────┐  ┌───────────┐  ┌─────────────┐  │
│  │   UART    │  │   CAN     │  │    ...      │  │
│  └───────────┘  └───────────┘  └─────────────┘  │
├─────────────────────────────────────────────────┤
│                    Kernel Layer                 │
│  ┌───────────┐  ┌───────────┐  ┌─────────────┐  │
│  │Task Mngt  │  │Time Mngt  │  │    ...       │  │
│  └───────────┘  └───────────┘  └─────────────┘  │
└─────────────────────────────────────────────────┘
```

## Core Features

### Sloop Kernel

The Sloop kernel is the core of the system, providing the following functions:

#### Task Management
- **Parallel Tasks**: Multiple tasks running simultaneously
- **Mutex Tasks**: Tasks that jump on demand
- **Once Tasks**: Tasks executed only once
- **Task Switching**: Non-preemptive task switching mechanism

#### Time Management
- **Timeout Tasks**: Execute after a specified delay
- **Cycle Tasks**: Execute repeatedly at fixed intervals
- **Multiple Tasks**: Execute cycle tasks a specified number of times

#### System Monitoring
- CPU load calculation and monitoring
- Task running time statistics
- System heartbeat and status monitoring

### Flow Workflow Mechanism

Flow is a cooperative workflow programming mechanism based on the sloop framework. It builds suspendable execution units through macro definitions, achieving coroutine-like breakpoint continuation capabilities. It adopts a linear code structure, making complex business logic clearer and easier to read.

**Key Features**:
- Linear code structure with clear logic
- Breakpoint continuation capability, similar to coroutines
- Low resource consumption, suitable for embedded systems
- Easy to integrate into existing sloop projects

## Quick Start

### Development Environment

- **Hardware Platform**: STM32 series microcontrollers
- **Development Tools**: Keil MDK-ARM
- **Compilation Toolchain**: ARMCC or GCC
- **Debugging Tools**: J-Link or ST-Link

### Project Structure

```
project/
├── Libraries/           # Third-party libraries
│   ├── CMSIS/           # Cortex-M kernel support
│   └── STM32F2xx_StdPeriph_Driver/ # STM32 standard library
├── MDK-ARM/             # Compilation configuration
└── user/                # User code
    ├── app/             # Application layer
    │   ├── config/      # Configuration files
    │   ├── tasks/       # Task implementations
    │   ├── main.c       # Program entry
    │   └── main.h       # Application header
    ├── service/         # Service layer
    │   ├── app_service_pack/ # Application service pack
    │   └── mcu_service_pack/ # MCU service pack
    └── sloop/           # Sloop kernel
        ├── RTT/         # Debugging tool
        └── kernel/      # Kernel core
```

### Compilation and Burning

1. Open the project file `project/MDK-ARM/project.uvprojx`
2. Select the target device
3. Configure compilation options and link scripts
4. Compile the project to generate firmware files
5. Use J-Link or ST-Link to burn the firmware to the target device

## Development Guide

### Task Development Process

Sloop adopts a **dynamic registration mechanism** where tasks do not need to be registered in advance; you can directly call APIs to start or stop tasks.

1. **Define Task**: Declare the task function in the header file
2. **Implement Task**: Implement the task logic in the source file
3. **Run Task**: Directly schedule and execute the task through kernel APIs

### Flow Workflow Usage

The Flow workflow mechanism builds suspendable execution units through macro definitions, providing coroutine-like breakpoint continuation capabilities.

```c
// Flow workflow example
void flow_example(void)
{
    FLOW_BEGIN();
    
    // Execute step 1
    sl_printf("Step 1\n");
    sl_wait(100);
    
    FLOW_YIELD(); // Suspend and continue execution next time
    
    // Execute step 2
    sl_printf("Step 2\n");
    sl_wait(100);
    
    FLOW_END();
}
```

## API Reference

### Kernel API

#### Initialization Functions
- `void sloop_init(void);` - Initialize Sloop kernel
- `void sloop(void);` - Run Sloop main loop

#### Task Management
- `void sl_task_start(pfunc task);` - Start parallel task
- `void sl_task_stop(pfunc task);` - Stop parallel task
- `void sl_task_once(pfunc task);` - Execute once task
- `void sl_goto(pfunc task);` - Jump to mutex task

#### Time Management
- `void sl_timeout_start(int ms, pfunc task);` - Start timeout task
- `void sl_timeout_stop(pfunc task);` - Stop timeout task
- `void sl_cycle_start(int ms, pfunc task);` - Start cycle task
- `void sl_cycle_stop(pfunc task);` - Stop cycle task
- `void sl_multiple_start(int num, int ms, pfunc task);` - Start multiple task
- `void sl_multiple_stop(pfunc task);` - Stop multiple task

#### Non-blocking Waiting
- `char sl_wait(int ms);` - Non-blocking wait for specified time
- `char sl_wait_bare(void);` - Non-blocking bare wait
- `char sl_is_waiting(void);` - Query wait status
- `void sl_wait_break(void);` - Interrupt wait
- `void sl_wait_continue(void);` - Ignore wait and continue execution

## System Console

Sloop provides a built-in system console that allows you to execute preset commands in the RTT terminal for easy debugging and system monitoring.

### Built-in Commands
- `reboot` - Reboot system
- `version` - Query version
- `task` - Query current task data
- `cpu` - Query CPU load
- `uart` - UART send
- `can` - CAN send
- `gpio` - GPIO control

## Version History

- **V1.0.0** (2024-12-20): Initial version, supporting basic kernel functions and hardware drivers
- **V1.1.0**: Added CAN bus support and parameter management functions
- **V1.2.0**: Optimized kernel performance, added system monitoring functions
- **V1.3.0**: Expanded service layer components, providing more application services
- **V1.4.0**: Added Flow workflow mechanism, providing coroutine-like programming experience

## Contribution Guide

Welcome to contribute to the Sloop project!

1. Fork the project repository
2. Create a feature branch
3. Commit code changes
4. Create a Pull Request

## License

The Sloop project is licensed under the MIT License, see the LICENSE file for details.

---

*Last updated: 2026-04-08*