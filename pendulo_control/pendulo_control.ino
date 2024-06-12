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
const double K_THETA = -0.0686;
const double K_THETA_PONTO = -0.3748;
const double K_ALPHA = 14.2718;
const double K_ALPHA_PONTO = 2.2349;
const double K[4] = {K_THETA, K_THETA_PONTO, K_ALPHA, K_ALPHA_PONTO};
const double TS = 0.1; // segundos

const int SETP_T = 0;
const int SETP_TP = 0;
const int SETP_A = 0;
const int SETP_AP = 0;
const int SETPOINT[4] = {SETP_T, SETP_TP, SETP_A, SETP_AP};

//// Resolução dos encoders ////
const double INCREMENTO_PEND = double(360)/1200;
// TODO : Descobrir P/R
const double INCREMENTO_MOT = double(360)/315;

//// Pinos ////
const int ENC_A_PEND = 22; // 2
const int ENC_B_PEND = 23; // 3
const int ENC_A_MOT = 32; // 4
const int ENC_B_MOT = 33; // 5

// TODO : definir pinos para ponte h
const int IN_1_MOT = 19; // Sentido horário 
const int IN_2_MOT = 21; // Sentido anti-horário 
// const int PWM_PONTE_H = 14; // 

// const int SW_PIN = 0;
// const int BOT_EMERG_PIN = 0;

// const int LED_1 = 0;
// const int LED_2 = 0;
// const int LED_3 = 0;

///////////////////////////////////////// VARIAVEIS GLOBAIS ////////////////////////////////////////

// lcd //
// LiquidCrystal_I2C lcd(0x27, 16, 2); // TODO : Confirmar se o endereco eh 0x27. 

// Leitura dos encoders //
volatile double ang_enc_pend = 0, ang_enc_mot = 0;

// Variáveis para amostragem //
double theta, theta_ponto, alpha, alpha_ponto;
unsigned long tempo_amostra;

// Variáveis para valores anteriores //
double alpha_inicial, theta_inicial;
unsigned long tempo_inicial;

// Erro das variáveis de estado //
double erro[4];

// Intervalo de tempo na amostragem//
double delta_tempo; // milissegundos

// Variáveis para o filtro de média móvel //
const int Qtd_Amostras = 10;
double alpha_filtrado, theta_filtrado;

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
}

double condicionamento_angulo(double angulo)
{
	// FIXME: testar outra abordagem
	// if(fabs(angulo) > 360)
	// {
	// 	angulo = angulo % 360;
	// }

	if(angulo < -179.9)
    {
        angulo += 360;
    }
    else if(angulo > 180)
    {
        angulo -= 360;
    }

	return angulo;
}

double filtroMediaMovel(double ang_filtro) {
    static int Leituras_anteriores[Qtd_Amostras];
    static int Posicao = 0;
    static long Soma = 0;
    static float Media = 0;
    static bool zera_vetor = 1;

    if (zera_vetor) {
        for (int i = 0; i < Qtd_Amostras; i++) {
            Leituras_anteriores[i] = 0;
        }
        zera_vetor = 0;
    } 
    Soma = ang_filtro - Leituras_anteriores[Posicao % Qtd_Amostras] + Soma;
    Leituras_anteriores[Posicao % Qtd_Amostras] = ang_filtro;
    Media = (float)Soma / (float)(Qtd_Amostras);
    Posicao = (Posicao + 1) % Qtd_Amostras;
    return ((double)Media);
}


/**
 * @brief Retorna os valores amostrados lidos.
 */
void monitoramento_valores(double pwm)
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
	Serial.print("°/s | ");
	Serial.print(pwm);
	Serial.println(" V");
	Serial.print("Alpha Filtrado: ");
	Serial.println('alpha_filtrado');
	Serial.print("Theta Filtrado: ");
	Serial.println('theta_filtrado');
}



//////////////////////////////////////// FUNCOES DE PROCESSO ///////////////////////////////////////

// void teste_motor()
// {
// 	analogWrite(PWM_PONTE_H, 10);
// 	digitalWrite(IN_1_MOT, HIGH);
// 	digitalWrite(IN_2_MOT, LOW);
// }

/**
 * @brief Envio o sinal pwm e indica o sentido de rotação do motor.
 * 
 * @param sinal_de_controle double Sinal de tensão referente à saída do controlador. 
 * @return double pwm e sinal (sentido de rotacao).
 */
double movimenta_motor(double sinal_de_controle)
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
	// analogWrite(PWM_PONTE_H, pwm);

	// Definição do sentido de rotação//
	if(sinal_de_controle > 0) // Horário
	{
		analogWrite(IN_1_MOT, pwm);
  		analogWrite(IN_2_MOT, LOW);
		return pwm;
	}
	else if(sinal_de_controle < 0) // Anti-horário
	{
		analogWrite(IN_1_MOT, LOW);
  		analogWrite(IN_2_MOT, pwm);
		return -pwm;
	}
}

/**
 * @brief Retorna a derivada (temporal) de uma dada variável.
 * 
 * @param valor_prev double Valor prévio da variável.
 * @param valor_atual double Valor atual da variável.
 * @param deltaT double Intervalo de tempo entre as medições de valor_prev e valor_atual.
 * @return double
 */
double calcula_derivada(double valor_prev, double valor_atual, double deltaT)
{
	// NOTE : função para facilitar a implementação do filtro
	return (valor_atual - valor_prev)/deltaT;
}

/**
 * @brief Calcula o sinal de controle para o acionamento do motor.
 * 
 * @param erro double[4] Erro entre medição das variáveis de estado e o setpoint.
 * @return double 
 */
double sinal_de_controle(double erro[4])
{
	// Calculo do sinal de controle u
	double u = (K[0] * erro[0] + 
				K[1] * erro[1] +
				K[2] * erro[2] + 
				K[3] * erro[3]);

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
	// pinMode(PWM_PONTE_H, OUTPUT);

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
	// analogWrite(PWM_PONTE_H, 0);
	digitalWrite(IN_1_MOT, LOW); 
	digitalWrite(IN_2_MOT, LOW);
	alpha_inicial = ang_enc_pend;
	theta_inicial = ang_enc_mot;
	tempo_inicial = millis();
}

////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

/**
 * @brief Definição do processo.
 */
void loop()
{
	// Amostragem das leituras
	tempo_amostra = millis();
	delta_tempo = (tempo_amostra - tempo_inicial)/double(1000); // s

	if(delta_tempo >= TS)
	{
		alpha = ang_enc_pend; // °
		theta = ang_enc_mot; // °
		alpha_filtrado = filtroMediaMovel(alpha);
		theta_filtrado = filtroMediaMovel(theta);

		// Calcula derivadas
		alpha_ponto = calcula_derivada(alpha_inicial, alpha, delta_tempo); // °/s
		theta_ponto = calcula_derivada(theta_inicial, theta, delta_tempo); // °/s
		alpha_inicial = alpha;
		theta_inicial = theta;
		alpha = condicionamento_angulo(alpha);
		theta = condicionamento_angulo(theta);

		// Calcula o erro
		double graus_p_rad = PI/double(180);
		erro[0] = SETPOINT[0] - (theta * graus_p_rad);
		erro[1] = SETPOINT[1] - (theta_ponto * graus_p_rad);
		erro[2] = SETPOINT[2] - (alpha * graus_p_rad);
		erro[3] = SETPOINT[3] - (alpha_ponto * graus_p_rad);

		// Calcula sinal de controle
		double u = sinal_de_controle(erro); // V

		// Aciona motor
		double pwm = movimenta_motor(u);

		// teste_motor();

		// Monitoramento dos valores amostrados
		monitoramento_valores(u);

		// Atualização dos valores préviso
		tempo_inicial = tempo_amostra;
	}
}


