// Motor Front Left connections
#define fL1 1
#define fL2 2
// Motor Front Right connections
#define fR1 3
#define fR2 4
// Motor Back Left connections
#define bL1 5
#define bL2 6
// Motor Back Right connections
#define bR1 7
#define bR2 8
//Ir sensors
#define irLeft 9
#define irRight 10
//Ultrasonic Sensor
#define echoPin 11
#define trigPin 12

int distance;     // variable for the distance measurement
long duration;    // variable for the traveltime of sound waves

void setup() {
  pinMode(fL1, OUTPUT);
  pinMode(fL2, OUTPUT);
  pinMode(fR1, OUTPUT);
  pinMode(fR2, OUTPUT);
  pinMode(bL1, OUTPUT);
  pinMode(bL2, OUTPUT);
  pinMode(bR1, OUTPUT);
  pinMode(bR2, OUTPUT);
  //Set the IR sensors to an input
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  //Set the Ultrasonic Sensor pins as output or input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  drive('forward');
  checkSensor();
}

//Function to drive the motors to given direction
void drive(char mode){
  switch (mode){
    case 'forward':
      digitalWrite(fL1, HIGH);
      digitalWrite(fL2, LOW);
      digitalWrite(fR1, HIGH);
      digitalWrite(fR2, LOW);
      digitalWrite(bL1, HIGH);
      digitalWrite(bL2, LOW);
      digitalWrite(bR1, HIGH);
      digitalWrite(bR2, LOW);
      break;
    case 'reverse':
      digitalWrite(fL1, LOW);
      digitalWrite(fL2, HIGH);
      digitalWrite(fR1, LOW);
      digitalWrite(fL1, HIGH);
      digitalWrite(fR2, HIGH);
      digitalWrite(bL1, LOW);
      digitalWrite(bL2, HIGH);
      digitalWrite(bR1, LOW);
      digitalWrite(bR2, HIGH);
      break;
    case 'left':
      digitalWrite(fL1, LOW);
      digitalWrite(fL2, HIGH);
      digitalWrite(fR1, HIGH);
      digitalWrite(fR2, LOW);
      digitalWrite(bL1, LOW);
      digitalWrite(bL2, HIGH);
      digitalWrite(bR1, HIGH);
      digitalWrite(bR2, LOW);
      break;
    case 'right':
      digitalWrite(fL1, HIGH);
      digitalWrite(fL2, LOW);
      digitalWrite(fR1, LOW);
      digitalWrite(fR2, HIGH);
      digitalWrite(bL1, HIGH);
      digitalWrite(bL2, LOW);
      digitalWrite(bR1, LOW);
      digitalWrite(bR2, HIGH);
      break;
    case 'park':
      digitalWrite(fL1, LOW);
      digitalWrite(fL2, LOW);
      digitalWrite(fR1, LOW);
      digitalWrite(fR2, LOW);
      digitalWrite(bL1, LOW);
      digitalWrite(bL2, LOW);
      digitalWrite(bR1, LOW);
      digitalWrite(bR2, LOW);
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

void checkSensor(){
    if(frontUltrasone() <= 15){
    drive('left');
    delay(3000);
    drive('forward');
  }
}
