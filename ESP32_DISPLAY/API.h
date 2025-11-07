 
#ifndef __API__
#define __API__

    /**
        Weather conditions

        1 - sunny : 2,26
        2 - partially cloudy : 3,4,5,6,8,27,28,29,30,31
        3 - cloudy : 7
        4 - rain : 10,11,12
        5 - sun+rain : 13,32
        6 - storm: 14,15,33
        7 - snow  :16-24,34,35
        8 - hailstorm: 25
        9 - fog : 9

        at nignt:
        clear sky: 26
     */

    typedef struct {
        char ssid[30];
        char pass[30];
        int refresh_time;
        char lang[2];
        int timezone;  //currnet timezone shift relavive to the UTC. (+/-)1,2,3
        unsigned char time_format;
        unsigned char date_format;
        unsigned char temp_units;
    }Config;

    typedef struct{
        int max_temp;
        int min_temp;
        int icon_id;
        char summary[100];
        char date[12];
    }Weather_Data;

    typedef struct {
        char date[12];
        char time[12];
    } DateTime; 

    void readWeatherAPI(Weather_Data* forecast, int size);
    void readTimeAPI(DateTime* date);
    
    int loadDeviceConfig(void);
    void getDeviceConfig(Config* conf);
    void shell(void);
    

    /** ALTERNATIVE APIs

        LCD Documentation: https://www.lcdwiki.com/4.0inch_SPI_Module_ST7796
        Icons: https://www.iconfinder.com/weather-icons?price=free
        rgb565 color picker: https://rgbcolorpicker.com/565

        === WEATHER APIs ===

        https://open-meteo.com/en/docs
        https://www.weatherapi.com/docs/
        condition codes: https://www.weatherapi.com/docs/conditions.json
        https://api.weatherapi.com/v1/forecast.json?key=86a605c5ebbd4c40a09135726210410&q=Barcelona&lang=en&dt=2025-06-14
            const char* API_KEY = "milkulGrUGbG3q0idvGoMape8x0c9ARp"; //ACUWEATHER API KEY

        === TIME APIs ===

        https://timeapi.io/api/time/current/zone?timeZone=Europe%2FMadrid
    */

#endif
