volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder

const double INCREMENTO_PEND = double(360)/1200;
    
void setup() {
  Serial.begin (9600);
  pinMode(2, INPUT_PULLUP);
  
  pinMode(3, INPUT_PULLUP);
  
  //A rising
  attachInterrupt(0, ai0, RISING);
   
  //B rising 
  attachInterrupt(1, ai1, RISING);
  }
   
  void loop() {
  // Send the value of counter
  if( counter != temp ){
  Serial.println (counter);
  temp = counter;
  }
  }
   
  void ai0() { // A
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(3)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() { // B
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(2)==LOW) {
  counter--;
  }else{
  counter++;
  }
  }
