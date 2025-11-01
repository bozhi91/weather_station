
#include "Wifi_Module.h"
#include "API.h"
#include "display.h"
#include "MemoryCard.h"
#include "hardware.h"
#include "Layout.h"
#include "evManager.h"
#include "LayoutComponents.h"

void setup() {
  
  Serial.begin(115200);
  while (!Serial && millis() < 3000); 

 // bootScreen();

  /** INITIALIZE PERIPHERAL DEVICES ***/
  initDisplay();  //Install and configure the display
  setCurrentLayout(LAYOUT_BOOT_SCREEN);

  initSDCard();   //Install the SD card driver
  
  if(sd_Status()<0){
    msgBox("SD CARD NOT FOUND!", TYPE_ERROR);
    halt();
  }
  
  loadDeviceConfig(); //Loads the device config from a JSON file

  //Initialize the wifi module
  /*if(initWifi() != 2){
    msgBox("CONNECTION PROBLEM", TYPE_ERROR);
    halt();
  }*/

  setCurrentLayout(LAYOUT_INFO);    
}

void loop() {

  static int progress = 0;
  
  eventManager();
  callLayoutController();

  delay(200);

/*
  setCurrentLayout(LAYOUT_HOME);
  delay(500);
  
  callLayoutController();
  delay(3000);
  
  setCurrentLayout(LAYOUT_INFO);
  delay(500);*/

 /* progressBar(progress);
  delay(500);
  progress+=5;*/

  //progress = (progress<100) ? progress+5 : 0;
  //loadLayout(LAYOUT_HOME);  //Initializes the main Layout 

  //delay(3000);
  //loadLayout(LAYOUT_INFO);
}

