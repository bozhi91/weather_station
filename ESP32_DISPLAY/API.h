 
#ifndef __API__
#define __API__

  
    typedef struct {
        char ssid[30];
        char pass[30];
        int refresh_time;
        char lang[2];
        char timezone[20];
        int time_format;
        char temp_units;
    }Config;

    typedef struct{
        int max_temp;
        int min_temp;
        char condition[10];
        int cond_id;
        char date[6];
    }Current_weather;

    void readWeatherAPI(Current_weather* current);
    void readTimeAPI(char* result);
    
    int loadConfig(void);
    void getConfig(Config* conf);

#endif
