/*
 * @Author       : wang chao
 * @Date         : 2023-10-16 22:53:41
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-10-19 22:51:43
 * @FilePath     : u_adc.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#include "u_adc.h"

#define DBG_TAG "LK"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define AI_ADC_DEV_NAME "adc1"

static rt_adc_device_t st_adc_dev;

extern void u8g2_show_init(void);
extern void u8g2_show(float cha, float chb, float chc);

static int st_adc_init(void)
{
    st_adc_dev = (rt_adc_device_t)rt_device_find(AI_ADC_DEV_NAME);
    if (st_adc_dev != RT_NULL)
    {
        rt_adc_enable(st_adc_dev, AI_ADC_DEV_NAME);
    }
    else
    {
        LOG_E("can't find %s device!", AI_ADC_DEV_NAME);
        return RT_ERROR;
    }
    return RT_EOK;
}

static rt_uint32_t st_get_adc_value(rt_uint8_t chx)
{
    if (st_adc_dev != RT_NULL)
    {
        return rt_adc_read(st_adc_dev, chx);
    }
    return 0;
}

static state_led_init(void)
{
    rt_pin_mode(GET_PIN(D, 12), PIN_MODE_OUTPUT);
}

static state_led_ctrl(rt_uint8_t cmd)
{
    rt_pin_write(GET_PIN(D, 12), cmd ? 1 : 0);
}

#define DQ1_Pin (GET_PIN(D, 1))
#define DQ2_Pin (GET_PIN(D, 3))

static dq_init(void)
{
    rt_pin_mode(DQ1_Pin, PIN_MODE_OUTPUT);
    rt_pin_mode(DQ2_Pin, PIN_MODE_OUTPUT);
    rt_pin_write(DQ2_Pin, 0);
    rt_pin_write(DQ2_Pin, 0);
}

typedef enum
{
    INIT_ACTION,
    CHARGE,
    WAIT_CHARGE_DONE,
    DISCHARGE,
    WAIT_DISCHARGE_DONE,
    STEWING
} run_state_t;

static run_state_t sys_run_state = INIT_ACTION;

float f_cur_1 = 0.0;
float f_cur_2 = 0.0;

static void do_init(void)
{
    rt_pin_write(DQ1_Pin, 1);
    rt_pin_write(DQ2_Pin, 0);
    //
    if (f_cur_1 > 0.5)
    {
        sys_run_state = CHARGE;
    }
}

static void do_charge(void)
{
    rt_pin_write(DQ1_Pin, 1);
    rt_pin_write(DQ2_Pin, 0);
    if (f_cur_1 < 0.5)
    {
        sys_run_state = WAIT_CHARGE_DONE;
    }
}

static void do_charge_wait(void)
{
    rt_pin_write(DQ1_Pin, 0);
    rt_pin_write(DQ2_Pin, 1);
    if (f_cur_2 > 0.5)
    {
        sys_run_state = DISCHARGE;
    }
}

static void do_discharge(void)
{
    rt_pin_write(DQ1_Pin, 0);
    rt_pin_write(DQ2_Pin, 1);
    if (f_cur_2 < 0.5)
    {
        sys_run_state = WAIT_DISCHARGE_DONE;
    }
}

static void do_discharge_wait(void)
{
    rt_pin_write(DQ1_Pin, 1);
    rt_pin_write(DQ2_Pin, 0);
    if (f_cur_1 > 0.5)
    {
        sys_run_state = CHARGE;
    }
}

extern void u8g2_init_action_show();
extern void u8g2_charge_show();
extern void u8g2_discharge_show();
extern void u8g2_stewing_show(rt_uint8_t direction);
extern void u8g2_current_show(float v1, float v2);

void u_adc_detect_thread_entry(void)
{
    rt_uint16_t loop = 0;
    rt_uint32_t adc_value1 = 0;
    rt_uint32_t adc_value2 = 0;
    rt_uint32_t adc_value3 = 0;

    float vout1 = 0.0;
    float vout2 = 0.0;
    float vout3 = 0.0;
    float vout1_t = 0.0;
    float vout2_t = 0.0;
    float vout3_t = 0.0;

    rt_uint8_t cnt = 0;

    for (;;)
    {
        adc_value1 = 0;
        adc_value2 = 0;
        adc_value3 = 0;

        cnt++;
        if (cnt > 3)
        {
            state_led_ctrl(0);
        }
        for (loop = 0; loop < 20; loop++)
        {
            adc_value1 += st_get_adc_value(10);
            adc_value2 += st_get_adc_value(11);
            // adc_value3 += st_get_adc_value(12);
            rt_thread_mdelay(10);
        }
        adc_value1 = adc_value1 / 20;
        adc_value2 = adc_value2 / 20;
        // adc_value3 = adc_value3 / 10;

        vout1_t = (float)adc_value1 / 4095 * 3.28 * 100 + 6.799999;
        vout2_t = (float)adc_value2 / 4095 * 3.28 * 100 + 6.799999;
        // vout3_t = (float)adc_value3 / 4095 * 3.28 * 100 + 6.799999;

        if (vout1_t * 2 > 250)
        {

            vout1 = (vout1_t * 2 - 250.0) / 43.1;
            vout2 = (vout2_t * 2 - 250.0) / 43.1;
        }
        else
        {
            vout1 = (vout1_t * 2 - 250.0) / 38.5;
            vout2 = (vout2_t * 2 - 250.0) / 38.5;
        }

        f_cur_1 = vout1;
        f_cur_2 = vout2;

        if (f_cur_1 < 0.35)
        {
            f_cur_1 = 0.0;
        }
        else if (f_cur_1 > 2.0)
        {
            f_cur_1 = 2.1;
        }

        if (f_cur_2 < 0.35)
        {
            f_cur_2 = 0.0;
        }
        else if (f_cur_2 > 2.0)
        {
            f_cur_2 = 2.1;
        }

        // 截止目前读取到了真实的电流数据，接下来做判断

        switch (sys_run_state)
        {
        case INIT_ACTION:
            do_init();
            u8g2_init_action_show();
            break;
        case CHARGE:
            do_charge();
            u8g2_charge_show();
            break;
        case WAIT_CHARGE_DONE:
            do_charge_wait();
            u8g2_stewing_show(1);
            break;
        case DISCHARGE:
            do_discharge();
            u8g2_discharge_show();
            break;
        case WAIT_DISCHARGE_DONE:
            do_discharge_wait();
            u8g2_stewing_show(2);
            break;
        }
        //
        u8g2_current_show(f_cur_1, f_cur_2);

        // LOG_I("adc_value1:%d", (int)vout1_t);
        // LOG_I("adc_value2:%d", (int)vout2_t);
        // LOG_I("adc_value3:%d", (int)vout3_t);
        state_led_ctrl(1);
        // rt_thread_mdelay(100);
        if (cnt > 3)
        {
            cnt = 0;
        }
    }
}

int u_adc_detect_thread(void)
{
    rt_thread_t tid;

    dq_init();
    state_led_init();
    u8g2_show_init();

    // st adc device init
    st_adc_init();

    tid = rt_thread_create("u_adc", u_adc_detect_thread_entry, RT_NULL, 1024, 12, 10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        goto __exit;
    }

    return RT_EOK;

__exit:
    if (tid)
        rt_thread_delete(tid);
    return RT_ERROR;
}

void u_data_thread_entry(void)
{
    LOG_W("=================================================");

    for (;;)
    {
        f_cur_1 = 2.0;
        f_cur_2 = 0.0;
        LOG_W("充电 111-NOW f_cur_1:%d f_cur_2:%d", (int)(f_cur_1 * 10), (int)(f_cur_2 * 10));
        rt_thread_mdelay(5000);

        f_cur_1 = 0.4;
        f_cur_2 = 0.2;
        LOG_W("静置 333-NOW f_cur_1:%d f_cur_2:%d", (int)(f_cur_1 * 10), (int)(f_cur_2 * 10));
        rt_thread_mdelay(5000);

        f_cur_1 = 0.4;
        f_cur_2 = 1.4;
        LOG_W("放电 222-NOW f_cur_1:%d f_cur_2:%d", (int)(f_cur_1 * 10), (int)(f_cur_2 * 10));
        rt_thread_mdelay(5000);

        f_cur_1 = 0.4;
        f_cur_2 = 0.2;
        LOG_W("静置 333-NOW f_cur_1:%d f_cur_2:%d", (int)(f_cur_1 * 10), (int)(f_cur_2 * 10));
        rt_thread_mdelay(5000);
    }
}

int u_data_thread(void)
{
    rt_thread_t tid;

    rt_thread_mdelay(1000);

    tid = rt_thread_create("u_data", u_data_thread_entry, RT_NULL, 512, 20, 10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        goto __exit;
    }

    return RT_EOK;

__exit:
    if (tid)
        rt_thread_delete(tid);
    return RT_ERROR;
}
