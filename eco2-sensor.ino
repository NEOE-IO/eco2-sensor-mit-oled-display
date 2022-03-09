/**********************************************************************************************************************************
  Arduino IoT Cloud Sketch für den "eCO2-Sensor mit OLED-Display".
  Tutorial: https://www.neoe.io/1004
  Datum der letzten Änderung: 9. März, 2022
**********************************************************************************************************************************/

#include "thingProperties.h"
#include "SGP30.h"
#include <U8g2lib.h>

SGP30 SGP;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Werte alle 10 Sekunden aktualisieren
int delay_time = 10000;

void setup() {

  Serial.begin(9600);
  delay(1500);

  // In thingProperties.h definiert
  initProperties();

  // Mit Arduino IoT Cloud verbinden
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  SGP.begin();
  u8g2.begin();

}

void loop() {
  ArduinoCloud.update();

  SGP.measure(false);      // gibt "false" zurück, falls keine Messung erfolgt

  eCO2 = SGP.getCO2();
  TVOC = SGP.getTVOC();

  // Daten an Display ausgeben
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvR10_tf);
  u8g2.drawButtonUTF8(64, 17, U8G2_BTN_HCENTER | U8G2_BTN_BW0, 0,  0,  0, "eCO2 (ppm)" );
  u8g2.setFont(u8g2_font_helvR24_tf);
  u8g2.drawButtonUTF8(64, 58, U8G2_BTN_HCENTER | U8G2_BTN_BW0, 0,  0,  0, String(SGP.getCO2()).c_str() );
  u8g2.sendBuffer();

  delay(delay_time);

}

