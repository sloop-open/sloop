/**
 ******************************************************************************
 * @file    task_demo
 * @author  sloop
 * @date    2025-1-13
 * @brief   演示任务
 *****************************************************************************/

#include "common.h"

void test(void);
void stop_test(void);

void task_demo(void)
{
    /* 初次进入任务时，执行一次 */
    _INIT;

    /* 开启测试任务 */
    sl_cycle_start(1000, test);

    /* 3秒后结束测试 */
    sl_timeout_start(3000, stop_test);

    /* 任务结束，不再执行时，释放资源 */
    _FREE;

    /* 离开任务自动关闭 */
    sl_cycle_stop(test);

    /* 下方开始进入任务运行逻辑 */
    _RUN;

    /* 等待测试结束 */
    if (sl_wait_bare())
        return;

    /* 跳转到空闲任务 */
    sl_goto(task_idle);
}

void test(void)
{
    sl_printf("test non blocking");
}

void stop_test(void)
{
    sl_wait_continue();
}

/************************** END OF FILE **************************/
