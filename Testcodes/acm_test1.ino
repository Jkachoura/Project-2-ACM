// Left Motor connections
#define lM1 1
#define lM2 2
// Right Motor connections
#define rM1 3
#define rM2 4

//Ir sensors
#define irLeft 9
#define irRight 10
//Ultrasonic Sensor
#define echoPin 11
#define trigPin 12

//States of IR sensor
int irLeftState;
int irRightState;
int distance;   // variable for the distance measurement
long duration;  // variable for the traveltime of sound waves

void setup() {
  //Set all the motor control pins to outputs
  pinMode(lM1, OUTPUT);
  pinMode(lM2, OUTPUT);
  pinMode(rM1, OUTPUT);
  pinMode(rM2, OUTPUT);
  //Set the IR sensors to an input
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  //Set the Ultrasonic Sensor pins as output or input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  //Putting the value what IR sensor gives into the state variable of the ir sensors
  irLeftState = !digitalRead(irLeft);
  irRightState = !digitalRead(irRight);
  
  //If statements for detection black line
  //LINE FOUND ON BOTH SENSORS
  if(irLeftState == 1 && irRightState == 1){
    drive('b');
    delay(1000);
    drive('r');
  }
  //LINE FOUND ON THE LEFT SENSOR
  else if(irLeftState == 1 && irRightState == 0){
    drive('r');
  }
  //LINE FOUND ON THE RIGHT SENSOR
  else if(irLeftState == 0 && irRightState == 1){
    drive('l');
  }
  //NO LINES FOUND
  else{
    drive('f');
  }
  
  checkUltrasoneSensors();
}
//Function to drive the motors to given direction
void drive(char mode){
  switch (mode){
    case 'f':
      digitalWrite(lM, HIGH);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, HIGH);
      digitalWrite(rM2, LOW);
      break;
    case 'b':
      digitalWrite(lM, LOW);
      digitalWrite(lM2, HIGH);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, HIGH);
      break;
    case 'l':
      digitalWrite(lM, LOW);
      digitalWrite(lM2, HIGH);
      digitalWrite(rM1, HIGH);
      digitalWrite(rM2, LOW);
      break;
    case 'r':
      digitalWrite(lM, HIGH);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, HIGH);
      break;
    case 'p':
      digitalWrite(lM, LOW);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, LOW);
      break;
  }
}

int frontUltrasone(){
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
  
  return distance;
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

void checkUltrasoneSensors(){
  if(frontUltrasone() <= 15){
    drive('l');
    delay(3000);
    drive('f');
  }
  else if(bottomUltrasone() > 5){
  drive('p');
  drive('b');
  delay(3400);
  driver('l');
  }
}
