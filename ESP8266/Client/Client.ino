#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid     = "my_server";
const char* password = "abhi@123";

const char* host = "192.168.4.1";  //Server IP Address here

void setup() {
  Serial.begin(115200);
  delay(100);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    WiFiClient client;
    HTTPClient Post;
    Post.begin(client, "http://192.168.4.1/body");   // pass Server IP Address here
    Post.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Post.POST("data=hellothere");
    delay(10);
    String payload = Post.getString();
    Serial.println(payload);
    Post.end();
  } 
  delay(100);  
}
