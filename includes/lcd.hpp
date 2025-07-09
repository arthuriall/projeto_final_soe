// =========================== Arquivo: include/lcd/lcd.hpp ===========================
#ifndef LCD_HPP
#define LCD_HPP

#include <string>

void lcdInit();
void lcdClear();
void lcdSetCursor(int row, int col);
void lcdPrint(const std::string& text);
extern int lcd_fd;

#endif
