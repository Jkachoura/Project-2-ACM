#include <WiFi.h>

// Left Motor connections
#define lM1 12
#define lM2 13
// Right Motor connections
#define rM1 2
#define rM2 15

const char* ssid     = "Tesla IoT";
const char* password = "fsL6HgjN";

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(5, OUTPUT);      // set the LED pin mode

    delay(10);

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

    drive('p');
}
int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/F\">here</a> to go forward.<br>");
            client.print("Click <a href=\"/B\">here</a> to go backward.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn left.<br>");
            client.print("Click <a href=\"/R\">here</a> to turn right.<br>");
            client.print("Click <a href=\"/S\">here</a> to stop.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /F")) {
          drive('f');
        }
        if (currentLine.endsWith("GET /B")) {
          drive('b');
        }
        if (currentLine.endsWith("GET /L")) {
          drive('l');
        }
        if (currentLine.endsWith("GET /R")) {
          drive('r');
        }
        if (currentLine.endsWith("GET /S")) {
          drive('p');
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
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
