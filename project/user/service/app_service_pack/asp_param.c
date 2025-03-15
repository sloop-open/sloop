/**
 ******************************************************************************
 * @file    asp_param
 * @author  暄暄
 * @date    2022-2-20
 * @brief   参数的保存和还原,添加参数，请参考注释执行6个步骤。
 * 默认以 FLASH 为介质，可选 EEP
 * ==此文件用户可以按步骤添加参数==
 *****************************************************************************/

#include "asp_param.h"

/* 重置EEP */
#define EEP_RESET 0

/* 配置参数存储介质 0:EEP 1:FLASH */
#define USE_FLASH 1

#if USE_FLASH

#include "msp_flash.h"
#define PARAM_ADDR FLASH_PARAM_BASE
#define msp_param_read(buf, len) msp_flash_read(PARAM_ADDR, buf, len)
#define msp_param_write(buf, len) msp_flash_write(PARAM_ADDR, buf, len)

#else

#include "asp_eep24c64.h"
#define PARAM_ADDR eep24c64_BASE
#define msp_param_read(buf, len) asp_eep24c64_read_bytes(PARAM_ADDR, buf, len)
#define msp_param_write(buf, len) asp_eep24c64_write_bytes(PARAM_ADDR, buf, len)

#endif

/* ============================================================== */
/* ==第1步：申明外部参数== */

/* 外部参数 */
extern float convert;

/* 需保存的参数 */
typedef struct
{
    /* ============================================================== */
    /* ==第2步：注册写入参数== */

    /* C臂CAN编码器角度系数 */
    float convert;

    /* 参数已初始化标志 */
    char InitFlag[4];

} param_typ;

static param_typ param;

/* 需保存的参数默认值 */
static const param_typ param_Default = {

    /* ============================================================== */
    /* ==第3步：给参数设置默认值== */

    .convert = 1,
    .InitFlag = "SET"};

/* 校验参数 */
void check_param(void);

/* 参数还原 */
void paramRestore(void)
{
    msp_param_read((uint8_t *)&param, sizeof(param));

    /* ============================================================== */
    /* ==第4步：实现参数还原== */

    /* 还原数据 */
    convert = param.convert;

    sys_prt_float(convert);

    sys_prt_withFunc("parameter restore successful");
}

void param_write(void)
{
    msp_param_write((uint8_t *)&param, sizeof(param));

    sys_prt_withFunc("parameters saved successfully");
}

void asp_param_save(void)
{
    param_typ paramOld;

    /* 读取写入前数据，用作比对，如一致则不写入 */
    msp_param_read((uint8_t *)&paramOld, sizeof(paramOld));

    /* ============================================================== */
    /* ==第5步：实现参数保存== */

    /* 同步数据 */
    param.convert = convert;

    if (!memcmp((void *)&param, (void *)&paramOld, sizeof(param)))
        return;

    /* 开启超时写入 */
    sys_timeout_start(100, param_write);
}

void asp_auto_init_param(void)
{

#if !USE_FLASH

    asp_eep24c64_init();

#endif

/* 重置EEP */
#if EEP_RESET

    msp_param_write((uint8_t *)&param_Default, sizeof(param_Default));

    sys_wait(10);

    sys_printf("force write default param");

#endif

    msp_param_read((uint8_t *)&param, sizeof(param));

    /* 如果参数存储区起始4字节不是 SET 则认为是初次烧录，需执行参数初始化 */
    if (strcmp(param.InitFlag, param_Default.InitFlag))
    {

#if !USE_FLASH

        /* 先擦除当前页，再写入，防止残留，导致出错 */
        asp_eep24c64_erase_page(PARAM_ADDR);

        sys_wait(10);

#endif

        /* 写入默认参数 */
        msp_param_write((uint8_t *)&param_Default, sizeof(param_Default));

        sys_wait(10);

        sys_printf("init: write default param");
    }

    /* 校验参数 */
    check_param();

    /* 还原参数 */
    paramRestore();
}

/* 校验参数 */
void check_param(void)
{
    msp_param_read((uint8_t *)&param, sizeof(param));

    /* ============================================================== */
    /* ==第6步：实现参数校验== */

    /* 还原数据 */
    convert = param.convert;

    if (!((convert > 0) && (convert < 1000)))
        goto lable;

    return;

lable:

    sys_error("param err, start default param");

#if !USE_FLASH

    /* 先擦除当前页，再写入，防止残留，导致出错 */
    asp_eep24c64_erase_page(PARAM_ADDR);

    sys_wait(10);

#endif

    /* 写入默认参数 */
    msp_param_write((uint8_t *)&param_Default, sizeof(param_Default));

    sys_wait(10);
}

/*********************************** END OF FILE ***********************************/
