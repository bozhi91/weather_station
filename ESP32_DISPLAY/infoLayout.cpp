 
#include "API.h"
#include "infoLayout.h"
#include "Layout.h"
#include "LayoutComponents.h"
#include "display.h"

    /*Image icons_list[] = {
      { 50,  20, 12,  11, bmp_warning, sizeof(bmp_warning), Display_Color_Green },
      { 90,  20, 15,  12, antena,      sizeof(antena),      Display_Color_Red   },
      { 130, 20, 14,  8,  battery,     sizeof(battery),     Display_Color_Blue  },
    };*/


static void displayTime(void);
static void blink(void);

LayoutTemplate HomeScreen[] = {
  { .type  = TYPE_LABEL, .event = {0, 0}, .text = { 100, 160, 2, Display_Color_Blue, "TEST LABEL 1 - HELLO", 0 } },
  { .type  = TYPE_LABEL, .event = {0, 0}, .text = { 100, 100, 2, Display_Color_Red,  "TEST LABEL 2 - WORLD", 0 } },
 };

LayoutTemplate InfoScreen[] = {

  { .type  = TYPE_RECTANGLE, .event = { 0, 0 }, .shape = { 10,  10, 200, 50, Display_Color_Red  } },
  { .type  = TYPE_RECTANGLE, .event = { 0, 0 }, .shape = { 220, 10, 200, 50, Display_Color_Blue } },

  { .type  = TYPE_BITMAP,    .event = { 0, 0 },        .bmp = { 50,  30, 12,  11, bmp_warning, sizeof(bmp_warning), Display_Color_Green } },
  { .type  = TYPE_BITMAP,    .event = { blink, 1000 }, .bmp = { 70,  30, 15,  12, antena,      sizeof(antena), Display_Color_Red   } },
  { .type  = TYPE_BITMAP,    .event = { 0, 0        }, .bmp = { 90,  30, 14,  8,  battery,     sizeof(battery), Display_Color_Blue } },

  { .type  = TYPE_LABEL,     .event = { displayTime, 60000 }, .text = { 100, 100, 2, Display_Color_Red,"Date/Time", 0 } },
};

void _viewInfoLayout(void){

  size_t count = sizeof(InfoScreen) / sizeof(InfoScreen[0]);
  loadLayout(InfoScreen, count, 1);
}

void _controllInfoLayout(void){

  size_t count = sizeof(InfoScreen) / sizeof(InfoScreen[0]);

  //Call certain functions periodically in order to refresh the displayed data
  for(int i=0; i<count; i++){

    if(InfoScreen[i].event.ev_ptr != NULL && InfoScreen[i].event.timeout != 0){

      if((millis() - InfoScreen[i].event.last_call > InfoScreen[i].event.timeout) || InfoScreen[i].event.last_call==0 ){
        InfoScreen[i].event.last_call = millis();
        InfoScreen[i].event.ev_ptr();
      }
      else if(InfoScreen[i].event.timeout == 0){ //If no timeout is defined, the function will be called without any delay
        InfoScreen[i].event.ev_ptr();
      }
    }
  }
}

static void displayTime(void){

  DateTime date;

  memset(&date, 0, sizeof date);

  //TODO: READ THE API ONLY ONCE TO ADJUST THE LOCALTIME.
  //THEN, EVERY SECOND THE TIME WILL BE INCREMENTED BY THE CPU.
  //CONNECT TO THE API PERIODICALLY TO AUTOMATICALLY ADJUST THE TIME.
  readTimeAPI(&date);
  sprintf(InfoScreen[5].text.label, "UTC: D:%s, T:%s", date.date, date.time);
  _drawLayoutLabel(InfoScreen[5].text);
}

static void _updateToolbar(void){
  //todo: implementation

}

//Icon blink test
static void blink(void){

  static int toggle = 0;

  InfoScreen[3].bmp.color = (toggle) ? Display_Color_Red : Display_Color_Green;
 _drawLayoutBMP(InfoScreen[3].bmp);
  toggle = !toggle;
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


