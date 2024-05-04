/**
 * @file pendulo_control.ino
 * @author Felipe de Paula Gabriel
 * @brief Script para controle de um Pendulo de Furuta utilizando ESP32.
 * @version 1.0.0
 * @date 2024-05-04
 * 
 * Este script realiza o controle de um pendulo invertido rotacional, conhecido como Pendulo de 
 * Furuta utilizando o microcontrolador ESP32. Foi utilizado um controle de realimentação de estados
 * com 4 estados: ângulo e velocidade do pêndulo e ângulo e velocidade do braço do motor.
 * 
 * Componentes:
 * # ESP32;
 * # Switch para ligar e desligar;
 * # Botão de emergência;
 * # LCD;
 * # LEDS de indicação.
 */
////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////// BIBLIOTECAS ///////////////////////////////////////////

//////////////////////////////////////////// CONSTANTES ////////////////////////////////////////////

const double K_THETA = -3.1623;
const double K_THETA_PONTO = -4.8792;
const double K_ALPHA = 296.5928;
const double K_ALPHA_PONTO = 33.0033;
// switch
// botao emerg
// lcd
// leds

////////////////////////////////////////////// FUNCOES /////////////////////////////////////////////

double leitura_encoder(){};

void movimenta_motor(){};

bool bancada_ok(){};

double calcula_derivada(){};

double sinal_de_controle(){};

void interrupcao_emergencia(){};

void atualiza_lcd(){};

/////////////////////////////////////////////// SETUP //////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:

}

////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

void loop() {
  // put your main code here, to run repeatedly:

}
