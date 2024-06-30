/**
 * @file leitura_encoder.ino
 * @author Luara Linhares, Marcos 
 * @brief Teste inicial para contagem de pulsos do encoder do pêndulo.
 * @version 0.1
 * @date 2024-05-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

volatile double temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder

const int ENC_A_PEND = 23; // Canal A do encoder | Arduino nano usa 2
const int ENC_B_PEND = 22; // Canal B do encoder | Arduino nano usa 3 

void setup() {
  Serial.begin (9600);
  pinMode(ENC_A_PEND, INPUT_PULLUP); // internal pullup input pin 2 
  
  pinMode(ENC_B_PEND, INPUT_PULLUP); // internalเป็น pullup input pin 3
   //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(ENC_A_PEND), ai0, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(ENC_B_PEND), ai1, RISING);
  }
   
  void loop() {
  // Send the value of counter
  if( counter != temp ){
  Serial.println (counter);
  temp = counter;
  }
  }
   
  void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(ENC_B_PEND)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(ENC_A_PEND)==LOW) {
  counter--;
  }else{
  counter++;
  }
  }