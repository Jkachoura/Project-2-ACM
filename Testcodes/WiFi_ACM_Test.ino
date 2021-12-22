#include <WiFi.h>
 // Left Motor connections
#define lM1 1
#define lM2 2
// Right Motor connections
#define rM1 3
#define rM2 4
// Reed sensor
#define reed 5
//Ir sensors
#define irLeft 6
#define irRight 7
//Ultrasonic Sensor
#define echoPin 8
#define trigPin 9
#define echoPin2 10
#define trigPin2 11

//States of IR sensor
int irLeftState;
int irRightState;
int reedState; //state of reed sensor
int distance; // variable for the distance measurement
long duration; // variable for the traveltime of sound waves
int distance2;
long duration2;

int serverState = 0;

const char * ssid = "yourssid";
const char * password = "yourpasswd";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  //Set all the motor control pins to outputs
  pinMode(lM1, OUTPUT);
  pinMode(lM2, OUTPUT);
  pinMode(rM1, OUTPUT);
  pinMode(rM2, OUTPUT);
  //Set Reed sensor to input
  pinMode(reed, INPUT);
  //Set the IR sensors to an input
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  //Set the Ultrasonic Sensor pins as output or input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  while (serverState == 0) {
    WiFiClient client = server.available(); // listen for incoming clients

    if (client) { // if you get a client,
      Serial.println("New Client."); // print a message out the serial port
      String currentLine = ""; // make a String to hold incoming data from the client
      while (client.connected()) { // loop while the client's connected
        if (client.available()) { // if there's bytes to read from the client,
          char c = client.read(); // read a byte, then
          Serial.write(c); // print it out the serial monitor
          if (c == '\n') { // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              // the content of the HTTP response follows the header:
              client.print("Click <a href=\"/F\">here</a> to drive forward.<br>");
              client.print("Click <a href=\"/B\">here</a> to drive backward.<br>");
              client.print("Click <a href=\"/L\">here</a> to turn left.<br>");
              client.print("Click <a href=\"/R\">here</a> to turn right.<br>");
              client.print("Click <a href=\"/S\">here</a> to turn stop.<br>");
              client.print("Click <a href=\"/A\">here</a> to turn atonomous.<br>");

              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            } else { // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') { // if you got anything else but a carriage return character,
            currentLine += c; // add it to the end of the currentLine
          }
          // Check to see if the client request was "GET /F", "GET /B", "GET /L", "GET /R", "GET /S" or "GET /A"
          if (currentLine.endsWith("GET /F")) {
            drive('f');
          }
          if (currentLine.endsWith("GET /B")) {
            drive('b');
          }
          if (currentLine.endsWith("GET /L")) {
            drive('l');
            delay(2500);
            drive('f');
          }
          if (currentLine.endsWith("GET /R")) {
            drive('r');
            delay(2500);
            drive('f');
          }
          if (currentLine.endsWith("GET /S")) {
            drive('p');
          }
          if (currentLine.endsWith("GET /A")) {
            serverState = 1;
          }
        }
        while (serverState == 1) {

          //Putting the value what IR sensor gives into the state variable of the ir sensors
          irLeftState = !digitalRead(irLeft);
          irRightState = !digitalRead(irRight);
          //Putting value waht Reed reads into state variable
          reedState = digitalRead(reed);
          //If statements for detection black line
          //LINE FOUND ON BOTH SENSORS
          if (irLeftState == 1 && irRightState == 1) {
            drive('b');
            delay(1000);
            drive('r');
          }
          //LINE FOUND ON THE LEFT SENSOR
          else if (irLeftState == 1 && irRightState == 0) {
            drive('r');
          }
          //LINE FOUND ON THE RIGHT SENSOR
          else if (irLeftState == 0 && irRightState == 1) {
            drive('l');
          }
          //NO LINES FOUND
          else {
            drive('f');
          }
          if (reedState == 1) {
            drive('p');
            delay(1000);
            drive('f');
          }
          checkUltrasoneSensors();
          if (currentLine.endsWith("GET /F")) {
            serverState = 0;
          }
          if (currentLine.endsWith("GET /B")) {
            serverState = 0;
          }
          if (currentLine.endsWith("GET /L")) {
            serverState = 0;
          }
          if (currentLine.endsWith("GET /R")) {
            serverState = 0;
          }
          if (currentLine.endsWith("GET /S")) {
            serverState = 0;
          }
          if (currentLine.endsWith("GET /A")) {
            serverState = 0;
          }
        }
      }
      // close the connection:
      client.stop();
      Serial.println("Client Disconnected.");
    }
  }
}

//Function to drive the motors to given direction
void drive(char mode) {
  switch (mode) {
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

int frontUltrasone() {
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

int bottomUltrasone() {
  int sensorHeight = 4;
  int distanceGround;

  // Clears the trigPin condition
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
  // Calculating the distance
  distance2 = duration2 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  distanceGround = distance2 - sensorHeight;

  return distanceGround;
}

void checkUltrasoneSensors() {
  if (frontUltrasone() <= 15) {
    drive('l');
    delay(3000);
    drive('f');
  } else if (bottomUltrasone() > 5) {
    drive('p');
    drive('b');
    delay(3400);
    drive('l');
  }
}
