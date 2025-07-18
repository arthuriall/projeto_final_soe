// ======================== Arquivo: src/button/button.cpp ========================
#include "button.hpp"      // Cabeçalho com as declarações de setupGPIO, setupButtonInterrupt etc.
#include <wiringPi.h>      // Biblioteca WiringPi para controle de GPIOs com interrupções
#include <atomic>          // Para usar a variável leituraAtiva de forma segura entre threads
#include <iostream>        // Para mensagens de erro com std::cerr
#include <unistd.h>        // Para funções como sleep/usleep

// Define o pino do LED (WiringPi 3 = GPIO22)
#define LED 3

// Define o pino do botão (WiringPi 7 = GPIO4)
#define BOTAO 7

// Variável atômica global que controla se a leitura está ativa ou pausada
std::atomic<bool> leituraAtiva(false);

// Variável estática que armazena o tempo da última interrupção (para debounce)
static unsigned long last_interrupt_time = 0;

// Tempo mínimo entre cliques para considerar válido (em milissegundos)
const long DEBOUNCE_MS = 200;

// === Função de configuração dos pinos do LED e botão ===
void setupGPIO() {
    pinMode(LED, OUTPUT);           // Define LED como saída
    digitalWrite(LED, LOW);         // Inicia com LED desligado
    pinMode(BOTAO, INPUT);          // Define botão como entrada
    pullUpDnControl(BOTAO, PUD_UP); // Ativa resistor de pull-up interno no botão
}

// === Função chamada quando o botão é pressionado (interrupção) ===
void buttonISR() {
    unsigned long now = millis(); // Pega o tempo atual em milissegundos

    // Verifica se passou tempo suficiente desde a última interrupção (debounce)
    if (now - last_interrupt_time > DEBOUNCE_MS) {
        leituraAtiva = !leituraAtiva;       // Inverte o estado da leitura
        last_interrupt_time = now;          // Atualiza o tempo da última interrupção
    }
}

// === Configura interrupção no botão ===
void setupButtonInterrupt() {
    // Configura interrupção para borda de descida (quando botão é pressionado)
    if (wiringPiISR(BOTAO, INT_EDGE_FALLING, &buttonISR) < 0) {
        std::cerr << "Erro ao configurar interrupcao do botao" << std::endl;
        exit(1); // Encerra o programa se falhar
    }
}
