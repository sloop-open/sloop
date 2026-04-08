# Sloop 嵌入式系统框架

## 项目概述

Sloop是一个基于STM32F2系列微控制器的轻量级嵌入式系统框架，提供完整的硬件驱动、协作式任务调度内核和应用服务层，旨在简化嵌入式系统开发流程，提高代码复用性和可维护性。

### 主要特性

- **轻量级协作式内核**：基于协作式的任务管理系统
- **完整硬件驱动**：支持UART、CAN、GPIO等外设的高效驱动
- **分层架构设计**：清晰的内核-驱动-服务-应用分层结构
- **丰富的服务组件**：提供通信、存储、外设控制等应用服务
- **强大的调试支持**：集成RTT调试和系统监控功能
- **易于扩展**：模块化设计支持功能定制和扩展

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

### 各层功能说明

1. **内核层**：提供实时操作系统核心功能，包括任务管理、时间管理、资源分配和系统监控
2. **驱动层**：封装硬件资源，提供统一的设备驱动接口
3. **服务层**：在驱动层基础上构建应用服务，提供更高层次的功能抽象
4. **应用层**：实现业务逻辑，通过调用服务层API完成具体功能

## 快速开始

### 开发环境

- **硬件平台**：STM32F2系列微控制器
- **开发工具**：Keil MDK-ARM
- **编译工具链**：ARMCC或GCC
- **调试工具**：J-Link或ST-Link

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
2. 选择目标设备(STM32F207VETx)
3. 配置编译选项和链接脚本
4. 编译工程生成固件文件
5. 使用J-Link或ST-Link烧录固件到目标设备

### 运行第一个程序

```c
// main.c
#include "common.h"

// 定义任务
weak_define(task_baseInit);
weak_define(task_idle);

int main(void)
{
    sloop_init();           // 初始化Sloop内核
    sl_goto(task_baseInit); // 跳转到基础初始化任务
    
    while (1)
    {
        sloop();            // 运行Sloop主循环
    }
}
```

## 核心功能

### Sloop 内核

Sloop内核是系统的核心，提供以下功能：

#### 任务管理

- **并行任务**：同时运行的多个任务
- **互斥任务**：按需跳转的任务
- **单次任务**：只执行一次的任务
- **任务切换**：非抢占式任务切换机制

```c
// 开始并行任务
sl_task_start(my_task);

// 跳转互斥任务
sl_goto(my_mutex_task);

// 开始单次任务
sl_task_once(my_once_task);
```

#### 时间管理

- **超时任务**：延迟指定时间后执行
- **周期任务**：按固定周期重复执行
- **多次任务**：执行指定次数的周期任务

```c
// 创建超时任务(100ms后执行)
sl_timeout_start(100, my_timeout_task);

// 创建周期任务(每500ms执行一次)
sl_cycle_start(500, my_cycle_task);

// 创建多次任务(执行10次，每200ms一次)
sl_multiple_start(10, 200, my_multiple_task);
```

#### 系统监控

- CPU负载计算和监控
- 任务运行时间统计
- 系统心跳和状态监控

### 硬件驱动

Sloop提供完整的硬件驱动支持，包括：

#### UART驱动

- 支持6个UART接口
- 基于DMA的高效收发
- 基于队列的收发
- 接收超时检测
- 调试输出支持


#### CAN驱动

- 支持2个CAN接口
- 基于队列的收发


### 应用服务

应用服务层提供更高层次的功能封装：

#### 通信服务

- 串口消息解析和封装
- CAN总线协议处理
- 数据帧格式转换

#### 外设服务

- EEPROM读写操作
- 蜂鸣器控制
- 运行灯管理

#### 参数管理

- 系统参数存储
- 参数读写接口
- 参数校验和保护

## 开发指南

### 任务开发流程

Sloop采用**动态注册机制**，任务不需要提前注册，直接调用API开始或停止任务。

1. **定义任务**：在头文件中声明任务函数
2. **实现任务**：在源文件中实现任务逻辑
3. **运行任务**：通过内核API直接调度执行任务

```c
// 1. 声明任务(在main.h中)
void task_myTask(void);

// 2. 实现任务(在task_myTask.c中)
#include "common.h"

void task_myTask(void)
{
    _INIT; // 任务初始化代码
    
    _FREE; // 任务释放代码

    _RUN; // 任务运行代码标记

    // 执行任务操作
    sl_wait(100); // 等待100ms

    sl_goto(task_idle);
}

### 任务类型与API

#### 并行任务
并行任务是在主线程轮询执行的任务，所有并行任务共享同一时间片。

```c
// 开始并行任务
sl_task_start(my_parallel_task);

// 停止并行任务
sl_task_stop(my_parallel_task);
```

**注意**：所有需要在主线程轮询的任务必须通过`sl_task_start`调用，这样才能保证`sl_wait`非阻塞等待功能正常。

#### 互斥任务
互斥任务是独占CPU执行的任务，一次只能执行一个互斥任务。

```c
// 跳转到互斥任务
sl_goto(my_mutex_task);
```

**互斥任务的三个阶段**：
- **初始化阶段**：`_INIT` 和 `_FREE` 之间的代码，仅在任务首次进入时执行一次
- **运行阶段**：`_RUN;` 之后的代码，任务的主要逻辑
- **运行终止**：`_FREE;` 和 `_RUN;` 之间的代码，在任务跳转前执行

#### 超时任务
超时任务在指定时间后执行一次。

```c
// 启动超时任务(100ms后执行)
sl_timeout_start(100, my_timeout_task);

// 停止超时任务
sl_timeout_stop(my_timeout_task);
```

#### 周期任务
周期任务按固定周期重复执行。

```c
// 启动周期任务(每500ms执行一次)
sl_cycle_start(500, my_cycle_task);

// 停止周期任务
sl_cycle_stop(my_cycle_task);
```

#### 多次任务
多次任务执行指定次数的周期任务。

```c
// 启动多次任务(执行10次，每200ms一次)
sl_multiple_start(10, 200, my_multiple_task);

// 停止多次任务
sl_multiple_stop(my_multiple_task);
```

#### 单次任务
单次任务只执行一次。

```c
// 执行单次任务
sl_task_once(my_once_task);
```

### 任务管理注意事项

1. **动态注册机制优势**：
   - 可以在运行时方便地启用和停止任务
   - 停止后没有副作用
   - 简化文件之间的调用耦合
   - 减少标志量使用，提高代码可读性

2. **超时与周期任务**：
   - 以回调函数名称作为ID
   - 同一时刻，同一个回调函数不支持开启多种超时或周期任务
   - 最新的会覆盖旧的
   - 同一个回调即开启超时又开启周期是允许的

3. **重置超时任务**：
   - 只需重新调用`sl_timeout_start`
   - 超时任务到达后会自动释放，没有副作用

4. **非阻塞等待**：
   - `sl_wait`和`sl_wait_bare`只能在互斥任务中调用
   - 不允许嵌套调用
   - 调用会阻塞当前任务，但不影响其他并行任务执行
   - 可以用`sl_wait_break`中断等待，`sl_wait_continue`忽略等待

```c
// sl_wait推荐使用方法
if(sl_wait(10)) // 等待10ms
    return; // 等待被中断，返回
// 等待完成后的业务逻辑
```


### 调试技巧

```c
// 打印调试信息
sl_printf("Debug message: %d\n", value);

// 打印变量
sl_prt_var(my_variable);

// 打印函数名和信息
sl_prt_withFunc("Function executed");
```

## 配置说明

Sloop通过配置文件提供灵活的功能定制：

### 内核配置(sl_config.h)

```c
// 任务数量限制
#define TIMEOUT_LIMIT 16      // 超时任务上限
#define CYCLE_LIMIT 16        // 周期任务上限
#define PARALLEL_TASK_LIMIT 32 // 并行任务上限

// 功能开关
#define SL_WDG_ENABLE 0      // 启用看门狗
#define SL_RTT_ENABLE 1      // 启用RTT打印
#define BHV_LOG_ENABLE 1      // 启用行为日志
#define SL_CMD_ENABLE 1      // 启用控制台
```

## 系统控制台

Sloop提供内置的系统控制台，可以在RTT终端执行预设命令，方便调试和系统监控。

### RTT设置
```
input->"end of line"->none
input->"echo input"->all enable
```

### 内置命令汇总

| 功能                  | 命令       | 简写 | 数字调用 | 参数                               |
|-----------------------|-----------|------|---------|-----------------------------------|
| 重启                  | reboot     | r/R  | 0      | 无                                 |
| 查询版本              | version    | v/V  | 1      | 无                                 |
| 查询当前任务数据       | task       | t/T  | 2      | 命令示例："task" 打印一次，"task on" 10Hz打印，"task off" 关闭10Hz打印 |
| 查询 cpu 负载         | cpu        | c/C  | 3      | 命令示例："cpu" 打印一次，"cpu on" 10Hz打印，"cpu off" 关闭10Hz打印 |
| 串口发送              | uart       | 无   | 无     | 命令示例：ASCLL 模式："uart6 hello"，HEX 模式："uart6 -hex 68 65 6C 6C 6F" |
| CAN发送               | can        | 无   | 无     | 命令示例：ASCLL 模式："can1 -id 1 hello"，HEX 模式："can1 -id 1 -hex 68 65 6C 6C 6F" |
| GPIO输出和输入回显开关 | gpio       | 无   | 无     | 命令示例："gpio pin_beep H", "gpio input on" or "gpio input off" |

**使用说明**：
- 输入 `m`、`M` 或 `menu` 显示命令菜单
- 输入命令 `-h` 可查看命令使用举例，比如 `uart -h`

## API参考

### 内核API

#### 初始化函数

```c
void sloop_init(void);      // 初始化Sloop内核
void sloop(void);           // 运行Sloop主循环
```

#### 任务管理

```c
void sl_task_start(pfunc task);  // 启动并行任务
void sl_task_stop(pfunc task);   // 停止并行任务
void sl_task_once(pfunc task);   // 执行单次任务
void sl_goto(pfunc task);        // 跳转到互斥任务
```

#### 时间管理

```c
void sl_timeout_start(int ms, pfunc task);  // 启动超时任务
void sl_timeout_stop(pfunc task);           // 停止超时任务
void sl_cycle_start(int ms, pfunc task);    // 启动周期任务
void sl_cycle_stop(pfunc task);             // 停止周期任务
void sl_multiple_start(int num, int ms, pfunc task); // 启动多次任务
void sl_multiple_stop(pfunc task);          // 停止多次任务
```

#### 非阻塞等待

```c
char sl_wait(int ms);            // 非阻塞等待指定时间
char sl_wait_bare(void);         // 非阻塞裸等待
char sl_is_waiting(void);        // 查询等待状态
void sl_wait_break(void);        // 中断等待
void sl_wait_continue(void);     // 忽略等待并继续执行
```


## 常见问题

### Q: 如何添加新的任务？
A: 1. 在头文件中声明任务函数 2. 在源文件中实现任务逻辑 3. 通过sl_goto或sl_task_start启用任务

### Q: 如何配置串口波特率？
A: 在uart_config.h文件中修改BAUD_RATE宏定义

### Q: 如何启用RTT调试？
A: 在sl_config.h中设置SL_RTT_ENABLE为1

### Q: 如何处理超时任务？
A: 使用sl_timeout_start函数创建超时任务，指定延迟时间和回调函数

## 版本历史

- **V1.0.0** (2024-12-20)：初始版本，支持基本内核功能和硬件驱动
- **V1.1.0**：增加CAN总线支持和参数管理功能
- **V1.2.0**：优化内核性能，增加系统监控功能
- **V1.3.0**：扩展服务层组件，提供更多应用服务

### 更新日志摘要

- **2025-3-11**：控制台新增GPIO输出命令和输入回显开关
- **2025-3-3**：全新GPIO配置模板，移除部分API
- **2025-2-27**：UART1~6验证通过，控制台支持UART发送
- **2025-2-19**：HardFault增加栈解析功能
- **2025-2-8**：新增任务周期统计功能
- **2025-2-7**：新增us级延时API，中断改为16级响应优先级
- **2025-1-20**：新增CPU负载查询命令，软定时任务移至主线程
- **2024-12-31**：新增可配置看门狗和行为日志功能
- **2024-12-25**：精简互斥任务，新增错误日志功能

详细更新日志请查看项目根目录下的`readme.txt`文件。

## 贡献指南

欢迎对Sloop项目进行贡献！

1. Fork项目仓库
2. 创建特性分支
3. 提交代码变更
4. 创建Pull Request

## 许可证

Sloop项目采用MIT许可证，详见LICENSE文件。

---

*最后更新：2025-01-10*


# Sloop Embedded System Framework

## Project Overview

Sloop is a lightweight embedded system framework based on STM32F2 series microcontrollers, providing complete hardware drivers, cooperative task scheduling kernel and application service layer, aiming to simplify the embedded system development process and improve code reusability and maintainability.

### Key Features

- **Lightweight Cooperative Kernel**: Cooperative-based task management system
- **Complete Hardware Drivers**: Efficient drivers for UART, CAN, GPIO and other peripherals
- **Layered Architecture Design**: Clear kernel-driver-service-application layered structure
- **Rich Service Components**: Communication, storage, peripheral control and other application services
- **Powerful Debugging Support**: Integrated RTT debugging and system monitoring functions
- **Easy to Extend**: Modular design supports function customization and extension

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

### Layer Function Description

1. **Kernel Layer**: Provides real-time operating system core functions, including task management, time management, resource allocation and system monitoring
2. **Driver Layer**: Encapsulates hardware resources and provides unified device driver interfaces
3. **Service Layer**: Builds application services based on the driver layer, providing higher-level functional abstraction
4. **Application Layer**: Implements business logic and completes specific functions by calling service layer APIs

## Quick Start

### Development Environment

- **Hardware Platform**: STM32F2 series microcontrollers
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
2. Select the target device (STM32F207VETx)
3. Configure compilation options and link scripts
4. Compile the project to generate firmware files
5. Use J-Link or ST-Link to burn the firmware to the target device

### Run Your First Program

```c
// main.c
#include "common.h"

// Define tasks
weak_define(task_baseInit);
weak_define(task_idle);

int main(void)
{
    sloop_init();           // Initialize Sloop kernel
    sl_goto(task_baseInit); // Jump to base initialization task
    
    while (1)
    {
        sloop();            // Run Sloop main loop
    }
}
```

## Core Features

### Sloop Kernel

The Sloop kernel is the core of the system, providing the following functions:

#### Task Management

- **Parallel Tasks**: Multiple tasks running simultaneously
- **Mutex Tasks**: Tasks that jump on demand
- **Once Tasks**: Tasks executed only once
- **Task Switching**: Non-preemptive task switching mechanism

```c
// Start parallel task
sl_task_start(my_task);

// Jump to mutex task
sl_goto(my_mutex_task);

// Start once task
sl_task_once(my_once_task);
```

#### Time Management

- **Timeout Tasks**: Execute after a specified delay
- **Cycle Tasks**: Execute repeatedly at fixed intervals
- **Multiple Tasks**: Execute cycle tasks a specified number of times

```c
// Create timeout task (execute after 100ms)
sl_timeout_start(100, my_timeout_task);

// Create cycle task (execute every 500ms)
sl_cycle_start(500, my_cycle_task);

// Create multiple task (execute 10 times, every 200ms)
sl_multiple_start(10, 200, my_multiple_task);
```

#### System Monitoring

- CPU load calculation and monitoring
- Task running time statistics
- System heartbeat and status monitoring

### Hardware Drivers

Sloop provides complete hardware driver support, including:

#### UART Drivers

- Support for 6 UART interfaces
- Efficient DMA-based transmission and reception
- Queue-based transmission and reception
- Receive timeout detection
- Debug output support

#### CAN Drivers

- Support for 2 CAN interfaces
- Queue-based transmission and reception

### Application Services

The application service layer provides higher-level function encapsulation:

#### Communication Services

- UART message parsing and encapsulation
- CAN bus protocol processing
- Data frame format conversion

#### Peripheral Services

- EEPROM read/write operations
- Buzzer control
- Running light management

#### Parameter Management

- System parameter storage
- Parameter read/write interfaces
- Parameter verification and protection

## Development Guide

### Task Development Process

Sloop adopts a **dynamic registration mechanism** where tasks do not need to be registered in advance; you can directly call APIs to start or stop tasks.

1. **Define Task**: Declare the task function in the header file
2. **Implement Task**: Implement the task logic in the source file
3. **Run Task**: Directly schedule and execute the task through kernel APIs

```c
// 1. Declare task (in main.h)
void task_myTask(void);

// 2. Implement task (in task_myTask.c)
#include "common.h"

void task_myTask(void)
{
    _INIT; // Task initialization code
    
    _FREE; // Task release code

    _RUN; // Task running code marker

    // Execute task operations
    sl_wait(100); // Wait for 100ms

    sl_goto(task_idle);
}

```

### Task Types and APIs

#### Parallel Tasks
Parallel tasks are tasks executed in a polling manner in the main thread, all sharing the same time slice.

```c
// Start parallel task
sl_task_start(my_parallel_task);

// Stop parallel task
sl_task_stop(my_parallel_task);
```

**Note**: All tasks that need to be polled in the main thread must be called through `sl_task_start` to ensure the normal operation of `sl_wait` non-blocking waiting function.

#### Mutex Tasks
Mutex tasks are tasks that exclusively occupy the CPU for execution, and only one mutex task can be executed at a time.

```c
// Jump to mutex task
sl_goto(my_mutex_task);
```

**Three stages of mutex tasks**:
- **Initialization stage**: Code between `_INIT` and `_FREE`, executed only once when the task is first entered
- **Running stage**: Code after `_RUN;`, the main logic of the task
- **Termination stage**: Code between `_FREE;` and `_RUN;`, executed before task switching

#### Timeout Tasks
Timeout tasks execute once after a specified time delay.

```c
// Start timeout task (execute after 100ms)
sl_timeout_start(100, my_timeout_task);

// Stop timeout task
sl_timeout_stop(my_timeout_task);
```

#### Cycle Tasks
Cycle tasks execute repeatedly at fixed intervals.

```c
// Start cycle task (execute every 500ms)
sl_cycle_start(500, my_cycle_task);

// Stop cycle task
sl_cycle_stop(my_cycle_task);
```

#### Multiple Tasks
Multiple tasks execute cycle tasks a specified number of times.

```c
// Start multiple task (execute 10 times, every 200ms)
sl_multiple_start(10, 200, my_multiple_task);

// Stop multiple task
sl_multiple_stop(my_multiple_task);
```

#### Once Tasks
Once tasks execute only once.

```c
// Execute once task
sl_task_once(my_once_task);
```

### Task Management Notes

1. **Advantages of dynamic registration mechanism**:
   - Tasks can be easily enabled and stopped at runtime
   - No side effects after stopping
   - Simplifies call coupling between files
   - Reduces the use of flags and improves code readability

2. **Timeout and cycle tasks**:
   - Use callback function names as IDs
   - At the same time, the same callback function does not support opening multiple timeout or cycle tasks
   - The latest one will overwrite the old one
   - It is allowed to open both timeout and cycle tasks with the same callback

3. **Reset timeout tasks**:
   - Simply call `sl_timeout_start` again
   - Timeout tasks will be automatically released after timeout, no side effects

4. **Non-blocking waiting**:
   - `sl_wait` and `sl_wait_bare` can only be called in mutex tasks
   - Nested calls are not allowed
   - Calling blocks the current task, but does not affect other parallel tasks
   - Can use `sl_wait_break` to interrupt waiting, `sl_wait_continue` to ignore waiting

```c
// Recommended usage of sl_wait
if(sl_wait(10)) // Wait for 10ms
    return; // Waiting interrupted, return
// Business logic after waiting completes
```

### Debugging Techniques

```c
// Print debug message
sl_printf("Debug message: %d\n", value);

// Print variable
sl_prt_var(my_variable);

// Print function name and information
sl_prt_withFunc("Function executed");
```

## Configuration

Sloop provides flexible function customization through configuration files:

### Kernel Configuration (sl_config.h)

```c
// Task quantity limits
#define TIMEOUT_LIMIT 16      // Timeout task limit
#define CYCLE_LIMIT 16        // Cycle task limit
#define PARALLEL_TASK_LIMIT 32 // Parallel task limit

// Function switches
#define SL_WDG_ENABLE 0      // Enable watchdog
#define SL_RTT_ENABLE 1      // Enable RTT printing
#define BHV_LOG_ENABLE 1      // Enable behavior logging
#define SL_CMD_ENABLE 1      // Enable console
```

## System Console

Sloop provides a built-in system console that allows you to execute preset commands in the RTT terminal for easy debugging and system monitoring.

### RTT Settings

```
input->"end of line"->none
input->"echo input"->all enable
```

### Built-in Command Summary

| Function | Command | Shortcut | Number Call | Parameters |
|----------|---------|----------|-------------|------------|
| Reboot | reboot | r/R | 0 | None |
| Query Version | version | v/V | 1 | None |
| Query Current Task Data | task | t/T | 2 | "task" - print once, "task on" - 10Hz continuous printing, "task off" - close continuous printing |
| Query CPU Load | cpu | c/C | 3 | "cpu" - print once, "cpu on" - 10Hz continuous printing, "cpu off" - close continuous printing |
| UART Send | uart | None | None | ASCII mode: "uart6 hello", HEX mode: "uart6 -hex 68 65 6C 6C 6F" |
| CAN Send | can | None | None | ASCII mode: "can1 -id 1 hello", HEX mode: "can1 -id 1 -hex 68 65 6C 6C 6F" |
| GPIO Control | gpio | None | None | Output: "gpio pin_beep H", Input echo: "gpio input on/off" |

**Usage Instructions**:
- Enter `m`, `M` or `menu` to display the command menu
- Enter command `-h` to see command usage examples, such as `uart -h`

## API Reference

### Kernel API

#### Initialization Functions

```c
void sloop_init(void);      // Initialize Sloop kernel
void sloop(void);           // Run Sloop main loop
```

#### Task Management

```c
void sl_task_start(pfunc task);  // Start parallel task
void sl_task_stop(pfunc task);   // Stop parallel task
void sl_task_once(pfunc task);   // Execute once task
void sl_goto(pfunc task);        // Jump to mutex task
```

#### Time Management

```c
void sl_timeout_start(int ms, pfunc task);  // Start timeout task
void sl_timeout_stop(pfunc task);           // Stop timeout task
void sl_cycle_start(int ms, pfunc task);    // Start cycle task
void sl_cycle_stop(pfunc task);             // Stop cycle task
void sl_multiple_start(int num, int ms, pfunc task); // Start multiple task
void sl_multiple_stop(pfunc task);          // Stop multiple task
```

#### Non-blocking Waiting

```c
char sl_wait(int ms);            // Non-blocking wait for specified time
char sl_wait_bare(void);         // Non-blocking bare wait
char sl_is_waiting(void);        // Query wait status
void sl_wait_break(void);        // Interrupt wait
void sl_wait_continue(void);     // Ignore wait and continue execution
```

## FAQ

### Q: How to add a new task?
A: 1. Declare the task function in the header file 2. Implement the task logic in the source file 3. Enable the task through sl_goto or sl_task_start

### Q: How to configure UART baud rate?
A: Modify the BAUD_RATE macro definition in the uart_config.h file

### Q: How to enable RTT debugging?
A: Set SL_RTT_ENABLE to 1 in sl_config.h

### Q: How to handle timeout tasks?
A: Use the sl_timeout_start function to create a timeout task, specifying the delay time and callback function

## Version History

- **V1.0.0** (2024-12-20): Initial version, supporting basic kernel functions and hardware drivers
- **V1.1.0**: Added CAN bus support and parameter management functions
- **V1.2.0**: Optimized kernel performance, added system monitoring functions
- **V1.3.0**: Expanded service layer components, providing more application services

### Update Log Summary

- **2025-3-11**: Console added GPIO output command and input echo switch
- **2025-3-3**: New GPIO configuration template, removed some APIs
- **2025-2-27**: UART1~6 verified, console supports UART sending
- **2025-2-19**: HardFault added stack parsing function
- **2025-2-8**: Added task cycle statistics function
- **2025-2-7**: Added us-level delay API, interrupt changed to 16-level response priority
- **2025-1-20**: Added CPU load query command, soft timer tasks moved to main thread
- **2024-12-31**: Added configurable watchdog and behavior logging functions
- **2024-12-25**: Streamlined mutex tasks, added error logging function

For detailed update logs, please refer to the `readme.txt` file in the project root directory.

## Contribution Guide

Welcome to contribute to the Sloop project!

1. Fork the project repository
2. Create a feature branch
3. Commit code changes
4. Create a Pull Request

## License

The Sloop project is licensed under the MIT License, see the LICENSE file for details.

---

*Last updated: 2025-01-10*