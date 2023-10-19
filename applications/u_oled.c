/*
 * @Author       : wang chao
 * @Date         : 2023-08-30 19:09:32
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-10-19 21:27:06
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
#define OLED_SPI_PIN_CLK GET_PIN(B, 7)  // PA7
#define OLED_SPI_PIN_MOSI GET_PIN(B, 6) // PA6
#define OLED_SPI_PIN_RES GET_PIN(B, 4)  // PA2
#define OLED_SPI_PIN_DC GET_PIN(B, 3)   // PA1
#define OLED_SPI_PIN_CS GET_PIN(B, 5)   // PA0

u8g2_t u8g2;

void u8g2_show_init(void)
{
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
    u8g2_SetFont(&u8g2, myfont_16);
    u8g2_ClearBuffer(&u8g2);
}

static char cha_buff[128] = {0};
static char chb_buff[128] = {0};
static char chc_buff[128] = {0};

void u8g2_charge_show(float v1, float v2)
{
    u8g2_ClearBuffer(&u8g2);
    // sprintf(cha_buff, "充电");
    u8g2_DrawUTF8(&u8g2, 6, 16, "充电中...");
    sprintf(chb_buff, "CH1电流: %0.1fA", v1);
    u8g2_DrawUTF8(&u8g2, 6, 36, chb_buff);
    sprintf(chc_buff, "CH2电流: %0.1fA", v2);
    u8g2_DrawUTF8(&u8g2, 6, 56, chc_buff);
    u8g2_SendBuffer(&u8g2);
}

void u8g2_discharge_show(float v1, float v2)
{
    u8g2_ClearBuffer(&u8g2);
    sprintf(cha_buff, "放电中...");
    u8g2_DrawUTF8(&u8g2, 6, 16, cha_buff);
    sprintf(chb_buff, "CH1电流: %0.1fA", v1);
    u8g2_DrawUTF8(&u8g2, 6, 36, chb_buff);
    sprintf(chc_buff, "CH2电流: %0.1fA", v2);
    u8g2_DrawUTF8(&u8g2, 6, 56, chc_buff);
    u8g2_SendBuffer(&u8g2);
}

void u8g2_stewing_show(float v1, float v2, rt_uint8_t direction)
{
    u8g2_ClearBuffer(&u8g2);
    if (direction == 1)
    {
        sprintf(cha_buff, "静置等待放电...");
    }
    else if (direction == 2)
    {
        sprintf(cha_buff, "静置等待充电...");
    }
    u8g2_DrawUTF8(&u8g2, 6, 16, cha_buff);
    sprintf(chb_buff, "CH1电流: %0.1fA", v1);
    u8g2_DrawUTF8(&u8g2, 6, 36, chb_buff);
    sprintf(chc_buff, "CH2电流: %0.1fA", v2);
    u8g2_DrawUTF8(&u8g2, 6, 56, chc_buff);
    u8g2_SendBuffer(&u8g2);
}

#endif
