
#include "Wifi_Module.h"
#include "API.h"
#include "display.h"
#include "MemoryCard.h"
#include "hardware.h"
#include "Layout.h"
#include "evManager.h"
#include "LayoutComponents.h"

void setup() {

  init_HAL();
  
  Serial.begin(115200);
  while (!Serial && millis() < 3000); 

 // bootScreen();

  /** INITIALIZE PERIPHERAL DEVICES ***/
  initDisplay();  //Install and configure the display
  setCurrentLayout(LAYOUT_BOOT_SCREEN);

  //Install the SD card driver
  initSDCard();
  
  if(sd_Status()<0){
    msgBox("SD CARD NOT FOUND!", TYPE_ERROR);
    halt();
  }

  //Loads the device config from a JSON file located on the SD card
  loadDeviceConfig();

  //Initialize the wifi module
  if(initWifi() != 2){
    msgBox("CONNECTION PROBLEM", TYPE_ERROR);
    halt();
  }

  setCurrentLayout(LAYOUT_INFO);
}

void loop() {

  static int progress = 0;
  
  //eventManager();
  //callLayoutController();

  delay(100);
}

