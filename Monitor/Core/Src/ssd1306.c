#include "ssd1306.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

static struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
} SSD1306;

static const uint16_t Font7x10[] = {
    0x0000, 0x0000, 0x0000, 0x0000, // пробіл (32)
    0x0000, 0x0006, 0x5F06, 0x0000, // !
    0x0000, 0x0403, 0x0403, 0x0000, // "
    0x0000, 0x7F24, 0x7F24, 0x0000, // #
    0x0000, 0x0E1F, 0x1F0E, 0x0000, // $ 
};

void SSD1306_WriteCommand(uint8_t byte) {
    HAL_I2C_Mem_Write(&hi2c1, SSD1306_I2C_ADDR, 0x00, 1, &byte, 1, 100);
}

void SSD1306_WriteData(uint8_t* buffer, size_t buff_size) {
    HAL_I2C_Mem_Write(&hi2c1, SSD1306_I2C_ADDR, 0x40, 1, buffer, buff_size, 100);
}

void SSD1306_Init(void) {
    HAL_Delay(100);
    
    SSD1306_WriteCommand(0xAE); // Вимкнути дисплей
    SSD1306_WriteCommand(0x20); // Режим пам'яті
    SSD1306_WriteCommand(0x00); // Горизонтальна адресація
    SSD1306_WriteCommand(0xB0); // Початок сторінки
    SSD1306_WriteCommand(0xC0); // COM scan direction
    SSD1306_WriteCommand(0x00); // Low column nibble
    SSD1306_WriteCommand(0x10); // High column nibble
    SSD1306_WriteCommand(0x40); // Start line
    SSD1306_WriteCommand(0x81); // Контраст
    SSD1306_WriteCommand(0xFF);
    SSD1306_WriteCommand(0xA1); // Segment remap
    SSD1306_WriteCommand(0xA6); // Normal display
    SSD1306_WriteCommand(0xA8); // Multiplex ratio
    SSD1306_WriteCommand(0x3F);
    SSD1306_WriteCommand(0xA4); // Output follows RAM
    SSD1306_WriteCommand(0xD3); // Display offset
    SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0xD5); // Clock divide ratio
    SSD1306_WriteCommand(0xF0);
    SSD1306_WriteCommand(0xD9); // Pre-charge period
    SSD1306_WriteCommand(0x22);
    SSD1306_WriteCommand(0xDA); // COM hardware config
    SSD1306_WriteCommand(0x12);
    SSD1306_WriteCommand(0xDB); // VCOMH deselect level
    SSD1306_WriteCommand(0x20);
    SSD1306_WriteCommand(0x8D); // Charge pump (важливо!)
    SSD1306_WriteCommand(0x14);
    SSD1306_WriteCommand(0xAF); // Увімкнути дисплей
    
    SSD1306_Fill(Black);
    SSD1306_UpdateScreen();
}

void SSD1306_Fill(SSD1306_COLOR color) {
    memset(SSD1306_Buffer, (color == Black) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void SSD1306_UpdateScreen(void) {
    for (uint8_t i = 0; i < 8; i++) {
        SSD1306_WriteCommand(0xB0 + i);
        SSD1306_WriteCommand(0x00);
        SSD1306_WriteCommand(0x10);
        SSD1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);
    }
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    if (color == White) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void SSD1306_SetCursor(uint8_t x, uint8_t y) {
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

char SSD1306_WriteChar(char ch, SSD1306_COLOR color) {
    static const uint8_t Font5x7[] = {
        0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
        0x00, 0x42, 0x7F, 0x40, 0x00, // 1
        0x42, 0x61, 0x51, 0x49, 0x46, // 2
        0x21, 0x41, 0x45, 0x4B, 0x31, // 3
        0x18, 0x14, 0x12, 0x7F, 0x10, // 4
        0x27, 0x45, 0x45, 0x45, 0x39, // 5
        0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
        0x01, 0x71, 0x09, 0x05, 0x03, // 7
        0x36, 0x49, 0x49, 0x49, 0x36, // 8
        0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    };

    uint32_t b;
    if (ch >= '0' && ch <= '9') {
        for (int i = 0; i < 5; i++) {
            b = Font5x7[(ch - '0') * 5 + i];
            for (int j = 0; j < 8; j++) {
                if ((b << j) & 0x80) SSD1306_DrawPixel(SSD1306.CurrentX + i, (SSD1306.CurrentY + j), color);
                else SSD1306_DrawPixel(SSD1306.CurrentX + i, (SSD1306.CurrentY + j), !color);
            }
        }
        SSD1306.CurrentX += 6;
    } 
    else {
        SSD1306.CurrentX += 4; 
    }
    
    return ch;
}

void SSD1306_WriteString(char* str, SSD1306_COLOR color) {
    while (*str) {
        SSD1306_WriteChar(*str, color);
        str++;
    }
}
