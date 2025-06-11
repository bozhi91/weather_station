
#include "Wifi_Module.h"
#include "API.h"
#include "display.h"
#include "MemoryCard.h"
#include "hardware.h"
#include "Layout.h"

void setup() {
  
  Serial.begin(9600);
  //while (!Serial && millis() < 3000); 

  Serial.printf(" \n\n ========== ESP32 WEATHER STATION ========== \n");
  Serial.println("Copyright: Bozhidar, 05/2025. Ver: 1.0");
  
  printCpuInfo();

  initSDCard();   //Install the SD card driver
  loadConfig();   //Loads the device config from a JSON file
  initDisplay();  //Install and configure the display

  if(!sd_Status()){
    msgBox("SD CARD NOT FOUND!", TYPE_ERROR);
    return;
  }
 
  initWifi();    //Initialize the wifi module
  loadLayout(0); //Initializes the main Layout 
}

void loop() {

  Serial.println("Refreshing data....");
  loadLayout(1);

  delay(30000);
}

