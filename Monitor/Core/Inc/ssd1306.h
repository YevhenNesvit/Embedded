#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f3xx_hal.h"

// Адреса екрану (спробуй 0x7A, якщо 0x78 не спрацює)
#define SSD1306_I2C_ADDR        0x78

// Розміри екрану
#define SSD1306_WIDTH           128
#define SSD1306_HEIGHT          64

// Колір
typedef enum {
    Black = 0x00, // Чорний (піксель вимкнено)
    White = 0x01  // Білий (піксель увімкнено)
} SSD1306_COLOR;

// Функції, якими ми будемо користуватися
void SSD1306_Init(void);
void SSD1306_Fill(SSD1306_COLOR color);
void SSD1306_UpdateScreen(void);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char SSD1306_WriteChar(char ch, SSD1306_COLOR color);
void SSD1306_WriteString(char* str, SSD1306_COLOR color);
void SSD1306_SetCursor(uint8_t x, uint8_t y);

#endif
