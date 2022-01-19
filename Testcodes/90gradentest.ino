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
  checkIRs();
}

void checkIRs(){
  boolean acmIsLeft = false;
  boolean acmIsRight = false;
  boolean acmIsCentered = false;
  
  //Putting the value what IR sensor gives into the state variable of the ir sensors
  irLeftState = digitalRead(irLeft);
  irRightState = digitalRead(irRight);
 
  //LINE FOUND ON BOTH SENSORS AND ACM WAS ON LEFT SIDE
  if((irLeftState == 1 && irRightState == 1) && acmIsLeft == true){
    acmIsLeft = false;
    acmIsRight = false;
    acmIsCentered = false;
    drive('b');
    delay(200);
    drive('l');
    delay(1750);
  }
  //LINE FOUND ON BOTH SENSORS AND ACM WAS ON RIGHT SIDE
  else if((irLeftState == 1 && irRightState == 1) && acmIsRight == true){
    acmIsLeft = false;
    acmIsRight = false;
    acmIsCentered = false;
    drive('b');
    delay(200);
    drive('r');
    delay(1750);
  }
  //LINE FOUND ON BOTH SENSORS AND ACM WAS CENTERED (SEES DOUBLE LINE FOR THE SECOND TIME)
  else if((irLeftState == 1 && irRightState == 1) && acmIsCentered == true){
    acmIsLeft = false;
    acmIsRight = false;
    acmIsCentered = false;
    drive('b');
    delay(200);
    drive('l');
    delay(3500);
  }
  //LINE FOUND ON BOTH SENSORS AND ACM WAS ON CENTERED (SEES DOUBLE LINE FOR THE FIRST TIME)
  else if(irLeftState == 1 && irRightState == 1){
    acmIsLeft = false;
    acmIsRight = false;
    acmIsCentered = true;
    drive('b');
    delay(200);
    drive('l');
    delay(1750);
  }
  //LINE FOUND ON THE LEFT SENSOR
  else if(irLeftState == 1 && irRightState == 0){
    acmIsLeft = true;
    acmIsRight = false;
    acmIsCentered = false;
    drive('r');
    delay(200);
  }
  //LINE FOUND ON THE RIGHT SENSOR
  else if(irLeftState == 0 && irRightState == 1){
    acmIsLeft = false;
    acmIsRight = true;
    acmIsCentered = false;
    drive('l');
    delay(200);
  }
  //NO LINES FOUND
  else{
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
