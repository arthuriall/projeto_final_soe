#include "color.hpp"     // Declarações relacionadas à detecção de cor (ex: detectColorHSV)
#include "lcd.hpp"       // Controle do display LCD
#include <wiringPi.h>    // Acesso GPIO via WiringPi
#include <unistd.h>      // Funções de pausa, sleep, usleep
#include <iostream>      // Entrada e saída padrão (cout, cerr)
#include <atomic>        // Para variável atômica leituraAtiva, thread-safe

extern std::atomic<bool> leituraAtiva;  // Variável global atômica indicando se leitura está ativa

// Função que recebe valores HSV e retorna uma string com o nome da cor detectada
std::string detectColorHSV(int hue, int saturation, int value) {
    if (value < 30) return "PRETO";                                 // Muito escuro → preto
    if (saturation < 20 && value > 220) return "BRANCO";            // Pouca saturação e muito claro → branco
    if (saturation < 30 && value >= 30 && value <= 220) return "CINZA"; // Pouca saturação e médio brilho → cinza
    if (saturation < 60 || value < 60) return "INDEFINIDO";          // Pouca saturação ou escuro → indefinido
    
    // Verifica faixas de matiz (hue) para cores específicas
    if ((hue >= 0 && hue <= 8) || (hue >= 170 && hue <= 179)) return "VERMELHO"; // Vermelho no início/fim da faixa HSV
    if (hue >= 18 && hue <= 30) return "AMARELO";
    if (hue >= 45 && hue <= 75) return "VERDE";
    if (hue >= 85 && hue <= 95) return "CIANO";
    if (hue >= 105 && hue <= 130) return "AZUL";
    if (hue >= 140 && hue <= 160) return "ROXO";
    
    return "INDEFINIDO";  // Caso não se encaixe em nenhum intervalo
}

// Função que captura um frame da câmera, processa e obtém valores HSV médios de uma região central
bool processFrameHSV(cv::VideoCapture& cap, int& h, int& s, int& v) {
    cv::Mat frame;
    cap >> frame;  // Captura um novo frame da câmera (grava em 'frame')

    if (frame.empty()) return false;  // Retorna falso se frame inválido ou vazio

    // Aplica desfoque gaussiano para reduzir ruídos
    cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0);

    // Define o centro do frame
    int x = frame.cols / 2;
    int y = frame.rows / 2;

    // Define um retângulo ROI (região de interesse) de 10x10 pixels centrado
    cv::Rect roi(x - 5, y - 5, 10, 10);

    // Verifica se o ROI está dentro dos limites da imagem para evitar erros
    if (roi.x < 0 || roi.y < 0 || roi.width <= 0 || roi.height <= 0 ||
        roi.x + roi.width > frame.cols || roi.y + roi.height > frame.rows) {
        return false;  // Retorna falso se ROI inválido
    }

    // Recorta a região do frame correspondente ao ROI
    cv::Mat region = frame(roi), hsv;

    // Converte a região de BGR (padrão OpenCV) para HSV
    cv::cvtColor(region, hsv, cv::COLOR_BGR2HSV);

    // Calcula a média dos valores HSV na região
    cv::Scalar meanHSV = cv::mean(hsv);

    // Passa os valores médios para as variáveis por referência
    h = meanHSV[0];
    s = meanHSV[1];
    v = meanHSV[2];

    return true;  // Frame processado com sucesso
}
