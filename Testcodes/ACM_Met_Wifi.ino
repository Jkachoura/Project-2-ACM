#include <WiFi.h>
// Left Motor connections
#define lM1 12
#define lM2 13
// Right Motor connections
#define rM1 2
#define rM2 15
//Ir sensors
#define irLeft 18
#define irRight 5
// reed sensor
#define reed 2
//Ultrasonic Sensor
#define echoPin 17
#define trigPin 16
//Ultrasonic Sensor
#define echoPin2 4
#define trigPin2 0

//States of IR sensor
int irLeftState;
int irRightState;
//Fron Ultrasone Sensor
int distance;
long duration;
//Bottom Ultrasone Sensor
int distance2;
long duration2;

// Replace with your network credentials
const char* ssid     = "Tesla IoT";
const char* password = "fsL6HgjN";

//Set web server port to 80 HTTP
WiFiServer server(80);

//Variable to store the HTTP request
String header;

//Boolean variables to store the current output state
bool manual = true;
bool autonomous = false;

bool forward = false;
bool backward = false;
bool turnRight = false;
bool turnLeft = false;

unsigned long currentTime = millis();   // Current time
unsigned long previousTime = 0;     // Previous time
const long timeoutTime = 3000;  // Define timeout time in milliseconds (example: 2000ms = 2s)

void setup() {
  Serial.begin(115200);
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
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  wifiLoop ();
  
  //Manual mode 
  if (manual) {
    if (forward){
      drive('f');
    }
    else if (backward) {
      drive('b');
    }
    else if (turnRight) {
      drive('r');
    }
    else if (turnLeft) {
      drive('l');
    }
    else {
      drive('p');
    }
  }

  //Autonomous mode
  else if (autonomous){
      //Putting the value what IR sensor gives into the state variable of the ir sensors
      irLeftState = digitalRead(irLeft);
      irRightState = digitalRead(irRight);
    
      drive('f');
      
      checkSensor();
      
      while(checkReed){
        drive('p');
      }
       
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
        delay(1000);
        drive('f');
      }
      //LINE FOUND ON THE RIGHT SENSOR
      else if(irLeftState == 0 && irRightState == 1){
        drive('l');
        delay(1000);
        drive('f');
      }
      drive('f');
      }
}

void wifiLoop () {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          //if the current line is blank, you got two newline characters in a row.
          //that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            //HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            //and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //Toggles the booleans values
            //Button for autonomous/manual mode
            if (header.indexOf("GET /mode/manual") >= 0) {
              manual = true;
              autonomous = false;
            } 
            else if (header.indexOf("GET /mode/autonomous") >= 0) {
              autonomous = true;
              manual = false;
              // Turn other buttons off
              forward = false;
              backward = false;
              turnLeft = false;
              turnRight = false;
            }

            //Button for forward
            else if (header.indexOf("GET /forward/on") >= 0) {
              forward = true;
              backward = false;
              turnLeft = false;
              turnRight = false;
            }

            //Button for backward 
            else if (header.indexOf("GET /backward/on") >= 0) {
              backward = true;
              forward = false;
              turnLeft = false;
              turnRight = false;
            }

            //Button for turning right
            else if (header.indexOf("GET /turn_right/on") >= 0) {
              turnRight = true;
              backward = false;
              forward = false;
              turnLeft = false;
            }

            //Button for turning left
            else if (header.indexOf("GET /turn_left/on") >= 0) {
              turnLeft = true;
              forward = false;
              turnRight = false;
              backward = false;
            }

            //Button for stopping
            else if (header.indexOf("GET /stop/on") >= 0) {
              turnLeft = false;
              forward = false;
              turnRight = false;
              backward = false;
            }
           
            //Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            //CSS to style the buttons 
            client.println("<style>html { font-family: Roboto; display: inline-block; margin: 0px auto; text-align: center; -webkit-user-select: none;}");
            client.println(".button { background-color: #E3BF00; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #CECBBB;}</style></head>");
            
            //Web Page Heading
            client.println("<body style=\"background-color: #00ACFF;\"><h1>Groep 10</h1>");

            //Display current state for the mode
            client.println("<p>Current control mode </p>");     
            if (autonomous) {
              client.println("<p><a href=\"/mode/manual\"><button class=\"button\">Autonomous</button></a></p>");
            } else {
              client.println("<p><a href=\"/mode/autonomous\"><button class=\"button\">Manual</button></a></p>");
            } 

            //Only show direction buttons when manual mode is on
            if (manual) {   
              client.println ("<p> Manual control buttons </p>");

              //Buttons to toggle forward, backward, left, right or stop
              client.println("<p><a href=\"/forward/on\"><button class=\"button\">Forward</button></a></p>");
              client.println("<p><a href=\"/backward/on\"><button class=\"button\">Backward</button></a></p>");
              client.println("<p><a href=\"/turn_left/on\"><button class=\"button\">Turn Left</button></a></p>");
              client.println("<p><a href=\"/turn_right/on\"><button class=\"button\">Turn Right</button></a></p>");
              client.println("<p><a href=\"/stop/on\"><button class=\"button\">Stop</button></a></p>");
            }
            client.println("</body></html>");
            //The HTTP response ends with another blank line
            client.println();
            //Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    //Clear the header variable
    header = "";
    //Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
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

int frontUltrasone(){
  //Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  //Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  //Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  return distance;
}

int bottomUltrasone(){
  int sensorHeight = 4;
  int distanceGround;
  
  //Clears the trigPin condition
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  //Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
  //Calculating the distance
  distance2 = duration2 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  distanceGround = distance2 - sensorHeight;
  
  return distanceGround;
}

void checkSensor(){
    if(frontUltrasone() <= 15){
    drive('l');
    delay(1000);
    drive('f');
  }
    if(bottomUltrasone() > 5){
    drive('p');
    drive('r');
    delay(3400);
    drive('f');
  }
}

//function to check the reed sensor
int checkReed() {                               
  if(digitalRead(reed) == HIGH){
     return 1;
  }else{
    return 0;
  }
}
