#include <Adafruit_ST7796S_kbv.h>
#include <SPI.h>
#include "png_convert.h"
#include <Adafruit_GFX.h>

//#include <Adafruit_GFX.h>
#include "display.h"
#include "hardware.h"
#include "MemoryCard.h"


Adafruit_ST7796S_kbv tft = Adafruit_ST7796S_kbv(TFT_CS, TFT_DC, TFT_RST);
GFXcanvas16 canvas(400, 80);

void initDisplay(void){

  Serial.print(" - Initializing display: ST7796S");

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(COLOR_BLACK);

  Serial.printf("\t [ DONE ] \n");
}

void displayLayout(void){

  //Draw directly on the screen
  //tft.drawRoundRect(5, 5, DISPLAY_WIDTH-10, DISPLAY_HEIGHT-10, 5, Display_Color_Yellow);

  //Draw Canvas
  //canvas.fillScreen(COLOR_BLACK);

  loadPNG("/part_cloud_2.png", 20, 10);
  loadPNG("/rain.png",     90, 10);
  loadPNG("/sunny_2.png",  160, 10);

  loadPNG("/snow.png",  225, 10);
  loadPNG("/wind.png",  290, 10);
  loadPNG("/fog.png",   355, 10);

  updateScreen();
}

/**
  Store a bitmap image to the canvas.
  The bitmap size and coordinates must not exceed the canvas area.

  @Pre: canvas area has to be created first.
  @Params: 
  - x/y: top-left coordinates of the bitmap in the canvas area. 
  - w/h: size of the bitmap in pixels
*/
void drawToCanvas(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h){
  canvas.drawRGBBitmap(x, y, bitmap, w, h);
}

/**
  Copy the canvas area to the video memory
  @Pre: Create a canvas area and fill it with some data.
*/
void updateScreen(void){
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
}

/************************ GUI FUNCTIONS ********************************/

/**
  Read a PNG file from the SD card and display it on the screen.
  The image is converted to RGB565 format since the display works with this color format only.

   @Params: 
    - fileName: absolute path to the file. Example: /file.png, /images/file.png
    - at_x/at_y: coordinates of the top-left corner of the image that will be printed on the screen
*/
void loadPNG(const char* fileName, int at_x, int at_y){
  
  uint16_t rgb565Buffer[320];
  unsigned char rawFileData[4096];
  std::vector<unsigned char> rgbaOutput;
  unsigned long width, height, pngSize=0;

  memset(rgb565Buffer, 0, sizeof rgb565Buffer);
  memset(rawFileData, 0, sizeof(rawFileData));

  //Read the png binary file and store it to RAM
  fread(fileName, rawFileData, &pngSize);

  //Decode the PNG file to RGB565 format
  int result = decodePNG(rgbaOutput, width, height, rawFileData, pngSize, 1);

  if(result != 0){
    Serial.print(" PNG DECODE ERROR!!!!");
    return;
  }

  Serial.printf("\n -> PNG Loaded: %dx%d \n", width, height);

  //Convert the RAW PNG Format to RGB565 and display the image on the screen
  //Only one line of the image is stored at the time.
  //This size has to be greater than the image length.

  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
    
        int i = (y * width + x) * 4;
        uint8_t r = rgbaOutput[i + 0];
        uint8_t g = rgbaOutput[i + 1];
        uint8_t b = rgbaOutput[i + 2];

        rgb565Buffer[x] = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) |(b >> 3);
    }

    drawToCanvas(at_x, at_y + y, rgb565Buffer, width, 1);
  }
}

void printText(String text, int pos_x, int pos_y) {

  tft.setCursor(pos_x, pos_y);
  tft.setTextColor(COLOR_GREEN);
  tft.setTextSize(2);
  tft.println(text);
}

/**
  Draw a monochromatic bitmap on the screen using the format RGB-565
**/
void drawBitmap(unsigned char startX, unsigned char  startY, unsigned char width, unsigned char  height, const unsigned char* bitmap, int nBytes, unsigned short color) {

  int x = 0, y = 0;

  for(int idx = 0; x < width; idx++){
    for(int i = 0; i<8; i++){

      //split the byte in bits. Each bit represents one pixel
     /* if (x + startX >= SCREEN_HEIGHT  || y + startY > SCREEN_HEIGHT) {
        break; // Stop if we exceed the display width
      }*/

      tft.drawPixel(x + startX, y + startY, (bitmap[idx] >>i) & 1 ? color : Display_Color_Black);
      y++;

      if(y == height){
        if(height%8 == 0){
          idx++;
        }

        x++;
        y = 0;
        break;
      }
    }
  }
}

/**
  Draw a single color line using the color format: RGB565.
*/
void drawRGBLine(const unsigned short buffer[], int start_X, int start_Y, unsigned int nBytes){
  
  for(int i=0; i < nBytes; i++){
    tft.drawPixel((i+start_X), start_Y, pgm_read_word(buffer+i));
  }
}

/***
  Draw a multicolor bitmap on the screen using the color format RGB-565
**/
void drawColorBitmap(const unsigned short image[], int posX, int posY, int sizeX, int sizeY){

  int row, col, buffidx=0;

  for(row=0; row < sizeY; row++){
    for(col=0; col < sizeX; col++){
        tft.drawPixel((col+posX),(row+posY), pgm_read_word(image + buffidx));
        buffidx++;
    }
  }
}
