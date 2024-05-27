/**
 * @file leitura_encoder_v2.ino
 * @author Felipe Gabriel
 * @brief Teste com leitura de angulo e velocidade do encoder. 
 * @version 0.1
 * @date 2024-05-25
 * 
 * @copyright Copyright (c) 2024
 * 
 * Microcontrolador: ESP32
 * Encoder: OVW6-06-2HC
 */

//////////////////////////////////////////// CONSTANTES ////////////////////////////////////////////

// Constantes gerais //
const double TS = 0.04; // Período de amostragem [s] | Arduino nano usa 0.08
const double INCREMENTO_PEND = double(360)/315; // Incremento angular por pulso do encoder

// Pinagens //
const int ENC_A_PEND = 23; // Canal A do encoder | Arduino nano usa 2
const int ENC_B_PEND = 22; // Canal B do encoder | Arduino nano usa 3 

///////////////////////////////////////// VARIAVEIS GLOBAIS ////////////////////////////////////////

// Leitura do encoder //
volatile double ang_enc_pend = 180;

// Variáveis para amostragem //
double alpha, alpha_ponto;
unsigned long tempo_amostra;

// Intervalo de tempo na amostragem//
double delta_tempo; // milissegundos

// Variáveis para valores anteriores //
double alpha_inicial, alpha_aux_vel_inicial;
unsigned long tempo_inicial;

//////////////////////////////////////// FUNCOES AUXILIARES ////////////////////////////////////////

/**
 * @brief Função de interrupção para o canal B do encoder do pêndulo.
 * 		  Incrementa ou decrementa o valore de angulo INCREMENTO_PEND a cada pulso lido.
 */
void atualiza_enc_b_pend()
{
	// Condicionamento dos limites //
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

double condicionamento_angulo(double angulo)
{
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

void monitoramento_valores()
{
	Serial.print("Tempo: ");
	Serial.print(delta_tempo);
	Serial.print("s || Alpha: "); //U+03B1
	Serial.print(alpha);
  	Serial.print("° | ");
	Serial.print(alpha_ponto);
	Serial.println("°/s");
}

/////////////////////////////////////////////// SETUP //////////////////////////////////////////////

/**
 * Configurações iniciais.
*/
void setup()
{
	Serial.begin (9600); // Inicialização do monitor serial

	// Definição dos pinos //
    pinMode(ENC_A_PEND, INPUT_PULLUP);
	pinMode(ENC_B_PEND, INPUT_PULLUP);

	// Definição das interrupção //
	// Arduino nano remover digitalPinToInterrupt() e usar 0 e 1, respectivamente
    attachInterrupt(digitalPinToInterrupt(ENC_A_PEND), atualiza_enc_a_pend, RISING); //0
	attachInterrupt(digitalPinToInterrupt(ENC_B_PEND), atualiza_enc_b_pend, RISING); //1

	// Inicialização de variáveis "anteriores" //
    tempo_inicial = millis(); // milissegundos
	alpha_inicial = 0; // graus
	alpha_aux_vel_inicial = 0;
}

////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

/**
 * @brief Definição do processo.
 * 
 */
void loop()
{
	// Amostragem das leituras
	tempo_amostra = millis();
	delta_tempo = tempo_amostra - tempo_inicial;
	if(delta_tempo >= TS*1000)
	{
		delta_tempo = delta_tempo/double(1000); // segundos
		alpha = ang_enc_pend; // °
		alpha_ponto = (alpha - alpha_aux_vel_inicial)/delta_tempo; // °/s
		alpha_aux_vel_inicial = alpha;
		alpha = condicionamento_angulo(alpha);

		// Monitoramento dos valores
		monitoramento_valores();

		// Atualização das variáveis 
		tempo_inicial = tempo_amostra;
		alpha_inicial = alpha;
	}
}
