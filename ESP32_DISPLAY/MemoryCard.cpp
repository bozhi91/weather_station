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
  static int sdCardStatus = 0;

  const  char* mem_card_type[] = {  
    "CARD_NONE",
    "CARD_MMC",
    "CARD_SD",
    "CARD_SDHC",
    "CARD_UNKNOWN"
  };

static void sd_listDir(fs::FS &fs, const char *dirname, uint8_t levels);
void enableSD(bool state);

char initSDCard(void){

  Serial.print(" - Initializing SD card... \t\t");
  sdCardStatus = 0;

  /** Initialize the SD card driver. 
      Assign the SD card with the SPI bus. Use the default parameteres:
      - Frequency: 4MHz
      - Mount point: /SD
   **/
  if (!SD.begin(SD_CS)) {
    Serial.println(" [ FAILED ] \n");
    sdCardStatus = -1;
    return sdCardStatus;
  }

  /** Check the card type */
  int8_t cardType = SD.cardType();
  Serial.println(" [ DONE ] \n");

  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached!");
    sdCardStatus = -2;
    return sdCardStatus;
  }

  Serial.printf("\n === SD Card Info=== \n\n");
  Serial.printf("- SD Card Type: %s\n", mem_card_type[cardType]);
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("- SD Card Size: %llu MB \n", cardSize);
  Serial.printf("- SD Card Used Space: %llu MB \n", SD.usedBytes()/(1024*1024));
  Serial.printf("- SD Card Free Space: %llu MB \n\n", cardSize - (SD.usedBytes()/(1024*1024)));

  return sdCardStatus;
}

/*
  Returns the current SD card status
  -1: Driver not installed
  -2: SD card not found
   0: Installed and works properly
*/
int sd_Status(void){
  return sdCardStatus;
}

int getFileSize(const char *path){

  //toggleSPI_Device(LCD_DISPLAY);
  toggleDisplay(0);

  File file  = SD.open(path);
  int f_size = -1;

  if(!file){
    Serial.printf("Failed to open file : %s \n", path);
    return -1;
  }

  f_size = file.size();
  file.close();
  toggleDisplay(1);

  return f_size;
}

/*
  Reads a file as binary from the SD card and store it to a RAM buffer

  @Params: 
  - path: absolute path to the file. Example: /file.txt, /folder_1/folder_2/file
  - fileBuffer: Pointer to the destination buffer we want to store the file to.
  - fSize: Size of the file in bytes.
*/
int fread(const char *path, unsigned char* fileBuffer, unsigned long* fSize, bool isBinary){
  
  toggleDisplay(0);

  File file = SD.open(path);

  if(!file){
    Serial.printf("Failed to open file : %s \n", path);
    return -1;
  }

  //Read the file and load it to RAM
  unsigned long fileSize = file.size();
  *fSize = fileSize;
  file.read(fileBuffer, fileSize);
  file.close();
  
  toggleDisplay(1);

  if(!isBinary) {
    fileBuffer[fileSize] = '\0';  // Null-terminate
  }

  Serial.printf("File loaded: [%s] |  (%d)bytes \n", path, fileSize);

  return 0;
}


int fwrite(const char *path, unsigned char* fileBuffer, int nBytes, bool isBinary){

  toggleDisplay(0);

  File file = SD.open(path, FILE_APPEND);

  if(!file){
    Serial.printf("Failed to open file [%s] for write\n", path);
    return -1;
  }

  int written = file.write(fileBuffer, nBytes);
  file.close();
  toggleDisplay(1);

  Serial.printf("Written %d bytes to file: [%s] \n", written, path);

  return written;
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
