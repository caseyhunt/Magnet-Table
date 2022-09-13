#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid     = "my_server";
const char* password = "abhi@123";

const char* host = "192.168.4.1";  //Server IP Address here

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         D3          // Configurable, see typical pin layout above
#define SS_PIN          D8        // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);
String rfid;

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

  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();  

 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
    WiFiClient client;
    HTTPClient Post;
    Post.begin(client, "http://192.168.4.1/");   // pass Server IP Address here
    Post.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Post.GET();
    delay(10);
    String payload = Post.getString();
    Serial.println(payload);
    Post.end();
  }

//  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
}

void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  printHex(mfrc522.uid.uidByte, mfrc522.uid.size);
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    WiFiClient client;
    HTTPClient Post;
    Post.begin(client, "http://192.168.4.1/body");   // pass Server IP Address here
    Post.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Post.POST("data=" + rfid);
    delay(10);
    String payload = Post.getString();
    Serial.println(payload);
    Post.end();
  } 
  delay(500);  
}

void printHex(byte *buffer, byte bufferSize) {
  rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
//    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//    Serial.print(buffer[i], HEX);
    rfid += buffer[i] < 0x10 ? " 0" : " ";
    rfid += String(buffer[i], HEX);
  }
  Serial.println();
  //Serial.print(rfid);
}
