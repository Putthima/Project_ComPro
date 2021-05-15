void Line_Notify(String message) ;

#include <ESP8266WiFi.h>
#include <WiFiClientSecureAxTLS.h> // กรณีขึ้น Error ให้เอาบรรทัดนี้ออก

// Config connect WiFi
#define WIFI_SSID "TUTOR 2G"
#define WIFI_PASSWORD "kai5689!"

// Line config
#define LINE_TOKEN "mV6kaQtirRA5l4aN74mqFmvfyV3LUti4rZ1pEgom1AG"

#define SW D5
int buttonPin = D1;
int statusPush = 0;
int statusActive = 0;
int buzzer = D2;
int buttonState = 0;
int I = 0;

String message = "ตอนนี้ฉันอยู่ในสถานการณ์อันตรายช่วยติดต่อฉันกลับด้วย ที่เบอร์: 0649041117"; // ArduinoIDE เวอร์ชั่นใหม่ ๆ ใส่ภาษาไทยลงไปได้เลย

void setup() {
  pinMode(SW, INPUT);
  pinMode(buttonPin, INPUT);

  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  digitalWrite(buzzer, HIGH);
  WiFi.mode(WIFI_STA);
  // connect to wifi.
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

void loop() {
  if (digitalRead(SW) == HIGH) {
    while(digitalRead(SW) == HIGH) delay(10);

    Serial.println("Enter !");
    Line_Notify(message);
    // Serial.println();
  }
  delay(10);

  
  buttonState = digitalRead(buttonPin);
if (I != buttonState ) {
I = buttonState;
if (buttonState == HIGH) {
 if (statusPush == 0) {
      statusPush = 1;

      Serial.println("-------------- Push ----------------");

      if (statusActive == 0) {
        statusActive = 1;
      }
      else if (statusActive == 1) {
        statusActive = 0;
      }
    }
}
else if (buttonState == LOW) {
    statusPush = 0;
  }
  
if (statusActive == 0) {
    digitalWrite(buzzer, LOW);
  }
  else if (statusActive == 1) {
    digitalWrite(buzzer, HIGH);
  }



}



}

void Line_Notify(String message) {
  axTLS::WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}
