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
const float TS = 0.04; // Período de amostragem [s] | Arduino nano usa 0.08
const float INCREMENTO_PEND = float(360)/1200; // Incremento angular por pulso do encoder

// Pinagens //
const int ENC_A_PEND = 23; // Canal A do encoder | Arduino nano usa 2
const int ENC_B_PEND = 22; // Canal B do encoder | Arduino nano usa 3 

///////////////////////////////////////// VARIAVEIS GLOBAIS ////////////////////////////////////////

// Leitura do encoder //
volatile float ang_enc_pend;

// Variáveis para amostragem //
double alpha, alpha_ponto, tempo_amostra;

// Intervalo de tempo na amostragem//
double delta_tempo; // milissegundos

// Variáveis para valores anteriores //
double tempo_inicial, alpha_inicial;

//////////////////////////////////////// FUNCOES AUXILIARES ////////////////////////////////////////

/**
 * @brief Função de interrupção para o canal B do encoder do pêndulo.
 * 		  Incrementa ou decrementa o valore de angulo INCREMENTO_PEND a cada pulso lido.
 */
void atualiza_enc_b_pend()
{
	// Atualização de ângulo //
	if(digitalRead(ENC_A_PEND) == LOW)
	{
		ang_enc_pend -= INCREMENTO_PEND;
	}
	else
	{
		ang_enc_pend += INCREMENTO_PEND;
	}

	// Condicionamento dos limites //
    if(ang_enc_pend < -179)
    {
        ang_enc_pend += 360;
    }
    else if(ang_enc_pend > 180)
    {
        ang_enc_pend -= 360;
    }
}

/**
 * @brief Função de interrupção para o canal A do encoder do pêndulo.
 * 		  Incrementa ou decrementa o valore de angulo INCREMENTO_PEND a cada pulso lido.
 */
void atualiza_enc_a_pend()
{
	// Atualização de ângulo //
	if(digitalRead(ENC_B_PEND) == LOW)
	{
		ang_enc_pend += INCREMENTO_PEND;
	}
	else
	{
		ang_enc_pend -= INCREMENTO_PEND;
	}

	// Condicionamento dos limites //
	if(ang_enc_pend < -179)
    {
        ang_enc_pend += 360;
    }
    else if(ang_enc_pend > 180)
    {
        ang_enc_pend -= 360;
    }
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
}

////////////////////////////////////////////// LOOP ////////////////////////////////////////////////

/**
 * @brief Definição do processo.
 * 
 */
void loop()
{
	// Amostragem das leituras
	delay(TS*1000); 
	tempo_amostra = millis();
	delta_tempo = (tempo_amostra - tempo_inicial)/1000; // segundos
	alpha = ang_enc_pend; // °
	alpha_ponto = (alpha - alpha_inicial)/delta_tempo; // °/s

	// Monitoramento dos valores
	Serial.print("Angulo pêndulo: ");
	Serial.print(alpha);
  	Serial.print("° | Tempo: ");
	Serial.print(delta_tempo);
	Serial.print("s | Velocidade: ");
	Serial.print(alpha_ponto);
	Serial.println("°/s");

	// Atualização das variáveis 
	tempo_inicial = tempo_amostra;
	alpha_inicial = alpha;
}
