#include "Adafruit_ST7796S_kbv.h"
 
#include "Layout.h"
#include "API.h"
#include "display.h"
#include <Adafruit_GFX.h>

static Adafruit_ST7796S_kbv* displInst = getDisplayInstance();
static GFXcanvas16* canvasInst         =  getCanvas();

//Layout manager. Loads layout to the screen by given 'id'
void loadLayout(int id){
  
  if(displInst == 0){
    displInst  = getDisplayInstance();
    canvasInst = getCanvas();
  }

  if(id == 0){
    displInst->fillScreen(COLOR_BLACK);
    displayMainLayout();  //init display main layout
  }
  else if(id == 1){
    displayHomeLayout(); //init home layout
  }
}

/**
  Draw display frame, toolbar and other static components.
  This components are usually visible in all of the layouts
*/
void displayMainLayout(void){

  displInst->drawRoundRect(5, 5, DISPLAY_WIDTH-10, DISPLAY_HEIGHT-10, 5, Display_Color_Yellow);
  msgBox("INITIALIZING, PLEASE WAIT...", TYPE_OK);
}

static char day[2];
void displayHomeLayout(void){

  //Display daily icon data. 
  //We use the same method and canvas memory for all the icons of hte week. Including the today's forecast icon.
  //We just change the icon parmeters and coordinates. Then we copy the canvas back to the screen.
  Current_weather current[5];
  memset(current, 0xff, sizeof(current));

  displayTimeDate();
  int d = atoi(day);

  /** Load the weather data for the next 5 days including today's forecast **/
  sprintf(current[0].date, "%d", d);
  readWeatherAPI(&current[0]);

  d++;
  sprintf(current[1].date, "%d", d);
  readWeatherAPI(&current[1]);

  d++;
  sprintf(current[2].date, "%d", d);
  readWeatherAPI(&current[2]);

  d++;
  sprintf(current[3].date, "%d", d);
  readWeatherAPI(&current[3]);

  displayDailyIcon(*canvasInst, &current[0], 30, 150);
  displayDailyIcon(*canvasInst, &current[1], 140 ,150);
  displayDailyIcon(*canvasInst, &current[2], 250 ,150);
  displayDailyIcon(*canvasInst, &current[3], 360 ,150);
}

/***
  Display the time and date in format: dd/mm/YY - HH:mm:ss(12/24 hour format)
*/
void displayTimeDate(void){

  //char timme_buff[50];
  DateTime time = {0};

  memset(&time, 0, sizeof time);
  readTimeAPI(&time);
  
  msgBox("", TYPE_NONE);

  displInst->fillRect(40, 30, 340,80, COLOR_BLACK);
  printTextEx(time.time, 4, 40, 30, Display_Color_Yellow);
  printTextEx(time.date, 4, 40, 70, Display_Color_Yellow);
  
  Serial.printf("Date: %s:%s \n", time.date, time.time);


  day[0] = time.date[3];
  day[1] = time.date[4];
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

