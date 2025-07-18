#include "lcd.hpp"                 // Cabeçalho com declarações das funções LCD
#include <fcntl.h>                 // Para open()
#include <unistd.h>                // Para usleep() e write()
#include <linux/i2c-dev.h>         // Definições I2C específicas do Linux
#include <sys/ioctl.h>             // Para ioctl()
#include <iostream>                // Para mensagens de erro

// Endereço I2C do LCD (verificado com i2cdetect -y 1)
#define I2C_ADDR 0x27              // Endereço padrão do módulo I2C para LCD
#define LCD_BACKLIGHT 0x08         // Bit para manter o backlight ligado
#define ENABLE 0b00000100          // Bit de habilitação (Enable bit)
#define LCD_CHR 1                  // Modo: envio de caractere
#define LCD_CMD 0                  // Modo: envio de comando

int lcd_fd = -1;                   // File descriptor para comunicação I2C

// === Função auxiliar: Envia pulso de enable para o LCD ===
static void lcd_toggle_enable(uint8_t bits) {
    if (lcd_fd == -1) return;          // Se o LCD não foi inicializado, sai
    write(lcd_fd, &bits, 1);           // Envia estado atual
    usleep(500);                       // Aguarda 500 us
    uint8_t enableBits = bits | ENABLE;
    write(lcd_fd, &enableBits, 1);     // Seta o bit Enable (HIGH)
    usleep(500);                       // Aguarda
    enableBits = bits & ~ENABLE;
    write(lcd_fd, &enableBits, 1);     // Reseta o bit Enable (LOW)
    usleep(500);
}

// === Função auxiliar: Envia byte para o LCD (comando ou caractere) ===
static void lcd_byte(uint8_t bits, uint8_t mode) {
    if (lcd_fd == -1) return;          // Verifica se o LCD está pronto

    // Envia os 4 bits mais significativos + modo (CMD ou CHR) + backlight
    uint8_t high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
    // Envia os 4 bits menos significativos
    uint8_t low  = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

    write(lcd_fd, &high, 1);
    lcd_toggle_enable(high);
    write(lcd_fd, &low, 1);
    lcd_toggle_enable(low);
}

// === Inicializa o display LCD ===
void lcdInit() {
    lcd_fd = open("/dev/i2c-1", O_RDWR);                 // Abre o barramento I2C-1
    if (lcd_fd < 0 || ioctl(lcd_fd, I2C_SLAVE, I2C_ADDR) < 0) {
        std::cerr << "Erro ao inicializar LCD" << std::endl;
        exit(1);                                         // Encerra se falhar
    }

    // Sequência de inicialização para modo de 4 bits
    lcd_byte(0x33, LCD_CMD);  // Inicialização
    lcd_byte(0x32, LCD_CMD);  // Modo 4 bits
    lcd_byte(0x06, LCD_CMD);  // Movimento automático do cursor
    lcd_byte(0x0C, LCD_CMD);  // Display ligado, cursor desligado
    lcd_byte(0x28, LCD_CMD);  // Função: 2 linhas, 5x8 pontos
    lcd_byte(0x01, LCD_CMD);  // Limpa o display
    usleep(2000);             // Espera para concluir o clear
}

// === Limpa completamente o display ===
void lcdClear() {
    lcd_byte(0x01, LCD_CMD); // Comando de clear
    usleep(2000);            // Tempo de espera necessário
}

// === Move o cursor para a posição (linha, coluna) ===
void lcdSetCursor(int row, int col) {
    int rowAddr[] = {0x00, 0x40};                        // Endereço base das linhas 0 e 1
    lcd_byte(0x80 + rowAddr[row] + col, LCD_CMD);        // Envia comando de posicionamento
}

// === Imprime texto na posição atual do cursor ===
void lcdPrint(const std::string& text) {
    for (char c : text) {
        lcd_byte(c, LCD_CHR);    // Envia caractere a caractere
    }
}
