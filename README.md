# sloop_cmake

> 复杂的逻辑写了一大堆 flag，越写越乱。试一下用 sloop 来管理裸机程序，轻量又实用。

一个基于 STM32G030 的轻量级任务调度系统，使用 CMake 构建。

## 项目简介

sloop 是一个专为嵌入式系统设计的轻量级任务调度框架，提供以下核心功能：

- **超时任务** - 延迟指定时间后执行一次
- **周期任务** - 以固定周期重复执行
- **多次任务** - 执行指定次数后停止
- **并发任务** - 多个任务并发运行
- **单次任务** - 用于中断中复杂逻辑下放执行
- **宏协程** - 非阻塞等待的协程编程模式
- **CPU 负载监控** - 实时监控系统负载
- **RTT 调试**：集成 SEGGER RTT 实时输出

## 硬件

- MCU: STM32G030F6P6 (ARM Cortex-M0+)
- 调试器: J-Link

## 目录结构

```
sloop/
├── project/                    # 主项目目录
│   ├── Core/                   # STM32 HAL 核心文件
│   │   ├── Inc/               # 头文件
│   │   └── Src/               # 源文件
│   ├── Drivers/                # 驱动库
│   │   ├── CMSIS/             # CMSIS 核心库
│   │   └── STM32G0xx_HAL_Driver/  # STM32 HAL 驱动
│   ├── cmake/                  # CMake 配置
│   │   ├── stm32cubemx/       # STM32CubeMX CMake 模块
│   │   ├── gcc-arm-none-eabi.cmake  # GCC 工具链配置
│   │   └── starm-clang.cmake  # Clang 工具链配置
│   ├── user/                   # 用户代码
│   │   ├── app/               # 应用层代码
│   │   │   ├── config/        # 配置文件
│   │   │   ├── _main.c        # 用户主程序入口
│   │   │   └── common.h       # 公共头文件
│   │   └── sloop/             # sloop 调度系统
│   │       ├── RTT/           # SEGGER RTT 调试
│   │       ├── kernel/        # 调度核心
│   │       ├── sloop.h        # sloop API 头文件
│   │       └── sl_common.h    # sloop 公共定义
│   ├── CMakeLists.txt         # 主 CMake 配置
│   ├── CMakePresets.json      # CMake 预设配置
│   └── STM32G030XX_FLASH.ld   # 链接脚本
├── images/                    # 项目图片
├── LICENSE                    # 许可证文件
└── README.md                  # 项目说明
```

## API 速查

### 系统控制

| 函数 | 说明 |
|------|------|
| `sloop_init()` | 初始化系统 |
| `sloop_run()` | 运行调度循环 |
| `sl_tick_irq()` | Tick 中断处理 |
| `sl_get_tick()` | 获取时间戳 |
| `sl_delay(ms)` | 阻塞延时 |

### 任务调度

| 函数 | 说明 |
|------|------|
| `sl_timeout_start(ms, task)` | 超时任务 |
| `sl_cycle_start(ms, task)` | 周期任务 |
| `sl_multiple_start(n, ms, task)` | 多次任务 |
| `sl_task_start(task)` | 并发任务 |
| `sl_task_once(task)` | 单次任务 |

### 宏协程

| 宏 | 说明 |
|----|------|
| `SL_BEGIN` / `SL_END` | 协程代码块 |
| `sl_wait_ms(ms)` | 时间等待 |
| `sl_wait_until(cond)` | 条件等待 |

## 快速开始

```bash
# 构建
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../project/cmake/gcc-arm-none-eabi.cmake
make -j4
```

## 配置

`project/user/app/config/sl_config.h`:

```c
#define SL_TIMEOUT_LIMIT     16   // 超时任务上限
#define SL_CYCLE_LIMIT       16   // 周期任务上限
#define SL_MULTIPLE_LIMIT    16   // 多次任务上限
#define SL_CONCURRENT_LIMIT  32   // 并发任务上限
#define SL_ONCE_LIMIT        16   // 单次任务上限
#define SL_RTT_ENABLE        1    // 启用 RTT
```

## 示例

### 周期任务

```c
void led_blink(void) {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

void _main(void) {
    sloop_init();
    sl_cycle_start(500, led_blink);  // 每 500ms 闪烁
    while (1) 
    {
        sloop_run();
    }
}
```

### 协程

```c
void sensor_task(void) {
    SL_BEGIN;
    for (;;) {
        sl_wait_until(sensor_ready);
        read_sensor();
        sl_wait_ms(1000);
    }
    SL_END;
}

void _main(void) {
    sloop_init();
    sl_task_start(sensor_task);  // 启动协程任务
    while (1) 
    {
        sloop_run();
    }
}
```

### sloop_init 核心示例

**心跳任务（周期任务 API）**：

```c
void system_heartbeat(void) {
    static int count;
    SEGGER_RTT_SetTerminal(1);
    sl_prt_var(count);
    SEGGER_RTT_SetTerminal(0);
    count++;
}

// sl_cycle_start(1000, system_heartbeat);
```

**CPU 负载监控（协程 API）**：

```c

/* 计算 cpu 负载 */
void calcul_cpu_load(void)
{
    loop++;

    load_calcu();

    judge_warning();
}

/* CPU 负载基数，即 CPU = 100% 时，一个 loop 的时间 单位0.1us，约定当loop 达到100us时，CPU 占用为100% */

/* 计算周期 ms */
#define LOOP_CYCLE (100)
/* 负载基数 us */
#define LOOP_BASE (100)

void load_calcu(void)
{
    SL_BEGIN;

    for (;;)
    {
        sl_wait_ms(LOOP_CYCLE);

        loop_us = LOOP_CYCLE * 1000 / loop;

        load = loop_us * 100 / LOOP_BASE;

        loop = 0;
    }

    SL_END;
}

/* 负载警告 */
void load_warning(void)
{
    SL_BEGIN;

    for (;;)
    {
        sl_error("cpu load over 80%%, reach %2d, average loop time: %d us", load, loop_us);

        sl_wait_ms(1000);
    }

    SL_END;
}

void judge_warning(void)
{
    SL_BEGIN;

    for (;;)
    {
        sl_wait_until(load > 800);

        sl_task_start(load_warning);

        sl_wait_until(load < 600);

        sl_task_stop(load_warning);
    }

    SL_END;
}


// sl_task_start(calcul_cpu_load);
```

## RTT 调试

1. 连接 J-Link
2. 打开 SEGGER RTT Viewer
3. 选择目标设备

## 许可证

MIT License