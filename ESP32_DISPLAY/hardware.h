 
#ifndef __HARDWARE_MODULE__
#define __HARDWARE_MODULE__

    #define MOSI    23
    #define MISO    19
    #define CLK     18
    #define SD_CS   4  //Chip Select for the SD card is at GPIO_0

    #define TFT_CS    16
    #define TFT_DC    5
    #define TFT_RST   17 // RST can be set to -1 if you tie it to Arduino's reset

    void printCpuInfo(void);
    void halt(void);
    void bootScreen(void);
    void init_HAL(void);
    void toggleSPI_Device(int dev_id);

    typedef struct {
        int device_id;
        int CS;
    }SPI_Dev_List;

    typedef enum {
        LCD_DISPLAY,
        SD_CARD,
        TOUCH,
    }Device_List;

#endif
