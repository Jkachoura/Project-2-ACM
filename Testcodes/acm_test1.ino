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
  
  checkUltrasoneSensors();
}
//Function to drive the motors to given direction
void drive(char mode){
  switch (mode){
    case 'forward':
      digitalWrite(lM, HIGH);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, HIGH);
      digitalWrite(rM2, LOW);
      break;
    case 'reverse':
      digitalWrite(lM, LOW);
      digitalWrite(lM2, HIGH);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, HIGH);
      break;
    case 'left':
      digitalWrite(lM, LOW);
      digitalWrite(lM2, HIGH);
      digitalWrite(rM1, HIGH);
      digitalWrite(rM2, LOW);
      break;
    case 'right':
      digitalWrite(lM, HIGH);
      digitalWrite(lM2, LOW);
      digitalWrite(rM1, LOW);
      digitalWrite(rM2, HIGH);
      break;
    case 'park':
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
  int sensorHeight = ?;
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
  
  distanceGround = sensorHeight - distance;
  
  return distanceGround;
}

void checkUltrasoneSensors(){
  if(frontUltrasone() <= 15){
    drive('left');
    delay(3000);
    drive('forward');
  }
  else if(bottomUltrasone() > 5){
  drive('park');
  drive('reverse');
  delay(3000);
  driver('left');
  }
}
