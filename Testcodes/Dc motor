//========== Test Code to see if a motor is working or not
#define in1 1
#define in2 2

void setup() {
  //Set all the motor control pins to outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

void loop() {
  forward();
  delay(1000);
  backward();
  delay(1000);
  stopping();
  delay(1000);
}
//Function to drive forward
void forward(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}
//Function to drive backwards
void backward(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}
//Function to stop the car
void stopping(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
