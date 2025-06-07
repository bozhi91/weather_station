
#include "Wifi_Module.h"
#include "API.h"
#include "display.h"
#include "MemoryCard.h"
#include "hardware.h"

void setup() {
  
  Serial.begin(9600);
  while (!Serial && millis() < 3000); 

  Serial.println();
  Serial.printf(" \n\n ========== ESP32 WEATHER STATION ========== \n");
 
  printCpuInfo();

  initWifi();
  initDisplay();
  initSDCard();
  displayLayout();

  //drawColorBitmap(sun, 100, 200, 24, 24);
  //drawColorBitmap(antena, 150, 200, 24, 24);
 //drawColorBitmap(antena, 150, 200, 42, 51);

  //tft.setScrollMargins(0,300);
  //tft.scrollTo(100);
}

void loop() {
  //displayLayout();
  //readAPI();
  delay(100);
}
