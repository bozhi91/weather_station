#include <Adafruit_ST7796S_kbv.h>
#include <SPI.h>
#include <Adafruit_GFX.h>

#include "display.h"
#include "hardware.h"
#include "API.h"
#include "Layout.h"
#include "LayoutComponents.h"

/**
   This file implements the low-level display driver functions.
   All the function related to display configuration, send/receive commands,
   memory allocation,
*/

Adafruit_ST7796S_kbv *tft = nullptr;
GFXcanvas16 *canvas       = nullptr;  //100x120x2 = 24 KB

void initDisplay(void){

  Serial.print(" - Initializing display: ST7796S... ");
  
  canvas = new GFXcanvas16(CANVAS_W, CANVAS_H);
  tft    = new Adafruit_ST7796S_kbv(TFT_CS, TFT_DC, TFT_RST);

  tft->begin(); //62.5mhz
  tft->setRotation(3);
  tft->fillScreen(COLOR_BLACK);

  Serial.printf("\t [ DONE ] \n");
}

/**
  Toggle the display command line by changing the CS(chip select) pin state.
  -> LOW is enabled!
*/
void toggleDisplay(bool state){

  digitalWrite(TFT_CS, !state);
  delay(10);
}

Adafruit_ST7796S_kbv* getDisplayInstance(void) {
  return tft;
}

GFXcanvas16* getCanvas(void){
  return canvas;
}

void displClrScreen(void){
  tft->fillScreen(COLOR_BLACK);
}

/** ================ CANVAS FUNCTIONS ================ */
/**
  Copy the canvas area to the video memory
  @Pre: Create a canvas area and fill it with some data.
*/
void canvasToScreen(int at_x, int at_y){

  tft->drawRGBBitmap(at_x, at_y, canvas->getBuffer(), canvas->width(), canvas->height());
}

void deleteCanvas(void){

  delete canvas;
  canvas = nullptr;
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

  canvas->drawRGBBitmap(x, y, bitmap, w, h);
}

