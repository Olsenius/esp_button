
// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include "configuration.h"

//set wifi settings from configuration
const char ssid[] = WIFI_SSID;
const char password[] = WIFI_PASSWORD;

const int awesomePin = D5;
const int okPin = D6;
const int badPin = D7;

int awesomeState, okState, badState = 0;
int lastAwesomeState, lastOkState, lastBadState = 0;
int awesomeCounter, okCounter, badCounter = 0;

// Web Server on port 80
WiFiServer server(80);

// only runs once on boot
void setup() {
  pinMode(awesomePin, INPUT);
  pinMode(okPin, INPUT);
  pinMode(badPin, INPUT);
  
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

// runs over and over again
void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>ESP buttons</h1>");
            client.print("Awesome - ");
            client.println(awesomeCounter);
            client.print("Ok - ");
            client.println(okCounter);
            client.print("Bad - ");
            client.println(badCounter);
            client.println("</body></html>");     
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  } else {
    awesomeState = digitalRead(awesomePin);
    if (awesomeState != lastAwesomeState && awesomeState == HIGH) {
      Serial.println("Awesome");
      awesomeCounter++; 
    }
    lastAwesomeState = awesomeState;    
    
    okState = digitalRead(okPin);
    if (okState != lastOkState && okState == HIGH) {
      Serial.println("OK");
      okCounter++; 
    }
    lastOkState = okState;    

    badState = digitalRead(badPin);
    if (badState != lastBadState && badState == HIGH) {
      Serial.println("Bad");
      badCounter++; 
    }
    lastBadState = badState;    
    
    delay(50);
  }
}   
