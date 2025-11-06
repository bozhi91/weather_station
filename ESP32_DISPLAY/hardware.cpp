#include "HardwareSerial.h"

#include <Arduino.h>
#include "hardware.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_heap_caps.h"

void print_memory_info(void);
void print_chip_info(void);
   
const char* RESET_TABLE[] = {

    "Reset reason can not be determined",
    "Reset due to power-on event",
    "Reset by external pin (not applicable for ESP32)",
    "Software reset via esp_restart",
    "Software reset due to exception/panic",
    "Reset (software or hardware) due to interrupt watchdog",
    "Reset due to task watchdog",
    "Reset due to other watchdogs",
    "Reset after exiting deep sleep mode",
    "Brownout reset (software or hardware)",
    "Reset over SDIO",
    "Reset by USB peripheral",
    "Reset by JTAG",
    "Reset due to efuse error",
    "Reset due to power glitch detected",
    "Reset due to CPU lock up (double exception)"
  };

void bootScreen(void){

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

static SPI_Dev_List spi_devs[] = {
  { DISPLAY, SD_CS  },
  { SD_CARD, TFT_CS }
};


/*Called after a critical system failure.
For a sequrity reasons, the cpu will be halted
*/
void halt(void){

  Serial.println(" >>> SYSTEM HALTED <<<");
  while(1){
    delay(50);
  }
}

void toggleSPI_Device(int dev_id){

  int size = sizeof(SPI_Dev_List)/sizeof(spi_devs[0]);

  //Disable all the SPI devices except the one we want to communicate with.
  //By setting the CS pin to HIGH, we disable the device. CS_LOW will enable it.
  for(int i=0; i<size; i++){
     digitalWrite(spi_devs[i].CS, spi_devs[i].device_id == dev_id ? 0:1);
  }
}

void init_HAL(void){

  pinMode(TFT_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);

  digitalWrite(TFT_CS, HIGH);
  digitalWrite(SD_CS, HIGH);
}



void printCpuInfo(void){

  Serial.printf(" - Last Reset reason: [ %s ] \n\n", RESET_TABLE[esp_reset_reason()]);

  //The last reset was due to a kernel panik. Halt the CPU in order to prevent an endless loops
  if(esp_reset_reason() == ESP_RST_PANIC){
    halt();
  }

  print_memory_info();
  Serial.println("----------------------------------------------------\n");
}

void print_memory_info(void) {

  /****
    PARTITION TABLE:
    - 3MB SKETCH
    - 1MB SPIFFS, NO OTA
  **/

  multi_heap_info_t heap_info;
  heap_caps_get_info(&heap_info, MALLOC_CAP_DEFAULT);

  Serial.println(" >>>>> Memory info <<<<< \n");
  Serial.printf("- Total free bytes: %d\n", heap_info.total_free_bytes);
  Serial.printf("- Free heap size: %d bytes\n", esp_get_free_heap_size());

  Serial.printf("- Total Sketch partition size:  %d bytes \n", ESP.getFreeSketchSpace());
  Serial.printf("- Total Flash: %d bytes \n", ESP.getFlashChipSize());
  Serial.printf("- Sketch size: %d bytes \n", ESP.getSketchSize());

  Serial.printf("- Chip Model:    %s \n", ESP.getChipModel());
  Serial.printf("- Core version:  %s \n", ESP.getCoreVersion());
  Serial.printf("- Chip cores:    %d \n", ESP.getChipCores());

  Serial.printf("- Flash Speed:  %d Mhz \n", ESP.getFlashChipSpeed()/1000000);
  Serial.printf("- CPU Freq:  %d Mhz \n", ESP.getCpuFreqMHz());

 // ESP.getFreeSketchSpace())
}


/****
#include <Arduino.h>
#include "SPIFFS.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== ESP32 System Info ===");

  // Chip and SDK info
  Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
  Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("SDK Version: %s\n", ESP.getSdkVersion());

  // Flash memory info
  Serial.printf("Flash Chip Size: %u bytes (%.2f MB)\n", ESP.getFlashChipSize(), ESP.getFlashChipSize() / 1024.0 / 1024.0);
  Serial.printf("Flash Speed: %u Hz\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash Mode: %u\n", ESP.getFlashChipMode());
  Serial.printf("Free Sketch Space: %u bytes\n", ESP.getFreeSketchSpace());

  // Heap / RAM info
  Serial.printf("Free Heap: %u bytes\n", ESP.getFreeHeap());
  Serial.printf("Min Free Heap Since Boot: %u bytes\n", ESP.getMinFreeHeap());
  Serial.printf("Max Allocatable Heap Block: %u bytes\n", ESP.getMaxAllocHeap());

  // Sketch info
  Serial.printf("Sketch Size: %u bytes\n", ESP.getSketchSize());

  // ESP32-specific MAC addresses
  Serial.printf("Chip ID (MAC-based): 0x%llX\n", ESP.getEfuseMac());
  Serial.printf("MAC Address: %s\n", WiFi.macAddress().c_str());

  // SPIFFS info
  if (SPIFFS.begin(true)) {
    size_t total = SPIFFS.totalBytes();
    size_t used = SPIFFS.usedBytes();
    Serial.printf("SPIFFS Total: %u bytes\n", total);
    Serial.printf("SPIFFS Used: %u bytes\n", used);
    Serial.printf("SPIFFS Free: %u bytes\n", total - used);
  } else {
    Serial.println("SPIFFS mount failed!");
  }

  Serial.println("==========================");
}

void loop() {
  // Nothing to do here
}


*/

  /*esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
    
  Serial.printf("Chip model: ESP32\n");
  Serial.printf("Cores: %d\n", chip_info.cores);

  Serial.printf("Features: %s %s\n",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi" : "");

  Serial.printf("Revision: %d\n", chip_info.revision);
  Serial.printf("Flash size: %dMB\n", spi_flash_get_chip_size() / (1024 * 1024));
*/
