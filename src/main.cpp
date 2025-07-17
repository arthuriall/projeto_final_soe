#include "lcd.hpp"
#include "button.hpp"
#include "color.hpp"
#include "util.hpp"

#include <wiringPi.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <iostream>

// Removido: extern std::atomic<bool> running;

int main() {
    std::cout << "Iniciando sistema de deteccao de cores...\n";

    if (wiringPiSetup() == -1) {
        std::cerr << "Erro: WiringPiSetup falhou!" << std::endl;
        std::cerr << "Por favor, reinicie o Raspberry Pi." << std::endl;
        return 1;
    }

    setupGPIO();
    setupButtonInterrupt(); // Esta interrupcao agora sera responsavel por interromper o loop principal

    lcdInit();
    showWelcomeMessages();

    cv::VideoCapture cap(0);
    if (!cap.isOpened() || !checkCameraWarmUp(cap)) {
        lcdClear();
        lcdPrint("ERRO: Camera!");
        lcdSetCursor(1,0);
        lcdPrint("Reinicie a Rasp."); // Nova mensagem para o usuario
        std::cerr << "Erro ao iniciar camera." << std::endl;
        std::cerr << "Por favor, reinicie o Raspberry Pi." << std::endl;
        
        // Mantem a mensagem do LCD visivel por alguns segundos antes de sair
        sleep(5); 
        if (lcd_fd != -1) close(lcd_fd); // Fecha os recursos do LCD antes de sair
        return 1;
    }

    showReadyMessage();

    std::thread t(colorThread, std::ref(cap));

    while (true) {
        // Este loop pode ser usado para outras tarefas da thread principal, se necessario.
        // Por enquanto, ele apenas fica ocioso.
        usleep(100000); 
    }

    // Esta parte do codigo so sera alcancada se o loop while(true) for encerrado,
    // o que implica um mecanismo de desligamento (por exemplo, um manipulador de sinal, ou a colorThread
    // terminando eventualmente e permitindo que a thread principal faca o join e saia).
    t.join(); 
    lcdClear(); lcdPrint("Encerrando...");
    cap.release();
    if (lcd_fd != -1) close(lcd_fd);
    std::cout << "Programa finalizado.\n";
    return 0;
}
