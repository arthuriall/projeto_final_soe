#ifndef COLOR_HPP
#define COLOR_HPP

// Inclui a biblioteca principal do OpenCV, que contém funções para captura e processamento de imagens
#include <opencv2/opencv.hpp>

// Inclui a biblioteca de manipulação de strings da STL
#include <string>

// Função que recebe valores de matiz (hue), saturação e valor (brilho) no espaço de cores HSV
// e retorna uma string com o nome da cor detectada (ex: "VERMELHO", "AZUL", etc.)
std::string detectColorHSV(int hue, int saturation, int value);

// Função que processa um frame capturado da câmera (cv::VideoCapture)
// e extrai os valores médios de hue, saturation e value de uma região de interesse.
// Retorna true se a captura e o processamento forem bem-sucedidos.
bool processFrameHSV(cv::VideoCapture& cap, int& h, int& s, int& v);

#endif // COLOR_HPP
