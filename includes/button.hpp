
#ifndef BUTTON_HPP
#define BUTTON_HPP

// Função responsável por configurar os pinos GPIO necessários (ex: modo de entrada, pull-up, etc.)
void setupGPIO();

// Função que configura a interrupção associada ao botão físico.
// Essa função define qual função será chamada (ISR) quando o botão for pressionado.
void setupButtonInterrupt();

// Função que será chamada automaticamente quando o botão for pressionado.
// ISR = Interrupt Service Routine (Rotina de Serviço de Interrupção)
void buttonISR();

#endif // BUTTON_HPP
