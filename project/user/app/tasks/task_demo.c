/**
 ******************************************************************************
 * @file    task_demo
 * @author  暄暄
 * @date    2025-1-13
 * @brief   演示任务
 *****************************************************************************/

#include "common.h"

void test(void);
void stop_test(void);

void task_demo(void)
{
    _INIT; /* 初次进入任务时，执行一次 */

    /* 开启测试任务 */
    sys_cycle_start(1000, test);

    /* 3秒后结束测试 */
    sys_timeout_start(3000, stop_test);

    _FREE; /* 任务结束，不再执行时，释放资源 */

    /* 离开任务自动关闭 */
    sys_cycle_stop(test);

    _RUN; /* 下方开始进入任务运行逻辑 */

    /* 等待测试结束 */
    if (sys_wait_bare())
        return;

    /* 跳转到空闲任务 */
    sys_goto(task_idle);
}

void test(void)
{
    sys_printf("test non blocking");
}

void stop_test(void)
{
    sys_wait_continue();
}

/************************** END OF FILE **************************/
