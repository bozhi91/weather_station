
#include <Arduino.h>
#include "hardware.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_heap_caps.h"

void print_detailed_heap_info(void);
void print_memory_info(void);
void print_chip_info(void);


void printCpuInfo(void){

  /*Serial.printf("ESP IDF-Version: %d.%d.%d \n", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);
  Serial.printf("ESP-IDF-Version: %s \n",esp_get_idf_version());
  */
  Serial.printf(" -> Reset reason: %d \n", esp_reset_reason());

  if(esp_reset_reason() == ESP_RST_PANIC){

    Serial.println(" RESET DUE TO PANIC. SYSTEM HALTED!!!!");

    while(1);
  }

  /*
  print_detailed_heap_info();
  print_memory_info();
  print_chip_info();*/
}

void print_detailed_heap_info(void) {

  multi_heap_info_t heap_info;
  heap_caps_get_info(&heap_info, MALLOC_CAP_DEFAULT);
  Serial.printf("Total free bytes: %d\n", heap_info.total_free_bytes);
  Serial.printf("Largest free block: %d\n", heap_info.largest_free_block);
  Serial.printf("Minimum free bytes ever: %d\n", heap_info.minimum_free_bytes);
}

void print_memory_info(void) {

  Serial.printf("Free heap size: %d bytes\n", esp_get_free_heap_size());
  Serial.printf("Minimum free heap ever: %d bytes\n", esp_get_minimum_free_heap_size());
}


void print_chip_info(void) {

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
} 
