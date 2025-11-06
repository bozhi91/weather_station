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

Adafruit_ST7796S_kbv tft = Adafruit_ST7796S_kbv(TFT_CS, TFT_DC, TFT_RST);
GFXcanvas16 canvas_area(CANVAS_W, CANVAS_H); //100x120x2 = 24 KB

void initDisplay(void){

  Serial.print(" - Initializing display: ST7796S");

  tft.begin(62500000); //62.5mhz
  tft.setRotation(3);
  tft.fillScreen(COLOR_BLACK);

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
  return &tft;
}

GFXcanvas16* getCanvas(void){
  return &canvas_area;
}

void displClrScreen(void){
  tft.fillScreen(COLOR_BLACK);
}

