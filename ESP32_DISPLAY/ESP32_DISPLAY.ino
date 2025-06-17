
#include "Wifi_Module.h"
#include "API.h"
#include "display.h"
#include "MemoryCard.h"
#include "hardware.h"
#include "Layout.h"

static void bootScreen(void);

static void bootScreen(void){

  Serial.printf("     ██████████  █████████  ███████████      ████████   ████████  \n");
  Serial.printf("    ░░███░░░░░█ ███░░░░░███░░███░░░░░███    ███░░░░███ ███░░░░███ \n");
  Serial.printf("     ░███  █ ░ ░███    ░░░  ░███    ░███   ░░░    ░███░░░    ░███ \n");
  Serial.printf("     ░██████   ░░█████████  ░██████████       ██████░    ███████  \n");
  Serial.printf("     ░███░░█    ░░░░░░░░███ ░███░░░░░░       ░░░░░░███  ███░░░░   \n");
  Serial.printf("     ░███ ░   █ ███    ░███ ░███            ███   ░███ ███      █ \n");
  Serial.printf("     ██████████░░█████████  █████          ░░████████ ░██████████ \n");
  Serial.printf("    ░░░░░░░░░░  ░░░░░░░░░  ░░░░░            ░░░░░░░░  ░░░░░░░░░  \n");
    

  Serial.printf(" \n\n\n ========== ESP32 WEATHER STATION ========== \n\n");
  Serial.println("Copyright: Bozhidar, 05/2025. Ver: 1.0");
  Serial.printf("Compilation: %s, %s \n\n",__DATE__, __TIME__);
  Serial.printf("-------------------------------------------------------------------\n");

  printCpuInfo();
  Serial.println(">>> Booting..... \n");
  
  // Sleep for 10 seconds (10,000,000 microseconds)
  //esp_sleep_enable_timer_wakeup(10 * 1000000);
  //esp_deep_sleep_start();
}

void setup() {
  
  Serial.begin(115200);
  while (!Serial && millis() < 3000); 

  bootScreen();

  /** INITIALIZE PERIPHERAL DEVICES ***/
  initDisplay();  //Install and configure the display
  initSDCard();   //Install the SD card driver
  
  if(sd_Status()<0){
    msgBox("SD CARD NOT FOUND!", TYPE_ERROR);
   // halt();
  }
 
  while(1){

    shell();

    //Serial.println("TASK...");
    delay(250);
  }

  loadConfig();   //Loads the device config from a JSON file
  initWifi();     //Initialize the wifi module
  loadLayout(0);  //Initializes the main Layout 
}

void loop() {

  Serial.println("Refreshing data....");
  //loadLayout(1);

  delay(30000);
}

