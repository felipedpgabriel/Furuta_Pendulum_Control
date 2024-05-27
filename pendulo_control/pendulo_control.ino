/**
 * @file pendulo_control.ino
 * @author Felipe de Paula Gabriel
 * @brief Script para controle de um Pendulo de Furuta utilizando ESP32.
 * @version 0.1
 * @date 2024-05-26
 * 
 * @copyright Copyright (c) 2024
 * 
 * Este script realiza o controle de um pendulo invertido rotacional, conhecido como Pendulo de 
 * Furuta utilizando o microcontrolador ESP32. Foi utilizado um controle de realimentação de estados
 * (LQR) com 4 estados: ângulo e velocidade do pêndulo e ângulo e velocidade do braço do motor.
 * 
 * Componentes (numero de pinos necessarios):
 * # ESP32;
 * # 1 encoder - OVW6-06-2HC (2).
 * # 1 ponte H - L298N (3);
 * # 1 motor com encoder (2).
 * # Switch para ligar e desligar (1);
 * # Botão de emergência (1);
 * # LCD com módulo I2C (2);
 * # LEDS de indicação (1 cada);
 */
////////////////////////////////////////////////////////////////////////////////////////////////////
// LEARN : debounce -> filtrar ruídos.
//////////////////////////////////////////// BIBLIOTECAS ///////////////////////////////////////////

// #include <LiquidCrystal_I2C.h>

//////////////////////////////////////////// CONSTANTES ////////////////////////////////////////////

//// Constantes do controlador ////
const float K_THETA = -0.1652;
const float K_THETA_PONTO = -0.7967;
const float K_ALPHA = 28.4287;
const float K_ALPHA_PONTO = 4.50;
const float K[4] = {K_THETA, K_THETA_PONTO, K_ALPHA, K_ALPHA_PONTO};
const float TS = 0.04; // segundos

const int SETP_T = 0;
const int SETP_TP = 0;
const int SETP_A = 0;
const int SETP_AP = 0;
const int SETPOINT[4] = {SETP_T, SETP_TP, SETP_A, SETP_AP};

//// Resolução dos encoders ////
const float INCREMENTO_PEND = float(360)/1200;
// TODO : Descobrir P/R
const float INCREMENTO_MOT = 0;

//// Pinos ////
const int ENC_A_PEND = 23; // 2
const int ENC_B_PEND = 22; // 3
// TODO : definir pinos do encoder do motor
const int ENC_A_MOT = 0; // 4
const int ENC_B_MOT = 0; // 5

// TODO : definir pinos para ponte h
const int IN_1_MOT = 0; // Sentido horário 
const int IN_2_MOT = 0; // Sentido anti-horário 
const int PWM_PONTE_H = 0; // 

// const int SW_PIN = 0;
// const int BOT_EMERG_PIN = 0;

// const int LED_1 = 0;
// const int LED_2 = 0;
// const int LED_3 = 0;

///////////////////////////////////////// VARIAVEIS GLOBAIS ////////////////////////////////////////

// lcd //
// LiquidCrystal_I2C lcd(0x27, 16, 2); // TODO : Confirmar se o endereco eh 0x27. 

// Leitura dos encoders //
volatile float ang_enc_pend = 0, ang_enc_mot = 0;

// Variáveis para amostragem //
float theta, theta_ponto, alpha, alpha_ponto;
unsigned long tempo_amostra;

// Variáveis para valores anteriores //
float alpha_inicial, theta_inicial;
unsigned long tempo_inicial;

// Erro das variáveis de estado //
float erro[4];

// Intervalo de tempo na amostragem//
double delta_tempo; // milissegundos

//////////////////////////////////////// FUNCOES AUXILIARES ////////////////////////////////////////

/**
 * @brief Escreve um texto iniciando em uma linha e coluna especifica. 
 * 
 * @param texto Conteudo a ser escrito.
 * @param linha Linha do display lcd.
 * @param coluna Coluna do display lcd.
 */
// void escrever_lcd(String texto, int linha, int coluna)
// {
// 	lcd.setCursor(coluna, linha);
// 	lcd.print(texto);
// }

/**
 * @brief Função de interrupção para o canal B do encoder do pêndulo.
 * 		  Incrementa ou decrementa o valore de angulo INCREMENTO_PEND a cada pulso lido.
 */
void atualiza_enc_b_pend()
{	
	// Atualização de ângulo //
	int leitura_a_pend = digitalRead(ENC_A_PEND);
	if(leitura_a_pend == LOW)
	{
		ang_enc_pend -= INCREMENTO_PEND;
	}
	else
	{
		ang_enc_pend += INCREMENTO_PEND;
	}

	// Condicionamento dos limites //
	if(ang_enc_pend < -179.9)
    {
        ang_enc_pend += 360;
    }
    else if(ang_enc_pend > 180)
    {
        ang_enc_pend -= 360;
    }
}

/**
 * @brief Função de interrupção para o canal B do encoder do motor.
 * 		  Incrementa ou decrementa o valore de angulo INCREMENTO_MOT a cada pulso lido.
 */
void atualiza_enc_b_mot()
{
	// Atualização de ângulo //
	int leitura_a_mot = digitalRead(ENC_A_MOT);
	if(leitura_a_mot == LOW)
	{
		ang_enc_mot -= INCREMENTO_MOT;
	}
	else
	{
		ang_enc_mot += INCREMENTO_MOT;
	}

	// Condicionamento dos limites //
	if(ang_enc_mot < -179.9)
    {
        ang_enc_mot += 360;
    }
    else if(ang_enc_mot > 180)
    {
        ang_enc_mot -= 360;
    }
}

/**
 * @brief Função de interrupção para o canal A do encoder do pêndulo.
 * 		  Incrementa ou decrementa o valore de angulo INCREMENTO_PEND a cada pulso lido.
 */
void atualiza_enc_a_pend()
{
	// Atualização de ângulo //
	int leitura_b_pend = digitalRead(ENC_B_PEND);
	if(leitura_b_pend == LOW)
	{
		ang_enc_pend += INCREMENTO_PEND;
	}
	else
	{
		ang_enc_pend -= INCREMENTO_PEND;
	}

	// Condicionamento dos limites //
	if(ang_enc_pend < -179.9)
    {
        ang_enc_pend += 360;
    }
    else if(ang_enc_pend > 180)
    {
        ang_enc_pend -= 360;
    }
}

/**
 * @brief Função de interrupção para o canal A do encoder do motor.
 * 		  Incrementa ou decrementa o valore de angulo INCREMENTO_MOT a cada pulso lido.
 */
void atualiza_enc_a_mot()
{
	// Condicionamento dos limites //
	int leitura_b_mot = digitalRead(ENC_B_MOT);
	if(leitura_b_mot == LOW)
	{
		ang_enc_mot += INCREMENTO_MOT;
	}
	else
	{
		ang_enc_mot -= INCREMENTO_MOT;
	}

	// Condicionamento dos limites //
	if(ang_enc_mot < -179.9)
    {
        ang_enc_mot += 360;
    }
    else if(ang_enc_mot > 180)
    {
        ang_enc_mot -= 360;
    }
}

/**
 * @brief Retorna os valores amostrados lidos.
 */
void monitoramento_valores()
{
	Serial.print("Tempo: ");
	Serial.print(delta_tempo);
	Serial.print("s || Alpha: "); //U+03B1
	Serial.print(alpha);
  	Serial.print("° | ");
	Serial.print(alpha_ponto);
	Serial.print("°/s || Theta: "); //U+03B8
	Serial.print(theta);
  	Serial.print("° | ");
	Serial.print(alpha_ponto);
	Serial.println("°/s");
}

//////////////////////////////////////// FUNCOES DE PROCESSO ///////////////////////////////////////

/**
 * @brief Envio o sinal pwm e indica o sentido de rotação do motor.
 * 
 * @param sinal_de_controle double Sinal de tensão referente à saída do controlador.
 */
void movimenta_motor(double sinal_de_controle)
{
	// Saturação //
	if(sinal_de_controle > 12)
	{
		sinal_de_controle = 12;
	}

	else if (sinal_de_controle < -12)
	{
		sinal_de_controle = -12;
	}

	// Definição do sinal PWM //
	double pwm = map(fabs(sinal_de_controle), 0, 12, 0, 255);
	analogWrite(PWM_PONTE_H, pwm);

	// Definição do sentido de rotação//
	if(sinal_de_controle > 0) // Horário
	{
		digitalWrite(IN_1_MOT, HIGH);
  		digitalWrite(IN_2_MOT, LOW);
	}
	else if(sinal_de_controle < 0) // Anti-horário
	{
		digitalWrite(IN_1_MOT, LOW);
  		digitalWrite(IN_2_MOT, HIGH);
	}
}

/**
 * @brief Retorna a derivada (temporal) de uma dada variável.
 * 
 * @param valor_prev float Valor prévio da variável.
 * @param valor_atual float Valor atual da variável.
 * @param deltaT double Intervalo de tempo entre as medições de valor_prev e valor_atual.
 * @return double
 */
double calcula_derivada(float valor_prev, float valor_atual, double deltaT)
{
	// NOTE : função para facilitar a implementação do filtro
	return (valor_atual - valor_prev)/deltaT;
}

/**
 * @brief Calcula o sinal de controle para o acionamento do motor.
 * 
 * @param erro float[4] Erro entre medição das variáveis de estado e o setpoint.
 * @return double 
 */
double sinal_de_controle(float erro[4])
{
	// Conversão para radianos
	double graus_p_rad = PI/double(180);

	// Calculo do sinal de controle u
	double u = (K[0] * erro[0] * graus_p_rad + 
				K[1] * erro[1] * graus_p_rad +
				K[2] * erro[2] * graus_p_rad + 
				K[3] * erro[3] * graus_p_rad );

	return u;
}

// bool bancada_ok(){};

// void interrupcao_emergencia(){};

// void atualiza_lcd(){};

/////////////////////////////////////////////// SETUP //////////////////////////////////////////////

/**
 * Configurações iniciais.
*/
void setup()
{
	Serial.begin(9600); // Inicialização do monitor serial para debug

	// Definição dos pinos //
	pinMode(ENC_A_PEND, INPUT_PULLUP);
	pinMode(ENC_B_PEND, INPUT_PULLUP);
	pinMode(ENC_A_MOT, INPUT_PULLUP);
	pinMode(ENC_B_MOT, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(ENC_A_PEND), atualiza_enc_a_pend, RISING);
	attachInterrupt(digitalPinToInterrupt(ENC_B_PEND), atualiza_enc_b_pend, RISING);
	attachInterrupt(digitalPinToInterrupt(ENC_A_MOT), atualiza_enc_a_mot, RISING);
	attachInterrupt(digitalPinToInterrupt(ENC_B_MOT), atualiza_enc_b_mot, RISING);

	pinMode(IN_1_MOT, OUTPUT);
	pinMode(IN_2_MOT, OUTPUT);

	// pinMode(SW_PIN, INPUT);
	// pinMode(BOT_EMERG_PIN, INPUT_PULLUP);

	// pinMode(LED_1, OUTPUT);
	// pinMode(LED_2, OUTPUT);
	// pinMode(LED_3, OUTPUT);

	// Configurar LCD
	// lcd.init();
	// lcd.backlight();
	// lcd.clear();

	// Inicialização de variáveis //
	digitalWrite(IN_1_MOT, LOW); 
	digitalWrite(IN_2_MOT, LOW);
	ang_enc_pend = 0;
	ang_enc_mot = 0;
	tempo_inicial = millis();
}

////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

/**
 * @brief Definição do processo.
 */
void loop()
{
	// Amostragem das leituras
	delay(TS*1000);
	tempo_amostra = millis();
	delta_tempo = (tempo_amostra - tempo_inicial)/double(1000); // s

	alpha = ang_enc_pend; // °
	theta = ang_enc_mot; // °

	// Calcula derivadas
	alpha_ponto = calcula_derivada(alpha_inicial, alpha, delta_tempo); // °/s
	theta_ponto = calcula_derivada(theta_inicial, theta, delta_tempo); // °/s

	// Calcula o erro
	erro[0] = SETPOINT[0] - theta;
	erro[1] = SETPOINT[1] - theta_ponto;
	erro[2] = SETPOINT[2] - alpha;
	erro[3] = SETPOINT[3] - alpha_ponto;

	// Calcula sinal de controle
	double u = sinal_de_controle(erro); // V

	// Aciona motor
	movimenta_motor(u);

	// Monitoramento dos valores amostrados
	monitoramento_valores();

	// Atualização dos valores préviso
	tempo_inicial = tempo_amostra;
	alpha_inicial = alpha;
	theta_inicial = theta;
}
