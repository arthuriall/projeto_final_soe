// Proteção contra múltiplas inclusões do mesmo cabeçalho
#ifndef UTIL_HPP        // Se UTIL_HPP ainda não foi definido...
#define UTIL_HPP        // Define UTIL_HPP para evitar inclusões repetidas

#include <opencv2/opencv.hpp>  // Inclusão da biblioteca OpenCV (necessária para tipos como cv::VideoCapture)

// Declaração da função que exibe mensagens de boas-vindas no LCD
void showWelcomeMessages();

// Declaração da função que exibe mensagem informando que o sistema está pronto
void showReadyMessage();

#endif  // Fim da proteção de inclusão múltipla
