const double TS = 0.04; // segundos
const double INCREMENTO_PEND = double(360)/1200; // TODO : revisar para melhorar precisao

const int ENC_A_PEND = 23; //2
const int ENC_B_PEND = 22; //3

volatile double alpha;

double tempo_inicial, tempo_amostra;

double tempo;

void atualiza_enc_b_pend()
{
	if(digitalRead(ENC_A_PEND) == LOW)
	{
		alpha -= INCREMENTO_PEND;
	}
	else
	{
		alpha += INCREMENTO_PEND;
	}

    if(alpha < -179)
    {
        alpha += 360;
    }
    else if(alpha > 180)
    {
        alpha -= 360;
    }
}

void atualiza_enc_a_pend()
{
	if(digitalRead(ENC_B_PEND) == LOW)
	{
		alpha += INCREMENTO_PEND;
	}
	else
	{
		alpha -= INCREMENTO_PEND;
	}

    if(alpha < -179)
    {
        alpha += 360;
    }
    else if(alpha > 180)
    {
        alpha -= 360;
    }
}

void setup()
{
	Serial.begin (9600);
    pinMode(ENC_A_PEND, INPUT_PULLUP);
	pinMode(ENC_B_PEND, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENC_A_PEND), atualiza_enc_a_pend, RISING); //0
	attachInterrupt(digitalPinToInterrupt(ENC_B_PEND), atualiza_enc_b_pend, RISING); //1

    tempo_inicial = millis();
}

void loop()
{
	delay(TS*1000);
	tempo_amostra = millis();
	tempo = (tempo_amostra - tempo_inicial)/1000;
	Serial.print("Angulo pêndulo: ");
	Serial.print(alpha);
  	Serial.print(" | Tempo: ");
	Serial.println(tempo);
	tempo_inicial = tempo_amostra;
}
