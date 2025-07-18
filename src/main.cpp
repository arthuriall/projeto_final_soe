#include "lcd.hpp"       // Controle do display LCD
#include "button.hpp"    // Controle do botão físico com interrupção
#include "color.hpp"     // Funções de detecção de cor (HSV)
#include "util.hpp"      // Funções auxiliares (como setupGPIO)
#include <wiringPi.h>    // Biblioteca de GPIO do Raspberry Pi
#include <unistd.h>      // Para funções como sleep e usleep
#include <opencv2/opencv.hpp> // OpenCV para captura e processamento de imagem
#include <atomic> 
#include <iostream>

// Variáveis globais externas
extern std::atomic<bool> leituraAtiva; // Flag de controle do botão (ativa ou pausa a leitura)
extern int lcd_fd; // File descriptor do LCD I2C

int main() {
    std::cout << "Iniciando sistema de deteccao de cores...\n";

    // Inicialização do GPIO com WiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "Erro: WiringPiSetup falhou!" << std::endl;
        std::cerr << "Por favor, reinicie o Raspberry Pi." << std::endl;
        return 1;
    }

    // Configura GPIO e botão
    setupGPIO();
    setupButtonInterrupt(); 

    // Inicializa LCD e mostra mensagem de boas-vindas
    lcdInit();
    showWelcomeMessages();

    // Inicializa câmera padrão (dispositivo 0)
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) { 
        // Em caso de erro ao abrir a câmera, mostra mensagem e sai
        lcdClear();
        lcdPrint("ERRO: Camera!");
        lcdSetCursor(1, 0);
        lcdPrint("Reinicie a Rasp.");
        std::cerr << "Erro ao iniciar camera." << std::endl;
        std::cerr << "Por favor, reinicie o Raspberry Pi." << std::endl;
        
        sleep(5); // Espera 5 segundos antes de encerrar
        if (lcd_fd != -1) close(lcd_fd); 
        return 1;
    }

    // Mensagem de pronto
    showReadyMessage();

    // Estado anterior da leitura para detectar mudança
    bool lastState = !leituraAtiva; 
    bool boot = true; // Exibe mensagem inicial apenas uma vez

    // Loop principal do sistema
    while (true) {
        // Mensagem de instrução inicial
        if (boot) {
            lcdClear();
            lcdSetCursor(0, 0);
            lcdPrint("Pressione o");
            lcdSetCursor(1, 0);
            lcdPrint("Botao para Iniciar");
            usleep(1000000); // Espera 1 segundo
            boot = false;
        }

        // Atualiza LCD quando o estado de leitura mudar
        if (leituraAtiva != lastState) {
            lastState = leituraAtiva;
            lcdClear();
            lcdSetCursor(0, 0);
            lcdPrint(leituraAtiva ? "Leitura ATIVA" : "Leitura PAUSADA");
            lcdSetCursor(1, 0);
            lcdPrint(leituraAtiva ? "Detectando..." : "Aperte o Botao");
            usleep(1500000); // Espera 1.5 segundos para leitura do usuário
        }

        if (leituraAtiva) {
            int h, s, v;
            // Captura e processa frame da câmera
            if (processFrameHSV(cap, h, s, v)) { 
                std::string cor = detectColorHSV(h, s, v); // Detecta a cor pela média do ROI
                lcdClear();
                lcdSetCursor(0, 0);
                lcdPrint("Cor Detectada:");
                lcdSetCursor(1, 0);
                lcdPrint(cor);
                
                digitalWrite(3, HIGH); // Liga o LED
                usleep(500000);        // Espera 0.5 segundo
                digitalWrite(3, LOW);  // Desliga o LED
                usleep(500000);        // Espera 0.5 segundo
            } else {
                // Caso não consiga capturar frame válido
                lcdClear();
                lcdPrint("Aguardando Frame");
                lcdSetCursor(1, 0);
                lcdPrint("da Camera...");
                digitalWrite(3, LOW); // Garante que o LED esteja desligado
                usleep(500000);       // Espera 0.5 segundo
            }
        } else {
            // Se a leitura estiver pausada, garante LED desligado e reduz uso da CPU
            digitalWrite(3, LOW);
            usleep(300000); // Espera curta
        }
    }

    // Nunca chega aqui, mas está preparado para encerramento
    lcdClear();
    lcdPrint("Encerrando...");
    cap.release(); // Libera a câmera
    if (lcd_fd != -1) close(lcd_fd); // Fecha conexão com o LCD
    std::cout << "Programa finalizado.\n";
    return 0;
}
