#include "Adafruit_ST7796S_kbv.h"
 
#include "Layout.h"
#include "infoLayout.h"
#include "API.h"
#include "display.h"
#include "LayoutComponents.h"

#include <Adafruit_GFX.h>

//Define the local functions used to draw the basic layout components
static void _drawLayoutLabel(TextLabel text_label);
static void _drawLayoutShape(Shape shape);
static void _drawLayoutBMP(Image bmp);


/***************************************  LAYOUT DEFINITIONS  ***************************************/

//This array holds all the available layouts. Any new layout should be placed here.
//The layout view/controller definition goes to its corresponding .cpp file 
LayoutList myLayoutList[] = {

    {  _viewHomeLayout, _controllHomeLayout  },
    {  _viewInfoLayout, _controllInfoLayout  }
};

static Adafruit_ST7796S_kbv* displInst = getDisplayInstance();
static GFXcanvas16* canvasInst         = getCanvas();
static int currentLayoutId             = 0;

/***************************************  LOCAL FUNCTIONS  ***************************************/

/*
  Stores the ID of the current layout.
  Laods the view of the layout
*/
void setCurrentLayout(int layoutId){

  currentLayoutId = layoutId;
  myLayoutList[currentLayoutId].layoutView();
}

/**Call the controller of the current layout. 
  This will modify the values of the layout components, labels, icons, etc.
**/
void callLayoutController(void){
  myLayoutList[currentLayoutId].layoutCtrl();
}

//Initialize the graphic components and the default layout
void layoutInitializer(void){

  //Get the display and canvas instance
  if(displInst == 0){
    displInst  = getDisplayInstance();
    canvasInst = getCanvas();
  }

  setCurrentLayout(LAYOUT_INFO);

  //displClrScreen();
  //Toolbar(icons_list);
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

/**** DISPLAY THE LAYOUT BASIC COMPONENTS: SHAPE, LABEL, BUTTOM, BITMAP, ETC... *****/
static void _drawLayoutShape(Shape shape){
  displInst->drawRoundRect(shape.pos_x, shape.pos_y,shape.end_x, shape.end_y, 5, shape.color);
}

static void _drawLayoutLabel(TextLabel item){

  static int len = 0;
  int resize = 3; //text resize

  //Delete the previous text before drawing the new one
  if(len != 0){
    int width  = len*(CHAR_W+1)*resize;
    int height = (CHAR_H+1)*resize; 
    
    displInst->fillRoundRect(item.pos_x, item.pos_y, width, height, 5, COLOR_BLACK);
  }

  len = strlen(item.label);
  printTextEx(item.label, resize, item.pos_x, item.pos_y, item.color);
}

static void _drawLayoutBMP(Image bmp){
  drawMonochromeBitmap(bmp.pos_x, bmp.pos_y, bmp.width, bmp.height, bmp.bitmap, sizeof(bmp.size), bmp.color, COLOR_BLACK);
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

  displayIcon(*canvasInst, &current[0], 30, 150);
  displayIcon(*canvasInst, &current[1], 140 ,150);
  displayIcon(*canvasInst, &current[2], 250 ,150);
  displayIcon(*canvasInst, &current[3], 360 ,150);
}


