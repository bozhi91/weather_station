
#include "API.h"
#include "forecastLayout.h"
#include "Layout.h"
#include "LayoutComponents.h"
#include "display.h"
#include "Wifi_Module.h"
#include "evManager.h"

  /*Image icons_list[] = {
    { 50,  20, 12,  11, bmp_warning, sizeof(bmp_warning), Display_Color_Green },
    { 90,  20, 15,  12, antena,      sizeof(antena),      Display_Color_Red   },
    { 130, 20, 14,  8,  battery,     sizeof(battery),     Display_Color_Blue  },
  };*/

static void displayTime(void);
static void blink(void);

LayoutTemplate BootScreen[] = {

  {
    .type  = TYPE_LABEL,
    .event = { 0,0 },
    .text  = { 20, 160, 3, Display_Color_White, "Starting up. Please wait...", 0 }
  },
};

void _viewBootScrLayout(void){

  size_t count = sizeof(BootScreen) / sizeof(BootScreen[0]);
  loadLayout(BootScreen, count, 1);
}


LayoutTemplate ForecastLayout[] = {

  {
    .type   = TYPE_WEATHER_WIDGED,
    .event  = { 0, 0 },
    .widged = { "Weekly forecast" ,10, 130,
                 DISPLAY_WIDTH-20, 170, Display_Color_Blue, Display_Color_Black, 0 }
  },
  {
    .type  = TYPE_LABEL,
    .event = { 0,0 },
    .text  = { 20, 40, 2, Display_Color_White, "Date/Time:", 0 }
  },
};

void _viewForecastLayout(void){

  size_t count = sizeof(ForecastLayout) / sizeof(ForecastLayout[0]);
  loadLayout(ForecastLayout, count, 1);

  //Get current time from an external server
  //readTimeAPI();

  //Display time as so: Wed 06 Oct | 13:33
  displayTime();

  //Get the weather condition from the Weather API and display the current weather
  Weather_Data forecast[5];

  memset(&forecast, 0, sizeof forecast);
  readWeatherAPI(forecast, 4);
  int x=30;

  for(int i=0;i<4;i++){
    displayWeatherIcon(&forecast[i], x, 150);
    x+=CANVAS_W+10;
  }
}

void _ctrlForecastLayout(void){

  size_t size = sizeof(ForecastLayout) / sizeof(ForecastLayout[0]);
  evLayout(ForecastLayout, size);
}


static void displayTime(void){

  DateTime date;

  memset(&date, 0, sizeof date);

  readTimeAPI(&date);
  sprintf(ForecastLayout[1].text.label, "Date/Time: %s | %s", date.date, date.time);
  _drawLayoutLabel(ForecastLayout[1].text);
}


static void _updateToolbar(void){
  //todo: implementation
}

//Icon blink test
static void blink(void){

  /*static int toggle = 0;
  int status = getConnStatus();

  if(status!=2){
    InfoScreen[3].bmp.color = (toggle) ? Display_Color_Red : Display_Color_Green;
    _drawLayoutBMP(InfoScreen[3].bmp);
    toggle = !toggle;
  }
  else{
    InfoScreen[3].bmp.color = Display_Color_Green;
    _drawLayoutBMP(InfoScreen[3].bmp);
  }*/
}

//Display the layout components on screen
void _viewHomeLayout(void){

  /*size_t count = sizeof(HomeScreen) / sizeof(HomeScreen[0]);
  loadLayout(HomeScreen, count, 1);*/
}

//Modify the layout components values
void _ctrlHomeLayout(void){

  /*unsigned long tim = millis();
  sprintf(HomeScreen[0].text.label, "Uptime: %ld ms", tim);
  loadLayout(&HomeScreen[0], 1, 0);
*/
  //msgBox(str, TYPE_OK);
}

    /*HomeScreen[0].text  = { 100, 160,  Display_Color_Blue, "TEST LABEL 1 - HELLO", 0 };
    HomeScreen[1].text  = { 100, 100, Display_Color_Red,  "TEST LABEL 2 - WORLD", 0 };
    HomeScreen[2].shape = { 90,  90,  380, 50, Display_Color_Green   };
    HomeScreen[3].shape = { 90,  150, 380, 50, Display_Color_Yellow  };*/


