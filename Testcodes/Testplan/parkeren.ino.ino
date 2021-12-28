#include <WiFi.h>
// Left Motor connections
#define lM1 12
#define lM2 13
// Right Motor connections
#define rM1 2
#define rM2 15
// Replace with your network credentials
const char* ssid     = "Tesla IoT";
const char* password = "fsL6HgjN";

WiFiServer server(80);  // Set web server port number to 80

String header;  // Variable to store the HTTP request

// Auxiliar variables to store the current output state
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
  //Set all the motor control pins to outputs
  pinMode(lM1, OUTPUT);
  pinMode(lM2, OUTPUT);
  pinMode(rM1, OUTPUT);
  pinMode(rM2, OUTPUT);
  
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
  
  // ======== Manual mode 
  if (manual) {
    if (forward){
      drive ('f');
    }
    else if (backward) {
      drive ('b');
    }
    else if (turnRight) {
      drive ('r');
    }
    else if (turnLeft) {
      drive ('l');
    }
    else {
      drive('p');
    }
  }

  // ======== Autonomous mode
  else if (autonomous){
    drive('p');
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
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            


            // Toggles the booleans values
            // Button for autonomous/manual mode
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

            // Button for forward
            else if (header.indexOf("GET /forward/on") >= 0) {
              forward= false;
            } 
            else if (header.indexOf("GET /forward/off") >= 0) {
              forward = true;
              backward = false;
              turnLeft = false;
              turnRight = false;
            }

            // Button for backward
            else if (header.indexOf("GET /backward/on") >= 0) {
              backward = false;
            } 
            else if (header.indexOf("GET /backward/off") >= 0) {
              backward = true;
              forward = false;
              turnLeft = false;
              turnRight = false;
            }

            // Button for turning right
            else if (header.indexOf("GET /turn_right/on") >= 0) {
              turnRight = false;
            } 
            else if (header.indexOf("GET /turn_right/off") >= 0) {
              turnRight = true;
              backward = false;
              forward = false;
              turnLeft = false;
            }

            // Button for turning left
            else if (header.indexOf("GET /turn_left/on") >= 0) {
              turnLeft = false;
            } 
            else if (header.indexOf("GET /turn_left/off") >= 0) {
              turnLeft = true;
              forward = false;
              turnRight = false;
              backward = false;
            }
           
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Roboto; display: inline-block; margin: 0px auto; text-align: center; -webkit-user-select: none;}");
            client.println(".button { background-color: #E3BF00; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #CECBBB;}</style></head>");
            
            // Web Page Heading
            client.println("<body style=\"background-color: #00ACFF;\"><h1>Groep 10</h1>");

            // Display current state for the mode
            client.println("<p>Current control mode </p>");     
            if (autonomous) {
              client.println("<p><a href=\"/mode/manual\"><button class=\"button\">Autonomous</button></a></p>");
            } else {
              client.println("<p><a href=\"/mode/autonomous\"><button class=\"button\">Manual</button></a></p>");
            } 

            // Only if manual mode is on, show manual buttons
            if (manual) {   
              client.println ("<p> Manual control buttons </p>");

              // Forward, backward, right left hold buttons
              client.println("<p><button class=\"button\" onmousedown=\"toggle('forward/off')\" ontouchstart=\"toggle('forward/off')\" onmouseup=\"toggle('forward/on')\" ontouchend=\"toggle('forward/on')\">Forward</button></p>");
              client.println("<p><button class=\"button\" onmousedown=\"toggle('backward/off')\" ontouchstart=\"toggle('backward/off')\" onmouseup=\"toggle('backward/on')\" ontouchend=\"toggle('backward/on')\">Backward</button></p>");
              client.println("<p><button class=\"button\" onmousedown=\"toggle('turn_right/off')\" ontouchstart=\"toggle('turn_right/off')\" onmouseup=\"toggle('turn_right/on')\" ontouchend=\"toggle('turn_right/on')\">Turn right</button></p>");
              client.println("<p><button class=\"button\" onmousedown=\"toggle('turn_left/off')\" ontouchstart=\"toggle('turn_left/off')\" onmouseup=\"toggle('turn_left/on')\" ontouchend=\"toggle('turn_left/on')\">Turn left</button></p>");
            }
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
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
