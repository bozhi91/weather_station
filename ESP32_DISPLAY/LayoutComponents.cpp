
#include <Adafruit_ST7796S_kbv.h>
#include <SPI.h>
#include <Adafruit_GFX.h>

#include "LayoutComponents.h"
#include "display.h"
#include "png_convert.h"
#include "MemoryCard.h"

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
void printTextCanvas(GFXcanvas16& canvas_id, String text, int pos_x, int pos_y, unsigned short color) {

  canvas_id.setCursor(pos_x, pos_y);
  canvas_id.setTextColor(color);
  canvas_id.setTextSize(2);
  canvas_id.println(text);
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
  //Serial.printf("draw icon at: %dx%d, size: %dx%d \n\n", at_x, at_y, width, height);

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

    //The image is stored into a temporary memory(canvas). 
    //When all the data is written to the temp memory, the canvas data is send to the display
    drawToCanvas(canvas_id, at_x, at_y + y, rgb565Buffer, width, 1);
  }
}

/** ================ CANVAS FUNCTIONS ================ */
/**
  Copy the canvas area to the video memory
  @Pre: Create a canvas area and fill it with some data.
*/
void canvasToScreen(GFXcanvas16& canvas_id, int at_x, int at_y){
  getDisplayInstance()->drawRGBBitmap(at_x, at_y, canvas_id.getBuffer(), canvas_id.width(), canvas_id.height());
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

    /*GFXcanvas16 *canvas = new GFXcanvas16(100, 50);
    // use it
    tft.drawRGBBitmap(0, 0, canvas->getBuffer(), canvas->width(), canvas->height());
    delete canvas;  // free memory
    */

  getCanvas()->drawRGBBitmap(x, y, bitmap, w, h);
}



/************************ GUI FUNCTIONS ********************************/


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


/***
  Display the weather icon with some basic data on the canvas
  then copy the canvas memory to the display memory at given coordinates.
*/
void displayIcon(GFXcanvas16& canvas_id, Current_weather* current, int x, int y){

  Serial.printf(" T: %d, %d, %d \n", current->max_temp, current->min_temp, current->cond_id);

  //Display icon frame
  canvas_id.fillScreen(Display_Color_Black);
  canvas_id.drawRoundRect(0, 0, CANVAS_W, CANVAS_H-20, 5, Display_Color_Blue);
  canvas_id.drawLine(10, 80, CANVAS_W-10, 80, Display_Color_Blue);

  char icon_name[20];
  char data_buff[10];

  //Load and display the icon
  switch(current->cond_id){

    case 1000:
      strcpy(icon_name, "/sunny.png");
    break;

    case 1003:
      strcpy(icon_name, "/part_cloud.png");
    break;

    case 1006:
    case 1009:
      strcpy(icon_name, "/cloudy_2.png");
    break;

    case 1030:
    case 1135:
    case 1147:
      strcpy(icon_name, "/fog?@.png");
    break;

    case 1063:
    case 1180:
    case 1183:
    case 1186:
    case 1189:
    case 1192:
    case 1195:
      strcpy(icon_name, "/rain_3.png");
    break;

    default:
      strcpy(icon_name, "/part_cloud.png");
  }

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

