#include "Adafruit_ST7796S_kbv.h"
#include <FS.h>
#include <SD.h>
#include "hardware.h"
#include "MemoryCard.h"
#include "display.h"

/**
  https://www.freepik.com/search?format=search&last_filter=type&last_value=photo&query=Weather+Set&type=photo
  https://www.iconfinder.com/icons/2682849/cloud_cloudy_day_forecast_sun_weather_icon?coming-from=related-results
  
  https://www.flaticon.com/free-icons/weather
*/

  const  char* mem_card_type[] = {  
    "CARD_NONE",
    "CARD_MMC",
    "CARD_SD",
    "CARD_SDHC",
    "CARD_UNKNOWN"
  };

static void sd_listDir(fs::FS &fs, const char *dirname, uint8_t levels);

char initSDCard(void){

  Serial.println(" - Initializing SD card...");

  if (!SD.begin(SD_CS)) {
    Serial.println(" FAILED ");
    return -1;
  }

  int8_t cardType = SD.cardType();
  Serial.println(" - SD Card driiver initialized!");

  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached!");
    return -2;
  }

  Serial.printf("\n === SD Card Info=== \n\n");
  Serial.printf("- SD Card Type: %s\n", mem_card_type[cardType]);
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("- SD Card Size: %llu MB \n", cardSize);
  Serial.printf("- SD Card Used Space: %llu MB \n", SD.usedBytes()/(1024*1024));
  Serial.printf("- SD Card Free Space: %llu MB \n\n", cardSize - (SD.usedBytes()/(1024*1024)));

  //sd_listDir(SD, "/", 0);
  /*loadPNG("/cloud.png", 20, 50);
  loadPNG("/rain.png", 90, 50);
  loadPNG("/part_cloud_2.png", 160, 50);
  loadPNG("/night.png", 230, 50);
  loadPNG("/rain.png", 300, 50);*/

  return 0;
}

/*
  Reads a file as binary from the SD card and store it to a RAM buffer

  @Params: 
  - path: absolute path to the file. Example: /file.txt, /folder_1/folder_2/file
  - fileBuffer: Pointer to the destination buffer we want to store the file to.
  - fSize: Size of the file in bytes.
*/
int fread(const char *path, unsigned char* fileBuffer, unsigned long* fSize){
  
  fs::FS &fs = SD;

  File file = fs.open(path, "rb");

  if (!file) {
    Serial.printf("Failed to open file : %s \n", path);
    return -1;
  }

  //Read the file and load it to RAM
  unsigned long fileSize = file.size();
  *fSize = fileSize;
  file.read(fileBuffer, fileSize);
  file.close();

  Serial.printf("File loaded: %s(%d)bytes \n", path, fSize);  

  return 0;
}


static void sd_listDir(fs::FS &fs, const char *dirname, uint8_t levels) {

  Serial.printf(" \n Listing directory: %s \n\n", dirname);

  File root = fs.open(dirname);

  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();

  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        sd_listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
