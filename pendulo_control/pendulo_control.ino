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
// LEARN : debounce -> filtrar ruídos.
//////////////////////////////////////////// BIBLIOTECAS ///////////////////////////////////////////

#include <LiquidCrystal_I2C.h>

//////////////////////////////////////////// CONSTANTES ////////////////////////////////////////////

//// Constantes do controlador ////
const double K_THETA = -3.1623;
const double K_THETA_PONTO = -2.3353;
const double K_ALPHA = 80.6651;
const double K_ALPHA_PONTO = 12.71;
const double K[4] = {K_THETA, K_THETA_PONTO, K_ALPHA, K_ALPHA_PONTO};
const double TS = 0.04; // segundos

//// Incremento dos encoders ////
const double INCREMENTO_PEND = double(360)/1200;
// TODO : Descobrir P/R
const double INCREMENTO_MOT = 0;

//// Pinos ////
// TODO : definir pino dos componentes
const int ENC_A_PEND = 2; // 23
const int ENC_B_PEND = 3; // 22
const int ENC_A_MOT = 4;
const int ENC_B_MOT = 5;

const int SW_PIN = 0;
const int BOT_EMERG_PIN = 0;

const int LED_1 = 0;
const int LED_2 = 0;
const int LED_3 = 0;

const int IN_1_MOT = 0; // Snetido horário 
const int IN_2_MOT = 0; // Sentido anti-horário 
const int PWM_PONTE_H = 0; // 

const double SETP_T = 0;
const double SETP_TP = 0;
const double SETP_A = 0;
const double SETP_AP = 0;
const double SETPOINT[4] = {SETP_T, SETP_TP, SETP_A, SETP_AP};

///////////////////////////////////////// VARIAVEIS GLOBAIS ////////////////////////////////////////

LiquidCrystal_I2C lcd(0x27, 16, 2); // TODO : Confirmar se o endereco eh 0x27. 
double erro[4];

volatile double ang_enc_pend, ang_enc_mot;

double theta, theta_ponto, alpha, alpha_ponto;

unsigned long tempo_inicial, tempo_amostra;

double tempo;

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

volatile double saturacao_encoder(volatile double angulo)
{
	if(angulo < -179)
    {
        angulo += 360;
    }
    else if(angulo > 180)
    {
        angulo -= 360;
    }

	return angulo;
}

void atualiza_enc_b_pend()
{	
	int leitura_a_pend = digitalRead(ENC_A_PEND);
	if(leitura_a_pend == LOW)
	{
		ang_enc_pend -= INCREMENTO_PEND;
	}
	else
	{
		ang_enc_pend += INCREMENTO_PEND;
	}

	ang_enc_pend = saturacao_encoder(ang_enc_pend);

}

void atualiza_enc_b_mot()
{
	int leitura_a_mot = digitalRead(ENC_A_MOT);
	if(leitura_a_mot == LOW)
	{
		ang_enc_mot -= INCREMENTO_MOT;
	}
	else
	{
		ang_enc_mot += INCREMENTO_MOT;
	}

	ang_enc_mot = saturacao_encoder(ang_enc_mot);
}

void atualiza_enc_a_pend()
{
	int leitura_b_pend = digitalRead(ENC_B_PEND);
	if(leitura_b_pend == LOW)
	{
		ang_enc_pend += INCREMENTO_PEND;
	}
	else
	{
		ang_enc_pend -= INCREMENTO_PEND;
	}

	ang_enc_pend = saturacao_encoder(ang_enc_pend);
}

void atualiza_enc_a_mot()
{
	int leitura_b_mot = digitalRead(ENC_B_MOT);
	if(leitura_b_mot == LOW)
	{
		ang_enc_mot += INCREMENTO_MOT;
	}
	else
	{
		ang_enc_mot -= INCREMENTO_MOT;
	}

	ang_enc_mot = saturacao_encoder(ang_enc_mot);
}

//////////////////////////////////////// FUNCOES DE PROCESSO ///////////////////////////////////////

void movimenta_motor(double sinal_de_controle)
{
	// Saturação
	if(sinal_de_controle > 12)
	{
		sinal_de_controle = 12;
	}

	else if (sinal_de_controle < -12)
	{
		sinal_de_controle = -12;
	}

	double pwm = map(fabs(sinal_de_controle), 0, 12, 0, 255);
	analogWrite(PWM_PONTE_H, pwm)

	if(sinal_de_controle > 0)
	{
		digitalWrite(IN_1_MOT, HIGH);
  		digitalWrite(IN_2_MOT, LOW);
	}
	else if(sinal_de_controle < 0)
	{
		digitalWrite(IN_1_MOT, LOW);
  		digitalWrite(IN_2_MOT, HIGH);
	}
}

bool bancada_ok(){};

double calcula_derivada(double dentaVal, double deltaT){};

double sinal_de_controle(double[4] erro){};

void interrupcao_emergencia(){};

void atualiza_lcd(){};

/////////////////////////////////////////////// SETUP //////////////////////////////////////////////

void setup()
{
	// Inicializando porta Serial para debug
	Serial.begin(9600);

	// Configurar pinos //
	// Encoders
	pinMode(ENC_A_PEND, INPUT_PULLUP);
	pinMode(ENC_B_PEND, INPUT_PULLUP);
	pinMode(ENC_A_MOT, INPUT_PULLUP);
	pinMode(ENC_B_MOT, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(ENC_A_PEND), atualiza_enc_a_pend, RISING);
	attachInterrupt(digitalPinToInterrupt(ENC_B_PEND), atualiza_enc_b_pend, RISING);
	attachInterrupt(digitalPinToInterrupt(ENC_A_MOT), atualiza_enc_a_mot, RISING);
	attachInterrupt(digitalPinToInterrupt(ENC_B_MOT), atualiza_enc_b_mot, RISING);

	pinMode(SW_PIN, INPUT);
	pinMode(BOT_EMERG_PIN, INPUT_PULLUP);

	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);
	pinMode(LED_3, OUTPUT);

	// Motor
	pinMode(IN_1_MOT, OUTPUT);
	pinMode(IN_2_MOT, OUTPUT);
	digitalWrite(IN_1_MOT, LOW); 
	digitalWrite(IN_2_MOT, LOW);

	// Configurar LCD
	lcd.init();
	lcd.backlight();
	lcd.clear();

	tempo_inicial = millis();
	ang_enc_pend = 0;
	ang_enc_mot = 0;
}

////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

void loop()
{
	delay(TS*1000);
	tempo_amostra = millis();
	tempo = (tempo_amostra - tempo_inicial)/1000;

	alpha = ang_enc_pend;
	theta = ang_enc_mot;

	// calcula derivadas

	// calcula sinal de controle

	// aciona motor

	Serial.print("Tempo: ");
	Serial.println(tempo);
	Serial.print("Angulo pêndulo: ");
	Serial.println(alpha);

	tempo_inicial = tempo_amostra;
}
