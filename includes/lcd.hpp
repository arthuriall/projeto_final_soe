// Proteção contra múltiplas inclusões do mesmo cabeçalho
#ifndef LCD_HPP          // Se LCD_HPP ainda não foi definido...
#define LCD_HPP          // Define LCD_HPP para impedir inclusões múltiplas

#include <string>        // Necessário para usar std::string nas funções abaixo

// Inicializa o LCD (configura o barramento I2C e parâmetros do display)
void lcdInit();

// Limpa completamente o conteúdo do display LCD
void lcdClear();

// Define a posição do cursor no LCD: linha (row) e coluna (col)
void lcdSetCursor(int row, int col);

// Escreve um texto no display, a partir da posição atual do cursor
void lcdPrint(const std::string& text);

// Variável global (externa) que armazena o file descriptor do LCD (aberto via I2C)
extern int lcd_fd;

#endif  // Fim da proteção de inclusão múltipla
