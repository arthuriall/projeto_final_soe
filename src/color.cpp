
// ======================== Arquivo: src/color/color.cpp ========================
#include "color.hpp"
#include "lcd.hpp"
#include <wiringPi.h>
#include <unistd.h>
#include <iostream>
#include <atomic>

extern std::atomic<bool> leituraAtiva;
extern std::atomic<bool> running;

std::string detectColorHSV(int hue, int saturation, int value) {
    if (value < 30) return "PRETO";
    if (saturation < 20 && value > 220) return "BRANCO";
    if (saturation < 30 && value >= 30 && value <= 220) return "CINZA";
    if (saturation < 60 || value < 60) return "INDEFINIDO";
    if ((hue >= 0 && hue <= 8) || (hue >= 170 && hue <= 179)) return "VERMELHO";
    if (hue >= 18 && hue <= 30) return "AMARELO";
    if (hue >= 45 && hue <= 75) return "VERDE";
    if (hue >= 85 && hue <= 95) return "CIANO";
    if (hue >= 105 && hue <= 130) return "AZUL";
    if (hue >= 140 && hue <= 160) return "ROXO";
    return "INDEFINIDO";
}

bool processFrameHSV(cv::VideoCapture& cap, int& h, int& s, int& v) {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) return false;

    cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0);
    int x = frame.cols / 2, y = frame.rows / 2;
    cv::Rect roi(x - 5, y - 5, 10, 10);
    if (roi.x < 0 || roi.y < 0 || roi.width <= 0 || roi.height <= 0) return false;

    cv::Mat region = frame(roi), hsv;
    cv::cvtColor(region, hsv, cv::COLOR_BGR2HSV);
    cv::Scalar meanHSV = cv::mean(hsv);
    h = meanHSV[0]; s = meanHSV[1]; v = meanHSV[2];
    return true;
}

void colorThread(cv::VideoCapture& cap) {
    bool lastState = !leituraAtiva;
    bool boot = true;

    while (running) {
        if (boot) {
            lcdClear();
            lcdSetCursor(0, 0);
            lcdPrint("Pressione o");
            lcdSetCursor(1, 0);
            lcdPrint("Botao para Iniciar");
            usleep(1000000);
            boot = false;
        }
        if (leituraAtiva != lastState) {
            lastState = leituraAtiva;
            lcdClear();
            lcdSetCursor(0, 0);
            lcdPrint(leituraAtiva ? "Leitura ATIVA" : "Leitura PAUSADA");
            lcdSetCursor(1, 0);
            lcdPrint(leituraAtiva ? "Detectando..." : "Aperte o Botao");
            usleep(1500000);
        }
        if (leituraAtiva) {
            int h, s, v;
            if (processFrameHSV(cap, h, s, v)) {
                std::string cor = detectColorHSV(h, s, v);
                lcdClear();
                lcdSetCursor(0, 0);
                lcdPrint("Cor Detectada:");
                lcdSetCursor(1, 0);
                lcdPrint(cor);
                digitalWrite(3, HIGH);
                usleep(500000);
                digitalWrite(3, LOW);
                usleep(500000);
            } else {
                lcdClear();
                lcdPrint("Aguardando Frame");
                lcdSetCursor(1, 0);
                lcdPrint("da Camera...");
                digitalWrite(3, LOW);
                usleep(500000);
            }
        } else {
            digitalWrite(3, LOW);
            usleep(300000);
        }
    }
    digitalWrite(3, LOW);
}
