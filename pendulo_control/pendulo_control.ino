/**
 * @file pendulo_control.ino
 * @author Felipe de Paula Gabriel
 * @brief Script para controle de um Pendulo de Furuta utilizando ESP32.
 * @version 1.0.0
 * @date 2024-05-05
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

//// Constantes do controlador ////
const double K_THETA = -3.1623;
const double K_THETA_PONTO = -2.3353;
const double K_ALPHA = 80.6651;
const double K_ALPHA_PONTO = 12.71;
const double K[4] = {K_THETA, K_THETA_PONTO, K_ALPHA, K_ALPHA_PONTO};

//// Incremento dos encoders ////
// 600 P/R -> Pulso equivale a um período de onda A.
const double INCREMENTO_PEND = double(360)/600;
// TODO : Descobrir P/R
const double INCREMENTO_MOT = 0;

//// Pinos ////
// TODO : definir pino dos componentes
const int ENC_A_PEND = 0;
const int ENC_B_PEND = 0;
const int ENC_A_MOT = 0;
const int ENC_B_MOT = 0;

const int SW_PIN = 0;
const int BOT_EMERG_PIN = 0;

const int LED_1 = 0;
const int LED_2 = 0;
const int LED_3 = 0;

const int EN_MOT = 0; // PWM
const int IN_1_MOT = 0; // Snetido horário
const int IN_2_MOT = 0; // Sentido anti-horário

const int SETP_T = 0;
const int SETP_TP = 0;
const int SETP_A = 0;
const int SETP_AP = 0;
const int SETPOINT[4] = {SETP_T, SETP_TP, SETP_A, SETP_AP};

///////////////////////////////////////// VARIAVEIS GLOBAIS ////////////////////////////////////////

LiquidCrystal_I2C lcd(0x27, 16, 2); // TODO : Confirmar se o endereco eh 0x27. 
double erro[4];

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

void atualiza_encoder_A(){};

//////////////////////////////////////// FUNCOES DE PROCESSO ///////////////////////////////////////

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
	pinMode(ENC_A_PEND, INPUT);
	pinMode(ENC_B_PEND, INPUT);
	pinMode(ENC_A_MOT, INPUT);
	pinMode(ENC_B_MOT, INPUT);

	pinMode(SW_PIN, INPUT);
	pinMode(BOT_EMERG_PIN, INPUT_PULLUP);

	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);
	pinMode(LED_3, OUTPUT);

	pinMode(EN_MOT, OUTPUT);
	pinMode(IN_1_MOT, OUTPUT);
	pinMode(IN_2_MOT, OUTPUT);
	digitalWrite(IN_1_MOT, LOW); 
	digitalWrite(IN_2_MOT, LOW);

	// Configurar LCD
	lcd.init();
	lcd.backlight();
	lcd.clear();
}

////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

void loop()
{
	// put your main code here, to run repeatedly:
}
