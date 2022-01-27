// Left Motor connections
#define lM1 2
#define lM2 15
// Right Motor connections
#define rM1 12
#define rM2 13
//Ultrasonic Sensor
#define echoPin 4
#define trigPin 0
//Variable to see which IR last scanned the line
boolean acmIsLeft = false;
boolean acmIsRight = false;

int irLeftState;
int irRightState;

int distance;     // variable for the distance measurement
long duration;    // variable for the traveltime of sound waves

void setup() {
  //Set all the motor control pins to outputs
  pinMode(lM1, OUTPUT);
  pinMode(lM2, OUTPUT);
  pinMode(rM1, OUTPUT);
  pinMode(rM2, OUTPUT);
  //ir sensor
  pinMode(irRight, INPUT);
  pinMode(irLeft, INPUT):
  //Set the Ultrasonic Sensor pins as output or input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  drive('f');
  checkIRs();
  checkSensor();
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

int bottomUltrasone(){
  int sensorHeight = 4;
  int distanceGround;
  
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  distanceGround = distance - sensorHeight;
  
  return distanceGround;
}

void checkSensor(){
  if(bottomUltrasone() > 5){
    drive('p');
    drive('r');
    delay(3400);
    drive('f');
  }
}
