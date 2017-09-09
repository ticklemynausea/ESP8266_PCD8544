#include <ESP8266_PCD8544.h>

#include "bitmaps.h"

ESP8266_PDC8544 lcd = ESP8266_PDC8544(2, 12, 14, 4, 5);

void setup() {

    Serial.begin(115200);
    Serial.println("hi");

    lcd.initialize();

}

void loop() {

  /* test line drawing */
  lcd.clear();
  lcd.line(0, 0, LCD_WIDTH-1, 0);
  lcd.line(0, LCD_HEIGHT-1, LCD_WIDTH-1, LCD_HEIGHT-1);
  lcd.line(0, 0, 0, LCD_HEIGHT-1);
  lcd.line(LCD_WIDTH-1, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
  lcd.line(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
  lcd.line(0, LCD_HEIGHT-1, LCD_WIDTH-1, 0);
  lcd.update();
  delay(1000);


  /* test circle drawing */
  lcd.clear();
  lcd.circle(30, 30, 10);
  lcd.circle(60, 30, 10);
  lcd.circle(30, 60, 10);
  lcd.circle(60, 60, 10);
  lcd.update();
  delay(1000);

  /* test text screen wrap */
  lcd.clear();
  lcd.text(0, 0, "AS ARMAS E OS BAROES ASSINALADOS QUE DA OCIDENTAL PRAIA LUSITANA VIERAM CITAR FRASES DE LIVROS QUE NUNCA LERAM pois sim que cena fixe LOREM IPSUM !\"\\#$%&/()ABCDEFGHIJKLMNOPQRSTUVWXYXabcdefghijklmnopqrstuvwxyz1234567890+-benfica campeao 37 topkek lelele memmememe sadjhadsuifghpsodfhgos");
  lcd.update();
  delay(1000);

  /* show bitcoin bitmap */
  lcd.bitmap(bitcoin);
  lcd.update();
  delay(1000);

  /* show adafruit bitmap */
  lcd.bitmap(adafruit);
  lcd.update();
  delay(1000);

}
