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
 * Componentes (numero de pinos necessarios):
 * # ESP32;
 * # Switch para ligar e desligar (1);
 * # Botão de emergência (1);
 * # LCD com módulo I2C (2);
 * # LEDS de indicação (1 cada);
 * # 1 ponte H (2);
 * # 1 motor com encoder (3).
 * # 1 encoder (2).
 */
////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////// BIBLIOTECAS ///////////////////////////////////////////

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//////////////////////////////////////////// CONSTANTES ////////////////////////////////////////////

// Constantes do controlador
const double K_THETA = -3.1623;
const double K_THETA_PONTO = -4.8792;
const double K_ALPHA = 296.5928;
const double K_ALPHA_PONTO = 33.0033;
const double K[4] = {K_THETA, K_THETA_PONTO, K_ALPHA, K_ALPHA_PONTO};

// encoders # LEARN : Biblioteca RotaryEncoder de arduino ou implementar via 2 canais.
const int SWITCH_PIN = 0; // TODO : definir pino do switch.
const int BOTAO_EMERGENCIA_PIN = 0 // TODO : definir pino do botao.
//leds
// ponte H

///////////////////////////////////////// VARIAVEIS GLOBAIS ////////////////////////////////////////

LiquidCrystal_I2C lcd(0x27, 16, 2); // TODO : Confirmar se o endereco eh 0x27.

//////////////////////////////////////// FUNCOES AUXILIARES ////////////////////////////////////////

/**
 * @brief Escreve um texto iniciando em uma linha e coluna especifica. 
 * 
 * @param texto Conteudo a ser escrito.
 * @param linha Linha do display lcd.
 * @param coluna Coluna do display lcd.
 */
void escrever_lcd(String texto, int linha, int coluna)
{
	lcd.setCursor(coluna, linha);
	lcd.print(texto);
}

//////////////////////////////////////// FUNCOES DE PROCESSO ///////////////////////////////////////

double leitura_encoder(){};

void movimenta_motor(){};

bool bancada_ok(){};

double calcula_derivada(){};

double sinal_de_controle(){};

void interrupcao_emergencia(){};

void atualiza_lcd(){};

/////////////////////////////////////////////// SETUP //////////////////////////////////////////////

void setup()
{

	// Inicializando porta Serial para debug
	Serial.begin(9600);

	// Configurar pinos
	// INPUTS, OUTPUTS, ...
	pinMode(SWITCH_PIN, INPUT);

	// Interrupção 
	attachInterrupt(BOTAO_EMERGENCIA_PIN, interrupcao_emergencia, CHANGE)

	// LCD
	lcd.init();
	lcd.backlight();
	lcd.clear();
}

////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

void loop()
{
	// put your main code here, to run repeatedly:
}
