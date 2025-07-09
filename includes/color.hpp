// ========================= Arquivo: include/color/color.hpp =========================
#ifndef COLOR_HPP
#define COLOR_HPP

#include <opencv2/opencv.hpp>
#include <string>

std::string detectColorHSV(int hue, int saturation, int value);
bool processFrameHSV(cv::VideoCapture& cap, int& h, int& s, int& v);
void colorThread(cv::VideoCapture& cap);

#endif
