#include "util.hpp"
#include "lcd.hpp"
#include <unistd.h>

void showWelcomeMessages() {
    lcdClear();
    lcdSetCursor(0, 0); lcdPrint("  Bem-vindo(a)! ");
    lcdSetCursor(1, 0); lcdPrint("   Sistema ON   ");
    usleep(2000000);

    lcdClear();
    lcdSetCursor(0, 0); lcdPrint(" Detector de ");
    lcdSetCursor(1, 0); lcdPrint(" Cores do IoT ");
    usleep(2000000);

    lcdClear();
    lcdSetCursor(0, 0); lcdPrint("  Calibre as  ");
    lcdSetCursor(1, 0); lcdPrint("   cores, OK?   ");
    usleep(2000000);
}

void showReadyMessage() {
    lcdClear();
    lcdSetCursor(0, 0); lcdPrint("Camera OK!");
    lcdSetCursor(1, 0); lcdPrint("Botao=PAUSA/INICIA");
    usleep(2000000);
}

bool checkCameraWarmUp(cv::VideoCapture& cap) {
    for (int i = 0; i < 15; ++i) {
        cv::Mat test;
        cap >> test;
        if (!test.empty()) return true;
        usleep(200000);
    }
    return false;
}
