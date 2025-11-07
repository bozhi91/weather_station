
#include "evManager.h"
#include "API.h"
#include "forecastLayout.h"
#include "Layout.h"
#include "LayoutComponents.h"
#include "display.h"
#include "Wifi_Module.h"

static void _evTest(void);
static void _evTest_2(void);

Event eventList[] = {
  //{ _evTest,   1000 },
  //{ _evTest_2, 2000 },
};

static void _evTest(void){
  Serial.println(" Custom event running...");
}
static void _evTest_2(void){
  Serial.println(" Custom event_2 running...");
}

//Executes the event function defined in the layout structure template
void evLayout(void* layout, int size){

  LayoutTemplate* lay = (LayoutTemplate*)layout;

  //Call certain functions periodically in order to refresh the displayed data
  for(int i=0; i<size; i++){

    if(lay[i].event.ev_ptr != NULL && lay[i].event.timeout != 0){

      if((millis() - lay[i].event.last_call > lay[i].event.timeout) || lay[i].event.last_call==0 ){
        lay[i].event.last_call = millis();
        lay[i].event.ev_ptr();
      }
      else if(lay[i].event.timeout == 0){ //If no timeout is defined, the function will be called without any delay
        lay[i].event.ev_ptr();
      }
    }
  }
}

/**
  The event manager will run global/generic events which are responsible of
  controlling system processes, peripheral devices and others. 
*/
void eventManager(void){

  int size = sizeof(eventList)/sizeof(eventList[0]);

  //Call certain functions periodically in order to refresh the displayed data
  for(int i=0; i<size; i++){

    if(eventList[i].ev_ptr != NULL && eventList[i].timeout != 0){

      if((millis() - eventList[i].last_call > eventList[i].timeout) || eventList[i].last_call==0 ){
        eventList[i].last_call = millis();
        eventList[i].ev_ptr();
      }
      else if(eventList[i].timeout == 0){ //If no timeout is defined, the function will be called without any delay
        eventList[i].ev_ptr();
      }
    }
  }  
}

