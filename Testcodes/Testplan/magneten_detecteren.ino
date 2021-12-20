// Left Motor connections
#define lM1 1
#define lM2 2
// Right Motor connections
#define rM1 3
#define rM2 4
// Reed sensor
#define reed 5

int reedState; //state of reed sensor

void setup() {
  //Set all the motor control pins to outputs
  pinMode(lM1, OUTPUT);
  pinMode(lM2, OUTPUT);
  pinMode(rM1, OUTPUT);
  pinMode(rM2, OUTPUT);
  //Set Reed sensor to input
  pinMode(reed, INPUT);
}

void loop() {
  drive('f');
  while(checkReed()){
    drive('p');
    delay(1000);
    drive('f');
  }
  
}

//Function to drive the motors to given direction
void drive(char mode){
  switch (mode){
    case 'f':
      digitalWrite(lM1, HIGH);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, HIGH);
      digitalWrite(rM2, LOW);
      break;
    case 'b':
      digitalWrite(lM1, LOW);
      digitalWrite(lM2, HIGH);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, HIGH);
      break;
    case 'l':
      digitalWrite(lM1, LOW);
      digitalWrite(lM2, HIGH);
      digitalWrite(rM1, HIGH);
      digitalWrite(rM2, LOW);
      break;
    case 'r':
      digitalWrite(lM1, HIGH);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, HIGH);
      break;
    case 'p':
      digitalWrite(lM1, LOW);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, LOW);
      break;
  }
}

boolean checkReed(){
  //Putting value waht Reed reads into state variable
  reedState = digitalRead(reed);
  if(reedState == HIGH){
    return true;
  }
  else{
    return false;
  }
}
