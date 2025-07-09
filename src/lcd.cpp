
// =========================== Arquivo: src/lcd/lcd.cpp ===========================
#include "lcd.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <iostream>

#define I2C_ADDR 0x27
#define LCD_BACKLIGHT 0x08
#define ENABLE 0b00000100
#define LCD_CHR 1
#define LCD_CMD 0

int lcd_fd = -1;

static void lcd_toggle_enable(uint8_t bits) {
    if (lcd_fd == -1) return;
    write(lcd_fd, &bits, 1);
    usleep(500);
    uint8_t enableBits = bits | ENABLE;
    write(lcd_fd, &enableBits, 1);
    usleep(500);
    enableBits = bits & ~ENABLE;
    write(lcd_fd, &enableBits, 1);
    usleep(500);
}

static void lcd_byte(uint8_t bits, uint8_t mode) {
    if (lcd_fd == -1) return;
    uint8_t high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
    uint8_t low  = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

    write(lcd_fd, &high, 1);
    lcd_toggle_enable(high);
    write(lcd_fd, &low, 1);
    lcd_toggle_enable(low);
}

void lcdInit() {
    lcd_fd = open("/dev/i2c-1", O_RDWR);
    if (lcd_fd < 0 || ioctl(lcd_fd, I2C_SLAVE, I2C_ADDR) < 0) {
        std::cerr << "Erro ao inicializar LCD" << std::endl;
        exit(1);
    }
    lcd_byte(0x33, LCD_CMD);
    lcd_byte(0x32, LCD_CMD);
    lcd_byte(0x06, LCD_CMD);
    lcd_byte(0x0C, LCD_CMD);
    lcd_byte(0x28, LCD_CMD);
    lcd_byte(0x01, LCD_CMD);
    usleep(2000);
}

void lcdClear() {
    lcd_byte(0x01, LCD_CMD);
    usleep(2000);
}

void lcdSetCursor(int row, int col) {
    int rowAddr[] = {0x00, 0x40};
    lcd_byte(0x80 + rowAddr[row] + col, LCD_CMD);
}

void lcdPrint(const std::string& text) {
    for (char c : text) {
        lcd_byte(c, LCD_CHR);
    }
}
