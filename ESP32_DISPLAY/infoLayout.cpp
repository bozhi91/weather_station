 

#include "infoLayout.h"
#include "Layout.h"
#include "LayoutComponents.h"
#include "display.h"

    /*Image icons_list[] = {
      { 50,  20, 12,  11, bmp_warning, sizeof(bmp_warning), Display_Color_Green },
      { 90,  20, 15,  12, antena,      sizeof(antena),      Display_Color_Red   },
      { 130, 20, 14,  8,  battery,     sizeof(battery),     Display_Color_Blue  },
    };*/


LayoutTemplate HomeScreen[] = {
  { .type  = TYPE_LABEL, .text = { 100, 160, Display_Color_Blue, "TEST LABEL 1 - HELLO", 0 } },
  { .type  = TYPE_LABEL, .text = { 100, 100, Display_Color_Red,  "TEST LABEL 2 - WORLD", 0 } },
 };

LayoutTemplate InfoScreen[] = {
  { .type  = TYPE_RECTANGLE, .shape = { 10,  10, 200, 50, Display_Color_Red  } },
  { .type  = TYPE_RECTANGLE, .shape = { 220, 10, 200, 50, Display_Color_Blue } },
  { .type  = TYPE_BITMAP,    .bmp   = { 50,  30, 12,  11, bmp_warning, sizeof(bmp_warning), Display_Color_Green } },      
};

void _viewInfoLayout(void){

  /*
    InfoScreen[1].type  = TYPE_RECTANGLE;
    InfoScreen[1].shape = { 220, 10, 200, 50, Display_Color_Blue };

    InfoScreen[2].bmp   = { 50,  30, 12,  11, bmp_warning, sizeof(bmp_warning), Display_Color_Green };
    InfoScreen[3].bmp   = { 70,  30, 15,  12, antena,      sizeof(antena), Display_Color_Red        };
    InfoScreen[4].bmp   = { 90,  30, 14,  8,  battery,     sizeof(battery), Display_Color_Blue      };
    InfoScreen[5].text  = { 230, 20, Display_Color_Yellow, "HELLO", 0 };    
  */

  size_t count = sizeof(InfoScreen) / sizeof(InfoScreen[0]);
  loadLayout(InfoScreen, count, 1);
}

void _controllInfoLayout(void){

  char data[64];

  sprintf(data, "Chip model: %s ",  ESP.getChipModel());
  msgBox(data, TYPE_OK);
}

//Display the layout components on screen
void _viewHomeLayout(void){
  
  size_t count = sizeof(HomeScreen) / sizeof(HomeScreen[0]);
  loadLayout(HomeScreen, count, 1);
}

//Modify the layout components values
void _controllHomeLayout(void){

  unsigned long tim = millis();

  sprintf(HomeScreen[0].text.label, "Uptime: %ld ms", tim);
  loadLayout(&HomeScreen[0], 1, 0);
  
  //msgBox(str, TYPE_OK);
}

    /*HomeScreen[0].text  = { 100, 160,  Display_Color_Blue, "TEST LABEL 1 - HELLO", 0 };
    HomeScreen[1].text  = { 100, 100, Display_Color_Red,  "TEST LABEL 2 - WORLD", 0 };
    HomeScreen[2].shape = { 90,  90,  380, 50, Display_Color_Green   };
    HomeScreen[3].shape = { 90,  150, 380, 50, Display_Color_Yellow  };*/


