 
#ifndef __LAYOUT_H__
#define __LAYOUT_H__
    
    #include <cstddef>
    #include "evManager.h"

    typedef struct __attribute__((packed)){

        unsigned char  pos_x;
        unsigned char  pos_y;
        char           size;
        unsigned short color;
        char     label[100];
        unsigned char  val_len;
    }TextLabel;

    typedef struct __attribute__((packed)){

        unsigned int pos_x;
        unsigned int pos_y;
        unsigned int end_x;
        unsigned int end_y;
        unsigned short color;
    }Shape;

    typedef struct __attribute__((packed)){

        unsigned int pos_x;
        unsigned int pos_y;
        unsigned int width;
        unsigned int height;
        unsigned char* bitmap;
        size_t size;
        unsigned short color;
    }Image;

    typedef struct __attribute__((packed)){

        unsigned int height;
        unsigned short bg_color;
        unsigned short frame_color;
        int list_size;
        Image icons_list[];
    }Toolbar;

  typedef enum{

        TYPE_LABEL,
        TYPE_BITMAP,
        TYPE_LINE,
        TYPE_RECTANGLE,
        TYPE_BUTTON,
    }ELEMENT_TYPES;

    typedef enum{

        LAB_VCC = 1,
        LAB_CURRENT,
        LAB_TRANSMITTER,
    }LABEL_ALIAS;

    //Defines the layout's template. A layout may have a different components of a different type
    typedef struct {

        ELEMENT_TYPES type;
        Event event;

        union {
            TextLabel text;
            Image     bmp;
            Shape     shape;
          //Button    button;
        };
    }LayoutTemplate;

    //A prototype for a funciton pointer pointing at the layout main function/constructor
    //typedef void (*layoutFunction) (void); --> DEPRECATED. DO NOT DELETE YET
    
    //List of layout indices. Used as aliases to access the diffent layouts
    typedef enum{
        LAYOUT_HOME,
        LAYOUT_INFO,
        LAYOUT_BOOT_SCREEN
    }LAYOUT_LIST;

    //Layout list template. Used to access the layout view and controller
    typedef struct{
        void (*layoutView)(void);
        void (*layoutCtrl)(void);
    }LayoutList;

    void displayMainLayout(void);
    void displayHomeLayout(void);

    void loadLayout(LayoutTemplate* layout, int size, int clrScr);
    void layoutInitializer(void);
    void callLayoutController(void);
    void setCurrentLayout(int layoutId);

#endif
