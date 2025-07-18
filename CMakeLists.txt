cmake_minimum_required(VERSION 3.10)
project(ColorDetector)

set(CMAKE_CXX_STANDARD 17)

# Incluir diretórios de cabeçalhos
include_directories(includes)

# Adiciona os arquivos fonte
add_executable(ColorDetector
    src/main.cpp
    src/lcd.cpp
    src/button.cpp
    src/color.cpp
    src/util.cpp
)

# Linkar com OpenCV e outras libs necessárias
find_package(OpenCV REQUIRED)
target_link_libraries(ColorDetector ${OpenCV_LIBS} wiringPi)
