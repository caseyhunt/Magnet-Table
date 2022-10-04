#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// Replace with your network credentials
const char* ssid = "my_server";  
const char* password = "abhi@123";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

int motorpos;
int uppos;
int downpos;

bool started;
bool ended;

Servo servo;

char inByte;

void handleBody(){
  
  if (server.hasArg("data")== false){ //Check if body received
    String message = "Body not received";
    server.send(200, "text/plain", message);
    Serial.println(message);
    //return;
  }else{
    String message = "Body received:\n";
    message += server.arg("data");
    message += "\n";
    server.send(200, "text/plain", message);
    Serial.println(message);
  }
}

void handleRoot(){
  String message = "Welcome at root";
  server.send(200, "text/plain", message);
  Serial.println(message);
}

void handleServo(){
  String message = "<" + String(motorpos) + ">";
  server.send(200, "text/plain", message);
  Serial.println(message);
}

void setup(void){
   servo.attach(D4);
   servo.write(0);
   delay(10);
   servo.write(180);
  delay(1000);
  Serial.begin(115200);
  WiFi.softAP(ssid, password); //begin WiFi access point
  Serial.println("");

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP()); 
   
  server.on("/body", handleBody);

  server.on("/", handleRoot);

  server.on("/servo", handleServo);
  
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  while(Serial.available() > 0){
    inByte = Serial.read();
    if(inByte == '<'){
      started = true;
      ended = false;
    }
    else if(inByte == '>'){
      ended = true;
      break;
    }
    else{
      motorpos = inByte;
    }
  }

  if(started && ended){
    
    started = false;
    ended = false;
  }
  
  server.handleClient();
  
}
