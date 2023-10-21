/*
 * @Author       : wang chao
 * @Date         : 2023-08-30 18:30:30
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-10-19 20:14:06
 * @FilePath     : main.c
 * @Description  : helloworld
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

extern int u_adc_detect_thread(void);

extern int u_data_thread(void);

int main(void)
{
    rt_thread_mdelay(200);

    u_adc_detect_thread();

//    u_data_thread();

    return RT_EOK;
}
