/*
 * @Author       : wang chao
 * @Date         : 2023-08-30 19:09:32
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-08-30 22:08:32
 * @FilePath     : u_oled.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "u8g2_port.h"
#include "board.h"

#define DBG_TAG "oled"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static void u8g2_Setup_sh1106_i2c_128x64(int argc, char *argv[])
{
    u8g2_t u8g2;
    rt_uint16_t index = 0;

    // Initialization
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_rtthread);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, U8X8_PIN_I2C_CLOCK);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, U8X8_PIN_I2C_DATA);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_crox2c_mf);

    u8g2_DrawStr(&u8g2, 20, 16, "FunctionA");
    u8g2_DrawStr(&u8g2, 20, 32, "FunctionB");
    u8g2_DrawStr(&u8g2, 20, 48, "FunctionC");
    u8g2_DrawStr(&u8g2, 20, 64, "FunctionD");

    u8g2_SendBuffer(&u8g2);

    //    u8g2_SendBuffer(&u8g2);

    while (1)
    {
        index++;
        LOG_W("select index = %d", index);
        u8g2_ClearBuffer(&u8g2);

        u8g2_DrawStr(&u8g2, 20, 16, "FunctionA");
        u8g2_DrawStr(&u8g2, 20, 32, "FunctionB");
        u8g2_DrawStr(&u8g2, 20, 48, "FunctionC");
        u8g2_DrawStr(&u8g2, 20, 64, "FunctionD");

        u8g2_DrawStr(&u8g2, 8, index * 16 - 1, "#");
        u8g2_SendBuffer(&u8g2);

        if (index == 4)
        {
            index = 0;
        }
        rt_thread_mdelay(1000);
    }
}

MSH_CMD_EXPORT(u8g2_Setup_sh1106_i2c_128x64, U8G2 SH1106 128X64 SW I2C DEMO);
