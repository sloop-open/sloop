/**
 ******************************************************************************
 * @file    task_flow
 * @author  sloop
 * @date    2026-3-22
 * @brief   工作流编程demo
 *****************************************************************************/

#include "common.h"

FLOW_STATE_DEFINE(flow1);
FLOW_EVENT_DEFINE(evt1);

FLOW_STATE_DEFINE(flow2);
FLOW_EVENT_DEFINE(evt2);

void flow1(void);
void flow2(void);

static char var;

void task_flow(void)
{
    /* 初次进入任务时，执行一次 */
    _INIT;

    FLOW_START(flow1);
    FLOW_START(flow2);

    /* 任务结束，不再执行时，释放资源 */
    _FREE;

    FLOW_STOP(flow1);
    FLOW_STOP(flow2);

    /* 下方开始进入任务运行逻辑 */
    _RUN;
}

void flow1(void)
{
    /* 工作流上下文，工作流需要的数据在此静态定义 */
    _FLOW_CONTEXT(flow1);

    /* 初次进入工作流，执行一次，初始化工作流上下文 */
    _FLOW_INIT;
    sl_focus("flow1 start");

    /* 工作流结束，不再执行时，释放资源 */
    _FLOW_FREE(flow1);
    sl_focus("flow1 stop");

    /* 下方开始进入工作流运行逻辑 */
    _FLOW_RUN;

    var++;

    sl_prt_withFunc("flow1 run, var = %d", var);

    FLOW_WAIT(1000);

    if (var == 6)
    {
        FLOW_SEND_EVENT(evt1);

        FLOW_WAIT_EVENT(evt2);

        sl_prt_withFunc("response received");
    }

    _FLOW_END;
}

void flow2(void)
{
    /* 工作流上下文，工作流需要的数据在此静态定义 */
    _FLOW_CONTEXT(flow2);

    /* 初次进入工作流，执行一次，初始化工作流上下文 */
    _FLOW_INIT;
    sl_focus("flow2 start");

    /* 工作流结束，不再执行时，释放资源 */
    _FLOW_FREE(flow2);
    sl_focus("flow2 stop");

    /* 下方开始进入工作流运行逻辑 */
    _FLOW_RUN;

    FLOW_UNTIL(var > 3);

    sl_prt_withFunc("condition met");

    FLOW_WAIT_EVENT(evt1);

    sl_prt_withFunc("event met");

    /* 回复flow1 */
    FLOW_SEND_EVENT(evt2);

    FLOW_WAIT(2000);

    sl_prt_withFunc("wait 2s");

    sl_goto(task_idle);

    _FLOW_END;
}

/************************** END OF FILE **************************/
