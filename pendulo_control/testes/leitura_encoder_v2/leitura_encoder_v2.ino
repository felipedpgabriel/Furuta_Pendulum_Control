const double TS = 1/100; // segundos
const double INCREMENTO_PEND = double(360)/1200;

const int ENC_A_PEND = 0;
const int ENC_B_PEND = 0;

volatile int leitura_a_pend, leitura_b_pend;

double alpha;

double tempo_inicial, tempo_amostra;

double ajusta_angulo(double angulo)
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
	if(leitura_a_pend == LOW)
	{
		alpha -= INCREMENTO_PEND;
	}
	else
	{
		alpha += INCREMENTO_PEND;
	}

    alpha = ajusta_angulo(alpha);
}

void atualiza_enc_a_pend()
{
	if(leitura_b_pend == LOW)
	{
		alpha += INCREMENTO_PEND;
	}
	else
	{
		alpha -= INCREMENTO_PEND;
	}

    alpha = ajusta_angulo(alpha);
}

void setup()
{
    pinMode(ENC_A_PEND, INPUT_PULLUP);
	pinMode(ENC_B_PEND, INPUT_PULLUP);

    attachInterrupt(ENC_A_PEND, atualiza_enc_a_pend, RISING);
	attachInterrupt(ENC_B_PEND, atualiza_enc_b_pend, RISING);

    tempo_inicial = millis()
}

void loop()
{
	delay(TS*1000)
	tempo_amostra = millis()
	Serial.print("Tempo: ")
	Serial.println(tempo_amostra - tempo_inicial)
	Serial.print("Angulo pêndulo: ")
	Serial.println(alpha)
	tempo_inicial = tempo_amostra
}
