
// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include "configuration.h"

//set wifi settings from configuration
const char ssid[] = WIFI_SSID;
const char password[] = WIFI_PASSWORD;

const int awesomeButtonPin = 1;
const int okButtonPin = 2;
const int badButtonPin = 3;

int awesomeState = 0;
int okState = 0;
int badState = 0;

int awesomeCounter = 0;
int okCounter =0;
int badCounter = 0;

// Web Server on port 80
WiFiServer server(80);

// only runs once on boot
void setup() {
  pinMode(awesomeButtonPin, INPUT);
  pinMode(okButtonPin, INPUT);
  pinMode(badButtonPin, INPUT);
  
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
            // your actual web page that displays temperature and humidity
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>ESP buttons</h1>");
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
    awesomeState = digitalRead(awesomeButtonPin);
    okState = digitalRead(okButtonPin);
    badState = digitalRead(badButtonPin);
    if (awesomeState = HIGH){
      awesomeCounter += 1;
      Serial.println("Awesome");
      delay(100);
    }
    if (okState = HIGH){
      okCounter += 1;
      Serial.println("Ok");
      delay(100);
    }
    if (badState = HIGH){
      badCounter += 1;
      Serial.println("Bad");
      delay(100);
    }
  }
}   
