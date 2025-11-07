
#include <Adafruit_ST7796S_kbv.h>
#include <SPI.h>
#include <Adafruit_GFX.h>

#include "LayoutComponents.h"
#include "display.h"
#include "png_convert.h"
#include "MemoryCard.h"


/** ========================== BASIC LAYOUT COMPONENTS ========================== */

void _drawLayoutBMP(Image bmp){
  drawMonochromeBitmap(bmp.pos_x, bmp.pos_y, bmp.width, bmp.height, bmp.bitmap, sizeof(bmp.size), bmp.color, COLOR_BLACK);
}

/**** DISPLAY THE LAYOUT BASIC COMPONENTS: SHAPE, LABEL, BUTTOM, BITMAP, ETC... *****/
void _drawLayoutShape(Shape shape){
  getDisplayInstance()->drawRoundRect(shape.pos_x, shape.pos_y,shape.end_x, shape.end_y, 5, shape.color);
}

void _drawLayoutLabel(TextLabel item){

  static int len = 0;

  //Delete the previous text before drawing the new one
  if(len != 0){
    int width  = len*(CHAR_W+1)*item.size+5;
    int height = (CHAR_H+1)*item.size;

    getDisplayInstance()->fillRoundRect(item.pos_x, item.pos_y, width, height, 5, COLOR_BLACK);
  }

  len = strlen(item.label);
  printTextEx(item.label, item.size, item.pos_x, item.pos_y, item.color);
}

void _drawWeatherWidged(WeatherWidged widged){

   printTextEx(widged.label, 2, widged.width/3, widged.pos_y-20, Display_Color_White);

  //todo: not implemented yet
    getDisplayInstance()->fillRoundRect(widged.pos_x, widged.pos_y, widged.width, widged.height, 3, widged.bg_color);
    getDisplayInstance()->drawRoundRect(widged.pos_x, widged.pos_y, widged.width, widged.height, 3, widged.frame_color);
}
/** ========================== END OF: BASIC LAYOUT COMPONENTS ========================== */


/** ========================== LOW-LEVEL LAYOUT FUNCTIONS ========================== */

/** ================ TEXT FUNCTIONS ================ */

void printText(String text, int pos_x, int pos_y) {

  Adafruit_ST7796S_kbv* tft = getDisplayInstance();

  tft->setCursor(pos_x, pos_y);
  tft->setTextColor(COLOR_GREEN);
  tft->setTextSize(1);
  tft->println(text);
}

void printTextEx(String text, int size ,int pos_x, int pos_y, unsigned short color) {

  Adafruit_ST7796S_kbv* tft = getDisplayInstance();

  tft->setCursor(pos_x, pos_y);
  tft->setTextColor(color);
  tft->setTextSize(size);
  tft->println(text);
}

/**
  Print a text to the canvas memory. Later, that text will be sent to the video memory
**/
void printTextCanvas(GFXcanvas16* canvas_id, String text, int pos_x, int pos_y, unsigned short color) {

  canvas_id->setCursor(pos_x, pos_y);
  canvas_id->setTextColor(color);
  canvas_id->setTextSize(2);
  canvas_id->println(text);
}


/** ================ SHAPE FUNCTIONS ================ */

//Draw a single color line using the color format: RGB565.
void drawRGBLine(const unsigned short buffer[], int start_X, int start_Y, unsigned int nBytes){

  Adafruit_ST7796S_kbv* tft = getDisplayInstance();

  for(int i=0; i < nBytes; i++){
    tft->drawPixel((i+start_X), start_Y, pgm_read_word(buffer+i));
  }
}

void display_FillRect(int x, int y, int w, int h, unsigned short color){
}

/** ================ IMAGE FUNCTIONS ================ */

//Draw a monochromatic bitmap on the screen using the format RGB-565
void drawMonochromeBitmap(
  unsigned char startX, unsigned char  startY,
  unsigned char width, unsigned char  height,
  const unsigned char* bitmap, int nBytes,
  unsigned short color, unsigned short bg_color) {

  int x = 0, y = 0;

  for(int idx = 0; x < width; idx++){
    for(int i = 0; i<8; i++){

      //split the byte in bits. Each bit represents one pixel
     /* if (x + startX >= SCREEN_HEIGHT  || y + startY > SCREEN_HEIGHT) {
        break; // Stop if we exceed the display width
      }*/

      getDisplayInstance()->drawPixel(x + startX, y + startY, (bitmap[idx] >>i) & 1 ? color : bg_color);
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

//Draw a multicolor bitmap on the screen using the color format RGB-565
void drawColorBitmap(const unsigned short image[], int posX, int posY, int sizeX, int sizeY){

  int row, col, buffidx=0;

  for(row=0; row < sizeY; row++){
    for(col=0; col < sizeX; col++){
        getDisplayInstance()->drawPixel((col+posX),(row+posY), pgm_read_word(image + buffidx));
        buffidx++;
    }
  }
}

/**
  Read a PNG file from the SD card, transforms it to RGB565 format and stores it into a temporary RAM memory.

  Later, the image will be display it on the screen. 
  To do that, use the function: canvasToScreen(x, y);
  
   @Params:
    - fileName: absolute path to the file. Example: /file.png, /images/file.png
    - at_x/at_y: coordinates of the top-left corner of the image that will be printed on the screen
*/
int loadPNG(const char* fileName, int at_x, int at_y, unsigned short bg_color){

  int MAX_FILE_SIZE = 4096;
  int MAX_IMG_WIGTH = 64;
  unsigned short img_bg_color = Color_DGray;

  //max icon width: 64pix. 1pix=16bit --> 64x16 = 1024bits = 128Bytes
  std::vector<unsigned char> RGB565Data;
  unsigned long width, height, pngSize = 0, f_size = 0;
  unsigned char* fileContent;

  //Check the file size
  f_size = getFileSize(fileName);

  if(f_size >= MAX_FILE_SIZE){
    Serial.printf("ERROR! PNG file is too big: %d Bytes\n", f_size);
    return -1;
  }

  //Allocate enough memory to store the file in RAM
  fileContent = (unsigned char*)malloc(f_size);
  memset(fileContent,  0, f_size);

  //Read the png binary file and store it to RAM
  fread(fileName, fileContent, &pngSize, 1);

  //Decode the PNG file to RAW RGB buffer format
  int result = decodePNG(RGB565Data, width, height, fileContent, pngSize, 0);

  free(fileContent);

  if(result != 0){
    Serial.printf(" PNG DECODE ERROR! Code: %d \n", result);
    return result;
  }

  Serial.printf("PNG image decoded to RGB565. Size: %dx%d \n", width, height);

  //Convert the RAW PNG Format to RGB565 and display the image on the screen
  //Only one line of the image is stored at the time.
  //This size has to be greater than the image width.
  //Serial.printf("draw icon at: %dx%d, size: %dx%d \n\n", at_x, at_y, width, height);

  if(width > MAX_IMG_WIGTH){
    Serial.printf("PNG image width too big(w=%d). Max width: %d. Aborting!\n",width, MAX_IMG_WIGTH);
    return -1;
  }

  //unsigned short* pixels = (unsigned short*)RGB565Data;
  //drawToCanvas(at_x, at_y, pixels, 64, 64);

  /**
    - Convert the 32-bit RGBA color returned from pngConverter to RGB565.
    - Remove the background color of the image 
    - Write the image buffer to the canvas memory
  **/
  uint16_t pixel;
  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        
        int i = (y * width + x) * 4;

        uint8_t r = RGB565Data[i + 0];
        uint8_t g = RGB565Data[i + 1];
        uint8_t b = RGB565Data[i + 2];

        pixel = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) |(b >> 3);

        //Remove image background and replace it by a color of our choise
        if(pixel == Display_Color_Black){
          pixel = img_bg_color;
        }
        drawToCanvas(at_x + x, at_y + y, &pixel, 1, 1);
    }
  }
  return 0;
}

uint8_t rgb565_to_rgb332(uint16_t c) {

  uint8_t r = (c >> 11) & 0x1F;
  uint8_t g = (c >> 5)  & 0x3F;
  uint8_t b =  c        & 0x1F;

  return ((r >> 2) << 5) | ((g >> 3) << 2) | (b >> 3);
}

void rgb888_to_rgb565(uint16_t* dest, const uint8_t* src, size_t pixel_count) {

  for (size_t i = 0; i < pixel_count; i++) {

    uint8_t r = src[i * 3 + 0];
    uint8_t g = src[i * 3 + 1];
    uint8_t b = src[i * 3 + 2];

    dest[i] = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  }
}

void pack_4bpp(const uint8_t *indices, uint8_t *dest, size_t pixel_count) {

    size_t di = 0;

    for (size_t i = 0; i < pixel_count; i += 2) {
      uint8_t a = indices[i] & 0x0F;
      uint8_t b = (i+1 < pixel_count) ? (indices[i+1] & 0x0F) : 0;
      dest[di++] = (a << 4) | b;
    }
}
/** ========================== END OF: LOW-LEVEL LAYOUT FUNCTIONS ========================== */

/** ========================== HIGH-LEVEL LAYOUT COMPONENTS ========================== */
/**
  Display a text box at coords: 100,100 inside a yeallow frame
*/
void msgBox(const char* text,  MSG_BOX_TYPES type){

  static int prev_len   = 0;
  static int char_width = 12;
  static int padding    = 20; //horizontal padding


  Adafruit_ST7796S_kbv* tft = getDisplayInstance();

  if(type == TYPE_NONE){
    if (prev_len > 0) {
      tft->fillRoundRect(MSG_BOX_X, MSG_BOX_Y, prev_len, MSG_BOX_H, 5, Display_Color_Black);
    }
    Serial.printf("prev_len: %d \n",prev_len);
    return;
  }

  // Calculate length of the message box
  int len = strlen(text) * char_width + padding*2;  // 12 px per char + padding

  // Clear previous box (using previous size)
  if (prev_len > 0) {
    tft->fillRoundRect(MSG_BOX_X, MSG_BOX_Y, prev_len, MSG_BOX_H, 5, Display_Color_Black);
  }

  // Draw the new box
  tft->fillRoundRect(MSG_BOX_X, MSG_BOX_Y, len, MSG_BOX_H, 5, Display_Color_White);
  tft->drawRoundRect(MSG_BOX_X, MSG_BOX_Y, len, MSG_BOX_H, 5, Display_Color_Blue);

  // Draw the text
  printTextEx(text,2,  MSG_BOX_X + padding, 105, type == TYPE_OK ? Display_Color_Black : Display_Color_Red);

  // Save current length for next erase
  if(type == TYPE_NONE){
    prev_len = len;
  }
}

/**************************************************************************
  Display the weather icon with some basic data on the canvas
  then copy the canvas memory to the display memory at given coordinates.
***************************************************************************/
void displayWeatherIcon(Weather_Data* forecast, int x, int y){

  GFXcanvas16* canvas_id = getCanvas();

  unsigned short fill_color = Color_DGray;         //Icon Bg color
  unsigned short bg_color   = Display_Color_Black; //Bg color for the entire object including image, text, and frame

  //Display icon frame
  canvas_id->fillScreen(bg_color); //Clear canvas memory by filling the frame with a given color
  canvas_id->fillRoundRect(0, 0, CANVAS_W, CANVAS_H-20, 5, fill_color);     //Fill canvas frame
  canvas_id->drawRoundRect(0, 0, CANVAS_W, CANVAS_H-20, 5, Color_LGray);    //Display the canvas frame
  canvas_id->drawLine(10, 80, CANVAS_W-10, 80, Display_Color_Blue);         // Draw a separate line

  char icon_name[64];
  char data_buff[10];

  sprintf(icon_name, "/resources/default_icons/storm_1.png", 15);

  //Load a PNG image file from the SD card
  int result = loadPNG(icon_name, CANVAS_W/2-32, 10, fill_color);

  if(result != 0){
    Serial.printf(" PNG Convert error: %d \n", result);
    return;
  }

  //Display the weather data: temp(min/max), day of the week
  memset(data_buff, 0, sizeof data_buff);
  sprintf(data_buff,"%dC|%dC", forecast->max_temp, forecast->min_temp);
  printTextCanvas(canvas_id, data_buff, 5, 90, COLOR_YELLOW);

  //Display the date
  memset(data_buff, 0, sizeof data_buff);
  sprintf(data_buff,"%s", forecast->date);
  printTextCanvas(canvas_id, data_buff, CANVAS_W/2-32, 125, COLOR_WHITE);

  //Copy canvas memory to screen
  canvasToScreen(x, y);
}

void displayToolbar(Image iconList[]){

  /*
  int toolbar_y = 0;
  int toolbar_h = 40;

  //Draw the toolbar
  displInst->fillRoundRect(0, toolbar_y, DISPLAY_WIDTH, toolbar_h, 5, Display_Color_LBlack);
  displInst->drawRoundRect(0, toolbar_y, DISPLAY_WIDTH, toolbar_h, 5, COLOR_YELLOW);

  //Display a message box
  int mb_x = 50, mb_y = 70;*/

  /*displInst->fillRoundRect(mb_x, mb_y, 250, 40, 3, Display_Color_LBlack); //header bg
  displInst->drawRoundRect(mb_x, mb_y, 250, 40, 3, COLOR_YELLOW);         //header
  displInst->drawRoundRect(mb_x, mb_y, 250, 170,3, COLOR_YELLOW);         //mb body

  printTextEx("WARNING", 2, mb_x+10, mb_y+20, COLOR_WHITE);
  printTextEx("This is a test!", 2, mb_x+10, mb_y+60, COLOR_WHITE);
  printTextEx("You've been warned!", 2, mb_x+10, mb_y+80, COLOR_WHITE);*/
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  //progressBar(50);
}


void displayProgressBar(int percent){
/*
  int width = 400;
  int mb_x  = 20;
  int mb_y  = 100;

  int progress = 0;
  int txt_offset_x = width/2-60+mb_x;
  char txt[20];

  if(percent>100){
    return;
  }

  //Draw an empty progress bar frame
  if(percent==0){
    displInst->drawRoundRect(mb_x, mb_y+50, width, 35, 3, COLOR_YELLOW);
    displInst->fillRoundRect(mb_x+1, mb_y+51, width-2, 33, 3, Display_Color_Black);
  }

  progress = width*percent/100;
  if(progress>=width){
    progress = width-2;
  }

  //Print the progress in %
  sprintf(txt, "LOADING:%d%%",percent);//11*14+11
  displInst->fillRoundRect(txt_offset_x+93, mb_y+10, 50, 30, 0, Display_Color_Black);
  printTextEx(txt, 2, txt_offset_x, mb_y+20, COLOR_WHITE);//font size: 10x14

  //Fill the progress bar frame
  displInst->fillRoundRect(mb_x+1, mb_y+51, progress, 33, 3, Display_Color_Red);*/
}

/***
  Display the time and date in format: dd/mm/YY - HH:mm:ss(12/24 hour format)
*/
void displayTimeDate(void){

  //https://api.api-ninjas.com/v1/timezone?timezone=Europe/London
  /*DateTime time = {0};

  memset(&time, 0, sizeof time);
  readTimeAPI(&time);
  msgBox("", TYPE_NONE);

  displInst->fillRect(40, 30, 340,80, COLOR_BLACK);
  printTextEx(time.time, 4, 40, 30, Display_Color_Yellow);
  printTextEx(time.date, 4, 40, 70, Display_Color_Yellow);

  Serial.printf("Date: %s:%s \n", time.date, time.time);

  day[0] = time.date[3];
  day[1] = time.date[4];*/
}

