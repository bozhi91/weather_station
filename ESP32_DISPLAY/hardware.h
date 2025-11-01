 
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

#endif
