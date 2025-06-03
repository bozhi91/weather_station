
/***************************************************************************

  ===== Graphic manager for ST7796S DISPLAY =====

  Get rsources at:

  GUI Library:
    https://github.com/adafruit/Adafruit-GFX-Library

  Fonts: 
    https://rop.nl/truetype2gfx/
    https://ttfonts.net/font/letter/a.htm
    https://www.1001freefonts.com/lcd-fonts.php
    https://www.fontsquirrel.com/fonts/list/popular

  Tutorials:   
    https://github.com/vjmuzik/Adafruit_GFX_Buffer/blob/master/examples/ST7789/ST7789_Simple_PXP_DMA/ST7789_Simple_PXP_DMA.ino
    https://bytesnbits.co.uk/basic-animation-spi-touchscreen/

  
    Icon editor: 
      - http://www.faviconer.com/
      - https://www.xiconeditor.com/
      
    Icon to HEX concerter: 
      - http://dotmatrixtool.com/# :::>>> - 16px by 16px, row major, big endian. 
      - https://javl.github.io/image2cpp/  

        PARAMS:
       - Invert image colors: yes
       - Brightness / alpha threshold: >150
       - Draw mode: horizontal, 1 bit per pixel
  
      https://electropeak.com/learn/absolute-beginners-guide-to-tft-lcd-displays-by-arduino/

**************************************************************************/

#include "./gui.h"

  //Initialize the display
  Adafruit_ST7796S_kbv display = Adafruit_ST7796S_kbv(TFT_CS, TFT_DC, TFT_RST);

  GuiManager::GuiManager(){
    
    //tft.enableDisplay(true);
    // tft.initR(INITR_BLACKTAB);

    //Configure screen settings
    display.begin();
    display.setRotation(3);
    display.fillScreen(COLOR_BLACK);
    display.setFont(&FreeSans9pt7b);
  }

  Adafruit_ST7796S_kbv getDisplayInstance(void) {
    return display;
  } 

  //11 pixels per each character
  void GuiManager::printText(byte posX, byte posY, const char* text, int color, int size){
    
    display.setCursor(posX, posY);
    display.setTextSize(size);
    display.setTextColor(color); 
    display.println(text);
  }

//The toolbar uses the status byte as follows: D7|D6|D5|D4|D3|D2|D1|D0
// - D0: WIFI status(0: not connected, 1: connected)
// - D1: Bluetooth status (0: not connected, 1: connected)

void GuiManager::displayTooblar(byte status){

  //Display the toolbar
  if(status){
    display.setFont(&FreeSans9pt7b);
    display.fillRect(0, 0, display.width(),30, COLOR_LIGHTGREY);

    //Draw bitmap images/icons
    display.drawBitmap(10,10,wifi_icon, 20, 15, COLOR_BLACK);
    display.drawBitmap(40,10,bluetooth, 16,16, COLOR_BLACK);
    display.drawBitmap(60,10,battery_icon, 20,15, COLOR_BLACK);
    
    printText(90,20,"Toolbar",COLOR_BLACK,1);
  }
  else{
     display.fillRect(0, 0, display.width(),30, COLOR_DARKGREY);
  }
}

/*
void drawCanvas(bool clear){

  uint16_t width = 200, height = 200;

  GFXcanvas16 *canvas = new GFXcanvas16 (width,height);
  canvas->fillRect (0, 0, 100, 100, COLOR_RED);
  tftDisplay.drawRGBBitmap (0, 0, canvas->getBuffer (), width, height);

  delete canvas;
}*/

void GuiManager::displayLayout(int id){

//Render the layout
int pos = 0;
  do{
    switch(settings[pos].component){

        case CID_Text:
            renderText(settings[pos].params);
        break;

        case CID_Button:
           renderButton(settings[pos].params);
        break;
    }
    pos++;
  }while(settings[pos].component != ENDLIST);

  displayTooblar(true);
}


void GuiManager::renderText(int id){

    //Select the text components from the main layout
    CT_TEXT text = layoutTexts[id];
    printText(text.x,text.y,text.text,text.color,text.size);
}

void GuiManager::renderButton(int id){

  CT_BUTTON btn = layoutButtons[id];
  display.fillRoundRect(btn.x, btn.y, btn.sx, btn.sy, 5, btn.color);
 
  CT_TEXT text = layoutTexts[2];
  printText(btn.x+20, btn.y+20,text.text,COLOR_BLACK,text.size);
}


void GuiManager::displayTest(void){}

