// Left Motor connections
#define lM1 12
#define lM2 13
// Right Motor connections
#define rM1 2
#define rM2 15
//Ir sensors
#define irLeft 18
#define irRight 5

//States of IR sensor
int irLeftState;
int irRightState;
//Variable to see which IR last scanned the line
boolean acmIsLeft = false;
boolean acmIsRight = false;

void setup() {
  //Set all the motor control pins to outputs
  pinMode(lM1, OUTPUT);
  pinMode(lM2, OUTPUT);
  pinMode(rM1, OUTPUT);
  pinMode(rM2, OUTPUT);
  //Set the IR sensors to an input
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
}

void loop() {
  drive('f');
  checkIRs();
}

void checkIRs(){
  
  //Putting the value what IR sensor gives into the state variable of the ir sensors
  irLeftState = digitalRead(irLeft);
  irRightState = digitalRead(irRight);

  if (irLeftState == 1){
    if(acmIsRight){
      drive('b');
      delay(400);
      drive('l');
      delay(500);
      acmIsRight = false;
      acmIsLeft = true;
    }
    else{
      drive('b');
      delay(400);
      drive('r');
      delay(500);
      acmIsRight = false;
      acmIsLeft = true;
    }
  }
  if(irRightState == 1){
    if(acmIsLeft){
      drive('b');
      delay(400);
      drive('r');
      delay(500);
      acmIsRight = true;
      acmIsLeft = false;
    }
    else{
      drive('b');
      delay(400);
      drive('l');
      delay(500);
      acmIsRight = true;
      acmIsLeft = false;
    }
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
