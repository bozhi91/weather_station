
#ifndef __EV_MANAGER_H__
#define __EV_MANAGER_H__
    
  #include <cstddef>

  extern void evLayout(void* layout, int size);
  extern void eventManager(void);

  typedef struct{
    void (*ev_ptr)(void);    //Function pointer to the event
    unsigned long timeout;   //Call the event every X milliseconds
    unsigned long last_call; //Ellapsed time since the last call of the event
  }Event;

#endif
