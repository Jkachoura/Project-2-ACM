// Left Motor connections
#define lM1 2
#define lM2 15
// Right Motor connections
#define rM1 12
#define rM2 13
// Reed sensor
#define reed 36

int reed_status;
boolean acmIsLeft = false;
boolean acmIsRight = false;

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
  checkIRs();
  reed_status = digitalRead(reed);
  if(reed_status == 1){
    drive('f');
    
  }
  else{
    drive('p');
  }
}

void checkIRs(){
  
  //Putting the value what IR sensor gives into the state variable of the ir sensors
  irLeftState = digitalRead(irLeft);
  irRightState = digitalRead(irRight);
  if(irLeftState == 1){
    drive('r');
    delay(200);
    acmIsRight = false;
    acmIsLeft = true;
  }
  if(irRightState == 1){
    drive('l');
    delay(200);
    acmIsRight = true;
    acmIsLeft = false;
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
