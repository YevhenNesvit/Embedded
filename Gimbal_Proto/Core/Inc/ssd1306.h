#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f3xx_hal.h"

#define SSD1306_I2C_ADDR        0x78

#define SSD1306_WIDTH           128
#define SSD1306_HEIGHT          64

typedef enum {
    Black = 0x00,
    White = 0x01
} SSD1306_COLOR;

void SSD1306_Init(void);
void SSD1306_Fill(SSD1306_COLOR color);
void SSD1306_UpdateScreen(void);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char SSD1306_WriteChar(char ch, SSD1306_COLOR color);
void SSD1306_WriteString(char* str, SSD1306_COLOR color);
void SSD1306_SetCursor(uint8_t x, uint8_t y);

#endif
