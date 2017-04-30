#include <ESP8266_PCD8544.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define P(s) Serial.print(s)

#include "secrets.h"

ESP8266_PDC8544 lcd = ESP8266_PDC8544(2, 12, 14, 4, 5);
WiFiClient client;

void setup() {

    Serial.begin(115200);
    Serial.println("hi");

    lcd.initialize();

    WiFi.begin(WLAN_SSID, WLAN_PASS);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.text(0, 0, "WIFI CONNECTED");
    lcd.update();
    delay(1000);

}

void loop() {

  Serial.print("Connecting to "); Serial.print(IRC_SERVER); Serial.print(":"); Serial.println(IRC_PORT);

  if (!client.connect(IRC_SERVER, IRC_PORT)) {
    Serial.println("connection failed");
    return;
  }

  client.print(String("NICK ") + IRC_NICKNAME + String("\r\n"));
  client.print(String("USER ") + IRC_USERNAME + String( " 8 * :") + IRC_REALNAME + String("\r\n"));

  while (client.connected()) {

    while (client.available() > 0) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
      lcd.text(&line);
      lcd.update();
    }

    while (Serial.available() > 0) {
      char chr = Serial.read();
      client.write((uint8_t)chr);
    }

  }

  Serial.println("Connection terminated, retrying in 10s...");
  delay(10000);
}
