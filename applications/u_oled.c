/*
 * @Author       : wang chao
 * @Date         : 2023-08-30 19:09:32
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-08-30 19:35:17
 * @FilePath     : u_oled.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <u8g2_port.h>
#include "board.h"

static void u8g2_Setup_sh1106_i2c_128x64(int argc, char *argv[])
{
    u8g2_t u8g2;

    // Initialization
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_rtthread);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, U8X8_PIN_I2C_CLOCK);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, U8X8_PIN_I2C_DATA);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    /* full buffer example, setup procedure ends in _f */
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_timB18_tr);
    u8g2_DrawStr(&u8g2, 10, 30, "U8g2 on RT-Thread");
    u8g2_SendBuffer(&u8g2);
}

MSH_CMD_EXPORT(u8g2_Setup_sh1106_i2c_128x64, U8G2 SH1106 128X64 SW I2C DEMO);
