#include <ESP8266HTTPClient.h>  
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#define Rx 5
#define Tx 4
#define WIFI_SSID "iPhone XS"
#define WIFI_PASSWORD "88889999"

SoftwareSerial mySerial(Rx ,Tx); 

String URL = "http://172.20.10.6:3000/employee/getData";
String Link;
void connectWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  connectWifi(); 
}

void loop() {
  if(mySerial.available()){
      String Data = mySerial.readString();
      Data.remove(10,1);
      Link = URL + String(Data);     
      Serial.println(Data);
      Serial.println(Data.length());
//      Serial.println(Link);
//      Serial.println(Link.length());
      GetHttp();
  }
}

void GetHttp(){
  if (WiFi.status() == WL_CONNECTED) {
  HTTPClient http;
  http.begin(Link);
  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload); 
    if (httpCode >0 && httpCode ==200)  // check the returning code
    {
       Serial.println("Luu thanh cong!");
    }
    http.end(); 
    } else {
       Serial.println("Error in WiFi connection");
    }
}
