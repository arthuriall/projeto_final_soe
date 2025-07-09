
// ============================ Arquivo: src/main.cpp ============================
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

extern std::atomic<bool> running;

int main() {
    std::cout << "Iniciando sistema de deteccao de cores...\n";

    if (wiringPiSetup() == -1) {
        std::cerr << "Erro: WiringPiSetup falhou!" << std::endl;
        return 1;
    }

    setupGPIO();
    setupButtonInterrupt();

    lcdInit();
    showWelcomeMessages();

    cv::VideoCapture cap(0);
    if (!cap.isOpened() || !checkCameraWarmUp(cap)) {
        lcdClear(); lcdPrint("ERRO: Camera!");
        std::cerr << "Erro ao iniciar camera." << std::endl;
        return 1;
    }

    showReadyMessage();
    std::thread t(colorThread, std::ref(cap));

    while (running) {
        usleep(100000);
    }

    t.join();
    lcdClear(); lcdPrint("Encerrando...");
    cap.release();
    if (lcd_fd != -1) close(lcd_fd);
    std::cout << "Programa finalizado.\n";
    return 0;
}
