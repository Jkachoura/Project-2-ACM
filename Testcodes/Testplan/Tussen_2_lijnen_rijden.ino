// Left Motor connections
#define lM1 1
#define lM2 2
// Right Motor connections
#define rM1 3
#define rM2 4
//Ir sensors
#define irLeft 9
#define irRight 10

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
  //Putting the value what IR sensor gives into the state variable of the ir sensors
  irLeftState = !digitalRead(irLeft);
  irRightState = !digitalRead(irRight);
  
  //If statements for detection black line
  //LINE FOUND ON BOTH SENSORS
  if(irLeftState == 1 && irRightState == 1){
    drive('reverse');
    delay(1000);
    drive('right');
  }
  //LINE FOUND ON THE LEFT SENSOR
  else if(irLeftState == 1 && irRightState == 0){
    drive('right');
  }
  //LINE FOUND ON THE RIGHT SENSOR
  else if(irLeftState == 0 && irRightState == 1){
    drive('left');
  }
  //NO LINES FOUND
  else{
    drive('forward');
  }
}
//Function to drive the motors to given direction
void drive(char mode){
  switch (mode){
    case 'forward':
      digitalWrite(lM1, HIGH);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, HIGH);
      digitalWrite(rM2, LOW);
      break;
    case 'reverse':
      digitalWrite(lM1, LOW);
      digitalWrite(lM2, HIGH);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, HIGH);
      break;
    case 'left':
      digitalWrite(lM1, LOW);
      digitalWrite(lM2, HIGH);
      digitalWrite(rM1, HIGH);
      digitalWrite(rM2, LOW);
      break;
    case 'right':
      digitalWrite(lM1, HIGH);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, HIGH);
      break;
    case 'park':
      digitalWrite(lM1, LOW);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, LOW);
      break;
  }
}
