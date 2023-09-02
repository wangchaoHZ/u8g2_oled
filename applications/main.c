/*
 * @Author       : wang chao
 * @Date         : 2023-08-30 18:30:30
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-08-30 18:37:45
 * @FilePath     : main.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

int main(void)
{
    LOG_I("This is an u8g2 demo on rtt!");
    LOG_I("SCL Pin: GET_PIN(B, 6)");
    LOG_I("SDA Pin: GET_PIN(B, 7)");
    rt_thread_mdelay(200);

    extern void u8g2_show(void);
    u8g2_show();

    return RT_EOK;
}
