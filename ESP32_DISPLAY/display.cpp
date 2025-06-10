#include <Adafruit_ST7796S_kbv.h>
#include <SPI.h>
#include <Adafruit_GFX.h>

#include "png_convert.h"
#include "display.h"
#include "hardware.h"
#include "MemoryCard.h"
#include "API.h"

#define CANVAS_X  50
#define CANVAS_Y  150

#define CANVAS_W  100
#define CANVAS_H  140

#define MSG_BOX_X  40
#define MSG_BOX_Y  90
#define MSG_BOX_H  50

Adafruit_ST7796S_kbv tft = Adafruit_ST7796S_kbv(TFT_CS, TFT_DC, TFT_RST);
GFXcanvas16 canvas_icon(CANVAS_W, CANVAS_H); //100x120x2 = 24 KB

void displayMainLayout(void);
void displayHomeLayout(void);
void displayTimeDate(void);

void initDisplay(void){

  Serial.print(" - Initializing display: ST7796S");

  tft.begin(62500000); //62.5mhz
  tft.setRotation(3);
  tft.fillScreen(COLOR_BLACK);
  
  loadLayout(0);
  Serial.printf("\t [ DONE ] \n");
}

//Layout manager. Loads layout to the screen by given 'id'
void loadLayout(int id){

  if(id == 0){
    tft.fillScreen(COLOR_BLACK);
    displayMainLayout();  //init display main layout
  }
  else if(id == 1){
    displayHomeLayout(); //init home layout
  }
}

void displayMainLayout(void){
  tft.drawRoundRect(5, 5, DISPLAY_WIDTH-10, DISPLAY_HEIGHT-10, 5, Display_Color_Yellow);
  msgBox("INITIALIZING, PLEASE WAIT...", TYPE_OK);
}

void displayHomeLayout(void){

  //Display daily icon data. 
  //We use the same method and canvas memory for all the icons of hte week. Including the today's forecast icon.
  //We just change the icon parmeters and coordinates. Then we copy the canvas back to the screen.
  Current_weather current[5];

  memset(current, 0, sizeof(current));

  strcpy(current[0].date, "10");
  readWeatherAPI(&current[0]);

  strcpy(current[1].date, "11");
  readWeatherAPI(&current[1]);

  strcpy(current[2].date, "12");
  readWeatherAPI(&current[2]);

  strcpy(current[3].date, "13");
  readWeatherAPI(&current[3]);

  msgBox("", TYPE_NONE);
  
  displayDailyIcon(canvas_icon, &current[0], 30, 150);
  displayDailyIcon(canvas_icon, &current[1], 140 ,150);
  displayDailyIcon(canvas_icon, &current[2], 250 ,150);
  displayDailyIcon(canvas_icon, &current[3], 360 ,150);

  //displayDailyIcon(canvas_icon, &current[3], 360 , 20);

 /* displayDailyIcon(canvas_icon, "/sunny.png",  140 ,150);
  displayDailyIcon(canvas_icon, "/rain_3.png", 250 ,150);
  displayDailyIcon(canvas_icon, "/wind.png",   360 ,150);*/

  //TODO: display toolbar and other components
  displayTimeDate();
}

void displayTimeDate(void){

  char timme_buff[50];
  memset(timme_buff, 0, sizeof timme_buff);
  readTimeAPI(timme_buff);
  printTextEx(timme_buff, 50, 50, Display_Color_Yellow);
}

/***  
  Display the weather icon with some basic data on the canvas
  then copy the canvas memory to the display memory at given coordinates.
*/
void displayDailyIcon(GFXcanvas16& canvas_id, Current_weather* current, int x, int y){

  Serial.printf(" T: %d, %d, %d \n", current->max_temp, current->min_temp, current->cond_id);

  //Display icon frame
  canvas_id.fillScreen(Display_Color_Black);
  canvas_id.drawRoundRect(0, 0, CANVAS_W, CANVAS_H-20, 5, Display_Color_Blue);
  canvas_id.drawLine(10, 80, CANVAS_W-10, 80, Display_Color_Blue);
 
  char icon_name[20];
  char data_buff[10];

  //Load and display the icon
  //if(current->cond_id == 1003){
    strcpy(icon_name, "/part_cloud.png");
  
  loadPNG(canvas_id, icon_name, CANVAS_W/2-32, 10);

  //Display the weather data: temp(min/max), day of the week
  memset(data_buff, 0, sizeof data_buff);
  sprintf(data_buff,"%dC|%dC", current->max_temp, current->min_temp);
  printTextCanvas(canvas_id, data_buff, 5, 90, COLOR_YELLOW);

  //Display day of the week
  memset(data_buff, 0, sizeof data_buff);
  sprintf(data_buff,"%s", current->date);
  printTextCanvas(canvas_id, data_buff, CANVAS_W/2-32, 125, COLOR_WHITE);

  //Copy canvas memory to screen
  canvasToScreen(canvas_id, x, y);
}

/**
  Display a text box at coords: 100,100 inside a yeallow frame
*/
void msgBox(const char* text,  MSG_BOX_TYPES type){
  
  static int prev_len   = 0;
  static int char_width = 12;
  static int padding    = 20; //horizontal padding
  
  if(type == TYPE_NONE){
    if (prev_len > 0) {
      tft.fillRoundRect(MSG_BOX_X, MSG_BOX_Y, prev_len, MSG_BOX_H, 5, Display_Color_Black);
    }
    Serial.printf("prev_len: %d \n",prev_len);
    return;
  }

  // Calculate length of the message box
  int len = strlen(text) * char_width + padding*2;  // 12 px per char + padding

  // Clear previous box (using previous size)
  if (prev_len > 0) {
    tft.fillRoundRect(MSG_BOX_X, MSG_BOX_Y, prev_len, MSG_BOX_H, 5, Display_Color_Black);
  }

  // Draw the new box
  tft.fillRoundRect(MSG_BOX_X, MSG_BOX_Y, len, MSG_BOX_H, 5, Display_Color_White);
  tft.drawRoundRect(MSG_BOX_X, MSG_BOX_Y, len, MSG_BOX_H, 5, Display_Color_Blue);

  // Draw the text
  printTextEx(text, MSG_BOX_X + padding, 105, type == TYPE_OK ? Display_Color_Black : Display_Color_Red);

  // Save current length for next erase
  prev_len = len;
}

/**
  Copy the canvas area to the video memory
  @Pre: Create a canvas area and fill it with some data.
*/
void canvasToScreen(GFXcanvas16& canvas_id, int at_x, int at_y){
  tft.drawRGBBitmap(at_x, at_y, canvas_id.getBuffer(), canvas_id.width(), canvas_id.height());
}

/**
  Store a bitmap image to the canvas.
  The bitmap size and coordinates must not exceed the canvas area.

  @Pre: canvas area has to be created first.
  @Params: 
  - x/y: top-left coordinates of the bitmap in the canvas area. 
  - w/h: size of the bitmap in pixels
*/
void drawToCanvas(GFXcanvas16& canvas_id, int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h){
  canvas_id.drawRGBBitmap(x, y, bitmap, w, h);
}

/************************ GUI FUNCTIONS ********************************/

/**
  Read a PNG file from the SD card and display it on the screen.
  The image is converted to RGB565 format since the display works with this color format only.

   @Params: 
    - fileName: absolute path to the file. Example: /file.png, /images/file.png
    - at_x/at_y: coordinates of the top-left corner of the image that will be printed on the screen
*/
void loadPNG( GFXcanvas16& canvas_id, const char* fileName, int at_x, int at_y){
  
  uint16_t rgb565Buffer[320];
  unsigned char rawFileData[4096];
  std::vector<unsigned char> rgbaOutput;
  unsigned long width, height, pngSize=0;

  memset(rgb565Buffer, 0, sizeof rgb565Buffer);
  memset(rawFileData,  0, sizeof(rawFileData));

  //Read the png binary file and store it to RAM
  fread(fileName, rawFileData, &pngSize, 1);

  //Decode the PNG file to RGB565 format
  int result = decodePNG(rgbaOutput, width, height, rawFileData, pngSize, 1);

  if(result != 0){
    Serial.print(" PNG DECODE ERROR!!!!");
    return;
  }

  //Convert the RAW PNG Format to RGB565 and display the image on the screen
  //Only one line of the image is stored at the time.
  //This size has to be greater than the image length.
  Serial.printf("draw icon at: %dx%d, size: %dx%d \n\n", at_x, at_y, width, height);

  if(width>320){
    Serial.printf("image width too big. Aborting");
    return;
  }

  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
    
        int i = (y * width + x) * 4;
        uint8_t r = rgbaOutput[i + 0];
        uint8_t g = rgbaOutput[i + 1];
        uint8_t b = rgbaOutput[i + 2];

        rgb565Buffer[x] = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) |(b >> 3);
    }
    drawToCanvas(canvas_id, at_x, at_y + y, rgb565Buffer, width, 1);
  }
}

/**
  Toggle the display command line by changing the CS(chip select) pin state.
  
  - LOW is enabled!
*/
void toggleDisplay(bool state){

  digitalWrite(TFT_CS, !state);
  delay(10);
}

void printText(String text, int pos_x, int pos_y) {

  tft.setCursor(pos_x, pos_y);
  tft.setTextColor(COLOR_GREEN);
  tft.setTextSize(2);
  tft.println(text);
}

void printTextEx(String text, int pos_x, int pos_y, unsigned short color) {

  tft.setCursor(pos_x, pos_y);
  tft.setTextColor(color);
  tft.setTextSize(2);
  tft.println(text);
}

void printTextCanvas(GFXcanvas16& canvas_id, String text, int pos_x, int pos_y, unsigned short color) {

  canvas_id.setCursor(pos_x, pos_y);
  canvas_id.setTextColor(color);
  canvas_id.setTextSize(2);
  canvas_id.println(text);
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
