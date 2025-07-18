#include "util.hpp"  // Declarações dessas funções
#include "lcd.hpp"   // Funções para controle do display LCD
#include <unistd.h>  // Para usar usleep (delay em microssegundos)

// === Exibe mensagens iniciais de boas-vindas no LCD ===
void showWelcomeMessages() {
    lcdClear();                                // Limpa o LCD
    lcdSetCursor(0, 0); lcdPrint("  Bem-vindo(a)! ");   // Linha 1: Saudação
    lcdSetCursor(1, 0); lcdPrint("   Sistema ON   ");   // Linha 2: Status
    usleep(2000000);                            // Espera 2 segundos

    lcdClear();                                // Próxima mensagem
    lcdSetCursor(0, 0); lcdPrint(" Detector de ");       // Linha 1
    lcdSetCursor(1, 0); lcdPrint(" Cores do IoT ");      // Linha 2
    usleep(2000000);                            // Espera 2 segundos

    lcdClear();                                // Mensagem de instrução
    lcdSetCursor(0, 0); lcdPrint("  Calibre as  ");      // Linha 1
    lcdSetCursor(1, 0); lcdPrint("   cores, OK?   ");    // Linha 2
    usleep(2000000);                            // Espera 2 segundos
}

// === Mostra no LCD que o sistema está pronto ===
void showReadyMessage() {
    lcdClear();                                       // Limpa o display
    lcdSetCursor(0, 0); lcdPrint("Camera OK!");       // Linha 1: status da câmera
    lcdSetCursor(1, 0); lcdPrint("Botao=PAUSA/INICIA"); // Linha 2: instrução do botão
    usleep(2000000);                                  // Espera 2 segundos
}

// === Aguarda a câmera "aquecer" e começar a capturar corretamente ===
// Esse processo garante que o primeiro frame válido não seja lixo ou preto
bool checkCameraWarmUp(cv::VideoCapture& cap) {
    for (int i = 0; i < 15; ++i) {      // Tenta capturar 15 frames
        cv::Mat test;
        cap >> test;                    // Captura um frame
        if (!test.empty()) return true; // Se capturou corretamente, retorna sucesso
        usleep(200000);                // Espera 200 ms antes de tentar de novo
    }
    return false;                       // Se nenhum frame foi válido, retorna false
}
