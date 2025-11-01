
#include "Adafruit_ST7796S_kbv.h"
#include "Layout.h"
#include "infoLayout.h"
#include "API.h"
#include "display.h"
#include "LayoutComponents.h"

#include <Adafruit_GFX.h>


/***************************************  LAYOUT DEFINITIONS  ***************************************/

//This array holds all the available layouts. Any new layout should be placed here.
//The layout view/controller definition goes to its corresponding .cpp file 
LayoutList myLayoutList[] = {
  {  _viewHomeLayout,    _ctrlHomeLayout    },
  {  _viewInfoLayout,    _ctrlInfoLayout    },
  {  _viewBootScrLayout, 0 },
};

static int currentLayoutId = 0;

/*
  Stores the ID of the current layout.
  Laods the view of the layout
*/
void setCurrentLayout(int layoutId){

  currentLayoutId = layoutId;
  myLayoutList[currentLayoutId].layoutView();
}

int getCurrentLayout(void){
  return currentLayoutId;
}

/**Call the controller of the current layout. 
  This will modify the values of the layout components, labels, icons, etc.
**/
void callLayoutController(void){
  myLayoutList[currentLayoutId].layoutCtrl();
}

//Layout manager. Loads layout(by a given template) to the screen
void loadLayout(LayoutTemplate* layout, int size, int clrScr){
  
  if(clrScr){
    displClrScreen();
  }

  //Read the GuiElements[] array list and display the elements one by one
  for(int i=0; i<size; i++){

    switch(layout[i].type){
  
      case TYPE_LABEL:
        _drawLayoutLabel(layout[i].text);
      break;

      case TYPE_RECTANGLE:
        _drawLayoutShape(layout[i].shape);
      break;

      case TYPE_BITMAP:
        _drawLayoutBMP(layout[i].bmp);
      break;
    }
  }
}

static char day[2];
void displayHomeLayout(void){

  //Display daily icon data. 
  //We use the same method and canvas memory for all the icons of hte week. Including the today's forecast icon.
  //We just change the icon parmeters and coordinates. Then we copy the canvas back to the screen.
  /*Current_weather current[5];
  memset(current, 0xff, sizeof(current));

  displayTimeDate();
  int d = atoi(day);

  //Load the weather data for the next 5 days including today's forecast
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

  displayIcon(*canvasInst, &current[0], 30, 150);
  displayIcon(*canvasInst, &current[1], 140 ,150);
  displayIcon(*canvasInst, &current[2], 250 ,150);
  displayIcon(*canvasInst, &current[3], 360 ,150);*/
}


