 
#ifndef __LAYOUT_COMPONENTS_H__
#define __LAYOUT_COMPONENTS_H__

    #include "display.h"
    #include "Layout.h"

    //Image functions
    void drawMonochromeBitmap(unsigned char startX, unsigned char  startY, unsigned char width, unsigned char  height,
                              const unsigned char* bitmap, int nBytes, unsigned short color,unsigned short bg_color);

    void drawColorBitmap(const unsigned short image[],int posX, int posY,int sizeX, int sizeY);
    int loadPNG( GFXcanvas16* canvas_id, const char* fileName, int at_x, int at_y);
    void rgb888_to_rgb565(uint16_t* dest, const uint8_t* src, size_t pixel_count);


    //Shapes functions
    void drawRGBLine(const unsigned short buffer[], int start_X, int start_Y, unsigned int nBytes);
    void drawColorBitmap(const unsigned short image[], int posX, int posY, int sizeX, int sizeY);
    void drawRGBLine(const unsigned short buffer[], int start_X, int start_Y, unsigned int nBytes);
    void display_FillRect(int x, int y, int w, int h, unsigned short color);


    //Text funcitons
    void printText(String text, int pos_x, int pos_y);
    void printTextEx(String text, int size, int pos_x, int pos_y, unsigned short color);
    void printTextCanvas(GFXcanvas16* canvas_id, String text, int pos_x, int pos_y, unsigned short color);

    //canvas funcitons
    void drawToCanvas( GFXcanvas16* canvas_id, int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
    void canvasToScreen( GFXcanvas16* canvas_id, int at_x, int at_y);


    //GUI Functions
    void msgBox(const char* text, MSG_BOX_TYPES type);


    void displayWeatherIcon(Weather_Data* forecast, int x, int y);
    void displayProgressBar(int percent);
    void displayTimeDate(void);
    void displayToolbar(Image iconList[]);

    //Basic layout components
    void _drawLayoutBMP(Image bmp);
    void _drawLayoutShape(Shape shape);
    void _drawLayoutLabel(TextLabel item);
    void _drawWeatherWidged(WeatherWidged widged);

#endif
