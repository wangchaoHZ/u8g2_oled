/*
 * @Author       : wang chao
 * @Date         : 2023-08-30 19:09:32
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-09-02 22:13:48
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

#define USING_ST7567_SPI_12864S

#ifdef USING_ST7567_SPI_12864S

// SW Pins 定义
#define OLED_SPI_PIN_CLK GET_PIN(A, 7)  // PA7
#define OLED_SPI_PIN_MOSI GET_PIN(A, 6) // PA6
#define OLED_SPI_PIN_RES GET_PIN(A, 2)  // PA2
#define OLED_SPI_PIN_DC GET_PIN(A, 1)   // PA1
#define OLED_SPI_PIN_CS GET_PIN(A, 0)   // PA0

void u8g2_show(void)
{
    u8g2_t u8g2;
    rt_uint16_t index = 0;

    // Initialization
    u8g2_Setup_st7567_jlx12864g_6053_pn_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_rtthread);
    // u8x8_SetPin
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_SPI_CLOCK, OLED_SPI_PIN_CLK);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_SPI_DATA, OLED_SPI_PIN_MOSI);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_CS, OLED_SPI_PIN_CS);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_DC, OLED_SPI_PIN_DC);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_RESET, OLED_SPI_PIN_RES);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
#if (USING_SSD1312_IIC_12864S)
    // https://github.com/olikraus/u8g2/issues/1806
    u8g2_SendF(&u8g2, "c", 0xa0);
#endif

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_crox1cb_mf);

    u8g2_DrawStr(&u8g2, 20, 16, "FunctionA");
    u8g2_DrawStr(&u8g2, 20, 32, "FunctionB");
    u8g2_DrawStr(&u8g2, 20, 48, "FunctionC");
    u8g2_DrawStr(&u8g2, 20, 64, "FunctionD");

    u8g2_SendBuffer(&u8g2);

    for (rt_uint8_t loop = 0; loop < 2000; loop++)
    {
        index++;
        LOG_W("select index = %d", index);
        u8g2_ClearBuffer(&u8g2);
        u8g2_DrawStr(&u8g2, 8, index * 16 - 1, ">");

        u8g2_DrawStr(&u8g2, 20, 16, "FunctionA");
        u8g2_DrawStr(&u8g2, 20, 32, "FunctionB");
        u8g2_DrawStr(&u8g2, 20, 48, "FunctionC");
        u8g2_DrawStr(&u8g2, 20, 64, "FunctionD");

        u8g2_SendBuffer(&u8g2);

        if (index == 4)
        {
            index = 0;
            u8g2_ClearBuffer(&u8g2);
        }
        rt_thread_mdelay(1000);
    }
}

MSH_CMD_EXPORT(u8g2_show, u8g2 oled demo show);

#else
/**
 *  enable using oled type
 */
//
#define USING_SSD1312_IIC_12864S 0u
//
#define USING_SSD1306_IIC_12864S 1u
//
#define USING_SH1106_IIC_12864S 0u

void u8g2_show(void)
{
    u8g2_t u8g2;
    rt_uint16_t index = 0;

    // Initialization
#if (USING_SH1106_IIC_12864S)
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_rtthread);
#elif (USING_SSD1312_IIC_12864S)
    u8g2_Setup_ssd1312_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_rtthread);
#elif (USING_SSD1306_IIC_12864S)
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_rtthread);
#endif

    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, U8X8_PIN_I2C_CLOCK);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, U8X8_PIN_I2C_DATA);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
#if (USING_SSD1312_IIC_12864S)
    // https://github.com/olikraus/u8g2/issues/1806
    u8g2_SendF(&u8g2, "c", 0xa0);
#endif

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_crox1cb_mf);

    u8g2_DrawStr(&u8g2, 20, 16, "FunctionA");
    u8g2_DrawStr(&u8g2, 20, 32, "FunctionB");
    u8g2_DrawStr(&u8g2, 20, 48, "FunctionC");
    u8g2_DrawStr(&u8g2, 20, 64, "FunctionD");

    u8g2_SendBuffer(&u8g2);

    for (rt_uint8_t loop = 0; loop < 20; loop++)
    {
        index++;
        LOG_W("select index = %d", index);
        u8g2_ClearBuffer(&u8g2);
        u8g2_DrawStr(&u8g2, 8, index * 16 - 1, ">");

        u8g2_DrawStr(&u8g2, 20, 16, "FunctionA");
        u8g2_DrawStr(&u8g2, 20, 32, "FunctionB");
        u8g2_DrawStr(&u8g2, 20, 48, "FunctionC");
        u8g2_DrawStr(&u8g2, 20, 64, "FunctionD");

        u8g2_SendBuffer(&u8g2);

        if (index == 4)
        {
            index = 0;
            u8g2_ClearBuffer(&u8g2);
        }
        rt_thread_mdelay(1000);
    }
}

MSH_CMD_EXPORT(u8g2_show, u8g2 oled demo show);

#endif
